#ifndef GPIO_H
#define GPIO_H

#include "LPC407x_8x_177x_8x.h"
#include "SystemStructures.h"

//Correctly define GPIO_TypeDef struct.
typedef struct
{
  volatile	uint32_t DIR;
						uint32_t RESERVED0[3];
  volatile	uint32_t MASK;
  volatile	uint32_t PIN;
  volatile	uint32_t SET;
  volatile  uint32_t CLR;
} GPIO_TypeDef;

typedef enum {
	INPUT = 0,
	OUTPUT = 1
} GPIO_Direction;

typedef enum {
	LOW = 0,
	HIGH = 1
} GPIO_Value;


#define GPIO_ADDRESS	0x20098000

#define PORT0	((GPIO_TypeDef*) PORT0_BASE)
#define PORT1	((GPIO_TypeDef*) PORT1_BASE)
#define PORT2	((GPIO_TypeDef*) PORT2_BASE)
#define PORT3	((GPIO_TypeDef*) PORT3_BASE)
#define PORT4	((GPIO_TypeDef*) PORT4_BASE)
#define PORT5	((GPIO_TypeDef*) PORT5_BASE)

#define PORT0_BASE		(GPIO_ADDRESS + 0x000)
#define PORT1_BASE		(GPIO_ADDRESS + 0x020)
#define PORT2_BASE		(GPIO_ADDRESS + 0x040)
#define PORT3_BASE		(GPIO_ADDRESS + 0x060)
#define PORT4_BASE		(GPIO_ADDRESS + 0x080)
#define PORT5_BASE		(GPIO_ADDRESS + 0x0A0)

#define IOCON_MOTORA_IN1_ADDRESS	0x4002C054
#define IOCON_MOTORA_IN1	*((volatile uint32_t*)(IOCON_MOTORA_IN1_ADDRESS))
	
#define IOCON_MOTORA_IN2_ADDRESS  0x4002C068
#define IOCON_MOTORA_IN2	*((volatile uint32_t*)(IOCON_MOTORA_IN2_ADDRESS))
	
#define IOCON_MOTORB_IN1_ADDRESS	0x4002C024
#define IOCON_MOTORB_IN1	*((volatile uint32_t*)(IOCON_MOTORB_IN1_ADDRESS))
	
#define IOCON_MOTORB_IN2_ADDRESS	0x4002C020
#define IOCON_MOTORB_IN2	*((volatile uint32_t*)(IOCON_MOTORB_IN2_ADDRESS))

void GPIO_DIR_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value);
void GPIO_PIN_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value);
uint32_t GPIO_PIN_Read(GPIO_TypeDef* PORT,uint32_t MASK);
void MotorA_Clockwise();
void MotorA_CounterClockwise();
void MotorA_Init();
void MotorB_Clockwise();
void MotorB_CounterClockwise();
void MotorB_Init();
void Motor_Init();
void Motor_Forward();
void Motor_Backward();
void Motor_Right();
void Motor_Left();

#endif
