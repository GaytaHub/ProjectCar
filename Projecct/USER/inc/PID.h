#ifndef PID_h
#define PID_h

extern short int X_Error_PD(float Error, char Reset);
extern short int Y_Error_PD(float Error, char Reset);
extern short int Angle_PD(float AngleError, char Reset);
extern short int X_Speed_PID(float Set, float Real, char Reset);
extern short int Y_Speed_PID(float Set, float Real, char Reset);

#endif