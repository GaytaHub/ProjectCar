/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60DN10_lptmr.h
 * @brief      		�͹��Ķ�ʱ��������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/
 
 
 
#ifndef _MK60DN10_lptmr_h
#define _MK60DN10_lptmr_h


#include "misc.h"
#include "MK60D10.h"
#include "common.h"
#include "MK60DN10_port.h"
#include "MK60DN10_gpio.h"
#include "MK60DN10_port_cfg.h"


typedef enum
{
    lpt0_alt1 = 1,      // A19
    lpt0_alt2 = 2       // C5
} LPT0_ALTn;

typedef enum
{
    LPT_Rising  = 0,    //�����ش���
    LPT_Falling = 1     //�½��ش���
} LPT_CFG;


#define LPTMR_Flag_Clear()  (LPTMR0->CSR |= LPTMR_CSR_TCF_MASK)         //���LPT�Ƚϱ�־λ




void lptmr_pulse_init(LPT0_ALTn altn, uint16 count, LPT_CFG cfg);
uint16 lptmr_pulse_get(void);
void lptmr_pulse_clean(void);


#endif
