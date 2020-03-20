#include "GPIO.h"

void GPIO_DIR_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value) {
	if(value == 0) {
		PORT->DIR &= ~MASK;
	}
	else {
		PORT->DIR |= MASK;
	}
}

void GPIO_PIN_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value) {
	if(value == 0) {
		PORT->PIN &= ~MASK;
	}
	else {
		PORT->PIN |= MASK;
	}
}

uint32_t GPIO_PIN_Read(GPIO_TypeDef* PORT,uint32_t MASK) {
	return PORT->PIN &= MASK;
}

void Motor_Init(){
	MotorA_Init();
	MotorB_Init();
}
void MotorA_Init(){
	PCONP |= (1<<15);
	IOCON_MOTORA_IN1 &= ~(1<<0|1<<1|1<<2);
	IOCON_MOTORA_IN2 &= ~(1<<0|1<<1|1<<2);
	PORT0->DIR |=(1<<26 | 1<<21);
}

void MotorB_Init(){
	PCONP |= (1<<15);
	IOCON_MOTORB_IN1 &= ~(1<<0|1<<1|1<<2);
	IOCON_MOTORB_IN2 &= ~(1<<0|1<<1|1<<2);
	PORT0->DIR |=((1<<9) | (1<<8));
}

void MotorA_Clockwise(){
	uint32_t pin_value = PORT0->PIN;
	pin_value |= (1<<21);
	pin_value &= ~(1<<26);
	PORT0->PIN = pin_value;
}

void MotorA_CounterClockwise(){
	uint32_t pin_value = PORT0->PIN;
	pin_value |= (1<<26);
	pin_value &= ~(1<<21);
	PORT0->PIN = pin_value;
}

void MotorB_Clockwise(){
	uint32_t pin_value = PORT0->PIN;
	pin_value |= (1<<9);
	pin_value &= ~(1<<8);
	PORT0->PIN = pin_value;
}

void MotorB_CounterClockwise(){
	uint32_t pin_value = PORT0->PIN;
	pin_value |= (1<<8);
	pin_value &= ~(1<<9);
	PORT0->PIN = pin_value;
}

void Motor_Forward(){
	MotorA_CounterClockwise();
	MotorB_CounterClockwise();
}

void Motor_Backward(){
	MotorA_Clockwise();
	MotorB_Clockwise();
}

void Motor_Right(){
	MotorA_CounterClockwise();
	MotorB_Clockwise();
}

void Motor_Left(){
	MotorA_Clockwise();
	MotorB_CounterClockwise();
}