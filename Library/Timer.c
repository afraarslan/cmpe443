#include "Timer.h"

uint32_t ultrasonicSensorRisingCaptureTime;
uint32_t ultrasonicSensorFallingCaptureTime;

uint32_t ultrasonicSensorDuration = 0;
uint32_t ultrasonicSensorDistance = 0;
uint8_t ultrasonicSensorNewDataAvailable = 0;

uint8_t ultrasonicSensorTriggerStart = 0;
uint8_t ultrasonicSensorCaptureRisingEdge = 0;

 void Ultrasonic_Capture_Timer_Init() {
	 uint32_t value;
	//Change the mode of Timer2 to Timer Mode
	PCONP |= 1<<22;
	 
	 
	TIMER2->CTCR &= ~(1<<0 | 1<<1);
	 
	value = TIMER2_ECHO_IOCON;
  value |= 3;
  value &= ~4;
  TIMER2_ECHO_IOCON = value;
	 
	 TIMER2->PR = 59;
	
	TIMER2->TCR &= ~(1 << 0);
	
	TIMER2->TCR |= (1 << 1);
	
	//Change CCR value for getting Interrupt when Rising Edge Occur for CAP 1
	TIMER2->CCR |= (1<<3 | 1<<4 | 1<<5);
	
	TIMER2->TCR &= ~(1 << 1);
	
	TIMER2->TCR |= (1 << 0);

	NVIC_EnableIRQ(TIMER2_IRQn);
}

 void Ultrasonic_Trigger_Timer_Init() {
	//Turn on Timer3.
	PCONP |= 1<<23;
	//Change the mode of Timer3 to Timer Mode.
	TIMER3->CTCR &= ~(1<<0 | 1<<1);
	uint32_t value;
	value = TIMER3_TRIGGER_IOCON;
  value |= 3;
  value &= ~4;
  TIMER3_TRIGGER_IOCON = value;
	
	TIMER3->TCR &= ~(1 << 0);
	
	TIMER3->TCR |= (1 << 1);
	
	//Change PR Register value for 1 microsecond incrementing
	TIMER3->PR = 59;
	
	//Write the Correct Configuration for EMR (LOW output value of Trigger Pin when match occurs and Initial value is LOW)
	TIMER3->EMR |= (1<<8);
	TIMER3->EMR &= ~(1<<2 | 1<<9);
	
	NVIC_EnableIRQ(TIMER3_IRQn);
	
	NVIC_SetPriority(TIMER3_IRQn,5);
	
	NVIC_ClearPendingIRQ(TIMER3_IRQn);
}

 void ADC_Timer_Init() {
	TIMER_PIN_IOCON &= ~(0x07);
	TIMER_PIN_IOCON |= 0x03;
	
	PCONP |= 1 << 1;
	
	TIMER0->CTCR = 0x0;
	
	TIMER0->TCR &= ~(1 << 0);
	
	TIMER0->TCR |= (1 << 1);
	
	TIMER0->PR = PERIPHERAL_CLOCK_FREQUENCY / 1000000 - 1;
	
	//Toggle the corresponding External Match bit/output when MR1 matches with TC.
	 TIMER0->EMR |= (1 << 1 );
	 TIMER0->EMR |= 3 << 6;
	
	//Reset the TC value whenever MR1 matches it
	 TIMER0->MCR |= (1<<4);
	
	//Configure MR1 as ADC will start every 100 milliseconds (Do not forget you configured ADC when rising edge occurs on TIMER 0 MATCH 1) 
	TIMER0->MR1 = 100000;
	
	TIMER0->TCR &= ~(1 << 1);
	
	TIMER0->TCR |= (1 << 0);
}
 
void Ultrasonic_Start_Trigger_Timer() {
	//Change output value of Trigger Pin as HIGH
	TIMER3->EMR |= (1<<2);
	
	//Give correct value to corresponding MR Register for 10 microsecond
	TIMER3->MR2 = 10;
	
	//Enable interrupt for MR2 register, if MR2 register matches the TC.
	TIMER3->MCR |= (1<<6);
	
	//Remove the reset on counters of Timer3.
	TIMER3->TCR &= ~(1<<1);
	
	
	//Enable Timer Counter and Prescale Counter for counting.
	TIMER3->TCR |=(1<<0);
}

void TIMER3_IRQHandler() {
	//Write HIGH bit value to IR Register for Corresponding Interrupt
	TIMER3->IR |= (1<<2);
	
	if(ultrasonicSensorTriggerStart == 0) {
		//Change MR2 Register Value for Suggested Waiting
		TIMER3->MR2 = TIMER3->TC + 60000;
		ultrasonicSensorTriggerStart = 1;
	}
	else {
		TIMER3->EMR |= (1 << 2);
		TIMER3->MR2 = 10 + TIMER3->TC;
		
		ultrasonicSensorTriggerStart = 0;
	}
}

void TIMER2_IRQHandler(void)
{
		TIMER2->IR |= 1 << 5;
	
		if(ultrasonicSensorCaptureRisingEdge == 1) {
			ultrasonicSensorRisingCaptureTime = TIMER2->CR1;
			
			LPC_TIM2->CCR = (1 << 4) | (1 << 5);
			ultrasonicSensorCaptureRisingEdge = 0;
		}
		else {
			ultrasonicSensorFallingCaptureTime = TIMER2->CR1;
			ultrasonicSensorNewDataAvailable = 1;
			
			LPC_TIM2->CCR = (1 << 3) | (1 << 5);
			ultrasonicSensorCaptureRisingEdge = 1;
		}
}

uint32_t calculateUSDistance(){
	ultrasonicSensorNewDataAvailable = 0;
	return (ultrasonicSensorFallingCaptureTime - ultrasonicSensorRisingCaptureTime)/58;
}

void TimerInit(){
	ADC_Timer_Init();
	Ultrasonic_Trigger_Timer_Init();
	Ultrasonic_Capture_Timer_Init();
	Ultrasonic_Start_Trigger_Timer();
}