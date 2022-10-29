/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-22     lhylh       the first version
 */
#ifndef APPLICATIONS_HCSR04_H_
#define APPLICATIONS_HCSR04_H_

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

rt_err_t HCSR_mid_init(void);
rt_err_t HCSR_forward_init(void);
rt_err_t HCSR_back_init(void);

#endif /* APPLICATIONS_HCSR04_H_ */
