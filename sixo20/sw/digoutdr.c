/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-01 by Arnold Neugebauer
 *  Project:        SIxO
 *  Module:         Digital Output Driver
 *  Purpose:        Low level support of all digital outputs
 *                  (but not GPOs and Buzzer!)
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
 * Revision 3.7  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 3.6  2012/02/04 22:25:49  tuberkel
 * LEDs renamed
 *
 * Revision 3.5  2012/02/04 21:59:15  tuberkel
 * Toggle Pads renamed
 *
 * Revision 3.4  2012/02/04 21:49:42  tuberkel
 * All BeeperDriver functions mapped ==> DigOutDrv()
 *
 * Revision 3.3  2012/02/04 20:38:05  tuberkel
 * Moved all BeeperDriver / LEDDriver stuff ==> 'digoutdrv'
 *
 * Revision 3.2  2012/02/04 08:33:13  tuberkel
 * BugFix DigOut PWM
 *
 *
 ************************************************************************ */

#include <string.h>

#include "standard.h"
#include "timer.h"
#include "sfr62p.h"
#include "debug.h"
#include "digoutdr.h"




/* global symbols */
DIGOUT_PWMCTRL_TYPE DigOut_PWMCtrl[DIGOUT_PINS_MAX];     // DigOut PWM Control Structure





/***********************************************************************
 *  FUNCTION:       DigOut_Init
 *  DESCRIPTION:    Initializes all SIxO Output Pins and some solder pads
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        To prevents floating input pins p3_0 / p3_1 of
 *                  destroying transistors, should be called as early as
 *                  possible
 *********************************************************************** */
ERRCODE DigOutDrv_Init(void)
{
    /* --------------------------------------------------------- */
    /* set general purpose outputs GPO0/1 to prevent damage */
    PIN_GPO0    = 0;
    PIN_GPO1    = 0;
    PIN_GPO0_D  = 1;
    PIN_GPO1_D  = 1;

    /* --------------------------------------------------------- */
    /* Beeper port pin (switch off & set to OUT) */
    PIN_BEEP    = 0;
    PIN_BEEP_D  = 1;

    /* --------------------------------------------------------- */
    /* LED ports (Attention: pd9 protected by prc2!) */
    PORT_LED &= ~PORT_LED_MASK;     // switch off all leds (LED pins only) !
    prc2       = 1;                 // unprotect pd9 by prc2 (automatic falls back to protection)
    PORT_LED_D = PORT_LED_MASK;     // bits 3 and 4 reserved for display!


    /* switch on LED brightness control */
    PIN_LED_BRIGHT_D = 1;           // port direction to OUTPUT
    PIN_LED_BRIGHT   = 1;           // enable LED common current control transistor


    /* --------------------------------------------------------- */
    /* TEST LEDS: 'test flash' to display init success? */
    #if(TEST_LED_PORTS==1)
    if (fFlash == TRUE)
    {
        PORT_LED = PORT_LED_MASK;      // all LEDs on
        Delay_ms(500);      // wait
        PORT_LED &= ~PORT_LED_MASK;    // all LEDs off
    }
    #endif

    /* --------------------------------------------------------- */
    /* TEST PADS: Port p3_2/3/4 for ossi checks */
    #if(TEST_TOGGLEPADS_PORTS==1)
    PORT_TESTPAD   = PORT_TESTPAD   & (~PORT_TESTPAD_MASK);   /* clear output of p3_2/3/4 */
    PORT_TESTPAD_D = PORT_TESTPAD_D |   PORT_TESTPAD_MASK;    /* set p3_2/3/4 to output */
    // use macros PIN_TESTPAD9_TOGGLE/10/11 for further tests
    #endif

    /* --------------------------------------------------------- */
    /* reset or PWM control structure */
    memset( DigOut_PWMCtrl, 0x00, sizeof(DigOut_PWMCtrl) );

    // use macros PIN_GPO0/1 for further use
    ODS(DBG_DRV,DBG_INFO,"DigOutDrv_Init() done!");

    return ERR_OK;
}




 /***********************************************************************
 *  FUNCTION:       DigOutDrv_SetPin()
 *  DESCRIPTION:    DIRECT control over one single GPO
 *  PARAMETER:      ePin            GPO indicator
 *                  fActivate       TRUE = GPO on
 *  RETURN:         ERR_OK          ok
 *                  ERR_PARAM_ERR   parameter error
 *  COMMENT:        This function seems to be OBSOLETE, because the new
 *                  state could be set directly.
 *                  Nevertheless the function is very HELPFUL, because
 *                  it give the chance to set BREAKPOINT at a definite
 *                  action (set/clear)!
 *********************************************************************** */
ERRCODE DigOutDrv_SetPin( DIGOUT_PINS ePin, BOOL fActivate)
{
    switch (ePin)
    {
        /*------------------------- */
        case eDIGOUT_GPO_0:
            if (fActivate)
                 PIN_GPO0   = 1;
            else PIN_GPO0   = 0;
            break;
        case eDIGOUT_GPO_1:
            if (fActivate)
                 PIN_GPO1   = 1;
            else PIN_GPO1   = 0;
            break;
        /*------------------------- */
        case eDIGOUT_BEEP:
            if (fActivate)
                 PIN_BEEP   = 1;
            else PIN_BEEP   = 0;
            break;
        /*------------------------- */
        case eDIGOUT_LED_WARN:
            if (fActivate)
                 PIN_LED_WARN   = 1;
            else PIN_LED_WARN   = 0;
            break;
        case eDIGOUT_LED_ERROR:
            if (fActivate)
                 PIN_LED_ERR    = 1;
            else PIN_LED_ERR    = 0;
            break;
        case eDIGOUT_LED_INFO:
            if (fActivate)
                 PIN_LED_INFO   = 1;
            else PIN_LED_INFO   = 0;
            break;
        case eDIGOUT_LED_HBEAM:
            if (fActivate)
                 PIN_LED_HBEAM  = 1;
            else PIN_LED_HBEAM  = 0;
            break;
        case eDIGOUT_LED_NEUTR:
            if (fActivate)
                 PIN_LED_NEUTR  = 1;
            else PIN_LED_NEUTR  = 0;
            break;
        case eDIGOUT_LED_TURN:
            if (fActivate)
                 PIN_LED_TURN   = 1;
            else PIN_LED_TURN   = 0;
            break;
        /*------------------------- */
        default:
            return ERR_PARAM_ERR;
    }
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       DigOutDrv_GetPin()
 *  DESCRIPTION:    Returns current state of single port pin
 *  PARAMETER:      ePin            digital port pin indicator
 *  RETURN:         fActivated      TRUE  = GPO on
 *                                  FALSE = GPO off
 *  COMMENT:        -
 *********************************************************************** */
BOOL DigOutDrv_GetPin( DIGOUT_PINS ePin )
{
    BOOL fActivated;
    switch (ePin)
    {
        case eDIGOUT_GPO_0:      fActivated = PIN_GPO0;      break;
        case eDIGOUT_GPO_1:      fActivated = PIN_GPO1;      break;

        case eDIGOUT_BEEP:      fActivated = PIN_BEEP;      break;

        case eDIGOUT_LED_WARN:  fActivated = PIN_LED_WARN;  break;
        case eDIGOUT_LED_ERROR:   fActivated = PIN_LED_ERR;   break;
        case eDIGOUT_LED_INFO:  fActivated = PIN_LED_INFO;  break;
        case eDIGOUT_LED_HBEAM:  fActivated = PIN_LED_HBEAM; break;
        case eDIGOUT_LED_NEUTR: fActivated = PIN_LED_NEUTR; break;
        case eDIGOUT_LED_TURN:  fActivated = PIN_LED_TURN ; break;

        default:                fActivated = FALSE;         break;
     }
    return fActivated;
}




 /***********************************************************************
 *  FUNCTION:       DigOutDrv_SetNewState
 *  DESCRIPTION:    Setup of new PWM signal (granularity in ticks = 20 ms)
 *  PARAMETER:      ePin            index of digital output to be controlled
 *                  wOn_ms          cyclic GPO ON time in millisec (max. 3 sec.)
 *                  wOff_ms         cyclic GPO OFF time in millisec (max. 3 sec.)
 *                  wDur_ms    overall duration of PWM in millisec (max 60 s)
 *  RETURN:         error code
 *  COMMENT:        Special usage:
 *                      wOff_ms = 0 ==> 'permanent ON'
 *                      wOn_ms  = 0 ==> 'permanent OFF'
 *                      wDur_ms = 0 ==> 'never changes'
 *
 *                  If 'wDur_ms' has been set to '0' here, no further changes
 *                  will be done, until next call of this function!
 *
 *                  This function might be called repetive(!) with same parameters
 *                  (e.g. OIL-SWITCH). So we early check any changes to
 *                  prevent irrelvant setup/debugouts.
 *********************************************************************** */
ERRCODE DigOutDrv_SetNewState( DIGOUT_PINS ePin, UINT16 wOn_ms, UINT16 wOff_ms, UINT16 wDur_ms )
{
    ERRCODE RValue = ERR_OK;

    /* check: valid GPO pin index? */
    if ( ePin >= eDIGOUT_MAX )
    {   ODS1(DBG_SYS,DBG_WARNING,"DigOutDrv_SetNewState() Invalid index %u", ePin );
        RValue = ERR_OUT_OF_RANGE;
        return (RValue);
    }

    /* check: parameters inside valid area (20 ms Granularity)
       If (parameter < granularity) ==> assure at least 1x time usage!*/
    if ( ( wOn_ms  > 0 ) && ( wOn_ms < MS_PER_TICK ) )
         wOn_ms += MS_PER_TICK;
    if ( ( wOff_ms > 0 ) && ( wOn_ms < MS_PER_TICK ) )
         wOff_ms += MS_PER_TICK;
    if ( ( wDur_ms > 0 ) && ( wOn_ms < MS_PER_TICK ) )
         wDur_ms += MS_PER_TICK;

    /* check: any changes? */
    if (  ( DigOut_PWMCtrl[ePin].wOnTicks   == MS2TICKS(wOn_ms)   )
        &&( DigOut_PWMCtrl[ePin].wOffTicks  == MS2TICKS(wOff_ms)  )
        &&( DigOut_PWMCtrl[ePin].dwDurTicks == MS2TICKSL(wDur_ms) ) )
    {   //ODS1(DBG_SYS,DBG_INFO,"DigOutDrv_SetNewState() - no changes for GPO[%s]", DigOutDrv_GetPinName(ePin) );
        RValue = ERR_OK;
        return (RValue);
    }

    /* ok, save new PWM timings (assure at least one tick, intervall is < 1 tick!) */
    DigOut_PWMCtrl[ePin].wOnTicks    = MS2TICKS(wOn_ms  );
    DigOut_PWMCtrl[ePin].wOffTicks   = MS2TICKS(wOff_ms );
    DigOut_PWMCtrl[ePin].dwDurTicks  = MS2TICKSL(wDur_ms);

    /* initialize downcounting PWM timers */
    DigOut_PWMCtrl[ePin].wOnCurrTicks     = DigOut_PWMCtrl[ePin].wOnTicks;
    DigOut_PWMCtrl[ePin].wOffCurrTicks    = DigOut_PWMCtrl[ePin].wOffTicks;

    /* show internal control structure */
    ODS4(DBG_SYS,DBG_INFO,"GPO[%s]: ON:%u OFF:%u DUR:%ul (ticks)",
                            DigOutDrv_GetPinName(ePin),
                            DigOut_PWMCtrl[ePin].wOnTicks,
                            DigOut_PWMCtrl[ePin].wOffTicks,
                            DigOut_PWMCtrl[ePin].dwDurTicks);

    /* directly switch GPO to advised mode (ON/OFF, always starts with ON) */
    if ( DigOut_PWMCtrl[ePin].wOnTicks > 0)
         DigOutDrv_SetPin(ePin, DIGOUT_ON);    // switch ON  immedeately
    else DigOutDrv_SetPin(ePin, DIGOUT_OFF);   // switch OFF immedeately

    /* further PWM control is done in DigOutDrv_Service(),
       which is called with 50 Hz inside the Timer ISR */
    return RValue;
}





 /***********************************************************************
 *  FUNCTION:       DigOutDrv_Service
 *  DESCRIPTION:    Provides PWM control with execution time,
 *                  gets called from inside TimerISR
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        If duration has been set to 0 at DigOutDrv_SetNewState(),
 *                  no further change will done, until next DigOutDrv_SetNewState()
 *                  call!
 *                  Assumes to get called at every system tick (20 ms).
 *                  After PWM finished, assures to set back last GPO state (ON/OFF)
 *********************************************************************** */
ERRCODE DigOutDrv_Service(void)
{
    ERRCODE     RValue = ERR_OK;
    DIGOUT_PINS ePin;

    /* loop over available Leds */
    for ( ePin = eDIGOUT_GPO_0; ePin < eDIGOUT_LED_ERROR; ePin++ )
    {
        /* -------------------------------------------- */
        /* control only, if Duration > 0 */
        if ( DigOut_PWMCtrl[ePin].dwDurTicks > 0 )
        {
            /* count down our duration timer */
            DigOut_PWMCtrl[ePin].dwDurTicks--;

            /* right now expired? */
            if ( DigOut_PWMCtrl[ePin].dwDurTicks == 0 )
            {   /* assure GPO being off now for ever! */
                DigOutDrv_SetPin(ePin, DIGOUT_OFF);
            }
            else
            {
                /* GPO currently on? */
                if (TRUE == DigOutDrv_GetPin(ePin))
                {
                    /* OffTime available -> time to switch off again? */
                    if (DigOut_PWMCtrl[ePin].wOffTicks > 0)
                    {
                        /* count down ON timer (if possible) */
                        if (DigOut_PWMCtrl[ePin].wOnCurrTicks > 0 )
                        {   /* keep it on! */
                            DigOut_PWMCtrl[ePin].wOnCurrTicks--;
                        }
                        else
                        {   /* ok - done - switch it off! */
                            DigOutDrv_SetPin(ePin, DIGOUT_OFF);
                            /* reload off timer */
                            DigOut_PWMCtrl[ePin].wOffCurrTicks = DigOut_PWMCtrl[ePin].wOffTicks;
                        }
                    }
                    else
                    {   // nothing to do - never switch off inside duration */
                    }
                }
                /* GPO currently off? */
                else
                {
                    /* if OnTime Avaliable -> time to switch on again?*/
                    if (DigOut_PWMCtrl[ePin].wOnTicks > 0)
                    {
                        /* count down OFF timer (if possible) */
                        if (DigOut_PWMCtrl[ePin].wOffCurrTicks > 0 )
                        {   /* keep it off! */
                            DigOut_PWMCtrl[ePin].wOffCurrTicks--;
                        }
                        else
                        {   /* ok - done - switch it on again! */
                            DigOutDrv_SetPin(ePin, DIGOUT_ON);
                            /* reload off timer */
                            DigOut_PWMCtrl[ePin].wOnCurrTicks = DigOut_PWMCtrl[ePin].wOnTicks;
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
 *  FUNCTION:       DigOutDrv_GetPinName
 *  DESCRIPTION:    just a debug helper to return GPO Pin name
 *  PARAMETER:
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
STRING DigOutDrv_GetPinName( DIGOUT_PINS ePin )
{
    STRING RValue = "";
    switch (ePin)
    {
        case eDIGOUT_GPO_0:      RValue = "GPO0";        break;
        case eDIGOUT_GPO_1:      RValue = "GPO1";        break;
        case eDIGOUT_BEEP:      RValue = "BEEP";        break;
        case eDIGOUT_LED_WARN:  RValue = "eLED_WARN";    break;
        case eDIGOUT_LED_ERROR:   RValue = "LED_ERR";     break;
        case eDIGOUT_LED_INFO:  RValue = "eLED_INFO";    break;
        case eDIGOUT_LED_HBEAM:  RValue = "LED_HBEAM";   break;
        case eDIGOUT_LED_NEUTR: RValue = "eLED_NEUTR";   break;
        case eDIGOUT_LED_TURN:  RValue = "eLED_TURN";    break;

        default:                RValue = "?";           break;
    }
    return (RValue);
}


 