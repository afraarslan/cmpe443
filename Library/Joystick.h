#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "LPC407x_8x_177x_8x.h"

#include "GPIO.h"

#define JOYSTICK_PORT	PORT5

#define JOYSTICK_LEFT_MASK		((uint32_t) 1 << 0)
#define JOYSTICK_DOWN_MASK		((uint32_t) 1 << 1)
#define JOYSTICK_UP_MASK			((uint32_t) 1 << 2)
#define JOYSTICK_CENTER_MASK	((uint32_t) 1 << 3)
#define JOYSTICK_RIGHT_MASK		((uint32_t) 1 << 4)

#define IOCON_JOYSTICK_LEFT_ADDRESS			0x4002C280
#define IOCON_JOYSTICK_DOWN_ADDRESS			0x4002C284
#define IOCON_JOYSTICK_UP_ADDRESS				0x4002C288
#define IOCON_JOYSTICK_CENTER_ADDRESS		0x4002C28C
#define IOCON_JOYSTICK_RIGHT_ADDRESS		0x4002C290

#define IOCON_JOYSTICK_LEFT		*((volatile uint32_t*)(IOCON_JOYSTICK_LEFT_ADDRESS))
#define IOCON_JOYSTICK_DOWN		*((volatile uint32_t*)(IOCON_JOYSTICK_DOWN_ADDRESS))
#define IOCON_JOYSTICK_UP			*((volatile uint32_t*)(IOCON_JOYSTICK_UP_ADDRESS))
#define IOCON_JOYSTICK_CENTER	*((volatile uint32_t*)(IOCON_JOYSTICK_CENTER_ADDRESS))
#define IOCON_JOYSTICK_RIGHT	*((volatile uint32_t*)(IOCON_JOYSTICK_RIGHT_ADDRESS))

void Joystick_Init(void);

uint8_t Joystick_Left_Pressed(void);
uint8_t Joystick_Down_Pressed(void);
uint8_t Joystick_Up_Pressed(void);
uint8_t Joystick_Center_Pressed(void);
uint8_t Joystick_Right_Pressed(void);

#endif
