#include "drivers/hddisp.h"

// funkcja wysy�aj�ca do wy�wietlacza 8 bit�w po 4 bitowej magistrali
int HDDispSendByte(char Byte, enum DataOrOrdnung RS) {
	char LSB,MSB;	// starsze i mlodsze 4 bity do wyslania na wyswietlacz
//	int mask_null;	// maska do zerowania cztero bitowej magistrali danych
	LSB = (Byte & 0x0F);		// wyciaganie czterech najmniej znaczacych bitow
	MSB = (Byte & 0xF0) >> 4;	// wyciaganie czterech najbardziej znaczacych bitow 	
//	mask_null = 0xFFFFFFFF ^ (0x0000000F << FirstPin);	//maska do zerowania pinow magistrali wyswietlacza na porcie
	if (RS == ORDNUNG)
		Port->ODR &= RS_LOW; 		// je�eli przesy�any jest rozkaz to ustaw linie R/S na niski stan
	if (RS == DATA)
		Port->ODR |= RS_HIGH;		// je�eli przesy�ane s� dane to ustaw linie R/S na wysoki stan logiczny
	Port->ODR &= RW_LOW;
	Port->ODR |= EN_HIGH;
	Port->ODR &= DB_LINES_NULL;		// zerowanie lini danych
	Port->ODR |= MSB << FirstPin;	// wysy�anie danych
	Port->ODR &= EN_LOW;			// gaszenie pinu EN
	HDDispDelay(10);				// op�nienie dla wy�wietlacza na przemielenie danych
	Port->ODR |= EN_HIGH;
	Port->ODR &= DB_LINES_NULL;		// zerowanie magistrali
	Port->ODR |= LSB << FirstPin;	// wysy�anie danych
	Port->ODR &= EN_LOW;
	HDDispDelay(10);				
	return 0;
}

// funkcja j/w ale wysy�a tylko cztery bity (najm�odsze 4 z wej�cia)
int HDDispSend4Bits(char Byte){
	Port->ODR &= RW_LOW;
	Port->ODR &= RS_LOW;
	Port->ODR |= EN_HIGH;
	Port->ODR &= DB_LINES_NULL;
	Port->ODR |= Byte << FirstPin;
	Port->ODR &= EN_LOW;
	HDDispDelay(1);	
	return 0;	
}

// opoznienie o 40mikro sekund
void HDDispDelay(int num) {		
	int i,j;
	for (j = num; j>0; j--) 
		for (i=0; i<200; i++);
}

// Funkcja inicjuj�ca wy�wietlacz do pracy na magistrali 4 bity
int HDDispInit(void) {
	HDDispDelay(4000);
	HDConfigureGPIO();
	HDDispSend4Bits(0x03);
	HDDispDelay(1100);
	HDDispSend4Bits(0x03);
	HDDispDelay(100);
	HDDispSend4Bits(0x03);
	HDDispDelay(100);
	HDDispSend4Bits(0x02);
	HDDispDelay(20);
	HDDispSendByte(0x2C,ORDNUNG);
	HDDispSendByte(0x08,ORDNUNG);
	HDDispSendByte(0x06,ORDNUNG);
	HDDispSendByte(0x01,ORDNUNG);
	HDDispDelay(800);
	HDDispSendByte(0x0F,ORDNUNG);
	return 0;
} 

// funkcja konfigutuj�ca porty GPIO do kt�rych jest pod��czony wy�wietlacz
void HDConfigureGPIO(void) {
	Configure_GPIO(Port,FirstPin,GPOD_OUTPUT_2MHZ);
	Configure_GPIO(Port,FirstPin+1,GPOD_OUTPUT_2MHZ);
	Configure_GPIO(Port,FirstPin+2,GPOD_OUTPUT_2MHZ);
	Configure_GPIO(Port,FirstPin+3,GPOD_OUTPUT_2MHZ);
	Configure_GPIO(Port,FirstPin+4,GPOD_OUTPUT_2MHZ);
	Configure_GPIO(Port,FirstPin+5,GPOD_OUTPUT_2MHZ);
	Configure_GPIO(Port,FirstPin+6,GPOD_OUTPUT_2MHZ);
}

int HDGetOrdnungFromAddr(int addr) {
	if (addr < 0 || addr >= 128)
		return -1;
	return addr+128;

}

// ustawianie kursora na wy�wietlaczu w zadanej pozycji WxK
int HDGotoXY(int row, int column) {
	if (row > ROWNUM || column > COLNUM)
		return -1;				// je�eli podano adres spoza obszaru wy�wietlacza
	switch (row) {
		case 1:
			return HDDispSendByte(HDGetOrdnungFromAddr(FIRST_ROW_ADDR + column -1), ORDNUNG);
		case 2:
			return HDDispSendByte(HDGetOrdnungFromAddr(SECOND_ROW_ADDR + column -1), ORDNUNG);
		case 3:
			return HDDispSendByte(HDGetOrdnungFromAddr(THIRD_ROW_ADDR + column -1), ORDNUNG);
		case 4:
			return HDDispSendByte(HDGetOrdnungFromAddr(FOURTH_ROW_ADDR + column -1), ORDNUNG);
		default:
			return -1;	
	}		
}

// Wy�wietlanie zadanego napisu od konkretnej wsp�rz�dnej na wy�wietlaczu
int HDPrintString(char* string, int srow, int scol) {
	int i;
	if (srow > ROWNUM || scol > COLNUM)
		return -1;					// je�eli zadane wsp�rz�dne wychodz� poza rozdzielczo�� wy�wietlacza
	HDGotoXY(srow, scol);			// ustawia kursor w miejscu gdzie ma si� zaczyna� ci�g znak�w
	for (i = 0; *(string + i) != '\0'; i++) {
		HDDispSendByte(*(string + i), DATA);		// wysy�a dane do wy�wietlacza do puki nie natrafiono na koniec ci�gu
	}
	return 0;
}

// czyszczenie zawarto�ci wy�wietlacza
void HDClearDisp(void) {
	int i,j;
	for (i = 1; i <= ROWNUM; i++) {
		HDGotoXY(i,1);
		for (j = 1; j <= COLNUM; j++)
			HDDispSendByte(' ', DATA);
		}
}


