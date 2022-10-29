/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-29     10091       the first version
 */
#ifndef APPLICATIONS_OV_PID_H_
#define APPLICATIONS_DIRECTION_PID_H_

#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"

int angle_limit(rt_int32_t *angle);
int direction_pid_compute(rt_int32_t *val);

#endif /* APPLICATIONS_OV_PID_H_ */
