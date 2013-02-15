#ifndef __GPS_NAVI_H
#define __GPS_NAVI_H

#ifndef __GPS_NAVI

#include <stdlib.h>
#include <string.h>

struct GPSData {
	char FrameType;		// okreœla typ ramki
	// W pola ten struktury s¹ wpisywane przetworzone dane z ramki NMEA
	// W zale¿noœci od typu ramki bêd¹ tam trafia³y ró¿ne dane
	float First;		// dla ramki GGA dlugosc geograficzna
						// dla ramki RMC prêdkoœæ wzglêdem ziemii
	float Second;		// dla ramki GGA szerokosc geograficzna
						// dla ramki RMC kurs wzglêdem ziemii
	float Third;		// dla ramki GGA wysokosc MSL
	char Fourth;		// dla ramki GGA okreœla dla dlugosci E gdy zero albo W gdy jeden
	char Fifth;			// dla ramki GGA okreœla pó³kule dla szerokoœci N gdy zero albo S gdy jeden
	int Sixth;			// dla ramgi GGA godziny czasu UTC
						// dla ramki RMC bierz¹cy rok
	int Seventh;		// dla ramki GGA minuty czasu UTC
						// dla ramki RMC bierz¹cy miesi¹c
	int Eighth;			// dla ramki GGA sekundy czasu UTC
						// dla ramki RMC bierz¹cy dzieñ
};

#define __USER_INTERF
#include "user_interf.h"

#endif



#ifdef __GPS_NAVI

struct GPSData {
	char FrameType;
	float First;
	float Second;
	float Third;
	char Fourth;
	char Fifth;	
	int Sixth;	
	int Seventh;
	int Eighth;	
};

extern struct GPSData ParseNMEA(char* input);

#endif

#endif
