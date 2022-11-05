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
int straight_mid = 75;
float straight_kp = -0.92;
float straight_ki = 0;
float straight_kd = 0;
float straight_dia=0;
float error1=0,ierror1=0,derror1=0,errorlast1=0;
extern struct rt_completion straight_comp;
extern struct rt_device_pwm *direction_dev;
rt_uint32_t straight_max_pulse = 115,straight_min_pulse=24;
rt_uint32_t straight_middle_pulse = 70;
extern rt_uint32_t direction_period,direction_pulse;
extern rt_uint32_t ov_period,ov_pulse;
extern rt_uint32_t ov_location;
extern rt_mailbox_t straight_single;

int staright_pid_limit(int pwm_l)
{
    if(pwm_l<straight_min_pulse)
        pwm_l = straight_min_pulse;
    else if (pwm_l>straight_max_pulse) {
        pwm_l = straight_max_pulse;
    }
    //rt_kprintf("%d\n",*angle);
    rt_kprintf("pwm_l is:%d\n",pwm_l);
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
    direction_pulse = straight_middle_pulse-straight_dia;
    staright_pid_limit(direction_pulse);
    return 0;
}

void straight_entry(void *parameter)
{
    while(1)
    {
        rt_completion_wait(&straight_comp, RT_WAITING_FOREVER);
        straight_pid_compute(ov_pulse);
        rt_kprintf("ov_pulse is :%d\n",ov_pulse);
        rt_thread_mdelay(100);
    }

}


int straight_pid_init(void)
{
    straight_pid = rt_thread_create("thread_pid", straight_entry, RT_NULL, 1024, 7, 300);
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

int straight_pid_set(int argc,char **argv)
{
    if(argc < 4)
    {
        rt_kprintf("Too few parameters\n");
        return RT_ERROR;
    }
    straight_kp =  atof(argv[1]);
    straight_ki =  atof(argv[2]);
    straight_kd =  atof(argv[3]);
    rt_kprintf("straight_pid set ok\n");
    return RT_EOK;
}
MSH_CMD_EXPORT(straight_pid_set,ov_pid parameter set);
