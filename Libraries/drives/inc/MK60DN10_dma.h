/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60DN10_dma.h
 * @brief      		DMA������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/
 

 
#ifndef _MK60DN10_dma_h
#define _MK60DN10_dma_h


#include "misc.h"
#include "MK60D10.h"
#include "common.h"
#include "MK60DN10_port.h"
#include "MK60DN10_gpio.h"
#include "MK60DN10_port_cfg.h"

typedef enum
{
    //        ����ͨ��            
    Channel_Disabled        = 0,

    //        UART            
    DMA_UART0_Rx            = 2,
    DMA_UART0_Tx            = 3,
    DMA_UART1_Rx            = 4,
    DMA_UART1_Tx            = 5,
    DMA_UART2_Rx            = 6,
    DMA_UART2_Tx            = 7,
    DMA_UART3_Rx            = 8,
    DMA_UART3_Tx            = 9,
    DMA_UART4_Rx            = 10,
    DMA_UART4_Tx            = 11,
    DMA_UART5_Rx            = 12,
    DMA_UART5_Tx            = 13,

    //        I2S            
    DMA_I2S0_Rx             = 14,
    DMA_I2S0_Tx             = 15,

    //        SPI            
    DMA_SPI0_Rx             = 16,
    DMA_SPI0_Tx             = 17,
    DMA_SPI1_Rx             = 18,
    DMA_SPI1_Tx             = 19,

    //        I2C            
    DMA_I2C0                = 22,
    DMA_I2C1                = 23,

    //        FTM            
    DMA_FTM0_CH0            = 24,
    DMA_FTM0_CH1            = 25,
    DMA_FTM0_CH2            = 26,
    DMA_FTM0_CH3            = 27,
    DMA_FTM0_CH4            = 28,
    DMA_FTM0_CH5            = 29,
    DMA_FTM0_CH6            = 30,
    DMA_FTM0_CH7            = 31,

    DMA_FTM1_CH0            = 32,
    DMA_FTM1_CH1            = 33,

    DMA_FTM2_CH0            = 34,
    DMA_FTM2_CH1            = 35,

    DMA_FTM3_CH0            = 36,
    DMA_FTM3_CH1            = 37,
    DMA_FTM3_CH2            = 38,

    DMA_FTM1_CH3            = 39,

    //     ADC/DAC/CMP/CMT    
    DMA_ADC0                = 40,
    DMA_ADC1                = 41,
    DMA_CMP0                = 42,
    DMA_CMP1                = 43,
    DMA_CMP2                = 44,
    DMA_DAC0                = 45,
    DMA_DAC1                = 46,
    DMA_CMT                 = 47,

    DMA_PDB                 = 48,

    DMA_PORTA              = 49,
    DMA_PORTB              = 50,
    DMA_PORTC              = 51,
    DMA_PORTD              = 52,
    DMA_PORTE              = 53,

    DMA_FTM3_CH4            = 54,
    DMA_FTM3_CH5            = 55,
    DMA_FTM3_CH6            = 56,
    DMA_FTM3_CH7            = 57,

    DMA_Always_EN1          = 58,
    DMA_Always_EN2          = 59,
    DMA_Always_EN3          = 60,
    DMA_Always_EN4          = 61,
    DMA_Always_EN5          = 62,
    DMA_Always_EN6          = 63,
} DMA_sources;


typedef enum
{
    DADDR_RECOVER = 0,        //�ָ�Ŀ�ĵ�ַ
    DADDR_KEEPON  = 1,        //Ŀ�ĵ�ַ���ֲ���
} DMA_cfg;

typedef enum      //DMAÿ�δ����ֽ���
{
    DMA_BYTE1 = 0,
    DMA_BYTE2 = 1,
    DMA_BYTE4 = 2,
    DMA_BYTE16 = 4
} DMA_BYTEn;

typedef enum DMA_PORTx2BUFF_cfg
{
    DMA_rising          = 0x01u,            //�����ش���
    DMA_falling         = 0x02u,            //�½��ش���
    DMA_either          = 0x03u,            //�����ش���

    //�������λ��־����������
    DMA_rising_down     = 0x81u,            //�����ش�����Դ��ַIO�˿��ڲ�����
    DMA_falling_down    = 0x82u,            //�½��ش�����Դ��ַIO�˿��ڲ�����
    DMA_either_down     = 0x83u,            //�����ش�����Դ��ַIO�˿��ڲ�����

    DMA_rising_up       = 0xc1u,            //�����ش�����Դ��ַIO�˿��ڲ�����
    DMA_falling_up      = 0xc2u,            //�½��ش�����Դ��ַIO�˿��ڲ�����
    DMA_either_up       = 0xc3u,            //�����ش�����Դ��ַIO�˿��ڲ�����

    //��λ6����־�����������,Ŀ�ĵ�ַ���ֲ��䣬���ָ���ԭ����ַ
    DMA_rising_keepon          = 0x21u,     //�����ش���                      ��Ŀ�ĵ�ַ���ֲ���
    DMA_falling_keepon         = 0x22u,     //�½��ش���                      ��Ŀ�ĵ�ַ���ֲ���
    DMA_either_keepon          = 0x23u,     //�����ش���                      ��Ŀ�ĵ�ַ���ֲ���

    DMA_rising_down_keepon     = 0xA1u,     //�����ش�����Դ��ַIO�˿��ڲ�������Ŀ�ĵ�ַ���ֲ���
    DMA_falling_down_keepon    = 0xA2u,     //�½��ش�����Դ��ַIO�˿��ڲ�������Ŀ�ĵ�ַ���ֲ���
    DMA_either_down_keepon     = 0xA3u,     //�����ش�����Դ��ַIO�˿��ڲ�������Ŀ�ĵ�ַ���ֲ���

    DMA_rising_up_keepon       = 0xF1u,     //�����ش�����Դ��ַIO�˿��ڲ�������Ŀ�ĵ�ַ���ֲ���
    DMA_falling_up_keepon      = 0xF2u,     //�½��ش�����Դ��ַIO�˿��ڲ�������Ŀ�ĵ�ַ���ֲ���
    DMA_either_up_keepon       = 0xF3u,     //�����ش�����Դ��ַIO�˿��ڲ�������Ŀ�ĵ�ַ���ֲ���

} DMA_PORTx2BUFF_cfg, DMA_Count_cfg;

typedef enum
{
    DMA_CH0,
    DMA_CH1,
    DMA_CH2,
    DMA_CH3,
    DMA_CH4,
    DMA_CH5,
    DMA_CH6,
    DMA_CH7,
    DMA_CH8,
    DMA_CH9,
    DMA_CH10,
    DMA_CH11,
    DMA_CH12,
    DMA_CH13,
    DMA_CH14,
    DMA_CH15
} DMA_CHn;

#define  DMA_IRQ_EN(DMA_CHn)    enable_irq((IRQn_Type)((IRQn_Type)DMA_CHn + DMA0_IRQn))               //����DMAͨ����������ж�
#define  DMA_IRQ_DIS(DMA_CHn)   disable_irq((IRQn_Type)((IRQn_Type)DMA_CHn + DMA0_IRQn))              //��ֹDMAͨ����������ж�

#define  DMA_IRQ_CLEAN(DMA_CHn) DMA0->INT|=(DMA_INT_INT0_MASK<<DMA_CHn)           //���ͨ�������жϱ�־λ

#define  DMA_EN(DMA_CHn)        DMA0->ERQ |= (DMA_ERQ_ERQ0_MASK<<(DMA_CHn))       //ʹ��ͨ��Ӳ��DMA����
#define  DMA_DIS(DMA_CHn)       DMA0->ERQ &=~(DMA_ERQ_ERQ0_MASK<<(DMA_CHn))       //��ֹͨ��Ӳ��DMA����

#define  DMA_CHn_DIS(DMA_CHn)   DMAMUX->CHCFG[DMA_CHn] &= ~DMAMUX_CHCFG_SOURCE(Channel_Disabled)  //����ͨ��



//DMA �������ã�������Ϻ�һЩ������ı䣬��Ҫ���¸�ֵ
__STATIC_INLINE void dma_repeat(DMA_CHn CHn,void *SADDR, void *DADDR,uint32 count)//ʹ��__STATIC_INLINEΪ�˽���κ�����Ƕ��ʹ�øú����ĵط����������Լ��ٺ������õ�ʱ��
{
    DMA_IRQ_CLEAN(CHn);
    DMA0->TCD[CHn].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(count);  //��ǰ��ѭ������ ��ǰѭ�������ľ����ٴμ�����ʼ��ѭ������
    DMA0->TCD[CHn].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(count);  //��ʼ��ѭ������

    DMA0->TCD[CHn].SADDR = (uint32)SADDR;                           // ����  Դ��ַ
    DMA0->TCD[CHn].DADDR = (uint32)DADDR;                           // ����Ŀ�ĵ�ַ

    DMA_EN(DMA_CH0);                                                //ʹ��DMA Ӳ������

}


extern void dma_portx2buff_init(DMA_CHn CHn, void *SADDR, void *DADDR, PTX_n ptxn, DMA_BYTEn byten, uint32 count, uint32 cfg);
extern void DMA_count_Init(DMA_CHn CHn, PTX_n ptxn, uint32 count, DMA_PORTx2BUFF_cfg cfg);
extern uint32  DMA_count_get(DMA_CHn CHn);
extern void DMA_count_reset(DMA_CHn CHn);


#endif
