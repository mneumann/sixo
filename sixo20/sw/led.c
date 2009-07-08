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
 ************************************************************************ */


#include "standard.h"
#include "stdmsgs.h"
#include "msgqueue.h"
#include "timer.h"
#include "leddrv.h"
#include "led.h"
#include "debug.h"
#include "digindrv.h"

/* global symbols */
LEDTIMINGTYPE rgLedTiming[LEDDRV_MAX];     // LED control array


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

    /* reset timings */
    for (i=0; i<LEDDRV_MAX; i++)
    {
        rgLedTiming[i].wOnTicks  = 0;
        rgLedTiming[i].wOffTicks = 0;
    }

    /* reset low level HW */
    RValue = LEDDrvInit(FALSE);  // TRUE: leds 'flash' one time

    return RValue;
}

/***********************************************************************
 *  FUNCTION:       LEDMsgEntry
 *  DESCRIPTION:    Receive Message Handler of LED service
 *                  called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LEDMsgEntry(MESSAGE msg)
{
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID  MsgId;

    /* analyze message: it's for us? */
    MsgId = MSG_ID(msg);        /* get message id */
    switch (MsgId)
    {
        case MSG_LED_SET:
        {
            //ODS(DBG_SYS,DBG_INFO,"MSG_LED_SET received!");
            LEDSetNewState(msg);
            RValue = ERR_MSG_PROCESSED;
        } break;
        case MSG_LED_ON:
        {
            //ODS(DBG_SYS,DBG_INFO,"MSG_LED_ON received!");
            LEDSetNewState(msg);
            RValue = ERR_MSG_PROCESSED;
        } break;
        case MSG_LED_OFF:
        {
            //ODS(DBG_SYS,DBG_INFO,"MSG_LED_OFF received!");
            LEDSetNewState(msg);
            RValue = ERR_MSG_PROCESSED;
        } break;
        default: RValue = ERR_MSG_NOT_PROCESSED;
    }

#if(DEBUG==1)
    /* DebugPurpose: */
    // TestCheckKeyStateMsgs(msg);
#endif
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       LEDSetNewState
 *  DESCRIPTION:    handles new led status of any led
 *  PARAMETER:      MESSAGE     msg with all parameters
 *  RETURN:         error code
 *  COMMENT:        wOffTicks = 0    eq. 'permanent ON'
 *                  wOnTicks = 0     eq. 'permanent OFF'
 *********************************************************************** */
ERRCODE LEDSetNewState(MESSAGE GivenMsg)
{
    MESSAGE     NewMsg;
    ERRCODE     RValue = ERR_OK;
    MESSAGE_ID  MsgId;

    /* get parameters out of message */
    LEDDRV_LEDS eLed   = MSG_CHAR1(GivenMsg)&0x0f;          /* lower nibble eq. led index */
    BOOL        fMode = (MSG_CHAR1(GivenMsg)&0xf0) >> 4;    /* higher nibble eq. led mode */;

    /* check parameters */
    if (eLed >= LEDDRV_MAX)
    {
        ODS1(DBG_SYS,DBG_ERROR,"MSG_LED_X with invalid parameter (led:%d)", eLed);
        return ERR_PARAM_ERR;
    }
    /* analyze message: ON/OFF */
    MsgId = MSG_ID(GivenMsg);        /* get message id */

    switch (MsgId)
    {
        case MSG_LED_SET:
        {
            /* save led timings */
            rgLedTiming[eLed].wOnTicks  = MSG_CHAR2(GivenMsg);
            rgLedTiming[eLed].wOffTicks = MSG_CHAR3(GivenMsg);

            /* directly switch led to advised mode (ON/OFF/GREEN/RED) */
            LEDDrvSetLED(eLed, fMode);

            /* check led state for further messages */
            if (  (fMode == LED_ON)
                &&(rgLedTiming[eLed].wOffTicks  >  0) )
            {
                /* build msg to later switch OFF the red/green led */
                MSG_BUILD_UINT8(NewMsg, MSG_LED_OFF, (eLed | (fMode<<4)), 0, 0);
                /* switch OFF after OnTime */
                RValue = SetTimerMsg(NewMsg, rgLedTiming[eLed].wOnTicks);
            }
        } break;
        case MSG_LED_ON:
        {
            /* led has not been disabled (wOnTicks==0) inbetween? */
            if (rgLedTiming[eLed].wOnTicks > 0)
            {
                /* switch on NOW! */
                LEDDrvSetLED(eLed, fMode);

                /* permanent on? (wOffTicks==0) */
                if (rgLedTiming[eLed].wOffTicks > 0)
                {
                    /* build msg to later switch OFF the red/green led */
                    MSG_BUILD_UINT8(NewMsg, MSG_LED_OFF, (eLed | (fMode<<4)), 0, 0);
                    /* switch OFF after OnTime */
                    RValue = SetTimerMsg(NewMsg, rgLedTiming[eLed].wOnTicks);
                }
            }
        } break;
        case (MSG_LED_OFF):
        {
            /* led has not been permanent enabled (wOffTicks==0) inbetween? */
            if (rgLedTiming[eLed].wOffTicks > 0)
            {
                /* switch off NOW! */
                LEDDrvSetLED(eLed, LED_OFF);

                /* permanent off? (wOnTicks==0) */
                if (rgLedTiming[eLed].wOnTicks > 0)
                {
                    /* build msg to later switch ON the red/green led */
                    MSG_BUILD_UINT8(NewMsg, MSG_LED_ON, (eLed | (fMode<<4)), 0, 0);
                    /* switch ON after OffTime */
                    RValue = SetTimerMsg(NewMsg, rgLedTiming[eLed].wOffTicks);
                }
            }
        } break;
        default: break;
    }
    if (RValue != ERR_OK)
        ODS(DBG_SYS,DBG_ERROR,"Unable to send new MSG_LED_ON/OFF!");
    return RValue;
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
    MESSAGE msg;

    LED_MSG_MS(msg, LEDDRV_INFO, LED_ON, 1, 0);  /* set led ON now! */
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);
    LED_MSG_MS(msg, LEDDRV_INFO, LED_OFF, 0, 1); /* switch off led .. */
    SetTimerMsg(msg, MS2TICKS(600));            /* ... after 600 ms */
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
    MESSAGE msg;

    LED_MSG_MS(msg, LEDDRV_INFO, LED_ON, 100, 100); /* led blinks now! */
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);
    LED_MSG_MS(msg, LEDDRV_INFO, LED_OFF, 0, 1);    /* switch off led ..*/
    SetTimerMsg(msg, MS2TICKS(1000));              /* ... after 1000 ms */
}




#if(TESTLED==1)
/***********************************************************************
 *  FUNCTION:       TestLEDSendMessage
 *  DESCRIPTION:    just tests timer and timer messages
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void TestLEDSendMessage(void)
{
    MESSAGE msg;

    /* set InfoLed to blink with 1 Hz for 20 sec.*/
    LED_MSG_MS(msg, LED_INF, LED_ON, 100, 900 );
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);

    LED_MSG_TICKS(msg, LED_INF, LED_OFF, 0, 0);
    SetTimerMsg(msg, SEC2TICKS(20));

    /* set Warning led to blink with 0,1 Hz */
    LED_MSG_SEC(msg, LED_TURN, LED_ON, 1, 4);
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);
    LED_MSG_TICKS(msg, LED_TURN, LED_OFF, 0, 0);
    SetTimerMsg(msg, SEC2TICKS(20));

    /* set Error led to permanent ON */
    LED_MSG_TICKS(msg, LED_ERR, LED_ON, 1, 0);
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);
    LED_MSG_TICKS(msg, LED_ERR, LED_OFF, 0, 0);
    SetTimerMsg(msg, SEC2TICKS(20));

    /* set Beamer led to permanent fastest blink mode after 10 sec.*/
    LED_MSG_TICKS(msg, LED_BEAM, LED_ON, 1, 1);
    SetTimerMsg(msg, SEC2TICKS(10));
    LED_MSG_TICKS(msg, LED_BEAM, LED_OFF, 0, 0);
    SetTimerMsg(msg, SEC2TICKS(20));
}




/***********************************************************************
 *  FUNCTION:       TestCheckKeyStateMsgs
 *  DESCRIPTION:    Just to let a led flicker if a key is pressed
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void TestCheckKeyStateMsgs(MESSAGE msg)
{
    MESSAGE_ID  MsgId;
    UINT16      wDuration;
    MESSAGE     NewMsg;

    MsgId = MSG_ID(msg);        /* get message id */
    switch (MsgId)
    {
        case MSG_KEY_OK:
            switch (MSG_KEY_TRANSITION(msg))
            {
                case KEYTRANS_PRESSED:
                case KEYTRANS_ON:
                    LED_MSG_TICKS(NewMsg, LED_NEUTR, LED_ON, 1, 0);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    break;
                case KEYTRANS_RELEASED:
                    LED_MSG_TICKS(NewMsg, LED_NEUTR, LED_OFF, 0, 0);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    break;
                default: break;
            }
            break;
        case MSG_KEY_UP:
            switch (MSG_KEY_TRANSITION(msg))
            {
                case KEYTRANS_PRESSED:
                case KEYTRANS_ON:
                    LED_MSG_TICKS(NewMsg, LED_WARN, LED_ON, 1, 0);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    break;
                case KEYTRANS_RELEASED:
                    LED_MSG_TICKS(NewMsg, LED_WARN, LED_OFF, 0, 0);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    break;
                default: break;
            }
            break;
        case MSG_KEY_DOWN:
            switch (MSG_KEY_TRANSITION(msg))
            {
                case KEYTRANS_PRESSED:
                case KEYTRANS_ON:
                    LED_MSG_TICKS(NewMsg, LED_INF, LED_ON, 1, 0);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    break;
                case KEYTRANS_RELEASED:
                    LED_MSG_TICKS(NewMsg, LED_INF, LED_OFF, 0, 0);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    break;
                default: break;
            }
            break;
        default:
            break;
    }
}

#endif // TESTLED
