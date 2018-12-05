#ifndef _Steering_h
#define _Steering_h
#include "headfile.h"

void Steering_Init(void);
void SteeringOne_Control(uint16 duty);
void SteeringTwo_Control(uint16 duty);
void SteeringThree_Control(uint16 duty);
void SteeringFour_Control(uint16 duty);
void SteeringFive_Control(uint16 duty);
void SteeringAll_Control(uint16 duty1, uint16 duty2, uint16 duty3, uint16 duty4, uint16 duty5);
void Steering_Test(void);


#endif