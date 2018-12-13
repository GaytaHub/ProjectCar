#include "headfile.h"

//����ͷ��ʼ��
void Camera_Init(void)
{
   ov7725_init();
}

//��ȡλ��ƫ��,������Ĭ��Ϊ OV7725_H / 2, OV7725_W /2;
float Error_Get(char mode, int16 Set)
{
   int16 Weight = 0, Hight = 0, Position1 = 0, Position2 = 0; //���Weight �߶�Hight,Position �ݴ�߽�ֵ
   char flag = 0;                                     //˫�߼���־
   int16 XError[OV7725_H], YError[OV7725_W];         //x ��OV7725_W/2��ƫ��; y ��OV7725_H/2��ƫ��
   float SumError = 0.0;                              //ƫ���ۼӺ�
   float Result = 0.0;                                //ƫ�� ����ֵ

   int16 LeftBorder[OV7725_H];
   int16 RightBorder[OV7725_H]; //����ɨ�����ұ߽�
   int16 UpBorder[OV7725_W];
   int16 DownBorder[OV7725_W];                   //����ɨ�����±߽�
   int16 XMidline[OV7725_H], YMidline[OV7725_W]; //��ȡ������ֵ

   //X��ƫ����ȡ
   if (mode == 'X')
   {
      //��ȡ���ұ߽�
      for (Hight = OV7725_H - 1; Hight >= 0; Hight--)
      {
         for (Weight = 0; Weight < OV7725_W - 1; Weight++)
         {
            if (image_dec[Hight][Weight] == 255)
            {
               if ((image_dec[Hight][Weight + 1] == 0))
               {
                  flag = 1;
                  Position1 = Weight; //������ ��߽�洢
               }
            }
            if ((image_dec[Hight][Weight] == 0) && (flag == 1))
            {
               if (image_dec[Hight][Weight + 1] == 255)
               {
                  flag = 2;
                  Position2 = Weight + 1; //������ �ұ߽�洢
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 5))
               {
                  LeftBorder[Hight] = Position1;
                  RightBorder[Hight] = Position2;
               }
            }
            if (((Position1 - Position2) < 15) && ((Position1 - Position2) > 0) && (flag == 2)) //˫����ȡ�м����
            {
               flag = 0;
               RightBorder[Hight] = Position1;
               LeftBorder[Hight] = Position2;
            }
         }
      }
      //����ƫ��
      for (Hight = 0; Hight < OV7725_H; Hight++)
      {
         XMidline[Hight] = (int)(LeftBorder[Hight] + RightBorder[Hight]) / 2; //�ɱ߽�ó�����λ��
         XError[Hight] = XMidline[Hight] - Set;
      }
      for (Hight = 10; Hight < 50; Hight++)
      {
         SumError += XError[Hight];
      }
      Result = SumError / 40;
   }

   //Y��ƫ����ȡ
   if (mode == 'Y')
   {
      //��ȡ���±߽�
      for (Weight = 0; Weight < OV7725_W; Weight++)
      {
         for (Hight = 0; Hight < OV7725_H - 1; Hight++)
         {
            if (image_dec[Hight][Weight] == 255)
            {
               if (image_dec[Hight + 1][Weight] == 0)
               {
                  flag = 1;
                  Position1 = Hight; //������ �ϱ߽�洢
               }
            }
            if ((image_dec[Hight][Weight] == 0) && (flag == 1))
            {
               if (image_dec[Hight + 1][Weight] == 255)
               {
                  flag = 2;
                  Position2 = Hight + 1; //������ �±߽�洢
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 5))
               {
                  UpBorder[Weight] = Position1;
                  DownBorder[Weight] = Position2;
               }
            }
         }
      }
      //����ƫ��
      for (Weight = 0; Weight < OV7725_W; Weight++)
      {
         YMidline[Weight] = (int)(UpBorder[Weight] + DownBorder[Weight]) / 2; //�ɱ߽�ó�����λ��
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

//��ȡ�Ƕ�ƫ��(����ֵΪ�Ƕȵ�����ֵ)����һ���ķ�Χ�ڣ�����ֵ��������Ƕ�ֵ�������������Ҫ��
float Angle_Get(char mode)
{
   int16 Weight = 0, Hight = 0, Position1 = 0, Position2 = 0; //���Weight �߶�Hight,Position �ݴ�߽�ֵ
   char flag = 0;                                     //˫�߱�־λ
   int16 XError[OV7725_H], YError[OV7725_W];           //x,y�����
   float SumTheta = 0;                               //����б��ʱ�ĸ�б���ۼӺ�
   float Result;                                   //ֱ��б��  ����ֵ

   int16 LeftBorder[OV7725_H];
   int16 RightBorder[OV7725_H]; //����ɨ�����ұ߽�
   int16 UpBorder[OV7725_W];
   int16 DownBorder[OV7725_W];                   //����ɨ�����±߽�
   int16 XMidline[OV7725_H], YMidline[OV7725_W]; //x,y��������ȡ

   //X��Ƕ���ȡ
   if (mode == 'X')
   {
      //��ȡ���ұ߽�
      for (Hight = OV7725_H - 1; Hight >= 0; Hight--)
      {
         for (Weight = 0; Weight < OV7725_W - 1; Weight++)
         {
            if (image_dec[Hight][Weight] == 255)
            {
               if ((image_dec[Hight][Weight + 1] == 0))
               {
                  flag = 1;
                  Position1 = Weight; //������ ��߽�洢
               }
            }
            if ((image_dec[Hight][Weight] == 0) && (flag == 1))
            {
               if (image_dec[Hight][Weight + 1] == 255)
               {
                  flag = 2;
                  Position2 = Weight + 1; //������ �ұ߽�洢
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
      //����б��
      for (Hight = OV7725_H - 1; Hight >= 0; Hight--)
      {
         XMidline[Hight] = (int)(LeftBorder[Hight] + RightBorder[Hight]) / 2; //�ɱ߽�ó�����λ��
         XError[Hight] = XMidline[Hight] - XMidline[OV7725_H - 1];
      }
      for (Hight = 10; Hight < 30; Hight++) //�޷��˲�
      {
         if ((XError[Hight] < 30) && (XError[Hight] > -30))
            SumTheta += XError[Hight] / (Hight * 1.0);
      }
      Result = SumTheta * 10;
   }

   //Y��Ƕ���ȡ
   if (mode == 'Y')
   {
      //��ȡ���±߽�
      for (Weight = 0; Weight < OV7725_W; Weight++)
      {
         for (Hight = 0; Hight < OV7725_H - 1; Hight++)
         {
            if (image_dec[Hight][Weight] == 255)
            {
               if (image_dec[Hight + 1][Weight] == 0)
               {
                  flag = 1;
                  Position1 = Hight; //������ �ϱ߽�洢
               }
            }
            if ((image_dec[Hight][Weight] == 0) && (flag == 1))
            {
               if (image_dec[Hight + 1][Weight] == 255)
               {
                  flag = 2;
                  Position2 = Hight + 1; //������ �±߽�洢
               }

               if (((Position2 - Position1) < 20) && ((Position2 - Position1) > 5))
               {
                  UpBorder[Weight] = Position1;
                  DownBorder[Weight] = Position2;
               }
            }
         }
      }
      //б�ʼ���
      for (Weight = 0; Weight < OV7725_W; Weight++)
      {
         YMidline[Weight] = (int)(UpBorder[Weight] + DownBorder[Weight]) / 2; //�ɱ߽�ó�����λ��
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

//����ͷ����
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