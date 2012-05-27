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
 * Revision 3.8  2012/05/27 16:01:36  tuberkel
 * All Eeprom/Nvram Variables renamed
 *
 * Revision 3.7  2012/02/11 09:12:06  tuberkel
 * BugFix: Beeper use donly if enbaled by user
 * New: Beep_SignalAck()
 *
 * Revision 3.6  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 3.5  2012/02/04 21:49:42  tuberkel
 * All BeeperDriver functions mapped ==> DigOutDrv()
 *
 * Revision 3.4  2012/02/04 20:38:05  tuberkel
 * Moved all BeeperDriver / BEEPDriver stuff ==> 'digoutdrv'
 *
 * Revision 3.3  2012/01/14 10:26:44  tuberkel
 * BEEP PWM handling changed:
 * - no longer Msgs/TimerMsgs used (inaccurate optic)
 * - instead use TimerISR to control PWM
 * - Granulartiy is SystemTicks (20 ms)
 * - works well
 * - prevent direct BEEPDrv access (if possible)
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

/* external symbols */
extern DEVFLAGS2_TYPE   EE_DevFlags_2;  // device parameters



/***********************************************************************
 *  FUNCTION:       Beep_Init
 *  DESCRIPTION:    Initializes Beep API
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE Beep_Init(void)
{
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       Beep_SetNewState
 *  DESCRIPTION:    setup of BEEP PWM signal (granularity in ticks = 20 ms)
 *  PARAMETER:      wOn_ms      cyclic BEEP ON time in millisec (max. 3 sec.)
 *                  wOff_ms     cyclic BEEP OFF time in millisec (max. 3 sec.)
 *                  wDur_ms     overall duration of PWM in millisec (max 60 s)
 *  RETURN:         error code
 *  COMMENT:        wOff_ms = 0         eq. 'permanent ON'
 *                  wOn_ms = 0          eq. 'permanent OFF'
 *                  wDur_ms = 0    eg. 'permanent ON/OFF'
 *
 *                  If 'wDur_ms' has been set to 0 at Beep_SetNewState(),
 *                  no further change will done, until next Beep_SetNewState()
 *                  call!
 *
 *                  Might be called repetive with same parameters (e.g. OIL-SWITCH).
 *                  so we early check any changes to prevent irrelvant setup/debugouts.
 *********************************************************************** */
ERRCODE Beep_SetNewState(UINT16 wOn_ms, UINT16 wOff_ms, UINT16 wDur_ms )
{
    ERRCODE RValue = ERR_OK;

    /* Beeper enabled by user */
    if ( EE_DevFlags_2.flags.BeeperAvail == TRUE )
    {
        /* ok - now map to the driver function */
        RValue = DigOutDrv_SetNewState( eDIGOUT_BEEP, wOn_ms, wOff_ms, wDur_ms );
    }
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       Beep_GetState()
 *  DESCRIPTION:    Returns current state of Beeper
 *  PARAMETER:      -
 *  RETURN:         fActivate       TRUE  = BEEP on
 *                                  FALSE = BEEP off
 *  COMMENT:        -
 *********************************************************************** */
BOOL Beep_GetState( void )
{
    ERRCODE RValue;

    /* ok - now map to the driver function */
    RValue = DigOutDrv_GetPin( eDIGOUT_BEEP );
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       Beep_SignalOk
 *  DESCRIPTION:    creates an 'OK'-Beep: 600 ms ON
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void Beep_SignalOk(void)
{
    /* Beeper enabled by user */
    if ( EE_DevFlags_2.flags.BeeperAvail == TRUE )
    {
        /* setup Beeper PWM! */
        Beep_SetNewState(1, 0, 600 );
    }
}


/***********************************************************************
 *  FUNCTION:       Beep_SignalAck
 *  DESCRIPTION:    creates a short 'Ack'-Beep: 300 ms ON
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void Beep_SignalAck(void)
{
    /* Beeper enabled by user */
    if ( EE_DevFlags_2.flags.BeeperAvail == TRUE )
    {
        /* setup Beeper PWM! */
        Beep_SetNewState(1, 0, 300 );
    }
}



/***********************************************************************
 *  FUNCTION:       Beep_SignalEsc
 *  DESCRIPTION:    creates an 'Esc'-Beep: 1000 ms duration pulsed
 *                  with 50 ms ON / 50 ms OFF
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void Beep_SignalEsc(void)
{
    /* Beeper enabled by user */
    if ( EE_DevFlags_2.flags.BeeperAvail == TRUE )
    {
        /* setup Beeper PWM! */
        Beep_SetNewState( 50, 50, 1000 );
    }
}



/***********************************************************************
 *  FUNCTION:       Beep_SignalClick
 *  DESCRIPTION:    simulates a very short click: 20 ms permanent ON
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void Beep_SignalClick(void)
{
    /* make the shortest possible pulse */
    Beep_SetNewState( 1, 0, 20 );
}



/***********************************************************************
 *  FUNCTION:       Beep_Test
 *  DESCRIPTION:    just tests timer and timer messages
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Sends a few messages wich will activate a
 *                  sequence of beeps.
 *********************************************************************** */
#if(TESTBEEP==1)
void Beep_Test(void)
{
    MESSAGE msg;

     /* make the shortest possible pulse */
    Beep_SetNewState(  1, 0, 20 );
    Delay_ms(2000);

   /* set Beeper to pulse with on/off with 1 Hz for 5 sec.*/
    Beep_SetNewState(500, 500, 5000 );
    Delay_ms(2000);

    /* set Beeper to pulse with on/off with 5 Hz for 5 sec.*/
    Beep_SetNewState( 100, 100, 5000 );
    Delay_ms(2000);

}
#endif // TESTBEEP



