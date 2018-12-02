#ifndef _Encoder_h
#define _Encoder_h

#define LF_Dir gpio_get(B2)
#define RF_Dir gpio_get(A15)
#define LB_Dir gpio_get(A16)
#define RB_Dir gpio_get(A17)

extern void Encoder_Init(void);
extern int16 LF_Encoder_Get(void);
extern int16 RF_Encoder_Get(void);
extern int16 LB_Encoder_Get(void);
extern int16 RB_Encoder_Get(void);
extern void Encoder_Test(void);

#endif
