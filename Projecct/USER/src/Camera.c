#include "headfile.h"

//摄像头初始化
void Camera_Init(void)
{
   ov7725_init();
}

//提取位移偏差,对齐线默认为 OV7725_H / 2, OV7725_W /2;
float Offset_Get(char mode, uint8 Set)
{
   char W, H;                                                     //宽度W 高度H
   char flag = 0, Position1, Position2;                           //Position 暂存边界值
   char transverse_err_x_[OV7725_H], transverse_err_y_[OV7725_W]; //x 与OV7725_W/2的偏差; y 与OV7725_H/2的偏差
   float sum_offset = 0.0;                                        //偏差累加和
   float offset = 0.0;                                            //偏差 返回值

   uint8 image_L_border[OV7725_H];
   uint8 image_R_border[OV7725_H]; //横向扫描左右边界
   uint8 image_U_border[OV7725_W];
   uint8 image_D_border[OV7725_W];                         //纵向扫描上下边界
   uint8 image_line_x_[OV7725_H], image_line_y_[OV7725_W]; //x,y向中线提取

   //X轴偏差提取
   if (mode == 'X')
   {
      //获取左右边界
      for (H = OV7725_H - 1; H >= 0; H--)
      {
         for (W = 0; W < OV7725_W - 1; W++)
         {
            if (image_dec[H][W] == 255)
            {
               if ((image_dec[H][W + 1] == 0))
               {
                  flag = 1;
                  Position1 = W; //白跳黑 左边界存储
               }
            }
            if ((image_dec[H][W] == 0) && (flag == 1))
            {
               if (image_dec[H][W + 1] == 255)
               {
                  flag = 2;
                  Position2 = W + 1; //黑跳白 右边界存储
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 5))
               {
                  image_L_border[H] = Position1;
                  image_R_border[H] = Position2;
               }
            }
            if (((Position1 - Position2) < 15) && ((Position1 - Position2) > 0) && (flag == 2)) //双线提取中间白线
            {
               flag = 0;
               image_R_border[H] = Position1;
               image_L_border[H] = Position2;
            }
         }
      }
      //计算偏差
      for (H = 0; H < OV7725_H; H++)
      {
         image_line_x_[H] = (int)(image_L_border[H] + image_R_border[H]) / 2; //由边界得出中线位置
         transverse_err_x_[H] = image_line_x_[H] - Set;
      }
      for (H = 10; H < 50; H++)
      {
         sum_offset += transverse_err_x_[H];
      }
      offset = sum_offset / 40;
   }

   //Y轴偏差提取
   if (mode == 'Y')
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
                  flag = 1;
                  Position1 = H; //白跳黑 上边界存储
               }
            }
            if ((image_dec[H][W] == 0) && (flag == 1))
            {
               if (image_dec[H + 1][W] == 255)
               {
                  flag = 2;
                  Position2 = H + 1; //黑跳白 下边界存储
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 5))
               {
                  image_U_border[W] = Position1;
                  image_D_border[W] = Position2;
               }
            }
            // if (((Position1 - Position2) < 15) && ((Position1 - Position2) > 0) && (flag == 2))
            // {
            //    flag = 0;
            //    image_D_border[W] = Position1;
            //    image_U_border[W] = Position2;
            // }
         }
      }
      //计算偏差
      for (W = 0; W < OV7725_W; W++)
      {
         image_line_y_[W] = (int)(image_U_border[W] + image_D_border[W]) / 2; //由边界得出中线位置
         transverse_err_y_[W] = image_line_y_[W] - Set;
      }
      for (W = 20; W < 60; W++)
      {
         sum_offset += transverse_err_y_[W];
      }
      offset = sum_offset / 40;
   }

   return offset;
}

//提取角度偏差(返回值为角度的正切值)，在一定的范围内，正切值可以替代角度值，可以满足设计要求
float Angle_get(char mode)
{
   char W, H;                                             //宽度W 高度H
   char flag = 0, Position1, Position2;                   //Position 暂存边界值
   char linear_err_x_[OV7725_H], linear_err_y_[OV7725_W]; //x,y向直线偏差
   float sum_theta = 0;                                   //计算斜率时的各斜率累加和
   float tan_theta;                                       //直线斜率  返回值

   uint8 image_L_border[OV7725_H];
   uint8 image_R_border[OV7725_H]; //横向扫描左右边界
   uint8 image_U_border[OV7725_W];
   uint8 image_D_border[OV7725_W];                         //纵向扫描上下边界
   uint8 image_line_x_[OV7725_H], image_line_y_[OV7725_W]; //x,y向中线提取

   //X轴角度提取
   if (mode == 'X')
   {
      //获取左右边界
      for (H = OV7725_H - 1; H >= 0; H--)
      {
         for (W = 0; W < OV7725_W - 1; W++)
         {
            if (image_dec[H][W] == 255)
            {
               if ((image_dec[H][W + 1] == 0))
               {
                  flag = 1;
                  Position1 = W; //白跳黑 左边界存储
               }
            }
            if ((image_dec[H][W] == 0) && (flag == 1))
            {
               if (image_dec[H][W + 1] == 255)
               {
                  flag = 2;
                  Position2 = W + 1; //黑跳白 右边界存储
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 5))
               {
                  image_L_border[H] = Position1;
                  image_R_border[H] = Position2;
               }
            }
            if (((Position1 - Position2) < 20) && ((Position1 - Position2) > 0) && (flag == 2))
            {
               flag = 0;
               image_R_border[H] = Position1;
               image_L_border[H] = Position2;
            }
         }
      }
      //计算斜率
      for (H = OV7725_H - 1; H >= 0; H--)
      {
         image_line_x_[H] = (int)(image_L_border[H] + image_R_border[H]) / 2; //由边界得出中线位置
         linear_err_x_[H] = image_line_x_[H] - image_line_x_[OV7725_H - 1];
      }
      for (H = 10; H < 30; H++) //限幅滤波
      {
         if ((linear_err_x_[H] < 30) && (linear_err_x_[H] > -30))
            sum_theta += linear_err_x_[H] / (H * 1.0);
      }
      tan_theta = sum_theta * 10;
   }

   //Y轴角度提取
   if (mode == 'Y')
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
                  flag = 1;
                  Position1 = H; //白跳黑 上边界存储
               }
            }
            if ((image_dec[H][W] == 0) && (flag == 1))
            {
               if (image_dec[H + 1][W] == 255)
               {
                  flag = 2;
                  Position2 = H + 1; //黑跳白 下边界存储
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 5))
               {
                  image_U_border[W] = Position1;
                  image_D_border[W] = Position2;
               }
            }
            // if (((Position1 - Position2) < 15) && ((Position1 - Position2) > 0) && (flag == 2))
            // {
            //    flag = 0;
            //    image_D_border[W] = Position1;
            //    image_U_border[W] = Position2;
            // }
         }
      }
      //斜率计算
      for (W = 0; W < OV7725_W; W++)
      {
         image_line_y_[W] = (int)(image_U_border[W] + image_D_border[W]) / 2; //由边界得出中线位置
         linear_err_y_[W] = image_line_y_[W] - image_line_y_[0];
      }
      for (W = 20; W < 40; W++)
      {
         if ((linear_err_y_[W] < 30) && (linear_err_y_[W] > -30))
            sum_theta += linear_err_y_[W] / (W * 1.0);
      }
      tan_theta = sum_theta * 10;
   }
   return tan_theta;
}

//摄像头测试
float X_Offset, Y_Offset;
float Angle_Offset_X, Angle_Offset_Y;

void Camera_Test(void)
{
   OLED_Init();
   Camera_Init();
   for(;;)
   {
      Image_Decompression(image_bin,image_dec[0]);
      dis_bmp(60,80,image_dec[0],0x7F);
      X_Offset = Offset_Get('X', OV7725_W / 2);
      Y_Offset = Offset_Get('Y', OV7725_H / 2);
      Angle_Offset_X = Angle_get('X');
      Angle_Offset_Y = Angle_get('Y');
   }
}