#include "headfile.h"

/**********����ͷ��ʼ��**************/
void Camera_Init(void)
{
   ov7725_init();
}

/*************��������ɨ��õ�����ֵ**************/
float LeftToRightMidline[OV7725_H] = {0}; //������ȡֵ
void LeftToRight_Scan(void)
{
   int LeftBorder[OV7725_H];  //����ɨ����߽�
   int RightBorder[OV7725_H]; //����ɨ���ұ߽�
   int Width = 0, Height = 0; //ѭ������
   int First = 0;             //˫����߽�ɨ���־λ
   int Border1, Border2;      //�߽��ݴ�ֵ

   //��ȡ���ұ߽�
   //��������ɨ��
   for (Height = 0; Height < OV7725_H; Height++)
   {
      //ÿ�δ�������ɨ��ǰ���ð��߼���־
      //ɨ��ǰ���ñ߽��ݴ�ֵ
      First = False;
      Border1 = 0;
      Border2 = 0;
#define LeftToRightScanRange 8 //ͼ��ʴ��Χ
      //��������ɨ��
      for (Width = LeftToRightScanRange; Width < OV7725_W - 1 - LeftToRightScanRange; Width++)
      {
         //������
         if (image_dec[Height][Width] == 255 && First == False)
         {
            if ((image_dec[Height][Width + 1] == 0))
            {
               First = True;    //��һ��ɨ�������ڱ߽�
               Border1 = Width; //�߽�洢
            }
         }
         //������
         if (image_dec[Height][Width] == 0) //��������ɨ���ĺ����ױ߽�
         {
            if (image_dec[Height][Width + 1] == 255)
            {
               Border2 = Width; //�߽�洢
            }
         }
      }
      LeftBorder[Height] = Border1;
      RightBorder[Height] = Border2;
   }
   //�ɱ߽�ó�����λ��,�������¼���
   for (Height = 0; Height < OV7725_H; Height++)
   {
      LeftToRightMidline[Height] = (LeftBorder[Height] + RightBorder[Height]) / 2.0;
   }
   //�����˲�
   for (Height = 1; Height < OV7725_H; Height++)
   {
      if (myabs(LeftToRightMidline[Height] - LeftToRightMidline[Height - 1]) > 2.0)
         LeftToRightMidline[Height] = LeftToRightMidline[Height - 1] * 0.8 + LeftToRightMidline[Height] * 0.2;
   }
}

/**************************��������ɨ��õ�����ֵ***************************/
float UpToDownMidline[OV7725_W] = {0}; //������ȡֵ
void UpToDown_Scan(void)
{
   int UpBorder[OV7725_W];    //����ɨ����߽�
   int DownBorder[OV7725_W];  //����ɨ���ұ߽�
   int Width = 0, Height = 0; //ѭ������
   int Border1, Border2;      //�߽��ݴ�ֵ

   //��ȡ���±߽�
   //��������ɨ��
   for (Width = 0; Width < OV7725_W; Width++)
   {
      //ɨ��ǰ���ñ߽��ݴ�ֵ
      Border1 = 0;
      Border2 = 0;
#define UpToDownRange 12 //ͼ��ʴ��Χ
      //��������ɨ��
      for (Height = UpToDownRange; Height < OV7725_H - 1 - UpToDownRange; Height++)
      {
         //������
         if (image_dec[Height][Width] == 255)
         {
            if ((image_dec[Height + 1][Width] == 0))
            {
               Border1 = Height; //�߽�洢
            }
         }
         //������
         if (image_dec[Height][Width] == 0) //��������ɨ���ĺ����ױ߽�
         {
            if (image_dec[Height + 1][Width] == 255)
            {
               Border2 = Height; //�߽�洢
            }
         }
      }
      UpBorder[Width] = Border1;
      DownBorder[Width] = Border2;
   }
   //�ɱ߽�ó�����λ��,�������Ҽ���
   for (Width = 0; Width < OV7725_W; Width++)
   {
      UpToDownMidline[Width] = (UpBorder[Width] + DownBorder[Width]) / 2.0;
   }
   //���ߵ�ͨ�˲�
   for (Width = 1; Width < OV7725_W; Width++)
   {
      if (myabs(UpToDownMidline[Width] - UpToDownMidline[Width - 1]) > 1.5)
         UpToDownMidline[Width] = UpToDownMidline[Width - 1] * 0.7 + UpToDownMidline[Width] * 0.3;
   }
}

/*********************�����������˲��㷨***************************/
float FilteringMidlineLength, FilteringMidlineHead, FilteringMidlineTail;
int Height, i = 1; //ѭ������
float LineHead[60] = {0}, LineTail[60] = {0}, Length[60] = {0};
float Head = 0, Tail = 0;
void Midline_filter(void)
{

   for (Height = 0; Height < OV7725_H - 1; Height++)
   {
      if (myabs(LeftToRightMidline[Height] - LeftToRightMidline[Height - 1]) > 2)
      {
         Tail = Height;           //��ȡβ
         LineHead[i] = Head;      //��¼ͷ
         LineTail[i] = Tail;      //��¼β
         Length[i] = Tail - Head; //��¼����
         Head = Height;           //����ͷ
         i++;
         if (i == 60)
            i = 0;
      }
   }
   for (i = 1; i < 6; i++)
   {
      //��������μ�¼
      if (Length[i] > Length[i - 1])
      {
         FilteringMidlineLength = Length[i];
         FilteringMidlineHead = LineHead[i];
         FilteringMidlineTail = LineTail[i];
      }
   }
}

/**********************�����ȡ***************************/
float Error_Get(char Axis, char Mode)
{
   int Width = 0, Height = 0;                                             //ѭ������
   float LeftToRightError[OV7725_H] = {0}, UpToDownError[OV7725_W] = {0}; //��������ɨ��ʱ�����,��������ɨ��ʱ�����
   float SumDownReferencePoint = 0, SumUpReferencePoint = 0;              //��������ɨ��ʱ�ο���λ���ܺ�
   float SumLeftReferencePoint = 0, SumRightReferencePoint = 0;           //��������ɨ��ʱ�ο���λ���ܺ�
   float DownReferencePoint = 0, UpReferencePoint = 0;                    //��������ɨ��ʱ�ο���λ��
   float LeftReferencePoint = 0, RightReferencePoint = 0;                 //��������ɨ��ʱ�ο���λ��
   float SumError = 0.0;                                                  //�����
   float Result;                                                          //���

   //��X���ƶ�ʱ�����ȡ
   if (Axis == 'X')
   {
      //��ȡ�����
      if (Mode == 'E')
      {
         //����ƫ��
         for (Height = 0; Height < OV7725_H; Height++)
         {
            LeftToRightError[Height] = LeftToRightMidline[Height] - OV7725_W / 2.0; //��������Ұ���ߵ���Ϊ���㼫��Ҫ���ú��߼�ȥ����
            SumError += LeftToRightError[Height];
         }
         Result = SumError / OV7725_H;

         return Result;
      }
      //��ȡƫ�ƽ�
      else if (Mode == 'T')
      {
#define ReferrncePointNum 6
         //�����²ο���λ��(���·�N����Ϊ�ο�)
         for (Height = 0; Height < ReferrncePointNum; Height++)
         {
            SumUpReferencePoint += LeftToRightMidline[Height];
         }
         //�����ϲο���λ��
         for (Height = OV7725_H - 1; Height >= OV7725_H - ReferrncePointNum; Height--)
         {
            SumDownReferencePoint += LeftToRightMidline[Height];
         }
         UpReferencePoint = SumUpReferencePoint / ReferrncePointNum;                              //��ȡ�ϲο���ƽ��λ��
         DownReferencePoint = SumDownReferencePoint / ReferrncePointNum;                          //��ȡ�²ο���ƽ��λ��
         Result = (DownReferencePoint - UpReferencePoint) / (OV7725_H - ReferrncePointNum) * 1.0; //Ϊ��������ͷ�Ƕȼ����趨�����¼���

         return Result * 10;
      }
   }
   //��Y���ƶ�ʱ�����ȡ
   else if (Axis == 'Y')
   {
      //��ȡ�����
      if (Mode == 'E')
      {
         //����ƫ��
         for (Width = 0; Width < OV7725_W; Width++)
         {
            UpToDownError[Width] = OV7725_H / 2.0 - UpToDownMidline[Width]; //�����������Ұ���ߵ���Ϊ���㼫��Ҫ�������߼�ȥ����
            SumError += UpToDownError[Width];
         }
         Result = SumError / OV7725_W;

         return Result;
      }
      //��ȡƫ�ƽ�
      else if (Mode == 'T')
      {
         //������ο���λ��(�����N����Ϊ�ο�)
         for (Width = 0; Width < ReferrncePointNum; Width++)
         {
            SumLeftReferencePoint += UpToDownMidline[Width];
         }
         //�����Ҳο���λ��
         for (Width = OV7725_W - 1; Width >= OV7725_W - ReferrncePointNum; Width--)
         {
            SumRightReferencePoint += UpToDownMidline[Width];
         }
         LeftReferencePoint = SumLeftReferencePoint / ReferrncePointNum;                             //��ȡ�ϲο���ƽ��λ��
         RightReferencePoint = SumRightReferencePoint / ReferrncePointNum;                           //��ȡ�²ο���ƽ��λ��
         Result = (LeftReferencePoint - RightReferencePoint) / (OV7725_W - ReferrncePointNum) * 1.0; //Ϊ���㼫��Ҫ���������

         return Result * 10;
      }
   }

   return 0;    //û��ѡ��ʱ����0
}

/********************************����ͷ���Գ���***************************/
void Camera_Test(void)
{
   static float XError, YError;
   static float XTheta, YTheta;
   Camera_Init();
   OLED_Init();
   for (;;)
   {
      Image_Decompression(image_bin, image_dec[0]); //����ͷͼ���ѹ
      dis_bmp(60, 80, image_dec[0], 0x7F);
      LeftToRight_Scan();           //ɨ��
      UpToDown_Scan();
      Midline_filter();             //�����˲�
      XError = Error_Get('X', 'E'); //�������ȡ
      YError = Error_Get('Y', 'E');
      XTheta = Error_Get('X', 'T'); //ƫ�ƽǶ�ȡ
      YTheta = Error_Get('Y', 'T');
   }
}