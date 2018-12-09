#ifndef Steering_h
#define Steering_h

void Steering_Init(void);
void SteeringOne_Control(unsigned short int duty);
void SteeringTwo_Control(unsigned short int duty);
void SteeringThree_Control(unsigned short int duty);
void SteeringFour_Control(unsigned short int duty);
void SteeringFive_Control(unsigned short int duty);
void SteeringAll_Control(unsigned short int duty1, unsigned short int duty2, unsigned short int duty3, unsigned short int duty4, unsigned short int duty5);
void Steering_Test(void);


#endif