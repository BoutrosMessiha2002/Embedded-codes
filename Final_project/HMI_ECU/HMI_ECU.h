/*
 * HMI_ECU.h
 *
 *  Created on: Nov 3, 2023
 *      Author: Boutros
 */

#ifndef HMI_ECU_H_
#define HMI_ECU_H_
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
void HMI_createPass(void);
void HMI_displayOpt(void);
uint8 HMI_readyToRecieve(void);
void HMI_readyToSend(uint8 sel);
void HMI_enterPassAfterOpt1(void);
void HMI_enterPassAfterOpt2(void);
void HMI_alertUser(void);
void HMI_openingDoor(void);
void HMI_countTime(void);
void HMI_alarm(void);


#endif /* HMI_ECU_H_ */
