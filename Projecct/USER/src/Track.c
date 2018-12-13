#include "headfile.h"

#define SpeedTest
// #define ErrorTest
// #define AngleErrorTest

//循迹函数
float XError = 0.0, YError = 0.0, AngleError = 0.0;       //偏差
float LFRev = 0.0, RFRev = 0.0, LBRev = 0.0, RBRev = 0.0; //轮子转速
float XErrorSet = 0.0, YErrorSet = 0.0;                   //偏差转化速度设定
float AngleErrorSet = 0.0;                                //角度偏差转速度设定
float LFSet = 0.0, RFSet = 0.0, LBSet = 0.0, RBSet = 0.0;
float LFPwm = 0.0, RFPwm = 0.0, LBPwm = 0.0, RBPwm = 0.0; //电机输出
void Track(float XSpeedSet, float YSpeedSet, char Reset)
{

    
    LFRev = LF_Encoder_Get();
    RFRev = RF_Encoder_Get();
    LBRev = LB_Encoder_Get();
    RBRev = RB_Encoder_Get();
    XSpeedReal = +LFRev - RFRev + LBRev - RBRev; //车体X轴实际速度计算
    YSpeedReal = -LFRev - RFRev + LBRev + RBRev; //车体Y轴实际速度计算
    
#ifndef SpeedTest
    Image_Decompression(image_bin, image_dec[0]); //摄像头图像解压
    
    if (XSpeed == 0) //沿Y轴循迹
    {
        //图像处理
        XError = Error_Get('X', OV7725_W / 2);
        AngleError = Angle_Get('Y');
        //偏差转速度设定值
        XErrorSet = X_Error_PD(XError, Reset);
        AngleErrorSet = Angle_PD(AngleError, Reset);
    }
    if (YSpeed == 0) //沿X轴循迹
    {
        YError = Error_Get('Y', OV7725_H / 2);
        AngleError = Angle_Get('X');
        
        YErrorSet = Y_Error_PD(YError, Reset);
        AngleErrorSet = Angle_PD(AngleError, Reset);
    }
#endif
#ifdef ErrorTest
    AngleErrorSet = 0.0;
#endif
#ifdef AngleErrorTest
    XErrorSet = 0.0;
    YErrorSet = 0.0;
#endif
    
    LFSet = -XSpeedSet + YSpeedSet + XErrorSet - YErrorSet + AngleErrorSet;
    RFSet = -XSpeedSet - YSpeedSet + XErrorSet + YErrorSet + AngleErrorSet;
    LBSet = +XSpeedSet + YSpeedSet - XErrorSet - YErrorSet + AngleErrorSet;
    RBSet = +XSpeedSet - YSpeedSet - XErrorSet + YErrorSet + AngleErrorSet;
    
    LFPwm=LF_Speed_PID(LFSet,LFRev,Reset);
    RFPwm=RF_Speed_PID(RFSet,RFRev,Reset);
    LBPwm=LB_Speed_PID(LBSet,LBRev,Reset);
    RBPwm=RB_Speed_PID(RBSet,RBRev,Reset);
    
    LF_Motor_Control((int16)LFPwm);
    RF_Motor_Control((int16)RFPwm);
    LB_Motor_Control((int16)LBPwm);
    RB_Motor_Control((int16)RBPwm);
}
