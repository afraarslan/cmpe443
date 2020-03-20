#ifndef ADC_H
#define ADC_H

#include "LPC407x_8x_177x_8x.h"

#include "SystemStructures.h"

typedef struct {
  volatile	uint32_t CR;
  volatile	uint32_t GDR;
						uint32_t RESERVED0;
  volatile	uint32_t INTEN;
  volatile	uint32_t DR[8];
  volatile	uint32_t STAT;
  volatile	uint32_t TRM;
} ADC_TypeDef;

#define ADC_CLOCK_FREQUENCY 1000000

//Write the IOCON address of Analog Pin
#define	ANALOG_PIN_IOCON_ADDRESS	 0x4002C05C
#define ANALOG_PIN_IOCON	*((volatile uint32_t*)(ANALOG_PIN_IOCON_ADDRESS))
	
#define	LDR1_PIN_IOCON_ADDRESS	 0x4002C064
#define LDR1_PIN_IOCON	*((volatile uint32_t*)(LDR1_PIN_IOCON_ADDRESS))
	
#define	LDR2_PIN_IOCON_ADDRESS	 0x4002C060
#define LDR2_PIN_IOCON	*((volatile uint32_t*)(LDR2_PIN_IOCON_ADDRESS))
	
#define	ULTRASONIC_PIN_IOCON_ADDRESS	 0x4002C060
#define ULTRASONIC_PIN_IOCON	*((volatile uint32_t*)(ULTRASONIC_PIN_IOCON_ADDRESS))

//Write the max value of ADC.
#define ADC_MAX_VALUE 0xFFF

//Define a ADC_CLKDIV variable for given ADC_CLOCK_FREQUENCY.
#define ADC_CLKDIV 59

#define ADC_BASE	0x40034000
#define ADC	((ADC_TypeDef*) ADC_BASE)

extern uint32_t ADC_Pm_Last;
extern uint8_t ADC_Pm_New_Data_Available;

void ADC_Init(void);

void ADC_Pm_Init(void);
void ADC_Pm_Start(void);
void ADC_Pm_Stop(void);
uint32_t ADC_Pm_GetLastValue(void);


extern uint32_t ADC_LDR_Last;
extern uint8_t ADC_LDR_New_Data_Available;
extern uint32_t LDR1_value;
extern uint32_t LDR2_value;

void ADC_LDR_Init(void);
void ADC_LDR_Start(void);
void ADC_LDR_Stop(void);
uint32_t ADC_LDR_GetLastValue(void);

#endif
