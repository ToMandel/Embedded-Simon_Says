/***********************************************************************************
            
			PIC32 STARTER KIT Simon Says Game Program

 ***********************************************************************************
 * FileName:        simon_says.c
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
 *************************************************************************************/
/* PIC32 Peripheral library functions and macros */
#include <plib.h>
#include "simon_says.h"

/* application typedefs */
typedef enum 
{
    SIMON_SAYS_START,
    SIMON_SAYS_LEVEL_SEL,
	SIMON_SAYS_START_PLAY,
	SIMON_SAYS_EVAL,
    SIMON_SAYS_PLAY,
    SIMON_SAYS_WIN,
    SIMON_SAYS_LOSER
}SIMON_SAYS_GAME_STATE;

enum
{
    EASY,
    MEDIUM,
    HARD
};

typedef struct
{
    union
    {
        struct
        {
            BYTE    red:    1;
            BYTE    yellow: 1;
            BYTE    green:  1;
            BYTE    spare:  5;
        };
        BYTE val;
    };
    UINT    period;
}SIMON_SAYS_SEQ;

PRIVATE SIMON_SAYS_GAME_STATE   _gameState;
PRIVATE UINT                    _clock_rate; 
PRIVATE UINT                    _period;
PRIVATE SIMON_SAYS_SEQ          _gameSequence[SIMON_SAYS_HARD_LEVEL_SEQ + 1];
PRIVATE BYTE                    _gameLevel;
PRIVATE UINT                    _seqIdx;
PRIVATE UINT                    _seqStop;
PRIVATE BOOL					_seqOn;

// APIs
void _MakeGameSequence(UINT level);
BYTE _GetLedSeq(UINT level, UINT led);

/******************************************************************************
*	SIMONSAYSInit()
*
*	This function initializes the game state, core timer and enables interrupts.
******************************************************************************/
void SIMONSAYSInit(UINT sys_clock)
{
    UINT i;

    _gameState = SIMON_SAYS_START;
    _clock_rate = sys_clock >> 1;

    for(i = 0; i <= SIMON_SAYS_HARD_LEVEL_SEQ; i++)
        _gameSequence[i].val = 0;

    mConfigIntCoreTimer(CT_INT_OFF | CT_INT_PRIOR_3 | CT_INT_SUB_PRIOR_2);

    // Enable System interrupts
    INTEnableSystemMultiVectoredInt();

}

/******************************************************************************
*	SIMONSAYSTask()
*
*	This function handles the tasks based on the game state.
******************************************************************************/
void SIMONSAYSTask(void)
{
    switch(_gameState)
    {
    case SIMON_SAYS_START:
        _period = _clock_rate;
        OpenCoreTimer(_period);
        mSetAllLedsON();
        _gameState = SIMON_SAYS_LEVEL_SEL;
        mEnableIntCoreTimer();
        break;

    case SIMON_SAYS_LEVEL_SEL:
        
        if(mGetRedSwitch())
            _MakeGameSequence(HARD);
        else if(mGetYellowSwitch())
            _MakeGameSequence(MEDIUM);
        else if(mGetGreenSwitch())
            _MakeGameSequence(EASY);
        else
            return;

        _seqIdx = 0;
        _period = _clock_rate << 1;
        _gameState = SIMON_SAYS_START_PLAY;
        break;

    case SIMON_SAYS_EVAL:
        if(_seqIdx > _seqStop)
        {
            _seqStop++;
			_seqIdx = 0;
            UpdateCoreTimer(_period);
            _gameState = SIMON_SAYS_PLAY;
            return;
        }

	    if(switches.redSwitch)
	    {
	        mSetRedLedON();
	    }else
	    {
	        mSetRedLedOFF();
	    }
	
	    if(switches.yellowSwitch)
	    {
	        mSetYellowLedON();
	    }else
	    {
	        mSetYellowLedOFF();
	    }
	    if(switches.greenSwitch)
	    {
	        mSetGreenLedON();
	    }else
	    {
	        mSetGreenLedOFF();
	    }

        {
            SIMON_SAYS_SEQ  input;

            input.val = 0;

            if(mGetRedSwitch())
                input.red = 1;

            if(mGetYellowSwitch())
                input.yellow = 1;

            if(mGetGreenSwitch())
                input.green = 1;

            if(input.val)
			{
				if(input.val == _gameSequence[_seqIdx].val)
            	{
                	UpdateCoreTimer(_gameSequence[_seqIdx].period);
                	_seqIdx++;
            	}else
				{
			        _gameState = SIMON_SAYS_LOSER;
			        UpdateCoreTimer(_period << 2);
			        mSetAllLedsON();
				}

			}                

        }
        break;

    }

}

/******************************************************************************
*	_MakeGameSequence()
*
*	This function creates the game sequence.
******************************************************************************/
void _MakeGameSequence(UINT level)
{
    UINT    seq;
    UINT    idx;
    UINT    period;
    UINT    number;

    for(idx = 0; idx <= SIMON_SAYS_HARD_LEVEL_SEQ; idx++)
        _gameSequence[idx].val = 0;

    switch(level)
    {
	    default:
	    case EASY:
	        number = SIMON_SAYS_EASY_LEVEL_SEQ;
	        break;
	
	    case MEDIUM:
	        number = SIMON_SAYS_MID_LEVEL_SEQ;
	        break;
	
	    case HARD:
	        number = SIMON_SAYS_HARD_LEVEL_SEQ;
	        break;
    }        

    idx = 0;
    period = _clock_rate * 3;
    period >>= 1;
    seq = 0;

    while(idx < number)
    {
        BYTE led;
        
        if((idx & 0x0003) == 3)
        {
            period -= (_clock_rate >> 3);
        }

        if(!seq)
            seq = ReadCoreTimer();

        
        led = _GetLedSeq(level, seq);

        seq >>= 3;

        if(led == 0xFF)
            continue;

            
        _gameSequence[idx].val = led;
        _gameSequence[idx++].period = period;

    }

    _gameSequence[idx].val      = 0;
    _gameSequence[idx].period   = 0;

}

/******************************************************************************
*	_GetLedSeq()
*
*	This function determines the led sequence pattern.
******************************************************************************/
BYTE _GetLedSeq(UINT level, UINT led)
{
    
    led &= 7;

    switch(level)
    {
	    default:
	    case EASY:
	        switch(led)
	        {
		        case 1:
		        case 2:
		        case 4:
		            break;
		
		        case 7:
		        case 0:
		            return 0xFF;
		
		        default:
		            led ^= 0x07;
		            break;
	        }
	        break;
    
        case MEDIUM:
            if((!led) || (led == 7))
                return 0xFF;
            break;

        case HARD:
            break;
    }

    return (BYTE)led;
}

/******************************************************************************
*	SIMONSAYSTimerInt()
*
*	This function is the core timer interrupt handler.
******************************************************************************/
void __ISR(_CORE_TIMER_VECTOR, ipl3) SIMONSAYSTimerInt(void)
{
    mCTClearIntFlag();
   
    switch(_gameState)
    {
	    case SIMON_SAYS_LEVEL_SEL:
	        mSetAllLedsINV();
	        UpdateCoreTimer(_period);
	        break;  
	        
	    case SIMON_SAYS_START_PLAY:
	        mSetAllLedsON();
	        UpdateCoreTimer(_period);
	        _gameState = SIMON_SAYS_PLAY;
	        _seqIdx = 0;
	        _seqStop = 0;
			_seqOn = TRUE;
	        _period >>= 4;
	        break;

	    case SIMON_SAYS_PLAY:
	        if(!_gameSequence[_seqStop].period)
	        {
	            _gameState = SIMON_SAYS_WIN;
	            UpdateCoreTimer(_period);
	            _gameSequence[0].red = 1;
	            _seqIdx = 0;
	            return;
	        }
        
			mSetAllLedsOFF();
	
			if(_seqOn)
			{
				UpdateCoreTimer(_period);
				_seqOn = FALSE;
				return;
			}
	
	        if(_seqIdx <= _seqStop)
	        {
	            if(_gameSequence[_seqIdx].red)
	                mSetRedLedON();
	
	            if(_gameSequence[_seqIdx].yellow)
	                mSetYellowLedON();
	            
	            if(_gameSequence[_seqIdx].green)
	                mSetGreenLedON();
	
	            UpdateCoreTimer(_gameSequence[_seqIdx].period);
	
	            _seqIdx++;
				_seqOn = TRUE;
	
	        }
			else
	        {
	            UpdateCoreTimer(_gameSequence[_seqIdx].period << 1);
	            _seqIdx = 0;
	            _gameState = SIMON_SAYS_EVAL;
				_seqOn = TRUE;
	        }
	        break;

	    case SIMON_SAYS_EVAL:
	        _gameState = SIMON_SAYS_LOSER;
	        UpdateCoreTimer(_period << 1);
	        mSetAllLedsON();
	        break;
        

	    case SIMON_SAYS_LOSER:
	        _gameState = SIMON_SAYS_START;
	        break;

	    case SIMON_SAYS_WIN:
	        UpdateCoreTimer(_period);

	        if(_seqIdx < 50)
	        {
	            mSetAllLedsOFF();
	
	            if(_gameSequence[_seqIdx].red)
	            {
	                mSetRedLedON();
	                _gameSequence[_seqIdx].yellow = 1;
	                _gameSequence[_seqIdx].red = 0;
	            }
				else if(_gameSequence[_seqIdx].yellow)
	            {
	                mSetYellowLedON();
	                _gameSequence[_seqIdx].green = 1;
	                _gameSequence[_seqIdx].yellow = 0;
	            }
				else if(_gameSequence[_seqIdx].green)
	            {
	                mSetGreenLedON();
	                _gameSequence[_seqIdx].red = 1;
	                _gameSequence[_seqIdx].green = 0;
	            }
	        }
			else
	        {
	            _gameState = SIMON_SAYS_START;
	        }
	
	        break;

    }
}
