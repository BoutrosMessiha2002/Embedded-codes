/*
 * buzzer.c
 *
 *  Created on: Nov 4, 2023
 *      Author: Boutros
 */
#include"buzzer.h"
#include"gpio.h"
/*
 * Driver description: it initializes the buzzer and configures its pins but also has an ON and OFF functions.
 */
void BUZZER_init(void)
{
	GPIO_setupPinDirection(PORTD_ID,PIN2_ID,PIN_OUTPUT);
	GPIO_writePin(PORTD_ID,PIN2_ID,LOGIC_LOW);
}
void BUZZER_on(void)
{
	GPIO_writePin(PORTD_ID,PIN2_ID,LOGIC_HIGH);
}
void BUZZER_off(void)
{
	GPIO_writePin(PORTD_ID,PIN2_ID,LOGIC_LOW);
}
