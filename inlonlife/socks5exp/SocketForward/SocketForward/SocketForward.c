//
//  SocketForward.c
//  SocketForward
//
//  Created by 咲衣憧 on 15/7/1.
//  Copyright (c) 2015年 Kingfree. All rights reserved.
//

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
#include <netinet/in.h>
#include <kern/locks.h>
#include <kern/assert.h>
#include <kern/debug.h>
#include <netinet/in.h>

#include <libkern/OSMalloc.h>
#include <libkern/OSAtomic.h>
#include <sys/kern_control.h>
#include <sys/kauth.h>
#include <sys/time.h>
#include <stdarg.h>

#define SF_HANDLE 'sfw0'
#define BUNDLE_ID "kingfree.SocketForward"

struct data_pack {
    mbuf_t mbuf;
};

static lck_mtx_t* global_mutex = NULL;
static lck_grp_t* global_mutex_group = NULL;

kern_return_t SocketForward_start(kmod_info_t* ki, void* d);
kern_return_t SocketForward_stop(kmod_info_t* ki, void* d);

static void sf_printf(const char* fmt, ...)
{
    va_list p;
    char buf[128];

    va_start(p, fmt);

    vsnprintf(buf, sizeof(buf), fmt, p);
    printf(BUNDLE_ID ": %s\n", buf);

    va_end(p);
}

static errno_t alloc_locks(void)
{
    errno_t ret = 0;

    global_mutex_group = lck_grp_alloc_init(BUNDLE_ID, LCK_GRP_ATTR_NULL);
    if (global_mutex_group == NULL) {
        sf_printf("lck_grp_alloc_init 失败");
        ret = ENOMEM;
    }

    if (ret == 0) {
        global_mutex = lck_mtx_alloc_init(global_mutex_group, LCK_ATTR_NULL);
        if (global_mutex == NULL) {
            sf_printf("lck_mtx_alloc_init 失败");
            ret = ENOMEM;
        }
    }

    return ret;
}

static void free_locks(void)
{
    if (global_mutex) {
        lck_mtx_free(global_mutex, global_mutex_group);
        global_mutex = NULL;
    }

    if (global_mutex_group) {
        lck_grp_free(global_mutex_group);
        global_mutex_group = NULL;
    }
}

static void sf_unregistered(sflt_handle handle)
{
}

static errno_t sf_attach(void** cookie, socket_t so)
{
    errno_t ret = 0;
    
    lck_mtx_lock(global_mutex);

    char name[PATH_MAX];
    
    proc_selfname(name, PATH_MAX);
    sf_printf("proc_selfname: %s", name);
    if (strncmp(name, "nc", 3)) {
        sf_printf("装载到进程: %s", name);
    } else {
        ret = ENOPOLICY;
    }
    
    lck_mtx_unlock(global_mutex);

    return ret;
}

static void sf_detach(void* cookie, socket_t so)
{
}

static void sf_notify(void* cookie, socket_t so, sflt_event_t event, void* param)
{
}

static errno_t sf_data_in(void* cookie, socket_t so, const struct sockaddr* from, mbuf_t* data, mbuf_t* control, sflt_data_flag_t flags)
{
    errno_t ret = 0;
    return ret;
}

static errno_t sf_data_out(void* cookie, socket_t so, const struct sockaddr* to, mbuf_t* data, mbuf_t* control, sflt_data_flag_t flags)
{
    errno_t ret = 0;
    return ret;
}

static errno_t sf_connect_in(void* cookie, socket_t so, const struct sockaddr* from)
{
    errno_t ret = 0;
    return ret;
}

//static char* proxy_ip = "127.0.0.1";

static errno_t sf_connect_out(void* cookie, socket_t so, const struct sockaddr* to)
{
    errno_t ret = 0;
    
    lck_mtx_lock(global_mutex);
    
//    struct sockaddr_in* addr = (struct sockaddr_in*)to;
//    if (inet_pton(AF_INET, proxy_ip, &addr->sin_addr.s_addr) == 0) {
//        sf_printf("inet_pton(%s) 失败", proxy_ip);
//        return -1;
//    }
    
    char name[PATH_MAX];

    proc_selfname(name, PATH_MAX);
    sf_printf("proc_selfname: %s", name);
    
    lck_mtx_unlock(global_mutex);

    return ret;
}

static errno_t sf_bind(void* cookie, socket_t so, const struct sockaddr* to)
{
    errno_t ret = 0;
    return ret;
}

static errno_t sf_setoption(void* cookie, socket_t so, sockopt_t opt)
{
    errno_t ret = 0;
    return ret;
}

static errno_t sf_getoption(void* cookie, socket_t so, sockopt_t opt)
{
    errno_t ret = 0;
    return ret;
}

static errno_t sf_listen(void* cookie, socket_t so)
{
    errno_t ret = 0;
    return ret;
}

static OSMallocTag gOSMallocTag;

static errno_t sf_init()
{
    errno_t ret = 0;
//    
//    SLIST_HEAD(slisthead, entry) head = SLIST_HEAD_INITIALIZER(head);
////    struct slisthead *headp;                /* Singly-linked List head. */
//    struct entry {
//        SLIST_ENTRY(entry) entries;     /* Singly-linked List. */
//    } *n1, *n2, *n3, *np;
//    
//    SLIST_INIT(&head);                      /* Initialize the list. */
//    
//    n1 = OSMalloc(sizeof(struct entry), gOSMallocTag);      /* Insert at the head. */
//    SLIST_INSERT_HEAD(&head, n1, entries);
//    
//    n2 = OSMalloc(sizeof(struct entry), gOSMallocTag);      /* Insert after. */
//    SLIST_INSERT_AFTER(n1, n2, entries);
//    
//    SLIST_REMOVE(&head, n2, entry, entries);/* Deletion. */
//    OSFree(n2, sizeof(n2), gOSMallocTag);
//    
//    n3 = SLIST_FIRST(&head);
//    SLIST_REMOVE_HEAD(&head, entries);      /* Deletion from the head. */
//    OSFree(n3, sizeof(n3), gOSMallocTag);
//    /* Forward traversal. */
//    SLIST_FOREACH(np, &head, entries) {
//        //np->do_stuff();
//        SLIST_REMOVE(&head, np, entry, entries);
//        OSFree(np, sizeof(np), gOSMallocTag);
//    }
//    
//    while (!SLIST_EMPTY(&head)) {           /* List Deletion. */
//        n1 = SLIST_FIRST(&head);
//        SLIST_REMOVE_HEAD(&head, entries);
//        OSFree(n1, sizeof(n1), gOSMallocTag);
//    }
//    
    return ret;

}

static struct sflt_filter sf_sflt_filter = {
    SF_HANDLE,
    SFLT_GLOBAL,
    BUNDLE_ID,
    sf_unregistered,
    sf_attach,
    sf_detach,
    sf_notify,
    NULL,
    NULL,
    sf_data_in,
    sf_data_out,
    sf_connect_in,
    sf_connect_out,
    sf_bind,
    sf_setoption,
    sf_getoption,
    sf_listen,
    NULL
};

kern_return_t SocketForward_start(kmod_info_t* ki, void* d)
{
    kern_return_t ret = KERN_SUCCESS;

    sf_printf("启动");

    if (alloc_locks() != 0) {
        ret = KERN_FAILURE;
        goto finally;
    }
    
    ret = sf_init();
    if (ret != 0) {
        goto finally;
    }

    ret = sflt_register(&sf_sflt_filter, PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ret == 0) {
        sf_printf("sflt_register 注册, 返回 %d.", ret);
    }
    else {
        goto finally;
    }

finally:
    
    sflt_unregister(SF_HANDLE);

    free_locks();

    return ret;
}

kern_return_t SocketForward_stop(kmod_info_t* ki, void* d)
{
    kern_return_t ret = KERN_SUCCESS;

    sf_printf("停止");
    
    ret = sflt_unregister(SF_HANDLE);
    if (ret == 0) {
        sf_printf("sflt_register 注销, 返回 %d.", ret);
    }
    else {
        goto finally;
    }
    
finally:

    free_locks();

    return ret;
}
