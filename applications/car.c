/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-28     10091       the first version
 */
#include "car.h"

#define DBG_TAG "car"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define CAR_MSH_ENABLE 1

extern rt_uint32_t speed_period,speed_pulse;
extern rt_uint32_t direction_period,direction_pulse;
extern struct rt_device_pwm *left_dev;
extern struct rt_device_pwm *right_dev;
extern struct rt_device_pwm *direction_dev;
int car_init(void)
{
    rt_err_t ret = RT_EOK;

    rt_pin_mode(AIN1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(AIN2_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(BIN1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(BIN2_PIN, PIN_MODE_OUTPUT);

    rt_pin_write(AIN1_PIN, PIN_LOW);
    rt_pin_write(AIN2_PIN, PIN_LOW);
    rt_pin_write(BIN1_PIN, PIN_LOW);
    rt_pin_write(BIN2_PIN, PIN_LOW);

    return ret;
}

int car_start(void)
{
    rt_pin_write(AIN1_PIN, PIN_HIGH);
    rt_pin_write(AIN2_PIN, PIN_LOW);
    rt_pin_write(BIN1_PIN, PIN_HIGH);
    rt_pin_write(BIN2_PIN, PIN_LOW);
    my_pwm_enable();
    return RT_EOK;
}

int car_stop(void)
{
    rt_pin_write(AIN1_PIN, PIN_LOW);
    rt_pin_write(AIN2_PIN, PIN_LOW);
    rt_pin_write(BIN1_PIN, PIN_LOW);
    rt_pin_write(BIN2_PIN, PIN_LOW);
    my_pwm_disable();
    rt_kprintf("now car is stop\r\n");
    return RT_EOK;
}

int car_left(void)
{
    rt_pin_write(AIN1_PIN, PIN_HIGH);
    rt_pin_write(AIN2_PIN, PIN_LOW);
    rt_pin_write(BIN1_PIN, PIN_HIGH);
    rt_pin_write(BIN2_PIN, PIN_LOW);
    rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*30/1000);
    my_pwm_set(RIGHT_CHANNEL, speed_pulse+10);
    my_pwm_set(LEFT_CHANNEL, speed_pulse-10);
    my_pwm_enable();
    return 0;
}

int car_right(void)
{
    rt_pin_write(AIN1_PIN, PIN_HIGH);
    rt_pin_write(AIN2_PIN, PIN_LOW);
    rt_pin_write(BIN1_PIN, PIN_HIGH);
    rt_pin_write(BIN2_PIN, PIN_LOW);
    rt_pwm_set(direction_dev, DIRECTION_CHANNEL, direction_period, direction_period*120/1000);
    my_pwm_set(RIGHT_CHANNEL, speed_pulse-10);
    my_pwm_set(LEFT_CHANNEL, speed_pulse+10);
    my_pwm_enable();
    return 0;
}

int car_speed_set(int argc,char **argv)
{
    if(argc==2)
    {
        speed_pulse = atoi(argv[1]);
        my_pwm_set(RIGHT_CHANNEL, speed_pulse);
        my_pwm_set(LEFT_CHANNEL, speed_pulse);
        rt_kprintf("set pulse to %d\n",speed_pulse);
    }
    else
    {
        rt_kprintf("format error!\n");
    }
    return 0;
}

#if CAR_MSH_ENABLE

MSH_CMD_EXPORT(car_stop,car stop);
MSH_CMD_EXPORT(car_start,car forward);
MSH_CMD_EXPORT(car_speed_set,car_speed_set);
MSH_CMD_EXPORT(car_left,car_left);
MSH_CMD_EXPORT(car_right,car_right);
#endif
