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
 *  Module:         LED
 *  Purpose:        led services (timing, auto update)
 *  Comments:       -
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
LEDTIMINGTYPE rgLedTiming[NUMBER_LEDS];


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
    for (i=0; i<NUMBER_LEDS; i++)
    {
        rgLedTiming[i].wOnTicks  = 0;
        rgLedTiming[i].wOffTicks = 0;
    }
    /* reset hw */
    RValue = LEDDrvInit();
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

#ifdef DEBUG
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
    LED_ENUMTYPE    bLed   = MSG_CHAR1(GivenMsg)&0x0f;          /* lower nibble eq. led index */
    LED_MODETYPE    bMode = (MSG_CHAR1(GivenMsg)&0xf0) >> 4;    /* higher nibble eq. led mode */;

    /* check parameters */
    if ((bLed > LED3) || (bMode > LED_RED) )
    {
        ODS2(DBG_SYS,DBG_ERROR,"MSG_LED_X with invalid parameter(s) (led:%d state:%d)", bLed, bMode);
        return ERR_PARAM_ERR;
    }
    /* analyze message: ON/OFF */
    MsgId = MSG_ID(GivenMsg);        /* get message id */

    switch (MsgId)
    {
        case MSG_LED_SET:
        {
            /* save led timings */
            rgLedTiming[bLed].wOnTicks  = MSG_CHAR2(GivenMsg);
            rgLedTiming[bLed].wOffTicks = MSG_CHAR3(GivenMsg);

            /* directly switch led to advised mode (ON/OFF/GREEN/RED) */
            LEDDrvSetLED(bLed, bMode);

            /* check led state for further messages */
            if (  (bMode != LED_OFF)
                &&(rgLedTiming[bLed].wOffTicks  >  0) )
            {
                /* build msg to later switch OFF the red/green led */
                MSG_BUILD_UINT8(NewMsg, MSG_LED_OFF, (bLed | (bMode<<4)), 0, 0);
                /* switch OFF after OnTime */
                RValue = SetTimerMsg(NewMsg, rgLedTiming[bLed].wOnTicks);
            }
        } break;
        case MSG_LED_ON:
        {
            /* led has not been disabled (wOnTicks==0) inbetween? */
            if (rgLedTiming[bLed].wOnTicks > 0)
            {
                /* switch on NOW! */
                LEDDrvSetLED(bLed, bMode);

                /* permanent on? (wOffTicks==0) */
                if (rgLedTiming[bLed].wOffTicks > 0)
                {
                    /* build msg to later switch OFF the red/green led */
                    MSG_BUILD_UINT8(NewMsg, MSG_LED_OFF, (bLed | (bMode<<4)), 0, 0);
                    /* switch OFF after OnTime */
                    RValue = SetTimerMsg(NewMsg, rgLedTiming[bLed].wOnTicks);
                }
            }
        } break;
        case (MSG_LED_OFF):
        {
            /* led has not been permanent enabled (wOffTicks==0) inbetween? */
            if (rgLedTiming[bLed].wOffTicks > 0)
            {
                /* switch off NOW! */
                LEDDrvSetLED(bLed, LED_OFF);

                /* permanent off? (wOnTicks==0) */
                if (rgLedTiming[bLed].wOnTicks > 0)
                {
                    /* build msg to later switch ON the red/green led */
                    MSG_BUILD_UINT8(NewMsg, MSG_LED_ON, (bLed | (bMode<<4)), 0, 0);
                    /* switch ON after OffTime */
                    RValue = SetTimerMsg(NewMsg, rgLedTiming[bLed].wOffTicks);
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
 *  DESCRIPTION:    let the white led be an acknowledge:
 *                  600 ms permanent ON
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        #ifdef HW_LEDRYW:   - uses white LED for BMW version
 *                  #else               - uses lower led in green mode
 *********************************************************************** */
void LEDOk(void)
{
    MESSAGE msg;

#ifdef HW_LEDRYW /* WE USE WHITE OK LED */
    LED_MSG_MS(msg, LED_FLASH, LED_ON, 1, 0);       /* set led ON now! */
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);
    LED_MSG_MS(msg, LED_FLASH, LED_OFF, 0, 1);      /* switch off led after 600 ms */
    SetTimerMsg(msg, MS2TICKS(600));
#else            /* WE USE GREEN DUAL LED */
    LED_MSG_MS(msg, LED1, LED_GREEN, 1, 0);         /* set led ON now! */
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);
    LED_MSG_MS(msg, LED1, LED_OFF, 0, 1);           /* switch off led after 600 ms */
    SetTimerMsg(msg, MS2TICKS(600));
#endif
}

/***********************************************************************
 *  FUNCTION:       LEDEsc
 *  DESCRIPTION:    let the red led be an error indicator:
 *                  1000 ms duration pulsed with 100 ms ON / 100 ms OFF
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        #ifdef HW_LEDRYW: - uses red LED for BMW version
 *                  #else             - uses lower led in red mode
 *********************************************************************** */
void LEDEsc(void)
{
    MESSAGE msg;

#ifdef HW_LEDRYW /* WE USE RED ESC LED */
    LED_MSG_MS(msg, LED_OIL, LED_ON, 100, 100);         /* set led ON now! */
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);
    LED_MSG_MS(msg, LED_OIL, LED_OFF, 0, 1);            /* switch off led after 1000 ms */
    SetTimerMsg(msg, MS2TICKS(1000));
#else            /* WE USE RED DUAL LED */
    LED_MSG_MS(msg, LED1, LED_RED, 100, 100);           /* set led ON now! */
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);
    LED_MSG_MS(msg, LED1, LED_OFF, 0, 1);               /* switch off led after 1000 ms */
    SetTimerMsg(msg, MS2TICKS(1000));
#endif
}




#ifdef DEBUG
/***********************************************************************
 *  FUNCTION:       TestLEDSendMessage
 *  DESCRIPTION:    just tests timer and timer messages
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void TestLEDSendMessage(void)
{
//#ifdef LEDTESTMESSAGE
    MESSAGE msg;

    /* set led1 to blink red on/off with 1 Hz */
    LED_MSG_MS(msg, LED1, LED_RED, 100, 900 );
    //MSG_BUILD_UINT8(msg, MSG_LED_SET, (LED1 | (LED_RED<<4)), MS2TICKS(100),MS2TICKS(900) );
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);
    /* set led2 to blink green on/off with 0,1 Hz */
    LED_MSG_SEC(msg, LED2, LED_GREEN, 1, 4);
    //MSG_BUILD_UINT8(msg, MSG_LED_SET, (LED2 | (LED_GREEN<<4)), SEC2TICKS(1), SEC2TICKS(4) );
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);
    /* set led3 to permanenrt ON */
    LED_MSG_TICKS(msg, LED3, LED_RED, 1, 0);
    //MSG_BUILD_UINT8(msg, MSG_LED_SET, (LED3 | (LED_RED<<4)), 1, 0);
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);
    /* set led3 to permanent fastest blink mode after 10 sec.*/
    LED_MSG_TICKS(msg, LED3, LED_RED, 1, 1);
    //MSG_BUILD_UINT8(msg, MSG_LED_SET, (LED3 | (LED_RED<<4)), 1, 1);
    SetTimerMsg(msg, SEC2TICKS(10));
    /* set led3 to OFF after 20 sec.*/
    LED_MSG_TICKS(msg, LED3, LED_OFF, 0, 0);
    //MSG_BUILD_UINT8(msg, MSG_LED_SET, (LED3 | (LED_OFF<<4)), 0, 0);
    SetTimerMsg(msg, SEC2TICKS(20));
    /* set led2 to OFF after 20 sec.*/
    LED_MSG_TICKS(msg, LED2, LED_OFF, 0, 0);
    //MSG_BUILD_UINT8(msg, MSG_LED_SET, (LED2 | (LED_OFF<<4)), 0, 0);
    SetTimerMsg(msg, SEC2TICKS(20));
    /* set led1 to OFF after 20 sec.*/
    LED_MSG_TICKS(msg, LED1, LED_OFF, 0, 0);
    //MSG_BUILD_UINT8(msg, MSG_LED_SET, (LED1 | (LED_OFF<<4)), 0, 0);
    SetTimerMsg(msg, SEC2TICKS(20));
//#endif
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
            if (MSG_KEY_TRANSITION(msg) == KEYTRANS_PRESSED)
            {
                /* set led1 to green */
                LED_MSG_TICKS(NewMsg, LED1, LED_GREEN, 1, 0);
                MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
            }
            else if (MSG_KEY_TRANSITION(msg) == KEYTRANS_ON)
            {
                /* set led1 to red */
                LED_MSG_TICKS(NewMsg, LED1, LED_RED, 1, 0);
                MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
            }
            else if (MSG_KEY_TRANSITION(msg) == KEYTRANS_RELEASED)
            {
                /* set led1 to permanent OFF */
                LED_MSG_TICKS(NewMsg, LED1, LED_OFF, 0, 0);
                MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
            }
            break;
        case MSG_KEY_UP:
            if (MSG_KEY_TRANSITION(msg) == KEYTRANS_PRESSED)
            {
                /* set led2 to green */
                LED_MSG_TICKS(NewMsg, LED2, LED_GREEN, 1, 0);
                MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
            }
            else if (MSG_KEY_TRANSITION(msg) == KEYTRANS_ON)
            {
                /* set led2 to red */
                LED_MSG_TICKS(NewMsg, LED2, LED_RED, 1, 0);
                MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
            }
            else if (MSG_KEY_TRANSITION(msg) == KEYTRANS_RELEASED)
            {
                /* set led2 to permanent OFF */
                LED_MSG_TICKS(NewMsg, LED2, LED_OFF, 0, 0);
                MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
            }
            break;
        case MSG_KEY_DOWN:
            if (MSG_KEY_TRANSITION(msg) == KEYTRANS_PRESSED)
            {
                /* set led3 to green */
                LED_MSG_TICKS(NewMsg, LED3, LED_GREEN, 1, 0);
                MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
            }
            else if (MSG_KEY_TRANSITION(msg) == KEYTRANS_ON)
            {
                /* set led3 to red */
                LED_MSG_TICKS(NewMsg, LED3, LED_RED, 1, 0);
                MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
            }
            else if (MSG_KEY_TRANSITION(msg) == KEYTRANS_RELEASED)
            {
                /* set led3 to permanent OFF */
                LED_MSG_TICKS(NewMsg, LED3, LED_OFF, 0, 0);
                MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
            }
            break;
        default: break;
    }
}

#endif // DEBUG
