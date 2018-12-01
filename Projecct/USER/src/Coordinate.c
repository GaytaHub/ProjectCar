#include "headfile.h"

/*计线传感器组为四个一组，成正方形排列，经过黑线时，
以同一行或者列上的两个传感器同时触发为一个稳定触发状态，
再没有经过黑线时，四个传感器都处于没有触发的状态，
经过黑线时，首先会有一排传感器被触发，这时认为车体处于坐标点上，坐标加0.5，
继续向前运动，另一排传感器被触发，由于车体比格子的长和宽小，
因此传感器又处于未触发状态，此时认为车子处于坐标过度状态，坐标加0.5。
设起始位置坐标为（0.5，0.5）的过渡状态，当坐标为整数时，说明车体在其整数点的垂直线上，
坐标为小数时，说明车体在两整数坐标垂线的中间。由此完成对车体大致位置的确定*/

//横坐标记录
void X_Coordinate(void)
{
    if (Sensor_Trigger() == 14 || Sensor_Trigger() == 23)
    {
        do
        {
            ; //等待触发结束
        } while (Sensor_Trigger() == 0);

        //判断速度方向以判断坐标加减
        if (X_Speed_Real > 0)
            X_Position += 0.5;
        else if (X_Speed_Real < 0)
            X_Position -= 0.5;
    }
}

//纵坐标记录
void Y_Coordinate(void)
{
    if (Sensor_Trigger() == 12 || Sensor_Trigger() == 34)
    {
        systick_delay_ms(250); //做长延时以跳过双线中间的白色
        do
        {
            ; //等待触发结束
        } while (Sensor_Trigger() == 0);

        //判断速度方向以判断坐标加减
        if (Y_Speed_Real > 0)
            Y_Position += 0.5;
        else if (Y_Speed_Real < 0)
            Y_Position -= 0.5;
    }
}

//坐标记录测试
void Coordinate_Test(void)
{
    GraySensor_Init();
    for (;;)
    {
        X_Coordinate();
        Y_Coordinate();
    }
}
