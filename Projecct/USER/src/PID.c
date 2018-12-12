#include "headfile.h"

/*思路：固定坐标系，摄像头只看向一个方向，分为速度、偏移量、角度三环闭合，速度和偏移量分为X、Y两个方向，
将三个环的设定值按一定规律叠加作为各个电机的速度设定输入，实际值由编码器读出，形成闭环控制，将运动控制封装成函数，
输入X或Y方向(沿一个方向运动时，把另一个方向速度设置为0，同一时刻只有
一个方向的速度与另一个方向的偏差)的速度、偏差以及角度偏差，
即可控制车体沿不同方向移动，移动过程中记录当前坐标，移动至指定坐标时进入微调，
当进入微调模式时，速度全部设置为零，输入偏差除已有的横向偏移量与角度偏移量，
再加上另一方向上的定位偏差(X轴方向的角度偏差与横向偏差加上Y方向的定位偏差)，即可实现精确定位*/
//全局坐标系：出发点向右为X轴正方向，出发点向前为Y轴正方向，逆时针为正方向
//局部坐标系(以车体为参考点)：与全局坐标系方向相同
//编码器方向：从车轮外向内看，逆时针旋转为正值
//电机输出极性：电机输出为正值时应使轮子逆时针转动
//摄像头视野偏差提取：
/*摄像头看向全局坐标系的X轴正方向*/
/*摄像头偏差符号定向：
规定摄像头视野铅锤中线，在与X轴平行的黑线左侧时，偏差为正，从中线旋转到黑线得到的角，
逆时针为正；摄像头视野水平中线，在与Y轴平行的黑线上方时，偏差为正，从中线旋转到黑线
得到的角，逆时针为正*/
/*车体单方向运行时，轮子旋转方向(正负号表示编码器读取方向)：
X+: LF——顺时针(-) RF——顺时针(-) LB——逆时针(+) RB——逆时针(+)
Y+: LF——逆时针(+) RF——顺时针(-) LB——逆时针(+) RB——顺时针(-)
X-: LF——逆时针(+) RF——逆时针(+) LB——顺时针(-) RB——顺时针(-)
Y-: LF——顺时针(-) RF——逆时针(+) LB——顺时针(-) RB——逆时针(+)
A+: 全为顺时针(-)
A-: 全为逆时针(+)*/
/*X、Y轴速度 即车体沿X、Y轴的移动速度(车体沿X、Y轴正方向行驶为正，设定值为某一固定的正值) 
XSpeedSet = Constant,YSpeedSet = Constant*/
/*偏差 即车体X、Y轴（交点由摄像头所看位置确定）到黑线的垂直距离：
(X_Error:Y轴在黑线右侧为正，需沿X-移动消除偏差，Y_Error:X轴在黑线左侧为正,需沿Y-移动消除偏差，设定值为0)
XErrorSet = PD(XError), YErrorSet = PD(YError)*/
/*角度 即车体X、Y轴与黑线的夹角(从X、Y轴转动到黑线得到的夹角，逆时针为正，需沿顺时针旋转消除偏差，设定值为0) 
AngleSet = PD(Angle)*/
/*各个轮子的设定值计算：
LFSet = - XSpeedSet + YSpeedSet + XErrorSet - YErrorSet + AngleSet
RFSet = - XSpeedSet - YSpeedSet + XErrorSet + YErrorSet + AngleSet
LBSet = + XSpeedSet + YSpeedSet - XErrorSet - YErrorSet + AngleSet
RBSet = + XSpeedSet - YSpeedSet - XErrorSet + YErrorSet + AngleSet*/

//X轴偏差控制PID
float X_Error_PD(float Error, char Reset)
{
    static float LastError = 0.0;
    float Result;

    if (Reset == True)
    {
        Result = 0;
        LastError = 0;
    }
    else if (Reset == False)
    {
        Result = ErrorPara.P * Error + ErrorPara.D * (Error - LastError);
        LastError = Error;
    }

    return Result;
}

//Y轴偏差控制
float Y_Error_PD(float Error, char Reset)
{
    static float LastError = 0.0;
    float Result;

    if (Reset == True)
    {
        Result = 0;
        LastError = 0;
    }
    else if (Reset == False)
    {
        Result = ErrorPara.P * Error + ErrorPara.D * (Error - LastError);
        LastError = Error;
    }

    return Result;
}

//角度控制PID
float Angle_PD(float AngleError, char Reset)
{
    static float AngleLastError = 0.0;
    float Result;

    if (Reset == True)
    {
        Result = 0;
        AngleLastError = 0;
    }
    else if (Reset == False)
    {
        Result = AnglePara.P * AngleError + AnglePara.D * (AngleError - AngleLastError);
        AngleLastError = AngleError;
    }

    return Result;
}

//左前轮速度控制PID
#define IntegralRange 20   //积分范围
#define AllIntegralRange 5 //全积分范围

float LF_Speed_PID(float Set, float Real, char Reset)
{
    float index = 1.0;
    float NewError, Result;
    static float LastError = 0.0, Integral = 0.0;

    NewError = Set - Real;
    if (Reset == False)
    {
        if (myabs(NewError > IntegralRange))
        {
            index = 0.0;
        }
        else if (myabs(NewError < AllIntegralRange))
        {
            index = 1.0;
            Integral += NewError;
        }
        else
        {
            index = (IntegralRange - myabs(NewError)) / (IntegralRange - AllIntegralRange);
            Integral += NewError;
        }
        Result = SpeedPara.P * NewError + SpeedPara.I * index * Integral + SpeedPara.D * (NewError - LastError);
        LastError = NewError;
    }
    else if (Reset == True)
    {
        Result = 0;
        Integral = 0.0;
        LastError = 0.0;
    }

    return Result;
}

//右前轮控制PID
float RF_Speed_PID(float Set, float Real, char Reset)
{
    float index = 1.0;
    float NewError, Result;
    static float LastError = 0.0, Integral = 0.0;

    NewError = Set - Real;
    if (Reset == False)
    {
        if (myabs(NewError > IntegralRange))
        {
            index = 0.0;
        }
        else if (myabs(NewError < AllIntegralRange))
        {
            index = 1.0;
            Integral += NewError;
        }
        else
        {
            index = (IntegralRange - myabs(NewError)) / (IntegralRange - AllIntegralRange);
            Integral += NewError;
        }
        Result = SpeedPara.P * NewError + SpeedPara.I * index * Integral + SpeedPara.D * (NewError - LastError);
        LastError = NewError;
    }
    else if (Reset == True)
    {
        Result = 0;
        Integral = 0.0;
        LastError = 0.0;
    }

    return (int16)Result;
}

//左后轮控制pid
float LB_Speed_PID(float Set, float Real, char Reset)
{
    float index = 1.0;
    float NewError, Result;
    static float LastError = 0.0, Integral = 0.0;

    NewError = Set - Real;
    if (Reset == False)
    {
        if (myabs(NewError > IntegralRange))
        {
            index = 0.0;
        }
        else if (myabs(NewError < AllIntegralRange))
        {
            index = 1.0;
            Integral += NewError;
        }
        else
        {
            index = (IntegralRange - myabs(NewError)) / (IntegralRange - AllIntegralRange);
            Integral += NewError;
        }
        Result = SpeedPara.P * NewError + SpeedPara.I * index * Integral + SpeedPara.D * (NewError - LastError);
        LastError = NewError;
    }
    else if (Reset == True)
    {
        Result = 0;
        Integral = 0.0;
        LastError = 0.0;
    }

    return Result;
}

//右后轮速度控制pid
float RB_Speed_PID(float Set, float Real, char Reset)
{
    float index = 1.0;
    float NewError, Result;
    static float LastError = 0.0, Integral = 0.0;

    NewError = Set - Real;
    if (Reset == False)
    {
        if (myabs(NewError > IntegralRange))
        {
            index = 0.0;
        }
        else if (myabs(NewError < AllIntegralRange))
        {
            index = 1.0;
            Integral += NewError;
        }
        else
        {
            index = (IntegralRange - myabs(NewError)) / (IntegralRange - AllIntegralRange);
            Integral += NewError;
        }
        Result = SpeedPara.P * NewError + SpeedPara.I * index * Integral + SpeedPara.D * (NewError - LastError);
        LastError = NewError;
    }
    else if (Reset == True)
    {
        Result = 0.0;
        Integral = 0.0;
        LastError = 0.0;
    }

    return Result;
}
