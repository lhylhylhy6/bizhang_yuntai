/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-29     10091       the first version
 */
#include "uart2.h"

extern rt_mutex_t ov_val_pro;
struct rt_completion ov_comp;
struct rt_completion straight_comp;
rt_sem_t ov_rx_sem = RT_NULL;
rt_device_t ov_uart = RT_NULL;
rt_thread_t ov_pid_thread;
rt_uint32_t location = 0;
rt_uint32_t ov_location;
rt_uint8_t final_stop=0;
extern rt_uint8_t red_flag;

static rt_err_t ov_rx_interupt(rt_device_t dev,rt_size_t size)
{
     rt_sem_release(ov_rx_sem);
     return RT_EOK;
}

static void ov_thread_enty(void *parameter)
{
    char ch;
    while(1)
    {
        while(rt_device_read(ov_uart, -1, &ch, 1)!=1)
        {
            rt_sem_take(ov_rx_sem, RT_WAITING_FOREVER);
        }
        if(ch=='[')
         {
            rt_mutex_take(ov_val_pro, RT_WAITING_FOREVER);
         }
         else if(ch==']')
         {
             ov_location = location;
             rt_mutex_release(ov_val_pro);
             rt_completion_done(&ov_comp);
             red_flag=1;
             //rt_kprintf("%d\n",ov_location);
             location = 0 ;
         }
         else if(ch=='c')
         {
             final_stop = 1;
         }
         else if(ch>='0'&&ch<='9')
         {
             location=location *10 + ch-'0';
         }
         ch=0;
    }
}

rt_err_t ov_uart_init(void)
{
    rt_err_t ret = RT_EOK;
    ov_val_pro = rt_mutex_create("ov_val_pro", RT_IPC_FLAG_PRIO);
    rt_mutex_release(ov_val_pro);
    ov_rx_sem = rt_sem_create("ov_rx", 0, RT_IPC_FLAG_PRIO);
    rt_completion_init(&ov_comp);
    rt_completion_init(&straight_comp);
    ov_uart = rt_device_find(ov_uart_name);
    if(ov_uart)
    {
        ret = rt_device_open(ov_uart, RT_DEVICE_FLAG_INT_RX);
    }
    else {
        rt_kprintf("error ov_uart find\r\n");
        return -1;
    }
    rt_device_set_rx_indicate(ov_uart, ov_rx_interupt);


    ov_pid_thread = rt_thread_create("ov_read_thread", ov_thread_enty, 0, 1024, 5, 300);
    if(ov_pid_thread)
    {
        rt_thread_startup(ov_pid_thread);
    }
    else {
        rt_kprintf("create ov_pid_thread error\r\n");
    }
    return ret;

}

int uart2_send_down(void)
{
    char ch = 'c';
    rt_device_write(ov_uart, -1, &ch, sizeof(char));
    rt_kprintf("%c\n",ch);
    return 0;
}
int uart2_send_up(void)
{
    char ch = 'a';
    rt_device_write(ov_uart, -1, &ch, sizeof(char));
    rt_kprintf("%c\n",ch);
    return 0;
}


