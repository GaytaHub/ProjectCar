#include "headfile.h"

/*计线传感器组为四个一组，成正方形排列，经过黑线时，
以同一行或者列上的两个传感器同时触发为一个稳定触发状态，
再没有经过黑线时，四个传感器都处于没有触发的状态，
经过黑线时，首先会有一排传感器被触发，这时认为车体处于坐标点上，坐标加0.5，
继续向前运动，另一排传感器被触发，由于车体比格子的长和宽小，
因此传感器又处于未触发状态，此时认为车子处于坐标过度状态，坐标加0.5。
设起始位置坐标为（0.5，0.5）的过渡状态，当坐标为整数时，说明车体在其整数点的垂直线上，
坐标为小数时，说明车体在两整数坐标垂线的中间。由此完成对车体大致位置的确定*/

/*灰度传感器组状态定义
   ^    车头方向
   |
2————1
|    |——>摄像头视野方向
|    |
3————4
传感器读到黑线返回Black,否则返回White
传感器组未触发时，函数返回0，
触发一组时，小数在前大数在后返回被触发的传感器组*/
int8 Sensor_Trigger(void)
{
    static uint8 Sensor_Grop[2][2] = {{White, White},
                                      {White, White}}; //传感器状态
    //按照1，2，3，4象限的顺序读取
    Sensor_Grop[0][1] = Gray_Sensor1_Read();
    Sensor_Grop[0][0] = Gray_Sensor2_Read();
    Sensor_Grop[1][0] = Gray_Sensor3_Read();
    Sensor_Grop[1][1] = Gray_Sensor4_Read();

    if (Sensor_Grop[0][1] == Black &&
        Sensor_Grop[0][0] == Black &&
        Sensor_Grop[1][0] == White &&
        Sensor_Grop[1][1] == White)
        return 12;
    else if (Sensor_Grop[0][1] == Black &&
             Sensor_Grop[0][0] == White &&
             Sensor_Grop[1][0] == White &&
             Sensor_Grop[1][1] == Black)
        return 14;
    else if (Sensor_Grop[0][1] == White &&
             Sensor_Grop[0][0] == Black &&
             Sensor_Grop[1][0] == Black &&
             Sensor_Grop[1][1] == White)
        return 23;
    else if (Sensor_Grop[0][1] == White &&
             Sensor_Grop[0][0] == White &&
             Sensor_Grop[1][0] == Black &&
             Sensor_Grop[1][1] == Black)
        return 34;
    else
        return 0;
}

//横坐标记录
float X_Coordinate(void)
{
    static float X_Position = 0.5; //起始横坐标设为:0.5

    if (Sensor_Trigger() == 14 ||
        Sensor_Trigger() == 23)
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

    return X_Position;
}

//纵坐标记录
float Y_Coordinate(void)
{
    static float Y_Position = 0.5; //起始纵坐标设置为0.5

    if (Sensor_Trigger() == 12 ||
        Sensor_Trigger() == 34)
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

    return Y_Position;
}

//坐标记录测试
float X,Y;
void Coordinate_Test(void)
{
    GraySensor_Init();
    for(;;)
    {
        X = X_Coordinate();
        Y = Y_Coordinate();
    }
}
