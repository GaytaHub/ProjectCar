#ifndef _Encoder_h
#define _Encoder_h

extern void Encoder_Init(void);
extern int16 LF_Encoder_Get(void);
extern int16 RF_Encoder_Get(void);
extern int16 LB_Encoder_Get(void);
extern int16 RB_Encoder_Get(void);
extern void Encoder_Test(void);

#endif
