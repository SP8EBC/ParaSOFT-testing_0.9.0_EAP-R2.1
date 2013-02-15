#ifndef __GPS_NAVI_H
#define __GPS_NAVI_H

#ifndef __GPS_NAVI

#include <stdlib.h>
#include <string.h>

struct GPSData {
	char FrameType;		// okre�la typ ramki
	// W pola ten struktury s� wpisywane przetworzone dane z ramki NMEA
	// W zale�no�ci od typu ramki b�d� tam trafia�y r�ne dane
	float First;		// dla ramki GGA dlugosc geograficzna
						// dla ramki RMC pr�dko�� wzgl�dem ziemii
	float Second;		// dla ramki GGA szerokosc geograficzna
						// dla ramki RMC kurs wzgl�dem ziemii
	float Third;		// dla ramki GGA wysokosc MSL
	char Fourth;		// dla ramki GGA okre�la dla dlugosci E gdy zero albo W gdy jeden
	char Fifth;			// dla ramki GGA okre�la p�kule dla szeroko�ci N gdy zero albo S gdy jeden
	int Sixth;			// dla ramgi GGA godziny czasu UTC
						// dla ramki RMC bierz�cy rok
	int Seventh;		// dla ramki GGA minuty czasu UTC
						// dla ramki RMC bierz�cy miesi�c
	int Eighth;			// dla ramki GGA sekundy czasu UTC
						// dla ramki RMC bierz�cy dzie�
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
