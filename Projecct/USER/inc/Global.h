#ifndef Global_h
#define Global_h 

//PID结构体结构
struct Data
{
    float P; //比例项
    float I; //积分项
    float D; //微分项
};

#define True 1
#define False 0

#define White 1
#define Black 0

extern short int XSpeed, YSpeed;
extern short int XSpeedReal, YSpeedReal;
extern float XPosition, YPosition;

#endif