#include "headfile.h"

//Unit Test
//#define DirectionTset

#ifndef DirectionTset
 #define SpeedTest
 #define OffsetTest
 #define AngleTest
#endif

//循迹函数
int16 X_Speed_Real, Y_Speed_Real; //车体实际速度
void Track(char Axis, char Dir, char Reset)
{
        float X_Offset, Y_Offset, Angle_Offset; //偏差
        int16 LF_W, RF_W, LB_W, RB_W;           //轮子转速
        int16 X_Speed_Set, Y_Speed_Set;         //车体设定速度
        int16 X_Offset_PWM, Y_Offset_PWM;       //偏差输出
        int16 X_Speed_PWM, Y_Speed_PWM;         //速度输出
        int16 Angle_Offset_PWM;                 //角度输出
        int16 LF_PWM, RF_PWM, LB_PWM, RB_PWM;   //电机输出

        LF_W = LF_Encoder_Get();
        RF_W = RF_Encoder_Get();
        LB_W = LF_Encoder_Get();
        RB_W = RB_Encoder_Get();
        X_Speed_Real = +LF_W - RF_W + LB_W - RB_W;
        Y_Speed_Real = -LF_W - RF_W + LB_W + RB_W;

        if (Dir == '+')
        {
                X_Speed_Set = +X_Speed;
                Y_Speed_Set = +Y_Speed;
        }
        if (Dir == '-')
        {
                X_Speed_Set = -X_Speed;
                Y_Speed_Set = -Y_Speed;
        }

        Image_Decompression(image_bin, image_dec[0]); //摄像头图像解压
        if (Axis == 'X')                              //沿X轴循迹
        {
                Y_Offset = Offset_Get('Y', OV7725_H / 2);
                Angle_Offset = Angle_Get('X');

#ifdef SpeedTest
                X_Speed_PWM = X_Speed_PID(X_Speed_Set, X_Speed_Real, Reset);
#endif
#ifdef OffsetTest
                Y_Offset_PWM = Y_Offset_PD(Y_Offset, Reset);
#endif
#ifdef AngleTest
                Angle_Offset_PWM = Angle_PD(Angle_Offset, Reset);
#endif
        }
        else if (Axis == 'Y')
        {
                X_Offset = Offset_Get('X', OV7725_W / 2);
                Angle_Offset = Angle_Get('Y');

#ifdef SpeedTest
                Y_Speed_PWM = Y_Speed_PID(Y_Speed_Set, Y_Speed_Real, Reset);
#endif
#ifdef OffsetTest
                X_Offset_PWM = X_Offset_PD(X_Offset, Reset);
#endif
#ifdef AngleTest
                Angle_Offset_PWM = Angle_PD(Angle_Offset, Reset);
#endif
        }

#ifdef DirectionTset
        X_Speed_PWM = 0;
        Y_Speed_PWM = 0;
        X_Offset_PWM = 0;
        Y_Offset_PWM = 0;
        Angle_Offset_PWM = 0;
#endif

        LF_PWM = -X_Speed_PWM - Y_Offset_PWM + Y_Speed_PWM + X_Offset_PWM - Angle_Offset_PWM;
        RF_PWM = -X_Speed_PWM + Y_Offset_PWM - Y_Speed_PWM + X_Offset_PWM - Angle_Offset_PWM;
        LB_PWM = +X_Speed_PWM - Y_Offset_PWM + Y_Speed_PWM - X_Offset_PWM - Angle_Offset_PWM;
        RB_PWM = +X_Speed_PWM + Y_Offset_PWM - Y_Speed_PWM - X_Offset_PWM - Angle_Offset_PWM;

        LF_Motor_Control(LF_PWM);
        RF_Motor_Control(RF_PWM);
        LB_Motor_Control(LB_PWM);
        RB_Motor_Control(RB_PWM);
}
