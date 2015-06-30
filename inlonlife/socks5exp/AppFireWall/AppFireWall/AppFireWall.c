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
    printf("AppFireWall: %s:%d\n", addstr, ntohs(addr->sin_port));
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
    printf("AppFireWall: 注销\n");
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
        printf("AppFileWall: 装载到进程: %s\n", name);
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
            printf("AppFileWall: ===================================\n");
            printf("AppFileWall: 进程报告: %s\n", entry->desc.name);

            if (entry->desc.do_block) {
                printf("AppFileWall:     拦截输入流量: %d\n", entry->desc.inbound_blocked);
                printf("AppFileWall:     拦截输出流量: %d\n", entry->desc.outbound_blocked);
            }
            else {
                printf("AppFileWall:     输入字节: %lu\n", entry->desc.bytes_in);
                printf("AppFileWall:     输出字节: %lu\n", entry->desc.bytes_out);
                printf("AppFileWall:     输入数据包: %lu\n", entry->desc.packets_in);
                printf("AppFileWall:     输出数据包: %lu\n", entry->desc.packets_out);
            }

            cookie = NULL;
        }

        lck_mtx_unlock(g_mutex);
    }
    return;
}

static struct sflt_filter socket_tcp_filter = {
    APPWALL_FLT_TCP_HANDLE,
    SFLT_GLOBAL,
    BUNDLE_ID,
    appwall_unregistered,
    appwall_attach,
    appwall_detach,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
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

    return KERN_SUCCESS;
}

kern_return_t AppFireWall_start(kmod_info_t* ki, void* d)
{
    TAILQ_INIT(&g_block_list);

    if (!(g_osm_tag = OSMalloc_Tagalloc(BUNDLE_ID, OSMT_DEFAULT))) {
        printf("AppFireWall: 初始化内存标记失败\n");
        goto failed;
    }

    if (!(g_mutex_group = lck_grp_alloc_init(BUNDLE_ID, LCK_GRP_ATTR_NULL))) {
        printf("AppFireWall: 初始化锁组失败\n");
        goto failed;
    }

    if (!(g_mutex = lck_mtx_alloc_init(g_mutex_group, LCK_ATTR_NULL))) {
        printf("AppFireWall: 初始化锁失败\n");
        goto failed;
    }

    if (sflt_register(&socket_tcp_filter, PF_INET, SOCK_STREAM, IPPROTO_TCP) != KERN_SUCCESS) {
        printf("AppFireWall: 注册套接字过滤器失败\n");
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
