#ifndef PID_h
#define PID_h

extern float X_Error_PD(float Error, char Reset);
extern float Y_Error_PD(float Error, char Reset);
extern float Angle_PD(float AngleError, char Reset);
extern float LF_Speed_PID(float Set, float Real, char Reset);
extern float LB_Speed_PID(float Set, float Real, char Reset);
extern float RF_Speed_PID(float Set, float Real, char Reset);
extern float RB_Speed_PID(float Set, float Real, char Reset);

#endif