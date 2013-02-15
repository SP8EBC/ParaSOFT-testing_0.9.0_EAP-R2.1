#ifndef __HDDISP_H
#define __HDDISP_H

#ifndef __HDDISP

#include <stm32f10x.h>
#include "gpio_conf.h"


////////////////////////// ROZLOZENIE PINOW WYSWIETLACZA HD44780
// DB4 - GPIOx:FirstPin
// DB5 - GPIOx:FirstPin+1
// DB6 - GPIOx:FirstPin+2
// DB7 - GPIOx:FirstPin+3
// EN - GPIOx:FirstPin+4
// RW - GPIOx:FirstPin+5
// RS - GPIOx:FirstPin+6
//////////////////////////	

GPIO_TypeDef* Port = GPIOE;	//port do ktorego zastal podlanczony wyswietlacz
int FirstPin = 0;

enum DataOrOrdnung {
	DATA,ORDNUNG
};

#define DB_LINES_NULL	(0xFFFFFFFF ^ (0x0000000F << FirstPin))
#define EN_HIGH			(0x00000001 << FirstPin+4)			
#define EN_LOW			(0xFFFFFFFF ^ (0x00000001 << FirstPin+4))
#define RW_HIGH			(0x00000001 << FirstPin+5)
#define RW_LOW			(0xFFFFFFFF ^ (0x00000001 << FirstPin+5))
#define RS_HIGH			(0x00000001 << FirstPin+6)
#define RS_LOW			(0xFFFFFFFF ^ (0x00000001 << FirstPin+6))

#define ROWNUM			4		// liczba wierszy wyswietlacza
#define	COLNUM			16		// liczba kolumn wyswietlacza
#define FIRST_ROW_ADDR	0		// adres pierwszego znaku pierwszego wiersza
#define SECOND_ROW_ADDR	64		// adres pierwszego znaku drugiego wiersza
#define THIRD_ROW_ADDR	16		// adres pierwszego znaku trzeciego wiersza
#define FOURTH_ROW_ADDR	80		// adres pierwszego znaku czwartego wiersza

int colnum = COLNUM;

int HDDispSendByte(char Byte, enum DataOrOrdnung RS);
void HDDispDelay(int num);
void HDConfigureGPIO(void);
int HDDispInit(void);
int HDDispSend4Bits(char Byte);
int HDGetOrdnungFromAddr(int addr);
int HDGotoXY(int row, int column);
int HDPrintString(char* string, int srow, int scol);
void HDClearDisp(void);
#endif

#ifdef __HDDISP

extern int HDDispSendByte(char Byte, enum DataOrOrdnung RS);
extern void HDDispDelay(int num);
extern void HDConfigureGPIO(void);
extern int HDDispInit(void);
extern int HDDispSend4Bits(char Byte);
extern int HDGetOrdnungFromAddr(int addr);
extern int HDGotoXY(int row, int column);
extern int HDPrintString(char* string, int srow, int scol);
extern void HDClearDisp(void);
#endif


#endif
