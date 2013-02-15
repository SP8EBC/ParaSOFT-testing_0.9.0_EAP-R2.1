#ifndef __MS5611_H
#define __MS5611_H

#ifndef __MS5611

#include <math.h>

void SensorReset(int addr);
int SensorReadCalData(int addr, int* cal_data);
//unsigned char crc4(unsigned int n_prom[]);
unsigned char crc4(int n_prom[]);
long int SensorStartMeas(int param_to_meas);
float SensorBringTemperature(void);
double SensorBringPressure(void);


int SensorCalData[8];
double SensorDT;

#define __I2C
#include "drivers/i2c.h"
//extern int i2cSendData(int addr, int* data, int null);
//extern int i2cReceiveData(int addr, int* data, int num);
//extern int i2cTXing;
//extern int i2cRXing;
//extern int i2cRXData[16];

#define __USER_INTERF
#include "user_interf.h"
//extern void TableCpy(int* copy_from, int* copy_to);
//extern void Delay1ms(void);
//extern void Delay10ms(void);

#endif

#ifdef __MS5611

extern void SensorReset(int addr);
extern int SensorReadCalData(int addr, int* cal_data);
//unsigned char crc4(unsigned int n_prom[]);
extern unsigned char crc4(int n_prom[]);
extern long int SensorStartMeas(int param_to_meas);
extern float SensorBringTemperature(void);
extern double SensorBringPressure(void);

#endif

#endif
