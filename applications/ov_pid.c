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
rt_int32_t max_pulse = 125,min_pulse = 25;
extern rt_int32_t ov_period,ov_pulse;
extern struct rt_device_pwm *ov_dev;
int middle = 160;
float kp = 0;
float ki = 0;
float kd = 0;
float dia=0;
float error=0,ierror=0,derror=0,errorlast=0;

int angle_limit(rt_int32_t *angle)
{
    if(*angle<min_pulse)
        *angle = min_pulse;
    else if (*angle>max_pulse) {
        *angle = max_pulse;
    }
    rt_pwm_set(ov_dev,OV_CHANNEL, ov_period, *angle);
    return RT_EOK;
}

int direction_pid_compute(rt_int32_t *val)
{
       error = middle*1.0 - *val;
       ierror=ierror+error;
       derror=error-errorlast;
       errorlast=error;
       if(ierror>3000) ierror=3000;
       else if(ierror<-3000) ierror=-3000;
       dia = kp*error+ki*ierror+kd*derror;
       ov_pulse = ov_pulse+dia;

       return 0;
}
