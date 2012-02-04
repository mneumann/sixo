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
 *  Module:         LED
 *  Purpose:        led services (timing, auto update)
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
 * Revision 3.5  2012/02/04 20:38:05  tuberkel
 * Moved all BeeperDriver / LEDDriver stuff ==> 'digoutdrv'
 *
 * Revision 3.4  2012/02/04 08:34:10  tuberkel
 * BugFix LED PWM
 *
 *
 ************************************************************************ */


#include <string.h>

#include "standard.h"
#include "stdmsgs.h"
#include "msgqueue.h"
#include "timer.h"
#include "digoutdr.h"
#include "led.h"
#include "debug.h"
#include "digindrv.h"

/* global symbols */
LEDTIMINGTYPE LedTiming[LED_MAX];     // LED control array


/***********************************************************************
 *  FUNCTION:       LEDInit
 *  DESCRIPTION:    Initializes led service & hw driver
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LEDInit(void)
{
    ERRCODE RValue;
    UINT8   i;

    /* reset PWM timings */
    memset( &LedTiming, 0x00, sizeof(LEDTIMINGTYPE));

    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       LEDService
 *  DESCRIPTION:    Provides PWM control with execution time,
 *                  gets called from inside TimerISR
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        If duration has been set to 0 at LEDSetNewState(),
 *                  no further change will done, until next LEDSetNewState()
 *                  call!
 *                  Assumes to get called at every system tick (20 ms).
 *                  Assure the LED to be off, after duration time (if any).
 *********************************************************************** */
ERRCODE LEDService(void)
{
    ERRCODE     RValue = ERR_OK;
    LED_ENUM    eLed;

    /* loop over available Leds
       Note: some of the Leds are currently directly controled via GPIs 8e.g. TURN_L/R) */
    for ( eLed = LED_NEUTR; eLed < LED_ERR; eLed++ )
    {
        /* -------------------------------------------- */
        /* control only, if Duration > 0 */
        if ( LedTiming[eLed].dwDurTicks > 0 )
        {
            /* count down our duration timer */
            LedTiming[eLed].dwDurTicks--;

            /* right now expired? */
            if ( LedTiming[eLed].dwDurTicks == 0 )
            {   LEDDrvSetLED(eLed, LED_OFF);         /* assure Led being off now for ever! */
            }
            else
            {
                /* Led currently on? */
                if (TRUE == LEDDrvGetLED(eLed))
                {
                    /* OffTime available -> time to switch off again? */
                    if (LedTiming[eLed].wOffTicks > 0)
                    {
                        /* count down ON timer (if possible) */
                        if (LedTiming[eLed].wOnCurrTicks > 0 )
                        {   LedTiming[eLed].wOnCurrTicks--;      /* keep it on! */
                        }
                        else
                        {   LEDDrvSetLED(eLed, LED_OFF);                         /* ok - done - switch it off! */
                            LedTiming[eLed].wOffCurrTicks = LedTiming[eLed].wOffTicks;    /* reload off timer */
                        }
                    }
                    else
                    {   // nothing to do - never switch off inside duration */
                    }
                }
                /* Led currently off? */
                else
                {
                    /* if OnTime Avaliable -> time to switch on again?*/
                    if (LedTiming[eLed].wOnTicks > 0)
                    {
                        /* count down OFF timer (if possible) */
                        if (LedTiming[eLed].wOffCurrTicks > 0 )
                        {   LedTiming[eLed].wOffCurrTicks--;      /* keep it off! */
                        }
                        else
                        {   LEDDrvSetLED(eLed, LED_ON);                          /* ok - done - switch it on again! */
                            LedTiming[eLed].wOnCurrTicks = LedTiming[eLed].wOnTicks;      /* reload off timer */
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
        /* -------------------------------------------- */
    }
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       LEDSetNewState
 *  DESCRIPTION:    setup of LED PWM signal (granularity in ticks = 20 ms)
 *  PARAMETER:      eLED            index of LE to be controlled
 *                  wOn_ms          cyclic LED ON time in millisec (max. 3 sec.)
 *                  wOff_ms         cyclic LED OFF time in millisec (max. 3 sec.)
 *                  wDuration_ms    overall duration of PWM in millisec (max 60 s)
 *  RETURN:         error code
 *  COMMENT:        wOff_ms = 0         eq. 'permanent ON'
 *                  wOn_ms = 0          eq. 'permanent OFF'
 *                  wDuration_ms = 0    eg. 'permanent ON/OFF'
 *
 *                  If 'wDuration_ms' has been set to 0 at LEDSetNewState(),
 *                  no further change will done, until next LEDSetNewState()
 *                  call!
 *
 *                  Might be called repetive with same parameters (e.g. OIL-SWITCH).
 *                  so we early check any changes to prevent irrelvant setup/debugouts.
 *********************************************************************** */
ERRCODE LEDSetNewState(LED_ENUM eLed, UINT16 wOn_ms, UINT16 wOff_ms, UINT16 wDuration_ms )
{
    ERRCODE RValue = ERR_OK;

    /* check: valid LED index? */
    if ( ( eLed <= LED_MIN ) || ( eLed >= LED_MAX ) )
    {   ODS1(DBG_SYS,DBG_WARNING,"LEDSetNewState() Invalid index %u", eLed );
        RValue = ERR_OUT_OF_RANGE;
        return (RValue);
    }

    /* check: parameters inside valid area (ms ==> SystemTicks, 20 ms Granularity) */
    if ( ( wOn_ms > 0 ) && ( wOn_ms < MS_PER_TICK ) )
         wOn_ms += MS_PER_TICK;
    if ( ( wOff_ms > 0 ) && ( wOn_ms < MS_PER_TICK ) )
         wOff_ms += MS_PER_TICK;
    if ( ( wDuration_ms > 0 ) && ( wOn_ms < MS_PER_TICK ) )
         wDuration_ms += MS_PER_TICK;

    /* check: any changes? */
    if (  ( LedTiming[eLed].wOnTicks   == MS2TICKS(wOn_ms)       )
        &&( LedTiming[eLed].wOffTicks  == MS2TICKS(wOff_ms)      )
        &&( LedTiming[eLed].dwDurTicks == MS2TICKSL(wDuration_ms) ) )
    {   //ODS1(DBG_SYS,DBG_INFO,"LEDSetNewState() - no changes for LED[%s]", LEDgetName(eLed) );
        RValue = ERR_OK;
        return (RValue);
    }

    /* ok, save new PWM timings (assure at least one tick, intervall is < 1 tick!) */
    LedTiming[eLed].wOnTicks    = MS2TICKS(wOn_ms      );
    LedTiming[eLed].wOffTicks   = MS2TICKS(wOff_ms     );
    LedTiming[eLed].dwDurTicks  = MS2TICKSL(wDuration_ms);

    /* initialize downcounting PWM timers */
    LedTiming[eLed].wOnCurrTicks     = LedTiming[eLed].wOnTicks;
    LedTiming[eLed].wOffCurrTicks    = LedTiming[eLed].wOffTicks;

    /* show internal control structure */
    ODS4(DBG_SYS,DBG_INFO,"LED[%s]: ON:%u OFF:%u DUR:%ul (ticks)",
                            LEDGetName(eLed),
                            LedTiming[eLed].wOnTicks,
                            LedTiming[eLed].wOffTicks,
                            LedTiming[eLed].dwDurTicks);

    /* directly switch LED to advised mode (ON/OFF, always starts with ON) */
    if ( LedTiming[eLed].wOnTicks > 0)
         LEDDrvSetLED(eLed, LED_ON);    // switch ON  immedeately
    else LEDDrvSetLED(eLed, LED_OFF);   // switch OFF immedeately

    /* further LED PWM control is done in LedService(), which is called with 50 Hz
       inside the Timer ISR */
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       LEDGetState()
 *  DESCRIPTION:    Returns current state of single LED
 *  PARAMETER:      eLED            LED indicator
 *  RETURN:         fActivate       TRUE  = LED on
 *                                  FALSE = LED off
 *  COMMENT:        -
 *********************************************************************** */
BOOL LEDGetState( LED_ENUM eLED )
{
    return ( LEDDrvGetLED( eLED ) );
}




/***********************************************************************
 *  FUNCTION:       LEDOk
 *  DESCRIPTION:    let the info led be an acknowledge:
 *                  600 ms permanent ON
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void LEDOk(void)
{
    /* prevent usage, if INFO led already on! */
    if ( FALSE == LEDGetState( LED_INFO ) )
    {
        /* set Info-LED ON now! (and later off) */
        LEDSetNewState( LED_INFO, 1, 0, 600 );
    }
}



/***********************************************************************
 *  FUNCTION:       LEDEsc
 *  DESCRIPTION:    let the info led be an error indicator:
 *                  1000 ms duration pulsed with 100 ms ON / 100 ms OFF
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void LEDEsc(void)
{
    /* prevent usage, if INFO led already on! */
    if ( FALSE == LEDGetState( LED_INFO ) )
    {
        /* pulse LED now! */
        LEDSetNewState( LED_INFO, 100, 100, 1000 );
    }
}


/***********************************************************************
 *  FUNCTION:       LEDGetName
 *  DESCRIPTION:    just a debug helper to return LED name
 *  PARAMETER:
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
STRING LEDGetName( LED_ENUM eLED )
{
    STRING RValue = "";

    switch (eLED)
    {
        case LED_NEUTR  : RValue = "NTR"; break;
        case LED_TURN   : RValue = "TRN"; break;
        case LED_INFO   : RValue = "INF"; break;
        case LED_BEAM   : RValue = "HBM"; break;
        case LED_WARN   : RValue = "WRN"; break;
        case LED_ERR    : RValue = "ERR"; break;
        default         : RValue = "?"; break;
    }
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       LEDTest
 *  DESCRIPTION:
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
#if(TESTLED==1)
void LEDTest(unsigned int loopdelay)
{
    LEDDRV_LEDS eLED;

    for (eLED = 0; eLED < LEDDRV_MAX; eLED++)
    {
        LEDDrvSetLED(eLED, TRUE);
        Delay_ms(loopdelay);
    }
    for (eLED = 0; eLED < LEDDRV_MAX; eLED++)
    {
        LEDDrvSetLED(eLED, FALSE);
        Delay_ms(loopdelay);
    }
}
#endif //(TESTLED==1)
