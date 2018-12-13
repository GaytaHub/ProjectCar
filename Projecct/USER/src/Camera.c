#include "headfile.h"

//摄像头初始化
void Camera_Init(void)
{
   ov7725_init();
}

//提取位移偏差,对齐线默认为 OV7725_H / 2, OV7725_W /2;
float Error_Get(char mode, int16 Set)
{
   int16 Weight = 0, Hight = 0, Position1 = 0, Position2 = 0; //宽度Weight 高度Hight,Position 暂存边界值
   char flag = 0;                                     //双线检测标志
   int16 XError[OV7725_H], YError[OV7725_W];         //x 与OV7725_W/2的偏差; y 与OV7725_H/2的偏差
   float SumError = 0.0;                              //偏差累加和
   float Result = 0.0;                                //偏差 返回值

   int16 LeftBorder[OV7725_H];
   int16 RightBorder[OV7725_H]; //横向扫描左右边界
   int16 UpBorder[OV7725_W];
   int16 DownBorder[OV7725_W];                   //纵向扫描上下边界
   int16 XMidline[OV7725_H], YMidline[OV7725_W]; //提取黑线中值

   //X轴偏差提取
   if (mode == 'X')
   {
      //获取左右边界
      for (Hight = OV7725_H - 1; Hight >= 0; Hight--)
      {
         for (Weight = 0; Weight < OV7725_W - 1; Weight++)
         {
            if (image_dec[Hight][Weight] == 255)
            {
               if ((image_dec[Hight][Weight + 1] == 0))
               {
                  flag = 1;
                  Position1 = Weight; //白跳黑 左边界存储
               }
            }
            if ((image_dec[Hight][Weight] == 0) && (flag == 1))
            {
               if (image_dec[Hight][Weight + 1] == 255)
               {
                  flag = 2;
                  Position2 = Weight + 1; //黑跳白 右边界存储
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 5))
               {
                  LeftBorder[Hight] = Position1;
                  RightBorder[Hight] = Position2;
               }
            }
            if (((Position1 - Position2) < 15) && ((Position1 - Position2) > 0) && (flag == 2)) //双线提取中间白线
            {
               flag = 0;
               RightBorder[Hight] = Position1;
               LeftBorder[Hight] = Position2;
            }
         }
      }
      //计算偏差
      for (Hight = 0; Hight < OV7725_H; Hight++)
      {
         XMidline[Hight] = (int)(LeftBorder[Hight] + RightBorder[Hight]) / 2; //由边界得出中线位置
         XError[Hight] = XMidline[Hight] - Set;
      }
      for (Hight = 10; Hight < 50; Hight++)
      {
         SumError += XError[Hight];
      }
      Result = SumError / 40;
   }

   //Y轴偏差提取
   if (mode == 'Y')
   {
      //获取上下边界
      for (Weight = 0; Weight < OV7725_W; Weight++)
      {
         for (Hight = 0; Hight < OV7725_H - 1; Hight++)
         {
            if (image_dec[Hight][Weight] == 255)
            {
               if (image_dec[Hight + 1][Weight] == 0)
               {
                  flag = 1;
                  Position1 = Hight; //白跳黑 上边界存储
               }
            }
            if ((image_dec[Hight][Weight] == 0) && (flag == 1))
            {
               if (image_dec[Hight + 1][Weight] == 255)
               {
                  flag = 2;
                  Position2 = Hight + 1; //黑跳白 下边界存储
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 5))
               {
                  UpBorder[Weight] = Position1;
                  DownBorder[Weight] = Position2;
               }
            }
         }
      }
      //计算偏差
      for (Weight = 0; Weight < OV7725_W; Weight++)
      {
         YMidline[Weight] = (int)(UpBorder[Weight] + DownBorder[Weight]) / 2; //由边界得出中线位置
         YError[Weight] = YMidline[Weight] - Set;
      }
      for (Weight = 20; Weight < 60; Weight++)
      {
         SumError += YError[Weight];
      }
      Result = SumError / 40;
   }

   return Result;
}

//提取角度偏差(返回值为角度的正切值)，在一定的范围内，正切值可以替代角度值，可以满足设计要求
float Angle_Get(char mode)
{
   int16 Weight = 0, Hight = 0, Position1 = 0, Position2 = 0; //宽度Weight 高度Hight,Position 暂存边界值
   char flag = 0;                                     //双线标志位
   int16 XError[OV7725_H], YError[OV7725_W];           //x,y向误差
   float SumTheta = 0;                               //计算斜率时的各斜率累加和
   float Result;                                   //直线斜率  返回值

   int16 LeftBorder[OV7725_H];
   int16 RightBorder[OV7725_H]; //横向扫描左右边界
   int16 UpBorder[OV7725_W];
   int16 DownBorder[OV7725_W];                   //纵向扫描上下边界
   int16 XMidline[OV7725_H], YMidline[OV7725_W]; //x,y向中线提取

   //X轴角度提取
   if (mode == 'X')
   {
      //获取左右边界
      for (Hight = OV7725_H - 1; Hight >= 0; Hight--)
      {
         for (Weight = 0; Weight < OV7725_W - 1; Weight++)
         {
            if (image_dec[Hight][Weight] == 255)
            {
               if ((image_dec[Hight][Weight + 1] == 0))
               {
                  flag = 1;
                  Position1 = Weight; //白跳黑 左边界存储
               }
            }
            if ((image_dec[Hight][Weight] == 0) && (flag == 1))
            {
               if (image_dec[Hight][Weight + 1] == 255)
               {
                  flag = 2;
                  Position2 = Weight + 1; //黑跳白 右边界存储
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 5))
               {
                  LeftBorder[Hight] = Position1;
                  RightBorder[Hight] = Position2;
               }
            }
            if (((Position1 - Position2) < 20) && ((Position1 - Position2) > 0) && (flag == 2))
            {
               flag = 0;
               RightBorder[Hight] = Position1;
               LeftBorder[Hight] = Position2;
            }
         }
      }
      //计算斜率
      for (Hight = OV7725_H - 1; Hight >= 0; Hight--)
      {
         XMidline[Hight] = (int)(LeftBorder[Hight] + RightBorder[Hight]) / 2; //由边界得出中线位置
         XError[Hight] = XMidline[Hight] - XMidline[OV7725_H - 1];
      }
      for (Hight = 10; Hight < 30; Hight++) //限幅滤波
      {
         if ((XError[Hight] < 30) && (XError[Hight] > -30))
            SumTheta += XError[Hight] / (Hight * 1.0);
      }
      Result = SumTheta * 10;
   }

   //Y轴角度提取
   if (mode == 'Y')
   {
      //获取上下边界
      for (Weight = 0; Weight < OV7725_W; Weight++)
      {
         for (Hight = 0; Hight < OV7725_H - 1; Hight++)
         {
            if (image_dec[Hight][Weight] == 255)
            {
               if (image_dec[Hight + 1][Weight] == 0)
               {
                  flag = 1;
                  Position1 = Hight; //白跳黑 上边界存储
               }
            }
            if ((image_dec[Hight][Weight] == 0) && (flag == 1))
            {
               if (image_dec[Hight + 1][Weight] == 255)
               {
                  flag = 2;
                  Position2 = Hight + 1; //黑跳白 下边界存储
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 5))
               {
                  UpBorder[Weight] = Position1;
                  DownBorder[Weight] = Position2;
               }
            }
         }
      }
      //斜率计算
      for (Weight = 0; Weight < OV7725_W; Weight++)
      {
         YMidline[Weight] = (int)(UpBorder[Weight] + DownBorder[Weight]) / 2; //由边界得出中线位置
         YError[Weight] = YMidline[Weight] - YMidline[0];
      }
      for (Weight = 20; Weight < 40; Weight++)
      {
         if ((YError[Weight] < 30) && (YError[Weight] > -30))
            SumTheta += YError[Weight] / (Weight * 1.0);
      }
      Result = SumTheta * 10;
   }
   return Result;
}

//摄像头测试
//float XError, YError;
//float XAngleError, YAngleError;
//
//void Camera_Test(void)
//{
//   OLED_Init();
//   Camera_Init();
//   for (;;)
//   {
//      Image_Decompression(image_bin, image_dec[0]);
//      dis_bmp(60, 80, image_dec[0], 0x7F);
//      XError = Error_Get('X', OV7725_W / 2);
//      YError = Error_Get('Y', OV7725_H / 2);
//      XAngleError = Angle_Get('X');
//      YAngleError = Angle_Get('Y');
//   }
//}