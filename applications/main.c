/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-28     RT-Thread    first version
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "car.h"
#include "uart2.h"
#include "ov_pid.h"

int main(void)
{
    int count = 1;
    LOG_D("v0.5\n");
    car_init();
    my_pwm_init();
    ov_uart_init();
    ov_pid_init();
//    HCSR_forward_init();
    LOG_D("init ok\n");
    while (count++)
    {
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}
