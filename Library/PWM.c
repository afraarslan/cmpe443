#include "PWM.h"

void PWM_Init() {
	set_pwm_func(&IOCON_LEFT_LED1);
	set_pwm_func(&IOCON_LEFT_LED2);
	set_pwm_func(&IOCON_RIGHT_LED1);
	set_pwm_func(&IOCON_RIGHT_LED2);
	set_pwm1_func(&IOCON_MOTORA_SPEED);
	set_pwm1_func(&IOCON_MOTORB_SPEED);
	
	PCONP |= (1 << 5 | 1<<6);
	
	//Enable PWM output for corresponding pin.
	PWM0->PCR |= (1<<10 |1<<9 |1<<12 |1<<11);
	PWM1->PCR |= (1<<13 | 1<<12);
	
	PWM0->TCR = 1 << 1;
	
	PWM0->PR = 0;
	
	PWM0->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * 1000 * 1000;
	
	PWM1->TCR = 1 << 1;
	
	PWM1->PR = 9;
	
	PWM1->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * 20* 1000;
	
	//Reset TC, when MR0 matches TC. Also generate an interrupt when MR0 matches the value in the TC.
	PWM0->MCR = 1 << 1;
	
	PWM0->LER |= 1 << 0;
	
	PWM0->TCR = (1 << 0 | 1 << 3);
	
	PWM1->MCR = 1 << 1;
	
	PWM1->LER |= 1 << 0;
	
	PWM1->TCR = (1 << 0 | 1 << 3);
	
	
	//PWM_Write(50);
}

void set_pwm_func(volatile uint32_t* iocon){
	uint32_t ioconRegisterValue = *iocon;
	ioconRegisterValue |= 0x3;
	ioconRegisterValue &= ~(1 << 2);
	*iocon = ioconRegisterValue;
}

void set_pwm1_func(volatile uint32_t* iocon){
	uint32_t ioconRegisterValue = *iocon;
	ioconRegisterValue |= 0x2;
	ioconRegisterValue &= ~(1 << 2 | 1<<0);
	*iocon = ioconRegisterValue;
}

void PWM_Cycle_Rate(uint32_t period_In_Cycles, PWM_TypeDef *PWMX) {
	PWMX->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * period_In_Cycles * 1000;
	
	PWMX->LER |= 1 << 0;
}


void PWM_Write(uint32_t T_ON, uint8_t mr_number, PWM_TypeDef *PWMX) {	
	if(T_ON > 100) {
		T_ON = 100;
	}
	
	T_ON = (uint32_t)(((PWMX->MR0) * T_ON) / 100);
	
	if (T_ON == PWMX->MR0) {
		T_ON++;
	}
	if(mr_number == 1){
		PWMX->MR1 = T_ON;
		PWMX->LER |= 1 << 1;
	}else if(mr_number==2){
		PWMX->MR2 = T_ON;
		PWMX->LER |= 1 << 2;
	}else if(mr_number==3){
		PWMX->MR3 = T_ON;
		PWMX->LER |= 1 << 3;
	}else if(mr_number==4){
		PWMX->MR4 = T_ON;
		PWMX->LER |= 1 << 4;
	}else if(mr_number==5){
		PWMX->MR5 = T_ON;
		PWMX->LER |= 1 << 5;
	}else if(mr_number==6){
		PWMX->MR6 = T_ON;
		PWMX->LER |= 1 << 6;
	}
}
