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
#include <sys/socket.h>
#include <sys/kpi_socket.h>
#include <sys/kpi_mbuf.h>
#include <sys/kpi_socket.h>
#include <sys/kpi_socketfilter.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/mbuf.h>

#include <sys/proc.h>

#include <netinet/in.h>
#include <kern/task.h>
#include <kern/locks.h>
#include <kern/assert.h>
#include <kern/debug.h>

#include <libkern/OSMalloc.h>

#include <sys/kern_control.h>

kern_return_t AppFireWall_start(kmod_info_t* ki, void* d);
kern_return_t AppFireWall_stop(kmod_info_t* ki, void* d);

#define APPWALL_FLT_TCP_HANDLE 'apw0'
#define BUNDLE_ID "kingfree.AppFireWall"

struct app_descriptor {
    char name[PATH_MAX];
    unsigned long bytes_in;
    unsigned long bytes_out;
    unsigned long packets_in;
    unsigned long packets_out;
    int do_block;
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

static boolean_t g_filter_registered = FALSE;
static boolean_t g_filter_unregister_started = FALSE;
static OSMallocTag g_osm_tag;

TAILQ_HEAD(appwall_entry_list, appwall_entry);
static struct appwall_entry_list g_block_list;

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
    printf("应用防火墙: 注销\n");
    g_filter_registered = FALSE;
}

static errno_t appwall_attach(void** cookie, socket_t so)
{
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
        printf("应用防火墙: 装载到进程: %s\n", name);
    }
    else {
        result = ENOPOLICY; // 不要装载到这个套接字
    }
    
    lck_mtx_unlock(g_mutex);
    
    return result;
}

static void appwall_detach(void* cookie, socket_t so)
{
    struct appwall_entry* entry;
    
    if (cookie) {
        entry = (struct appwall_entry*)cookie;
        
        lck_mtx_lock(g_mutex);
        
        entry->users--;
        if (entry->users == 0) {
            if (entry->desc.name[0]) {
                printf("应用防火墙: 进程报告: %s\n", entry->desc.name);
            }
            
            if (entry->desc.do_block) {
                printf("应用防火墙:     拦截输入流量: %d\n", entry->desc.inbound_blocked);
                printf("应用防火墙:     拦截输出流量: %d\n", entry->desc.outbound_blocked);
            }
            else {
                printf("应用防火墙:     输入字节: %lu\n", entry->desc.bytes_in);
                printf("应用防火墙:     输出字节: %lu\n", entry->desc.bytes_out);
                printf("应用防火墙:     输入数据包: %lu\n", entry->desc.packets_in);
                printf("应用防火墙:     输出数据包: %lu\n", entry->desc.packets_out);
            }
            
            cookie = NULL;
        }
        
        lck_mtx_unlock(g_mutex);
    }
    return;
}

static errno_t appwall_data_in(void* cookie, socket_t so, const struct sockaddr* from, mbuf_t* data, mbuf_t* control, sflt_data_flag_t flags)
{
    struct appwall_entry* entry;
    errno_t result = 0;
    
    if (!(entry = (struct appwall_entry*)cookie)) {
        goto failed;
    }
    
    lck_mtx_lock(g_mutex);
    
    size_t len = mbuf_pkthdr_len(*data);
    printf("应用防火墙: 检测到 %s 的来路连接 大小: %lu", entry->desc.name, len);
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

void appwall_sock_upcall(socket_t so, void *cookie, int waitf)
{
//    struct sockaddr sin, sout;
//    sock_getsockname(so, &sin, sizeof(sin));
//    sock_getpeername(so, &sout, sizeof(sout));
//    printf("应用防火墙: ");
//    log_ip_and_port_addr((struct sockaddr_in*)&sin);
//    printf(" - ");
//    log_ip_and_port_addr((struct sockaddr_in*)&sout);
//    printf(" ... (%d)\n", waitf);
}

static errno_t appwall_data_out(void* cookie, socket_t so, const struct sockaddr* to, mbuf_t* data, mbuf_t* control, sflt_data_flag_t flags)
{
    struct appwall_entry* entry;
    errno_t result = 0;
    
    if (!(entry = (struct appwall_entry*)cookie)) {
        goto failed;
    }
    
    lck_mtx_lock(g_mutex);
    
    size_t len = mbuf_pkthdr_len(*data);
//    mbuf_t* newdata;
//    mbuf_allocpacket(MBUF_DONTWAIT, mbuf_maxlen(*data), &len, newdata);
//    mbuf_copydata(*data, 0, mbuf_len(*data), newdata);

    struct sockaddr sin, sout;
    sock_getsockname(so, &sin, sizeof(sin));
    sock_getpeername(so, &sout, sizeof(sout));

//    socket_t newso;
//    printf("应用防火墙: 尝试建立套接字: ");
//    log_ip_and_port_addr((struct sockaddr_in*)&sin);
//    printf(" - ");
//    log_ip_and_port_addr((struct sockaddr_in*)&sout);
//    printf(" ...\n");
//    if (sock_socket(PF_INET, SOCK_STREAM, IPPROTO_TCP, appwall_sock_upcall, cookie, &newso) == 0) {
//        printf("已建立套接字..");
//        if (sock_connect(newso, &sout, MSG_DONTWAIT) == 0) {
//            printf("已连接套接字..");
//            if (sock_sendmbuf(newso, mbuf_pkthdr_header(*data), *data, MSG_DONTROUTE, &len) == 0) {
//                printf("已传送mbuf..");
//            }
//        }
//        sock_close(newso);
//        printf("已关闭套接字\n");
//    }

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
    
    lck_mtx_unlock(g_mutex);
    
failed:
    return result;
}

static errno_t appwall_connect_in(void* cookie, socket_t so, const struct sockaddr* from)
{
    struct appwall_entry* entry;
    errno_t result = 0;
    
    if (!(entry = (struct appwall_entry*)cookie)) {
        goto failed;
    }
    
    lck_mtx_lock(g_mutex);
    
    if (entry->desc.do_block) {
        printf("应用防火墙: 拦截到 %s 的来路连接", entry->desc.name);
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
    struct appwall_entry* entry;
    errno_t result = 0;
    
    if (!(entry = (struct appwall_entry*)cookie)) {
        goto failed;
    }
    
    lck_mtx_lock(g_mutex);
    
    if (entry->desc.do_block) {
        printf("应用防火墙: 拦截来自 %s 的出路连接", entry->desc.name);
        if (to) {
            printf(" 到: ");
            log_ip_and_port_addr((struct sockaddr_in*)to);
        }
        printf("\n");
        entry->desc.outbound_blocked++;
        result = EPERM;
    }
    
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

void appwall_notify_func(void *cookie, socket_t so, sflt_event_t event, void *param)
{
    struct appwall_entry* entry;
    
    if (!(entry = (struct appwall_entry*)cookie)) {
        goto failed;
    }
    
    lck_mtx_lock(g_mutex);
    
    struct sockaddr name;
    printf("应用防火墙: %s 状态 %s ", entry->desc.name, event_t_string[event]);
    sock_getsockname(so, &name, sizeof(name));
    log_ip_and_port_addr((struct sockaddr_in*)&name);
    printf(" - ");
    sock_getpeername(so, &name, sizeof(name));
    log_ip_and_port_addr((struct sockaddr_in*)&name);
    printf("\n");
    
    lck_mtx_unlock(g_mutex);
    
failed:
    ;
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
        entry->desc.do_block = block;
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
        printf("应用防火墙: 初始化内存标记失败\n");
        goto failed;
    }
    
    if (!(g_mutex_group = lck_grp_alloc_init(BUNDLE_ID, LCK_GRP_ATTR_NULL))) {
        printf("应用防火墙: 初始化锁组失败\n");
        goto failed;
    }
    
    if (!(g_mutex = lck_mtx_alloc_init(g_mutex_group, LCK_ATTR_NULL))) {
        printf("应用防火墙: 初始化锁失败\n");
        goto failed;
    }
    
    if (sflt_register(&socket_tcp_filter, PF_INET, SOCK_STREAM, IPPROTO_TCP) != KERN_SUCCESS) {
        printf("应用防火墙: 注册套接字过滤器失败\n");
        goto failed;
    }
    
    add_entry("QQ", 0); // 记录所有QQ请求
    add_entry("Thunder", 1); // 拦截所有迅雷请求
    
    g_filter_registered = TRUE;
    
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
    
    if (g_filter_registered == TRUE && !g_filter_unregister_started) {
        sflt_unregister(APPWALL_FLT_TCP_HANDLE);
        g_filter_unregister_started = TRUE;
    }
    /* 如果注销失败 */
    if (g_filter_registered == TRUE) {
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
