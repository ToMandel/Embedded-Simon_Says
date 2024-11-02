/***********************************************************************************
            PIC32 STARTER KIT Simon Says Game Program
***********************************************************************************
 * FileName:        simon.h
 * Dependencies:    None
 * Processor:       PIC32
 *
 * Complier:        MPLAB C Compiler for PIC32 v1.04
 *                  MPLAB IDE v8.15 or higher
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
 *********************************************************************
 * Change History:
 * Name         Date            Changes
 * C11842       10/13/2008      Updated comments
 ********************************************************************/
#ifndef _SIMMON_HEADER_FILE
#define _SIMMON_HEADER_FILE

// LED Macros
#define mSetAllLedsON()			(mSetRedLedON(), mSetYellowLedON(), mSetGreenLedON())
#define mSetAllLedsOFF()		(mSetRedLedOFF(), mSetYellowLedOFF(), mSetGreenLedOFF())
#define mSetAllLedsINV()		(mSetRedLedINV(), mSetYellowLedINV(), mSetGreenLedINV())

#define mSetRedLedON()			mPORTDSetBits(BIT_0)
#define mSetRedLedOFF()			mPORTDClearBits(BIT_0)
#define mSetRedLedINV()			mPORTDToggleBits(BIT_0)

#define mSetYellowLedON()		mPORTDSetBits(BIT_1)
#define mSetYellowLedOFF()		mPORTDClearBits(BIT_1)
#define mSetYellowLedINV()		mPORTDToggleBits(BIT_1)

#define mSetGreenLedON()		mPORTDSetBits(BIT_2)
#define mSetGreenLedOFF()		mPORTDClearBits(BIT_2)
#define mSetGreenLedINV()		mPORTDToggleBits(BIT_2)

// Switch Macros
#define mGetRawRedSwitch()			(!mPORTDReadBits(BIT_6))
#define mGetRawYellowSwitch()		(!mPORTDReadBits(BIT_7))
#define mGetRawGreenSwitch()		(!mPORTDReadBits(BIT_13))


typedef union
{
	struct
	{
		BYTE	redSwitch:		1;
		BYTE	yellowSwitch:	1;
		BYTE	greenSwitch:	1;
	};
	BYTE val;
}SIMMON_SWITCHES;

extern SIMMON_SWITCHES	press;
extern SIMMON_SWITCHES	switches;

#define mGetRedSwitch()			(press.redSwitch)
#define mGetYellowSwitch()		(press.yellowSwitch)
#define mGetGreenSwitch()		(press.greenSwitch)
#endif
