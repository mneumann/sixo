/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-04-11 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         trip counter device
 *  Purpose:        handles the trip counter (A & B) screen device
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
 *      Arnold Neugebauer                   Ralf Krizsan
 *      Stöckener Str. 115                  Knickstr. 10
 *      30419 Hannover                      30890 Barsinghausen
 *      arnold.neugebauer@web.de            ralf.krizsan@web.de
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
 * Revision 2.3  2007/03/30 09:58:22  tuberkel
 * language specific display content
 *
 * Revision 2.2  2007/03/26 23:19:25  tuberkel
 * changed MOTOBAU version handling
 * - eBikeType -> #define
 *
 * Revision 2.1  2006/10/01 22:13:27  tuberkel
 * just comment
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.12  2006/03/18 08:58:14  Ralf
 * - TripCounter improved to 4 different counters
 *
 * Revision 1.11  2006/02/18 14:31:44  Ralf
 * - BIKE_MOTOBAU: switching to lapcounter instead to monitor device
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
#include "timedate.h"

/* defines for device data */
#define BIGTRIP_STRSIZE    7            /* to contain max. string    '999,99' km */
#define SMALLTRIP_STRSIZE 10            /* to contain max. string '999999,99' km */
#define BUFFER_STRSIZE    16

/* device static objects */
static DEVDATA      TripCntDev;

static TEXTOBJECT   BigTripCntObj;                      /* contains the bigger trip counter object data */
static TEXTOBJECT   SmallTripCntObj;                    /* contains the smaller trip counter object data */

static CHAR         BigTripCntTxt[BUFFER_STRSIZE]   = {' ', ' ', '0', RESTXT_DEC_SEPARATOR, '0', '0'};  /* buffer for big trip counter string */
static CHAR         SmallTripCntTxt[BUFFER_STRSIZE] = {' ', ' ', ' ', ' ', ' ', '0', RESTXT_DEC_SEPARATOR, '0', '0'};  /* buffer for samll trip counter string */

static TEXTOBJECT   VehSpeedTxtObj;                     /* stateline speed object */
static CHAR         szVehSpeed[9] = "--- km/h";         /* buffer for speed string */

static TEXTOBJECT   TimeTxtObj;                         /* stateline time object */
static CHAR         szTime[15] = "--:--:--";            /* buffer for time string */

/* external symbols */
extern UINT16           wMilliSecCounter;           /* valid values: 0h .. ffffh */
extern STRING far       szDevName[];                /* device names */
extern SYSFLAGS_TYPE    gSystemFlags;               /* system parameters */
extern TRPCNTFL_TYPE    gTripCntFlags;              /* tripcounter flags */
extern BIKE_TYPE        gBikeType;                  /* bike type selecetion */


/* internal prototypes */
ERRCODE TripCntKeyhandling(MESSAGE GivenMsg);
UINT16 TripCntToggleSpeed(MESSAGE GivenMsg);
void TripCntUpdateTimeDate(void);


/* text object table */
static const TEXTOBJECT_INITTYPE TextObjects[] =
{
    /*pObject                   X    Y  Font            H  Width  Align     Format    string ptr        State      */
    /* ----------------------- ---- --- -------------- --- ----- --------- ---------- ----------------- ---------- */
    { &BigTripCntObj,            0,  0, DPLFONT_24X32,  1,  6, TXT_RIGHT,  TXT_NORM, BigTripCntTxt,     OC_DISPL },
    { &SmallTripCntObj,          6, 38, DPLFONT_14X16,  1,  9, TXT_RIGHT,  TXT_NORM, SmallTripCntTxt,   OC_DISPL },
    { &VehSpeedTxtObj,           2, 56, DPLFONT_6X8,    1,  8, TXT_CENTER, TXT_NORM, szVehSpeed,        OC_DISPL },
    { &TimeTxtObj,              62, 56, DPLFONT_6X8,    1, 14, TXT_CENTER, TXT_NORM, szTime,            OC_DISPL }
};





/***********************************************************************
 *  FUNCTION:       TripCntDevInit
 *  DESCRIPTION:    all initial stuff for all objects
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE TripCntDevInit(void)
{
    int i;

    /* device main data */
    TripCntDev.eDevID       = DEVID_TRIPCOUNT;
    TripCntDev.szDevName    = szDevName[DEVID_TRIPCOUNT];
    TripCntDev.fFocused     = FALSE;
    TripCntDev.fScreenInit  = FALSE;

    /* initialize text objects */
    for (i = 0; i < ARRAY_SIZE(TextObjects); i++)
    {
        ObjTextInit(TextObjects[i].pObject,
                    TextObjects[i].wOrgPosX,
                    TextObjects[i].wOrgPosY,
                    TextObjects[i].eFont,
                    TextObjects[i].bWindHeight,
                    TextObjects[i].bWindWidth,
                    TextObjects[i].eAlign,
                    TextObjects[i].eFormat,
                    TextObjects[i].szText,
                    TextObjects[i].bState);
    }

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
    ERRCODE     error = ERR_OK;
    MESSAGE     NewMsg;             /* for screen fresh message */
    DIST_TYPE   TripCnt;            /* trip counter A & B 4 byte structure */

    /* 'show' or 'clear' screen? */
    if (fShow == TRUE)
    {
        /* -------------------------------------------------- */
        // display versions: Show like Roadbook, LongDistance above, ShortDist lower part
        // NOTE: behaviour on pressed keys (reset/inc/decr) is fixed to TripX-values, not to display!
        // TBD:  this should later be done via Eeprom settings!
        TRIPC_ID BigTripCnt;
        TRIPC_ID SmallTripCnt;
        UINT16   wWheelSpeed;

        if ( gTripCntFlags.flags.LongDistUp == 1 )
        {   BigTripCnt   = eTRIPC_A;
            SmallTripCnt = eTRIPC_B;
        }
        else
        {
            BigTripCnt = eTRIPC_B;
            SmallTripCnt = eTRIPC_A;
        }

        /* get trip counter display values */
        sprintf(BigTripCntTxt,   "%3u%c%.2u", MeasGetTripCnt(BigTripCnt,   MR_KM_ONLY), RESTXT_DEC_SEPARATOR, MeasGetTripCnt(BigTripCnt,   MR_DKM_ONLY));
        sprintf(SmallTripCntTxt, "%6u%c%.2u", MeasGetTripCnt(SmallTripCnt, MR_KM_ONLY), RESTXT_DEC_SEPARATOR, MeasGetTripCnt(SmallTripCnt, MR_DKM_ONLY));

        /* -------------------------------------------------- */
        /* update vehicle speed '123 km/h */
        wWheelSpeed = MeasGetWheelSpeed(MR_KM_PER_H);
        sprintf( szVehSpeed, "%3u %s", wWheelSpeed, RESTXT_SPEED_DESC);

        /* -------------------------------------------------- */
        /* update time for lower state line '13:15:24' */
        // TimeDate_GetString( GERM_HHMMSS,  szTime ); // will be updated by special message only

        /* -------------------------------------------------- */
        /* do we have to repaint all? */
        if (TripCntDev.fScreenInit == FALSE)
        {
            /* set ne state */
            TripCntDev.fScreenInit  = TRUE;

            /* yes, repaint complete screen */
            ObjTextShow( &BigTripCntObj);
            ObjTextShow( &SmallTripCntObj );
            ObjTextShow( &VehSpeedTxtObj );
            TripCntUpdateTimeDate();        // at initial time only!

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
            ObjTextShow( &BigTripCntObj   );
            ObjTextShow( &SmallTripCntObj );
            ObjTextShow( &VehSpeedTxtObj );
            //ObjTextShow( &TimeTxtObj ); will be refreshed by special message only
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

                    // special MOTOBAU behaviour
                    #ifdef BIKE_MOTOBAU
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_TRIPCOUNT, DEVID_LAPCNT);  // goto LapCounterDevice
                    #else // BIKE_MOTOBAU
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_TRIPCOUNT, DEVID_MONITOR); // goto MonitorDevice
                    #endif // BIKE_MOTOBAU
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_KEY_OK:
            case MSG_KEY_UP:
            case MSG_KEY_DOWN:
                RValue = TripCntKeyhandling(GivenMsg);          /* used for Ok, Up, Down: */
                break;

            /* trigger time / date screen update only */
            case MSG_TIMEDATE_SECOND_GONE:
                TripCntUpdateTimeDate();
                RValue = ERR_MSG_PROCESSED;
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
                /* clear big trip counter (here: TripCntB) any way */
                TripCnt.km = 0;                         /* clear local counter */
                MeasSetTripCnt(eTRIPC_B, &TripCnt);     /* copy to TripCnt B */
            }
            if (MSG_KEY_DURATION(GivenMsg) > KEYSECURE )                // pressed > 3 sec?
            {
                /* check: long distance counter to be reseted? */
                if (MeasGetTripCnt(eTRIPC_A, MR_DKM) > 0)
                {
                    /* clear both trip counter the same time  */
                    TripCnt.km = 0;                         /* clear local counter */
                    MeasSetTripCnt(eTRIPC_B, &TripCnt);     /* copy to TripCnt B */
                    MeasSetTripCnt(eTRIPC_A, &TripCnt);     /* copy to TripCnt A */
                    BeepOk();
                    LEDOk();
                }
            }
            break;
        case MSG_KEY_UP:                        /* increment smaller tripcounter (A) */
            if (  (MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED )   // just pressed?
                ||(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON      ) ) // key repetition rate active?
            {
                UINT16 wIncrVal = TripCntToggleSpeed(GivenMsg);         /* get incr value = f(key press duration) */
                MeasGetRawTripCnt(eTRIPC_A, &TripCnt);                  /* get current trip counter value */
                if ((TripCnt.dkm + wIncrVal) < DIST_MAX_VEHIC)          /* no overflow? */
                {
                    TripCnt.dkm += wIncrVal;                            /* increment local copy */
                    MeasSetTripCnt(eTRIPC_A, &TripCnt);                 /* copy to TripCnt A */
                }
            }
            break;
        case MSG_KEY_DOWN:                      /* decrement smaller tripcounter (A) */
            if (  (MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED )   // just pressed?
                ||(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON      ) ) // key repetition rate active?
            {
                UINT16 wDecrValue = TripCntToggleSpeed(GivenMsg);       /* get decr value = f(key press duration) */
                MeasGetRawTripCnt(eTRIPC_A, &TripCnt);                              /* get current trip counter value */
                if (wDecrValue > TripCnt.dkm)                           /* too big to decr? */
                    wDecrValue = TripCnt.dkm;                           /*-> clip to ensure reset to zero! */
                if ((TripCnt.dkm - wDecrValue) <= TripCnt.dkm)          /* no underflow? */
                {
                    TripCnt.dkm -= wDecrValue;                          /* decrement local copy */
                    MeasSetTripCnt(eTRIPC_A, &TripCnt);                 /* copy to TripCnt B */
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


/***********************************************************************
 *  FUNCTION:       TripCntUpdateTimeDate
 *  DESCRIPTION:    Separate handling of TimeDate screen refreshs
 *                  to synchronize display to RTC seconds
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void TripCntUpdateTimeDate(void)
{
    // check conditions to display timedate */
    if ( TripCntDev.fScreenInit == TRUE  )       // screen is ready?
    {
        /* update time for lower state line '13:15:24' */
        TimeDate_GetString( RESENUM_HHMMSS,  szTime );
        ObjTextShow( &TimeTxtObj );
    }
}


