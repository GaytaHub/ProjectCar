#ifndef Camera_h
#define Camera_h

extern void Camera_Init(void);
extern float Error_Get(char mode,short int Set); //x 横向扫描(对应竖线) y 纵向扫描(对应横线) 获取偏差（直线与屏幕中央的偏差）
extern float Angle_Get(char mode);             //x 横向扫描(对应竖线) y 纵向扫描(对应横线) 获取斜线斜率大小
extern void Camera_Test(void);

#endif