#ifndef _Motor_h
#define _Motor_h


void Motor_Init(void);
void LF_Motor_Control(int16 duty);
void RF_Motor_Control(int16 duty);
void LB_Motor_Control(int16 duty);
void RB_Motor_Control(int16 duty);
void Motor_Test(void);


#endif