//
//  SocketForward.c
//  SocketForward
//
//  Created by 咲衣憧 on 15/7/1.
//  Copyright (c) 2015年 Kingfree. All rights reserved.
//

#include <mach/mach_types.h>

kern_return_t SocketForward_start(kmod_info_t * ki, void *d);
kern_return_t SocketForward_stop(kmod_info_t *ki, void *d);

kern_return_t SocketForward_start(kmod_info_t * ki, void *d)
{
    return KERN_SUCCESS;
}

kern_return_t SocketForward_stop(kmod_info_t *ki, void *d)
{
    return KERN_SUCCESS;
}
