//
//  AppFireWall.c
//  AppFireWall
//
//  Created by 咲衣憧 on 15/6/30.
//  Copyright (c) 2015年 Kingfree. All rights reserved.
//

#include <mach/mach_types.h>
#include <mach/vm_types.h>
#include <mach/kmod.h>
#include <sys/kpi_mbuf.h>
#include <sys/kpi_socket.h>
#include <sys/kpi_socketfilter.h>
#include <sys/mbuf.h>
#include <sys/proc.h>
#include <sys/socket.h>
#include <sys/systm.h>
#include <netinet/in.h>
#include <kern/assert.h>
#include <kern/debug.h>
#include <kern/locks.h>
#include <kern/task.h>
#include <libkern/OSAtomic.h>
#include <libkern/OSMalloc.h>
#include <sys/kernel_types.h>
#include <sys/kern_control.h>
#include <netinet/ip.h>
#include "socks5.h"

kern_return_t AppFireWall_start(kmod_info_t* ki, void* d);
kern_return_t AppFireWall_stop(kmod_info_t* ki, void* d);

#define APPWALL_FLT_TCP_HANDLE 'apw0'
#define BUNDLE_ID "kingfree.AppFireWall"

#define BUFSIZE 2048

#define PROXY_ADDR 0x7f000001
#define PROXY_PORT 7890 // 127.0.0.1:7890
#define PROXY_USER "123456"
#define PROXY_PASS "123456"

struct app_descriptor {
    char name[PATH_MAX];
    unsigned long bytes_in;
    unsigned long bytes_out;
    unsigned long packets_in;
    unsigned long packets_out;
    int do_block;
    int do_forward;
    int outbound_blocked;
    int inbound_blocked;
};

struct appwall_entry {
    TAILQ_ENTRY(appwall_entry) link;
    struct app_descriptor desc;
    int users;
};

static lck_mtx_t* g_mutex = NULL;
static lck_grp_t* g_mutex_group = NULL;

static boolean_t g_proxy_registered = FALSE;
static boolean_t g_proxy_unregister_started = FALSE;
static OSMallocTag g_osm_tag;

TAILQ_HEAD(appwall_entry_list, appwall_entry);
static struct appwall_entry_list g_block_list;

static void sf_println(const char* fmt, ...)
{
    va_list p;
    char buf[128];
    
    va_start(p, fmt);
    
    vsnprintf(buf, sizeof(buf), fmt, p);
    printf(BUNDLE_ID ": %s\n", buf);
    
    va_end(p);
}

static void log_ip_and_port_addr(struct sockaddr_in* addr)
{
    unsigned char addstr[256];
    inet_ntop(AF_INET, &addr->sin_addr, (char*)addstr, sizeof(addstr));
    printf("%s:%d", addstr, ntohs(addr->sin_port));
}

static struct appwall_entry* find_entry_by_name(const char* name)
{
    struct appwall_entry *entry, *next_entry;

    for (entry = TAILQ_FIRST(&g_block_list); entry; entry = next_entry) {
        next_entry = TAILQ_NEXT(entry, link);
        if (strcmp(name, entry->desc.name) == 0)
            return entry;
    }
    return NULL;
}

static void appwall_unregistered(sflt_handle handle)
{
    sf_println("注销");
    g_proxy_registered = FALSE;
}

static errno_t appwall_attach(void** cookie, socket_t so)
{
    sf_println("appwall_attach");
    errno_t result = 0;
    struct appwall_entry* entry;
    char name[PATH_MAX];

    *cookie = NULL;

    proc_selfname(name, PATH_MAX);

    lck_mtx_lock(g_mutex);

    entry = find_entry_by_name(name);
    if (entry) {
        entry->users++;
        *cookie = (void*)entry;
        sf_println("装载到进程: %s", name);
    }
    else {
        result = ENOPOLICY; // 不要装载到这个套接字
    }
    
//    if (entry->desc.do_forward) {
//        sf_println("需要转发 %s", entry->desc.name);
//        // 
//    }

    lck_mtx_unlock(g_mutex);

    return result;
}

static void appwall_detach(void* cookie, socket_t so)
{
    sf_println("appwall_detach");
    struct appwall_entry* entry;

    if (cookie) {
        entry = (struct appwall_entry*)cookie;

        lck_mtx_lock(g_mutex);

        entry->users--;
        if (entry->users == 0) {
            if (entry->desc.name[0]) {
                sf_println("进程报告: %s", entry->desc.name);
            }

            if (entry->desc.do_block) {
                sf_println("     拦截输入流量: %d", entry->desc.inbound_blocked);
                sf_println("     拦截输出流量: %d", entry->desc.outbound_blocked);
            }
            else {
                sf_println("     输入字节: %lu", entry->desc.bytes_in);
                sf_println("     输出字节: %lu", entry->desc.bytes_out);
                sf_println("     输入数据包: %lu", entry->desc.packets_in);
                sf_println("     输出数据包: %lu", entry->desc.packets_out);
            }

            cookie = NULL;
        }

        lck_mtx_unlock(g_mutex);
    }
    return;
}

static errno_t appwall_data_in(void* cookie, socket_t so, const struct sockaddr* from, mbuf_t* data, mbuf_t* control, sflt_data_flag_t flags)
{
    sf_println("appwall_data_in");
    struct appwall_entry* entry;
    errno_t result = 0;

    if (!(entry = (struct appwall_entry*)cookie)) {
        goto failed;
    }

    lck_mtx_lock(g_mutex);

    size_t len = mbuf_pkthdr_len(*data);
    u_int8_t *datas = mbuf_data(*data);
    for (int i = 0; i < len; i++) {
        printf("%02x ", datas[i]);
    }
    printf("\n");
    sf_println(" 检测到 %s 的来路连接 大小: %lu", entry->desc.name, len);
    if (from) {
        printf(" 来自: ");
        log_ip_and_port_addr((struct sockaddr_in*)from);
    }
    printf("\n");
   

    entry->desc.bytes_in += len;
    entry->desc.packets_in++;
    
    if (entry->desc.do_block)
        result = EPERM;

    lck_mtx_unlock(g_mutex);

failed:
    return result;
}

void appwall_sock_upcall(socket_t so, void* cookie, int waitf)
{

}

static mbuf_tag_id_t appwall_id;
static mbuf_tag_type_t appwall_type;

static errno_t appwall_data_out(void* cookie, socket_t so, const struct sockaddr* to, mbuf_t* data, mbuf_t* control, sflt_data_flag_t flags)
{
    sf_println("appwall_data_out");
    struct appwall_entry* entry;
    errno_t result = 0;
    size_t len;

    if (!(entry = (struct appwall_entry*)cookie)) {
        sf_println("no cookie");
        goto failed;
    }

    int* tag_ref;
    errno_t status = mbuf_tag_find(*data, appwall_id, appwall_type, &len, (void**)&tag_ref);
    if (status == 0 && *tag_ref == 4 && len == sizeof(int)) {
        sf_println("获取到标记");
//        return 0;
    }
    
    sf_println("getting mutex");
    lck_mtx_lock(g_mutex);
    sf_println("get mutex");

    len = mbuf_pkthdr_len(*data);
    u_int8_t *datas = mbuf_data(*data);
    for (int i = 0; i < len; i++) {
        printf("%02x ", datas[i]);
    }
    printf("\n");

    struct sockaddr sin, sout;
    sock_getsockname(so, &sin, sizeof(sin));
    sock_getpeername(so, &sout, sizeof(sout));

    printf("应用防火墙: 检测来自 %s 的出路连接 大小: %lu", entry->desc.name, len);
    printf(" ");
    log_ip_and_port_addr((struct sockaddr_in*)&sin);
    printf(" - ");
    log_ip_and_port_addr((struct sockaddr_in*)&sout);
    if (to) {
        printf(" 到: ");
        log_ip_and_port_addr((struct sockaddr_in*)to);
    }
    printf("\n");
    entry->desc.bytes_out += len;
    entry->desc.packets_out++;

    if (entry->desc.do_block)
        result = EPERM;
    
    if (entry->desc.do_forward) {
        sf_println("do_forward");
    }

    lck_mtx_unlock(g_mutex);

failed:
    return result;
}

static errno_t appwall_connect_in(void* cookie, socket_t so, const struct sockaddr* from)
{
    sf_println("appwall_connect_in");
    struct appwall_entry* entry;
    errno_t result = 0;

    if (!(entry = (struct appwall_entry*)cookie)) {
        goto failed;
    }

    lck_mtx_lock(g_mutex);

    if (entry->desc.do_block) {
        printf(" 拦截到 %s 的来路连接", entry->desc.name);
        if (from) {
            printf(" 来自: ");
            log_ip_and_port_addr((struct sockaddr_in*)from);
        }
        printf("\n");
        entry->desc.inbound_blocked++;
        result = EPERM;
    }

    lck_mtx_unlock(g_mutex);

failed:
    return result;
}

static errno_t appwall_connect_out(void* cookie, socket_t so, const struct sockaddr* to)
{
    sf_println("appwall_connect_out");
    struct appwall_entry* entry;
    errno_t result = 0;

    if (!(entry = (struct appwall_entry*)cookie)) {
        goto failed;
    }

    lck_mtx_lock(g_mutex);

    if (entry->desc.do_block) {
        printf(" 拦截来自 %s 的出路连接", entry->desc.name);
        if (to) {
            printf(" 到: ");
            log_ip_and_port_addr((struct sockaddr_in*)to);
        }
        printf("\n");
        entry->desc.outbound_blocked++;
        result = EPERM;
    }

    /*
    if (entry->desc.do_forward) {
        printf(" 转发来自 %s 的出路连接", entry->desc.name);
        struct sockaddr_in* addr = (struct sockaddr_in*)to;
        // inet_pton(PF_INET, "127.0.0.1", &addr->sin_addr.s_addr);
        // inet_pton 在内核中无法使用
        addr->sin_addr.s_addr = htonl(PROXY_ADDR);
        addr->sin_port = htons(PROXY_PORT);
        printf("到地址 ");
        log_ip_and_port_addr(addr);
        printf("\n");
    } */

    lck_mtx_unlock(g_mutex);

failed:
    return result;
}

static char* event_t_string[] = {
    "",
    "sock_evt_connecting",
    "sock_evt_connected",
    "sock_evt_disconnecting",
    "sock_evt_disconnected",
    "sock_evt_flush_read",
    "sock_evt_shutdown",
    "sock_evt_cantrecvmore",
    "sock_evt_cantsendmore",
    "sock_evt_closing",
    "sock_evt_bound"
};

static errno_t prepend_mbuf_hdr(mbuf_t* data, size_t pkt_len)
{
    mbuf_t new_hdr;
    errno_t status;
    
    status = mbuf_gethdr(MBUF_WAITOK, MBUF_TYPE_DATA, &new_hdr);
    if (status == 0) {
        mbuf_setnext(new_hdr, *data);
        
        mbuf_setnextpkt(new_hdr, mbuf_nextpkt(*data));
        mbuf_pkthdr_setlen(new_hdr, pkt_len);
        mbuf_setlen(new_hdr, 0);
        
        mbuf_pkthdr_setrcvif(*data, NULL);
        
        *data = new_hdr;
    }
    return status;
}

void appwall_notify_func(void* cookie, socket_t so, sflt_event_t event, void* param)
{
    sf_println("appwall_notify_func");
    struct appwall_entry* entry;

    if (!(entry = (struct appwall_entry*)cookie)) {
        goto finally;
    }

    lck_mtx_lock(g_mutex);

    /* struct sockaddr name;
    sf_println(" %s 状态 %s ", entry->desc.name, event_t_string[event]);
    sock_getsockname(so, &name, sizeof(name));
    log_ip_and_port_addr((struct sockaddr_in*)&name);
    printf(" - ");
    sock_getpeername(so, &name, sizeof(name));
    log_ip_and_port_addr((struct sockaddr_in*)&name);
    printf("\n");
    
    if (entry->desc.do_forward && event == sock_evt_connected) {
        mbuf_t data;
        char buf[256] = {0}, *p;
        size_t len;
        unsigned int chunks = 0;
        
        struct sockaddr sin, sout;
        sock_getsockname(so, &sin, sizeof(sin));
        sock_getpeername(so, &sout, sizeof(sout));
        
        sf_println("入口: ");
        log_ip_and_port_addr((struct sockaddr_in*)&sin);
        sf_println("出口: ");
        log_ip_and_port_addr((struct sockaddr_in*)&sout);
        sf_println(" 地址");
        
        if (mbuf_allocpacket(MBUF_DONTWAIT, 256, &chunks, &data) != 0) {
            sf_println("mbuf_allocpacket() 失败");
            goto failed;
        }
        sf_println("mbuf_allocpacket()");
        
        p = buf;
        *p++ = 0x05; // VER
        *p++ = 0x01; // NMETHODS
        *p++ = 0x02; // METHODS[1]
        len = p - buf;
        
        if (mbuf_copyback(data, 0, len, buf, MBUF_DONTWAIT) != 0) {
            sf_println("mbuf_copyback() 失败");
            goto failed;
        }
        sf_println("mbuf_copyback()");
        
        int* tag_ref = NULL;
        int value = 4;
        errno_t status = mbuf_tag_allocate(data, appwall_id, appwall_type, sizeof(value), MBUF_WAITOK, (void**)&tag_ref);
        if (status == 0) {
            *tag_ref = value;
            sf_println("已打上标记");
        } else if (status == EINVAL) {
            mbuf_flags_t flags;
            flags = mbuf_flags(data);
            if ((flags & MBUF_PKTHDR) == 0) {
                mbuf_t m = data;
                size_t totalbytes = 0;
                sf_println("mbuf_t 未设置 MBUF_PKTHDR 位");
                
                while (m) {
                    totalbytes += mbuf_len(m);
                    m = mbuf_next(m);
                }
                status = prepend_mbuf_hdr(&data, totalbytes);
                if (status == KERN_SUCCESS) {
                    status = mbuf_tag_allocate(data, appwall_id, appwall_type, sizeof(value), MBUF_WAITOK, (void**)&tag_ref);
                    if (status) {
                        sf_println("mbuf_tag_allocate 再次失败 返回 %d", status);
                    }
                }
            }
        }
        
        if (sock_sendmbuf(so, NULL, data, 0, &len) != 0) {
//        if (sock_send(so, buf, 0, &len) != 0) {
            sf_println("sock_sendmbuf() 失败");
            goto failed;
        }
        sf_println("sock_sendmbuf(%uz)", len);

        if (sock_receivembuf(so, NULL, &data, MSG_DONTWAIT, &len) != 0) {
            sf_println("sock_receivembuf() 失败");
            goto failed;
        }
        sf_println("sock_receivembuf() %zu", len);
        
        if (buf[0] != 0x05 || buf[1] != 0x02) { // VER | METHOD
            sf_println("不可认证");
            goto failed;
        }
        sf_println("协商成功");

        p = buf;
        *p++ = 0x01; // VER
        
        len = strlen(PROXY_USER);
        *p++ = len; // ULEN
        strncpy((char*)p, PROXY_USER, len); // UNAME
        p += len;
        
        len = strlen(PROXY_PASS);
        *p++ = len; // PLEN
        strncpy((char*)p, PROXY_PASS, len); // PASSWD
        p += len;
        
        len = p - buf;
        
        if (mbuf_copyback(data, 0, len, buf, MBUF_DONTWAIT) != 0) {
            sf_println("mbuf_setdata() 失败");
            goto failed;
        }
        sf_println("mbuf_setdata()");

        if (sock_sendmbuf(so, NULL, data, MSG_DONTWAIT, &len) != 0) {
            sf_println("sock_sendmbuf() 失败");
            goto failed;
        }
        sf_println("sock_sendmbuf() %zu", len);

        if (sock_receivembuf(so, NULL, &data, MSG_DONTWAIT, &len) != 0) {
            sf_println("sock_receivembuf() 失败");
            goto failed;
        }
        sf_println("sock_receivembuf() %zu", len);
        
        if (buf[0] != 0x01 || buf[1] != 0x00) { // VER | STATUS
            sf_println("认证失败");
            goto failed;
        }
        sf_println("认证成功");
        
        

    failed:
        mbuf_free(data);
    }
*/

finally:

    lck_mtx_unlock(g_mutex);
    
}

static struct sflt_filter socket_tcp_filter = {
    APPWALL_FLT_TCP_HANDLE,
    SFLT_GLOBAL,
    BUNDLE_ID,
    appwall_unregistered,
    appwall_attach,
    appwall_detach,
    appwall_notify_func,
    NULL,
    NULL,
    appwall_data_in,
    appwall_data_out,
    appwall_connect_in,
    appwall_connect_out,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

static int add_entry(const char* app_name, int block)
{
    struct appwall_entry* entry = NULL;

    lck_mtx_lock(g_mutex);

    entry = find_entry_by_name(app_name);
    if (entry) {
        entry->desc.do_block = block;
    }
    else {
        entry = OSMalloc(sizeof(struct appwall_entry), g_osm_tag);
        if (!entry) {
            lck_mtx_unlock(g_mutex);
            return ENOMEM;
        }

        strncpy(entry->desc.name, app_name, PATH_MAX);
        entry->desc.bytes_in = 0;
        entry->desc.bytes_out = 0;
        entry->desc.inbound_blocked = 0;
        entry->desc.outbound_blocked = 0;
        entry->desc.packets_in = 0;
        entry->desc.packets_out = 0;
        entry->desc.do_block = block == 1 ? 1 : 0;
        entry->desc.do_forward = block == 2 ? 1 : 0;
        entry->users = 0;

        TAILQ_INSERT_TAIL(&g_block_list, entry, link);
    }

    lck_mtx_unlock(g_mutex);

    return 0;
}

kern_return_t AppFireWall_start(kmod_info_t* ki, void* d)
{
    TAILQ_INIT(&g_block_list);

    if (!(g_osm_tag = OSMalloc_Tagalloc(BUNDLE_ID, OSMT_DEFAULT))) {
        sf_println("初始化内存标记失败");
        goto failed;
    }

    if (!(g_mutex_group = lck_grp_alloc_init(BUNDLE_ID, LCK_GRP_ATTR_NULL))) {
        sf_println("初始化锁组失败");
        goto failed;
    }

    if (!(g_mutex = lck_mtx_alloc_init(g_mutex_group, LCK_ATTR_NULL))) {
        sf_println("初始化锁失败");
        goto failed;
    }
    
    if (mbuf_tag_id_find(BUNDLE_ID, &appwall_id) != 0) {
        sf_println("mbuf_tag_id_find 失败");
        goto failed;
    }

    if (sflt_register(&socket_tcp_filter, PF_INET, SOCK_STREAM, IPPROTO_TCP) != KERN_SUCCESS) {
        sf_println("注册套接字过滤器失败");
        goto failed;
    }
    
    add_entry("QQ", 0); // 记录所有QQ请求
    // add_entry("Thunder", 1); // 拦截所有迅雷请求
    add_entry("nc", 2); // 转发所有nc请求
    
    g_proxy_registered = TRUE;

    return KERN_SUCCESS;

failed:

    if (g_mutex) {
        lck_mtx_free(g_mutex, g_mutex_group);
        g_mutex = NULL;
    }

    if (g_mutex_group) {
        lck_grp_free(g_mutex_group);
        g_mutex_group = NULL;
    }

    if (g_osm_tag) {
        OSMalloc_Tagfree(g_osm_tag);
        g_osm_tag = NULL;
    }

    return KERN_FAILURE;
}

kern_return_t AppFireWall_stop(kmod_info_t* ki, void* d)
{
    struct appwall_entry *entry, *next_entry;

    if (g_proxy_registered == TRUE && !g_proxy_unregister_started) {
        sflt_unregister(APPWALL_FLT_TCP_HANDLE);
        g_proxy_unregister_started = TRUE;
    }
    /* 如果注销失败 */
    if (g_proxy_registered == TRUE) {
        printf("应用防火墙: 正在忙，无法卸载\n");
        return EBUSY;
    }

    lck_mtx_lock(g_mutex);

    /* 清理 */
    for (entry = TAILQ_FIRST(&g_block_list); entry; entry = next_entry) {
        next_entry = TAILQ_NEXT(entry, link);
        TAILQ_REMOVE(&g_block_list, entry, link);
        OSFree(entry, sizeof(struct appwall_entry), g_osm_tag);
    }

    lck_mtx_unlock(g_mutex);

    lck_mtx_free(g_mutex, g_mutex_group);
    lck_grp_free(g_mutex_group);
    g_mutex = NULL;
    g_mutex_group = NULL;

    OSMalloc_Tagfree(g_osm_tag);
    g_osm_tag = NULL;

    return KERN_SUCCESS;
}
