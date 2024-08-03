/*
 * motor.c
 *
 *  Created on: Oct 10, 2023
 *      Author: Boutros
 */

#include"gpio.h"
#include"motor.h"
#include"pwm.h"
#include<avr/io.h>
void DcMotor_Rotate(DcMotor_State state,uint8 speed);
/*this file is responsible in configuring motor pins,speed and state, whether clockwise
 * anti-clockwise or stopped
 */
void DcMotor_init(void)
{
	GPIO_setupPinDirection(PORTB_ID,PIN0_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID,PIN1_ID,PIN_OUTPUT);
	GPIO_writePin(PORTB_ID,PIN0_ID,LOGIC_LOW);
	GPIO_writePin(PORTB_ID,PIN1_ID,LOGIC_LOW);
}
void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	PORTB=(PORTB&0xFC)|(state);
	PWM_Timer0_Start(speed);
}

