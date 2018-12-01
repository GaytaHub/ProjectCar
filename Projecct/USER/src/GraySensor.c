#include "headfile.h"

//以象限定义计线传感器位置
#define Gray_Sensor1 ADC1_SE8
#define Gray_Sensor2 ADC1_SE8
#define Gray_Sensor3 ADC1_SE8
#define Gray_Sensor4 ADC1_SE8

//阈值宏定义
#define threshold 600

//灰度传感器读取接口初始化
void GraySensor_Init(void)
{
    adc_init(Gray_Sensor1);
    adc_init(Gray_Sensor2);
    adc_init(Gray_Sensor3);
    adc_init(Gray_Sensor4);
}

//灰度传感器二值化读取，接收的反射光越强，ADC读取值越大，即黑线为0
uint8 Gray_Sensor1_Read(void)
{
    return (adc_average(Gray_Sensor1, ADC_10bit) > threshold) ? White : Black;
}

uint8 Gray_Sensor2_Read(void)
{
    return (adc_average(Gray_Sensor2, ADC_10bit) > threshold) ? White : Black;
}

uint8 Gray_Sensor3_Read(void)
{
    return (adc_average(Gray_Sensor3, ADC_10bit) > threshold) ? White : Black;
}

uint8 Gray_Sensor4_Read(void)
{
    return (adc_average(Gray_Sensor4, ADC_10bit) > threshold) ? White : Black;
}

//灰度传感器测试
uint8 Sensor1, Sensor2, Sensor3, Sensor4;
void GraySensor_Tset(void)
{
    GraySensor_Init();
    for (;;)
    {
        Sensor1 = Gray_Sensor1_Read();
        Sensor2 = Gray_Sensor2_Read();
        Sensor3 = Gray_Sensor3_Read();
        Sensor4 = Gray_Sensor4_Read();
    }
}