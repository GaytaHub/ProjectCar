#include "headfile.h"

//#define SpeedTest
//#define XErrorTest
//#define YErrorTest
 #define AngleErrorTest

//循迹函数

void Track(float XSpeedSet, float YSpeedSet, char Reset)
{
    float XError = 0.0, YError = 0.0, AngleError = 0.0, AngleLastError = 0.0; //偏差
    float LFRev = 0.0, RFRev = 0.0, LBRev = 0.0, RBRev = 0.0;                 //轮子转速
    float XErrorSet = 0.0, YErrorSet = 0.0;                                   //偏差转化速度设定
    float AngleErrorSet = 0.0;                                                //角度偏差转速度设定
    float LFSet = 0.0, RFSet = 0.0, LBSet = 0.0, RBSet = 0.0;
    float LFPwm = 0.0, RFPwm = 0.0, LBPwm = 0.0, RBPwm = 0.0; //电机输出
    
    LFRev = LF_Encoder_Get();
    RFRev = RF_Encoder_Get();
    LBRev = LB_Encoder_Get();
    RBRev = RB_Encoder_Get();
    XSpeedReal = +LFRev - RFRev + LBRev - RBRev;  //车体X轴实际速度计算
    YSpeedReal = -LFRev - RFRev + LBRev + RBRev;  //车体Y轴实际速度计算
    Image_Decompression(image_bin, image_dec[0]); //摄像头图像解压

#ifndef SpeedTest

    //    if (XSpeed == 0) //沿Y轴循迹
    //    {
    //        YError = Camera_Get('Y', 'E');
    //        AngleError = Camera_Get('Y', 'T');
    //
    //        YErrorSet = Y_Error_PD(YError, Reset);
    //        AngleErrorSet = Angle_PD(AngleError, Reset);
    //    }
    if (YSpeed == 0) //沿X轴循迹
    {
        //图像处理
        XError = Camera_Get('X', 'E');
        if(myabs(XError)<1)
            XError = 0;
        AngleError = Camera_Get('X', 'T'); //偏差转速度设定值
        if(myabs(AngleError)<1)
            AngleError = 0;
            //偏差低通滤波
//        if (myabs(AngleError - AngleLastError) > 3)
//            AngleError = AngleLastError* 0.8 + AngleError *0.2;
//        AngleLastError = AngleError;
        XErrorSet = X_Error_PD(XError, Reset);
        AngleErrorSet = Angle_PD(AngleError, Reset);
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

    LFSet = -XSpeedSet + YSpeedSet - XErrorSet - YErrorSet + AngleErrorSet;
    LBSet = +XSpeedSet + YSpeedSet - XErrorSet - YErrorSet + AngleErrorSet;
    RFSet = -XSpeedSet - YSpeedSet + XErrorSet + YErrorSet + AngleErrorSet;
    RBSet = +XSpeedSet - YSpeedSet + XErrorSet + YErrorSet + AngleErrorSet;

    LFPwm = LF_Speed_PID(LFSet, LFRev, Reset);
    RFPwm = RF_Speed_PID(RFSet, RFRev, Reset);
    LBPwm = LB_Speed_PID(LBSet, LBRev, Reset);
    RBPwm = RB_Speed_PID(RBSet, RBRev, Reset);

    LF_Motor_Control((int16)LFPwm);
    RF_Motor_Control((int16)RFPwm);
    LB_Motor_Control((int16)LBPwm);
    RB_Motor_Control((int16)RBPwm);
}
