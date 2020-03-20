#include "LPC407x_8x_177x_8x.h"
#include "Library/GPIO.h"
#include "Library/PWM.h"
#include "Library/External.h"
#include "Library/LED.h"
#include "Library/HM10.h"
#include "Library/Timer.h"
#include "Library/ADC.h"
#include <stdio.h> 
#include <string.h>

char characters[256] = ""; 
uint8_t mode = 0 ;
uint8_t stop = 0 ;
uint32_t speed = 0;
uint8_t started = 0;
uint8_t finished = 0;

void init() {
	TimerInit();
	ADC_Init();
	External_Init();
  __enable_irq();
	PWM_Init();
	HM10_Init();
	Motor_Init();
	PWM_Write(100, 5, PWM1);
	PWM_Write(100, 4, PWM1);
	HM10NewDataAvailable = 1;
	HM10_ClearBuffer();
	HM10_SendCommand("AT\r\n");
	HM10NewDataAvailable = 1;
	PWM_Cycle_Rate(10,PWM1);
}

/*
		This function takes 5 parameters. 4 of them is used for LEDs and last one for Motor.
		It sets duty cycle of PWM.
*/

void set_pwm_rates(uint32_t left1_rate, uint32_t left2_rate, uint32_t right1_rate,uint32_t right2_rate,uint32_t motor_rate){
		PWM_Write(left1_rate, 1, PWM0);
		PWM_Write(left2_rate, 2, PWM0);
		PWM_Write(right1_rate, 3, PWM0);
		PWM_Write(right2_rate, 4, PWM0);
		PWM_Write(motor_rate, 5, PWM1);
		PWM_Write(motor_rate, 4, PWM1);
}

/*
		This function takes 1 parameter and sets duty cycle of motor.
*/

void motor_pwm_write(uint32_t rate){
	PWM_Write(rate, 5, PWM1);
	PWM_Write(rate, 4, PWM1);
}

void update() {
	if(HM10NewDataAvailable && HM10Buffer[HM10CurrentBufferIndex-1]==10){
		strcpy(characters,HM10Buffer);
		HM10_SendCommand(characters);
		HM10_ClearBuffer();
	}
	if(strcmp(characters,"TEST\r\n")==0)
		mode = 0;
	if(strcmp(characters,"AUTO\r\n")==0){
		mode = 1;
		stop = 0;
		finished = 0;
		set_pwm_rates(0,0,0,0,0);
		speed = 0;
	}
	if(!stop){
		if(ADC_LDR_Last<2700){
			speed=0;
			motor_pwm_write(0);
			stop=1;
			started = 0;
			finished = 1;
		}	else {
				if(ADC_Pm_Last < 300){
				speed=0;
				motor_pwm_write(0);
			} else if (ADC_Pm_Last < 600){
				speed=10;
				motor_pwm_write(10);
			} else if (ADC_Pm_Last < 2500){
				speed=30;
				motor_pwm_write(30);
			}else if (ADC_Pm_Last < 3500){
				speed=50;
				motor_pwm_write(50);
			} else if (ADC_Pm_Last < 3950){
				speed=60;
				motor_pwm_write(60);
			} else if (ADC_Pm_Last < 4050){
				speed=85;
				motor_pwm_write(85);
			} else{
				speed=100;
				motor_pwm_write(100);
			}
		}
	}
	if(mode==0){
		started = 0;
		if(strcmp(characters,"STATUS\r\n")==0){
			char buffer[200]; 
			uint32_t distance= calculateUSDistance();
			sprintf(buffer, "{\"distance\":%d,\"light_level_left\":%d,\"light_level_right\":%d,\"op_mode\":\"AUTO\"}\r\n", distance, LDR1_value, LDR2_value); 
			HM10_SendCommand(buffer);
		}
		if(strcmp(characters,"FORWARD\r\n")==0){
			Motor_Forward();
			PWM_Cycle_Rate(1,PWM0);
			set_pwm_rates(100,0,100,0,speed);
			stop=0;
		}
		if(strcmp(characters,"BACK\r\n")==0){
			Motor_Backward();
			PWM_Cycle_Rate(1,PWM0);
			set_pwm_rates(0,100,0,100,speed);
			stop=0;
		}
		if(strcmp(characters,"LEFT\r\n")==0){
			Motor_Left();
			uint32_t local_rotation = rotations;
			PWM_Cycle_Rate(1000,PWM0);
			set_pwm_rates(50,50,0,0,speed);
			while((rotations - local_rotation) < 2){}
			set_pwm_rates(0,0,0,0,0);
			stop=1;
		}
		if(strcmp(characters,"RIGHT\r\n")==0){
			Motor_Right();
			uint32_t local_rotation = rotations;
			PWM_Cycle_Rate(1000,PWM0);
			set_pwm_rates(0,0,50,50,speed);
			while((rotations - local_rotation) < 2){}
			set_pwm_rates(0,0,0,0,0);
			stop=1;
		}
		if(strcmp(characters,"STOP\r\n")==0){
			speed=0;
			set_pwm_rates(0,0,0,0,0);
			stop=1;
		}
	}else{
		if(strcmp(characters,"START\r\n")==0){
			started = 1;
			finished = 0;
		}
		if(finished){
			HM10_SendCommand("FINISH\r\n");
			finished = 0;
		}
		if(started){
			uint32_t distance;
			Motor_Forward();
			PWM_Cycle_Rate(1,PWM0);
			set_pwm_rates(100,0,100,0,speed);
			if(ultrasonicSensorNewDataAvailable){
					distance = calculateUSDistance();
					if(distance<10){
						uint32_t local_rotation = rotations;
						Motor_Right();
						PWM_Cycle_Rate(1000,PWM0);
						set_pwm_rates(0,0,50,50,speed);
						while((rotations - local_rotation) < 1){}
						Motor_Forward();
						PWM_Cycle_Rate(1,PWM0);
						set_pwm_rates(100,0,100,0,speed);
						local_rotation = rotations;
						while((rotations - local_rotation) < 3){}
					}
					else if(distance<35){
						Motor_Forward();
						PWM_Cycle_Rate(1,PWM0);
						set_pwm_rates(100,0,100,0,speed);
					}
					else{
						uint32_t local_rotation = rotations;
						Motor_Left();
						PWM_Cycle_Rate(1000,PWM0);
						set_pwm_rates(50,50,0,0,speed);
						while((rotations - local_rotation) < 1){}
						Motor_Forward();
						PWM_Cycle_Rate(1,PWM0);
						set_pwm_rates(100,0,100,0,speed);
						local_rotation = rotations;
						while((rotations - local_rotation) < 3){}
					}
			}	
		}		
		if(strcmp(characters,"STATUS\r\n")==0){
			char buffer[200]; 
			uint32_t distance= calculateUSDistance();
			sprintf(buffer, "{\"distance\":%d,\"light_level_left\":%d,\"light_level_right\":%d,\"op_mode\":\"AUTO\"}\r\n", distance, LDR1_value, LDR2_value); 
			HM10_SendCommand(buffer);
		}
		if(strcmp(characters,"STOP\r\n")==0){
			speed=0;
			set_pwm_rates(0,0,0,0,0);
			stop=1;
		}
	}
	memset(characters, 0, 255);	
}

 
int main() {
    init();
 
    while(1) {
        update();
    }
}
