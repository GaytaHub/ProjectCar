#include "headfile.h"

/*思路：固定坐标系，摄像头只看向一个方向，分为速度、偏移量、角度三环闭合，速度和偏移量分为X、Y两个方向，
将三个环、两个方向的控制量全部叠加作为电机的PWM输入，将运动控制封装成函数，
输入X或Y方向(沿一个方向运动时，把另一个方向速度设置为0，同一时刻只有
一个方向的速度与另一个方向的偏差)的速度、偏差以及角度偏差，
即可控制车体沿不同方向移动，移动过程中记录当前坐标，移动至指定坐标时进入微调，
当进入微调模式时，速度全部设置为零，输入偏差除已有的横向偏移量与角度偏移量，
再加上另一方向上的定位偏差(X轴方向的角度偏差与横向偏差加上Y方向的定位偏差)，即可实现精确定位*/
//全局坐标系：出发点向右为X轴正方向，出发点向前为Y轴正方向，逆时针为正方向
//局部坐标系(以车体为参考点)：与全局坐标系方向相同
//编码器方向：从车轮外向内看，逆时针旋转为正值
//电机输出极性：电机输出为正值时应使轮子逆时针转动

/*车体单方向运行时，轮子旋转方向(正负号表示编码器读取方向)：
Y+: LF——逆时针(+) RF——顺时针(-) LB——逆时针(+) RB——顺时针(-)
X+: LF——顺时针(-) RF——顺时针(-) LB——逆时针(+) RB——逆时针(+)
Y-: LF——顺时针(-) RF——逆时针(+) LB——顺时针(-) RB——逆时针(+)
X-: LF——逆时针(+) RF——逆时针(+) LB——顺时针(-) RB——顺时针(-)
A+: 全为顺时针(-)
A-: 全为逆时针(+)
由此车体速度可表示为(正值表示与坐标系正方向同向)：
YSpeedReal = LF_W_Real - RF_W_Real + LB_W_Real - RB_W_Real
XSpeedReal = -LF_W_Real - RF_W_Real + LB_W_Real + RB_W_Real*/

/*偏差 即车体X、Y轴到黑线的垂直距离：
(X_Error:Y轴在黑线右侧为正，需沿X-移动消除偏差，Y_Error:X轴在黑线左侧为正,需沿Y-移动消除偏差，设定值为0)
——> X_Error ——> X_Error_PWM = PID(X_Error)
——> Y_Error ——> Y_Error_PWM = PID(Y_Error)*/
/*角度 即车体X、Y轴与黑线的夹角(从X、Y轴转动到黑线得到的夹角，逆时针为正，需沿顺时针旋转消除偏差，设定值为0) 
——> A_Error ——> A_PWM = PID(A_Error)*/
/*X、Y轴速度 即车体沿X、Y轴的移动速度(车体沿X、Y轴正方向行驶为正，设定值为某一固定的正值) 
——> X_Speed_Error = X_Speed_Set - XSpeedReal ——> X_Speed_PWM = PID(X_Error)
——> Y_Speed_Error = Y_Speed_Set - YSpeedReal ——> Y_Speed_PWM = PID(Y_Error)*/
/*车轮转速 即单位时间编码器脉冲数(设定值由前三者自然得出，前三者闭环即可闭环这一环节)*/
/*PWM为正值时使W为正*/

/*沿X、Y轴方向行驶叠加：
LF_PWM = - X_Speed_PWM - Y_Error_PWM + Y_Speed_PWM + X_Error_PWM - A_PWM
RF_PWM = - X_Speed_PWM + Y_Error_PWM - Y_Speed_PWM + X_Error_PWM - A_PWM
LB_PWM = + X_Speed_PWM - Y_Error_PWM + Y_Speed_PWM - X_Error_PWM - A_PWM
RB_PWM = + X_Speed_PWM + Y_Error_PWM - Y_Speed_PWM - X_Error_PWM - A_PWM*/

//摄像头视野偏差提取：
/*摄像头看向全局坐标系的X轴正方向*/
/*摄像头偏差符号定向：
规定摄像头视野铅锤中线，在与X轴平行的黑线左侧时，偏差为正，从中线旋转到黑线得到的角，
逆时针为正；摄像头视野水平中线，在与Y轴平行的黑线上方时，偏差为正，从中线旋转到黑线
得到的角，逆时针为正*/

//X轴偏差控制PID  
extern struct Data ErrorPara; 
int16 X_Error_PD(float Error, char Reset) {
    static float LastError = 0.0; 
    float Result; 
    
    if (Reset == True) {
        Result = 0; 
        LastError = 0; 
    }
    else if (Reset == False) {
        Result = ErrorPara.P * Error + ErrorPara.D * (Error - LastError);
        LastError = Error;
    }   
    
    return (int16)Result; 
}

//Y轴偏差控制
int16 Y_Error_PD(float Error, char Reset) {
    static float LastError = 0.0; 
    float Result; 
    
    if (Reset == True) {
        Result = 0; 
        LastError = 0; 
    }
    else if (Reset == False) {
        Result = ErrorPara.P * Error + ErrorPara.D * (Error - LastError);
        LastError = Error;
    }   
    
    return (int16)Result; 
}

//角度控制PID
extern struct Data AnglePara; 
int16 Angle_PD(float AngleError, char Reset) {
    static float AngleLastError = 0.0; 
    float Result; 
    
    if (Reset == True) {
        Result = 0; 
        AngleLastError = 0; 
    }
    else if (Reset == False) {
        Result = AnglePara.P * AngleError + AnglePara.D * (AngleError - AngleLastError); 
        AngleLastError = AngleError; 
    }
    
    return (int16)Result; 
}

//X轴速度控制PID
extern struct Data SpeedPara; 
#define IntegralRange 20      //积分范围
#define AllIntegralRange 5   //全积分范围
int16 X_Speed_PID(float Set, float Real, char Reset) {
    float index = 1.0; 
    float NewError, Result; 
    static float LastError = 0.0, Integral = 0.0; 
    
    NewError = Set - Real; 
    if (Reset == False) {
        if (myabs(NewError > IntegralRange)) {
            index = 0.0; 
        }
        else if (myabs(NewError < AllIntegralRange)) {
            index = 1.0; 
            Integral += NewError; 
        }
        else {
            index = (IntegralRange - myabs(NewError))/(IntegralRange - AllIntegralRange); 
            Integral += NewError; 
        }
        Result = SpeedPara.P * NewError + SpeedPara.I * index * Integral + SpeedPara.D * (NewError - LastError); 
        LastError = NewError;        
    }
    else if (Reset == True) {
        Result = 0; 
        Integral = 0.0; 
        LastError = 0.0; 
    }
    
    return (int16)Result; 
}

//Y轴速度控制
int16 Y_Speed_PID(float Set, float Real, char Reset) {
    float index = 1.0; 
    float NewError, Result; 
    static float LastError = 0.0, Integral = 0.0; 
    
    NewError = Set - Real; 
    if (Reset == False) {
        if (myabs(NewError > IntegralRange)) {
            index = 0.0; 
        }
        else if (myabs(NewError < AllIntegralRange)) {
            index = 1.0; 
            Integral += NewError; 
        }
        else {
            index = (IntegralRange - myabs(NewError))/(IntegralRange - AllIntegralRange); 
            Integral += NewError; 
        }
        Result = SpeedPara.P * NewError + SpeedPara.I * index * Integral + SpeedPara.D * (NewError - LastError); 
        LastError = NewError;        
    }
    else if (Reset == True) {
        Result = 0; 
        Integral = 0.0; 
        LastError = 0.0; 
    }
    
    return (int16)Result; 
}
