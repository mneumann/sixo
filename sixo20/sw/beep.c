/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-01 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         Beep
 *  Purpose:        Beep services (timing, auto update)
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
 *  This information is automatically added while 'commiting' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 3.5  2012/02/04 21:49:42  tuberkel
 * All BeeperDriver functions mapped ==> DigOutDrv()
 *
 * Revision 3.4  2012/02/04 20:38:05  tuberkel
 * Moved all BeeperDriver / LEDDriver stuff ==> 'digoutdrv'
 *
 * Revision 3.3  2012/01/14 10:26:44  tuberkel
 * LED PWM handling changed:
 * - no longer Msgs/TimerMsgs used (inaccurate optic)
 * - instead use TimerISR to control PWM
 * - Granulartiy is SystemTicks (20 ms)
 * - works well
 * - prevent direct LEDDrv access (if possible)
 *
 * Revision 3.2  2012/01/14 08:26:00  tuberkel
 * Beeper PWM handling changed:
 * - no longer Msgs/TimerMsgs used (inaccurate acoustic)
 * - instead use TimerISR to control PWM
 * - Granulartiy is SystemTicks (20 ms)
 * - works well
 *
 * Revision 3.1  2011/05/29 12:44:19  tuberkel
 * Beeper: Removed unnecssary Warning if switched off
 *
 * Revision 3.0  2010/11/07 12:43:05  tuberkel
 * V30 Preparations:
 * - Beeper can be disabled by user setting
 *
 * Revision 2.2  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.1  2009/07/08 21:41:17  tuberkel
 * All compiler defines reviewed:
 * - set to unique usage: set define to 0 or 1
 * - default values set, if not used
 * - see 'Project Editor' for details
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 *
 ************************************************************************ */


#include <string.h>

#include "standard.h"
#include "stdmsgs.h"
#include "msgqueue.h"
#include "timer.h"
#include "digoutdr.h"
#include "beep.h"
#include "debug.h"
#include "digindrv.h"
#include "sysparam.h"


/* global symbols */
BEEPTIMINGTYPE BeepTiming;

/* external symbols */
extern DEVFLAGS2_TYPE gDeviceFlags2;


/***********************************************************************
 *  FUNCTION:       BeepInit
 *  DESCRIPTION:    Initializes Beep service & hw driver
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE BeepInit(void)
{
    ERRCODE RValue;

    /* reset PWM timings */
    memset( &BeepTiming, 0x00, sizeof(BEEPTIMINGTYPE));

    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       BeepService
 *  DESCRIPTION:    Provides PWM control with execution time,
 *                  gets called from inside TimerISR
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        If duration has been set to 0 at BeepSetNewState(),
 *                  no further change will done, until next BeepSetNewState()
 *                  call!
 *                  Assumes to get called at every system tick (20 ms).
 *                  Assure the Beeper to be off, after duration time (if any).
 *********************************************************************** */
ERRCODE BeepService(void)
{
    ERRCODE RValue = ERR_OK;

    /* control only, if Duration > 0 */
    if ( BeepTiming.wDurationTicks > 0 )
    {
        /* count down our duration timer */
        BeepTiming.wDurationTicks--;

        /* right now expired? */
        if ( BeepTiming.wDurationTicks == 0 )
        {   DigOutDrv_SetPin( eDIGOUT_BEEP, DIGOUT_OFF );   /* assure beeper being off now for ever! */
        }
        else
        {
            /* beeper currently on? */
            if (TRUE == DigOutDrv_GetPin(eDIGOUT_BEEP) )
            {
                /* OffTime available -> time to switch off again? */
                if (BeepTiming.wOffTicks > 0)
                {
                    /* count down ON timer (if possible) */
                    if (BeepTiming.wOnCurrTicks > 0 )
                    {   BeepTiming.wOnCurrTicks--;      /* keep it on! */
                    }
                    else
                    {   DigOutDrv_SetPin( eDIGOUT_BEEP, DIGOUT_OFF );                         /* ok - done - switch it off! */
                        BeepTiming.wOffCurrTicks = BeepTiming.wOffTicks;    /* reload off timer */
                    }
                }
                else
                {   // nothing to do - never switch off inside duration */
                }
            }
            /* beeper currently off? */
            else
            {
                /* if OnTime Avaliable -> time to switch on again?*/
                if (BeepTiming.wOnTicks > 0)
                {
                    /* count down OFF timer (if possible) */
                    if (BeepTiming.wOffCurrTicks > 0 )
                    {   BeepTiming.wOffCurrTicks--;      /* keep it off! */
                    }
                    else
                    {   DigOutDrv_SetPin( eDIGOUT_BEEP, DIGOUT_ON );                          /* ok - done - switch it on again! */
                        BeepTiming.wOnCurrTicks = BeepTiming.wOnTicks;      /* reload off timer */
                    }
                }
                else
                {   // nothing to do - never switch on inside duration */
                }
            }
        }
    }
    else
    {   // nothing to do (anymore)
    }
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       BeepSetNewState
 *  DESCRIPTION:    setup of beeper PWM signal (granularity in ticks = 20 ms)
 *  PARAMETER:      wOn_ms          cyclic LED ON time in millisec
 *                  wOff_ms         cyclic LED OFF time in millisec
 *                  wDuration_ms    overall duration of PWM in millisec
 *  RETURN:         error code
 *  COMMENT:        wOff_ms = 0         eq. 'permanent ON'
 *                  wOn_ms = 0          eq. 'permanent OFF'
 *                  wDuration_ms = 0    eg. 'permanent ON/OFF'
 *
 *  Note:           If 'wDuration_ms' has been set to 0 at BeepSetNewState(),
 *                  no further change will done, until next BeepSetNewState()
 *                  call!
 *********************************************************************** */
ERRCODE BeepSetNewState(UINT16 wOn_ms, UINT16 wOff_ms, UINT16 wDuration_ms )
{
    ERRCODE RValue = ERR_OK;

    // check: beeper disabled by user/eeprom settings?
    if ( gDeviceFlags2.flags.BeeperAvail == FALSE )
    {   // do nothing, just ignore message
        //ODS(DBG_SYS,DBG_WARNING,"Unable to use MSG_BEEP_ON/OFF,  beeper disabled!");
        RValue = ERR_OK;
        return (RValue);
    }

    /* ok, save new beep PWM timings (asure at least one tick, intervall is < 1 tick!) */
    BeepTiming.wOnTicks         = MS2TICKS(wOn_ms       + (MS_PER_TICK-1) );
    BeepTiming.wOffTicks        = MS2TICKS(wOff_ms      + (MS_PER_TICK-1));
    BeepTiming.wDurationTicks   = MS2TICKS(wDuration_ms + (MS_PER_TICK-1));

    /* initialize downcounting PWM timers */
    BeepTiming.wOnCurrTicks     = BeepTiming.wOnTicks;
    BeepTiming.wOffCurrTicks    = BeepTiming.wOffTicks;

    /* show internal control structure */
    ODS3(DBG_SYS,DBG_INFO,"Beeper setup: ON:%u OFF:%U DUR:%u",
                BeepTiming.wOnTicks, BeepTiming.wOffTicks, BeepTiming.wDurationTicks);

    /* directly switch beeper to advised mode (ON/OFF, always starts with ON) */
    if ( BeepTiming.wOnTicks > 0)
         DigOutDrv_SetPin( eDIGOUT_BEEP, DIGOUT_ON );     // switch ON immedeately
    else DigOutDrv_SetPin( eDIGOUT_BEEP, DIGOUT_OFF );    // switch OFF immedeately

    /* further beeper PWM control is done in BeepService(), which is called with 50 Hz
       inside the Timer ISR */
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       BeepOk
 *  DESCRIPTION:    creates an 'OK'-Beep: 600 ms permanent ON
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void BeepOk(void)
{
    /* set Beeper ON now! */
    BeepSetNewState(1, 0, 600 );
}



/***********************************************************************
 *  FUNCTION:       BeepEsc
 *  DESCRIPTION:    creates an 'Esc'-Beep: 1000 ms duration pulsed
 *                  with 50 ms ON / 50 ms OFF
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void BeepEsc(void)
{
    /* pulse Beeper now! */
    BeepSetNewState( 50, 50, 1000 );
}



/***********************************************************************
 *  FUNCTION:       BeepClick
 *  DESCRIPTION:    simulates a very short click: 20 ms permanent ON
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void BeepClick(void)
{
    /* make the shortest possible pulse */
    BeepSetNewState( 1, 0, 20 );
}



/***********************************************************************
 *  FUNCTION:       TestBeepSendMessage
 *  DESCRIPTION:    just tests timer and timer messages
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Sends a few messages wich will activate a
 *                  sequence of beeps.
 *********************************************************************** */
#if(TESTBEEP==1)
void TestBeepSendMessage(void)
{
    MESSAGE msg;

     /* make the shortest possible pulse */
    BeepSetNewState(  1, 0, 20 );
    Delay_ms(2000);

   /* set Beeper to pulse with on/off with 1 Hz for 5 sec.*/
    BeepSetNewState(500, 500, 5000 );
    Delay_ms(2000);

    /* set Beeper to pulse with on/off with 5 Hz for 5 sec.*/
    BeepSetNewState( 100, 100, 5000 );
    Delay_ms(2000);

}
#endif // TESTBEEP



