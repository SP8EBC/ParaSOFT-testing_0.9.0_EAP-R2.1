#ifndef __SERIAL_H
#define __SERIAL_H

#ifndef __SERIAL

#include <stm32f10x.h>
#include "gpio_conf.h"

int SrlBRRegValue = 0x0341;		// dla symulacji ---- baudrate 9600bps
//int SrlBRRegValue = 0x0209;		// dla realnego uk³¹du 

int SrlTXData[32] = {'\0'};		// dane do wys³ania do zdalnego urz¹dzenia
int SrlRXData[32] = {'\0'};		// dane odebrane od zdalnego urz¹dzenia
int SrlTXQueueLen = 0;
int SrlTRXDataCounter = 0;
int SrlTXing = 0;
int SrlRXing = 0;
int SrlRXBytesNum = 0;			// liczba bajtow do odebrania
char SrlRxDummy;				// zmienna pomocnicza do niepotrzebnych danych

void SrlConfig(void);
void SrlSendData(char* data);
void USART1_IRQHandler(void);
void SrlReceiveData(char* data, int num);

#endif

#ifdef __SERIAL

extern int SrlTXData[32];		
extern int SrlRXData[32];		
extern int SrlTXQueueLen;
extern int SrlTRXDataCounter;
extern int SrlTXing;
extern int SrlRXing;
extern int SrlRXBytesNum;			
extern char SrlRxDummy;				

extern void SrlConfig(void);
extern void SrlSendData(char* data);
extern void USART1_IRQHandler(void);
extern void SrlReceiveData(char* data, int num);

#endif

#endif
