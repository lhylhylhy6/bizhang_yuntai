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

#if CAR_MSH_ENABLE

MSH_CMD_EXPORT(car_stop,car stop);
MSH_CMD_EXPORT(car_start,car forward);
#endif
