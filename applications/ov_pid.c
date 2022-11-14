/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-29     10091       the first version
 */
#include <ov_pid.h>
#include "my_pwm.h"

rt_mutex_t ov_val_pro;
rt_thread_t ov_pid_compute;
rt_uint32_t max_pulse = 130,min_pulse = 20;

rt_mailbox_t straight_single = RT_NULL;
rt_uint32_t ov_mid_pulse = 75;

rt_uint8_t ov_stop_flag=1;

/*
 * 中间75 右打死 20 左打死130
 */
extern rt_uint32_t ov_period,ov_pulse;
extern struct rt_device_pwm *ov_dev;
extern struct rt_completion ov_comp;
extern rt_uint32_t ov_location;
extern struct rt_completion straight_comp;

int middle = 165;
float kp =  0.0335;
float ki = 0;
float kd = 0.029;
float dia=0;
float error=0,ierror=0,derror=0,errorlast=0;
rt_uint32_t old_pulse = 75;
int angle_limit(rt_uint32_t *angle)
{
    if(*angle<min_pulse)
        *angle = min_pulse;
    else if (*angle>max_pulse) {
        *angle = max_pulse;
    }

//    rt_kprintf("%d\n",*angle);
    rt_pwm_set(ov_dev,OV_CHANNEL, ov_period, ov_period*(*angle)/1000);

    return RT_EOK;
}

int direction_pid_compute(rt_int32_t val)
{
       error = middle*1.0 - val;

       ierror=ierror+error;
       derror=error-errorlast;
       errorlast=error;
       if(ierror>3000) ierror=3000;
       else if(ierror<-3000) ierror=-3000;
       dia = kp*error+ki*ierror+kd*derror;
       ov_pulse =  ov_pulse-dia; //125-25

       return 0;
}

void ov_pid_entry(void *parameter)
{
    while(1)
    {
        if(ov_stop_flag==1)
        {
            rt_completion_wait(&ov_comp, RT_WAITING_FOREVER);
            dia = 0;
            rt_mutex_take(ov_val_pro, RT_WAITING_FOREVER);
            direction_pid_compute(ov_location);
            rt_mutex_release(ov_val_pro);
            angle_limit(&ov_pulse);
            rt_completion_done(&straight_comp);
        }

        rt_thread_mdelay(10);
    }
}


int ov_pid_init(void)
{
    rt_err_t err = RT_EOK;
    straight_single = rt_mb_create("straight_single", 1, RT_IPC_FLAG_FIFO);
    ov_pid_compute = rt_thread_create("ov_pid", ov_pid_entry, RT_NULL, 1024, 7, 300);
    if(ov_pid_compute)
    {
       rt_kprintf("ov_pid_thread create successfully\n");
       rt_thread_startup(ov_pid_compute);
    }
    else {
        rt_kprintf("ov_pid_thread create error\n");
    }
    return err;
}

int ov_pid_set(int argc,char **argv)
{
    if(argc < 4)
    {
        rt_kprintf("Too few parameters\n");
        return error;
    }
    kp =  atof(argv[1]);
    ki =  atof(argv[2]);
    kd =  atof(argv[3]);
    rt_kprintf("ov_pid set ok\n");
    return RT_EOK;
}
void ov_pid_clearn(void)
{
    error=0;
    ierror=0;
    derror=0;
    errorlast=0;
}
MSH_CMD_EXPORT(ov_pid_set,ov_pid parameter set);
