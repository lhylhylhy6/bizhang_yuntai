/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-05     10091       the first version
 */
#include "test.h"
#include "straight_pid.h"
#include "ov_pid.h"
extern float f_dis;
extern struct rt_device_pwm *direction_dev;
extern struct rt_device_pwm *ov_dev;
extern struct rt_device_pwm *down_dev;
extern rt_uint32_t direction_period,direction_pulse;
extern rt_uint32_t ov_period,ov_pulse;
rt_uint32_t pulse_4 = 30;
rt_uint32_t qiwang=200;
int stop_flag = 0;
int xuanzhuan = 0;
int flag3 = 0;
int flag4=0;
int angle = 50;

int test_1(int argc,char **argv)
{

    if(argc<2)
    {
        rt_kprintf("error\n");
        return 0;
    }
    straight_pid_init();
    qiwang = atoi(argv[1]);
    rt_pwm_set(direction_dev,DIRECTION_CHANNEL, direction_period, direction_period*60/1000);
    car_start();
    while(stop_flag==0)
    {
      rt_thread_delay(200);
      if(f_dis<qiwang)
       stop_flag=1;
    }
    car_stop();
    return 0;
}
extern rt_uint8_t ov_stop_flag;
int test_2(int argc,char **argv)
{
    if(argc<2)
    {
        rt_kprintf("left_forward error\n");
    }
    angle = atoi(argv[1]);
    car_start();
    while(xuanzhuan==0)
    {
        car_left_forward();
        rt_thread_delay(200);
        if(ov_pulse<=angle)
            xuanzhuan = 1;

    }
    car_stop();
    ov_stop_flag=0;
    ov_pid_clearn();
    for(int i=10;i>=0;i--)
        rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*60/1000);
    return 0;
}

int test_3(int argc,char **argv)
{
    car_start();
    int i=5;
    for(i=5;i>=0;i--)
      rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*120/1000);


    while((flag3 == 0))
    {
        if(ov_stop_flag==0)
        {
            rt_thread_mdelay(1000);
            ov_stop_flag=1;
        }
        if(ov_pulse>=100)
            flag3 = 1;
        rt_thread_mdelay(200);
    }
    for(int i=0;i>=0;i--)
    {
        for(int i=200;i>=0;i--)
        {

        }
    }
    car_stop();


    return 0;
}

int test_4(int argc,char **argv)
{
    int delay_tim=0;
    pulse_4 = atoi(argv[1]);
    delay_tim = atoi(argv[2]);
    rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*pulse_4/1000);
    car_start();
    ov_stop_flag=0;
    ov_pid_clearn();
    rt_thread_mdelay(100);
    rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*40/1000);
    rt_thread_mdelay(delay_tim);
    ov_stop_flag=1;
    rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*75/1000);
    ov_pulse=75;
    while((flag4 == 0))
    {

        if(ov_pulse<=30)
            flag4 = 1;
        rt_thread_mdelay(200);
    }
    car_stop();
    return 0;
}
int test_5(int argc,char **argv)
{
    int direction_pulse=85;
    int test_ov = 75;
    int test_down = 90;
    if(argc==4)
    {
        direction_pulse = atoi(argv[1]);
        test_ov = atoi(argv[2]);
        test_down = atoi(argv[3]);
    }
    rt_pwm_set(down_dev, DOWN_CHANNEL,ov_period , ov_period*test_down/1000);
    rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*direction_pulse/1000);
    car_start();
    ov_stop_flag=0;
    ov_pid_clearn();
    rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*test_ov/1000);
    rt_thread_mdelay(1000);
    ov_stop_flag=1;

    return 0;
}

MSH_CMD_EXPORT(test_1,1_test);
MSH_CMD_EXPORT(test_2,2_test);
MSH_CMD_EXPORT(test_3,3_test);
MSH_CMD_EXPORT(test_4,4_test);
MSH_CMD_EXPORT(test_5,5_test);
