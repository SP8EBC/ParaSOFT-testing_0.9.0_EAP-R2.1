//#include "my.h"
#define STM32F10X_CL
#include "main.h"
#include <stm32f10x.h>

///////////////////////////////////////////////////////////////////////////////////
/////////////////////////// ParaSOFT wersja 0.9  //////////////////////////////////
////                                                                         //////
////              Autor: pil in�. Mateusz Lubecki SP8EBC                     //////
////         Akademicki Klub Lotniczy Politechniki Rzeszowskiej              //////



int i = 0;
int key_repeat = 0;
int keyb = 0;	// zmienna do wstrzymywania klawiszy
int param;
char *tekst = "testowy";
char dupa[7];
char duupa[7];
char dupa1[] = {0x45, 0x49, 0x35, '\0'};
int dupa2[8];
//char baro_s[9];
//char alti_s[9];
//char temp_s[9];
//char vario_s[9];

int tone_i = 0;

extern int j;

float flashtemp = 46523.563;
char ii;

///////
char* t_off = "Zegar wylaczony\n\r";
char* t_on = "Zegar wlaczony\n\r";
////


int main(void) 
{
	RCC->CR |= RCC_CR_HSEON;						// uruchmianie oscylatora HSE
	while (RCC->CR & RCC_CR_HSERDY != RCC_CR_HSERDY);	// czekanie na gotowo�� HSE
	RCC->CFGR |= RCC_CFGR_MCO_PLLCLK_Div2;			// ustawienie dzielnika na wyj�ciu zegarowym na /2
	RCC->CFGR |= RCC_CFGR_PLLMULL4;					// powielanie syntezy x4
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_PREDIV1;
	RCC->CFGR2 |= RCC_CFGR2_PREDIV2_DIV10;			// dzielnik przed synteza ustawiony na /10
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;				// dzielnik AHB ustawiony na /1
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;				// pierwszy dzielnik ABP (m.in. Timery od 2 do 7) na /2
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;				// drugi dzielnik ABP ustawiony na /2
	FLASH->ACR 	|= FLASH_ACR_LATENCY_0;				// konfiguracja bufor�w pami�ci flash
	RCC->CR	|= RCC_CR_PLLON;						// uruchamia synteze
    while(!(RCC->CR & RCC_CR_PLLRDY));				// i czeka az bedzie gotowa
	RCC->CFGR 	|= RCC_CFGR_SW_PLL;					// prze��cza �r�d�o sygna�u zegarowego na sytenze
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);	// czeka na ustabilizowanie sie zegara
	RCC->CFGR |= RCC_CFGR_MCO_SYSCLK;						// na wyjsciu zegarowym MCO sygnal taktujacy rdzen CPU
	RCC->APB2ENR |= ((1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6));	// wlanczenie zegara do portow I/O
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;				// uruchamianie zegar�w dla timer�w i kontrolera AFIO
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	///////////////////////////////////////////
	/// konfiguracja u�ywanych port�w GPIO	///
	///////////////////////////////////////////
	Configure_GPIO(GPIOE,14,GPOD_OUTPUT_2MHZ);			// wyj�cie otwarty dren
	Configure_GPIO(GPIOE,15,GPPP_OUTPUT_2MHZ);			// wyj�cie z podci�ganiem
	Configure_GPIO(GPIOE,8,PUD_INPUT);					// wej�cie z podci�ganiem
	Configure_GPIO(GPIOE,9,PUD_INPUT);
	Configure_GPIO(GPIOE,10,PUD_INPUT);
	Configure_GPIO(GPIOE,11,PUD_INPUT);
	Configure_GPIO(GPIOE,12,PUD_INPUT);
	GPIOE->ODR |= GPIO_ODR_ODR8;						// podci�gni�cie wej�cia PP do plusa zasilania
	GPIOE->ODR |= GPIO_ODR_ODR9;
	GPIOE->ODR |= GPIO_ODR_ODR10;
	GPIOE->ODR |= GPIO_ODR_ODR11;
	GPIOE->ODR |= GPIO_ODR_ODR12;
	Configure_GPIO(GPIOA,12,GPPP_OUTPUT_2MHZ);
	Configure_GPIO(GPIOA,11,GPPP_OUTPUT_2MHZ);
	GPIOA->BSRR |= GPIO_BSRR_BR12;
	GPIOA->BSRR |= GPIO_BSRR_BS11;
	////////////////////////////////////////////////////////////////////////////
	/// ustawia pin PA8 na funkcje alternatywna czyli wyjscie zegarowe		 ///
	////////////////////////////////////////////////////////////////////////////
	GPIOA->CRH |= (GPIO_CRH_MODE & GPIO_CRH_MODE8_0);				
	GPIOA->CRH |= (GPIO_CRH_MODE & GPIO_CRH_MODE8_1);
	GPIOA->CRH &= (0xFFFFFFFF ^ (GPIO_CRH_CNF & GPIO_CRH_CNF8_0));
	GPIOA->CRH |= (GPIO_CRH_CNF & GPIO_CRH_CNF8_1);
	////////////////////////////////////////////////////////////////////////////
	i2cConfigure();			// konfiguracja kontrolera i2c oraz pin�w we/wy przez niego u�ywanych
	SrlConfig();			// j/w ale dla portu szeregowego
	HDDispInit();			// inicjalizacja wy�wietlacza
	////////////////////////////////////////
	//// wy�wietlanie ekranu startowego	 ///
	////////////////////////////////////////
	HDPrintString(start1s,1,1);
	HDPrintString(start2s,2,1);
	HDPrintString(start3s,3,1);
	HDPrintString(start4s,4,1);
	for (i = 0; i<0xFFFFF; i++);     // op�nianie ekranu startowego
	HDClearDisp();					 // czysczenie zawarto�ci wy�wietlacza
	////////////////////////////////////////
	///////////////////////////
	/// konfiguracja TIM2 	///
	///////////////////////////
	NVIC_SetPriority(TIM2_IRQn, 5);				// priorytet przerwania
	TIM2->PSC = 1000;							// preskaler
	TIM2->ARR = 0xC86;							// di tej wartosci bedzie odliczalo
	TIM2->CR1 |= TIM_CR1_DIR;					//zliczanie w dol
	TIM2->CR1 &= (0xFFFFFFFF ^ TIM_CR1_DIR);  	// zliczanie w gore
	TIM2->CR1 |= TIM_CR1_CEN;
	TIM2->DIER |= 1;  							// w��cza zg�aszania przerwania
	NVIC_EnableIRQ( TIM2_IRQn );				// w��czanie kana�u przerwania w kontrolerze NVIC
	////////////////////////////
	qnh = *(int*)0x803E800;						// odczyt z flash zapisanej tam warto�ci QNH
	if ((GPIOE->IDR & GPIO_IDR_IDR11) == 0) {
		// je�eli przy starcie user wcisn�� "W LEWO" powrt�w do ustawie� domy�lnych
		qnh = 1013;
		ProgramFlashFromInt(0x803F000, 0);
		ProgramFlashFromInt(0x803F004, 0);
	}
	SetQNHAtStartup();							// wywo�anie funkcji pozwalaj�cen na ustawienie QNH
	ProgramFlashFromInt(0x803E800, qnh);		// zapisanie wybranej warto�ci qnh do pami�ci flash
	HDClearDisp();
	HDGotoXY(4,16);								// przesuwanie kursowa na ��dan� pozycj�
	SensorReset(0xEC);							// resetowanie czujnika oraz pobieranie danych kalibracyjnych
	////////////////////////////////////
	/// pierwszy odczyt z czujnika   ///
	////////////////////////////////////
	temp = SensorBringTemperature();	
	baro = SensorBringPressure();
	altitude = AltiFromPressure(baro, 1013);
	altitude2 = altitude;
	start_altitude = altitude;
	alti_th_start = altitude;
	alti_th_stop = altitude;
	///////////////////////////////////////
	/// zerowanie zmiennych procesowych ///
	///////////////////////////////////////
	i_alti_1 = 0.0;
	i_alti_2 = 0.0;
	vario = 0.0;
	//////////////////////////////
	////   konfiguracja TIM1   ///
	//////////////////////////////
	NVIC_SetPriority(TIM1_UP_IRQn, 9);
	TIM1->PSC = 1000;			// preskaler
	TIM1->ARR = 0x37A;			// di tej wartosci bedzie odliczalo		18x/sekunda
	TIM1->CR1 |= TIM_CR1_DIR;	//zliczanie w dol
	TIM1->CR1 &= (0xFFFFFFFF ^ TIM_CR1_DIR);  // zliczanie w gore
	TIM1->CR1 |= TIM_CR1_CEN;
	TIM1->DIER |= 1;  // w��cza Update Interrupt
	NVIC_EnableIRQ( TIM1_UP_IRQn );
	///////////////////////////////////////////////
	/// konfiguracja TIM6 do generowanie audio  ///
	///////////////////////////////////////////////
	TIM4->PSC = 1000;			// preskaler
	TIM4->ARR = 0x27;			// di tej wartosci bedzie odliczalo
	TIM4->CR1 |= TIM_CR1_DIR;	//zliczanie w dol
	TIM4->CR1 &= (0xFFFFFFFF ^ TIM_CR1_DIR);  // zliczanie w gore
	TIM4->CR1 |= TIM_CR1_CEN;
	TIM4->DIER |= 1;  // w��cza Update Interrupt
	NVIC_SetPriority(TIM4_IRQn, 11);
	NVIC_EnableIRQ( TIM4_IRQn );
	//////////////////////////////////////////////
	for(;;) {
		if( ((GPIOE->IDR & GPIO_IDR_IDR8) == GPIO_IDR_IDR8) && ((GPIOE->IDR & GPIO_IDR_IDR9) == GPIO_IDR_IDR9) && ((GPIOE->IDR & GPIO_IDR_IDR10) == GPIO_IDR_IDR10) && ((GPIOE->IDR & GPIO_IDR_IDR11) == GPIO_IDR_IDR11) && ((GPIOE->IDR & GPIO_IDR_IDR12) == GPIO_IDR_IDR12) && keyb == 1) {
		// zapobieganie dr�eniu klawiszy		
				keyb = 0;			// zmienna na dr�enie klawiszy
				key_repeat = 0;		// zmienna pomocnicza do obs�ugi zdarze� przytrzymania przycisku
		}
		if( ((GPIOE->IDR & GPIO_IDR_IDR12) == 0) && keyb == 0) {
		// zdarzenie na ekranie g��wnym od wci�ni�cia OK [ drugi od prawej ]
			MainMenu();
			GPIOE->ODR ^= (1 << 14); 
			keyb = 1;
		}
		if( ((GPIOE->IDR & GPIO_IDR_IDR9) == 0) && keyb == 0) {
		// zdarzenie na ekranie g��wnym od wci�ni�cia W GORE  [ pierwszy z prawej ]
			fl_clock_en ^= 1;					// w��czanie i wy��czenie odliczania czasu lotu
			GPIOE->ODR ^= (1 << 14); 
			keyb = 1;
			if (fl_clock_en == 1)
				start_altitude = altitude;			// je�eli odliczanie zosta�o w�a�nie uruchomione to ustaw wysoko�� startu
			if (fl_clock_en == 0)
				land_altitude = altitude;
		}		
		if( ((GPIOE->IDR & GPIO_IDR_IDR10) == 0) && keyb == 0) {
		// zdarzenie na ekranie g��wnym od wci�ni�cia D-pada W Dӣ  [ trzeci z prawej ] 
		/////
		// Cyklicznie zamienia wy�wietlanie wysoko�ciomierza pierwszego z drugim lub z max przewy�szeniem
			if (((disp_sw) & 0x03) == 0x02)
				disp_sw	&= (0xFFFFFFFF ^ 2);	// je�eli by�o max przewy�szenie do wr�� do alti1
			else if (((disp_sw) & 0x03) == 0x01) {
				disp_sw	&= (0xFFFFFFFF ^ 1);	// je�eli by�o alti2 to ustaw max przewy�szenie
				disp_sw |= 2;
			}
			else if (((disp_sw) & 0x03) == 0x00) 
				disp_sw |= 1;				// je�eli by�o alti1 do ustaw alti2


			GPIOE->ODR ^= (1 << 14); 
			keyb = 1;	
		}
		if( ((GPIOE->IDR & GPIO_IDR_IDR10) == 0) && keyb == 1) {
		///// zdarzenie od naci�ni�cia i przytrzymania D-pada w Dӣ  [ drugi z lewej ] 
			key_repeat++;						
			if (key_repeat == 0xF)
				altitude2 = altitude;			// resetowanie wysokoscimierza numer dwa
		}
		if( ((GPIOE->IDR & GPIO_IDR_IDR8) == 0) && keyb == 0) {
			// zdarzenie na ekranie g��wnym od wci�ni�cia W PRAWO  [ pierwszy z lewej ]
			struct FlightData lotp;						// definiowanie struktury zawieraj�cej zmienne procesowe lotu
			lotp.MaxSink = max_descending;
			lotp.MaxClimbing = max_climbing;
			lotp.MaxAltiGain = max_altitude_gain;
			lotp.BestThermalHeigh = best_thermal_height;
			lotp.FlightTime = fl_time;
			lotp.StartAltitude = start_altitude;
			lotp.LandingAltitude = land_altitude;
			FlightStats(lotp);							// wywo�ywanie podsumowania lotu
			GPIOE->ODR ^= (1 << 14); 
			keyb = 1;
		}
		if( ((GPIOE->IDR & GPIO_IDR_IDR11) == 0) && keyb == 0) {
			// zdarzenie na ekranie g��wnym od wci�ni�cia W LEWO
			GPIOE->ODR ^= (1 << 14); 
			keyb = 1;
		}

		if ((disp_sw & 0x01) == 0x01)
			altitude -= altitude2;				// je�eli u�ytkownik chc� widzie� drugi wysoko�cimierz
												// to przed wywo�aniem funkcji
		else if ((disp_sw & 0x02) == 0x02)		// je�eli u�ytkownik chce widzie� maksymalne przewy�szenie
			altitude = max_altitude_gain; 
		/////////////////////////////////////////////////////////////
		//             wywo�anie funkcji pokazuj�cej ekran g��wny
		MainScreen(vario,altitude,fl_time/2.5,disp_sw);
		
		///////////////////////////////////////////////////////
		if (SrlRXing == 0 && SrlTXing == 0) {
			if (SrlRXData[0] == 'q') {
				fl_clock_en ^= 1;
				SrlRXData[0] = 0x0;
				if (fl_clock_en == 0 && SrlRXing == 0)
					SrlSendData(t_off);
				if (fl_clock_en == 1 && SrlRXing == 0)
					SrlSendData(t_on);
			}
			if (SrlRXData[0] == 'z') {
				SrlRXData[0] = 0x0;
				j--;
			}
			if (SrlRXData[0] == 'x') {
				SrlRXData[0] = 0x0;
				j++;
			}
			if (SrlTXing == 0)
			SrlReceiveData(duupa,1);
				
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (altitude - start_altitude > max_altitude_gain)		// je�eli r�nica wysoko�ci i wysoko�ci startu jest wi�ksza ni�
			max_altitude_gain = altitude - start_altitude;		// zapisane maksymalne przewy�szenie to zaktualizuj jego wartosc
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (vario > max_climbing)								// je�eli warto�� aktualnego wznoszenia jest wi�ksza ni� to 
			max_climbing = vario;								// zarejestrowane jako najwi�ksze to zaktualizuj
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (vario < max_descending)								// jezeli warto�� aktualnego opadania jest wi�ksza ni� najmniejsze
			max_descending = vario;								// to zaktualizuj 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if ((iclimbing / 40) >= thermalling_thr)				// je�eli u�rednione wskazanie wariometru jest wi�ksze ni� zadany pr�g
			thermalling = 1; 									// to uznaj �e pilot zacz�� kr��y� w termice i ustaw odpowiedni� flag�
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		else if ((iclimbing / 40) < thermalling_thr)					// j/w ale reaguj�na zanik wznoszenia czyli wylecenie z komina
			thermalling = 0; 
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (thermalling == 0) {
			alti_th_start = altitude;							// gdy pilot nie kr��y w termice ustawiaj wysoko�� rozpocz�cie na aktualn�
																// w momencie rozpocz�cia kr��enia flaga przestawi si� na jeden i ta instrukcja
		}														// przestanie by� wykonywana pozostawiaj�c ostatni� warto�� 
		else if (thermalling == 1) {
			alti_th_stop = altitude;							// gdy pilot kr�zy w termice ustawiaj wysoko�� zako�czenia na aktualn�
		}														// dzia�a to na podobnej zasadzie jak powy�ej
			
		if (((alti_th_start - alti_th_stop) > best_thermal_height) && thermalling == 1)
			best_thermal_height =  (alti_th_start - alti_th_stop);
		
	}
}

void TIM1_UP_IRQHandler( void ) {
	/// g��wne przerwanie systemowe kt�re jest odpowiedzialne za dokonywanie pomiar�w z czujnika
	static int iii;											// statyczna zmienna pomocnicza u�ywana przy ca�kowaniu wariometru
	TIM1->SR &= ~(1<<0);
	temp = SensorBringTemperature();						// pomiar temperatury	
	baro = SensorBringPressure(); 							// pomiar ci�nienia
	temp = altitude;
	altitude = AltiFromPressure(baro, qnh);
	Vario(altitude);
	if (iii < 40) {
		iclimbing += vario;									// ca�kowanie ostatnich kilkudziesi�ciu wskaza� wariometru
		iii++;												// potrzebne to wyliczania warto�ci �redniej wznoszenia/opadania za zadany okres
	}
	if (iii >= 40) {
		iclimbing = 0;
		iii = 0;
	}	
	tone_i++;			
}

void TIM2_IRQHandler(void) {
	// timer s�u��cy do odliczania czasu
	if ((GPIOE->ODR & GPIO_ODR_ODR15)  == GPIO_ODR_ODR15) {
		GPIOE->BSRR |= GPIO_BSRR_BR15;
	}
	else if ((GPIOE->ODR & GPIO_ODR_ODR15)  == 0) {
		GPIOE->BSRR |= GPIO_BSRR_BS15;
	}
	TIM2->SR &= ~(1<<0);
	key = 0;
	if (fl_clock_en == 1)
		fl_time++;
}
