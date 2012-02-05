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
 * Revision 3.6  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 3.5  2012/02/04 20:38:05  tuberkel
 * Moved all BeeperDriver / LEDDriver stuff ==> 'digoutdrv'
 *
 * Revision 3.4  2012/02/04 08:34:10  tuberkel
 * BugFix LED PWM
 *
 *
 ************************************************************************ */


#include <string.h>

#include "sfr62p.h"
#include "standard.h"
#include "stdmsgs.h"
#include "msgqueue.h"
#include "timer.h"
#include "digoutdr.h"
#include "led.h"
#include "debug.h"



/* global symbols */



/***********************************************************************
 *  FUNCTION:       LED_Init
 *  DESCRIPTION:    Initializes LED API
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LED_Init(void)
{
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       LED_SetBrightness
 *  DESCRIPTION:    Switches common LED current control on/off
 *  PARAMETER:      bBrightness     0..255  brightness (0=off)
 *  RETURN:         ERR_OK
 *  COMMENT:        Currently Simply enables/disables LEDs:
 *                      0:  all LEDs off
 *                      >0: all LEDs on
 *
 *  T.B.D.: Common LED Brightness Control p8_0 will have to be done with
 *          analogue signal instead of digital switch yet.
 *
 *********************************************************************** */
ERRCODE LED_SetBrightness(unsigned char bBrightness)
{
    PIN_LED_BRIGHT = (bBrightness == 0) ? 0 : 1;   // set control
    return(ERR_OK);
}



/***********************************************************************
 *  FUNCTION:       LED_SetNewState
 *  DESCRIPTION:    setup of LED PWM signal (granularity in ticks = 20 ms)
 *  PARAMETER:      eLed            index of LE to be controlled
 *                  wOn_ms          cyclic LED ON time in millisec (max. 3 sec.)
 *                  wOff_ms         cyclic LED OFF time in millisec (max. 3 sec.)
 *                  wDur_ms    overall duration of PWM in millisec (max 60 s)
 *  RETURN:         error code
 *  COMMENT:        wOff_ms = 0         eq. 'permanent ON'
 *                  wOn_ms = 0          eq. 'permanent OFF'
 *                  wDur_ms = 0    eg. 'permanent ON/OFF'
 *
 *                  If 'wDur_ms' has been set to 0 at LED_SetNewState(),
 *                  no further change will done, until next LED_SetNewState()
 *                  call!
 *
 *                  Might be called repetive with same parameters (e.g. OIL-SWITCH).
 *                  so we early check any changes to prevent irrelvant setup/debugouts.
 *********************************************************************** */
ERRCODE LED_SetNewState(LED_ENUM eLed, UINT16 wOn_ms, UINT16 wOff_ms, UINT16 wDur_ms )
{
    ERRCODE RValue = ERR_OK;

    /* check: valid LED index? */
    if (  ( eLed < eDIGOUT_LED_NEUTR )
        ||( eLed > eDIGOUT_LED_ERROR   ) )
    {   ODS1(DBG_SYS,DBG_WARNING,"LED_SetNewState() Invalid index %u", eLed );
        RValue = ERR_OUT_OF_RANGE;
    }
    else
    {   /* ok - now map to the driver function */
        RValue = DigOutDrv_SetNewState( eLed, wOn_ms, wOff_ms, wDur_ms );
    }
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       LED_GetState()
 *  DESCRIPTION:    Returns current state of single LED
 *  PARAMETER:      eLED            LED indicator
 *  RETURN:         fActivate       TRUE  = LED on
 *                                  FALSE = LED off
 *  COMMENT:        -
 *********************************************************************** */
BOOL LED_GetState( LED_ENUM eLed )
{
    BOOL RValue;

    /* check: valid LED index? */
    if (  ( eLed < eDIGOUT_LED_NEUTR )
        ||( eLed > eDIGOUT_LED_ERROR   ) )
    {   ODS1(DBG_SYS,DBG_WARNING,"LED_GetState() Invalid index %u", eLed );
        RValue = FALSE;
    }
    else
    {   /* ok - now map to the driver function */
        RValue = DigOutDrv_GetPin( eLed );
    }
    return (RValue);
}




/***********************************************************************
 *  FUNCTION:       LED_SignalOk
 *  DESCRIPTION:    let the info led be an acknowledge:
 *                  600 ms permanent ON
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        If LED already on: do not start OK-Signal!
 *********************************************************************** */
void LED_SignalOk(void)
{
    /* prevent usage, if INFO led already on! */
    if ( FALSE == LED_GetState( eLED_INFO ) )
    {
        /* set Info-LED ON now! (and later off) */
        LED_SetNewState( eLED_INFO, 1, 0, 600 );
    }
}



/***********************************************************************
 *  FUNCTION:       LED_SignalEsc
 *  DESCRIPTION:    let the info led be an error indicator:
 *                  1000 ms duration pulsed with 100 ms ON / 100 ms OFF
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void LED_SignalEsc(void)
{
    /* prevent usage, if INFO led already on! */
    if ( FALSE == LED_GetState( eLED_INFO ) )
    {
        /* pulse LED now! */
        LED_SetNewState( eLED_INFO, 100, 100, 1000 );
    }
}


/***********************************************************************
 *  FUNCTION:       LED_Test
 *  DESCRIPTION:
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
#if(TESTLED==1)
void LED_Test(unsigned int loopdelay)
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
