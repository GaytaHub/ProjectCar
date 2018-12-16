#include "headfile.h"

//摄像头初始化
void Camera_Init(void)
{
   ov7725_init();
}
//x 横向扫描(对应竖线) y 纵向扫描(对应横线) 获取偏差（直线与屏幕中央的偏差）
//x 横向扫描(对应竖线) y 纵向扫描(对应横线) 获取斜线斜率大小
//两个输入量 coordinate（'x'或'y'对应不同坐标） value（offset或theta 对应偏差提取与斜率提取）
float Camera_Get(char Axis, char Mode)
{
   char W, H;                           //宽度W 高度H
   char Flag = 0, Position1, Position2; //Position 暂存边界值
   float TransverseErrX[OV7725_H];       //x 与OV7725_W/2的偏差
   char TransverseErrY[OV7725_W];       //y 与OV7725_H/2的偏差
   float LinearErrX[OV7725_H];           //x向直线偏差
   char LinearErrY[OV7725_W];           //y向直线偏差
   float SumOffset = 0.0;               //偏差累加和
   float SumTheta = 0;                  //计算斜率时的各斜率累加和
   float offset;                        //偏差 返回值
   float tantheta;                      //直线斜率  返回值

   uint8 ImageLBorder[OV7725_H];
   uint8 ImageRBorder[OV7725_H]; //横向扫描左右边界
   uint8 ImageUBorder[OV7725_W];
   uint8 ImageDBorder[OV7725_W]; //纵向扫描上下边界
   float ImageLineX[OV7725_H];   //x向中线提取
   uint8 ImageLineY[OV7725_W];   //y向中线提取

   //优先确定坐标系
   if (Axis == 'X')
   {
      //获取左右边界
      for (H = OV7725_H - 1; H >= 0; H--)
      {
         for (W = 20; W < OV7725_W - 20; W++)
         {
            if (image_dec[H][W] == 255)
            {
               if ((image_dec[H][W + 1] == 0))
               {
                  Flag = 1;
                  Position1 = W; //白跳黑 左边界存储
               }
            }
            if ((image_dec[H][W] == 0) && (Flag == 1))
            {
               if (image_dec[H][W + 1] == 255)
               {
                  Flag = 2;
                  Position2 = W + 1; //黑跳白 右边界存储
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 1))
               {
                  ImageLBorder[H] = Position1;
                  ImageRBorder[H] = Position2;
               }
            }
            if (((Position1 - Position2) < 15) && ((Position1 - Position2) > 3) && (Flag == 2))
            {
               Flag = 0;
               ImageRBorder[H] = Position1;
               ImageLBorder[H] = Position2;
            }
         }
      }

      //由边界得出中线位置
      for (H = 0; H < OV7725_H; H++)
      {
         ImageLineX[H] = (ImageLBorder[H] + ImageRBorder[H]) / 2.0;
         TransverseErrX[H] = ImageLineX[H] - OV7725_W / 2.0;
      }
      //中线滤波
      for (H = 1; H < OV7725_H; H++)
      {
         if (myabs(ImageLineX[H] - ImageLineX[H - 1]) > 3)
            ImageLineX[H] = ImageLineX[H - 1];
      }

      //然后确定被提取量
      if (Mode == 'E')
      {
         //计算偏差
         for (H = 0; H < 60; H++)
         {
            SumOffset += TransverseErrX[H];
         }
         offset = SumOffset / 60;
         return offset;
      }

      if (Mode == 'T')
      {
         //计算斜率
         for (H = 0; H < OV7725_H; H++)
         {
            LinearErrX[H] = ImageLineX[H] - ImageLineX[OV7725_H - 1];
         }

         for (H = 5; H < OV7725_H - 5; H++)
         {
            if ((LinearErrX[H] < 30) && (LinearErrX[H] > -30))
               SumTheta += LinearErrX[OV7725_H - H] / (H * 1.0);
         }
         tantheta = SumTheta;
         return tantheta;
      }
   }

   else if (Axis == 'Y')
   {
      //获取上下边界
      for (W = 0; W < OV7725_W; W++)
      {
         for (H = 0; H < OV7725_H - 1; H++)
         {
            if (image_dec[H][W] == 255)
            {
               if (image_dec[H + 1][W] == 0)
               {
                  Flag = 1;
                  Position1 = H; //白跳黑 上边界存储
               }
            }
            if ((image_dec[H][W] == 0) && (Flag == 1))
            {
               if (image_dec[H + 1][W] == 255)
               {
                  Flag = 2;
                  Position2 = H + 1; //黑跳白 下边界存储
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 3))
               {
                  ImageUBorder[W] = Position1;
                  ImageDBorder[W] = Position2;
               }
            }
            if (((Position1 - Position2) < 15) && ((Position1 - Position2) > 0) && (Flag == 2))
            {
               Flag = 0;
               ImageDBorder[W] = Position1;
               ImageUBorder[W] = Position2;
            }
         }
      }

      if (Mode == 'E')
      {
         //计算偏差
         for (W = 0; W < OV7725_W; W++)
         {
            ImageLineY[W] = (int)(ImageUBorder[W] + ImageDBorder[W]) / 2; //由边界得出中线位置
            TransverseErrY[W] = ImageLineY[W] - OV7725_H / 2;
         }
         for (W = 20; W < 60; W++)
         {
            SumOffset += TransverseErrY[W];
         }
         offset = SumOffset / 40;
         return offset;
      }

      if (Mode == 'T')
      {
         //斜率计算
         for (W = 0; W < OV7725_W; W++)
         {
            ImageLineY[W] = (int)(ImageUBorder[W] + ImageDBorder[W]) / 2; //由边界得出中线位置
         }

         for (W = 0; W < OV7725_W; W++)
         {
            LinearErrY[W] = ImageLineY[W] - ImageLineY[0];
         }

         for (W = 20; W < 40; W = W + 2)
         {
            if ((LinearErrY[W] < 30) && (LinearErrY[W] > -30))
               SumTheta += LinearErrY[W] / (W * 1.0);
         }
         tantheta = SumTheta * 10;
         return tantheta;
      }
   }
   return 0;
}

//摄像头测试程序
void Camera_Test(void)
{
   float XError, YError;
   float XTheta, YTheta;
   Camera_Init();
   OLED_Init();
   for (;;)
   {
      Image_Decompression(image_bin, image_dec[0]); //摄像头图像解压
      dis_bmp(60, 80, image_dec[0], 0x7F);
      XError = Camera_Get('X', 'E');
      XTheta = Camera_Get('X', 'T');
   }
}