#include "headfile.h"

//编码器方向读取接口
#define LFDirection gpio_get(A14)
#define RFDirection gpio_get(A15)
#define LBDirection gpio_get(A16)
#define RBDirection gpio_get(A17)

//编码器初始化
void Encoder_Init(void)
{
    //编码器脉冲接口
    DMA_count_Init(DMA_CH0, A5, 0x7FFF, DMA_falling_keepon);
    DMA_count_Init(DMA_CH1, B10, 0x7FFF, DMA_falling_keepon);
    DMA_count_Init(DMA_CH3, D0, 0x7FFF, DMA_falling_keepon);
    DMA_count_Init(DMA_CH4, E2, 0x7FFF, DMA_falling_keepon);
    
    //编码器方向接口
    gpio_init(A14, GPI, 0);
    gpio_init(A15, GPI, 0);
    gpio_init(A16, GPI, 0);
    gpio_init(A17, GPI, 0);
}

//左前轮编码器读取
int16 LF_Encoder_Get(void)
{
    int16 Pulse = 0;
    if (LFDirection)
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

//右前轮编码器读取
int16 RF_Encoder_Get(void)
{
    int16 Pulse = 0;
    if (RFDirection)
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

//左后轮编码器读取
int16 LB_Encoder_Get(void)
{
    int16 Pulse = 0;
    if (LBDirection)
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

//右后轮编码器读取
int16 RB_Encoder_Get(void)
{
    int16 Pulse = 0;
    if (gpio_get(A17))
    {
        Pulse = DMA_count_get(DMA_CH4);
    }
    else
    {
        Pulse = -DMA_count_get(DMA_CH4);
    }
    DMA_count_reset(DMA_CH4);
    return Pulse;
}

//编码器测试
int16 Pulse0, Pulse1, Pulse2, Pulse3;
int8 Dir0, Dir1, Dir2, Dir3;
void Encoder_Test(void)
{
    Encoder_Init();
    pit_init_ms(pit0, 10);
    set_irq_priority(PIT0_IRQn,2);						//设置优先级,根据自己的需求设置
    enable_irq(PIT0_IRQn);								//打开pit0的中断开关
    EnableInterrupts;
    for (;;)
    {
        Dir0 = LFDirection;
        Dir1 = RFDirection;
        Dir2 = LBDirection;
        Dir3 = RBDirection;
        Pulse0 = LF_Encoder_Get();
        Pulse1 = RF_Encoder_Get();
        Pulse2 = LB_Encoder_Get();
        Pulse3 = RB_Encoder_Get();
        systick_delay_ms(10);
    }
}
