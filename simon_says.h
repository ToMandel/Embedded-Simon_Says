/***********************************************************************************
            PIC32 STARTER KIT Simon Says Game Program
***********************************************************************************
 * FileName:        simon.h
 * Dependencies:    generic.h, simon.h
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

#ifndef _SIMON_SAYS_HEADER_FILE
#define _SIMON_SAYS_HEADER_FILE
#include <peripheral/generic.h>
#include "simon.h"

#define SIMON_SAYS_EASY_LEVEL_SEQ       10
#define SIMON_SAYS_MID_LEVEL_SEQ        20
#define SIMON_SAYS_HARD_LEVEL_SEQ       30

void SIMONSAYSInit(UINT sys_clock);
void SIMONSAYSTask(void);

#endif
