#ifndef Camera_h
#define Camera_h

extern void Camera_Init(void);
extern float Error_Get(char mode,short int Set); //x ����ɨ��(��Ӧ����) y ����ɨ��(��Ӧ����) ��ȡƫ�ֱ������Ļ�����ƫ�
extern float Angle_Get(char mode);             //x ����ɨ��(��Ӧ����) y ����ɨ��(��Ӧ����) ��ȡб��б�ʴ�С
extern void Camera_Test(void);

#endif