#ifndef _Camera_h
#define _Camera_h
#include "headfile.h"

extern void Camera_Init(void);
extern float Offset_Get(char mode, uint8 Set); //x ����ɨ��(��Ӧ����) y ����ɨ��(��Ӧ����) ��ȡƫ�ֱ������Ļ�����ƫ�
extern float Angle_Get(char mode);             //x ����ɨ��(��Ӧ����) y ����ɨ��(��Ӧ����) ��ȡб��б�ʴ�С
extern void Camera_Test(void);

#endif