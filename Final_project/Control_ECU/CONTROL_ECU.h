/*
 * CONTROL_ECU.h
 *
 *  Created on: Nov 3, 2023
 *      Author: Boutros
 */

/*
 * Header description:
 * it has the prototypes of the functions+ the bytes of each command
 */
#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_
#include"std_types.h"
#define CONTROL_READY 0x0A
#define HMI_READY 0x0B
#define CREATE_PASS 0x0C
#define CREATE_PASS2 0x0D
#define OPEN_DOOR_CHECK_PASS 0x0E
#define CHANGE_PASS 0x0F
#define POSITIVE 0x10
#define FAIL 0x11
#define CHECK_PASS 0x12
#define ALERT 0x13
#define OPENING_DOOR 0x14
#define ALARM 0x15
void CONTROL_readyToSend(uint8 status);
void CONTROL_openDoorCheckPass(void);
void CONTROL_changePass(void);
void CONTROL_Selection(void);
uint8 CONTROL_readyToRecieve(void);
void CONTROL_alertUser(void);
void CONTROL_createPass(void);
void CONTROL_createPass2(void);
void CONTROL_registerPass(uint8*pass1,uint8*pass2);
void CONTROL_checkPass(uint8*pass);
void CONTROL_countTime(void);
void CONTROL_doorOpening(void);
void CONTROL_alarm(void);

#endif /* CONTROL_ECU_H_ */
