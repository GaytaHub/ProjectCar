#include "headfile.h"

//#define SpeedTest
//#define XErrorTest
//#define YErrorTest
//#define AngleErrorTest

/********************************************循迹函数***********************************************/
    float XError = 0.0, YError = 0.0, XAngleError = 0.0, XAngleLastError = 0.0,YAngleError = 0.0, YAngleLastError = 0.0;             //偏差
    float XPositiveRev = 0.0, XNegativeRev = 0.0, YPositiveRev = 0.0, YNegativeRev = 0.0; //轮子转速
    float XErrorSet = 0.0, YErrorSet = 0.0, XAngleErrorSet = 0.0,YAngleErrorSet = 0.0;                          //偏差转化速度设定，角度偏差转速度设定
    float XPositiveSet = 0.0, XNegativeSet = 0.0, YPositiveSet = 0.0, YNegativeSet = 0.0; //速度设定值
    float XPositivePwm = 0.0, XNegativePwm = 0.0, YPositivePwm = 0.0, YNegativePwm = 0.0; //电机输出

void Track(float XSpeedSet, float YSpeedSet, char Reset)
{

    XPositiveRev = XPositive_Encoder_Get();
    XNegativeRev = XNegative_Encoder_Get();
    YPositiveRev = YPositive_Encoder_Get();
    YNegativeRev = YNegative_Encoder_Get();
    XSpeedReal = +YPositiveRev - YNegativeRev;    //车体X轴实际速度计算
    YSpeedReal = -XPositiveRev + XNegativeRev;    //车体Y轴实际速度计算
    Image_Decompression(image_bin, image_dec[0]); //摄像头图像解压

#ifndef SpeedTest

    if (XSpeedSet == 0.0) //沿Y轴循迹
    {
        UpToDown_Scan();
        YError = Error_Get('Y', 'E');
        YAngleError = Error_Get('Y', 'T');
        //角度误差低通滤波
        YAngleError = YAngleLastError * 0.8 + YAngleError * 0.2;
        YAngleLastError = YAngleError;

        YErrorSet = Y_Error_PD(YError, Reset);
        YAngleErrorSet = Angle_PD(YAngleError, Reset);
    }
    if (YSpeedSet == 0.0) //沿X轴循迹
    {
        LeftToRight_Scan();               //图像处理
        XError = Error_Get('X', 'E');     //误差量读取
        XAngleError = Error_Get('X', 'T'); //偏移角读取
        //角度误差低通滤波
        XAngleError = XAngleLastError * 0.7 + XAngleError * 0.3;
        XAngleLastError = XAngleError;

        XErrorSet = X_Error_PD(XError, Reset);
        XAngleErrorSet = Angle_PD(XAngleError, Reset);
    }
#endif
#ifdef XErrorTest
    AngleErrorSet = 0.0;
    YErrorSet = 0.0;
#endif
#ifdef YErrorTest
    AngleErrorSet = 0.0;
    XErrorSet = 0.0;
#endif
#ifdef AngleErrorTest
    XErrorSet = 0.0;
    YErrorSet = 0.0;
#endif
    //速度设定值计算
    XPositiveSet = -YSpeedSet + XErrorSet - YAngleErrorSet;
    YPositiveSet = +XSpeedSet + YErrorSet - XAngleErrorSet;
    XNegativeSet = +YSpeedSet - XErrorSet - YAngleErrorSet;
    YNegativeSet = -XSpeedSet - YErrorSet - XAngleErrorSet;
    //速度闭环
    XPositivePwm = XPositive_Speed_PID(XPositiveSet, XPositiveRev, Reset);
    YPositivePwm = YPositive_Speed_PID(YPositiveSet, YPositiveRev, Reset);
    XNegativePwm = XNegative_Speed_PID(XNegativeSet, XNegativeRev, Reset);
    YNegativePwm = YNegative_Speed_PID(YNegativeSet, YNegativeRev, Reset);
    //电机控制
    XPositive_Motor_Control((int16)XPositivePwm);
    YPositive_Motor_Control((int16)YPositivePwm);
    XNegative_Motor_Control((int16)XNegativePwm);
    YNegative_Motor_Control((int16)YNegativePwm);
}
