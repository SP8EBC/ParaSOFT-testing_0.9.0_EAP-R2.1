#ifndef __VARIO_ALTI_H
#define __VARIO_ALTI_H

#ifndef __VARIO_ALTI

#include "string.h"

volatile float vario;		// g³ówna i najwa¿niejsza zmienna w ca³ym programie która przechowujê 
							// aktualn¹ wartoœæ wznoszenia b¹dŸ opadania

int ai;
float i_alti_1, i_alti_2;	// dwie zmienne pomocnicze do obliczania wskazañ wariometru

//float alti_filter[10][2] = {
//	{0, 0},
//	{0, 0},
//	{0, 0},
//	{0, 0},
//	{0, 0},
//	{0, 0},
//	{0, 0},
//	{0, 0},
//	{0, 0},
//	{0, 0}
//};	// tablica pomocnicza dla filtra szumów w wariometrze

int qnh = 1013;				// zmienna przwchowuj¹ca qnh. Domyœlnie jest tam wpisywane qfe

int fl_time = 0;				// zmienna przechowuj¹ca czas lotu

int fl_clock_en = 0;		// zmienna uruchamiaj¹ca odliczanie czasu

float altitude, temp, baro;
float altitude2;			// zmienna przechowujê poprawkê dla drugiego wysokoœciomierza.
							// poprawka ta jest liczb¹ odejmowan¹ od wysokoœci bezwzglêdnej
							// w celu uzyskania wzglêdnej wysokoœci od punktu zresetowania

float alti_th_start;		// zmienna przechowuj¹ca wysokoœæ na której rozpoczê³o siê kr¹¿enie w termice

float alti_th_stop;			// zmienna przechowuj¹ca wysokoœæ na której wylecono z komina

float max_altitude_gain = 0.0;	// zmienna przechowujê najwiêksze przewy¿szenie rozumiane jako maksymalna ró¿nica
							// wysokoœci chwilowej od wysokoœci z pocz¹tku lotu

float best_thermal_height = 0.0;		// najwiêkszy zysk wysokoœci w kominie/ najwy¿ej podrêcony komin 

float start_altitude = 0.0;		// wysokoœæ npm na pocz¹tku lotu

float land_altitude = 0.0;		// wysokoœæ npm po wyl¹dowaniu

float max_climbing = 0.0;	// zmienna przechowujê wartoœæ maksymalnego wznoszenia podczas ca³ego lotu

float max_descending = 0.0;	// j/w ale przechowuj¹ maksymalne opadanie

int cycle = 0;				// zmienna pomocniczna do pulsowania tonu wariometru.

float iclimbing = 0;		// zmienna s³u¿¹ca do ca³kowania wskazañ wariometru wykrywaj¹cego moment wejœcia
							// i wyjœcia z obszaru wznoszenia (domyœlnie z komina). Ma to s³u¿yæ jako pomoc
							// przy obliczaniu wysokoœci uzyskanej w termice. 
							
float thermalling_thr = 0.5;	// próg sca³kowanego wznoszenia oznaczaj¹cy wejœcie i wyjœcie z komina

int thermalling = 0;		// flaga sygnalizuj¹ca kr¹¿enie w termice

extern int tone_i;

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



///////// "czêstotliwoœci" tonów wariometru. Tak naprawdê to nie s¹ tony tylko wartoœæ ///////////////////////
///////// zmiennej ³adowanej do odpowiedniego rejestru wariometru  ///////////////////////////////////////////
volatile int vario_tones[] = {0xA7, 0x9F, 0x92, 0x8A, 0x80, 0x77, 0x6E, 0x62,			 
							  0x5A, 0x53, 0x4D, 0x45, 0x40, 0x3A, 0x32, 0x2F,			 /////// opadanie
							  0x27, 0x25, 0x23, 0x22, 0x21, 0x20, 0x1F, 0x1E,			 //////// wznoszenie
							  0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////// Czêstotliwoœæ pulsowania tonu wariometru. Tylko na wznoszenie ///////////////////////////////////////
volatile int pulse_cycle[] = {1, 1, 1, 1, 1, 1, 1, 1,
							  1, 1, 1, 1, 1, 1, 1, 1,
							  15, 14, 13, 12, 11, 10, 9, 8,
							  7, 6, 5, 4, 3, 2, 1, 1};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <stm32f10x.h>

char* str1 = "Podaj QNH:";

float AltiFromPressure(float pressure, int qnh);
float Vario (float calti);
void SetQNHAtStartup(void);
void TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (int tone);		// tego chyba nie trzeba wyjaœniaæ :) :)

#define __HDDISP
#include "drivers/hddisp.h"
//extern int HDPrintString(char* string, int srow, int scol);
//extern void HDClearDisp(void);

#define __USER_INTERF
#include "user_interf.h"
//extern void int2string(int input, char* output);
//extern void float2string(float input, char* output, int accur);
//extern void FixString(char* input, int len);

#define __SERIAL
#include "drivers/serial.h"
//extern void SrlSendData(char* data);
//extern int SrlTXing;

#define __FLASH
#include "drivers/flash.h"

#endif

#ifdef __VARIO_ALTI

extern float AltiFromPressure(float pressure, int qnh);
extern float Vario (float calti);
extern void SetQNHAtStartup(void);
extern void TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (int tone);
extern void StoreFlightStatsToFlash(struct FlightData flight);
struct FlightData ReadFlightStatsFromFlash(int index);

extern float altitude, altitude2, temp, baro, vario,i_alti_1, i_alti_2, max_altitude_gain, start_altitude, land_altitude;
extern float max_climbing, max_descending, iclimbing, thermalling_thr, alti_th_start, alti_th_stop, best_thermal_height;
extern int fl_time, qnh, fl_clock_en, thermalling;

//struct FlightData {
//	// definicja struktury u¿ywanej do zapisu parametrów lotu do pamiêci flash
//	float MaxSink;
//	float MaxClimbing;
//	float MaxAltiGain;
//	float BestThermalHeigh;
//	int FlightTime;
//	float StartAltitude;
//	float LandingAltitude;
//};

#endif

#endif
