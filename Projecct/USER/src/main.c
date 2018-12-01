#include "headfile.h"

//Unit Test//
// #define  MotorTest
// #define  SteeringTest
// #define  EncoderTest
// #define TrackTest
// #define SensorTset
// #define CoordinateTest
// #define CameraTest

//test2
int main(void)
{
    get_clk();
    
#ifdef MotorTest
    Motor_Test();
#endif
    
#ifdef SteeringTest
    Steering_Test();
#endif
    
#ifdef EncoderTest
    Encoder_Test();
#endif
    
#ifdef SensorTset
    GraySensor_Tset();
#endif
    
#ifdef CoordinateTest
    Coordinate_Test();
#endif
    
#ifdef CameraTest
    Camera_Test();
#endif
    
#ifdef TrackTest
    Motor_Init();
    Encoder_Init();
    Camera_Init();
    pit_init_ms(pit0, 10);
#endif
    
    for (;;)
    {
        ;
    }
}

//中断服务函数
void PIT0_IRQHandler(void)
{
    PIT_FlAG_CLR(pit0);
#ifdef TrackTest
    Track('X', '+', False);
    // Track('X', '-', False);
    // Track('Y', '+', False);
    // Track('Y', '-', False);
#endif
}

//PORTC中断标志位清除
void PORTC_IRQHandler(void)
{
    //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
    //PORTC->ISFR = 0xffffffff;
    //使用我们编写的宏定义清除发生中断的引脚
    //PORTC_FLAG_CLR(C1);
    
    //此中断的标志位在ov7725_vsync中清除，因此不需要再这里清除
    ov7725_vsync();
}

//DMA0中断清除
void DMA0_IRQHandler(void)
{
    DMA_IRQ_CLEAN(DMA_CH0);	
}