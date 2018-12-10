#include "headfile.h"

//�����ʼ��
void Steering_Init(void)
{
    ftm_pwm_init(ftm1, ftm_ch0, 50, 750);
    ftm_pwm_init(ftm1, ftm_ch1, 50, 750);
    ftm_pwm_init(ftm2, ftm_ch0, 50, 750);
    ftm_pwm_init(ftm2, ftm_ch1, 50, 750);
    
    cmt_pwm_init(50, 750);
}

#define SteeringMin 250
#define SteeringMax 1250

//һ�Ŷ���޷����
void SteeringOne_Control(uint16 duty)
{
    duty = (uint16)limit_ab(duty, SteeringMin, SteeringMax);
    ftm_pwm_duty(ftm1, ftm_ch0, duty);
}

//���Ŷ���޷����
void SteeringTwo_Control(uint16 duty)
{
    duty = (uint16)limit_ab(duty, SteeringMin, SteeringMax);
    ftm_pwm_duty(ftm1, ftm_ch1, duty);
}

//���Ŷ���޷����
void SteeringThree_Control(uint16 duty)
{
    duty = (uint16)limit_ab(duty, SteeringMin, SteeringMax);
    ftm_pwm_duty(ftm2, ftm_ch0, duty);
}

//�ĺŶ���޷����
void SteeringFour_Control(uint16 duty)
{
    duty = (uint16)limit_ab(duty, SteeringMin, SteeringMax);
    ftm_pwm_duty(ftm2, ftm_ch1, duty);
}

//��Ŷ���޷����
void SteeringFive_Control(uint16 duty)
{
    duty = (uint16)limit_ab(duty, SteeringMin, SteeringMax);
    cmt_pwm_duty(duty);
}

//��������
void SteeringAll_Control(uint16 duty1, uint16 duty2, uint16 duty3, uint16 duty4, uint16 duty5)
{
    SteeringOne_Control(duty1);
    SteeringTwo_Control(duty2);
    SteeringThree_Control(duty3);
    SteeringFour_Control(duty4);
    SteeringFive_Control(duty5);
}

//Steering Test
void Steering_Test(void)
{
    Steering_Init();
    for (;;)
    {
        // SteeringOne_Control(500);
        // SteeringTwo_Control(500);
        // SteeringThree_Control(500);
        // SteeringFour_Control(500);
        // SteeringFive_Control(500);
        // systick_delay_ms(1000);
        SteeringAll_Control(1000, 1000, 1000, 1000, 1000);
    }
}