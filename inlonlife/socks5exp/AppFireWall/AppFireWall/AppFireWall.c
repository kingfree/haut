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

kern_return_t AppFireWall_start(kmod_info_t * ki, void *d);
kern_return_t AppFireWall_stop(kmod_info_t *ki, void *d);

#define APPWALL_FLT_TCP_HANDLE 'apw0'
#define BUNDLE_ID "kingfree.AppFireWall"

static boolean_t g_filter_registered = FALSE;

static void appwall_unregistered(sflt_handle handle)
{
    printf("AppFireWall: 未注册\n");
    g_filter_registered = FALSE;
}

static errno_t appwall_attach(void **cookie, socket_t so)
{
    errno_t result = 0;
    char name[PATH_MAX];
    *cookie = NULL;
    proc_selfname(name, PATH_MAX);
    printf("AppFireWall: 装载 %s\n", name);
    return result;
}

static void appwall_detach(void *cookie, socket_t so)
{
    printf("AppFireWall: 卸载\n");
    if (cookie) {
        cookie = NULL;
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

kern_return_t AppFireWall_start(kmod_info_t * ki, void *d)
{
    int ret = 0;
    ret = sflt_register(&socket_tcp_filter, PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ret != KERN_SUCCESS) {
        printf("AppFireWall: 注册套接字过滤器失败\n");
        goto failed;
    }
    printf("AppFireWall: hello, 世界\n");
    return KERN_SUCCESS;
failed:
    return KERN_FAILURE;
}

kern_return_t AppFireWall_stop(kmod_info_t *ki, void *d)
{
    return KERN_SUCCESS;
}
