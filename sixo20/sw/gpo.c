/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2012-02-01 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         GPO
 *  Purpose:        GPO services (timing, auto update)
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
 * Revision 1.7  2012/02/11 12:21:45  tuberkel
 * dedicated COOLRIDE macros prepared & used
 *
 * Revision 1.6  2012/02/05 11:58:01  tuberkel
 * Coolride Makros adapted
 *
 * Revision 1.5  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 1.4  2012/02/04 22:25:49  tuberkel
 * LEDs renamed
 *
 * Revision 1.3  2012/02/04 21:49:42  tuberkel
 * All BeeperDriver functions mapped ==> DigOutDrv()
 *
 * Revision 1.2  2012/02/04 08:33:37  tuberkel
 * BugFix DigOut PWM
 *
 *
 ************************************************************************ */

#include <string.h>

#include "standard.h"
#include "stdmsgs.h"
#include "msgqueue.h"
#include "timer.h"
#include "digoutdr.h"
#include "gpo.h"
#include "debug.h"
#include "sysparam.h"


/* global symbols */





/***********************************************************************
 *  FUNCTION:       GPO_Init
 *  DESCRIPTION:    Initializes GPO API
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE GPO_Init(void)
{
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       GPO_SetNewState
 *  DESCRIPTION:    setup of GPO PWM signal (granularity in ticks = 20 ms)
 *  PARAMETER:      eGPO            index of GPO to be controlled
 *                  wOn_ms          cyclic GPO ON time in millisec (max.3 sec.)
 *                  wOff_ms         cyclic GPO OFF time in millisec (max.3 sec.)
 *                  wDur_ms    overall duration of PWM in millisec (max.60 sec.)
 *  RETURN:         error code
 *  COMMENT:        wOff_ms = 0         eq. 'permanent ON'
 *                  wOn_ms = 0          eq. 'permanent OFF'
 *                  wDur_ms = 0    eg. 'permanent ON/OFF'
 *
 *                  If 'wDur_ms' has been set to 0 at GPO_SetNewState(),
 *                  no further change will done, until next GPO_SetNewState()
 *                  call!
 *
 *********************************************************************** */
ERRCODE GPO_SetNewState(GPO_ENUM eGpo, UINT16 wOn_ms, UINT16 wOff_ms, UINT16 wDur_ms )
{
    ERRCODE RValue = ERR_OK;

    /* check: valid LED index? */
    if (  ( eGpo < eDIGOUT_GPO_0 )
        ||( eGpo > eDIGOUT_GPO_1 ) )
    {   ODS1(DBG_SYS,DBG_WARNING,"GPO_SetNewState() Invalid index %u", eGpo );
        RValue = ERR_OUT_OF_RANGE;
    }
    else
    {   /* ok - now map to the driver function */
        RValue = DigOutDrv_SetNewState( eGpo, wOn_ms, wOff_ms, wDur_ms );
    }
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       GPO_GetState()
 *  DESCRIPTION:    Returns current state of single GPO
 *  PARAMETER:      eGPO            GPO indicator
 *  RETURN:         fActivate       TRUE  = GPO on
 *                                  FALSE = GPO off
 *  COMMENT:        -
 *********************************************************************** */
BOOL GPO_GetState( GPO_ENUM eGpo )
{
    BOOL RValue;

    /* check: valid LED index? */
    if (  ( eGpo < eDIGOUT_GPO_0 )
        ||( eGpo > eDIGOUT_GPO_1 ) )
    {   ODS1(DBG_SYS,DBG_WARNING,"GPO_SetNewState() Invalid index %u", eGpo );
        RValue = FALSE;
    }
    else
    {
        RValue = DigOutDrv_GetPin(eGpo);
    }

    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       GPO_SignalCoolRide
 *  DESCRIPTION:    Special handler to generate a 'KeyPress-Signal'
 *                  for 'Coolride' heat control unit.
 *  PARAMETER:      eGPO            GPO indicator
 *  RETURN:         fActivate       TRUE  = GPO on
 *                                  FALSE = GPO off
 *  COMMENT:        CoolRide Control Key-Input requires
 *                      - at least 100 ms ON
 *                      - at least 380 ms OFF
 *********************************************************************** */
ERRCODE GPO_SignalCoolRide( void )
{
    ERRCODE RValue;
    RValue = DigOutDrv_SetNewState( COOLR_KEYOUT_PORT, COOLR_KEYOUT_SIGNAL );
    return (RValue);
}



