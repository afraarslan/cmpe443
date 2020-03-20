#include "TIMER.h"

#define CLOCKDIV _____
#define CLOCKPERIOD _____

enum edge_state {yes,first,no};
static uint32_t both_captured=no;
static uint32_t rising_edge, falling_edge, timeout, pulsewidth;


void TIMER2_Init(void)
{
    
    uint32_t value;
    
    //Step 1 - Turn on TIMER2 module: PCTIM2, bit22
    PCONP |= 1<<22;
    //Step 2 - Set mode of P0_4 as T2_CAP0 (IOCON_P0_4[2:0]=3)
    value = IOCON_P0_4;
    value |= 3;
    value &= ~4;
    IOCON_P0_7 = value;
    //Step 3 - Determine Prescalar value PR if necessary
    TIMER2->PR = CLOCKDIV-1;
    //Step 4 - The timer will detect a positive pulse and generate an interrupt at both edges
    TIMER2->CCR |= 7;
    //Step 5 - Enable Timer Counter, Clear CRST (i.e., reset counter)
    TIMER2->TCR = 2; //Synchronously reset TC and PC
    TIMER2->TCR = 1; //Enable TC and PC to run from 0.
    //Step 6 - Clear interrupt flag of CAP0
    TIMER2->IR |= 4;
    //Step 7 - Enable TIMER2_IRQ interrupt
    NVIC_ClearPendingIRQ(TIMER2_IRQ);
    NVIC_EnableIRQ(TIMER2_IRQ);
    //

 }

void TIMER2_IRQHandler(void)
{
 //Clear interrupt flag of CAP0
    TIMER2->IR |= 4;
    if(both_captured==no) {
        both_captured=first;
        rising_edge=TIMER2->CR0;
    }
    else if (both_captured==first) {
        both_captured=yes;
        falling_edge=TIMER2->CR0;
    }
    else both_captured=no;
}

void main (void)
{
    TIMER2_Init();
    __enable_irq();
    while(1)
    {
        __WFI();
        if (both_captured == yes)
        {
            timeout=falling_edge-rising_edge;
            pulsewidth=timeout*CLOCKPERIOD;
        }
    }
}