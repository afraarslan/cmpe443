#include "ADC.h"

uint32_t ADC_Pm_Last = 0;
uint8_t ADC_Pm_New_Data_Available = 0;

uint32_t ADC_LDR_Last = 0;
uint8_t ADC_LDR_New_Data_Available = 0;
uint32_t LDR1_value = 0;
uint32_t LDR2_value = 0;

void ADC_Init() {
	uint32_t value;
	//Turn on ADC.
	PCONP |= (1<<12);
	
	//Set the CLKDIV and make the A/D converter operational without Burst mode.
	value = ADC->CR;
	value &= (0xFFFF0000);
	value |= (ADC_CLKDIV<<8);
	value &= ~(1<<16);

	//Make the A/D converter operational
	value |= (1<<21);
	ADC->CR = value;
	ADC_Pm_Init();
	ADC_LDR_Init();
	
	
	//Enable ADC_IRQn (Interrupt Request).
	NVIC_EnableIRQ(ADC_IRQn);
	
}

void ADC_LDR_Init() {
	uint32_t value;
	//Change the function value of pin to ADC.
	value = LDR1_PIN_IOCON;
	value &= ~(7);
	value |= (1<<0);
	
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	value &= ~(1<<3 | 1<<4 |1<<7);
	LDR1_PIN_IOCON = value;
	
	value = LDR2_PIN_IOCON;
	value &= ~(7);
	value |= (1<<0);
	
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	value &= ~(1<<3 | 1<<4 |1<<7);
	LDR2_PIN_IOCON = value;
	
	value = ADC->CR; 
	value |= (1<<1 |1<<2);
	ADC->CR = value;
	
	//Enable interrupt for corresponding pin.
	ADC->INTEN |= (1<<1 | 1<<2);
	
	ADC_LDR_Start();
}

void ADC_LDR_Start () {
	//Write a code for starting A/D conversion on a rising edge on the TIMER 0 MATCH 1.
	uint32_t value = ADC->CR;
	value &= ~(15<<24);
	value |= (1<<26);
	ADC->CR = value;
}

uint32_t ADC_LDR_GetLastValue() {
	ADC_LDR_New_Data_Available = 0;
	return ADC_LDR_Last;
}

void ADC_Pm_Init() {
	uint32_t value;
	//Change the function value of pin to ADC.
	value = ANALOG_PIN_IOCON;
	value &= ~(7);
	value |= (1<<0);
	
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	value &= ~(1<<3 | 1<<4 |1<<7);
	ANALOG_PIN_IOCON = value;
	
	value = ADC->CR; 
	//Change Analog/Digital mode of pin to Analog.
	value |= (1<<0);
	ADC->CR = value;
	
	//Enable interrupt for corresponding pin.
	ADC->INTEN |= (1<<0);
	
	ADC_Pm_Start();
}

void ADC_Pm_Start () {
	//Write a code for starting A/D conversion on a rising edge on the TIMER 0 MATCH 1.
	uint32_t value = ADC->CR;
	value &= ~(15<<24);
	value |= (1<<26);
	ADC->CR = value;
}

uint32_t ADC_Pm_GetLastValue() {
	ADC_Pm_New_Data_Available = 0;
	return ADC_Pm_Last;
}

void ADC_IRQHandler() {
	ADC->GDR &= ~((uint32_t)1 << 31);
	
	//Write the converted data (only the converted data) to ADC_Last variable.	
	if(ADC->STAT & (1<<0)){
		ADC_Pm_Last = ADC->DR[0];
		ADC_Pm_Last = ADC_Pm_Last << 16;
		ADC_Pm_Last = ADC_Pm_Last >> 20;
		
		ADC_Pm_New_Data_Available = 1;
	}
	if(ADC->STAT & (1<<2)){
		LDR1_value = ADC->DR[1];
		LDR1_value = LDR1_value << 16;
		LDR1_value = LDR1_value >> 20;
		LDR2_value = ADC->DR[2];
		LDR2_value = LDR2_value << 16;
		LDR2_value= LDR2_value >> 20;
		ADC_LDR_Last = (LDR1_value  + LDR2_value) /2;
		
		ADC_LDR_New_Data_Available = 1;
	}
}
