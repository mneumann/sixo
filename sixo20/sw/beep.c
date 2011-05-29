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



#include "standard.h"
#include "stdmsgs.h"
#include "msgqueue.h"
#include "timer.h"
#include "beepdrv.h"
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

    /* reset timings */
    BeepTiming.wOnTicks  = 0;
    BeepTiming.wOffTicks = 0;

    /* reset hw */
    RValue = BeepDrvInit();
    return RValue;
}


/***********************************************************************
 *  FUNCTION:       BeepMsgEntry
 *  DESCRIPTION:    Receive Message Handler of Beep service
 *                  called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE BeepMsgEntry(MESSAGE msg)
{
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID  MsgId;

    /* analyze message: it's for us? */
    MsgId = MSG_ID(msg);        /* get message id */
    switch (MsgId)
    {
        case MSG_BEEP_SET:
        {
            //ODS(DBG_SYS,DBG_INFO,"MSG_BEEP_SET received!");
            BeepSetNewState(msg);
            RValue = ERR_MSG_PROCESSED;
        } break;
        case MSG_BEEP_ON:
        {
            //ODS(DBG_SYS,DBG_INFO,"MSG_BEEP_ON received!");
            BeepSetNewState(msg);
            RValue = ERR_MSG_PROCESSED;
        } break;
        case MSG_BEEP_OFF:
        {
            //ODS(DBG_SYS,DBG_INFO,"MSG_BEEP_OFF received!");
            BeepSetNewState(msg);
            RValue = ERR_MSG_PROCESSED;
        } break;
        default: RValue = ERR_MSG_NOT_PROCESSED;
    }
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       BeepSetNewState
 *  DESCRIPTION:    handles new Beeper status
 *  PARAMETER:      MESSAGE     msg with all parameters
 *  RETURN:         error code
 *  COMMENT:        wOffTicks = 0    eq. 'permanent ON'
 *                  wOnTicks = 0     eq. 'permanent OFF'
 *********************************************************************** */
ERRCODE BeepSetNewState(MESSAGE GivenMsg)
{
    MESSAGE         NewMsg;
    ERRCODE         RValue = ERR_OK;
    MESSAGE_ID      MsgId;
    BOOL            fMode;

    // check: beeper disabled by user/eeprom settings?
    if ( gDeviceFlags2.flags.BeeperAvail == FALSE )
    {
        // do nothing, just ignore message
        //ODS(DBG_SYS,DBG_WARNING,"Unable to use MSG_BEEP_ON/OFF,  beeper disabled!");
        RValue = ERR_OK;
        return (RValue);
    }

    /* analyse message: ON/OFF */
    MsgId = MSG_ID(GivenMsg);       /* get message id */

    switch (MsgId)
    {
        case MSG_BEEP_SET:
        {
            /* save new beep timings */
            BeepTiming.wOnTicks  = MSG_CHAR2(GivenMsg);
            BeepTiming.wOffTicks = MSG_CHAR3(GivenMsg);
            fMode = MSG_CHAR1(GivenMsg);                    /* get on/off mode */

            /* directly switch beep to advised mode (ON/OFF) */
            BeepDrvSetBeeper(fMode);

            /* check beeper state for further messages */
            if (  (fMode == BEEP_ON)                        /* beeper now ON */
                &&(BeepTiming.wOffTicks > 0) )              /* AND later OFF? */
            {
                /* build msg to later switch OFF the beeper */
                MSG_BUILD_UINT8(NewMsg, MSG_BEEP_OFF, 0, 0, 0);
                /* switch OFF after OnTime */
                RValue = SetTimerMsg(NewMsg, BeepTiming.wOnTicks);
            }
            if (  (fMode == BEEP_OFF)                       /* beeper now OFF */
                &&(BeepTiming.wOnTicks > 0) )               /* AND later ON? */
            {
                /* build msg to later switch ON the beeper */
                MSG_BUILD_UINT8(NewMsg, MSG_BEEP_ON, 0, 0, 0);
                /* switch ON after OffTime */
                RValue = SetTimerMsg(NewMsg, BeepTiming.wOffTicks);
            }
        } break;
        case MSG_BEEP_ON:
        {
            /* beeper has not been disabled (wOnTicks==0) inbetween? */
            if (BeepTiming.wOnTicks > 0)
            {
                /* switch on NOW! */
                BeepDrvSetBeeper(BEEP_ON);

                /* permanent on? (wOffTicks==0) */
                if (BeepTiming.wOffTicks > 0)
                {
                    /* build msg to later switch OFF the beeper */
                    MSG_BUILD_UINT8(NewMsg, MSG_BEEP_OFF, 0, 0, 0);
                    /* switch OFF after OnTime */
                    RValue = SetTimerMsg(NewMsg, BeepTiming.wOnTicks);
                }
            }
        } break;
        case (MSG_BEEP_OFF):
        {
            /* beeper has not been permanent enabled (wOffTicks==0) inbetween? */
            if (BeepTiming.wOffTicks > 0)
            {
                /* switch off NOW! */
                BeepDrvSetBeeper(BEEP_OFF);

                /* permanent off? (wOnTicks==0) */
                if (BeepTiming.wOnTicks > 0)
                {
                    /* build msg to later switch ON the beeper */
                    MSG_BUILD_UINT8(NewMsg, MSG_BEEP_ON, 0, 0, 0);
                    /* switch ON after OffTime */
                    RValue = SetTimerMsg(NewMsg, BeepTiming.wOffTicks);
                }
            }
        } break;
        default: break;
    }
    if (RValue != ERR_OK)
        ODS(DBG_SYS,DBG_ERROR,"Unable to send new MSG_BEEP_ON/OFF!");
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
    MESSAGE msg;

    /* set Beeper ON now! */
    BEEP_MSG_MS(msg, BEEP_ON, 1, 0 );
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);

    /* limit Beeper output to 600 ms */
    BEEP_MSG_MS(msg, BEEP_OFF, 0, 1);
    SetTimerMsg(msg, MS2TICKS(600));
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
    MESSAGE msg;

    /* pulse Beeper now! */
    BEEP_MSG_MS(msg, BEEP_ON, 50, 50 );
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);

    /* limit Beeper output to 1000 ms */
    BEEP_MSG_MS(msg, BEEP_OFF, 0, 1);
    SetTimerMsg(msg, MS2TICKS(1000));
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
    MESSAGE msg;

    /* make the shortest possible pulse */
    BEEP_MSG_TICKS(msg, BEEP_ON, 1, 10 );
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);

    /* limit Beeper output to 2 ticks */
    BEEP_MSG_MS(msg, BEEP_OFF, 0, 1);
    SetTimerMsg(msg, 2);
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
    BEEP_MSG_TICKS(msg, BEEP_ON, 1, 10 );
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);

    /* limit Beeper output to 2 ticks */
    BEEP_MSG_MS(msg, BEEP_OFF, 0, 1);
    SetTimerMsg(msg, 2);

   /* set Beeper to pulse with on/off with 1 Hz */
    BEEP_MSG_MS(msg, BEEP_ON, 500, 500 );
    SetTimerMsg(msg, SEC2TICKS(2));

    /* set Beeper to permanent ON beep after 5 sec.*/
    BEEP_MSG_TICKS(msg, BEEP_ON, 1, 0);
    SetTimerMsg(msg, SEC2TICKS(5));

    /* set Beeper to pulse with on/off with 5 Hz after 10 sec.*/
    BEEP_MSG_MS(msg, BEEP_ON, 100, 100 );
    SetTimerMsg(msg, SEC2TICKS(10));

    /* set Beeper to permanent OFF beep after 15 sec.*/
    BEEP_MSG_TICKS(msg, BEEP_OFF, 0, 1);
    SetTimerMsg(msg, SEC2TICKS(15));
}
#endif // TESTBEEP



