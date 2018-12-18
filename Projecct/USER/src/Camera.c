#include "headfile.h"

/**********摄像头初始化**************/
void Camera_Init(void)
{
   ov7725_init();
}

/*************从左向右扫描得到中线值**************/
float LeftToRightMidline[OV7725_H] = {0}; //中线提取值
void LeftToRight_Scan(void)
{
   int LeftBorder[OV7725_H];  //横向扫描左边界
   int RightBorder[OV7725_H]; //横向扫描右边界
   int Width = 0, Height = 0; //循环变量
   int First = 0;             //双线左边界扫描标志位
   int Border1, Border2;      //边界暂存值

   //获取左右边界
   //从上向下扫描
   for (Height = 0; Height < OV7725_H; Height++)
   {
      //每次从左向右扫描前重置白线检测标志
      //扫描前重置边界暂存值
      First = False;
      Border1 = 0;
      Border2 = 0;
#define LeftToRightScanRange 8 //图像腐蚀范围
      //从左向右扫描
      for (Width = LeftToRightScanRange; Width < OV7725_W - 1 - LeftToRightScanRange; Width++)
      {
         //白跳黑
         if (image_dec[Height][Width] == 255 && First == False)
         {
            if ((image_dec[Height][Width + 1] == 0))
            {
               First = True;    //第一次扫到白跳黑边界
               Border1 = Width; //边界存储
            }
         }
         //黑跳白
         if (image_dec[Height][Width] == 0) //储存所有扫到的黑跳白边界
         {
            if (image_dec[Height][Width + 1] == 255)
            {
               Border2 = Width; //边界存储
            }
         }
      }
      LeftBorder[Height] = Border1;
      RightBorder[Height] = Border2;
   }
   //由边界得出中线位置,从上向下计算
   for (Height = 0; Height < OV7725_H; Height++)
   {
      LeftToRightMidline[Height] = (LeftBorder[Height] + RightBorder[Height]) / 2.0;
   }
   //中线滤波
   for (Height = 1; Height < OV7725_H; Height++)
   {
      if (myabs(LeftToRightMidline[Height] - LeftToRightMidline[Height - 1]) > 2.0)
         LeftToRightMidline[Height] = LeftToRightMidline[Height - 1] * 0.8 + LeftToRightMidline[Height] * 0.2;
   }
}

/**************************从上向下扫描得到中线值***************************/
float UpToDownMidline[OV7725_W] = {0}; //中线提取值
void UpToDown_Scan(void)
{
   int UpBorder[OV7725_W];    //横向扫描左边界
   int DownBorder[OV7725_W];  //横向扫描右边界
   int Width = 0, Height = 0; //循环变量
   int Border1, Border2;      //边界暂存值

   //获取上下边界
   //从左向右扫描
   for (Width = 0; Width < OV7725_W; Width++)
   {
      //扫描前重置边界暂存值
      Border1 = 0;
      Border2 = 0;
#define UpToDownRange 12 //图像腐蚀范围
      //从上向下扫描
      for (Height = UpToDownRange; Height < OV7725_H - 1 - UpToDownRange; Height++)
      {
         //白跳黑
         if (image_dec[Height][Width] == 255)
         {
            if ((image_dec[Height + 1][Width] == 0))
            {
               Border1 = Height; //边界存储
            }
         }
         //黑跳白
         if (image_dec[Height][Width] == 0) //储存所有扫到的黑跳白边界
         {
            if (image_dec[Height + 1][Width] == 255)
            {
               Border2 = Height; //边界存储
            }
         }
      }
      UpBorder[Width] = Border1;
      DownBorder[Width] = Border2;
   }
   //由边界得出中线位置,从左向右计算
   for (Width = 0; Width < OV7725_W; Width++)
   {
      UpToDownMidline[Width] = (UpBorder[Width] + DownBorder[Width]) / 2.0;
   }
   //中线低通滤波
   for (Width = 1; Width < OV7725_W; Width++)
   {
      if (myabs(UpToDownMidline[Width] - UpToDownMidline[Width - 1]) > 1.5)
         UpToDownMidline[Width] = UpToDownMidline[Width - 1] * 0.7 + UpToDownMidline[Width] * 0.3;
   }
}

/*********************连续段中线滤波算法***************************/
float FilteringMidlineLength, FilteringMidlineHead, FilteringMidlineTail;
int Height, i = 1; //循环变量
float LineHead[60] = {0}, LineTail[60] = {0}, Length[60] = {0};
float Head = 0, Tail = 0;
void Midline_filter(void)
{

   for (Height = 0; Height < OV7725_H - 1; Height++)
   {
      if (myabs(LeftToRightMidline[Height] - LeftToRightMidline[Height - 1]) > 2)
      {
         Tail = Height;           //截取尾
         LineHead[i] = Head;      //记录头
         LineTail[i] = Tail;      //记录尾
         Length[i] = Tail - Head; //记录长度
         Head = Height;           //重置头
         i++;
         if (i == 60)
            i = 0;
      }
   }
   for (i = 1; i < 6; i++)
   {
      //最大连续段记录
      if (Length[i] > Length[i - 1])
      {
         FilteringMidlineLength = Length[i];
         FilteringMidlineHead = LineHead[i];
         FilteringMidlineTail = LineTail[i];
      }
   }
}

/**********************误差提取***************************/
float Error_Get(char Axis, char Mode)
{
   int Width = 0, Height = 0;                                             //循环变量
   float LeftToRightError[OV7725_H] = {0}, UpToDownError[OV7725_W] = {0}; //从左向右扫描时的误差,从上向下扫描时的误差
   float SumDownReferencePoint = 0, SumUpReferencePoint = 0;              //从左向右扫描时参考点位置总和
   float SumLeftReferencePoint = 0, SumRightReferencePoint = 0;           //从上向下扫描时参考点位置总和
   float DownReferencePoint = 0, UpReferencePoint = 0;                    //从左向右扫描时参考点位置
   float LeftReferencePoint = 0, RightReferencePoint = 0;                 //从上向下扫描时参考点位置
   float SumError = 0.0;                                                  //总误差
   float Result;                                                          //结果

   //沿X轴移动时误差提取
   if (Axis == 'X')
   {
      //提取误差量
      if (Mode == 'E')
      {
         //计算偏差
         for (Height = 0; Height < OV7725_H; Height++)
         {
            LeftToRightError[Height] = LeftToRightMidline[Height] - OV7725_W / 2.0; //计算与视野中线的误差，为满足极性要求，用黑线减去中线
            SumError += LeftToRightError[Height];
         }
         Result = SumError / OV7725_H;

         return Result;
      }
      //提取偏移角
      else if (Mode == 'T')
      {
#define ReferrncePointNum 6
         //计算下参考点位置(以下方N个点为参考)
         for (Height = 0; Height < ReferrncePointNum; Height++)
         {
            SumUpReferencePoint += LeftToRightMidline[Height];
         }
         //计算上参考点位置
         for (Height = OV7725_H - 1; Height >= OV7725_H - ReferrncePointNum; Height--)
         {
            SumDownReferencePoint += LeftToRightMidline[Height];
         }
         UpReferencePoint = SumUpReferencePoint / ReferrncePointNum;                              //求取上参考点平均位置
         DownReferencePoint = SumDownReferencePoint / ReferrncePointNum;                          //求取下参考点平均位置
         Result = (DownReferencePoint - UpReferencePoint) / (OV7725_H - ReferrncePointNum) * 1.0; //为满足摄像头角度极性设定，用下减上

         return Result * 10;
      }
   }
   //沿Y轴移动时误差提取
   else if (Axis == 'Y')
   {
      //提取误差量
      if (Mode == 'E')
      {
         //计算偏差
         for (Width = 0; Width < OV7725_W; Width++)
         {
            UpToDownError[Width] = OV7725_H / 2.0 - UpToDownMidline[Width]; //计算黑线与视野中线的误差，为满足极性要求用中线减去黑线
            SumError += UpToDownError[Width];
         }
         Result = SumError / OV7725_W;

         return Result;
      }
      //提取偏移角
      else if (Mode == 'T')
      {
         //计算左参考点位置(以左边N个点为参考)
         for (Width = 0; Width < ReferrncePointNum; Width++)
         {
            SumLeftReferencePoint += UpToDownMidline[Width];
         }
         //计算右参考点位置
         for (Width = OV7725_W - 1; Width >= OV7725_W - ReferrncePointNum; Width--)
         {
            SumRightReferencePoint += UpToDownMidline[Width];
         }
         LeftReferencePoint = SumLeftReferencePoint / ReferrncePointNum;                             //求取上参考点平均位置
         RightReferencePoint = SumRightReferencePoint / ReferrncePointNum;                           //求取下参考点平均位置
         Result = (LeftReferencePoint - RightReferencePoint) / (OV7725_W - ReferrncePointNum) * 1.0; //为满足极性要求，用左减右

         return Result * 10;
      }
   }

   return 0;    //没有选择时返回0
}

/********************************摄像头测试程序***************************/
void Camera_Test(void)
{
   static float XError, YError;
   static float XTheta, YTheta;
   Camera_Init();
   OLED_Init();
   for (;;)
   {
      Image_Decompression(image_bin, image_dec[0]); //摄像头图像解压
      dis_bmp(60, 80, image_dec[0], 0x7F);
      LeftToRight_Scan();           //扫描
      UpToDown_Scan();
      Midline_filter();             //中线滤波
      XError = Error_Get('X', 'E'); //误差量读取
      YError = Error_Get('Y', 'E');
      XTheta = Error_Get('X', 'T'); //偏移角读取
      YTheta = Error_Get('Y', 'T');
   }
}