/***********************************************************************
 *
 *  $RCSfile$
 *
 *  $Revision$
 *
 *  $Date$
 *
 *  $Author$
 *
 *  --------------------------------------------------------------------
 *  Company:        N&K Development, all rights reserved
 *
 *                  Neugebauer & Krizsan Cooperation, Germany
 *                  contact:    arnold.neugebauer@web.de
 *                              ralf.krizsan@web.de
 *  --------------------------------------------------------------------
 *  Created:        25-10-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         Timer
 *  Purpose:        time processing for general processing and message queue
 *  Comments:       -
 *
 ************************************************************************ */

#include <string.h>
#include "sfr62.h"
#include "standard.h"
#include "msgqueue.h"
#include "timer.h"
#include "debug.h"
#include "measdrv.h"
#include "digoutdr.h"



/* max number of functions to be registered */
#define MAX_TIMER_FUNCTIONS        6


/* -------------------------------------------- */
/* global variables                             */

EntryFunction   gaEntryFunctions[MAX_TIMER_FUNCTIONS];  // array of timer entry functions
TIMERMSG        gaMessageQueueTimer[MAX_TIMERMSG_NUM];  // array of countdown timers.
                                                        // message will be posed when timer expires

UINT8   gbFuncs                 = 0;        // counter of registered functions
UINT32  gdwTicks                = 0;        // system time in ticks
UINT8   gbLastTimer             = 0;        // counter for used timers
UINT8   gbMaxTimerQueueEntries  = 0;        // index of last used timer

UINT16  wMilliSecCounter      = 0;        // valid values: 0h .. ffffh
UINT16  wSecCounter           = 0;        // valid values: 0h .. ffffh
                                            // use TimerGetSys_sec() & TimerGetSys_msec()

UINT16  gbTimerInterruptLoad    = 0;        // processor load by TimerInterrupt() in percent


/* -------------------------------------------- */
/* external variables                           */
extern  UINT16  wUSPMax;                    // maximum user stack pointer
extern  UINT16  wUSP;                       // for fast local copy
extern  UINT16  wISPMax;                    // maximum interrupt stack pointer
extern  UINT16  wISP;                       // for fast local copy





/******************************************************************************
* initilize timer
*******************************************************************************/
ERRCODE TimerInit(void)
{
   UINT8 i;

   /* initialize hardware */
   InitTimerHW();

   /* no timer functions registered */
   gbFuncs = 0;
   for ( i = 0; i < MAX_TIMER_FUNCTIONS; i++ )
   {
      gaEntryFunctions[i] = NULL;
   };
   ODS(DBG_SYS,DBG_INFO,"TimerInit() done!");
   return ERR_OK;
}

/******************************************************************************
* kill timer
*******************************************************************************/
ERRCODE TimerExit(void)
{
   UINT8 i;

   gbFuncs = 0; /* no function registered */
   for ( i = 0; i < MAX_TIMER_FUNCTIONS; i++ )
   {
      gaEntryFunctions[i] = NULL;
   };

   return ERR_OK;
}

/******************************************************************************
*  register entry function
*******************************************************************************/
ERRCODE TimerRegisterEntryFunction(EntryFunction pFunc)
{
   INT8 i;

   /* check if function already registered*/
   for ( i = 0; i < gbFuncs; i++ )
   {
      /* function is registered */
                if (gaEntryFunctions[i] == pFunc) return ERR_OK;
   }

   /* check for available space in array */
        if ( gbFuncs < MAX_TIMER_FUNCTIONS )
   {
      /* register */
                gaEntryFunctions[gbFuncs] = pFunc;
                gbFuncs++; // then increment the count
      return ERR_OK;
   }
   else // no space
   {
      return ERR_TIMER_NO_FUNC;
   }
}


/******************************************************************************
* remove entry function
*******************************************************************************/
ERRCODE TimerRemoveEntryFunction(EntryFunction pFunc)
{
        /* ToDo: implement */
}


/******************************************************************************
* return system time in ticks
*******************************************************************************/
UINT32 TimerGetTicks(void)
{
    UINT32 dwTicks;
    INT_GLOB_DISABLE
    dwTicks = gdwTicks;
    INT_GLOB_ENABLE
    return dwTicks;
}

/***********************************************************************
 *  FUNCTION:       SetTimerMsg
 *  DESCRIPTION:    Takes a message to be sent later
 *  PARAMETER:      MESSAGE msg     Message to be sent delayed
 *                  INT16   time    time in internal ticks
 *  RETURN:         ERRCODE:
 *                  - ERR_TIMERMSG_NOT_FOUND:   not a valid message
 *                  - ERR_TIMERMSG_FULL:        no more timer msg accepted
 *                  - ERR_OK:                   msg will be sent
 *  COMMENT:        Use MS2TICKS SEC2TICKS to give usefull time format
 *                  - ToBeDoneLater: dynamically register MesgEntryFunction...
 *********************************************************************** */
ERRCODE SetTimerMsg(MESSAGE msg, INT16 time)
{
   TIMERMSG TIMERMSG;

   // makes no sense to post a MSG_NULL_MSG
   if ( MSG_ID(msg) == MSG_NULL_MSG )
   {
        return ERR_TIMERMSG_NOT_FOUND;
   }

   // check: accept one more timer message?
   if ( gbLastTimer < MAX_TIMERMSG_NUM )
   {
      TIMERMSG.msg  = msg;                          // internal format
      TIMERMSG.time = time;
      gaMessageQueueTimer[gbLastTimer] = TIMERMSG;  // copy into array
      INT_GLOB_DISABLE;                             // disable interrupts ??
      gbLastTimer++;                                // hold current number
      if ( gbLastTimer > gbMaxTimerQueueEntries )
      {
         gbMaxTimerQueueEntries = gbLastTimer;      // diagnostic info
      }
      INT_GLOB_ENABLE;                              // enable interrupts
   }
   else
   {
      return ERR_TIMERMSG_FULL;
   }

   return ERR_OK;
}


/******************************************************************************
* removes message from queue
*******************************************************************************/
ERRCODE TimerRemoveMsg(MESSAGE msg)
{
   INT8    i;

   for( i = 0; i < gbLastTimer; i++ )
   {
      /* compare whole structure */
      if (        ( MSG_ID(gaMessageQueueTimer[i].msg) == MSG_ID(msg) )
                   && ( memcmp(&(gaMessageQueueTimer[i].msg), &msg, 3) == 0 ) )
      {
                         /* disable interrupt while deleting timer from queue */
                        INT_GLOB_DISABLE;
         gaMessageQueueTimer[i] = gaMessageQueueTimer[gbLastTimer-1];
         gbLastTimer--;
         INT_GLOB_ENABLE;         //INT enable

         return ERR_OK;
      }
        }  /* of for */

        return ERR_TIMERMSG_NOT_FOUND;
}

/******************************************************************************
* removes all messages with given id from queue
*******************************************************************************/
ERRCODE TimerRemoveMsgId(MESSAGE_ID msg_id)
{
    INT8    i;
    ERRCODE RetVal = ERR_TIMERMSG_NOT_FOUND;
    BOOL msg_found;

    do
    {
        msg_found = FALSE;
        for( i = 0; i < gbLastTimer; i++ )
        {
            /* compare whole structure */
            if ((msg_found == FALSE) && (MSG_ID(gaMessageQueueTimer[i].msg) == msg_id))
            {
                /* disable interrupt while deleting timer from queue */
                INT_GLOB_DISABLE;
                gaMessageQueueTimer[i] = gaMessageQueueTimer[gbLastTimer-1];
                gbLastTimer--;
                INT_GLOB_ENABLE;         //INT enable

                msg_found = TRUE;
                RetVal = ERR_OK;
            }
        }
    }  while (msg_found == TRUE); /* of for */

    return RetVal;
}


/***********************************************************************
 * FUNCTION:       TimerInterrupt
 * DESCRIPTION:    Directly connected to m16c uC timer ta1
 *                 Currently called every 20 ms (50 Hz)
 *                 Purpose:    - Call all registered timer functions
 *                             - Handle messages to be send with delay
 *                             - Support internal system time (TICKS)
 * PARAMETER:      -
 * RETURN:         -
 * COMMENT:        -
 *********************************************************************** */
#pragma INTERRUPT TimerInterrupt        // declare as M16C isr
void TimerInterrupt(void)
{
    UINT16  wEntry_ms    = 0;
    UINT16  wExit_ms     = 0;
    UINT8   bActLoad     = 0;
    UINT8   i;

    TOGGLE_PAD28;

    // get current last time
    TimerGetSys_msec(wEntry_ms);

    // !!! Re-Enable higher interrupts than this interrupt!
    // (for use of MilliSecCounter)
    INT_GLOB_ENABLE

    // -----------------------------------------
    // increment internal system time
    gdwTicks++;

    // -----------------------------------------
    // call all registered timer entry functions
    for( i = 0; i < gbFuncs; i++ )
    {
        UINT16  wEntry_ms    = 0;
        UINT16  wExit_ms     = 0;

        EntryFunction pFn;

        TimerGetSys_msec(wEntry_ms);

        if( (pFn = gaEntryFunctions[i]) != NULL )
        {
            (*pFn)();  // execute registered entry function
        }

        TimerGetSys_msec(wExit_ms);
        if ((wExit_ms - wEntry_ms) > WARN_TIMERLOAD_MS)
            ODS2(   DBG_SYS, DBG_WARNING,
                    "TimerEntryFct 0x%lx took %u ms!",
                    pFn, (wExit_ms - wEntry_ms));
    }

    // -----------------------------------------
    // check for any expired message timer
    for( i = 0; i < gbLastTimer; i++ )
    {
        if ( gaMessageQueueTimer[i].time == 0 )
        {
            // try to post message with HIGH priority
            if ( MsgQPostMsg(gaMessageQueueTimer[i].msg, MSGQ_PRIO_HIGH) == ERR_OK )
            {
                gbLastTimer--;
                gaMessageQueueTimer[i] = gaMessageQueueTimer[gbLastTimer];
            }
            else
            {
                /* posting not successful, try again next time */
            }
        }
        else
        {
            /* not expired, decrement time */
            gaMessageQueueTimer[i].time--;
        }
    }  // of checking timers

    // check last use of this isr
    TimerGetSys_msec(wExit_ms);

    // calculate processor load by this isr in percent
    // (should be about 10%)
    bActLoad = ((wExit_ms - wEntry_ms) * TICKS_PER_SECOND) / 10;

    // safe max load
    if (bActLoad > gbTimerInterruptLoad)
        gbTimerInterruptLoad = bActLoad;

    // 'out of echtzeit' warning: Load > 100%
    if (bActLoad >= 100)
        ODS1(DBG_SYS, DBG_WARNING, "TimerInterrupt overloaded! (Load: %d \x25)", bActLoad);
}

/***********************************************************************
 *        FUNCTION:       msIntFunction
 *        DESCRIPTION:    called by 1 ms timer ta0
 *        PARAMETER:      -
 *        RETURN:         -
 *        COMMENT:        supports global variables:
 *                        - UINT16 MilliSecCounter;
 *                        - UINT16 SecCounter;
 *                        uses m16c fast isr mode (switches register bank)
 *********************************************************************** */
#pragma INTERRUPT/B msIntFunction           // '/B' declare as 'fast' M16C isr
void msIntFunction(void)
{
    // !!! Re-Enable higher interrupts than this interrupt!
    INT_GLOB_ENABLE

    TOGGLE_PAD27;

    // diagnostic: get current INTERRUPT stack pointer
    // (note: we ARE in a isr, so flagregister u-flag is 0!)
    asm(" stc sp,$$", wISP);
    if (wISP < wISPMax)                     // get highest interrupt stack pointer
        wISPMax = wISP;

    // diagnostic: get current USER stack pointer
    // (note: we ARE in a isr, so flagregister u-flag first has to be set to 1!)
    asm(" fset u");                         // select user stack
    asm(" stc sp,$$", wUSP);
    if (wUSP < wUSPMax)                     // get highest interrupt stack pointer
        wUSPMax = wUSP;
    asm(" fclr u");                         // set back to interrupt stack

    // system time
    wMilliSecCounter++;                   // inkr. ms counter
    if (wMilliSecCounter % 1000 == 0)
            wSecCounter++;                // inkr. sec counter

    // DEBUG:
    //if (wMilliSecCounter % 200 == 0)
        //ODS2(DBG_SYS, DBG_INFO, "WHEEL: %u   RPM: %u", MeasDrvGetWheelPeriod(), MeasDrvGetRPMPeriod() );
}



/***********************************************************************
 *        FUNCTION:       TimerGetTimerISRLoad
 *        DESCRIPTION:    returns the system load caused by TimerInterrupt()
 *        PARAMETER:      -
 *        RETURN:         system load in percent
 *        COMMENT:        for debugging purpose
 *********************************************************************** */
UINT8 TimerGetTimerISRLoad(void)
{
    return gbTimerInterruptLoad;
}



/***********************************************************************
 *        FUNCTION:       InitTimerHW
 *        DESCRIPTION:    Initialize timer hardware of m16c
 *        PARAMETER:      -
 *        RETURN:         -
 *        COMMENT:        timer a0 reserved for ms counter purpose:
 *                          - 10 MHhz           -> 100ns instruction cycle
 *                          - f32 divisor       -> 3.2�s counter decr. unit
 *                          - start value 313   -> every 1,0016 ms isr call
 *                        timer a1 reserved for MessageQueueTimer 50Hz:
 *                          - 10 MHhz           -> 100ns instruction cycle
 *                          - f32 divisor       -> 3.2�s counter decr. unit
 *                          - start value 6250  -> every 20 ms isr call
 *********************************************************************** */
void InitTimerHW(void)
{
    /* TIMERA0 as MilliSecCounter Timer */
    INT_GLOB_DISABLE
    ta0s    = FALSE;        // stop timer0 NOW (if running...)
    ta0mr   = 0x80;         // timer0 mode register:
                            //  - timer mode
                            //  - no pulse output
                            //  - no gate function
                            //  - count source: f32
    ta0ic   = 0x05;         // timer0 interrupt control register:
                            //  - interrupt level = 5 (medium high)
    ta0ud   = FALSE;        // timer0 updown flag: 0 -> down
    ta0     = 313;          // timer0 start value & reload register
    ta0s    = TRUE;         // timer0 start flag:  1 -> run NOW!

    /* TIMERA1 as MessageQueue Timer with 50 Hz */
    ta1s    = FALSE;        // stop timer1 NOW (if running...)
    ta1mr   = 0x80;         // timer1 mode register:
                            //  - timer mode
                            //  - no pulse output
                            //  - no gate function
                            //  - count source: f32
    ta1ic   = 0x02;         // timer1 interrupt control register:
                            //  - interrupt level = 2 (eq. 2nd lowest)
    ta1ud   = FALSE;        // timer1 updown flag: 0 -> down
    ta1     = 6250;         // timer1 start value & reload register
    ta1s    = TRUE;         // timer1 start flag:  1 -> run NOW!
    INT_GLOB_ENABLE
}

/***********************************************************************
 *  FUNCTION:       Delay_ms
 *  DESCRIPTION:    simulated delay in ms
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void Delay_ms( unsigned int uiMilliSec )
{
	UINT16 wStart = 0;
	UINT16 wAct = 0;

	TimerGetSys_msec(wStart);
    TimerGetSys_msec(wAct);
	while ( (wAct - wStart) < uiMilliSec)
	    TimerGetSys_msec(wAct);
}


