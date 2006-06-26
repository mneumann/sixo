/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-24 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         test devices
 *  Purpose:        handles the test screen device with all
 *                  subsequent potential screens
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
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.3  2006/02/18 15:16:24  Ralf
 * - just resource text name changed
 *
 *
 ************************************************************************ */


/* include */
#include <stdio.h>
#include "standard.h"
#include "debug.h"
#include "msgqueue.h"
#include "timer.h"
#include "display.h"
#include "resource.h"
#include "objects.h"
#include "device.h"
#include "digindrv.h"
#include "sysparam.h"


/* device static objects */
static DEVDATA      TestScreenDev;
static TEXTOBJECT   TextObj1;
static TEXTOBJECT   TextObj2;
static UINT16       wTestValue = 8877;
const static CHAR   szFormat[] = "%5d";
static CHAR         szBuffer[20];

/* text object */
static EDITTEXTOBJECT   EditTextObj1;
static CHAR             szDescr1[] = "Lower:";
static CHAR             szEditText1[] = "abcdefgh";

/* number object */
extern UINT16           wWheelSize;                         /* original wheel size from eeprom in mm */
static UINT16           wWheelSizeCopy;                     /* copy value to edit */
static EDITNUMBEROBJECT EditWheelSizeObj;                   /* complete number object */
static CHAR             szWheelSizeDescr[] = RESTXT_SET_WHEELSIZEDESC;  /* descriptor string */
static CHAR             szWheelSizeUnits[] = RESTXT_SET_WHEELSIZEUNIT;   /* units string */
static UINT32           dwTest32 = 99000;
static UINT32            dwTest32Copy;

/* external symbols */
extern UINT16           wMilliSecCounter;       // valid values: 0h .. ffffh
extern STRING far       szDevName[];            // device names
extern SYSFLAGS_TYPE    gSystemFlags;           /* system parameters */

/* internal prototypes */
INT16 Test_ValueInkr(MESSAGE msg);


/***********************************************************************
 *  FUNCTION:       TestScreenInit
 *  DESCRIPTION:    all initial stuff for all objects of
 *                  'test screen device'
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE TestScreenInit(void)
{
    /* device main data */
    TestScreenDev.eDevID       = DEVID_TEST;
    TestScreenDev.szDevName    = szDevName[DEVID_TEST];
    TestScreenDev.fFocused     = FALSE;
    TestScreenDev.fScreenInit  = FALSE;

    /* init objects */
    sprintf(szBuffer,szFormat, wTestValue);

    /* Headline */
    ObjTextInit(    &TextObj1,
                    0, 0, DPLFONT_8X16, 1, 16, TXT_CENTER, TXT_UNDERL,
                    RESTXT_TEST1,
                    OC_DISPL );

    /* edit field 1 */
    ObjEditTextInit(    &EditTextObj1,
                        5, 25, DPLFONT_6X8, 16,
                        szEditText1, szDescr1, 3,
                        CHARL_LOWER,
                        OC_DISPL | OC_SELECT | OC_EDIT );

    /* number object */
    ObjEditNumInit( &EditWheelSizeObj,      // object to be initilized
                    0, 35,                  // X / Y origin
                    DPLFONT_6X8,            // used font
                    21,                     // overall window width in chars
                    &dwTest32,              // pointer to original value
                    &dwTest32Copy,          // pointer to work copy buffer
                    eULONG,0L, 999999L, 0L, // type of number, min, max Limits, stepsize
                    eDez, eColumn, 0,       // dez display format, column edit mode, comma pos
                    "",//szWheelSizeDescr,       // descriptor string
                    "",//szWheelSizeUnits,       // units string
                    6,                      // edit field length in chars
                    OC_DISPL | OC_SELECT | OC_EDIT ); // init states

    /* TEST */
    //Test_TestScreenStartMsg();


    return ERR_OK;
}

/***********************************************************************
 *  FUNCTION:       TestScreenShow
 *  DESCRIPTION:    bring updated 'test screen device' to display
 *                  by calling Show-Fct. of all objects
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void TestScreenShow(BOOL fShow)
{
    /* update data */
    sprintf(szBuffer,szFormat, wTestValue);

    /* its on screen? */
    if (fShow == TRUE)
    {
        /* show objects */
        ObjTextShow( &TextObj1 );
        ObjEditTextShow( &EditTextObj1, SHOW_ALL);
        ObjEditNumShow( &EditWheelSizeObj, SHOW_ALL);
        TestScreenDev.fScreenInit = TRUE;
    }
    else
    {
        DisplClearScreen(0x0);
        TestScreenDev.fScreenInit  = FALSE;
    }
}


/***********************************************************************
 *  FUNCTION:       TestScreenMsgEntry
 *  DESCRIPTION:    Receive Message Handler of 'test screen' device
 *                  called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE TestScreenMsgEntry(MESSAGE GivenMsg)
{
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID  MsgId;
    MESSAGE     NewMsg;


    /* analyze message: it's for us? */
    MsgId = MSG_ID(GivenMsg);        /* get message id */
    switch (MsgId)
    {
        case MSG_GET_FOCUS:
        {
            /*  Some device want's to get the focus:
                If we've currently got the focus, we'll
                answer to SET his focus! */
            if ( TestScreenDev.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_TEST]);
                MSG_BUILD_SETFOCUS(NewMsg,DEVID_TEST,MSG_SENDER_ID(GivenMsg));   /* build answer message */
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                            /* send answer message */
                TestScreenDev.fFocused = FALSE;                                        /* clear our focus */
                TestScreenShow(FALSE);                                                 /* clear our screen */
                RValue = ERR_MSG_PROCESSED;
            }
        } break;
        case MSG_SET_FOCUS:
        {
            /* messsage for us? */
            if (MSG_RECEIVER_ID(GivenMsg) == DEVID_TEST)
            {
                /*  Someone wants us to take the focus?
                    We assume, that nobody else has the focus
                    and we've got the the screen now for us! */
                if (  (TestScreenDev.fFocused   == TRUE             )
                    &&(MSG_RECEIVER_ID(GivenMsg) == DEVID_TEST) )
                    ODS2(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but it already has focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_TEST]);
                else /*
                    ODS2(   DBG_SYS, DBG_INFO,
                            "FOCUS: %s -> %s!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_TEST]) */ ;
                TestScreenDev.fFocused = TRUE;                          /* set our focus */
                TestScreenShow(TRUE);                                   /* show our screen */
                gSystemFlags.flags.ActDevNr = DEVID_TEST;               /* save device# for restore */
                RValue = ERR_MSG_PROCESSED;
             }
             else
             {
                if ( TestScreenDev.fFocused == TRUE )
                {
                    /*  Some other device should be given the focus,
                        BUT WE'VE GOT THE FOCUS!
                        It should first ask US to give the focus! :-( */
                    ODS3(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but %s had focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[MSG_RECEIVER_ID(GivenMsg)],
                            szDevName[DEVID_TEST]);
                    TestScreenDev.fFocused = FALSE;                        /* loose our focus */
                    ODS1(   DBG_SYS, DBG_WARNING,
                            "%s now loosing focus :-( ",
                            szDevName[DEVID_TEST]);
                    RValue = ERR_MSG_PROCESSED;
                }
             }
        } break;
    }

    /* this part is only for FOCUSED time
       AND msg not already processed */
    if (  (TestScreenDev.fFocused == TRUE )
        &&(RValue == ERR_MSG_NOT_PROCESSED ) )
    {
        switch (MsgId)
        {
            case MSG_SCREEN_REFRESH:
                TestScreenShow(TRUE);              /*  The system tells focused device to refresh its screen! */
                RValue = ERR_MSG_PROCESSED;
                break;

            /* all these messages handled by objects */
            case MSG_DPL_FLASH_ON:
            case MSG_DPL_FLASH_OFF:
            case MSG_KEY_OK:
            case MSG_KEYS_PRESSED:
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = ObjEditTextMsgEntry(&EditTextObj1, GivenMsg);
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = ObjEditNumMsgEntry(&EditWheelSizeObj, GivenMsg);
                if (RValue == ERR_MSG_PROCESSED)
                {
                    /* something changed -> Refresh! */
                    TestScreenShow(TRUE);
                }
                /* check if UP&DOWN are pressed */
                if (  (RValue == ERR_MSG_NOT_PROCESSED      )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN))
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYSHORT              ) )
                {
                    /* give focus immediatly to IntroScreen */
                    TestScreenDev.fFocused = FALSE;                         /* clear our focus */
                    TestScreenShow(FALSE);                                  /* clear our screen */
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_TEST, DEVID_INTRO);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_KEY_UP:
                ObjEditTextMsgEntry(&EditTextObj1, GivenMsg);
                ObjEditNumMsgEntry(&EditWheelSizeObj, GivenMsg);
                if (  (EditTextObj1.State.bits.fEditActive == FALSE)
                    &&(EditWheelSizeObj.State.bits.fEditActive == FALSE) )
                {
                    EditTextObj1.State.bits.fSelected = TRUE;
                    EditWheelSizeObj.State.bits.fSelected = FALSE;
                }
                TestScreenShow(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;
            case MSG_KEY_DOWN:
                if (  (EditTextObj1.State.bits.fEditActive == TRUE)
                    ||(EditWheelSizeObj.State.bits.fEditActive == TRUE) )
                {
                    ObjEditTextMsgEntry(&EditTextObj1, GivenMsg);
                    ObjEditNumMsgEntry(&EditWheelSizeObj, GivenMsg);
                }
                else
                {
                    EditTextObj1.State.bits.fSelected = FALSE;
                    EditWheelSizeObj.State.bits.fSelected = TRUE;
                }
                TestScreenShow(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;
            default: return ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}





void Test_TestScreenStartMsg(void)
{
    MESSAGE NewMsg;
    ERRCODE RValue;

    /* Test A: get focus after 2 sec from 'unknown' */
    MSG_BUILD_SETFOCUS( NewMsg, DEVID_UNKNOWN, DEVID_TEST);
    RValue = SetTimerMsg(NewMsg, MS2TICKS(20));

    /* Test B: loose focus after 4 sec to 'unknown' */
    //MSG_BUILD_GETFOCUS( NewMsg, DEVID_ALL, DEVID_UNKNOWN);
    //RValue = SetTimerMsg(NewMsg, SEC2TICKS(4));

    /* Test C: reset  focus after 6 sec to 'unknown' */
    //MSG_BUILD_SETFOCUS( NewMsg, DEVID_UNKNOWN, DEVID_TEST);
    //RValue = SetTimerMsg(NewMsg, SEC2TICKS(6));
}





void Test_ValueInkrMsg(MESSAGE GivenMsg)
{
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID  MsgId;
    MESSAGE     NewMsg;

    /* Test */
    MsgId = MSG_ID(GivenMsg);        /* get message id */
    switch (MsgId)
    {
        case MSG_KEY_UP:
            wTestValue += Test_ValueInkr(GivenMsg);
            if ( TestScreenDev.fFocused == TRUE )
            {
                TestScreenShow(TRUE);
            }
            break;
        case MSG_KEY_DOWN:
            wTestValue -= Test_ValueInkr(GivenMsg);
            if (wTestValue < 0)
                wTestValue = 0;
            {
                TestScreenShow(TRUE);            }
            break;
        case MSG_KEY_OK:
            wTestValue = 0;
            if ( TestScreenDev.fFocused == TRUE )
                TestScreenShow(TRUE);
            break;
        default: RValue = ERR_MSG_NOT_PROCESSED;
    }
}




INT16 Test_ValueInkr(MESSAGE KeyMsg)
{
    static UINT16 wLastInkr_ms = 0;
    UINT16      wActInkr_ms = 0;
    UINT16      wDuration;
    KEYTRANSIT  KeyTransition;
    UINT16      wInterval;

    /* get a time stamp */
    TimerGetSys_msec(wActInkr_ms);

    /* first message? */
    KeyTransition = MSG_KEY_TRANSITION(KeyMsg);
    if (KeyTransition == KEYTRANS_PRESSED)
    {
        wLastInkr_ms = wActInkr_ms;
        return 1;
    }

    /* set dynamic key interval */
    wDuration = MSG_KEY_DURATION(KeyMsg);
    if (wDuration < 3000)
        wInterval = 300;
    else if (wDuration < 8000)
        wInterval = 500;
    else if (wDuration < 16000)
        wInterval = 800;
    else if (wDuration < 24000)
        wInterval = 800;
    else wInterval = 800;

    /* longer pressed, time to update? */
    if (  ((wActInkr_ms - wLastInkr_ms) > wInterval)
        &&(KeyTransition == KEYTRANS_ON      ) )
    {
        wLastInkr_ms = wActInkr_ms;

        /* set return value */
        if (wDuration < 3000)
            return 1;
        else if (wDuration < 8000)
            return 10;
        else if (wDuration < 16000)
            return 100;
        else if (wDuration < 24000)
            return 1000;
        else return 10000;
    }
    else return 0;
}
