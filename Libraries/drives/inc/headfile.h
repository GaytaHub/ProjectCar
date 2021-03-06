/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2016,逐飞科技
* All rights reserved.
* 技术讨论QQ群：179029047
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file       		headfile.h
* @brief      		头文件汇总库
* @company	   		成都逐飞科技有限公司
* @author     		Go For It(1325536866)
* @version    		v1.0
* @Software 		IAR 7.7 or MDK 5.17
* @Target core		MK60DN512VLL10
* @Taobao   		https://seekfree.taobao.com/
* @date       		2016-02-25
********************************************************************************************************************/

#ifndef _headfile_h
#define _headfile_h

#include "MK60D10.h"
#include "MK60DN10_port_cfg.h"
#include "common.h"
#include "misc.h"

//--------函数库--------
#include "MK60DN10_rtc.h"
#include "MK60DN10_gpio.h"
#include "MK60DN10_systick.h"
#include "MK60DN10_port.h"
#include "MK60DN10_uart.h"
#include "MK60DN10_pit.h"
#include "MK60DN10_lptmr.h"
#include "MK60DN10_ftm.h"
#include "MK60DN10_adc.h"
#include "MK60DN10_dac.h"
#include "MK60DN10_flash.h"
#include "MK60DN10_spi.h"
#include "MK60DN10_i2c.h"
#include "MK60DN10_wdog.h"
#include "MK60DN10_dma.h"
#include "MK60DN10_cmt.h"
#include "MK60DN10_sdhc.h"

#include "isr.h"

//fatfs文件系统
#include "ff.h"

//--------逐飞科技产品例程库--------
#include "SEEKFREE_MT9V032.h"
#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_FUN.h"
#include "SEEKFREE_TSL1401.h"
#include "SEEKFREE_7725.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_NRF24L01.h"
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_L3G4200D.h"
#include "SEEKFREE_MMA8451.h"
#include "SEEKFREE_MPU6050.h"

//用户头文件
#include "Motor.h"
#include "Steering.h"
#include "Encoder.h"
#include "PID.h"
#include "Parameter.h"
#include "Track.h"
#include "Coordinate.h"
#include "GraySensor.h"
#include "Camera.h"

//PID结构体结构
struct Data
{
    float P; //比例项
    float I; //积分项
    float D; //微分项
};

#define True 1
#define False 0

#define White 1
#define Black 0

#endif
