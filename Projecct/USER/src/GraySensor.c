#include "headfile.h"

//以象限定义计线传感器位置
#define GraySensor1 B0
#define GraySensor2 B1
#define GraySensor3 B2
#define GraySensor4 B3

//灰度传感器读取接口初始化
void GraySensor_Init(void)
{
    gpio_init(GraySensor1, GPI, White);
    gpio_init(GraySensor2, GPI, White);
    gpio_init(GraySensor3, GPI, White);
    gpio_init(GraySensor4, GPI, White);
}

//灰度传感器二值化读取，黑线为0
uint8 Gray_Sensor1_Read(void)
{
    return gpio_get(GraySensor1);
}

uint8 Gray_Sensor2_Read(void)
{
    return gpio_get(GraySensor2);
}

uint8 Gray_Sensor3_Read(void)
{
    return gpio_get(GraySensor3);
}

uint8 Gray_Sensor4_Read(void)
{
    return gpio_get(GraySensor4);
}

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
    static uint8 SensorGrop[2][2] = {{White, White},
                                     {White, White}}; //传感器状态
    //按照1，2，3，4象限的顺序读取
    SensorGrop[0][1] = Gray_Sensor1_Read();
    SensorGrop[0][0] = Gray_Sensor2_Read();
    SensorGrop[1][0] = Gray_Sensor3_Read();
    SensorGrop[1][1] = Gray_Sensor4_Read();

    if (SensorGrop[0][1] == Black && SensorGrop[0][0] == Black &&
        SensorGrop[1][0] == White && SensorGrop[1][1] == White)
        return 12;

    else if (SensorGrop[0][1] == Black && SensorGrop[0][0] == White &&
             SensorGrop[1][0] == White && SensorGrop[1][1] == Black)
        return 14;

    else if (SensorGrop[0][1] == White && SensorGrop[0][0] == Black &&
             SensorGrop[1][0] == Black && SensorGrop[1][1] == White)
        return 23;

    else if (SensorGrop[0][1] == White && SensorGrop[0][0] == White &&
             SensorGrop[1][0] == Black && SensorGrop[1][1] == Black)
        return 34;
    else
        return 0;
}

//灰度传感器测试
uint8 Sensor1, Sensor2, Sensor3, Sensor4, Status;
void GraySensor_Tset(void)
{
    GraySensor_Init();
    for (;;)
    {
        Sensor1 = Gray_Sensor1_Read();
        Sensor2 = Gray_Sensor2_Read();
        Sensor3 = Gray_Sensor3_Read();
        Sensor4 = Gray_Sensor4_Read();
        Status = Sensor_Trigger();
    }
}