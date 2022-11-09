/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-29     10091       the first version
 */
#ifndef APPLICATIONS_UART2_H_
#define APPLICATIONS_UART2_H_

#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"

#define ov_uart_name "uart2"

int uart2_send_down(void);
int uart2_send_up(void);

#define OV_DOWM uart2_send_down()
#define OV_UP   uart2_send_up()

rt_err_t ov_uart_init(void);

#endif /* APPLICATIONS_UART2_H_ */
