#ifndef Global_h
#define Global_h

//PID结构体结构
struct Data
{
    float P; //比例项
    float I; //积分项
    float D; //微分项
};

extern struct Data ErrorPara;
extern struct Data AnglePara; 
extern struct Data SpeedPara;

enum
{
    False,
    True
};

enum
{
    Black,
    White
};

extern short int XSpeed, YSpeed;
extern short int XSpeedReal, YSpeedReal;
extern float XPosition, YPosition;

#endif