#ifndef GraySensor_h
#define GraySensor_h

extern void GraySensor_Init(void);
extern unsigned char Gray_Sensor1_Read(void);
extern unsigned char Gray_Sensor2_Read(void);
extern unsigned char Gray_Sensor3_Read(void);
extern unsigned char Gray_Sensor4_Read(void);
extern char Sensor_Trigger(void);
extern void GraySensor_Tset(void);

#endif