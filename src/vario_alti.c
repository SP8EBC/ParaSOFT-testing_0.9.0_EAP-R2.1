#include "vario_alti.h"

// funkcja obliczaj¹ca wartoœæ wysokoœci na podstawie zmierzonego ciœnienia
// oraz podanego QNH. Linijki oznaczone jako "pomiary" s¹ odpowiedzialne za 
// transmisjê tych odczytów przez port szeregowy
float AltiFromPressure(float pressure, int qnh) {
	char temp[10];					// pomiary
	char* endl = ";\n\r";			// pomiary
	float output, powexp;
	powexp = 1/5.255;
	output = 44330 * (1 - pow((pressure / qnh),powexp));
	if (SrlTXing != 1) {					 //// pomiary
	float2string(pressure,temp, 2);			 /// pomiary
	strcpy(temp+7,endl);						 /// pomiary
	FixString(temp,10);					 //// pomiary
	SrlSendData(temp);						 //// pomiary
	}
	return output;
}

// funkcja uœredniaj¹ca wskazania wariometru. Ma pracowaæ jako swego rodzaju
// dolnoprzepustowy filtr szumów
float Vario (float calti) {
//	if (ai == 0) {
//		// czyszczenie zmiennych procesowych
//		i_alti_1 = calti;
//		i_alti_2 = 0.0;
//	}
//	if (ai < 5 && ai > 0) {	
//		i_alti_1 = i_alti_1*0.98 + calti*0.02;		// zbieranie pierwszych piêciu próbek
//	}
//	if (ai == 5)
//		i_alti_2 = calti;
//	if (ai > 5) {
//		i_alti_2 = i_alti_2*0.98 + calti*0.02;		// zbieranie piêciu kolejnych
//	}
//	if (ai == 9) {
//		ai = -1;
//		vario = i_alti_1*0.2 - i_alti_2*0.2;		// obliczanie wskazañ oraz obliczanie wskazañ vario
//		vario *= 1.8;
//	}
//	ai++;
//	return vario;			 	// zwracanie otrzymanego wskazania wariometru
	if (ai == 0)
		i_alti_1 = calti;
	else if (ai < 18)
		i_alti_1 += calti;
	else if (ai == 18) {
		if (i_alti_2 != 0.0)
			vario = (i_alti_1/18 - i_alti_2/18);
		i_alti_2 = calti;
	}
	else if (ai > 18 && ai != 35)
		i_alti_2 += calti;
	else if (ai == 35) {
		i_alti_2 += calti;
		vario = (i_alti_2/18 - i_alti_1/18);
		ai = -1;
	}
	else;	
	ai++;
	return vario;

}

// funkcja wywo³ywana na starcie urz¹dzenia. Umo¿liwia ustawienie wartoœci QNH
void SetQNHAtStartup(void) {
	// PE10 -- zmiejszanie
	// PE9 -- zwiekszanie
	// PE8 -- OK
	extern int key;
	char temp[9];
	char* hpa = "hPa";
	char* blank = "    ";
	key = 0;
	HDPrintString(str1,2,4);
	int2string(qnh,temp);
	HDPrintString(temp,3,5);
	HDPrintString(hpa,3,10);
	while((GPIOE->IDR & GPIO_IDR_IDR12) == GPIO_IDR_IDR12) {		// dopuki nie zosta³ naciœniety przycisk OK
		if( ((GPIOE->IDR & GPIO_IDR_IDR10) == GPIO_IDR_IDR10) && ((GPIOE->IDR & GPIO_IDR_IDR9) == GPIO_IDR_IDR9) && key == 1) {	// drzenie przycisków
			key = 0;
		}
		if( (GPIOE->IDR & GPIO_IDR_IDR10) == 0 && key == 0) {
			// obs³uga zdarzenia naciœniêcie zmniejszania
			qnh--;
			int2string(qnh,temp);
			HDPrintString(blank,3,5);
			HDPrintString(temp,3,5);
			key = 1;
		}
		if( (GPIOE->IDR & GPIO_IDR_IDR9) == 0 && key == 0) {
			// obs³uga zdarzenia naciœniêcie zwiekszania
			qnh++;
			int2string(qnh,temp);
			HDPrintString(blank,3,5);
			HDPrintString(temp,3,5);
			key = 1;
		}		
	}
	fl_time = 0;
}

void StoreFlightStatsToFlash(struct FlightData flight) {
	int fligts_in_memory, most_recent_flight;					// iloœæ zapisanych w pamiêci lotów
	fligts_in_memory = *(int*)0x803F000;						// odczytywanie iloœci zapisanych lotów w pamiêci
	most_recent_flight = *(int*)0x803F004;						// numer najnowszego lotu
	ProgramFlashFromAddr(0x803F008 + fligts_in_memory * 0x28,(int*)&flight.MaxSink);
	ProgramFlashFromAddr(0x803F00C + fligts_in_memory * 0x28,(int*)&flight.MaxClimbing);
	ProgramFlashFromAddr(0x803F010 + fligts_in_memory * 0x28,(int*)&flight.MaxAltiGain);
	ProgramFlashFromAddr(0x803F014 + fligts_in_memory * 0x28,(int*)&flight.BestThermalHeigh);
	ProgramFlashFromAddr(0x803F018 + fligts_in_memory * 0x28,(int*)&flight.FlightTime);
	ProgramFlashFromAddr(0x803F01C + fligts_in_memory * 0x28,(int*)&flight.StartAltitude);
	ProgramFlashFromAddr(0x803F020 + fligts_in_memory * 0x28,(int*)&flight.LandingAltitude);
	//////////////////////////////////////////////////////////////
	// INDEKSY LOTÓW ZACZYNAJ¥ SIÊ OD ZERA A NIE OD JEDYNKI!!   //
	// 															//
	// > Pamiêæ lotów mo¿e przechowywaæ maksymalnie 16 pozycji	//
	// > Gdy zostanie wyczerpana nadpisywane s¹ najstarsze loty	//
	//////////////////////////////////////////////////////////////
	if (fligts_in_memory < 15)
		fligts_in_memory++; 					// zwiêkszanie licznika zapisanych lotów o jeden
	if (most_recent_flight < 15)
		most_recent_flight++;					// zwiêkszanie numeru niedawnego lotu
	else
		most_recent_flight = 0;
	ProgramFlashFromInt(0x803F000, fligts_in_memory); 		// zapis aktualnej iloœci lotów
	ProgramFlashFromInt(0x803F004, most_recent_flight);		// zapis numeru niedawnego lotu
}

// funkcja odczytuje z pamiêci flash zapisane dane o locie
// a nastêpnie zwraca je w postaci pól struktury
struct FlightData ReadFlightStatsFromFlash(int index) {
	struct FlightData buffer;
	// jeden lot zajmuje w pamiêci 40 bajtów, pomimo i¿ nie ca³a przestrzeñ jest wykorzystywana
	// pozostawiono "dziury" aby umo¿liwiaæ w przysz³oœci dogrywanie innych danych
	buffer.MaxSink = *(float*)(0x803F008 + index * 0x28);
	buffer.MaxClimbing = *(float*)(0x803F00C + index * 0x28);
	buffer.MaxAltiGain = *(float*)(0x803F010 + index * 0x28);
	buffer.BestThermalHeigh = *(float*)(0x803F014 + index * 0x28);
	buffer.FlightTime = *(int*)(0x803F018 + index * 0x28);
	buffer.StartAltitude = *(float*)(0x803F01C + index * 0x28);
	buffer.LandingAltitude = *(float*)(0x803F020 + index * 0x28);
	return buffer;
}

// funkcja odpowiedzialna za sterowanie sygnalizacj¹ dŸwiêkowa waria.
// Zmienia konfiguracjê TIM4 w zale¿noœci od aktualnej wartoœci wznoszenia/opadania
void TheMostBeutifulSoundForEveryParagliderPilotOnTheWholeWourldGenerator (int tone) {
	TIM4->ARR = vario_tones[0+tone];
	cycle = pulse_cycle[0+tone];	 
}

void TIM4_IRQHandler(void) {
	TIM4->SR &= ~(1<<0);
	if (tone_i % cycle == 0 ) {
		if ((GPIOA->ODR & GPIO_ODR_ODR12)  == GPIO_ODR_ODR12) {
			GPIOA->BSRR |= GPIO_BSRR_BR12;
		}
		else if ((GPIOA->ODR & GPIO_ODR_ODR12)  == 0) {
			GPIOA->BSRR |= GPIO_BSRR_BS12;
		}
		if ((GPIOA->ODR & GPIO_ODR_ODR11)  == GPIO_ODR_ODR11) {
			GPIOA->BSRR |= GPIO_BSRR_BR11;
		}
		else if ((GPIOA->ODR & GPIO_ODR_ODR11)  == 0) {
			GPIOA->BSRR |= GPIO_BSRR_BS11;
		}
	}
}
