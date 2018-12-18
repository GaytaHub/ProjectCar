#ifndef Motor_h
#define Motor_h

void Motor_Init(void);
extern void XPositive_Motor_Control(short int duty);
extern void XNegative_Motor_Control(short int duty);
extern void YPositive_Motor_Control(short int duty);
extern void YNegative_Motor_Control(short int duty);
void Motor_Test(void);

#endif