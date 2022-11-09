/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-09     lhylh       the first version
 */
#include "uart5.h"

#define jg_uart_name "uart5"

rt_thread_t jg_read_thread;
rt_sem_t jg_rx_sem = RT_NULL;

rt_device_t jg_uart = RT_NULL;

rt_uint32_t jg_temp = 0;
rt_uint16_t jg_val;

static rt_err_t jg_rx_interupt(rt_device_t dev,rt_size_t size)
{
     rt_sem_release(jg_rx_sem);
     return RT_EOK;
}

static void jg_thread_enty(void *parameter)
{
    char ch;
    while(1)
    {
        while(rt_device_read(jg_uart, -1, &ch, 1)!=1)
        {
            rt_sem_take(jg_rx_sem, RT_WAITING_FOREVER);
        }
         if(ch=='[')
         {

         }
         else if(ch==']')
         {
             jg_val = jg_temp;
             jg_temp = 0 ;
         }
         else if(ch>='0'&&ch<='9')
         {
             jg_temp=jg_temp *10 + ch-'0';
         }
         ch=0;
    }
}

rt_err_t jg_uart_init(void)
{
    rt_err_t ret = RT_EOK;
    jg_rx_sem = rt_sem_create("jg_rx", 0, RT_IPC_FLAG_PRIO);
    jg_uart = rt_device_find(jg_uart_name);
    if(jg_uart)
    {
        ret = rt_device_open(jg_uart, RT_DEVICE_FLAG_INT_RX);
    }
    else {
        rt_kprintf("error jg_uart find\r\n");
        return -1;
    }
    rt_device_set_rx_indicate(jg_uart, jg_rx_interupt);


    jg_read_thread = rt_thread_create("jg_read_thread", jg_thread_enty, 0, 1024, 5, 300);
    if(jg_read_thread)
    {
        rt_thread_startup(jg_read_thread);
    }
    else {
        rt_kprintf("create jg_read_thread error\r\n");
    }
    return ret;

}
