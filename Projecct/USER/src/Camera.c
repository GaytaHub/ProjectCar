#include "headfile.h"

//����ͷ��ʼ��
void Camera_Init(void)
{
   ov7725_init();
}
//x ����ɨ��(��Ӧ����) y ����ɨ��(��Ӧ����) ��ȡƫ�ֱ������Ļ�����ƫ�
//x ����ɨ��(��Ӧ����) y ����ɨ��(��Ӧ����) ��ȡб��б�ʴ�С
//���������� coordinate��'x'��'y'��Ӧ��ͬ���꣩ value��offset��theta ��Ӧƫ����ȡ��б����ȡ��
float Camera_Get(char Axis, char Mode)
{
   char W, H;                           //���W �߶�H
   char Flag = 0, Position1, Position2; //Position �ݴ�߽�ֵ
   float TransverseErrX[OV7725_H];       //x ��OV7725_W/2��ƫ��
   char TransverseErrY[OV7725_W];       //y ��OV7725_H/2��ƫ��
   float LinearErrX[OV7725_H];           //x��ֱ��ƫ��
   char LinearErrY[OV7725_W];           //y��ֱ��ƫ��
   float SumOffset = 0.0;               //ƫ���ۼӺ�
   float SumTheta = 0;                  //����б��ʱ�ĸ�б���ۼӺ�
   float offset;                        //ƫ�� ����ֵ
   float tantheta;                      //ֱ��б��  ����ֵ

   uint8 ImageLBorder[OV7725_H];
   uint8 ImageRBorder[OV7725_H]; //����ɨ�����ұ߽�
   uint8 ImageUBorder[OV7725_W];
   uint8 ImageDBorder[OV7725_W]; //����ɨ�����±߽�
   float ImageLineX[OV7725_H];   //x��������ȡ
   uint8 ImageLineY[OV7725_W];   //y��������ȡ

   //����ȷ������ϵ
   if (Axis == 'X')
   {
      //��ȡ���ұ߽�
      for (H = OV7725_H - 1; H >= 0; H--)
      {
         for (W = 20; W < OV7725_W - 20; W++)
         {
            if (image_dec[H][W] == 255)
            {
               if ((image_dec[H][W + 1] == 0))
               {
                  Flag = 1;
                  Position1 = W; //������ ��߽�洢
               }
            }
            if ((image_dec[H][W] == 0) && (Flag == 1))
            {
               if (image_dec[H][W + 1] == 255)
               {
                  Flag = 2;
                  Position2 = W + 1; //������ �ұ߽�洢
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

      //�ɱ߽�ó�����λ��
      for (H = 0; H < OV7725_H; H++)
      {
         ImageLineX[H] = (ImageLBorder[H] + ImageRBorder[H]) / 2.0;
         TransverseErrX[H] = ImageLineX[H] - OV7725_W / 2.0;
      }
      //�����˲�
      for (H = 1; H < OV7725_H; H++)
      {
         if (myabs(ImageLineX[H] - ImageLineX[H - 1]) > 3)
            ImageLineX[H] = ImageLineX[H - 1];
      }

      //Ȼ��ȷ������ȡ��
      if (Mode == 'E')
      {
         //����ƫ��
         for (H = 0; H < 60; H++)
         {
            SumOffset += TransverseErrX[H];
         }
         offset = SumOffset / 60;
         return offset;
      }

      if (Mode == 'T')
      {
         //����б��
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
      //��ȡ���±߽�
      for (W = 0; W < OV7725_W; W++)
      {
         for (H = 0; H < OV7725_H - 1; H++)
         {
            if (image_dec[H][W] == 255)
            {
               if (image_dec[H + 1][W] == 0)
               {
                  Flag = 1;
                  Position1 = H; //������ �ϱ߽�洢
               }
            }
            if ((image_dec[H][W] == 0) && (Flag == 1))
            {
               if (image_dec[H + 1][W] == 255)
               {
                  Flag = 2;
                  Position2 = H + 1; //������ �±߽�洢
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
         //����ƫ��
         for (W = 0; W < OV7725_W; W++)
         {
            ImageLineY[W] = (int)(ImageUBorder[W] + ImageDBorder[W]) / 2; //�ɱ߽�ó�����λ��
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
         //б�ʼ���
         for (W = 0; W < OV7725_W; W++)
         {
            ImageLineY[W] = (int)(ImageUBorder[W] + ImageDBorder[W]) / 2; //�ɱ߽�ó�����λ��
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

//����ͷ���Գ���
void Camera_Test(void)
{
   float XError, YError;
   float XTheta, YTheta;
   Camera_Init();
   OLED_Init();
   for (;;)
   {
      Image_Decompression(image_bin, image_dec[0]); //����ͷͼ���ѹ
      dis_bmp(60, 80, image_dec[0], 0x7F);
      XError = Camera_Get('X', 'E');
      XTheta = Camera_Get('X', 'T');
   }
}