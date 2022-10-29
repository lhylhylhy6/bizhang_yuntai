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

int main(void)
{
    int count = 1;
    car_init();
    ov_uart_init();
    while (count++)
    {
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}
