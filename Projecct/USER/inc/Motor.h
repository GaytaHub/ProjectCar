#ifndef Motor_h
#define Motor_h

void Motor_Init(void);
void LF_Motor_Control(short int duty);
void RF_Motor_Control(short int duty);
void LB_Motor_Control(short int duty);
void RB_Motor_Control(short int duty);
void Motor_Test(void);

#endif