#ifndef Camera_h
#define Camera_h

extern void Camera_Init(void);
extern void LeftToRight_Scan(void);
extern void UpToDown_Scan(void);
extern float Error_Get(char Axis, char Mode);
extern void Camera_Test(void);

#endif