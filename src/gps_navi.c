#include "gps_navi.h"

struct GPSData ParseNMEA(char* input) {
	struct GPSData output;			// struktura z przetworzonymi danymi. W zale¿noœci od typu ramki
									// bêd¹ siê tutaj znajdowaæ ró¿ne dane, dlatego funkcja wywo³uj¹ca
									// musi sama wiedzieæ co gdzie jest
									//////////////////////////////////
	char fields[20][20];			// tablica zawieraj¹ca rozdzielone pola ramki NMEA
	char temp[50];					// tablica pomocnicza do konwersji z typu char na float i int
	int ic =0, ij = 0;
	int field_number = 0;
	if (*input != '\0') {			// je¿eli przekazano nie pusty ci¹g znaków
		//////////////////////////////////////////////////////////////////////////////
		///// PRZEPISYWANIE I ROZDZIELANIE PÓL Z RAMKI NMEA DO OSOBNEJ TABLICY
		///////////////////////////////////////////////////////////////////////////// 	
		for (ic = 0;(*(input + ic) != 0x0D && ic <= 120) ;ic++) {
			if (*(input + ic + 1) != ',') {
				fields[field_number][ij] = *(input + ic);
				ij++;
			}
			else {
				fields[field_number][ij] = *(input + ic);
				fields[field_number][ij+1] = 0x00;
				field_number++;
				ic++;
				ij = 0;
			}
		}
		////////////////////////////////////////////////////////////////////////////////
		if (strcmp(fields[0], "$GPGGA") == 0 && (fields[6][0] == 0x31 || fields[6][0] == 0x32)) {
			output.FrameType = 0x01;			// ramka typu GGA
			//////////////////////////////////////////// DLUGOSC GEOGRAFICZNA
			strcpy(temp,fields[2]);
			FixString(temp,9);
			output.First = atof(temp);	
			if (fields[3][0] == 'N')
				output.Fifth = 0x00;		// pó³kula pó³nocna
			else if (fields[3][0] == 'S')
				output.Fifth = 0x01;		// pó³kula po³udniowa
			else
				output.Fifth = 0xFF;		// b³¹d	
			////////////////////////////////////////////// SZEROKOSC GEOGRAFICZNA
			strcpy(temp,fields[4]);
			FixString(temp,10);
			output.Second = atof(temp);
			if (fields[5][0] == 'E')
				output.Fourth = 0x00;
			else if (fields[5][0] == 'W')
				output.Fourth = 0x01;
			else
				output.Fifth = 0xFF;
			///////////////////////////////////////////////// WYSOKOŒÆ GPS
			strcpy(temp,fields[9]);
			FixString(temp,5);
			output.Third = atoi(temp);
			//////////////////////////////////////////////  CZAS UTC
			strcpy(temp,fields[1]);
			FixString(temp,10);
			output.Sixth = (int)(atof(temp) / 10000 );	// godzina
			output.Seventh = (int)((atoi(temp) / 100) % (output.Sixth * 100) );	// minuta
			output.Eighth = (int)(atoi(temp) % (output.Sixth * 10000 + output.Seventh * 100 )); // sekunda
			return output;
		}
		////////////////////////////////////////////////////////////////////////////////////
		else if (strcmp(fields[0], "$GPRMC") == 0 && fields[2][0] == 'A') {
			output.FrameType = 0x02;				// ramka typu RMC
			////////////////////////////////////////// PRÊDKOŒÆ WZGLÊDEM ZIEMII
			strcpy(temp,fields[7]);
			FixString(temp,5);
			output.First = atof(temp);	
			output.First *= 1.852;		// prêdkoœæ przeliczona z wêz³ów na km.h
			////////////////////////////////////////// KURS WZGLÊDEM ZIEMII
			strcpy(temp,fields[8]);
			FixString(temp,5);
			output.Second = atof(temp);
			///////////////////////////////////////////// AKTUALNE DATA
			strcpy(temp,fields[9]);
			FixString(temp,6);
			output.Eighth = (int)(atoi(temp) / 10000);		// dzieñ
			output.Seventh = (int)((atoi(temp) / 100) % (output.Eighth * 100) );		// miesi¹c
			output.Sixth = (int)(atoi(temp) % (output.Seventh * 100 + output.Eighth * 10000 ));		// rok
		}
		else
			output.First = -1;						// fix niepoprawny b¹dŸ uszkodzony
	}
	return output;
}
