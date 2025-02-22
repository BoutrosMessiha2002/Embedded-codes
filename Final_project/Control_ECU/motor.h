/*
 * motor.h
 *
 *  Created on: Oct 10, 2023
 *      Author: Boutros
 */

#ifndef MOTOR_H_
#define MOTOR_H_
typedef enum{
	OFF,ACW,CW
}DcMotor_State;

void DcMotor_init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* MOTOR_H_ */
