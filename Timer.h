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

#define IOCON_P0_4_ADDRESS	0x4002C010
#define IOCON_P0_4	*((volatile uint32_t*)(IOCON_P0_4_ADDRESS))
	
#define PCONP_ADDRESS	0x400FC0C4
#define PCONP	*((volatile uint32_t*)(PCONP_ADDRESS))

#define CPU_CLOCK_FREQUENCY 120000000
#define PERIPHERAL_CLOCK_FREQUENCY 60000000

#endif