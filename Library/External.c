#include "External.h"


uint8_t counter=0;
uint32_t rotations=0;

void External_Init() {
	//Change the functionality of the push button as EINT0
	IOCON_SPEED_SENSOR |= (1<<0);
	IOCON_SPEED_SENSOR &= ~(1<<1 | 1<<2);
	
	//Change the External interrupt mode as Edge Sensitive
	EXT->EXTMODE |= (1<<0);
	
	//Change polarity of the External Interrupt as Low-active
	EXT->EXTPOLAR &= ~(1<<0);
	//Enable interrupt for EINT0_IRQn
	NVIC_EnableIRQ(EINT0_IRQn);
}

void EINT0_IRQHandler() {
	//Clear interrupt for EINT0
	EXT->EXTINT |= (1<<0);
	if(counter<1){
			counter++;
		}else{
			rotations++;
			counter=0;
	}
}
