#include "headfile.h"

//编码器方向读取接口
#define XPositiveDirection gpio_get(B10)
#define YPositiveDirection gpio_get(B11)
#define XNegativeDirection gpio_get(B16)
#define YNegativeDirection gpio_get(B17)

/*********************编码器初始化***********************/
void Encoder_Init(void)
{
    //编码器脉冲接口
    DMA_count_Init(DMA_CH0, A5, 0x7FFF, DMA_falling_keepon);
    DMA_count_Init(DMA_CH1, B9, 0x7FFF, DMA_falling_keepon);
    DMA_count_Init(DMA_CH3, D0, 0x7FFF, DMA_falling_keepon);
    DMA_count_Init(DMA_CH4, E2, 0x7FFF, DMA_falling_keepon);

    //编码器方向接口
    gpio_init(B10, GPI, 0);
    gpio_init(B11, GPI, 0);
    gpio_init(B16, GPI, 0);
    gpio_init(B17, GPI, 0);
}

/*********************X+方向轮子编码器读取***********************/
int16 XPositive_Encoder_Get(void)
{
    int16 Pulse = 0;
    if (XPositiveDirection)
    {
        Pulse = DMA_count_get(DMA_CH0);
    }
    else
    {
        Pulse = -DMA_count_get(DMA_CH0);
    }
    DMA_count_reset(DMA_CH0);
    return Pulse;
}

/**********************Y+方向轮编码器读取**********************/
int16 YPositive_Encoder_Get(void)
{
    int16 Pulse = 0;
    if (YPositiveDirection)
    {
        Pulse = DMA_count_get(DMA_CH1);
    }
    else
    {
        Pulse = -DMA_count_get(DMA_CH1);
    }
    DMA_count_reset(DMA_CH1);
    return Pulse;
}

/******************X-方向轮编码器读取**************************/
int16 XNegative_Encoder_Get(void)
{
    int16 Pulse = 0;
    if (XNegativeDirection)
    {
        Pulse = DMA_count_get(DMA_CH3);
    }
    else
    {
        Pulse = -DMA_count_get(DMA_CH3);
    }
    DMA_count_reset(DMA_CH3);
    return Pulse;
}

/********************Y-方向轮编码器读取************************/
int16 YNegative_Encoder_Get(void)
{
    int16 Pulse = 0;
    if (YNegativeDirection)
    {
        Pulse = DMA_count_get(DMA_CH4);
    }
    else
    {
        Pulse = - DMA_count_get(DMA_CH4);
    }
    DMA_count_reset(DMA_CH4);
    return Pulse;
}

/******************编码器测试**************************/
int16 Pulse0, Pulse1, Pulse2, Pulse3;
int8 Dir0, Dir1, Dir2, Dir3;
void Encoder_Test(void)
{
    Encoder_Init();
    Motor_Init();
    pit_init_ms(pit0, 10);
    set_irq_priority(PIT0_IRQn, 2); //设置优先级,根据自己的需求设置
    enable_irq(PIT0_IRQn);          //打开pit0的中断开关
    EnableInterrupts;
    for (;;)
    {
        XPositive_Motor_Control(300);
        YPositive_Motor_Control(-300);
        XNegative_Motor_Control(300);
        YNegative_Motor_Control(-300);
        Dir0 = XPositiveDirection;
        Dir1 = YPositiveDirection;
        Dir2 = XNegativeDirection;
        Dir3 = YNegativeDirection;
        Pulse0 = XPositive_Encoder_Get();
        Pulse1 = YPositive_Encoder_Get();
        Pulse2 = XNegative_Encoder_Get();
        Pulse3 = YNegative_Encoder_Get();
        systick_delay_ms(50);
    }
}
