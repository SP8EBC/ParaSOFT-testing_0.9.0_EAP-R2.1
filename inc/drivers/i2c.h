#ifndef __I2C_H
#define __I2C_H

#ifndef __I2C

#include "gpio_conf.h"
#include <stm32f10x.h>

int i2cPinRemap = 0;
//int i2cClockSpeed = 8;  // w realu tu powinno byæ 5 przy moich ustawieniach zegara
int i2cClockSpeed = 5;
int i2cCCRegisterValue = 0x16;
//int i2cRiseRegisterValue = 0x09;	// w realu 6
int i2cRiseRegisterValue = 0x06;

volatile int i2cRemoteAddr = 0;			// adres zdalnego urz¹dzenia
volatile int i2cTXData[32] = {'\0'};		// dane do wys³ania do zdalnego urz¹dzenia
volatile int i2cRXData[32] = {'\0'};		// dane odebrane od zdalnego urz¹dzenia
volatile int i2cRXing = 0;				// ustawiony na 1 kiedy trwa odbiór danych
volatile int i2cTXing = 0;				// ustawiony na 1 kiedy trwa wysy³anie danych
volatile int i2cDone = 0;				// ustawiany na jeden w momencie zakoñczenia wysy³ania/odbioru
volatile int i2cTXQueueLen = 0;			// liczba bajtów w kolejce do wys³ania
volatile int i2cTRXDataCounter = 0;		// licznik odebranych/wyslanych danych
volatile int i2cRXBytesNumber = 0;		// liczba bajtow do odebrania
volatile int i2cErrorCounter = 0;		// liczbnik b³êdów transmisji

void i2cConfigure(void);
int i2cSendData(int addr, int* data, int null);
int i2cReceiveData(int addr, int* data, int num);
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);
void i2cVariableReset(void);

#endif

#ifdef __I2C

extern void i2cConfigure(void);
extern int i2cSendData(int addr, int* data, int null);
extern int i2cReceiveData(int addr, int* data, int num);
extern void I2C1_EV_IRQHandler(void);
extern void I2C1_ER_IRQHandler(void);
extern void i2cVariableReset(void);

extern volatile int i2cRemoteAddr;		
extern volatile int i2cTXData[32];
extern volatile int i2cRXData[32];
extern volatile int i2cRXing;			
extern volatile int i2cTXing;			
extern volatile int i2cDone;			
extern volatile int i2cTXQueueLen;		
extern volatile int i2cTRXDataCounter;	
extern volatile int i2cRXBytesNumber;	
extern volatile int i2cErrorCounter;	

#endif

#endif
