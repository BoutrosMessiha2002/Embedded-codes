/*
 * HMI_ECU.c
 *
 *  Created on: Nov 3, 2023
 *      Author: Boutros
 */
//Final project: Door lock security system
//HMI_MCU description
#include"lcd.h"
#include"uart.h"
#include"keypad.h"
#include"HMI_ECU.h"
#include"timer1.h"
#include<avr/io.h>
#include<util/delay.h>
uint8 status;//status for match or mismatch of passwords
uint8 count=0;//count to count time
uint8 alert_count=0;//count to signal false password entry
uint8 trig;// this trigger variable signals whether we entered the alerting state from open door(0) or change
           //pass(1).
/*
 * Function description:
 * this function creates the password after startup, receiving first entry, then send it to the CONTROL MCU
 * through UART and then receives second entry which is confirmation, then send it also to CONTROL MCU through
 * UART. And then it receives the status whether they match or not, if matching then it displays:
 * + open door, - change pass
 * If not, then it restarts the function until the user enter both passwords correctly.
 */
void HMI_createPass()
{
	_delay_ms(300);
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("plz enter pass: ");
	LCD_moveCursor(1,0);
	HMI_readyToSend(CREATE_PASS);
	for(uint8 i=0;i<5;i++)
	{
		UART_sendByte(KEYPAD_getPressedKey());
		LCD_displayCharacter('*');
		_delay_ms(400);
	}
	while(KEYPAD_getPressedKey()!='=');
	_delay_ms(300);
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("plz re-enter the");
	LCD_moveCursor(1,0);
	LCD_displayString("same pass: ");
	HMI_readyToSend(CREATE_PASS2);
	for(uint8 i=0;i<5;i++)
		{
			UART_sendByte(KEYPAD_getPressedKey());
			LCD_displayCharacter('*');
			_delay_ms(400);
		}
	while(KEYPAD_getPressedKey()!='=');
	_delay_ms(300);
	LCD_clearScreen();
	status=HMI_readyToRecieve();
	if(status==POSITIVE)
		 HMI_displayOpt();
	else if(status==FAIL)
		 HMI_createPass();
}
/*
 * Function description:
 * it displays on LCD whether the user wants to open door or change password
 */
void HMI_displayOpt()
{
	_delay_ms(300);
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("+:Open door");
	LCD_moveCursor(1,0);
	LCD_displayString("-:Change pass");
	uint8 num=KEYPAD_getPressedKey();
	if(num=='+')
	 HMI_enterPassAfterOpt1();
	 else if(num=='-')
     HMI_enterPassAfterOpt2();
}
/*
 * Function description:
 * It requests the entry of the password already entered and confirmed after choosing to open door
 * to send it to the CONTROL MCU to check similarity between it and the one stored in EEPROM
 * If matched then it opens the door
 * If not, it enters an alert state
 */
void HMI_enterPassAfterOpt1()
{
	_delay_ms(300);
	LCD_clearScreen();
	LCD_moveCursor(0,0);
		LCD_displayString("plz enter pass: ");
		LCD_moveCursor(1,0);
		HMI_readyToSend(OPEN_DOOR_CHECK_PASS);
			for(uint8 i=0;i<5;i++)
			{
				UART_sendByte(KEYPAD_getPressedKey());
				LCD_displayCharacter('*');
				_delay_ms(400);

			}
		while(KEYPAD_getPressedKey()!='=');
		LCD_clearScreen();
		status=HMI_readyToRecieve();
		if(status==POSITIVE)
		HMI_openingDoor();
		else if(status==FAIL)
		{
		alert_count++;
		trig=0;
		HMI_alertUser();
		}
}
/*
 * Function description:
 * Same function described in CONTROL MCU, the HMI sends a byte to signal that it is ready to send the
 * required selected operation to be executed by the CONTROL MCU
 */
void HMI_readyToSend(uint8 sel)
{
	UART_sendByte(HMI_READY);
	while(UART_recieveByte()!=CONTROL_READY);
	UART_sendByte(sel);
}
/*
 * Function description:
 * Same function described in CONTROL MCU, the HMI enters a polling state waiting for the byte of CONTROL
 * READY and after that it sends HMI READY to signals the polling CONTROL MCU that it is ready to receive
 * the required byte, in this file it is the status that describes the match or the mismatch of
 * the passwords.
 */
uint8 HMI_readyToRecieve(void)
{
	while(UART_recieveByte()!=CONTROL_READY);
	UART_sendByte(HMI_READY);
	return UART_recieveByte();
}
/*
 * Function description:
 * It requests the entry of the password already entered and confirmed after choosing to open door
 * to send it to the CONTROL MCU to check similarity between it and the one stored in EEPROM
 * If matched then it changes the password to the one the user desires
 * If not, it enters an alert state
 */
void HMI_enterPassAfterOpt2()
{
	_delay_ms(300);
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("plz enter pass: ");
	LCD_moveCursor(1,0);
	HMI_readyToSend(CHANGE_PASS);
	for(uint8 i=0;i<5;i++)
				{
					UART_sendByte(KEYPAD_getPressedKey());
					LCD_displayCharacter('*');
					_delay_ms(400);
				}
			while(KEYPAD_getPressedKey()!='=');
	LCD_clearScreen();
			status=HMI_readyToRecieve();
			if(status==POSITIVE)
				HMI_createPass();
			else if(status==FAIL)
			{
				trig=1;
				alert_count++;
				HMI_alertUser();
			}
}
/*
 * Function description:
 * This function is accessed through the mismatch of passwords from open door function or change pass function
 * and it has an alert count that increments each time the user enters an incorrect passwords and displays
 * on LCD the remaining trials of entering passwords, if the user enters a correct password, it then proceeds
 * to open door or change password based on the value of the variable trig described above in this file.
 * If the user enters 3 incorrect passwords, it then goes to the ALARM state.
 */
void HMI_alertUser(void)
{
	_delay_ms(300);
	if(alert_count==3)
	{
		alert_count=0;
		HMI_alarm();
	}
		LCD_clearScreen();
		LCD_moveCursor(0,0);
		LCD_displayString("plz enter pass: ");
		LCD_moveCursor(1,0);
		LCD_displayString("remaining:");
		LCD_intgerToString(3-alert_count);
		LCD_moveCursor(2,0);
		HMI_readyToSend(ALERT);
		for(uint8 i=0;i<5;i++)
						{
							UART_sendByte(KEYPAD_getPressedKey());
							LCD_displayCharacter('*');
							_delay_ms(400);
						}
					while(KEYPAD_getPressedKey()!='=');
			LCD_clearScreen();
					status=HMI_readyToRecieve();
					if(status==POSITIVE&&trig==0)
					{
                        alert_count=0;
						HMI_openingDoor();
					}
					else if(status==POSITIVE&&trig==1)
					{
						alert_count=0;
						HMI_enterPassAfterOpt2();
					}
					else if(status==FAIL)
					{
						alert_count++;
						HMI_alertUser();
					}
}
/*
 * Function description:
 * It is responsible to open the door after checking the matching of the passwords in the
 * HMI_enterPassAfterOpt1() function. It displays door is opening for 15 seconds, holds for 3 seconds,
 * then displays door is locking for 15 seconds.
 * NOTICE THAT: Both MCUs do the same operation differently but with the same timer because the HMI MCU sends
 * the OPENING DOOR command to the CONTROL MCU to signal it to control the motor to open and close the door.
 * Same timer1 driver implemented in CONTROL MCU using dynamic configuration and callback technique, it is
 * disabled after successfully opening the door and re-initialized if opening the door is requested by
 * the user.
 */
void HMI_openingDoor(void)
{
	HMI_readyToSend(OPENING_DOOR);
	Timer1_ConfigType TIMER1_Config={0,31250,PS_256,CTC};
	TIMER1_init(&TIMER1_Config);
	TIMER1_setCallBack(HMI_countTime);
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	while(count!=15)
	{
		LCD_displayString("Door is opening");
		LCD_moveCursor(0,0);
	}
	count=0;
	while(count!=3);
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	count=0;
	while(count!=15)
	{
		LCD_displayString("Door is Locking");
		LCD_moveCursor(0,0);
	}
	LCD_clearScreen();
	TIMER1_deInit();
	count=0;
	HMI_displayOpt();
}
/*
 * Function description:
 * It is the alarm state, where it displays ERROR on LCD, stops from taking and inputs from keypad,
 * and signals the CONTROL MCU to begin activating the buzzer for 1min while displaying the error message,
 * to signal that the passwords have been incorrectly entered.
 */
void HMI_alarm(void)
{
	HMI_readyToSend(ALARM);
	Timer1_ConfigType TIMER1_Config={0,31250,PS_256,CTC};
		TIMER1_init(&TIMER1_Config);
		TIMER1_setCallBack(HMI_countTime);
		LCD_clearScreen();
		LCD_moveCursor(0,0);
		while(count!=60)
		{
			LCD_displayString("ERROR");
			LCD_moveCursor(0,0);
		}
		count=0;
		TIMER1_deInit();
		HMI_displayOpt();
}
/*
 * Function description:
 * Function that counts time, configured by the call back technique.
 */
void HMI_countTime(void)
{
	count++;
}
int main()
{
	SREG|=(1<<7);//enabling global interrupt, I-bit
	UART_ConfigType UART_Config={EIGHT_BIT,DISABLED,ONE_BIT,BR_9600};//Configuration of UART with 9600 baud rate
	UART_init(&UART_Config);
	LCD_init();//initialize lcd
	HMI_createPass();//Function called only one time because the programs loops around different function after that.
	while(1)
	{
//empty
	}
}

