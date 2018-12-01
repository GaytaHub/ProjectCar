#ifndef _PID_h
#define _PID_h

extern int16 X_Offset_PD(float Offset, char Reset);
extern int16 Y_Offset_PD(float Offset, char Reset);
extern int16 Angle_PD(float Angle_Offset, char Reset);
extern int16 X_Speed_PID(float Set, float Real, char Reset);
extern int16 Y_Speed_PID(float Set, float Real, char Reset);

#endif