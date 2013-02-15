#ifndef __MAIN_H
#define __MAIN_H

#include "gpio_conf.h"
#include <string.h>

//enum DataOrOrdnung {
//	DATA,ORDNUNG
//};

int disp_sw = 0;	// zmienna przechowujê aktualn¹ konfiguracjê ekranu g³ównego
					// czyli wybór parametrów które maj¹ byæ na nim wyœwietlane

struct FlightData {
	// definicja struktury u¿ywanej do zapisu parametrów lotu do pamiêci flash
	float MaxSink;
	float MaxClimbing;
	float MaxAltiGain;
	float BestThermalHeigh;
	int FlightTime;
	float StartAltitude;
	float LandingAltitude;
};

///////////////// Zewnêtrzne zmienne procesowe, dostarczane z pliku vario_alti.o //////
//extern float altitude, altitude2, temp, baro, vario,i_alti_1, i_alti_2, max_altitude_gain, start_altitude;
//extern float max_climbing, max_descending;
//extern int fl_time, qnh, fl_clock_en;
///////////////////////////////////////////////////////////////



///////////////// Zewnêtrzne zmienne procesowe, dostarczane z pliku drivers/serial.o //////
//extern int SrlRXing, SrlTXing;
//extern int SrlRXData[32];
///////////////////////////////////////////////////////////////

#define __HDDISP
#include "drivers/hddisp.h"
//extern int HDDispSendByte(char Byte, enum DataOrOrdnung RS);
//extern void HDDispDelay(int num);
//extern void HDConfigureGPIO(void);
//extern int HDDispSend4Bits(char Byte);
//extern int HDDispInit(void);
//extern int HDGetOrdnungFromAddr(int addr);
//extern int HDGotoXY(int row, int column);
//extern int HDPrintString(char* string, int srow, int scol);
//extern void HDClearDisp(void);

#define __USER_INTERF
#include "user_interf.h"
//extern void int2string(int input, char* output);
//extern void float2string(float input, char* output, int accur);
//extern void MainScreen(float vario, float alti, int fltime, int settings);
//extern void FlightStats(void);

#define __I2C
#include "drivers/i2c.h"
//extern void i2cConfigure(void);
//extern int i2cSendData(int addr, int* data);

#define __SERIAL
#include "drivers/serial.h"
//extern void SrlConfig(void);
//extern void SrlSendData(char* data);
//extern void SrlReceiveData(char* data, int num);

#define __MS5611
#include "drivers/ms5611.h"
//extern void SensorReset(int addr);
//extern float SensorBringTemperature(void);
//extern double SensorBringPressure(void);

#define __VARIO_ALTI
#include "vario_alti.h"
//extern float AltiFromPressure(float pressure, int qnh);
//extern float Vario (float calti);
//extern void SetQNHAtStartup(void);

#define __FLASH
#include "drivers/flash.h"
//extern int FlashUnlock(void);
//extern void FlashProgram(int addr, int data);

#define __GPS_NAVI
#include "gps_navi.h"


// Definicjê przechowuj¹ce zawartoœæ ekranu rozruchowego
char* start1s = "WARIOMETR EAP-R2";
char* start2s = " -----devel---- ";
char* start3s = "Mateusz SP8EBC";
char* start4s = "################";

extern int key;

extern char* testing1;
extern char* testing2;
extern char* testing3;
extern char* testing4;
extern char* NullLineS;

char* nmeatest = "$GPGGA,092153.123,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E";
//char* nmeatest = "$GPRMC,161229.487,A,3723.2475,N,12158.3416,W,0.13,309.62,120598, ,*10";

//extern volatile float vario,alti;
//extern float i_alti_1, i_alti_2; 
//extern int qnh;

#endif
