#include "headfile.h"

//����ͷ��ʼ��
void Camera_Init(void)
{
   ov7725_init();
}

//��ȡλ��ƫ��,������Ĭ��Ϊ OV7725_H / 2, OV7725_W /2;
float Offset_Get(char mode, uint8 Set)
{
   char W, H;                                                     //���W �߶�H
   char flag = 0, Position1, Position2;                           //Position �ݴ�߽�ֵ
   char transverse_err_x_[OV7725_H], transverse_err_y_[OV7725_W]; //x ��OV7725_W/2��ƫ��; y ��OV7725_H/2��ƫ��
   float sum_offset = 0.0;                                        //ƫ���ۼӺ�
   float offset = 0.0;                                            //ƫ�� ����ֵ

   uint8 image_L_border[OV7725_H];
   uint8 image_R_border[OV7725_H]; //����ɨ�����ұ߽�
   uint8 image_U_border[OV7725_W];
   uint8 image_D_border[OV7725_W];                         //����ɨ�����±߽�
   uint8 image_line_x_[OV7725_H], image_line_y_[OV7725_W]; //x,y��������ȡ

   //X��ƫ����ȡ
   if (mode == 'X')
   {
      //��ȡ���ұ߽�
      for (H = OV7725_H - 1; H >= 0; H--)
      {
         for (W = 0; W < OV7725_W - 1; W++)
         {
            if (image_dec[H][W] == 255)
            {
               if ((image_dec[H][W + 1] == 0))
               {
                  flag = 1;
                  Position1 = W; //������ ��߽�洢
               }
            }
            if ((image_dec[H][W] == 0) && (flag == 1))
            {
               if (image_dec[H][W + 1] == 255)
               {
                  flag = 2;
                  Position2 = W + 1; //������ �ұ߽�洢
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 5))
               {
                  image_L_border[H] = Position1;
                  image_R_border[H] = Position2;
               }
            }
            if (((Position1 - Position2) < 15) && ((Position1 - Position2) > 0) && (flag == 2)) //˫����ȡ�м����
            {
               flag = 0;
               image_R_border[H] = Position1;
               image_L_border[H] = Position2;
            }
         }
      }
      //����ƫ��
      for (H = 0; H < OV7725_H; H++)
      {
         image_line_x_[H] = (int)(image_L_border[H] + image_R_border[H]) / 2; //�ɱ߽�ó�����λ��
         transverse_err_x_[H] = image_line_x_[H] - Set;
      }
      for (H = 10; H < 50; H++)
      {
         sum_offset += transverse_err_x_[H];
      }
      offset = sum_offset / 40;
   }

   //Y��ƫ����ȡ
   if (mode == 'Y')
   {
      //��ȡ���±߽�
      for (W = 0; W < OV7725_W; W++)
      {
         for (H = 0; H < OV7725_H - 1; H++)
         {
            if (image_dec[H][W] == 255)
            {
               if (image_dec[H + 1][W] == 0)
               {
                  flag = 1;
                  Position1 = H; //������ �ϱ߽�洢
               }
            }
            if ((image_dec[H][W] == 0) && (flag == 1))
            {
               if (image_dec[H + 1][W] == 255)
               {
                  flag = 2;
                  Position2 = H + 1; //������ �±߽�洢
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
      //����ƫ��
      for (W = 0; W < OV7725_W; W++)
      {
         image_line_y_[W] = (int)(image_U_border[W] + image_D_border[W]) / 2; //�ɱ߽�ó�����λ��
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

//��ȡ�Ƕ�ƫ��(����ֵΪ�Ƕȵ�����ֵ)����һ���ķ�Χ�ڣ�����ֵ��������Ƕ�ֵ�������������Ҫ��
float Angle_get(char mode)
{
   char W, H;                                             //���W �߶�H
   char flag = 0, Position1, Position2;                   //Position �ݴ�߽�ֵ
   char linear_err_x_[OV7725_H], linear_err_y_[OV7725_W]; //x,y��ֱ��ƫ��
   float sum_theta = 0;                                   //����б��ʱ�ĸ�б���ۼӺ�
   float tan_theta;                                       //ֱ��б��  ����ֵ

   uint8 image_L_border[OV7725_H];
   uint8 image_R_border[OV7725_H]; //����ɨ�����ұ߽�
   uint8 image_U_border[OV7725_W];
   uint8 image_D_border[OV7725_W];                         //����ɨ�����±߽�
   uint8 image_line_x_[OV7725_H], image_line_y_[OV7725_W]; //x,y��������ȡ

   //X��Ƕ���ȡ
   if (mode == 'X')
   {
      //��ȡ���ұ߽�
      for (H = OV7725_H - 1; H >= 0; H--)
      {
         for (W = 0; W < OV7725_W - 1; W++)
         {
            if (image_dec[H][W] == 255)
            {
               if ((image_dec[H][W + 1] == 0))
               {
                  flag = 1;
                  Position1 = W; //������ ��߽�洢
               }
            }
            if ((image_dec[H][W] == 0) && (flag == 1))
            {
               if (image_dec[H][W + 1] == 255)
               {
                  flag = 2;
                  Position2 = W + 1; //������ �ұ߽�洢
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
      //����б��
      for (H = OV7725_H - 1; H >= 0; H--)
      {
         image_line_x_[H] = (int)(image_L_border[H] + image_R_border[H]) / 2; //�ɱ߽�ó�����λ��
         linear_err_x_[H] = image_line_x_[H] - image_line_x_[OV7725_H - 1];
      }
      for (H = 10; H < 30; H++) //�޷��˲�
      {
         if ((linear_err_x_[H] < 30) && (linear_err_x_[H] > -30))
            sum_theta += linear_err_x_[H] / (H * 1.0);
      }
      tan_theta = sum_theta * 10;
   }

   //Y��Ƕ���ȡ
   if (mode == 'Y')
   {
      //��ȡ���±߽�
      for (W = 0; W < OV7725_W; W++)
      {
         for (H = 0; H < OV7725_H - 1; H++)
         {
            if (image_dec[H][W] == 255)
            {
               if (image_dec[H + 1][W] == 0)
               {
                  flag = 1;
                  Position1 = H; //������ �ϱ߽�洢
               }
            }
            if ((image_dec[H][W] == 0) && (flag == 1))
            {
               if (image_dec[H + 1][W] == 255)
               {
                  flag = 2;
                  Position2 = H + 1; //������ �±߽�洢
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
      //б�ʼ���
      for (W = 0; W < OV7725_W; W++)
      {
         image_line_y_[W] = (int)(image_U_border[W] + image_D_border[W]) / 2; //�ɱ߽�ó�����λ��
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

//����ͷ����
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