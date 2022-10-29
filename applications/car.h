/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-28     10091       the first version
 */
#ifndef APPLICATIONS_CAR_H_
#define APPLICATIONS_CAR_H_

#include"rtthread.h"
#include"rtdevice.h"
#include"board.h"

#define AIN1_PIN GET_PIN(F, 6)
#define AIN2_PIN GET_PIN(F, 7)
#define BIN1_PIN GET_PIN(F, 8)
#define BIN2_PIN GET_PIN(F, 9)

int car_init(void);
int car_stop(void);
int car_start(void);

#endif /* APPLICATIONS_CAR_H_ */
