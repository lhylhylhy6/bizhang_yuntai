/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-28     10091       the first version
 */
#include "my_pwm.h"


struct rt_device_pwm *left_dev;
struct rt_device_pwm *right_dev;
struct rt_device_pwm *direction_dev;
struct rt_device_pwm *ov_dev;

rt_int32_t speed_period,speed_pulse;
rt_int32_t direction_period,direction_pulse;
rt_int32_t ov_period,ov_pulse;

int my_pwm_enable(void);
int my_pwm_disable(void);

int my_pwm_init(void)
{
    rt_err_t err = RT_EOK;
    speed_period = 1000000;
    speed_pulse  = 50;

    left_dev = (struct rt_device_pwm*)rt_device_find(LEFT_PWM);
    if(left_dev == RT_NULL)
    {
        rt_kprintf("left_pwm init error\n");
    }
    right_dev = (struct rt_device_pwm*)(rt_device_find(RIGHT_PWM));
    if(right_dev == RT_NULL)
    {
        rt_kprintf("right_pwm init error\n");
    }
    rt_pwm_set(left_dev, LEFT_CHANNEL, speed_period, speed_period*speed_pulse/100);
    rt_pwm_set(right_dev, RIGHT_CHANNEL, speed_period, speed_period*speed_pulse/100);


    my_pwm_disable();


    direction_period = 20000000,direction_pulse = 75;
    direction_dev = (struct rt_device_pwm*)rt_device_find(DIRECTION_PWM);
    if(direction_dev == RT_NULL)
    {
        rt_kprintf("direction_dev init error\n");
    }
    rt_pwm_set(direction_dev, DIRECTION_CHANNEL,direction_period , direction_period*direction_period/1000);

    ov_period = 20000000,ov_pulse = 75;
    ov_dev = (struct rt_device_pwm*)rt_device_find(OV_PWM);
    if(ov_dev == RT_NULL)
    {
        rt_kprintf("ov_dev init error\n");
    }
    rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*ov_pulse/1000);

    rt_pwm_enable(direction_dev, DIRECTION_CHANNEL);
    rt_pwm_enable(ov_dev, OV_CHANNEL);
    return err;
}

INIT_APP_EXPORT(my_pwm_init);

int my_pwm_enable(void)
{
    rt_pwm_enable(left_dev, LEFT_CHANNEL);
    rt_pwm_enable(right_dev, RIGHT_CHANNEL);
    return RT_EOK;
}

int my_pwm_disable(void)
{
    rt_pwm_disable(left_dev, LEFT_CHANNEL);
    rt_pwm_disable(right_dev, RIGHT_CHANNEL);
    return RT_EOK;
}

int my_pwm_set(struct rt_device_pwm *pwm,rt_int32_t new_pulse)
{
    if(pwm == left_dev)
        rt_pwm_set(left_dev, LEFT_CHANNEL, speed_period, new_pulse*speed_period/100);
    else if (pwm == right_dev)
    {
        rt_pwm_set(right_dev, RIGHT_CHANNEL, speed_period, new_pulse*speed_period/100);
    }
    return RT_EOK;
}

int my_pwm_extern_set_pulse(int argc,char **argv)
{
    char *dev_name;
    rt_uint32_t new_pulse=0;
    if(argc>=2)
    {
        dev_name = argv[1];
        new_pulse = atoi(argv[2]);

        if(strcmp(dev_name,"left")==0)
        {
            my_pwm_set(left_dev, new_pulse);
        }
        else if(strcmp(dev_name,"right")==0)
        {
            my_pwm_set(right_dev, new_pulse);
        }
        else if(strcmp(dev_name,"dir")==0)
        {
            rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, new_pulse);
        }
        else if(strcmp(dev_name,"ov")==0)
        {
            rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, new_pulse);
        }
    }
    else
    {
        rt_kprintf("please input my_pwm_extern_set_pulse <dev_name> <new_pulse>\r\n");
        rt_kprintf("<dev_name> can be left/right/dir/ov \r\n");
    }

    return 0;
}


MSH_CMD_EXPORT(my_pwm_extern_set_pulse,set dev pulse);

