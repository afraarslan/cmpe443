#ifndef TIMER_H
#define TIMER_H

#include "LPC407x_8x_177x_8x.h"
#include "SystemStructures.h"

typedef struct
{
  volatile	uint32_t IR;
  volatile	uint32_t TCR;
  volatile	uint32_t TC;
  volatile	uint32_t PR;
  volatile	uint32_t PC;
  volatile	uint32_t MCR;
  volatile	uint32_t MR0;
  volatile	uint32_t MR1;
  volatile	uint32_t MR2;
  volatile	uint32_t MR3;
  volatile	uint32_t CCR;
  volatile	uint32_t CR0;
  volatile	uint32_t CR1;
						uint32_t RESERVED0[2];
  volatile	uint32_t EMR;
						uint32_t RESERVED1[12];
  volatile	uint32_t CTCR;
} TIMER_TypeDef;

#define TIMER0_BASE	0x40004000
#define TIMER1_BASE	0x40008000
#define TIMER2_BASE	0x40090000
#define TIMER3_BASE	0x40094000

#define TIMER0	((TIMER_TypeDef*) TIMER0_BASE)
#define TIMER1	((TIMER_TypeDef*) TIMER1_BASE)
#define TIMER2	((TIMER_TypeDef*) TIMER2_BASE)
#define TIMER3	((TIMER_TypeDef*) TIMER3_BASE)

#define IOCON_TIMER_ADDRESS	0x4002C010
#define IOCON_TIMER	*((volatile uint32_t*)(IOCON_TIMER_ADDRESS))
	
#define	TIMER_PIN_IOCON_ADDRESS	0x4002C0F4
#define TIMER_PIN_IOCON	*((volatile uint32_t*)(TIMER_PIN_IOCON_ADDRESS))
	
#define	TIMER3_TRIGGER_IOCON_ADDRESS	0x4002C288
#define TIMER3_TRIGGER_IOCON	*((volatile uint32_t*)(TIMER3_TRIGGER_IOCON_ADDRESS))
	
#define	TIMER2_ECHO_IOCON_ADDRESS	0x4002C014
#define TIMER2_ECHO_IOCON	*((volatile uint32_t*)(TIMER2_ECHO_IOCON_ADDRESS))

void TIMER2_IRQHandler(void);
void ADC_Timer_Init();
void Ultrasonic_Trigger_Timer_Init(void);
void Ultrasonic_Capture_Timer_Init(void);

void Ultrasonic_Start_Trigger_Timer(void);
void TimerInit();
uint32_t calculateUSDistance();
extern uint32_t ultrasonicSensorRisingCaptureTime;
extern uint32_t ultrasonicSensorFallingCaptureTime;
extern uint32_t ultrasonicSensorDistance;
extern uint8_t ultrasonicSensorNewDataAvailable;

extern uint8_t ultrasonicSensorTriggerStart;
extern uint8_t ultrasonicSensorCaptureRisingEdge;

#endif
