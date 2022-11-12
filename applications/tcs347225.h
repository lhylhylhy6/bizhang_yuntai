/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-10     10091       the first version
 */
#ifndef APPLICATIONS_TCS347225_H_
#define APPLICATIONS_TCS347225_H_

#include "board.h"
#include "rtthread.h"
#include "rtdevice.h"
#define u8 rt_uint8_t
#define u16 rt_uint16_t
typedef struct{
    unsigned short  c;      //[0-65536]
    unsigned short  r;
    unsigned short  g;
    unsigned short  b;
}COLOR_RGBC;//RGBC

typedef struct{
    unsigned short h;       //[0,360]
    unsigned char  s;       //[0,100]
    unsigned char  l;       //[0,100]
}COLOR_HSL;//HSL

void TCS34725_I2C_Init();
void TCS34725_I2C_Start();
void TCS34725_I2C_Stop();
u8 TCS34725_I2C_Wait_ACK();
void TCS34725_I2C_ACK();
void TCS34725_I2C_NACK();
void TCS34725_I2C_Send_Byte(u8 byte);
u8 TCS34725_I2C_Read_Byte(u8 ack);
void TCS34725_I2C_Write(u8 slaveAddress, u8* dataBuffer,u8 bytesNumber, u8 stopBit);
void TCS34725_I2C_Read(u8 slaveAddress, u8* dataBuffer, u8 bytesNumber, u8 stopBit);
void TCS34725_Write(u8 subAddr, u8* dataBuffer, u8 bytesNumber);
void TCS34725_Read(u8 subAddr, u8* dataBuffer, u8 bytesNumber);
void TCS34725_SetIntegrationTime(u8 time);
void TCS34725_SetGain(u8 gain);
void TCS34725_Enable(void);
void TCS34725_Disable(void);
u8 TCS34725_Init(void);
u16 TCS34725_GetChannelData(u8 reg);
u8 TCS34725_GetRawData(COLOR_RGBC *rgbc);
void RGBtoHSL(COLOR_RGBC *Rgb, COLOR_HSL *Hsl);
#endif /* APPLICATIONS_TCS347225_H_ */
