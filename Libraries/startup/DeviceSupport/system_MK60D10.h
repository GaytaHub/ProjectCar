/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		system_MK60D10.h
 * @brief      		ʱ�ӳ�ʼ��
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/

#ifndef SYSTEM_MK60D10_H_
#define SYSTEM_MK60D10_H_                       

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
    

 
void Start (void);


void NMI_Handler(void);

#ifdef __cplusplus
}
#endif

#endif 
