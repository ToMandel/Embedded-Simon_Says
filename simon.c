/***********************************************************************************
            
			PIC32 STARTER KIT Simon Says Game Program

 ***********************************************************************************
 * FileName:        simon.c
 * Dependencies:    None
 * Processor:       PIC32
 *
 * Complier:        MPLAB C Compiler for PIC32 v1.04 or higher
 *                  MPLAB IDE v8.20 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Copyright (c) 2008 Microchip Technology, Inc.
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PIC32 Microcontroller is intended
 * and supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PIC32 Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ***************************************************************************
 * Platforms:
 *			PIC32MX Starter Kit DM320001
 *			PIC32MX USB Starter Kit DM320003
 *
 * How To Use:
 *			Refer to the PIC32MX Starter Kit Users Guide, DS61144B or later.
 *
 * Tools:
 *			1. MPLAB IDE 8.20 or higher
 *			2. MPLAB C Compiler for PIC32 v1.04 or higher
 *			3. General Purpose Starter Kit DM320001 or USB Starter board DM320003
 *			4. USB Cable
 *
 ****************************************************************************
 * Resources:
 *			LED1 (RED)		= PORTD.RD0
 *			LED2 (YELLOW)	= PORTD.RD1
 *			LED3 (GREEN)	= PORTD.RD2
 *			SW1				= PORTD.RD6  (change notice pin CN15)
 *			SW2				= PORTD.RD7	 (change notice pin CN16)
 *			SW3				= PORTD.RD13 (change notice pin CN19)
 *			JTAG.TMS		= PORTA.RA0
 *			JTAG.TCK		= PORTA.RA1
 *			JTAG.TDO		= PORTA.RA5
 *			JTAG.TDI		= PORTA.RA4
 *			PGC2			= PORTB.RB6	 (used by debugger)
 *			PGD2			= PORTB.RB7  (used by debugger)
 *
 * Notes:
 *			1.	There are no pullup resistors on SW1, SW2 or SW3.
 *				To enable the PORTD pullups for SW1, SW2 and SW3:
 *				CNPUE = (CN15_PULLUP_ENABLE | CN16_PULLUP_ENABLE | CN19_PULLUP_ENABLE);
 *			2.  Do not disable PIC32MX JTAG.  This will prevent the PIC32MX Starter Kit
 *				debugger(PIC18F4550) from communicating with the PIC32MX device.
 *			3.  Do not configure the SYSTEM CLOCK to operate faster than 80MHz.
 *****************************************************************************
 * Change History:
 * Name         Date            Changes
 * C11842       10/13/2008      Modified for 80MHz operation, Updated comments
 ***********************************************************************************/
/* PIC32 Peripheral library functions and macros */
#include <plib.h>					        // Peripheral Library's

#include "simon_says.h"
#include "simon.h"

/* Configuration Bits */
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (PIC32 Starter Kit: use divide by 2 only)
#pragma config FPLLMUL  = MUL_20        // PLL Multiplier
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer 
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = XT            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = OFF           // Debugger Disabled for Starter Kit
            


/*  The following is used by the main application */
#define SYS_FREQ		                    (80000000)
#define DEBOUNCE_TIME			            ((SYS_FREQ / 2) /5)		// 100ms

/* Private helper functions */
void Initialize(void);
void DebounceSwitches(void);
void PressSwitches(void);

/* application defines */
UINT	time = 0;
SIMMON_SWITCHES	debounce[3];
SIMMON_SWITCHES	switches;
SIMMON_SWITCHES	press;
SIMMON_SWITCHES	past;
BYTE debounceIdx;

/*  main demonstration application code */
int main(void)
{
	/* refer to ReleaseNotes.htm for details about SYSTEMConfig*/
   	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

	Initialize();
	
	SIMONSAYSInit(SYS_FREQ);
	
	while (1)
	{
		SIMONSAYSTask();
		DebounceSwitches();
		PressSwitches();
	}

	return 0;
}

/******************************************************************************
*	Initialize()
*
*	This function initializes the LEDS and enables weak pullups.
******************************************************************************/
// Initialize routine
void Initialize(void)
{
	// Turn All Led's Off
	mSetAllLedsOFF();

	// Set LED Pins to Outputs
	PORTSetPinsDigitalOut(IOPORT_D, BIT_0 | BIT_1 | BIT_2);

	// Set Switch Pins to Inputs
	PORTSetPinsDigitalIn(IOPORT_D, BIT_6 | BIT_7 | BIT_13);

	// Enable Pull-ups
	CNPUE = 0x98000;

	switches.val = 0;
	past.val = 0;
	press.val = 0;

	{
		int i;

		for(i = 0; i < 3; i++)
			debounce[i].val = 0;

		debounceIdx = 0;
	}

}

/******************************************************************************
*	DebounceSwitches()
*
*	This function debounces the switches.
******************************************************************************/
void DebounceSwitches(void)
{
	UINT curr_time;
	UINT delta;
	UINT i;

	curr_time = ReadCoreTimer();

	if(curr_time >= time)
		delta = curr_time - time;
	else
		delta = (0xFFFFFFFF - curr_time) + time;

	if(delta < DEBOUNCE_TIME)
		return;

	debounce[debounceIdx].redSwitch = mGetRawRedSwitch();
	debounce[debounceIdx].yellowSwitch = mGetRawYellowSwitch();
	debounce[debounceIdx].greenSwitch =  mGetRawGreenSwitch();

	debounceIdx++;

	if(debounceIdx >= 3)
		debounceIdx = 0;

	switches.val = 7;

	for(i = 0; i < 3; i++)
	{
		switches.redSwitch &= debounce[i].redSwitch;
		switches.yellowSwitch &= debounce[i].yellowSwitch;
		switches.greenSwitch &= debounce[i].greenSwitch;
	}

}

/******************************************************************************
*	PressSwitches()
*
*	This function determines the state of the switches.
******************************************************************************/
void PressSwitches(void)
{
	if((past.redSwitch) && (!switches.redSwitch))
		press.redSwitch = 1;
	else
		press.redSwitch = 0;
			
	if((past.yellowSwitch) && (!switches.yellowSwitch))
		press.yellowSwitch = 1;
	else
		press.yellowSwitch = 0;
			
	if((past.greenSwitch) && (!switches.greenSwitch))
		press.greenSwitch = 1;
	else
		press.greenSwitch = 0;

	past.redSwitch = switches.redSwitch;
	past.yellowSwitch = switches.yellowSwitch;
	past.greenSwitch = switches.greenSwitch;

}
