/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-04-11 by Ralf Schwarzer
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
 * Revision 2.9  2010/11/07 12:37:05  tuberkel
 * V30 Preparations:
 * - Device/Object Handling completely revised & simplified
 * - New: Compass Display
 * - Compass display mode Heading/Graph+Head/Graph supported
 *
 * Revision 2.8  2009/07/19 12:33:45  tuberkel
 * - ObjectInit reviewed
 *
 * Revision 2.7  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.6  2009/07/08 21:41:17  tuberkel
 * All compiler defines reviewed:
 * - set to unique usage: set define to 0 or 1
 * - default values set, if not used
 * - see 'Project Editor' for details
 *
 * Revision 2.5  2009/07/08 18:46:54  tuberkel
 * Test
 *
 * Revision 2.4  2009/06/21 21:17:43  tuberkel
 * Changes by AN and SCHW:
 * - new Compass layout inside TripCounter Module
 * - can be disabled via user settings 'Tripcounter/upperlower:
 * Bitcoded:
 * - bit0: LongDistance:     1=upside (like roadbook), 0=downside
 * - bit1: ShowCompassValue: 1=show, 0=off
 * - bit2: ShowCompassBar:   1=show, 0=off
 * CompassValue ist shown in footer line
 * CompassCompassBar as graphic
 *
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
//#*#AN debug begin
#include "compassdrv.h"
//#*#AN debug end


// Screen Appearance:  +----.----.----.----.-+
// (21 Chars width)    |                     |
//                     |  0000,0 TripCount   |
//                     |                     |
//                     |---------------------|
//                     |   00000,0 TripCnt   |
//                     |                     |
//                     |124km/h 360° 22:19:30|  Speed + Compass Heading + Time
//                     +----.----.----.----.-+




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
static CHAR         szVehSpeed[8] = "---km/h";          /* buffer for speed string */

static TEXTOBJECT   CompassTxtObj;                      /* stateline compass object */
static CHAR         szCompass[5] = "---°";              /* buffer for time string */

static TEXTOBJECT   TimeTxtObj;                         /* stateline time object */
static CHAR         szTime[10] = "--:--:--a";           /* buffer for time string (add. am/pm in engl. version!)*/


/* external symbols */
extern UINT16           wMilliSecCounter;           /* valid values: 0h .. ffffh */
extern STRING far       szDevName[];                /* device names */
extern DEVFLAGS1_TYPE    gDeviceFlags1;               /* system parameters */
extern DEVFLAGS2_TYPE    gDeviceFlags2;              /* tripcounter flags */
extern BIKE_TYPE        gBikeType;                  /* bike type selecetion */

/* compass display mode */
COMPASSCNTFL_TYPE gCompDplMode;

/* external bitmaps */
extern const unsigned char far * rgCompassTop144x8[];
extern const unsigned char far * rgCompassBot144x8[];


/* internal prototypes */
ERRCODE TripCntKeyhandling(MESSAGE GivenMsg);
UINT16  TripCntToggleSpeed(MESSAGE GivenMsg);
void    TripCntUpdateTimeDate(void);
void    TripCntUpdateCompassHeading(void);
void    TripCntUpdateCompassBargraph(void);

/* text object table */
static const TEXTOBJECT_INITTYPE TextObjInit[] =
{
    /*pObject                   X    Y  Font            H  Width  Align     Format    string ptr        State      */
    /* ----------------------- ---- --- -------------- --- ----- --------- ---------- ----------------- ---------- */
    { &BigTripCntObj,            0,  0, DPLFONT_24X32,  1,  6, TXT_RIGHT,  TXT_NORM, BigTripCntTxt,     OC_DISPL | OC_DYN },
    { &SmallTripCntObj,          6, 38, DPLFONT_14X16,  1,  9, TXT_RIGHT,  TXT_NORM, SmallTripCntTxt,   OC_DISPL | OC_DYN },
    { &VehSpeedTxtObj,           2, 56, DPLFONT_6X8,    1,  7, TXT_LEFT,   TXT_NORM, szVehSpeed,        OC_DISPL | OC_DYN },
    { &CompassTxtObj,           52, 56, DPLFONT_6X8,    1,  4, TXT_CENTER, TXT_NORM, szCompass,         OC_DISPL | OC_DYN },
    { &TimeTxtObj,              45, 56, DPLFONT_6X8,    1, 14, TXT_RIGHT,  TXT_NORM, szTime,            OC_DISPL | OC_DYN }
};
#define TEXTOBJECTLISTSIZE   (sizeof(TextObjInit)/sizeof(TEXTOBJECT_INITTYPE))


/* this devices object focus handling - list of all objects */
/* NOTE:    this device does not need any focus, this
            list is for common object handling only! */
static const void far * ObjectList[] =
{
    (void far *) &BigTripCntObj,
    (void far *) &SmallTripCntObj,
    (void far *) &VehSpeedTxtObj,
    (void far *) &CompassTxtObj,
    (void far *) &TimeTxtObj,
};
#define OBJECTLIST_SIZE   (sizeof(ObjectList)/sizeof(OBJSTATE)/sizeof(void far *))





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

    /* initialize all objects of any type */
    DevObjInit( &TripCntDev, (void far *)TextObjInit,   TEXTOBJECTLISTSIZE,     OBJT_TXT   );

    // initialize this devices objects list
    TripCntDev.Objects.ObjList       = ObjectList;
    TripCntDev.Objects.ObjCount      = OBJECTLIST_SIZE;
    TripCntDev.Objects.FirstSelObj   = DevObjGetFirstSelectable(&TripCntDev, ObjectList, OBJECTLIST_SIZE );
    TripCntDev.Objects.LastSelObj    = DevObjGetLastSelectable (&TripCntDev, ObjectList, OBJECTLIST_SIZE );

    /* return */
    ODS( DBG_SYS, DBG_INFO, "- TripCntDevInit() done!");
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

        // User setting: Which counter should be shown as BIG upper one?
        if (gDeviceFlags2.flags.TripCLongDistUp == 1)
        {   BigTripCnt   = eTRIPC_A;
            SmallTripCnt = eTRIPC_B;
        }
        else
        {   BigTripCnt   = eTRIPC_B;
            SmallTripCnt = eTRIPC_A;
        }

        /* get trip counter display values */
        sprintf(BigTripCntTxt,   "%3u%c%.2u", MeasGetTripCnt(BigTripCnt,   MR_KM_ONLY), RESTXT_DEC_SEPARATOR, MeasGetTripCnt(BigTripCnt,   MR_DKM_ONLY));
        sprintf(SmallTripCntTxt, "%6u%c%.2u", MeasGetTripCnt(SmallTripCnt, MR_KM_ONLY), RESTXT_DEC_SEPARATOR, MeasGetTripCnt(SmallTripCnt, MR_DKM_ONLY));

        /* -------------------------------------------------- */
        /* update vehicle speed '123 km/h */
        wWheelSpeed = MeasGetWheelSpeed(MR_KM_PER_H);
        sprintf( szVehSpeed, "%3u%s", wWheelSpeed, RESTXT_SPEED_DESC);

        /* -------------------------------------------------- */
        /* update time for lower state line '13:15:24p' */
        // TimeDate_GetString( GERM_HHMMSS,  szTime ); // will be updated by special message only

        /* -------------------------------------------------- */
        /* do we have to repaint all? */
        if (TripCntDev.fScreenInit == FALSE)
        {
            /* set ne state */
            TripCntDev.fScreenInit  = TRUE;

            /* yes, repaint complete screen */

            /* show BIGCOUNTER onyl if compass BARGRAGH disabled */
            if (gDeviceFlags2.flags.ShowCompassBar == 0)
                ObjTextShow( &BigTripCntObj);
            ObjTextShow( &SmallTripCntObj );
            ObjTextShow( &VehSpeedTxtObj );

            // the following should be initialized ONCE,
            // but refreshed with diccated message!
            TripCntUpdateTimeDate();

#if (COMPASS==1)
            // show compass VALUE only if enabled
            if (gDeviceFlags2.flags.ShowCompassValue == 1)
                TripCntUpdateCompassHeading();

            // show compass BARGRAGH only if enabled
            if (gDeviceFlags2.flags.ShowCompassBar == 1)
                TripCntUpdateCompassBargraph();
#endif //COMPASS

            /* horizontal line between big & small trip counter */
            /* to be removed to an 'LineObject' !!! */
            {
                DISPLXY Coord = {0,34};
                DisplDrawHorLine(&Coord, 128, 0x03, DPLXOR);
            }
        }
        else
        {
            /* No, repaint only changed stuff */
            /* show BIGCOUNTER onyl if compass BARGRAGH disabled */
            if (gDeviceFlags2.flags.ShowCompassBar == 0)
                ObjTextShow( &BigTripCntObj);
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
                gDeviceFlags1.flags.ActDevNr = DEVID_TRIPCOUNT;          /* save device# for restore */
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
            /* standard refresha message */
            case MSG_SCREEN_REFRESH:
                // Please note, that Compass Data and Time is refreshed on dedicated
                // message MSG_COMPASS_REFRESH and MSG_TIMEDATE_SECOND_GONE only (see below)
                TripCntDevShow(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;

            /* any user action on MULTIPLE keys */
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
                    #if(BIKE_MOTOBAU==1)
                        MSG_BUILD_SETFOCUS(NewMsg, DEVID_TRIPCOUNT, DEVID_LAPCNT);  // goto LapCounterDevice
                    #else // BIKE_MOTOBAU
                        MSG_BUILD_SETFOCUS(NewMsg, DEVID_TRIPCOUNT, DEVID_MONITOR); // goto MonitorDevice
                    #endif // BIKE_MOTOBAU
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;

            /* any user action on a SINGLE key */
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

#if (COMPASS==1)
            /* got a fresh compass heading information */
            case MSG_COMPASS_REFRESH:
                // show compass VALUE only if enabled
                if (gDeviceFlags2.flags.ShowCompassValue == 1)
                    TripCntUpdateCompassHeading();
                // show compass BARGRAGH only if enabled
                if (gDeviceFlags2.flags.ShowCompassBar == 1)
                    TripCntUpdateCompassBargraph();
                // msg processed anyway
                RValue = ERR_MSG_PROCESSED;
                break;
#endif // COMPASS

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
        /* update time for lower state line '13:15:24p' */
        TimeDate_GetString( RESENUM_HHMMSS,  szTime );
        ObjTextShow( &TimeTxtObj );
    }
}


/***********************************************************************
 *  FUNCTION:       TripCntUpdateCompassHeading
 *  DESCRIPTION:    Separate handling of CompassHeading screen refreshs,
 *                  update compass heading for lower state line '359°'
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
#if (COMPASS==1)
void TripCntUpdateCompassHeading(void)
{
    tCompassHeadingInfo *ptHeadingInfo;
    static usOldHeading = 0xffff;

    // check conditions to display timedate */
    if ( TripCntDev.fScreenInit == TRUE  )       // screen is ready?
    {
        /* time to get a fresh heading from driver */
        ptHeadingInfo = CompassGetHeadingInfo();

        /* update only if valid value */
        if( ptHeadingInfo->bValid )
        {
            sprintf( szCompass, "%3d°", ptHeadingInfo->usHeading );
            usOldHeading = ptHeadingInfo->usHeading;
            ObjTextShow( &CompassTxtObj );
        }
        else
        {   // nothing worth to be displayed
        }
    }
}
#endif //COMPASS


/***********************************************************************
 *  FUNCTION:       TripCntUpdateCompassBargraph
 *  DESCRIPTION:    Shows a big graphic on display (half filled)
 *                  with a small floating bar of heading values
 *                  and a big heading value in front.
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        This is in conflict to the upper area
 *                  big tripcounter value, so only one of them
 *                  can be enabled!
 *********************************************************************** */
#if (COMPASS==1)
void TripCntUpdateCompassBargraph(void)
{
    UINT16 usHeading;
    UINT16 usOffset;
    INT16  sTextPos;
    INT16  sTextVal;
    char   szText[8]; //we only need 4 but some space is left if something goes wrong with sprintf
    UINT8  ucTextIdx;

    BITMAP  tBitmap;
    DISPLXY tPixelCoord;

    tCompassHeadingInfo *ptHeadingInfo;
    static usOldHeading = 0xffff;

    tBitmap.wWidth  = 128;
    tBitmap.wHeight = 8;


    // check conditions to display timedate */
    if ( TripCntDev.fScreenInit == FALSE  )       // screen is NOT ready?
    {   return;
    }

    /* time to get a fresh heading from driver */
    ptHeadingInfo = CompassGetHeadingInfo();

    /* update only if valid value */
    if( ptHeadingInfo->bValid == FALSE)
    {   return;
    }

    // ####### OK - START TO DRAW GRAPHIC HERE ! ###########
    usHeading = ptHeadingInfo->usHeading;

    //--------- draw scales at top and bottom ---------
    usOffset = usHeading + 1; //rounding
    usOffset /= 3;            //3 degree steps
    usOffset %= 5;            //reposition to 0 all 3*5=15 degrees
    usOffset *= 4;            //one scale division is 4 pixels wide

    tPixelCoord.wXPos = 0;
    tPixelCoord.wYPos = 0;
    tBitmap.fpucBitmap = (UINT8 far*)( ((UINT32)rgCompassTop144x8) + ((UINT32)usOffset) );
    DisplPrintABitmap( &tBitmap, &tPixelCoord, DPLNORM );

    tPixelCoord.wYPos = 25;
    tBitmap.fpucBitmap = (UINT8 far*)( ((UINT32)rgCompassBot144x8) + ((UINT32)usOffset) );
    DisplPrintABitmap( &tBitmap, &tPixelCoord, DPLNORM );

    //--------- delete values of major lines  ---------
    tPixelCoord.wYPos = 12;   //= 34/2 - 6 + 1 = total height/2 - char height + 1
    DisplDrawHorLine( &tPixelCoord, 128, 0, DPLNORM );

    //--------- print values of major lines   ---------
    //Note that sTextPos may be negative, text is clipped at display borders.
    //First major line is 4 pixels from the left (no offset).
    //Middle of 3 chars of the 4x6 font is 5 pixels.
    sTextPos =  4 - 5;
    sTextPos -= usOffset,     //lines are moving left with growing offset/heading

    sTextVal = usHeading + 1; //rounding (3 degree steps)
    sTextVal /= 15;           //floor to next major value
    sTextVal *= 15;
    sTextVal -= 3*15;         //3 major lines to the left from current heading
    if( sTextVal < 0 ) sTextVal += 360;

    while( sTextPos < 128 )
    {   //end of display
        sprintf( szText, "%03d", sTextVal );
        ucTextIdx = 0;         //position within the string szText
        tPixelCoord.wXPos = sTextPos;

        //Major line may not be visible. Display functions can't clip at the
        //left border. So truncate the string instead.
        if( sTextPos < -4 )
        {   tPixelCoord.wXPos += 8; //print rightmost digit, 2 x char width
            ucTextIdx += 2;
        }
        else if( sTextPos < 0 )
        {   tPixelCoord.wXPos += 4; //print right 2 digits, 1 x char with
            ucTextIdx += 1;
        }

        //only print if value is visible
        if( sTextPos >= -8 ) DisplPrintAString( &(szText[ucTextIdx]), &tPixelCoord, DPLFONT_4X6, DPLNORM );

        sTextPos +=20;          //20 pixels between major lines
        sTextVal += 15;         //15 degrees between major lines
        if( sTextVal >= 360 )
            sTextVal -= 360;
    }

    //--------- fat current heading in the middle -----
    tPixelCoord.wXPos = 64 - 20; //total width/2 - middle of 3 chars of 14x16 font
    tPixelCoord.wYPos = 9;       //= 34/2 - 16/2 = total height/2 - char height/2
    sprintf( szText, "%03d", usHeading );
    DisplPrintAString( szText, &tPixelCoord, DPLFONT_14X16, DPLNORM );
}
#endif //COMPASS


