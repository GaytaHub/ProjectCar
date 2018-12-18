#include "headfile.h"

//偏差参数
struct Data ErrorPara = {0.8, 0, 0.0};

//角度参数
struct Data AnglePara = {1.5, 0, 0.0};

//速度参数
float XSpeed = 0, YSpeed = 0; //设定速度
float XSpeedReal, YSpeedReal; //车体实际速度
struct Data SpeedPara = {65, 15, 0};

//全局坐标
float XPosition = 0.5; //起始横坐标设为:0.5
float YPosition = 0.5; //起始纵坐标设置为0.5





