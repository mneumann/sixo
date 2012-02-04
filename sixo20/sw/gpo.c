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


/* global symbols */
GPOTIMINGTYPE GpoTiming[GPO_COUNT];     // GPO control array


/***********************************************************************
 *  FUNCTION:       GPOInit
 *  DESCRIPTION:    Initializes gpo service & hw driver
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE GPOInit(void)
{
    ERRCODE RValue;
    UINT8   i;

    /* reset PWM timings */
    memset( &GpoTiming, 0x00, sizeof(GPOTIMINGTYPE));

    /* reset low level HW */
    RValue = DigOutInit();

    return RValue;
}



/***********************************************************************
 *  FUNCTION:       GPOService
 *  DESCRIPTION:    Provides PWM control with execution time,
 *                  gets called from inside TimerISR
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        If duration has been set to 0 at GPOSetNewState(),
 *                  no further change will done, until next GPOSetNewState()
 *                  call!
 *                  Assumes to get called at every system tick (20 ms).
 *                  Assure the GPO to be off, after duration time (if any).
 *********************************************************************** */
ERRCODE GPOService(void)
{
    ERRCODE     RValue = ERR_OK;
    GPO_ENUM    eGpo;

    /* loop over available Gpos */
    for ( eGpo = 0; eGpo < GPO_COUNT; eGpo++ )
    {
        /* -------------------------------------------- */
        /* control only, if Duration > 0 */
        if ( GpoTiming[eGpo].dwDurTicks > 0 )
        {
            /* count down our duration timer */
            GpoTiming[eGpo].dwDurTicks--;

            /* right now expired? */
            if ( GpoTiming[eGpo].dwDurTicks == 0 )
            {   DigOutDrv_SetPin(eGpo, GPO_OFF);         /* assure GPO being off now for ever! */
            }
            else
            {
                /* GPO currently on? */
                if (TRUE == DigOutDrv_GetPin(eGpo))
                {
                    /* OffTime available -> time to switch off again? */
                    if (GpoTiming[eGpo].wOffTicks > 0)
                    {
                        /* count down ON timer (if possible) */
                        if (GpoTiming[eGpo].wOnCurrTicks > 0 )
                        {   GpoTiming[eGpo].wOnCurrTicks--;      /* keep it on! */
                        }
                        else
                        {   DigOutDrv_SetPin(eGpo, GPO_OFF);         /* ok - done - switch it off! */
                            GpoTiming[eGpo].wOffCurrTicks = GpoTiming[eGpo].wOffTicks;    /* reload off timer */
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
                    if (GpoTiming[eGpo].wOnTicks > 0)
                    {
                        /* count down OFF timer (if possible) */
                        if (GpoTiming[eGpo].wOffCurrTicks > 0 )
                        {   GpoTiming[eGpo].wOffCurrTicks--;      /* keep it off! */
                        }
                        else
                        {   DigOutDrv_SetPin(eGpo, GPO_ON);                                 /* ok - done - switch it on again! */
                            GpoTiming[eGpo].wOnCurrTicks = GpoTiming[eGpo].wOnTicks;    /* reload off timer */
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
 *  FUNCTION:       GPOSetNewState
 *  DESCRIPTION:    setup of GPO PWM signal (granularity in ticks = 20 ms)
 *  PARAMETER:      eGPO            index of GPO to be controlled
 *                  wOn_ms          cyclic GPO ON time in millisec (max.3 sec.)
 *                  wOff_ms         cyclic GPO OFF time in millisec (max.3 sec.)
 *                  wDuration_ms    overall duration of PWM in millisec (max.60 sec.)
 *  RETURN:         error code
 *  COMMENT:        wOff_ms = 0         eq. 'permanent ON'
 *                  wOn_ms = 0          eq. 'permanent OFF'
 *                  wDuration_ms = 0    eg. 'permanent ON/OFF'
 *
 *                  If 'wDuration_ms' has been set to 0 at GPOSetNewState(),
 *                  no further change will done, until next GPOSetNewState()
 *                  call!
 *
 *********************************************************************** */
ERRCODE GPOSetNewState(GPO_ENUM eGpo, UINT16 wOn_ms, UINT16 wOff_ms, UINT16 wDuration_ms )
{
    ERRCODE RValue = ERR_OK;

    /* check: valid GPO index? */
    if ( eGpo >= GPO_COUNT )
    {   ODS1(DBG_SYS,DBG_WARNING,"GPOSetNewState() Invalid index %u", eGpo );
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
    if (  ( GpoTiming[eGpo].wOnTicks    == MS2TICKS(wOn_ms)       )
        &&( GpoTiming[eGpo].wOffTicks   == MS2TICKS(wOff_ms)      )
        &&( GpoTiming[eGpo].dwDurTicks  == MS2TICKSL(wDuration_ms)) )
    {   //ODS1(DBG_SYS,DBG_INFO,"GPOSetNewState() - no changes for GPO[%s]", GPOgetName(eGpo) );
        RValue = ERR_OK;
        return (RValue);
    }

    /* ok, save new PWM timings (assure at least one tick, intervall is < 1 tick!) */
    GpoTiming[eGpo].wOnTicks    = MS2TICKS(wOn_ms      );
    GpoTiming[eGpo].wOffTicks   = MS2TICKS(wOff_ms     );
    GpoTiming[eGpo].dwDurTicks  = MS2TICKSL(wDuration_ms);

    /* initialize downcounting PWM timers */
    GpoTiming[eGpo].wOnCurrTicks     = GpoTiming[eGpo].wOnTicks;
    GpoTiming[eGpo].wOffCurrTicks    = GpoTiming[eGpo].wOffTicks;

    /* show internal control structure */
    ODS4(DBG_SYS,DBG_INFO,"GPO[%s]: ON:%u OFF:%u DUR:%u (ticks)",
                            GPOGetName(eGpo),
                            GpoTiming[eGpo].wOnTicks,
                            GpoTiming[eGpo].wOffTicks,
                            GpoTiming[eGpo].dwDurTicks);

    /* directly switch GPO to advised mode (ON/OFF, always starts with ON) */
    if ( GpoTiming[eGpo].wOnTicks > 0)
         DigOutDrv_SetPin(eGpo, GPO_ON);    // switch ON  immedeately
    else DigOutDrv_SetPin(eGpo, GPO_OFF);   // switch OFF immedeately

    /* further GPO PWM control is done in GpoService(), which is called with 50 Hz
       inside the Timer ISR */
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       GPOGetState()
 *  DESCRIPTION:    Returns current state of single GPO
 *  PARAMETER:      eGPO            GPO indicator
 *  RETURN:         fActivate       TRUE  = GPO on
 *                                  FALSE = GPO off
 *  COMMENT:        -
 *********************************************************************** */
BOOL GPOGetState( GPO_ENUM eGPO )
{
    return ( DigOutDrv_GetPin( eGPO ) );
}



/***********************************************************************
 *  FUNCTION:       GPOGetName
 *  DESCRIPTION:    just a debug helper to return GPO name
 *  PARAMETER:
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
STRING GPOGetName( GPO_ENUM eGPO )
{
    STRING RValue = "";

    switch (eGPO)
    {
        case eGPO_0: RValue = "PIN_GPO0"; break;
        case eGPO_1: RValue = "PIN_GPO1"; break;
        default         : RValue = "?"; break;
    }
    return (RValue);
}

