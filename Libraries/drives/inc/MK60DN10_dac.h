/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60DN10_dac.h
 * @brief      		DAC������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/
 
 
 
#ifndef _MK60DN10_dac_h
#define _MK60DN10_dac_h


#include "misc.h"
#include "MK60D10.h"
#include "common.h"
#include "MK60DN10_port.h"
#include "MK60DN10_gpio.h"
#include "MK60DN10_port_cfg.h"

typedef enum DACn       //DACģ��
{
    dac0,
    dac1
} DACn;


void dac_init(DACn dacn,uint16 volta);
void dac_out(DACn dacn, uint16 volta);

#endif
