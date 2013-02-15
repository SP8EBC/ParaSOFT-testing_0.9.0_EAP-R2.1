#include "drivers/i2c.h"


void i2cConfigure() {			// funkcja konfiguruje pierwszy kontroler i2c!!!
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;	// w³¹czenie zegara dla i2c
//	DBGMCU->CR |= DBGMCU_CR_DBG_I2C1_SMBUS_TIMEOUT; // wy³¹czanie timeoutów podczas debugowania
	if (i2cPinRemap == 0) {
		Configure_GPIO(GPIOB,6,AFOD_OUTPUT_2MHZ);		//SCL
		Configure_GPIO(GPIOB,7,AFOD_OUTPUT_2MHZ);		//SDA
	}
	else {
		AFIO->MAPR |= AFIO_MAPR_I2C1_REMAP;
		Configure_GPIO(GPIOB,8,AFOD_OUTPUT_2MHZ);
		Configure_GPIO(GPIOB,9,AFOD_OUTPUT_2MHZ);				
	}
//	NVIC->IP[31] = 0x55;
	NVIC_SetPriority(I2C1_EV_IRQn, 5);
	I2C1->CR2 |= I2C_CR2_ITEVTEN;		// w³¹czenie generowanie przerwania od zdarzen i2c
	I2C1->CR2 |= I2C_CR2_ITBUFEN;
	I2C1->CR2 |= I2C_CR2_ITERREN;
	I2C1->CR2 |= i2cClockSpeed;
	I2C1->CCR |= i2cCCRegisterValue;
	I2C1->TRISE |= i2cRiseRegisterValue;
	I2C1->CR1 |= I2C_CR1_PE;			// uruchomienie i2c. To powinno byæ zawsze ostatnie
	NVIC_EnableIRQ( I2C1_EV_IRQn );		// w³¹czenie w kontrolerze przerwan
	NVIC_EnableIRQ( I2C1_ER_IRQn );
}

int i2cSendData(int addr, int* data, int null) {
	int i;
	for (i = 0; (i<32 && *(data+i) != '\0'); i++)
		i2cTXData[i]=data[i];
	if (null == 0x01) {					// je¿eli do slave trzeba wys³aæ 0x00
		i2cTXData[0] = 0x00;
		i = 1;
	}
	i2cTXQueueLen = i;
	i2cRemoteAddr = addr;
	
	i2cTXing = 1;
	i2cErrorCounter = 0;
	I2C1->CR1 |= I2C_CR1_START;			// zadanie warunkow startowych
	return 0;
}

int i2cReceiveData(int addr, int* data, int num) {
	i2cRXBytesNumber = num;
	i2cRemoteAddr = addr;
	i2cTRXDataCounter = 0;
//	NVIC_EnableIRQ( I2C1_EV_IRQn );		// w³¹czenie w kontrolerze przerwan
//	NVIC_EnableIRQ( I2C1_ER_IRQn );
	i2cRXing = 1;
	I2C1->CR1 |= I2C_CR1_START;			// zadanie warunkow startowych
	return 0;		 
}

void I2C1_EV_IRQHandler(void) {
NVIC_ClearPendingIRQ(I2C1_EV_IRQn);
//	int i;
	if ((I2C1->SR1 & I2C_SR1_SB) == I2C_SR1_SB && (i2cTXing == 1 || i2cRXing == 1)) {
	// po nadaniu warunków startu podczas wysy³ania danych do slave  EV5
		I2C1->DR = i2cRemoteAddr;				// wpisanie do DR adresu slave do nadania		
		I2C1->SR1 &= (0xFFFFFFFF ^ I2C_SR1_SB);	// gaszenie flagi SB



//		I2C1->SR1 &  I2C_SR1_SB;
	
	}
	if ((I2C1->SR1 & I2C_SR1_ADDR) == I2C_SR1_ADDR && (i2cTXing == 1 || i2cRXing == 1)) {
	// po wys³aniu adresu slave      EV6
		I2C1->SR1 &= (0xFFFFFFFF ^ I2C_SR1_ADDR);				
		I2C1->SR2 &= (0xFFFFFFFF ^ I2C_SR2_TRA);
//		I2C1->SR1 &= (0xFFFFFFFF ^ I2C_SR1_TXE);
//		for (i=0; i<0xFFF; i++);
		if (i2cRXBytesNumber == 1 && i2cRXing == 1) {
			/// EV_6_1
			I2C1->CR1 &= (0xFFFFFFFF ^ I2C_CR1_ACK);	// wy³czanie odpowiadania ACK przy odbiorze je¿eli tylko 1 bjt
			I2C1->CR1 |= I2C_CR1_STOP;
			}
//		if (i2cRXBytesNumber == 2 && i2cRXing == 1) {
//			/// EV_6_1
//			I2C1->CR1 &= (0xFFFFFFFF ^ I2C_CR1_ACK);	// wy³czanie odpowiadania ACK przy odbiorze je¿eli tylko 2 bajty
//			}
		if (i2cRXing == 1)
			I2C1->CR1 |= I2C_CR1_ACK;			// ustawienie bity ACK								
	}
	if ((I2C1->SR1 & I2C_SR1_TXE) == I2C_SR1_TXE && i2cTXing == 1) {
	//    EV_8_1
		I2C1->DR = i2cTXData[0];		
		i2cTRXDataCounter++;				
	}
	if (i2cTRXDataCounter == i2cTXQueueLen && i2cTXing == 1) {
	// przes³ano wszystkie dane, czyli teraz trzeba daæ warunki STOP
		i2cTXing = 0;
		I2C1->CR1 |= I2C_CR1_STOP;
		while ((I2C1->CR1 & I2C_CR1_STOP) == I2C_CR1_STOP);
//		NVIC_DisableIRQ( I2C1_EV_IRQn );
//		NVIC_DisableIRQ( I2C1_ER_IRQn );
		i2cVariableReset();
	}
	//	if ((I2C1->SR1 & I2C_SR1_RXNE) == I2C_SR1_RXNE && i2cTXing == 1) {
	if ((I2C1->SR1 & I2C_SR1_BTF) == I2C_SR1_BTF && i2cTXing == 1) { 
	// EV_8
		if ((I2C1->SR1 & I2C_SR1_TXE) == I2C_SR1_TXE && i2cTXing == 1 && i2cTRXDataCounter < i2cTXQueueLen) {	
				// zrobione ¿eby móc rêcznie wys³aæ ACK przez wirtualne rejestry
		I2C1->DR = i2cTXData[i2cTRXDataCounter];
		i2cTRXDataCounter++;
		I2C1->SR1 &= (0xFFFFFFFF ^ I2C_SR1_BTF);
		}					
	}	 
	if ((I2C1->SR1 & I2C_SR1_RXNE) == I2C_SR1_RXNE && i2cRXing == 1) {
	// EV_7
		*(i2cRXData + i2cTRXDataCounter) = I2C1->DR & I2C_DR_DR;
		i2cTRXDataCounter++;
		if (i2cRXBytesNumber-i2cTRXDataCounter == 1) {
			I2C1->CR1 &= (0xFFFFFFFF ^ I2C_CR1_ACK);	//jezeli odebrano przedostatni bajt to trzeba
														// zgasic bit ACK zeby nastepnie wyslano NACK na koniec	
		}
		if (i2cRXBytesNumber-i2cTRXDataCounter == 0) {
			I2C1->CR1 |= I2C_CR1_STOP;		// po odczytaniu z rejestru DR ostatniego bajtu w sekwencji
											// nastêpuje wys³anie warunków STOP na magistrale
//			NVIC_DisableIRQ( I2C1_EV_IRQn );// wy³¹czenie przerwania od kontrolera i2c
//			NVIC_DisableIRQ( I2C1_ER_IRQn );
			while ((I2C1->CR1 & I2C_CR1_STOP) == I2C_CR1_STOP);
			i2cRXing = 0;
			*(i2cRXData + i2cTRXDataCounter) = '\0';
			i2cVariableReset();
						
		}
													
	}


//I2C1->SR1 &= (0xFFFFFFFF ^ I2C_SR1_SB);
//I2C1->SR1 &= (0xFFFFFFFF ^ I2C_SR1_ADDR);				
//I2C1->SR2 &= (0xFFFFFFFF ^ I2C_SR2_TRA);
//I2C1->SR1 &= (0xFFFFFFFF ^ I2C_SR1_BTF);
	
}

void I2C1_ER_IRQHandler(void) {
	if (((I2C1->SR1 & I2C_SR1_AF) == I2C_SR1_AF) && i2cTRXDataCounter == 0 ) {
		// slave nie odpowiedzia³ ack na swój adres
		I2C1->SR1 &= (0xFFFFFFFF ^ I2C_SR1_AF);
		I2C1->CR1 |= I2C_CR1_STOP;		// zadawanie warunkow STOP i przerywanie komunikacji
		while ((I2C1->CR1 & I2C_CR1_STOP) == I2C_CR1_STOP);
		i2cErrorCounter++;				// zwieksza wartosc licznika b³êdów transmisji
		if(i2cErrorCounter >= 3) {
		//je¿eli wykryto ju¿ trzy b³êdy to przerwij
			i2cRXing = 0;			
			i2cTXing = 0;			
			i2cTXQueueLen = 0;		
			i2cTRXDataCounter = 0;
			i2cRXBytesNumber = 0;
//			NVIC_DisableIRQ( I2C1_ER_IRQn );
//			NVIC_DisableIRQ( I2C1_EV_IRQn );		
		}
		else
			I2C1->CR1 |= I2C_CR1_START;		// ponawianie komunikacji				

	}
	if (((I2C1->SR1 & I2C_SR1_AF) == I2C_SR1_AF) && i2cTRXDataCounter != 0 ) {
		//jezeli slave nie odpowiedzia³ ack na wys³any do niego bajt danych
		I2C1->SR1 &= (0xFFFFFFFF ^ I2C_SR1_AF);
		i2cErrorCounter++;
		if(i2cErrorCounter >= 3) {
			i2cRXing = 0;			
			i2cTXing = 0;			
			i2cTXQueueLen = 0;		
			i2cTRXDataCounter = 0;
			i2cRXBytesNumber = 0;
//			NVIC_DisableIRQ( I2C1_ER_IRQn );
//			NVIC_DisableIRQ( I2C1_EV_IRQn );						
		}
		else  
			i2cTRXDataCounter--;	// zmniejszanie wartoœci licznika danych aby nadac jeszcze raz to samo

	}
}

void i2cVariableReset(void) {
	I2C1->DR = 0x00;
	i2cTRXDataCounter = 0;
	i2cTXQueueLen = 0;
	i2cRXBytesNumber = 0;
}
