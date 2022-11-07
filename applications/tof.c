/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-05     10091       the first version
 */


#include <rtthread.h>
#include <rtdevice.h>
#include "sensor.h"
#include "vl53l0x.h"

rt_thread_t distance_thread_f,distance_thread_b;
rt_uint32_t f_dis=9999,b_dis=9999;

void read_for_distance_entry(void *parameter)
{
    rt_device_t temp_dev = RT_NULL;
    struct rt_sensor_data temp_data;
    rt_size_t res = 0;

    temp_dev = rt_device_find("tof_f_v");
    if (temp_dev == RT_NULL)
    {
         rt_kprintf("not found tof_vl53l0x_f device\r\n");
        return;
    }

    if (rt_device_open(temp_dev, RT_DEVICE_FLAG_RDONLY) != RT_EOK)
    {
        rt_kprintf("open tof_vl53l0x_f failed\r\n");
        return;
    }

    while (1)
    {
        rt_uint32_t level;
        level = rt_hw_interrupt_disable();
        res = rt_device_read(temp_dev, 0, &temp_data, 1);
        rt_hw_interrupt_enable(level);
        if (res == 0)
        {

            rt_kprintf("read data failed! result is %d\n", res);;
        }
        else
        {
            f_dis = temp_data.data.proximity;
        }

        rt_thread_mdelay(2000);
    }
}

void read_back_distance_entry(void *parameter)
{
    rt_device_t temp_dev = RT_NULL;
    struct rt_sensor_data temp_data;
    rt_size_t res = 0;

    temp_dev = rt_device_find("tof_b_v");
    if (temp_dev == RT_NULL)
    {
         rt_kprintf("not found tof_vl53l0x_b device\r\n");
        return;
    }

    if (rt_device_open(temp_dev, RT_DEVICE_FLAG_RDONLY) != RT_EOK)
    {
        rt_kprintf("open tof_vl53l0x_b failed\r\n");
        return;
    }

    while (1)
    {
        rt_uint32_t level;
        level = rt_hw_interrupt_disable();
        res = rt_device_read(temp_dev, 0, &temp_data, 1);
        rt_hw_interrupt_enable(level);
        if (res == 0)
        {

            rt_kprintf("read data failed! result is %d\n", res);
        }
        else
        {
            b_dis = temp_data.data.proximity;
        }

        rt_thread_mdelay(2000);
    }
}

int read_distance_init(void)
{

    distance_thread_f = rt_thread_create("tof_f",
                                     read_for_distance_entry,
                                     RT_NULL,
                                     2048,
                                     RT_THREAD_PRIORITY_MAX / 2,
//                                     4,
                                     800);
    if (distance_thread_f != RT_NULL)
    {
        rt_thread_startup(distance_thread_f);
    }

    distance_thread_b = rt_thread_create("tof_b",
                                     read_back_distance_entry,
                                     RT_NULL,
                                     2048,
                                    RT_THREAD_PRIORITY_MAX / 2,
 //                                    4,
                                     800);
    if (distance_thread_b != RT_NULL)
    {
        rt_thread_startup(distance_thread_b);
    }

    return RT_EOK;
}

int rt_hw_vl53l0x_port(void)
{
    struct rt_sensor_config cfg1,cfg2;

    cfg1.intf.dev_name = "i2c1";         /* i2c bus */
    cfg1.intf.user_data = (void *)0x29;  /* i2c slave addr */
    rt_hw_vl53l0x_init("f_v", &cfg1, 57);/* xshutdown ctrl pin */

    cfg2.intf.dev_name = "i2c2";         /* i2c bus */
    cfg2.intf.user_data = (void *)0x29;  /* i2c slave addr */
    rt_hw_vl53l0x_init("b_v", &cfg2, 58);/* xshutdown ctrl pin */


    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_vl53l0x_port);
