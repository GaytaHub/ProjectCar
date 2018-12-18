#ifndef PID_h
#define PID_h

extern float X_Error_PD(float Error, char Reset);
extern float Y_Error_PD(float Error, char Reset);
extern float Angle_PD(float AngleError, char Reset);
extern float XPositive_Speed_PID(float Set, float Real, char Reset);
extern float YPositive_Speed_PID(float Set, float Real, char Reset);
extern float XNegative_Speed_PID(float Set, float Real, char Reset);
extern float YNegative_Speed_PID(float Set, float Real, char Reset);

#endif