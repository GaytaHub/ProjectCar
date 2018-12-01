#include "headfile.h"

//偏差参数
struct Data Offset_Para = {0, 0, 0};

//角度参数
struct Data Angle_Para = {0, 0, 0};

//速度参数
int16 X_Speed = 0, Y_Speed = 0; //设定速度
int16 X_Speed_Real, Y_Speed_Real; //车体实际速度
struct Data Speed_Para = {0, 0, 0};

//全局坐标
float X_Position = 0.5; //起始横坐标设为:0.5
float Y_Position = 0.5; //起始纵坐标设置为0.5





