#include "user_interf.h"

// funkcja zamienia cyfrê na odpowiadaj¹cy jej kod ASCII
int GetCodeFromDigit(int input) {
	switch(input) {
	case 1: return ascii[1];
	case 2: return ascii[2];
	case 3: return ascii[3];
	case 4: return 4[ascii];
	case 5: return 5[ascii];
	case 6: return ascii[6];
	case 7: return ascii[7];
	case 8: return ascii[8];
	case 9: return ascii[9];
	case 0: return ascii[0];
	case 0x2E: return 0x2E;
	default: return 0x20;
	}
}

// funkcja zamienia liczbe z zakresu -9999 do 9999 na string
void int2string(int input, char* output) {
	volatile int digits[] = {0, 0, 0, 0} ;
	volatile int bz = 0, i = 0, j = 0;
	if (input < 0) {
		bz = 1;
		}
	if (abs(input) >= 1000 && abs(input) < 10000) {
		digits[0] = abs(input) / 1000;			// tysiace
		digits[1] = (abs(input) % 1000) / 100;	// setki
		digits[2] = (abs(input) % 100) / 10;		// dziesiatki
		digits[3] = (abs(input) % 10) / 1;		// jednosci
		if (bz == 1) {
			*output = 0x2D;					// minus
			i++;
		}
		for (j; j<4; j++) {
			*(output+i) = GetCodeFromDigit(digits[j]);
			i++;
		}
		*(output+i) = '\0';	
	}
	if (abs(input) >= 100 && abs(input) < 1000) {
		digits[0] = (abs(input) % 1000) / 100;	// setki
		digits[1] = (abs(input) % 100) / 10;		// dziesiatki
		digits[2] = (abs(input) % 10) / 1;		// jednosci
		if (bz == 1) {
			*output = 0x2D;					// minus
			i++;
		}
		for (j; j<3; j++) {
			*(output+i) = GetCodeFromDigit(digits[j]);
			i++;
		}
		*(output+i) = '\0';	
	}
	if (abs(input) >= 10 && abs(input) < 100) {
		digits[0] = (abs(input) % 100) / 10;		// dziesiatki
		digits[1] = (abs(input) % 10) / 1;		// jednosci
		if (bz == 1) {
			*output = 0x2D;					// minus
			i++;
		}
		for (j; j<2; j++) {
			*(output+i) = GetCodeFromDigit(digits[j]);
			i++;
		}
		*(output+i) = '\0';	
	}
	if (abs(input) >= 0 && abs(input) < 10) {
		if (bz == 1) {
			*output = 0x2D;					// minus
			*(output + 1) = GetCodeFromDigit(abs(input));
			*(output + 2) = '\0';
		}
		else {
			*(output) = GetCodeFromDigit(abs(input));
			*(output + 1) = '\0';
		}
	}
	
		
}

// funckja zamienia liczbê zmienno przecinkow¹ z zakresu -9999,99 do 9999,99
// na tablice charow.. Zmienna accur steruje iloœcia liczb po przecinku
void float2string(float input, char* output, int accur) {
	volatile int digits[] = {0, 0, 0, 0, 0, 0, 0} ;
	volatile int bz = 0, i = 0, j = 0, k = 0;
	if (input < 0) {
		bz = 1;
		}
	if (abs(input) >= 1000 && abs(input) < 10000) {
		input *= 100;	// ¿eby przesun¹æ przecinek o dwa miejsca w prawo
		input /= 1;
		digits[0] = abs(input) / 100000;			// tysiace
		digits[1] = (abs(input) % 100000) / 10000;	// setki
		digits[2] = (abs(input) % 10000) / 1000;		// dziesiatki
		digits[3] = (abs(input) % 1000) / 100;		// jednosci
		digits[4] = 0x2E;		// kropka
		digits[5] = (abs(input) % 100) / 10;		// 10^-1
		if (accur == 2)
			digits[6] = (abs(input) % 10) / 1;			// 10^-2
		else
			k = 1;
		if (bz == 1) {
			*output = 0x2D;					// minus
			i++;
		}
		for (j; j<=6-k; j++) {
			*(output+i) = GetCodeFromDigit(digits[j]);
			i++;
		}
		*(output+i) = '\0';	
	}
	if (abs(input) >= 100 && abs(input) < 1000) {
		input *= 100;	// ¿eby przesun¹æ przecinek o dwa miejsca w prawo
		input /= 1;
		digits[0] = (abs(input) % 100000) / 10000;	// setki
		digits[1] = (abs(input) % 10000) / 1000;		// dziesiatki
		digits[2] = (abs(input) % 1000) / 100;		// jednosci
		digits[3] = 0x2E;		// kropka
		digits[4] = (abs(input) % 100) / 10;		// 10^-1
		if (accur == 2)
			digits[5] = (abs(input) % 10) / 1;			// 10^-2
		else
			k = 1;
		if (bz == 1) {
			*output = 0x2D;					// minus
			i++;
		}
		for (j; j<=5-k; j++) {
			*(output+i) = GetCodeFromDigit(digits[j]);
			i++;
		}
		*(output+i) = '\0';	
	}
	if (abs(input) >= 10 && abs(input) < 100) {
		input *= 100;	// ¿eby przesun¹æ przecinek o dwa miejsca w prawo
		input /= 1;
		digits[0] = (abs(input) % 10000) / 1000;		// dziesiatki
		digits[1] = (abs(input) % 1000) / 100;		// jednosci
		digits[2] = 0x2E;		// kropka
		digits[3] = (abs(input) % 100) / 10;		// 10^-1
		if (accur == 2)
			digits[4] = (abs(input) % 10) / 1;			// 10^-2
		else
			k = 1;
		if (bz == 1) {
			*output = 0x2D;					// minus
			i++;
		}
		for (j; j<=4-k; j++) {
			*(output+i) = GetCodeFromDigit(digits[j]);
			i++;
		}
		*(output+i) = '\0';	
	}
	if (abs(input) >= 0 && abs(input) < 10) {
			input *= 100;
			input /= 1;
		if (bz == 1) {
			*output = 0x2D;					// minus
			*(output + 1) = GetCodeFromDigit((abs(input) % 1000) / 100); // jednoœci
			*(output + 2) = GetCodeFromDigit(0x2E);	// kropka
			*(output + 3) = GetCodeFromDigit((abs(input) % 100) / 10);	// 10^-1
			if (accur == 2) {
				*(output + 4) = GetCodeFromDigit((abs(input) % 10) / 1);		// 10^-2]
				*(output + 5) =	'\0';
			}
			else
				*(output + 4) =	'\0';
				
		}
		else {
			*output = GetCodeFromDigit((abs(input) % 1000) / 100); // jednoœci
			*(output + 1) = GetCodeFromDigit(0x2E);	// kropka
			*(output + 2) = GetCodeFromDigit((abs(input) % 100) / 10);	// 10^-1
			if (accur == 2) {
				*(output + 3) = GetCodeFromDigit((abs(input) % 10) / 1);		// 10^-2]
				*(output + 4) =	'\0';
			}
			else
				*(output + 3) =	'\0';
		}
	}
	
		
}

void MainScreen(float vario, float alti, int fltime, int settings) {
	// zmienna settings okresla jakie dane powinny byæ wyœwietlane na ekranie g³ównym
	// jest to zalezne od ustawienia badz nie poszczególnych bitow. Rozpiska znajduje siê poni¿ej
	//
	// bit 0 [LSB] -- Gdy ustawiony wyœwietlanie drugiego wysokoœciomierza zamiast pierwszego
	// bit 1 -- Gdy ustawiony wyœwietlanie wartoœci maksymalnego przewy¿szenia zamiast wysokoœci
	// bit 2 -- Gdy ustawiony wyœwietlanie wartoœci œredniej wariometru zamiast chwilowej
	// bit 3 -- Gdy ustawiony wyœwietlanie maksymalnego wznoszenia zamiast wariometru
	// bit 4 -- Gdy ustawiony wyœwietlanie maksymalnego opadania zamiast wariometru
	// bit 5 -- Gdy ustawiony bargraf wariometru u¿ywa skali logarytmicznej od 0 do 20 ze œrodkiem w 2
	//			Gdy nie ustawiony skala jest liniowa od 0 do 8 ze œrodkiem w 4
	char Main1Line[17];
	char Main2Line[17];
	char Main3Line[17];
//	char Main4Line[16];
	char L1Buff[6];		// Bufor dla wartoœci liczbowych w pierwszej linijsce (domyœlnie czas lotu)
	char L2Buff[9];		// Bufor dla wartoœci liczbowych w drugiej linijsce (domyœlnie wysokoœæ)    )))) 6
	char L3Buff[8];		// Bufor dla wartoœci liczbowych w trzeciej linijsce (domyœlnie wariometr)
//	char* temp = ";\n\r";
	//////////////////////////////////////////////////////////////////////////////////
	/////////////// KLAWISZOLOGIA NA EKRANIE G£ÓWNYM
	//////////////////////////////////////////////////////////////////////////////////
	//  D-Pad W DÓ£: Prze³¹cza wysokoœciomierze. D³u¿sze przytrzymanie resetuje ALTI2
	//  D-Pad W GÓRÊ: Zatrzymuje i startuje odliczanie czasu lotu
	//  D-Pad W LEWO:
	//  D-Pad W PRAWO: Przechodzi do Pamiêci i podsumowania lotów
	//  D-Pad OK: Przechodzi do Menu G³ównego
	//////////////////////////////////////////////////////////////////////////////////  
	if ((settings & 0x1) == 0x1)						// druga linia wyœwietlacza
		strcpy(Main2Line, Alti2Line);
	if ((settings & (0x1 << 1)) == (0x1 << 1))
		strcpy(Main2Line, MaxAltGain);
	if (((settings & 0x1) | (settings & (0x1 << 1))) == 0x0)  
		strcpy(Main2Line, Alti1Line);
	float2string(alti,L2Buff,1);
	if (alti > -1000.0 && alti <= -100.0)
		strcpy(Main2Line+7, L2Buff);
	if (alti > -100.0 && alti <= -10.0)
		strcpy(Main2Line+8, L2Buff);
	if (alti > -10.0 && alti <= 0.0)
		strcpy(Main2Line+9, L2Buff);
	if (alti < 10.0 && alti >= 0.0)
		strcpy(Main2Line+10, L2Buff);
	if (alti >= 10.0 && alti < 100.0)
		strcpy(Main2Line+9, L2Buff);
	if (alti >= 100.0 && alti < 1000.0)
		strcpy(Main2Line+8, L2Buff);
	if (alti >= 1000.0 && alti < 10000.0)
		strcpy(Main2Line+7, L2Buff);
	FixString(Main2Line,16);							// usuwanie niepotrzebnych znaków koñca ci¹gu
	HDPrintString(Main2Line,2,1);						// wysy³anie gotowego wiersza do wyœwietlacza
//	strcpy(ddupa,L2Buff);		
	///////////////////////////////////////////////////////////////////////////////
	if ((settings & (0x1 << 2)) == (0x1 << 2))			// trzecia linijka
		strcpy(Main3Line, VarioAvgLine);
	if ((settings & (0x1 << 3)) == (0x1 << 3))
		strcpy(Main3Line, MaxClimbingLine);
	if ((settings & (0x1 << 4)) == (0x1 << 4))
		strcpy(Main3Line, MaxDescendingLine);
	if (((settings & (0x1 << 2)) | (settings & (0x1 << 3)) | (settings & (0x1 << 4))) == 0x0) {
		strcpy(Main3Line, VarioLine);
		float2string(vario,L3Buff,1);
		if (vario <= -10.0)
			strcpy(Main3Line+6, L3Buff);
		if (vario > -9.9 && vario < 0.0)
			strcpy(Main3Line+7, L3Buff);
		if (vario >= 0.0 && vario < 10.0)
			strcpy(Main3Line+8, L3Buff);
		if (vario >= 10.0)
			strcpy(Main3Line+7, L3Buff);
		FixString(Main3Line,16);
		HDPrintString(Main3Line,3,1);		
	}
	////////////////////////////////////////////////////////////////////////////////
	if (vario != 0) {						 
		if ((settings & (0x1 << 5)) == 0x0) {		// bargraf w czwartej linijce
			if (vario <= -7) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph1h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (1);
			 }
			if (vario > -7 && vario <= -6.5) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph2h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (2);
			 }
			if (vario > -6.5 && vario <= -6) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph3h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (3);
			 }
			if (vario > -6 && vario <= -5.5) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph4h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (4);
			 }
			if (vario > -5.5 && vario <= -5) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph5h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (5);
			 }
			if (vario > -5 && vario <= -4.5) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph6h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (6);
			 }
			if (vario > -4.5 && vario <= -4) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph7h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (7);
			 }
			if (vario > -4 && vario <= -3.5) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph8h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (8);
			 }
			if (vario > -3.5 && vario <= -3) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph9h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (9);
			 }
			if (vario > -3 && vario <= -2.5) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph10h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (10);
			 }
			if (vario > -2.5 && vario <= -2) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph11h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (11);
			 }
			if (vario > -2 && vario <= -1.5) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph12h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (12);
			 }
			if (vario > -1.5 && vario <= -1) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph13h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (13);
			 }
			if (vario > -1 && vario <= -0.5) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph14h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (14);
			 }
			if (vario > -0.5 && vario < 0) 
			 {
				HDPrintString(Bargraph16h,4,9);
				HDPrintString(Bargraph15h,4,1);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (15);
			 }
			if (vario > 0 && vario <= 0.5) 
			 {							  	// ZERO
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph1h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (16);
			 }
			if (vario > 0.5 && vario <= 1)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph2h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (17);
			 }
			if (vario > 1 && vario <= 1.5)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph3h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (18);
			 }
			if (vario > 1.5 && vario <= 2)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph4h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (19);
			 }
			if (vario > 2 && vario <= 2.5)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph5h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (20);
			 }
			if (vario > 2.5 && vario <= 3)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph6h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (21);
			 }
			if (vario > 3 && vario <= 3.5)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph7h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (22);
			 }
			if (vario > 3.5 && vario <= 4)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph8h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (23);
			 }
			if (vario > 4 && vario <= 4.5)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph9h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (24);
			 }
			if (vario > 4.5 && vario <= 5)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph10h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (25);
			 }	 
			if (vario > 5 && vario <= 5.5)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph11h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (26);
			 }
			if (vario > 5.5 && vario <= 6)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph12h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (27);
			 }
			if (vario > 6 && vario <= 6.5)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph13h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (28);
			 }
			if (vario > 6.5 && vario <= 7)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph14h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (29);
			 }
			if (vario > 7 && vario <= 7.5)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph15h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (30);
			 }
			if (vario > 7.5)
						 {
				HDPrintString(Bargraph16h,4,1);
				HDPrintString(Bargraph15h,4,9);
				TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (31);
			 }
						
		}
		if ((settings & (0x1 << 5)) != (0x1 << 5)) { 	
			if (vario > 0 && vario <= 0.5);
			if (vario > 0.5 && vario <= 1);
			if (vario > 1 && vario <= 1.5);
			if (vario > 1.5 && vario <= 2);
			if (vario > 2 && vario <= 2.5);
			if (vario > 2.5 && vario <= 3);
			if (vario > 3 && vario <= 3.5);
			if (vario > 3.5 && vario <= 4);
			if (vario > 4 && vario <= 4.5);
			if (vario > 4.5 && vario <= 5);
			if (vario > 5 && vario <= 5.5);
			if (vario > 5.5 && vario <= 6);
			if (vario > 6 && vario <= 6.5);
			if (vario > 6.5 && vario <= 7);
			if (vario > 7 && vario <= 7.5);
			if (vario > 7.5);
		}
	}
	else;	// je¿eli wskazanie wariometru jest równe 0 to bargraf jest pusty
	////////////////////////////////////////////////////////////////////////////////
	strcpy(Main1Line, AirTimeLine);						// pierwsza linijka , zawsze czas
	int2string((int)fltime/2,L1Buff);
	if (((int)fltime/2) < 10)
		strcpy(Main1Line+12, L1Buff);
	if (((int)fltime/2) < 100 && ((int)fltime/2) >= 10)
		strcpy(Main1Line+11, L1Buff);
	if (((int)fltime/2) < 1000 && ((int)fltime/2) >= 100)
		strcpy(Main1Line+10, L1Buff);
	if (((int)fltime/2) < 10000 && ((int)fltime/2) >= 1000)
		strcpy(Main1Line+9, L1Buff);
	FixString(Main1Line,16);
	HDPrintString(Main1Line,1,1);
	////////////////////////////
//	TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (j);
	
	
//	SrlSendData((int*)L2Buff);
//////	if (SrlTXing != 1) {
//////	strcpy(L2Buff+6,temp);
//////	FixString(L2Buff,9);
//////	SrlSendData(L2Buff);
//////	}
//   HDPrintString(Bargraph16h,4,1);
//   HDPrintString(Bargraph16h,4,8);
}

// funkcja kopiuje zawartoœæ jednej tablicy do innej
void TableCpy(int* copy_from, int* copy_to) {
	int i = 0;
	for (i;*(copy_from+i) != '\0'; i++)
		*(copy_to + i) = *(copy_from + i);  
}


/////////////////////////////////////////////////////////////////////////////
/////   KLAWISZOLOGIA W MENU G£ÓWNYM I POZOSTA£YCH MENU
/////////////////////////////////////////////////////////////////////////////
//  D-pad W LEWO: 	Powrót do poprzedniego menu
//  D-pad W PRAWO:
//  D-pad DO GÓRY:  Poprzednia pozycja menu.. Zmniejszanie wartoœci
//  D-pad W DÓ³:	Nastêpna pozycja menu.. Zwiêkszanie wartoœci
// 	D-pad OK:		Wejœcie do wybranej opcji menu
//////////////////////////////////////////////////////////////////////////////	
void FlightStats(struct FlightData lot) {
	static int menu = 1;			// numer aktualnie wyœwietlanej pozycji w menu
	int menu_change = 0;			// zmienna u¿ywana to przeciwdzia³aniu dr¿eniu przycisków
	int k = 0;
	char temp[10]; 					// pomocnicza tablica charów do wyœwietlania napisaów
	char* nnn = "[[ / ]]";
	HDClearDisp();
	menu_change = 1;				// zmienna blokuj¹ca odœwie¿anie wyœwietlacza gdy nie zmieni³a siê
									// zawartoœæ na nim wyœwietlana. Zapobiega to migotaniu
	HDPrintString(FlightSummaryTitle,1,1);
	while(((GPIOE->IDR & GPIO_IDR_IDR11) == GPIO_IDR_IDR11)) {
		////////////////////////////////////////////////////////////////////////////////////
		// nale¿y zwróciæ uwagê i¿ w tym miejscu nie powinno byæ ¿adnych wywo³añ funkcji  //
		// zmieniaj¹cych zawartosc wyœwietlacza. Z powodu niskiej prêdkoœci pracy wysw.	  //
		// bêdzie widoczne migotanie zawartosci											  //
		//////////////////////////////////////////////////////////////////////////////////// 
		if (menu == 1 && menu_change == 1) {
			// wyœwietlanie pierwszej pozycji
			HDPrintString(nnn,2,5);
			int2string(menu,temp);
			HDPrintString(temp,2,7);
			int2string(FlightStatsC,temp);
			 HDPrintString(temp,2,9);
			float2string(lot.MaxSink, temp,1);
			HDPrintString(MaxDescendingS,3,1);
			HDPrintString(VarioGraphLine,4,1);
			HDPrintString(temp,4,9);
			menu_change = 0;
		}
		else if (menu == 2 && menu_change == 1) {
			// wyœwietlanie drugiej pozycji
			HDPrintString(nnn,2,5);
			int2string(menu,temp);
			HDPrintString(temp,2,7);
			int2string(FlightStatsC,temp);
			HDPrintString(temp,2,9);
			float2string(lot.MaxClimbing, temp,1);
			HDPrintString(MaxClimbingS,3,1);
			HDPrintString(VarioGraphLine,4,1);
			HDPrintString(temp,4,9);
		}
		else if (menu == 3 && menu_change == 1) {
			// wyœwietlanie trzeciej pozycji
			HDPrintString(nnn,2,5);
			int2string(menu,temp);
			HDPrintString(temp,2,7);
			int2string(FlightStatsC,temp);
			HDPrintString(temp,2,9);
			float2string(lot.MaxAltiGain, temp,1);
			HDPrintString(MaxAltGainS,3,1);
			HDPrintString(VarioGraphLine,4,1);
			HDPrintString(temp,4,9);
		}
		else if (menu == 4 && menu_change == 1) {
			// wyœwietlanie czwartej pozycji
			HDPrintString(nnn,2,5);
			int2string(menu,temp);
			HDPrintString(temp,2,7);
			int2string(FlightStatsC,temp);
			HDPrintString(temp,2,9);
			float2string(lot.BestThermalHeigh, temp,1);
			HDPrintString(BestThermalS,3,1);
			HDPrintString(VarioGraphLine,4,1);
			HDPrintString(temp,4,9);
		}
		else if (menu == 5 && menu_change == 1) {
			// wyœwietlanie drugiej pozycji
			HDPrintString(nnn,2,5);
			int2string(menu,temp);
			HDPrintString(temp,2,7);
			int2string(FlightStatsC,temp);
			HDPrintString(temp,2,9);
			int2string(lot.FlightTime, temp);
			HDPrintString(FlightTimeS,3,1);
			HDPrintString(VarioGraphLine,4,1);
			HDPrintString(temp,4,9);
		}
		else if (menu == 6 && menu_change == 1) {
			// wyœwietlanie drugiej pozycji
			HDPrintString(nnn,2,5);
			int2string(menu,temp);
			HDPrintString(temp,2,7);
			int2string(FlightStatsC,temp);
			HDPrintString(temp,2,9);
			float2string(lot.StartAltitude, temp, 2);
			HDPrintString(StartAltiS,3,1);
			HDPrintString(VarioGraphLine,4,1);
			HDPrintString(temp,4,9);
		}
		else if (menu == 7 && menu_change == 1) {
			// wyœwietlanie drugiej pozycji
			HDPrintString(nnn,2,5);
			int2string(menu,temp);
			HDPrintString(temp,2,7);
			int2string(FlightStatsC,temp);
			HDPrintString(temp,2,9);
			float2string(lot.LandingAltitude, temp, 2);
			HDPrintString(LandAltiS,3,1);
			HDPrintString(VarioGraphLine,4,1);
			HDPrintString(temp,4,9);
		}
		else if (menu == 8 && menu_change == 1) {
			// zapisywanie menu... OK potwierdza
			HDPrintString(nnn,2,5);
			int2string(menu,temp);
			HDPrintString(temp,2,7);
			int2string(FlightStatsC,temp);
			HDPrintString(temp,2,9);
			HDPrintString(VarioGraphLine,3,1);
			HDPrintString(SaveFlightS,4,1);
		}
		if(((GPIOE->IDR & GPIO_IDR_IDR9) == 0) && menu <= FlightStatsC && k == 0) {
			// D-PAD w górê.. Przewijanie do nastêpnej pozycji
			menu++;
			k = 1;
			menu_change = 1;
			
		}
		else if(((GPIOE->IDR & GPIO_IDR_IDR10) == 0)&& menu > 0 && k == 0) {
			// D-PAD w dó³... Poprzednia pozycja menu
			menu--;
			k = 1;
			menu_change = 1;
		}
		else if(((GPIOE->IDR & GPIO_IDR_IDR12) == 0)&& menu == 8) {
			// D-PAD OK... Je¿eli wybrano pozycjê 8 to zapis do pamiêci
//			k = 1;
			StoreFlightStatsToFlash(lot);
		}
		else if(((GPIOE->IDR & GPIO_IDR_IDR10) == GPIO_IDR_IDR10) && ((GPIOE->IDR & GPIO_IDR_IDR9) == GPIO_IDR_IDR9) && k == 1) {
			k = 0;
			menu_change = 0;
		}
	}
}

void MainMenu(void) {
	static int menu = 1;
	int menu_change = 0;
	int k = 0;
	char temp[10];
	int aux1,aux2, aux3;			// zmienne pomocnicze 
	char* nnn = "    [[ / ]]     ";
	HDClearDisp();
	menu_change = 1;
	HDPrintString(MenuTitle,1,1);					// wyœwietlanie tytu³u menu
	while(((GPIOE->IDR & GPIO_IDR_IDR11) == GPIO_IDR_IDR11)) {
		if(((GPIOE->IDR & GPIO_IDR_IDR10) == GPIO_IDR_IDR10) && ((GPIOE->IDR & GPIO_IDR_IDR9) == GPIO_IDR_IDR9) && ((GPIOE->IDR & GPIO_IDR_IDR8) == GPIO_IDR_IDR8) && k == 1) {
			k = 0;
			menu_change = 0;
		}
		if (menu == 1 && menu_change == 1) {
			HDPrintString(MenuTitle,1,1);					// wyœwietlanie tytu³u menu
			HDPrintString(nnn,2,1);
			int2string(menu,temp);
			HDPrintString(temp,2,7);
			int2string(MainMenuC,temp);
			HDPrintString(temp,2,9);
			HDPrintString(VarioGraphLine,3,1);
			HDPrintString(SetQNHS,4,1);
		}
		if (menu == 2 && menu_change == 1) {
			HDPrintString(MenuTitle,1,1);					// wyœwietlanie tytu³u menu
			HDPrintString(nnn,2,1);
			int2string(menu,temp);
			HDPrintString(temp,2,7);
			int2string(MainMenuC,temp);
			HDPrintString(temp,2,9);
			HDPrintString(VarioGraphLine,3,1);
			HDPrintString(LoadStoredFlight1S,3,1);
			HDPrintString(LoadStoredFlight2S,4,1);
		}
		if(((GPIOE->IDR & GPIO_IDR_IDR9) == 0) && menu <= MainMenuC && k == 0) {
			// D-PAD w górê.. Przewijanie do nastêpnej pozycji
			menu++;
			k = 1;
			menu_change = 1;
			
		}
		else if(((GPIOE->IDR & GPIO_IDR_IDR10) == 0)&& menu > 0 && k == 0) {
			// D-PAD w dó³... Poprzednia pozycja menu
			menu--;
			k = 1;
			menu_change = 1;
		}
		else if(((GPIOE->IDR & GPIO_IDR_IDR12) == 0)&& menu == 1 && k == 0) {
			// D-PAD OK... Je¿eli wybrano pozycjê 1 to ustawianie QNH
			k = 1;
			HDClearDisp();
			SetQNHAtStartup();
			menu_change = 1;
		}
		else if(((GPIOE->IDR & GPIO_IDR_IDR12) == 0)&& menu == 2 && k == 0) {
			// D-PAD OK... Je¿eli wybrano pozycjê 2 to czytanie lotu z pamiêci
			k = 1;
			aux1 = *(int*)0x803F000;		// ilosc zapamietanych lotow
			aux2 = *(int*)0x803F004;		// niedawny lot
			aux3 = 1;						// aktualnie wybrany w menu indeks lotu
			HDClearDisp();
			while (((GPIOE->IDR & GPIO_IDR_IDR11) == GPIO_IDR_IDR11)) {
				if (aux1 > 0 && menu_change == 1) {
					HDPrintString(ChooseFlightStatsToView,1,1);
					HDPrintString(nnn,2,1);
					int2string(aux3,temp);
					HDPrintString(temp,2,7);
					int2string(aux1,temp);
					HDPrintString(temp,2,9);
					HDPrintString(VarioGraphLine,3,1);
					if (aux3 == aux2)
						HDPrintString(MostRecent,3,1);	
				}
				else if (aux1 == 0)
					HDPrintString(MemoryEmpty,1,1);
				else;
				if(((GPIOE->IDR & GPIO_IDR_IDR10) == GPIO_IDR_IDR10) && ((GPIOE->IDR & GPIO_IDR_IDR9) == GPIO_IDR_IDR9) && ((GPIOE->IDR & GPIO_IDR_IDR8) == GPIO_IDR_IDR8) && k == 1) {
					k = 0;
					menu_change = 0;
				}
				else if(((GPIOE->IDR & GPIO_IDR_IDR9) == 0) && aux3 < aux1 && k == 0) {
					// D-PAD w górê.. Przewijanie do nastêpnej pozycji
					aux3++;
					k = 1;
					menu_change = 1;
				}
				else if(((GPIOE->IDR & GPIO_IDR_IDR10) == 0)&& aux3 > 0 && k == 0) {
					// D-PAD w dó³... Poprzednia pozycja menu
					aux3--;
					k = 1;
					menu_change = 1;
				}
				else if(((GPIOE->IDR & GPIO_IDR_IDR12) == 0) && k == 0) {
					// D-PAD OK... Je¿eli wybrano pozycjê 1 to ustawianie QNH
					k = 1;
					HDClearDisp();
					FlightStats(ReadFlightStatsFromFlash(aux3-1));
					menu_change = 1;
				}
			}	
			menu_change = 1;
		}
		}

}



void FixString(char* input, int len) {
	// funkcja ma usuwaæ zbêdne znaczniki koñca ci¹gu powsta³e po funkcji strcpy
	// zamienia wszystkie naporkane przed koñcem znaki /0 na spacjê
	int i = 1;
	for(i; (i<colnum && i<len); i++) {
		if(*(input+i) == '\0' && i != (len+1)  )
			*(input+i) = 0x20;
	*(input+len) = '\0';	
	}
}


void Delay1ms(void) {
	int i;
	for (i = 0; i<0x3E5; i++);
}

void Delay10ms(void) {
	int i;
	for (i = 0; i<0x3E50; i++);
}











