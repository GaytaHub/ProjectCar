/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		common.c
 * @brief      		common������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/



#include "common.h"
#include "MK60D10.h"
#include "MK60DN10_uart.h"


#define EX_REF_CLK  50 //(�����ⲿ�ο�ʱ��Ϊ50MHZ)
uint32 mcgout_clk_mhz = 100;
uint32 core_clk_mhz = 100;
uint32 bus_clk_mhz = 100;

void get_clk(void)
{
    mcgout_clk_mhz = EX_REF_CLK * ((MCG->C6 & MCG_C6_VDIV0_MASK) + 24) / ((MCG->C5 & MCG_C5_PRDIV0_MASK) + 1);
    core_clk_mhz = mcgout_clk_mhz / ((SIM->CLKDIV1 >> SIM_CLKDIV1_OUTDIV1_SHIFT) + 1);
    bus_clk_mhz = mcgout_clk_mhz / (((SIM->CLKDIV1 >> SIM_CLKDIV1_OUTDIV2_SHIFT) & 0x0f) + 1);
    
    
    
    uart_init(DEBUG_PORT,DEBUG_BAUD);   //��ʼ�����Դ��ڣ������ʹ��printf��������

}


