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
 *  Module:         timer
 *  Purpose:        timer export header
 *  Comments:       -
 *
 ************************************************************************ */
#ifndef _TIMER_H
#define _TIMER_H


#include "standard.h"
#include "msgqueue.h"

/* frequency, in which isr TimerInterrupt() is called
 *                -> act. value: 50 Hz
 *                -> to change this value look at InitTimerHW() */
#define TICKS_PER_SECOND  50        // in Hz


#define WARN_TIMERLOAD_MS 15          // ODS() warning if timer function took more than 15 ms

/* interface to get copy of system time counters */
#define TimerGetSys_sec(x)        (x = wSecCounter)
#define TimerGetSys_msec(x)       (x = wMilliSecCounter)

/* define timer message format */
typedef struct
{
        MESSAGE msg;
        INT16   time;
} TIMERMSG;

/* define max. number of timer messages in message queue */
#define MAX_TIMERMSG_NUM        10


/* macros to convert seconds to timerticks and vice versa */
/* change milliseconds to ticks */
#define MS2TICKS(x)        ((x)*TICKS_PER_SECOND/1000)
/* change seconds to ticks */
#define SEC2TICKS(x)        ((x)*TICKS_PER_SECOND)
/* change ticks to milliseconds */
#define TICKS2MS(x)        ((x)*1000/TICKS_PER_SECOND)
/* change ticks to seconds */
#define TICKS2SEC(x)        ((x)/TICKS_PER_SECOND)


/* timer entry function */
typedef void (*EntryFunction)(void);

/* prototypes */
ERRCODE TimerInit(void);                           /* initialize timer */
ERRCODE TimerExit(void);                        /* close timer */
ERRCODE SetTimerMsg(MESSAGE msg, INT16 time);   /* put timer message into queue */
ERRCODE TimerRemoveMsg(MESSAGE msg);            /* remove timer message from queue */
ERRCODE TimerRemoveMsgId(MESSAGE_ID msg_id);    /* remove all timer messages with given id from queue */
UINT32 TimerGetTicks(void);                     /* get timer count */
ERRCODE TimerRegisterEntryFunction(EntryFunction pFunc);    /* add entry function */
ERRCODE TimerRemoveEntryFunction(EntryFunction pFunc);      /* remove entry function */
void TimerInterrupt(void);                      /* timer interrupt routine  */
UINT8 TimerGetTimerISRLoad(void);               /* timer interrupt system load */
void InitTimerHW(void);                         /* timer hardware settings */
void Delay_ms( unsigned int uiMilliSec );       /* general purpose delay */

#endif  /* _TIMER_H    */
