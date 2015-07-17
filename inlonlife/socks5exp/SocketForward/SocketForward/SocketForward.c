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

static boolean_t g_filter_registered = FALSE;
static boolean_t g_filter_unregister_started = FALSE;


kern_return_t SocketForward_start(kmod_info_t* ki, void* d);
kern_return_t SocketForward_stop(kmod_info_t* ki, void* d);

static void pp(const char* fmt, ...)
{
    va_list p;
    char buf[128];

    va_start(p, fmt);

    vsnprintf(buf, sizeof(buf), fmt, p);
    printf(BUNDLE_ID ": %s\n", buf);

    va_end(p);
}

static errno_t alloc_locks()
{
    errno_t ret = 0;

    global_mutex_group = lck_grp_alloc_init(BUNDLE_ID, LCK_GRP_ATTR_NULL);
    if (global_mutex_group == NULL) {
        pp("lck_grp_alloc_init 失败");
        ret = ENOMEM;
    }

    if (ret == 0) {
        global_mutex = lck_mtx_alloc_init(global_mutex_group, LCK_ATTR_NULL);
        if (global_mutex == NULL) {
            pp("lck_mtx_alloc_init 失败");
            ret = ENOMEM;
        }
    }

    return ret;
}

static void free_locks()
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
    pp("尝试注销");
    g_filter_registered = FALSE;
}

static errno_t sf_attach(void** cookie, socket_t so)
{
    errno_t ret = 0;
    
    lck_mtx_lock(global_mutex);

    char name[PATH_MAX];
    
    proc_selfname(name, PATH_MAX);
    pp("proc_selfname: %s", name);
    if (strncmp(name, "nc", 3)) {
        pp("装载到进程: %s", name);
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
    pp("proc_selfname: %s", name);
    
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

static errno_t sf_init()
{
    errno_t ret = 0;
 
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

    pp("启动");

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
        pp("sflt_register 注册, 返回 %d.", ret);
    } else {
        pp("sflt_register 注册失败, 返回 %d.", ret);
        goto finally;
    }
    return ret;
    
    g_filter_registered = TRUE;

finally:
    
    sflt_unregister(SF_HANDLE);

    free_locks();

    return ret;
}

kern_return_t SocketForward_stop(kmod_info_t* ki, void* d)
{
    kern_return_t ret = KERN_SUCCESS;

    if (g_filter_registered == TRUE && !g_filter_unregister_started) {
        sflt_unregister(SF_HANDLE);
        g_filter_unregister_started = TRUE;
    }
    if (g_filter_registered == TRUE) {
        pp("正在忙，无法卸载");
        return EBUSY;
    }

    free_locks();

    return ret;
}
