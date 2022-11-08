/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-28     10091       the first version
 */
#ifndef APPLICATIONS_MY_PWM_H_
#define APPLICATIONS_MY_PWM_H_

#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"
#include "stdlib.h"
#include "string.h"

#define LEFT_PWM           "pwm3"
#define LEFT_CHANNEL         1
#define RIGHT_PWM          "pwm3"
#define RIGHT_CHANNEL        2

#define OV_PWM             "pwm2"
#define OV_CHANNEL           1
#define DIRECTION_PWM      "pwm2"
#define DIRECTION_CHANNEL    2
#define DOWN_PWM           "pwm2"
#define DOWN_CHANNEL         3


//channel3 mid110 xiazai 90


int my_pwm_init(void);
int my_pwm_enable(void);
int my_pwm_disable(void);
int my_pwm_set(int channel,rt_int32_t new_pulse);

#endif /* APPLICATIONS_MY_PWM_H_ */
