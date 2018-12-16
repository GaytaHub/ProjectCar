#include "headfile.h"

//Unit Test//
//#define MotorTest
// #define SteeringTest
// #define EncoderTest
  #define TrackTest
// #define SensorTset
// #define CoordinateTest
//#define CameraTest
// #define CoordinateCommandTest

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
    OLED_Init();
    pit_init_ms(pit0, 20);
    set_irq_priority(PIT0_IRQn, 1); //设置优先级,根据自己的需求设置
    enable_irq(PIT0_IRQn);          //打开pit0的中断开关
    EnableInterrupts;
#endif

#ifdef CoordinateCommandTest
    pit_init_ms(pit0, 50);
    set_irq_priority(PIT0_IRQn, 2); //设置优先级,根据自己的需求设置
    enable_irq(PIT0_IRQn);          //打开pit0的中断开关
    EnableInterrupts;
    //重置初始起点
    XPosition = 1;
    YPosition = 1;
    Coordinate_Test();
#endif

    for (;;)
    {
        dis_bmp(60, 80, image_dec[0], 0x7F);
    }
}

//中断服务函数
void PIT0_IRQHandler(void)
{
    PIT_FlAG_CLR(pit0);
#ifdef TrackTest
    Track(6, 0, False);
#endif

#ifdef CoordinateCommandTest
    Coordinate_Command(3, 4);
#endif
}


//PORTC中断标志位清除
void PORTC_IRQHandler(void)
{
    //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
    //PORTC->ISFR = 0xffffffff;
    //使用我们编写的宏定义清除发生中断的引脚
//    PORTC_FLAG_CLR(C1);

    //此中断的标志位在ov7725_vsync中清除，因此不需要再这里清除
    ov7725_vsync();
}


//DMA2中断清除
void DMA2_IRQHandler(void)
{
    ov7725_dma();
}

