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
#include "straight_pid.h"
#include "hcsr04.h"

extern rt_uint32_t direction_period,direction_pulse;
extern rt_uint32_t ov_period,ov_pulse;
extern struct rt_device_pwm *direction_dev;
extern struct rt_device_pwm *ov_dev;
extern float forward_min_val;
extern float back_min_val;
extern float forward_val;
extern float back_val;

rt_timer_t clearn_tim;
void clearn(void *parameter)
{
    //forward_min_val=9999;
    back_min_val=9999;
    //LOG_D("clearn\n");
}
int main(void)
{
    int count = 1;
    //LOG_D("v0.5\n");
    car_init();
    my_pwm_init();
    ov_uart_init();
    ov_pid_init();
//    straight_pid_init();
    HCSR_forward_init();
    HCSR_back_init();
    clearn_tim = rt_timer_create("clearner", clearn, RT_NULL, 500, RT_TIMER_FLAG_SOFT_TIMER|RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(clearn_tim);
    //LOG_D("init ok\n");
    while (count++)
    {
        //LOG_D("%f %f %f\n",forward_val,back_min_val,forward_val+back_min_val);
        car_right_forward();
        while(1)
        {
            if(ov_pulse>=110)
            {
                LOG_D("full left\n");
                car_left_forward();
            }
            LOG_D("%f %f %f\n",forward_val,back_min_val,forward_val+back_min_val);
            if(((back_min_val+forward_val)>=60)&&((back_min_val+forward_val)<=140))
            {
                LOG_D("full right\n");
                int level = rt_hw_interrupt_disable();
                //云台 轮子 右转打死
                rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*125/1000);
                rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*125/1000);
                rt_hw_interrupt_enable(level);
                LOG_D("full right over\n");
                break;
            }
            rt_thread_mdelay(200);
        }
        while(1)
        {
            LOG_D("break ok\n");
            if(((back_min_val+forward_val)>=80)&&((back_min_val+forward_val)<=120))
            {
                   LOG_D("```left full\n");
                   int level = rt_hw_interrupt_disable();
                   rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*24/1000);
                   rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*25/1000);
                   rt_hw_interrupt_enable(level);
                   break;
            }
            rt_thread_mdelay(200);
        }
        while(1)
        {
            rt_thread_mdelay(500);
        }
        rt_thread_mdelay(200);
    }

    return RT_EOK;
}
