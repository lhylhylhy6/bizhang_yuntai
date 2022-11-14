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
#include "uart2.h"
#include "tcs347225.h"
#define DBG_TAG "test"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

extern COLOR_RGBC rgb;
extern COLOR_HSL  hsl;
extern rt_device_t ov_uart;
extern rt_uint16_t jg_val;
extern struct rt_device_pwm *direction_dev;
extern struct rt_device_pwm *ov_dev;
extern struct rt_device_pwm *right_dev,*left_dev;
extern rt_uint32_t direction_period,direction_pulse;
extern rt_uint32_t ov_period,ov_pulse;
extern rt_uint8_t final_stop;
extern rt_uint32_t speed_period,speed_pulse;
extern rt_uint8_t ov_stop_flag;
rt_uint32_t pulse_4 = 30;
rt_uint32_t qiwang=450;
int stop_flag = 0;  //为0开启巡目标物体，为1关闭巡目标物体
int xuanzhuan = 0;
int flag3 = 0;
int flag4=0;
int angle = 50;
rt_int32_t zhangai=0; //障碍<75 右侧 zhangai<0
rt_uint32_t nums=0;
rt_uint8_t red_flag;
int test_1(void)
{

//    if(argc<2)
//    {
//        rt_kprintf("error\n");
//        return 0;
//    }
//    qiwang = atoi(argv[1]);

    //设置舵机方向，走直线到目标距离
    straight_pid_init();
    qiwang = 60;
    rt_pwm_set(direction_dev,DIRECTION_CHANNEL, direction_period, direction_period*60/1000);
    car_start();
    while(stop_flag==0)
    {
          //确定障碍方向
          red_flag=0;
          rt_thread_delay(200);
          if(red_flag==1)
          {
             nums++;
          }
          if(nums<=20&&red_flag==1)
          {
              if(ov_pulse>75)
                  zhangai+=1;
              else if(ov_pulse<75)
                  zhangai-=1;
          }

          //障碍<75 右侧 zhangai<0
          if(jg_val<=qiwang)
          {
              LOG_D("------------%d\n",jg_val);
              //关闭巡目标物体
              stop_flag=1;
          }
          rt_thread_delay(200);
    }
    car_stop();
    return 0;
}


int test_2(void)
{
//    if(argc<2)
//    {
//        rt_kprintf("left_forward error\n");
//    }
//    angle = atoi(argv[1]);
    angle = 55;
    car_start();

    //向右旋转直到舵机角度达到目标值
    while(xuanzhuan==0)
    {
        car_left_forward();     //右旋转
        rt_thread_delay(200);
        if(ov_pulse<=angle)
            xuanzhuan = 1;

    }
    rt_pwm_set(right_dev, 1, 1000000, 30*10000); //右
    rt_pwm_set(right_dev, 2, 1000000, 40*10000); //左
    rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*75/1000);
    rt_thread_mdelay(500);
    rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*55/1000);
    rt_pwm_set(right_dev, 1, 1000000, 30*10000); //右
    rt_pwm_set(right_dev, 2, 1000000, 30*10000); //左
    car_stop();

    //关闭ov寻物，设置ov为中值，pid清零，旋转舵机为左偏
    ov_stop_flag=0;
    ov_pulse=75;
    ov_pid_clearn();
    rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*85/1000);
    return 0;
}

int test_3(void)
{
//    car_start();
//    int i=5;
//    for(i=15;i>=0;i--)
//     rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*85/1000);
//     rt_thread_mdelay(1000);
//    rt_pwm_set(left_dev, 2,1000000 , 1000000*45/100);
//    rt_pwm_set(left_dev, 1,1000000 , 1000000*30/100);

    //手动扫描寻物
    red_flag=0;
    rt_uint8_t lll=ov_pulse,mmm=0;
    rt_thread_mdelay(1000);
    red_flag=0;
    while(1)
    {
        if(mmm==0)
            lll+=5;
        else if (mmm==1) {
            lll-=5;
        }
        if(lll>=130)
            mmm=1;
        else if (lll<=20) {
            mmm=0;
        }
        rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*lll/1000);
        if(red_flag==1)
            break;
        rt_thread_mdelay(100);
    }
    //设置ovpulse
    ov_pulse = lll;
    ov_stop_flag=1;
    car_start();
    //自动车巡物
    while((flag3 == 0))
    {
        //达到目标距离
        while(jg_val>=80)
        {
            stop_flag=0;
            rt_thread_mdelay(100);
        }
        //关闭自动车寻物
        stop_flag=1;
        //右轮打死，舵机达到目标角度退出
        rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*85/1000);
        rt_pwm_set(right_dev, 1, 1000000, 20*10000); //右
        rt_pwm_set(right_dev, 2, 1000000, 40*10000); //左
        if(ov_pulse>=100)
            flag3 = 1;
        rt_thread_mdelay(200);
    }
//    rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*85/1000);
    car_stop();
    rt_pwm_set(right_dev, 1, 1000000, 30*10000); //右
    rt_pwm_set(right_dev, 2, 1000000, 30*10000); //左
    return 0;
}

int test_4(void)
{
    int delay_tim=0;
//    pulse_4 = atoi(argv[1]);
//    delay_tim = atoi(argv[2]);
    pulse_4 = 20;
    delay_tim = 1000;
    //左轮打死，差速，开始左转绕柱
    rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*20/1000);
    rt_pwm_set(right_dev, 1, 1000000, 40*10000); //右
    rt_pwm_set(right_dev, 2, 1000000, 30*10000); //左
    car_start();
    //关闭ov寻物，设置ov右转，延迟一秒保证视野中无目标物体，打开ov寻物
    ov_stop_flag=0;
    ov_pid_clearn();
    rt_thread_mdelay(100);
    rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*40/1000);
    rt_thread_mdelay(delay_tim);
    ov_pulse = 40;
    ov_stop_flag=1;

    //设置ov向前看
    rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*75/1000);
    ov_pulse=75;
    //rt_thread_mdelay(1500);

    //一直左转，直到目标物体被识别到，更改差速为原速度
    red_flag=0;
    rt_thread_mdelay(20);
    while(red_flag==0)
    {
        rt_thread_mdelay(50);
    }
    rt_pwm_set(right_dev, 1, 1000000, 30*10000); //右
    rt_pwm_set(right_dev, 2, 1000000, 30*10000); //左

    //开启车巡物，直到距离达到目标
    stop_flag=0;
    while((flag4 == 0))
    {
        //卡死，直到达到目标距离
        while(1)
        {
            if(jg_val<=50)
            {
                stop_flag=1;
                break;
            }
            rt_thread_mdelay(100);
        }
//          if(jg_val<=65)
//          {
//            ppp=1;
//            //左打方向
//            rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*35/1000);
//            stop_flag=1;
//          }

        //左拐，判断角度
        rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*20/1000);

        while(1)
        {
            if(ov_pulse<=50)
            {
                flag4 = 1;
                break;
            }
            rt_thread_mdelay(100);
        }

        rt_thread_mdelay(200);
    }
//    car_stop();
//    rt_pwm_set(right_dev, 1, 1000000, 20*10000);
//    rt_pwm_set(right_dev, 2, 1000000, 20*10000);
//    while(jg_val>=80)
//    {
//        stop_flag=0;
//        rt_thread_mdelay(100);
//    }
//    stop_flag=1;
//    rt_pwm_set(right_dev, 1, 1000000, 30*10000);
//    rt_pwm_set(right_dev, 2, 1000000, 30*10000);
    car_stop();
    return 0;
}

int test_55(void)
{
    char ch='s';
    rt_device_write(ov_uart, -1,&ch , sizeof(ch));
    ov_stop_flag=0;
    car_start();

    int direction_pulse=85;

    //右轮打死右转
    rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*direction_pulse/1000);
    rt_pwm_set(right_dev, 1, 1000000, 40*10000); //右
    rt_pwm_set(right_dev, 2, 1000000, 25*10000); //左
    rt_thread_mdelay(1500);
    //旋转延时
    rt_uint8_t ttt=0;
    if(zhangai<=0) //障碍在右侧
       {
           rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*80/1000);
           rt_kprintf("~80~\n");
       }
    else {        //障碍在左侧
           rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*30/1000);
           rt_kprintf("~40~\n");
           ttt=1;
       }
    //判断障碍物在哪边

    rt_pwm_set(right_dev, 1, 1000000, 30*10000); //右
    rt_pwm_set(right_dev, 2, 1000000, 30*10000); //左
    rt_thread_mdelay(800);
    if(ttt==0)
    {
       rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*30/1000);
    }
    else if (ttt==1) {
        rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*90/1000);
    }
    rt_thread_mdelay(500);
    //最后反方向走一段
    car_stop();
    red_flag=0;
    rt_uint8_t tt=ov_pulse;
    rt_uint8_t fangxiang=0;
    while(1)
    {
       rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*tt/1000);
       if(red_flag==1)
           break;
       if(fangxiang==0)
          tt+=5;
       else if(fangxiang==1)
           tt-=5;
       if(tt>=130)
          fangxiang=1;
       else if(tt<=20)
           fangxiang=0;
       rt_thread_mdelay(300);
    }
    ov_pulse = tt;
    //扫描到红色

    rt_thread_mdelay(50);
    ov_stop_flag=1;
    stop_flag=0;
    car_start();
    while(1)
    {
         TCS34725_GetRawData(&rgb);
         if(rgb.c<1000)
         {
             rt_kprintf("````````\n");
             rt_thread_mdelay(600);
             car_stop();
             break;
         }
         rt_thread_mdelay(50);
    }

    return 0;
}

int test_5(void)
{
    int direction_pulse=85;
    int test_ov = 75;
//    rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*55/1000);
//    rt_thread_mdelay(500);
//    if(argc==4)
//    {
//        direction_pulse = atoi(argv[1]);
//        test_ov = atoi(argv[2]);
//    }
    char ch='s';
    rt_device_write(ov_uart, -1,&ch , sizeof(ch));
    rt_pwm_set(left_dev, 2, speed_period, speed_period *(speed_pulse+10)/100); //30
    rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*direction_pulse/1000);
    car_start();

    ov_stop_flag=0;
    ov_pid_clearn();
    rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*test_ov/1000);
    ov_pulse = test_ov;

    OV_UP;
    rt_thread_mdelay(2500); //旋转延时
    OV_DOWM;
    rt_uint8_t ffff=0;
    if(zhangai<=0) //障碍在右侧
    {
        rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*80/1000);
        rt_kprintf("~80~\n");
    }
    else {        //障碍在左侧
        rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*30/1000);
        rt_kprintf("~40~\n");
        ffff=1;
    }
    rt_thread_mdelay(900);
    if(ffff==1)
      rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*70/1000);
    else if (ffff==0) {
        rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*40/1000);
    }
    rt_pwm_set(left_dev, 2, speed_period, speed_period *speed_pulse/100); //30
    rt_thread_mdelay(1600);
    car_stop();

    red_flag=0;
    rt_uint8_t tt=ov_pulse;
    rt_uint8_t fangxiang=0;
    while(1)
    {
        rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*tt/1000);
        if(red_flag==1)
            break;
        if(fangxiang==0)
           tt+=5;
        else if(fangxiang==1)
            tt-=5;
        if(tt>=130)
           fangxiang=1;
        else if(tt<=20)
            fangxiang=0;
        rt_thread_mdelay(300);
    }
    ov_pulse = tt;
    rt_thread_mdelay(50);
    ov_stop_flag=1;
    stop_flag=0;
    car_start();
//    rt_thread_mdelay(1000);
//
//    ov_stop_flag=1;
//    if(final_stop==1)
//    {
//
//        rt_pwm_set(ov_dev, OV_CHANNEL, ov_period, ov_period*75/1000);
//        ov_pulse = 75;
//    }
    while(1)
    {
        TCS34725_GetRawData(&rgb);
        if(rgb.c<1000)
        {
            rt_kprintf("````````\n");
            rt_thread_mdelay(600);
            car_stop();
            break;
        }
        rt_thread_mdelay(50);
    }


    return 0;
}

//void test_001(int argc,char **argv)
//{
//    int a=speed_pulse;
//    if(argc==2)
//        a = atoi(argv[1]);
//
//    rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*20/1000);
//    rt_pwm_set(left_dev, 2, speed_period, speed_period *0 /100); //30
//    rt_pwm_set(right_dev, 1, speed_period, speed_period *a /100); //30
//    car_start();
//}
//MSH_CMD_EXPORT(test_001,test_001);

rt_thread_t test_thread;
void test_thread_entry(void *parameter)
{
    rt_kprintf("inter test_1\r\n");
    test_1();
    rt_kprintf("inter test_2\r\n");
    test_2();
    rt_kprintf("inter test_3\r\n");
    test_3();
    rt_kprintf("inter test_4\r\n");
    test_4();
//    rt_kprintf("inter test_5\r\n");
    test_55();
}
void test_init(void)
{
    char str='r';
    rt_device_write(ov_uart, -1,&str , sizeof(str));
    test_thread = rt_thread_create("test", test_thread_entry, RT_NULL, 1024, 5, 100);
    if(test_thread)
    {
        rt_thread_startup(test_thread);
    }
    else {
        rt_kprintf("start test thread error\r\n");
    }
}

MSH_CMD_EXPORT(test_1,1_test);
MSH_CMD_EXPORT(test_2,2_test);
MSH_CMD_EXPORT(test_3,3_test);
MSH_CMD_EXPORT(test_4,4_test);
MSH_CMD_EXPORT(test_5,5_test);
