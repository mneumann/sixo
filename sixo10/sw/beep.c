/***********************************************************************
 *
 *  $RCSfile$
 *
 *  $Revision$
 *
 *  $Date$
 *
 *  $Author$
 *
 *  --------------------------------------------------------------------
 *  Company:        N&K Development, all rights reserved
 *
 *                  Neugebauer & Krizsan Cooperation, Germany
 *                  contact:    arnold.neugebauer@web.de
 *                              ralf.krizsan@web.de
 *  --------------------------------------------------------------------
 *  Created:        01-11-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         Beep
 *  Purpose:        Beep services (timing, auto update)
 *  Comments:
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


/* global symbols */
BEEPTIMINGTYPE BeepTiming;


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


#ifdef DEBUG
/***********************************************************************
 *  FUNCTION:       TestBeepSendMessage
 *  DESCRIPTION:    just tests timer and timer messages
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void TestBeepSendMessage(void)
{
    MESSAGE msg;

    /* set Beeper to pulse with on/off with 1 Hz */
    BEEP_MSG_MS(msg, BEEP_ON, 500, 500 );
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);

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




#endif // DEBUG
