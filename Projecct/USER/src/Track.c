#include "headfile.h"

//Unit Test
//#define DirectionTset

#ifndef DirectionTset
 #define SpeedTest
 #define ErrorTest
 #define AngleTest
#endif

//循迹函数
void Track(char Axis, char Dir, char Reset)
{
        float XError, YError, AngleError; //偏差
        int16 LFRev, RFRev, LBRev, RBRev;           //轮子转速
        int16 XSpeedSet, YSpeedSet;         //车体设定速度
        int16 XErrorPwm, YErrorPwm;       //偏差输出
        int16 XSpeedPwm, YSpeedPwm;         //速度输出
        int16 AngleErrorPwm;                 //角度输出
        int16 LFPwm, RFPwm, LBPwm, RBPwm;   //电机输出

        LFRev = LF_Encoder_Get();
        RFRev = RF_Encoder_Get();
        LBRev = LF_Encoder_Get();
        RBRev = RB_Encoder_Get();
        XSpeedReal = +LFRev - RFRev + LBRev - RBRev;
        YSpeedReal = -LFRev - RFRev + LBRev + RBRev;

        //速度（方向）正负设定
        if (Dir == '+')
        {
                XSpeedSet = +XSpeed;
                YSpeedSet = +YSpeed;
        }
        if (Dir == '-')
        {
                XSpeedSet = -XSpeed;
                YSpeedSet = -YSpeed;
        }

        Image_Decompression(image_bin, image_dec[0]); //摄像头图像解压

        if (Axis == 'X') //沿X轴循迹
        {
                YError = Error_Get('Y', OV7725_H / 2);
                AngleError = Angle_Get('X');
#ifdef SpeedTest
                XSpeedPwm = X_Speed_PID(XSpeedSet, XSpeedReal, Reset);
#endif
#ifdef ErrorTest
                YErrorPwm = Y_Error_PD(YError, Reset);
#endif
#ifdef AngleTest
                AngleErrorPwm = Angle_PD(AngleError, Reset);
#endif
        }
        else if (Axis == 'Y')
        {
                XError = Error_Get('X', OV7725_W / 2);
                AngleError = Angle_Get('Y');
#ifdef SpeedTest
                YSpeedPwm = Y_Speed_PID(YSpeedSet, YSpeedReal, Reset);
#endif
#ifdef ErrorTest
                XErrorPwm = X_Error_PD(XError, Reset);
#endif
#ifdef AngleTest
                AngleErrorPwm = Angle_PD(AngleError, Reset);
#endif
        }

#ifdef DirectionTset
        XSpeedPwm = 0;
        YSpeedPwm = 0;
        XErrorPwm = 0;
        YErrorPwm = 0;
        AngleErrorPwm = 0;
#endif

        LFPwm = -XSpeedPwm - YErrorPwm + YSpeedPwm + XErrorPwm - AngleErrorPwm;
        RFPwm = -XSpeedPwm + YErrorPwm - YSpeedPwm + XErrorPwm - AngleErrorPwm;
        LBPwm = +XSpeedPwm - YErrorPwm + YSpeedPwm - XErrorPwm - AngleErrorPwm;
        RBPwm = +XSpeedPwm + YErrorPwm - YSpeedPwm - XErrorPwm - AngleErrorPwm;

        LF_Motor_Control(LFPwm);
        RF_Motor_Control(RFPwm);
        LB_Motor_Control(LBPwm);
        RB_Motor_Control(RBPwm);
}
