/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-02     10091       the first version
 */
#include "straight_pid.h"
#include "my_pwm.h"
rt_thread_t straight_pid;
int straight_mid = 50;
float straight_kp = -0.1105;
float straight_ki = 0;
float straight_kd = 0;
float straight_dia=0;
float error1=0,ierror1=0,derror1=0,errorlast1=0;

extern struct rt_device_pwm *direction_dev;
extern rt_uint32_t max_pulse,min_pulse;
extern rt_uint32_t direction_period,direction_pulse;
extern rt_uint32_t ov_period,ov_pulse;
int staright_pid_limit(int pwm_l)
{
    if(pwm_l<min_pulse)
        pwm_l = min_pulse;
    else if (pwm_l>max_pulse) {
        pwm_l = max_pulse;
    }
    //rt_kprintf("%d\n",*angle);
    rt_pwm_set(direction_dev,DIRECTION_CHANNEL,direction_period,direction_period*(pwm_l)/1000);
    return RT_EOK;
}

int straight_pid_compute(rt_uint32_t val)
{
    error1 = straight_mid*1.0 - val;
    ierror1=ierror1+error1;
    derror1=error1-errorlast1;
    errorlast1=error1;
    if(ierror1>3000) ierror1=3000;
    else if(ierror1<-3000) ierror1=-3000;
    straight_dia = straight_kp*error1+straight_ki*ierror1+straight_kd*derror1;
    direction_pulse = direction_pulse-straight_dia;
    staright_pid_limit(direction_pulse);
    return 0;
}

void straight_entry(void *parameter)
{
    straight_pid_compute(ov_pulse);
    rt_thread_mdelay(100);
}


int straight_pid_init(void)
{
    straight_pid = rt_thread_create("thread_pid", straight_entry, RT_NULL, 1024, 5, 300);
    if(straight_pid)
    {
        rt_thread_startup(straight_pid);
        rt_kprintf("straight init ok\n");
    }
    else {
        rt_kprintf("straight_pid error\n");
    }
    return RT_EOK;
}
