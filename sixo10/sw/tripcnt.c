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
 *  Created:        11-04-2002 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         trip counter device
 *  Purpose:        handles the trip counter (A & B) screen device
 *  Comments:       -
 *
 ************************************************************************ */

/* include */
#include <stdio.h>
#include <string.h>
#include "standard.h"
#include "debug.h"
#include "msgqueue.h"
#include "timer.h"
#include "display.h"
#include "resource.h"
#include "objects.h"
#include "device.h"
#include "digindrv.h"
#include "measure.h"
#include "sysparam.h"
#include "beep.h"
#include "led.h"


/* defines for device data */
#define BIGTRIP_STRSIZE    7            /* to contain max. string    '999,99' km */
#define SMALLTRIP_STRSIZE 10            /* to contain max. string '999999,99' km */
#define BUFFER_STRSIZE    16

/* device static objects */
static DEVDATA      TripCntDev;

static TEXTOBJECT   BigTripCntObj;                      /* contains the bigger trip counter object data */
static TEXTOBJECT   SmallTripCntObj;                    /* contains the smaller trip counter object data */

static CHAR         BigTripCntTxt[BUFFER_STRSIZE]   =    "  0,00";    /* buffer for big trip counter string */
static CHAR         SmallTripCntTxt[BUFFER_STRSIZE] = "     0,00";    /* buffer for samll trip counter string */


/* external symbols */
extern UINT16           wMilliSecCounter;           // valid values: 0h .. ffffh
extern STRING far       szDevName[];                // device names
extern SYSFLAGS_TYPE    gSystemFlags;               /* system parameters */

/* internal prototypes */
ERRCODE TripCntKeyhandling(MESSAGE GivenMsg);
UINT16 TripCntToggleSpeed(MESSAGE GivenMsg);



/***********************************************************************
 *  FUNCTION:       TripCntDevInit
 *  DESCRIPTION:    all initial stuff for all objects
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE TripCntDevInit(void)
{
    /* device main data */
    TripCntDev.eDevID       = DEVID_TRIPCOUNT;
    TripCntDev.szDevName    = szDevName[DEVID_TRIPCOUNT];
    TripCntDev.fFocused     = FALSE;
    TripCntDev.fScreenInit  = FALSE;

    /* big (upper) trip counter object */
    ObjTextInit(    &BigTripCntObj,
                    0, 0, DPLFONT_24X32, 1, 6, TXT_RIGHT, TXT_NORM,
                    BigTripCntTxt,
                    OC_DISPL );

    /* small (lower) trip counter object */
    ObjTextInit(    &SmallTripCntObj,
                    6, 39, DPLFONT_14X16, 1, 9, TXT_RIGHT, TXT_NORM,
                    SmallTripCntTxt,
                    OC_DISPL );

    /* return */
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       TripCntDevShow
 *  DESCRIPTION:    bring updated trip counter device to display
 *                  by calling Show-Fct. of all relevant objects
 *  PARAMETER:      BOOL    TRUE = show objects, FALSE = clear screen
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void TripCntDevShow(BOOL fShow)
{
    ERRCODE error = ERR_OK;
    MESSAGE NewMsg;             /* for screen fresh message */
    DIST_TYPE TripCnt;          /* trip counter A & B 4 byte structure */

    /* 'show' or 'clear' screen? */
    if (fShow == TRUE)
    {
        /* map trip counter A to big trip counter display */
        sprintf(BigTripCntTxt, "%3u,%.2u", MeasGetTripA(MR_KM_ONLY), MeasGetTripA(MR_DKM_ONLY));

        /* map trip counter B to small trip counter display */
        sprintf(SmallTripCntTxt, "%3u,%.2u", MeasGetTripB(MR_KM_ONLY), MeasGetTripB(MR_DKM_ONLY));

        /* do we have to repaint all? */
        if (TripCntDev.fScreenInit == FALSE)
        {
            /* yes, repaint complete screen */
            ObjTextShow( &BigTripCntObj);
            ObjTextShow( &SmallTripCntObj );
            TripCntDev.fScreenInit  = TRUE;

            /* horizontal line between big & small trip counter */
            /* to be removed to an 'LineObject' !!! */
            {
                DISPLXY Coord = {0,34};
                DisplDrawHorLine(&Coord, 128, 0x03);
            }
        }
        else
        {
            /* No, repaint only changed stuff */
            ObjTextShow( &BigTripCntObj);
            ObjTextShow( &SmallTripCntObj );
        }
    }
    else
    {
        DisplClearScreen(0x0);              /* leave a clear screen for next device */
        TripCntDev.fScreenInit = FALSE;     /* reset devices screen state */
    }
}


/***********************************************************************
 *  FUNCTION:       TripCntMsgEntry
 *  DESCRIPTION:    Receive Message Handler called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE TripCntMsgEntry(MESSAGE GivenMsg)
{
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID  MsgId;
    MESSAGE     NewMsg;

    /* analyze message: it's for us? */
    MsgId = MSG_ID(GivenMsg);        /* get message id */

    /* --------------------------------------------------- */
    /* focus handling */

    switch (MsgId)
    {
        case MSG_GET_FOCUS:
        {
            /*  Some device want's to get the focus:
                If we've currently got the focus, we'll
                answer to SET his focus! */
            if ( TripCntDev.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_TRIPCOUNT]);
                MSG_BUILD_SETFOCUS(NewMsg,DEVID_TRIPCOUNT,MSG_SENDER_ID(GivenMsg));   /* build answer message */
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                     /* send answer message */
                TripCntDev.fFocused = FALSE;                                     /* clear our focus */
                TripCntDevShow(FALSE);                                           /* clear our screen */
                RValue = ERR_MSG_PROCESSED;
            }
        } break;
        case MSG_SET_FOCUS:
        {
            /* messsage for us? */
            if (MSG_RECEIVER_ID(GivenMsg) == DEVID_TRIPCOUNT)
            {
                /*  Someone wants us to take the focus?
                    We assume, that nobody else has the focus
                    and we've got the the screen now for us! */
                if (  (TripCntDev.fFocused       == TRUE           )
                    &&(MSG_RECEIVER_ID(GivenMsg) == DEVID_TRIPCOUNT) )
                    ODS2(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but it already has focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_TRIPCOUNT]);
                else /*
                    ODS2(   DBG_SYS, DBG_INFO,
                            "FOCUS: %s -> %s!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_TRIPCOUNT]) */;
                TripCntDev.fFocused = TRUE;                             /* set our focus */
                TripCntDevShow(TRUE);                                   /* show our screen */
                gSystemFlags.flags.ActDevNr = DEVID_TRIPCOUNT;          /* save device# for restore */
                RValue = ERR_MSG_PROCESSED;
             }
             else
             {
                if ( TripCntDev.fFocused == TRUE )
                {
                    /*  Some other device should be given the focus,
                        BUT WE'VE GOT THE FOCUS!
                        It should first ask US to give the focus! :-( */
                    ODS3(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but %s had focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[MSG_RECEIVER_ID(GivenMsg)],
                            szDevName[DEVID_TRIPCOUNT]);
                    TripCntDev.fFocused = FALSE;                        /* loose our focus */
                    ODS1(   DBG_SYS, DBG_WARNING,
                            "%s now loosing focus :-( ",
                            szDevName[DEVID_TRIPCOUNT]);
                    RValue = ERR_MSG_NOT_PROCESSED;
                }
             }
        } break;
        default: break;             /* no default */
    } // of switch MsgId

    /* --------------------------------------------------- */
    /* this part is only for FOCUSED time
       AND msg not already processed */
    if (  (TripCntDev.fFocused == TRUE )
        &&(RValue == ERR_MSG_NOT_PROCESSED ) )
    {
        switch (MsgId)
        {
            case MSG_SCREEN_REFRESH:
                TripCntDevShow(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;
            case MSG_KEYS_PRESSED:
                /* check if UP&DOWN are pressed short: */
                if (  (RValue == ERR_MSG_NOT_PROCESSED                    )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN))
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYSHORT              ) )
                {
                    /* give focus immediatly to next screen */
                    TripCntDev.fFocused = FALSE;                              /* clear our focus */
                    TripCntDevShow(FALSE);                                    /* clear our screen */
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_TRIPCOUNT, DEVID_SET);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_KEY_OK:
            case MSG_KEY_UP:
            case MSG_KEY_DOWN:
                RValue = TripCntKeyhandling(GivenMsg);          /* used for Ok, Up, Down: */
                break;
            case MSG_DPL_FLASH_ON:
            case MSG_DPL_FLASH_OFF:
            default: RValue = ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}


/***********************************************************************
 *  FUNCTION:       TripCntKeyhandling
 *  DESCRIPTION:    Handling of all pressed keys
 *  PARAMETER:      MESSAGE Given Key Msg
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE TripCntKeyhandling(MESSAGE GivenMsg)
{
    ERRCODE     RValue = ERR_MSG_PROCESSED;
    MESSAGE_ID  MsgId;
    MESSAGE     NewMsg;
    DIST_TYPE   TripCnt;

    MsgId = MSG_ID(GivenMsg);                   /* get message id */
    switch (MsgId)
    {
        case MSG_KEY_OK:
            if (MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED )      // just pressed?
            {
                /* clear big trip counter (here: TripCntA) any way */
                TripCnt.km = 0;                         /* clear local counter */
                MeasSetTripA(&TripCnt);                 /* copy to TripCnt A */
            }
            if (MSG_KEY_DURATION(GivenMsg) > KEYSECURE )                // pressed > 3 sec?
            {
                /* clear small trip counter (here: TripCntB) */
                TripCnt.km = 0;                         /* clear local counter */
                MeasSetTripB(&TripCnt);                 /* copy to TripCnt B */
                BeepOk();
                LEDOk();
            }
            break;
        case MSG_KEY_UP:                        /* increment smaller tripcounter (B) */
            if (  (MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED )   // just pressed?
                ||(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON      ) ) // key repetition rate active?
            {
                UINT16 wIncrVal = TripCntToggleSpeed(GivenMsg);         /* get incr value = f(key press duration) */
                MeasGetRawTripB(&TripCnt);                              /* get current trip counter value */
                if ((TripCnt.dkm + wIncrVal) < 99999999L)               /* no overflow? */
                {
                    TripCnt.dkm += wIncrVal;                            /* increment local copy */
                    MeasSetTripB(&TripCnt);                             /* copy to TripCnt B */
                }
            }
            break;
        case MSG_KEY_DOWN:                      /* decrement smaller tripcounter (B) */
            if (  (MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED )   // just pressed?
                ||(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON      ) ) // key repetition rate active?
            {
                UINT16 wDecrValue = TripCntToggleSpeed(GivenMsg);       /* get decr value = f(key press duration) */
                MeasGetRawTripB(&TripCnt);                              /* get current trip counter value */
                if (wDecrValue > TripCnt.dkm)                           /* too big to decr? */
                    wDecrValue = TripCnt.dkm;                           /*-> clip to ensure reset to zero! */
                if ((TripCnt.dkm - wDecrValue) <= TripCnt.dkm)          /* no underflow? */
                {
                    TripCnt.dkm -= wDecrValue;                          /* decrement local copy */
                    MeasSetTripB(&TripCnt);                             /* copy to TripCnt B */
                }
            }
            break;
        default:
            RValue = ERR_MSG_NOT_PROCESSED;
            break;
    }

    /* refresh trip counter display immediatly, if necessary */
    if (RValue == ERR_MSG_PROCESSED)
        TripCntDevShow(TRUE);
    return RValue;
}


/***********************************************************************
 *  FUNCTION:       TripCntToggleSpeed
 *  DESCRIPTION:    gives a value adequate to time the user presses key
 *  PARAMETER:      Given msg including the duration
 *  RETURN:         inkr/decr value in 10 m
 *  COMMENT:        -
 *********************************************************************** */
UINT16 TripCntToggleSpeed(MESSAGE GivenMsg)
{
    UINT16 RValue;

    /* 2nd variant: incr. duration time */
    if (MSG_KEY_DURATION(GivenMsg) < 1000)
        return 1;
    else return (MSG_KEY_DURATION(GivenMsg)/1000);

    /* 1st variant: push 10th steps */
    if (MSG_KEY_DURATION(GivenMsg) <= 1000)
        RValue = 1;
    else if (MSG_KEY_DURATION(GivenMsg) < 10000)
        RValue = 10;
    else if (MSG_KEY_DURATION(GivenMsg) < 20000)
        RValue = 100;
    else if (MSG_KEY_DURATION(GivenMsg) < 30000)
        RValue = 1000;
    else RValue = 10000;

    return RValue;
}


