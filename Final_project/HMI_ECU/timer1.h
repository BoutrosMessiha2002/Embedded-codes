/*
 * timer1.h
 *
 *  Created on: Nov 3, 2023
 *      Author: Boutros
 */

#ifndef TIMER1_H_
#define TIMER1_H_
#include"std_types.h"
typedef enum{
NO_PS=1,PS_8,PS_64,PS_256,PS_1024
}Timer1_Prescaler;
typedef enum{
Normal,CTC=1
}Timer1_Mode;

typedef struct {
 uint16 initial_value;
 uint16 compare_value; // it will be used in compare mode only.
 Timer1_Prescaler prescaler;
 Timer1_Mode mode;
} Timer1_ConfigType;

void TIMER1_init(const Timer1_ConfigType * Config_Ptr);
void TIMER1_deInit(void);
void TIMER1_setCallBack(void(*a_ptr)(void));



#endif /* TIMER1_H_ */
