#ifndef _GraySensor_h
#define _GraySensor_h

extern void GraySensor_Init(void);
extern uint8 Gray_Sensor1_Read(void);
extern uint8 Gray_Sensor2_Read(void);
extern uint8 Gray_Sensor3_Read(void);
extern uint8 Gray_Sensor4_Read(void);
extern int8 Sensor_Trigger(void);
extern void GraySensor_Tset(void);

#endif