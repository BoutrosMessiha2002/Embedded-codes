/*
 * timer1.c
 *
 *  Created on: Nov 3, 2023
 *      Author: Boutros
 */
#include"timer1.h"
#include<avr/io.h>
#include<avr/interrupt.h>
/*
 * Description of driver:
 * this driver configures timer1 dynamically,by configuration structure and callback technique.
 */
static void (*g_callBackPtr)(void) = NULL_PTR;
void TIMER1_init(const Timer1_ConfigType * Config_Ptr)
{
	TCCR1A|=(1<<FOC1A);
	TCCR1B=(TCCR1B&0xE7)|((Config_Ptr->mode)<<3);
	TCCR1B=(TCCR1B&0xF8)|(Config_Ptr->prescaler);
	if(Config_Ptr->mode==Normal)
	{
		TIMSK|=(1<<TOIE1);
		TCNT1=Config_Ptr->initial_value;
	}
	else if(Config_Ptr->mode==CTC)
	{
		TCNT1=Config_Ptr->initial_value;
		OCR1A=Config_Ptr->compare_value;
		TIMSK|=(1<<OCIE1A);
	}
}
void TIMER1_deInit(void)
{
	TCCR1A=0;
	TCCR1B=0;
	TIMSK=0;
}
void TIMER1_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}
ISR(TIMER1_COMPA_vect)
{
		if(g_callBackPtr != NULL_PTR)
		{
			(*g_callBackPtr)();
		}
}
ISR(TIMER1_OVF_vect)
{
		if(g_callBackPtr != NULL_PTR)
		{
			(*g_callBackPtr)();
		}
}


