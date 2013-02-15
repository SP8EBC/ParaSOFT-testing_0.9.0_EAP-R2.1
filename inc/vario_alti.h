#ifndef __VARIO_ALTI_H
#define __VARIO_ALTI_H

#ifndef __VARIO_ALTI

#include "string.h"

volatile float vario;		// g��wna i najwa�niejsza zmienna w ca�ym programie kt�ra przechowuj� 
							// aktualn� warto�� wznoszenia b�d� opadania

int ai;
float i_alti_1, i_alti_2;	// dwie zmienne pomocnicze do obliczania wskaza� wariometru

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
//};	// tablica pomocnicza dla filtra szum�w w wariometrze

int qnh = 1013;				// zmienna przwchowuj�ca qnh. Domy�lnie jest tam wpisywane qfe

int fl_time = 0;				// zmienna przechowuj�ca czas lotu

int fl_clock_en = 0;		// zmienna uruchamiaj�ca odliczanie czasu

float altitude, temp, baro;
float altitude2;			// zmienna przechowuj� poprawk� dla drugiego wysoko�ciomierza.
							// poprawka ta jest liczb� odejmowan� od wysoko�ci bezwzgl�dnej
							// w celu uzyskania wzgl�dnej wysoko�ci od punktu zresetowania

float alti_th_start;		// zmienna przechowuj�ca wysoko�� na kt�rej rozpocz�o si� kr��enie w termice

float alti_th_stop;			// zmienna przechowuj�ca wysoko�� na kt�rej wylecono z komina

float max_altitude_gain = 0.0;	// zmienna przechowuj� najwi�ksze przewy�szenie rozumiane jako maksymalna r�nica
							// wysoko�ci chwilowej od wysoko�ci z pocz�tku lotu

float best_thermal_height = 0.0;		// najwi�kszy zysk wysoko�ci w kominie/ najwy�ej podr�cony komin 

float start_altitude = 0.0;		// wysoko�� npm na pocz�tku lotu

float land_altitude = 0.0;		// wysoko�� npm po wyl�dowaniu

float max_climbing = 0.0;	// zmienna przechowuj� warto�� maksymalnego wznoszenia podczas ca�ego lotu

float max_descending = 0.0;	// j/w ale przechowuj� maksymalne opadanie

int cycle = 0;				// zmienna pomocniczna do pulsowania tonu wariometru.

float iclimbing = 0;		// zmienna s�u��ca do ca�kowania wskaza� wariometru wykrywaj�cego moment wej�cia
							// i wyj�cia z obszaru wznoszenia (domy�lnie z komina). Ma to s�u�y� jako pomoc
							// przy obliczaniu wysoko�ci uzyskanej w termice. 
							
float thermalling_thr = 0.5;	// pr�g sca�kowanego wznoszenia oznaczaj�cy wej�cie i wyj�cie z komina

int thermalling = 0;		// flaga sygnalizuj�ca kr��enie w termice

extern int tone_i;

struct FlightData {
	// definicja struktury u�ywanej do zapisu parametr�w lotu do pami�ci flash
	float MaxSink;
	float MaxClimbing;
	float MaxAltiGain;
	float BestThermalHeigh;
	int FlightTime;
	float StartAltitude;
	float LandingAltitude;
};



///////// "cz�stotliwo�ci" ton�w wariometru. Tak naprawd� to nie s� tony tylko warto�� ///////////////////////
///////// zmiennej �adowanej do odpowiedniego rejestru wariometru  ///////////////////////////////////////////
volatile int vario_tones[] = {0xA7, 0x9F, 0x92, 0x8A, 0x80, 0x77, 0x6E, 0x62,			 
							  0x5A, 0x53, 0x4D, 0x45, 0x40, 0x3A, 0x32, 0x2F,			 /////// opadanie
							  0x27, 0x25, 0x23, 0x22, 0x21, 0x20, 0x1F, 0x1E,			 //////// wznoszenie
							  0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////// Cz�stotliwo�� pulsowania tonu wariometru. Tylko na wznoszenie ///////////////////////////////////////
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
void TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (int tone);		// tego chyba nie trzeba wyja�nia� :) :)

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
//	// definicja struktury u�ywanej do zapisu parametr�w lotu do pami�ci flash
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
