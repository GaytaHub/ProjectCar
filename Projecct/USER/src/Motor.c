#include "headfile.h"

//电机初始化
void Motor_Init(void)
{
    //电机初始化
    ftm_pwm_init(ftm0, ftm_ch0, 15000, 0);
    ftm_pwm_init(ftm0, ftm_ch1, 15000, 0);
    ftm_pwm_init(ftm0, ftm_ch2, 15000, 0);
    ftm_pwm_init(ftm0, ftm_ch3, 15000, 0);
    ftm_pwm_init(ftm0, ftm_ch4, 15000, 0);
    ftm_pwm_init(ftm0, ftm_ch5, 15000, 0);
    ftm_pwm_init(ftm0, ftm_ch6, 15000, 0);
    //与非门控制下的pwm输出
    gpio_init(E5, GPO, 1);
    gpio_init(E6, GPO, 0);
}

//左前轮电机驱动
void LF_Motor_Control(int16 duty)
{
    duty = (int16)limit(duty, FTM0_PRECISON);
    if (duty >= 0)
    {
        ftm_pwm_duty(ftm0, ftm_ch0, duty);
        ftm_pwm_duty(ftm0, ftm_ch1, 0);
    }
    else
    {
        ftm_pwm_duty(ftm0, ftm_ch0, 0);
        ftm_pwm_duty(ftm0, ftm_ch1, -duty);
    }
}

//右前轮电机驱动
void RF_Motor_Control(int16 duty)
{
    duty = (int16)limit(duty, FTM0_PRECISON);
    if (duty >= 0)
    {
        ftm_pwm_duty(ftm0, ftm_ch2, duty);
        ftm_pwm_duty(ftm0, ftm_ch3, 0);
    }
    else
    {
        ftm_pwm_duty(ftm0, ftm_ch2, 0);
        ftm_pwm_duty(ftm0, ftm_ch3, -duty);
    }
}

//左后轮电机驱动
void LB_Motor_Control(int16 duty)
{
    duty = (int16)limit(duty, FTM0_PRECISON);
    if (duty >= 0)
    {
        ftm_pwm_duty(ftm0, ftm_ch4, duty);
        ftm_pwm_duty(ftm0, ftm_ch5, 0);
    }
    else
    {
        ftm_pwm_duty(ftm0, ftm_ch4, 0);
        ftm_pwm_duty(ftm0, ftm_ch5, -duty);
    }
}

//右后轮电机驱动
void RB_Motor_Control(int16 duty)
{
    duty = (int16)limit(duty, FTM0_PRECISON);
    if (duty >= 0)
    {
        ftm_pwm_duty(ftm0, ftm_ch6, duty);
        gpio_set(E5, 1);
        gpio_set(E6, 0);
    }
    else
    {
        ftm_pwm_duty(ftm0, ftm_ch6, -duty);
        gpio_set(E5, 0);
        gpio_set(E6, 1);
    }
}

//Motor Test
void Motor_Test(void)
{
    Motor_Init();
    for (;;)
    {
        LF_Motor_Control(-300);
        RF_Motor_Control(-300);
        LB_Motor_Control(-300);
        RB_Motor_Control(-300);
    }
}
