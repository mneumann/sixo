/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-10-25 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         timer
 *  Purpose:        timer export header
 *  Comments:       -
 *
 *  --------------------------------------------------------------------
 *
 *  Compiler:       Renesas NC30WA V.5.00 Release 2
 *  Options:        -
 *
 *  --------------------------------------------------------------------
 *
 *  Licence details
 *
 *  This software is copyright © 2001-2004 by N&K Development, Germany
 *
 *  You can redistribute it and/or modify it under the terms of the
 *  GNU General Public License version 2 as published by the
 *  Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 *  MA  02111-1307, USA.
 *
 *  To purchase support or enhancements for this software, contact:
 *
 *      N&K Development                     N&K Development
 *      Arnold Neugebauer                   Ralf Schwarzer
 *      Stöckener Str. 115                  Hudeplan 37 A
 *      30419 Hannover                      30453 Hannover
 *      arnold.neugebauer@web.de            ralf.schwarzer@htp-tel.de
 *
 *      or try http://www.sixo.de
 *
 *  --------------------------------------------------------------------
 *
 *  Disclaimer of warrenty
 *
 *  The software is provided as is without warranties of
 *  any kind, either express or implied. To the fullest
 *  extent allowed by applicable law, N&K Development
 *  disclaims all warranties, express or implied,
 *  including, but not limited to, implied warranties of
 *  merchantability, fitness for a particular purpose, and
 *  non-infringement.
 *
 *  --------------------------------------------------------------------
 *
 *  CVS History
 *
 *  This information is automatically added while 'commit' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 3.1  2012/02/04 08:41:04  tuberkel
 * New: MS2TICKSL - long version
 *
 *
 ************************************************************************ */


#ifndef _TIMER_H
#define _TIMER_H


#include "standard.h"
#include "msgqueue.h"


/* hardware settings */
#define FQUARTZ     3686400L    // used Quartz
#define PLL_FACT    4L          // used PLL factor
#define FSYS        (4*FQUARTZ) // = 14745600L FSYS with PLL-factor


/* frequency, in which isr TimerInterrupt() is called
 *                -> act. value: 50 Hz
 *                -> to change this value look at InitTimerHW() */
#define TICKS_PER_SECOND    50  // in Hz, for use in common software
#define TICKS_PER_SECL      50L // in Hz, for use in Timer Init calculation with FSYS
#define MSEC              1000L // in ms/second, for use in Timer Init calculation with FSYS

#define MS_PER_TICK       MSEC/TICKS_PER_SECOND // in ms/Tick, cyccle time, with which the 50-Hz-timer ISR will be callled

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
#define MS2TICKSL(x)       ((x)*TICKS_PER_SECL/1000L)
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
