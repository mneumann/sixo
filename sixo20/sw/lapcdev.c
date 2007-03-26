/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2006-01-03 by Ralf Krizsan
 *  Project:        SIxO 20
 *  Module:         Lap Counter Device
 *  Purpose:        Lap Counter functions
 *  Comments:       -
 *  Changes:        -
 *  ToDo:           -
 *
 *  Compiler:       Renesas NC30WA V.5.00 Release 2
 *  Options:        -
 *
 *  --------------------------------------------------------------------
 *
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
 *      or try http:// www.sixo.de
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
 * Revision 2.1  2007/03/26 23:21:02  tuberkel
 * Improved Focus handling
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 * Revision 1.1  2006/02/18 15:20:22  Ralf
 * - BIKE_MOTOBAU: statistics and lapcounter device added
 *
 *
 ************************************************************************ */



// include
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
#include "displdrv.h"
#include "timedate.h"
#include "led.h"
#include "beep.h"



// defines for device data


// Screen Appearance:  +----.----.----.----.-+
// (21 Chars width)    |#01 01:23   #09 00:59|
//                     |#02 01:21   #10 01:10|
//                     |#03 01:17   #11 01:08|
//                     |#04 01:34   #12 01:45|
//                     |#05 01:12   #13 00:00|
//                     |#06 01:27   #14 00:00|
//                     |#07 01:22   #15 00:00|
//                     |#08 01:19   #16 00:00|
//                     +----.----.----.----.-+




// device object data
static DEVDATA          LapCntDevice;       // LapCounter Device object itself
extern STRING far       szDevName[];        // device names
extern SYSFLAGS_TYPE    gSystemFlags;      // system parameters

extern UINT16  wMilliSecCounter;        // high resolution short distance timer, ms,  max  65 sec
extern UINT16  wSecCounter;             // low  resolution long  distance timer, sec, max. 18 h



// non public prototypes
void    LapCntDeviceShow(BOOL fShow);
ERRCODE LapCntDeviceStateMachine(MESSAGE Msg);
ERRCODE LapCntDeviceSetFocus(UINT8 bLap);
ERRCODE LapCntDeviceResetMsg(MESSAGE Msg);
ERRCODE LapCntStartStop(MESSAGE Msg);


// lap timer text objects
#define LAP_TXTLEN  10                                      // we use 9 bytes for '#16 99:59' (+ NULL char)

static TEXTOBJECT   LapCntTxtObj[LAPS_MAX];                 // 16 lap objects
static CHAR         szLapCntText[LAPS_MAX][LAP_TXTLEN];     // 16 text buffers a 10 bytes

TIME_TYPE_LL LapCntTime[LAPS_MAX];                   // original values from EEPROM
LCSTATE_TYPE LapCounterState;                        // LapCounter state

static TEXTOBJECT   TestObject;
static CHAR         szTest;

/* text object table */
static const TEXTOBJECT_INITTYPE TextObject[] =
{
    /*object              X   Y  Font            H   W  align       format    string                 state    */
    /*----------------    -   -  -------------  --- --- ----------- --------- -------------------    -------- */
    { &LapCntTxtObj[0],   0,  0, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[0][0],  OC_DISPL },
    { &LapCntTxtObj[1],   0,  8, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[1][0],  OC_DISPL },
    { &LapCntTxtObj[2],   0, 16, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[2][0],  OC_DISPL },
    { &LapCntTxtObj[3],   0, 24, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[3][0],  OC_DISPL },
    { &LapCntTxtObj[4],   0, 32, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[4][0],  OC_DISPL },
    { &LapCntTxtObj[5],   0, 40, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[5][0],  OC_DISPL },
    { &LapCntTxtObj[6],   0, 48, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[6][0],  OC_DISPL },
    { &LapCntTxtObj[7],   0, 56, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[7][0],  OC_DISPL },

    { &LapCntTxtObj[8],  72,  0, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[8][0],  OC_DISPL },
    { &LapCntTxtObj[9],  72,  8, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[9][0],  OC_DISPL },
    { &LapCntTxtObj[10], 72, 16, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[10][0], OC_DISPL },
    { &LapCntTxtObj[11], 72, 24, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[11][0], OC_DISPL },
    { &LapCntTxtObj[12], 72, 32, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[12][0], OC_DISPL },
    { &LapCntTxtObj[13], 72, 40, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[13][0], OC_DISPL },
    { &LapCntTxtObj[14], 72, 48, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[14][0], OC_DISPL },
    { &LapCntTxtObj[15], 72, 56, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[15][0], OC_DISPL }

};










/***********************************************************************
 *  FUNCTION:       LapCntDeviceInit
 *  DESCRIPTION:    all initial stuff for all objects
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LapCntDeviceInit(void)
{
    int i;
    ERRCODE  RValue = ERR_OK;

    // device main data
    LapCntDevice.eDevID       = DEVID_LAPCNT;
    LapCntDevice.szDevName    = szDevName[DEVID_LAPCNT];
    LapCntDevice.fFocused     = FALSE;
    LapCntDevice.fScreenInit  = FALSE;
    LapCntDevice.wDevState    = 0;

    // LapCounterState.fActive     = 1;    // default disabled, might be enabled via EEPROM setting
    // LapCounterState.cCurrentLap = 0;    // default start with 0; might be changed via EEPROM setting

    /* initialize text objects */
    for (i = 0; i < ARRAY_SIZE(TextObject); i++)
    {
        ObjTextInit(TextObject[i].pObject,
                    TextObject[i].wOrgPosX,
                    TextObject[i].wOrgPosY,
                    TextObject[i].eFont,
                    TextObject[i].bWindHeight,
                    TextObject[i].bWindWidth,
                    TextObject[i].eAlign,
                    TextObject[i].eFormat,
                    TextObject[i].szText,
                    TextObject[i].bState);

        // check init process
        if ( RValue != ERR_OK )
        {
            ODS1(   DBG_SYS, DBG_WARNING, "LapCntDeviceInit(): TextObject[%u] not initialized!", i);
        }

    }
    // return
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       LapCntDeviceShow
 *  DESCRIPTION:    update devices appearence (new/update/clear)
 *                  by calling Show-Fct. of all relevant objects
 *  PARAMETER:      BOOL    TRUE = show objects,
 *                          FALSE = clear screen
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void LapCntDeviceShow(BOOL fShow)
{
    int     i;
    ERRCODE error = ERR_OK;
    MESSAGE NewMsg;                                         // for screen fresh message

    // update current time
    LapCntUpdateTime();

    // update all strings
    for ( i = 0; i < LAPS_MAX; i++)
    {
        // check: used/unused lapcounter?
        if (  (LapCntTime[i].bMin          == 0 )    // minutes still reseted?
            &&(LapCntTime[i].bSec          == 0 ) )   // seconds still reseted?
            //&&(LapCounterState.cCurrentLap != i ) )  // this lap is not focused?
             sprintf( szLapCntText[i],"#%02u --:--", i+1 );     // show unsed
        else sprintf( szLapCntText[i],"#%02u %02u:%02u", i+1, LapCntTime[i].bMin, LapCntTime[i].bSec );
    }

    // update current focused lap
    LapCntDeviceSetFocus(LapCounterState.cCurrentLap);

    // show mode of complete device
    if (fShow == TRUE)                                      // 'show' screen ?
    {
        // refresh dynamic values (only if not in edit mode)

        // do we have to repaint all?
        if (LapCntDevice.fScreenInit == FALSE)
        {
            /* initialize all text objects */
            for (i = 0; i < ARRAY_SIZE(TextObject); i++)
                ObjTextShow( TextObject[i].pObject);

            // reset init state
            LapCntDevice.fScreenInit = TRUE;
        }
        else
        {
            // NO, repaint only potential changed stuff

            // repaint all text objects
            for (i = 0; i < ARRAY_SIZE(TextObject); i++)
                ObjTextShow( TextObject[i].pObject);
        }
    }
    else                                                    // 'clear' screen!
    {
        DisplClearScreen(0x0);                              // leave a clear screen for next device
        LapCntDevice.fScreenInit = FALSE;                      // reset devices screen state
    }
}




/***********************************************************************
 *  FUNCTION:       LapCntDeviceMsgEntry
 *  DESCRIPTION:    Receive Message Handler called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LapCntDeviceMsgEntry(MESSAGE GivenMsg)
{
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID  MsgId;
    MESSAGE     NewMsg;


    // analyze message: it's for us?
    MsgId = MSG_ID(GivenMsg);                                                   // get message id
    switch (MsgId)
    {
        case MSG_GET_FOCUS:
        {
            // Some device want's to get the focus:
            // If we've currently got the focus, we'll
            // answer to SET his focus!
            if ( LapCntDevice.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_LAPCNT]);
                MSG_BUILD_SETFOCUS(NewMsg,DEVID_SET,MSG_SENDER_ID(GivenMsg));   // build answer message
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                    // send answer message
                LapCntDevice.fFocused = FALSE;                                     // clear our focus
                LapCntDeviceShow(FALSE);                                           // clear our screen
                RValue = ERR_MSG_PROCESSED;
            }
        } break;
        case MSG_SET_FOCUS:
        {
            // messsage for us?
            if (MSG_RECEIVER_ID(GivenMsg) == DEVID_LAPCNT)
            {
                // Someone wants us to take the focus?
                // We assume, that nobody else has the focus
                // and we've got the the screen now for us!
                if (  (LapCntDevice.fFocused     == TRUE        )
                    &&(MSG_RECEIVER_ID(GivenMsg) == DEVID_LAPCNT) )
                {
                    ODS2(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but it already has focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_LAPCNT] );
                }
                else
                {/*ODS2(   DBG_SYS, DBG_INFO,
                            "FOCUS: %s -> %s!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_SET]) */
                }
                LapCntDevice.fFocused = TRUE;                           // set our focus
                LapCntDeviceShow(TRUE);                                 // show our screen immediatly
                gSystemFlags.flags.ActDevNr = DEVID_LAPCNT;             // save device# for restore
                RValue = ERR_MSG_PROCESSED;
             }
             else
             {
                if ( LapCntDevice.fFocused == TRUE )
                {
                    // Some other device should be given the focus,
                    // BUT WE'VE GOT THE FOCUS!
                    // It should first ask US to give the focus! :-(
                    ODS3(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but %s had focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[MSG_RECEIVER_ID(GivenMsg)],
                            szDevName[DEVID_LAPCNT]);
                    LapCntDevice.fFocused = FALSE;                        // loose our focus
                    ODS1(   DBG_SYS, DBG_WARNING,
                            "%s now loosing focus :-( ",
                            szDevName[DEVID_LAPCNT]);
                    RValue = ERR_MSG_NOT_PROCESSED;
                }
             }
        } break;
    }

    // ----------------------------------------------------------------
    // this part is only for FOCUSED time AND msg not already processed
    if (  (LapCntDevice.fFocused == TRUE )
        &&(RValue == ERR_MSG_NOT_PROCESSED ) )
    {
        switch (MsgId)
        {
            case MSG_KEYS_PRESSED:
            case MSG_KEY_OK:
            case MSG_KEY_UP:
            case MSG_KEY_DOWN:
            case MSG_DPL_FLASH_ON:
            case MSG_DPL_FLASH_OFF:

                if( RValue == ERR_MSG_NOT_PROCESSED )
                    RValue = LapCntStartStop(GivenMsg);                         // try start/stop timer

                if( RValue == ERR_MSG_NOT_PROCESSED )
                    RValue = LapCntDeviceStateMachine(GivenMsg);                // try move focus

                if( RValue == ERR_MSG_NOT_PROCESSED )
                    RValue = LapCntDeviceResetMsg(GivenMsg);                    // try reset all counters


                if (  (RValue == ERR_MSG_NOT_PROCESSED                    )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN))     // try to give msg to next device
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYSHORT              ) )
                {
                    // give focus immediatly to next screen
                    LapCntDevice.fFocused = FALSE;                              // clear our focus
                    LapCntDeviceShow(FALSE);                                    // clear our screen
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_LAPCNT, DEVID_MONITOR);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                
                LapCntDeviceShow(TRUE);     // update screen anyway
                break;

            //case MSG_SCREEN_REFRESH:        /* standard refresh cycle */
            case MSG_TIMEDATE_SECOND_GONE:  /* additional blink lap trigger */            
                LapCntDeviceShow(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;
                                
            default: return ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}







/***********************************************************************
 *  FUNCTION:       LapCntDeviceStateMachine
 *  DESCRIPTION:    focus handles over all selectable objects
 *  PARAMETER:      message id (up/down)
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LapCntDeviceStateMachine(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                            // get message id
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;

    // move focus to previous lap timer?
    if (  (RValue == ERR_MSG_NOT_PROCESSED                )     // still unpocessed
        &&(MsgId  == MSG_KEY_UP                           )     // [UP]
        &&(  (MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED )     // now pressed
           ||(MSG_KEY_TRANSITION(Msg) == KEYTRANS_ON      ) ) ) // or longer pressed?
    {
        // decrement lap
        if ( LapCounterState.cCurrentLap > 0 )                  // underflow/wrap around?
             LapCounterState.cCurrentLap--;                     // NO:  set previous lap
        else LapCounterState.cCurrentLap = (LAPS_MAX-1);        // YES: set last lap
        LapCntDeviceSetFocus(LapCounterState.cCurrentLap);      // set focus
        LapCntDeviceShow(TRUE);                                 // immedeately show result
        RValue = ERR_MSG_PROCESSED;                             // processed!
        ODS1( DBG_SYS, DBG_INFO, "LapCnt decr to lap %u!", LapCounterState.cCurrentLap+1);
    }

    // move focus to next lap timer?
    if (  (RValue == ERR_MSG_NOT_PROCESSED                )     // still unpocessed
        &&(MsgId  == MSG_KEY_DOWN                         )     // [DOWN]
        &&(  (MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED )     // now pressed
           ||(MSG_KEY_TRANSITION(Msg) == KEYTRANS_ON      ) ) ) // or longer pressed?
    {
        if (LapCounterState.cCurrentLap < (LAPS_MAX-1) )        // wrap around?
             LapCounterState.cCurrentLap++;                     // NO:  set next lap
        else LapCounterState.cCurrentLap = 0;                   // YES: set first last
        LapCntDeviceSetFocus(LapCounterState.cCurrentLap);      // set focus
        LapCntDeviceShow(TRUE);                                 // immedeately show result
        RValue = ERR_MSG_PROCESSED;                             // processed!
        ODS1( DBG_SYS, DBG_INFO, "LapCnt incr. to lap %u!", LapCounterState.cCurrentLap+1);
    }
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       LapCntDeviceSetFocus
 *  DESCRIPTION:    focus handles over all selectable objects
 *  PARAMETER:      lap to be focused
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LapCntDeviceSetFocus(UINT8 bLap)
{
    int       i;
    ERRCODE RValue = ERR_MSG_NOT_PROCESSED;

    // clear any focus before setting new one
    for (i = 0; i < ARRAY_SIZE(TextObject); i++)
        TextObject[i].pObject->State.bits.fSelected = 0;

    // set focus to current lap
    TextObject[bLap].pObject->State.bits.fSelected = 1;

    // let the focus blink with 1 Hz, if lap counter active
    TextObject[bLap].pObject->State.bits.fSelected = 1;
    if (  (  LapCounterState.fActive  == 1 )
        &&( (LapCntTime[bLap].bSec%2) == 0 ) )
             TextObject[bLap].pObject->State.bits.fSelected = 0;
    
    return (RValue);    
}


/***********************************************************************
 *  FUNCTION:       LapCntDeviceResetMsg
 *  DESCRIPTION:    Handle the 'reset' messages for all lap timers
 *  PARAMETER:      MESSAGE
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LapCntDeviceResetMsg(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    static BOOL fLocked = FALSE;                    /* TRUE = key not yet relases */

    /* [OK] pressed+released long'? -> ITS FOR US! -> Re-enable Reset!
      Note: This code shall prevent a 'flickering info led' if the user doesn't
            release the key which would repeat the distance reset */
    if (  ( MsgId == MSG_KEY_OK                          )      /* OK key?  */
        &&( MSG_KEY_TRANSITION(Msg) == KEYTRANS_RELEASED )      /* right now released* */
        &&( MSG_KEY_DURATION(Msg) > KEYSAVE              )      /* has just shortly been pressed? */
        &&( fLocked == TRUE                              ) )    /* is not already active? */
    {
        fLocked = FALSE;                // Re-enable another Reset of distances
        RValue = ERR_MSG_PROCESSED;     // done!
    }

    /* user presses OK Button > 2 sec: focus & start edit */
    else if (  ( MsgId == MSG_KEY_OK                    )    /* [OK] pressed? */
             &&( MSG_KEY_TRANSITION(Msg) == KEYTRANS_ON )    /* still pressed? */
             &&( MSG_KEY_DURATION(Msg) > KEYSAVE        )    /* longer than 2 sec.? */
             &&( fLocked == FALSE                       ) )  /* key released since last reset? */
    {
        int i;

        // stop lap timer and reset selected lap
        LapCounterState.fActive = 0;
        LapCounterState.cCurrentLap = 0;

        // reset all lap timers
        for ( i = 0; i < (LAPS_MAX); i++ )
        {
            LapCntTime[i].bSec = 0;
            LapCntTime[i].bMin = 0;
        }

        // acknowledge to user
        BeepOk();                   /* beep ok */
        LEDOk();                    /* LED ok */
        fLocked = TRUE;             /* don't repeat this until key released */

        ODS( DBG_SYS, DBG_INFO, "LapCnt reseted!");

        // ready
        RValue = ERR_MSG_PROCESSED; /* done */
    }
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       LapCntStartStop
 *  DESCRIPTION:    Handle messages for start/stop the lap timer
 *  PARAMETER:      MESSAGE
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        This function tries to detect a short-pressed-OK-key!
 *                  This is done via detection of pressed- and relased
 *                  states. Only if the OK key is released after short
 *                  time, it is detected.
 *********************************************************************** */
ERRCODE LapCntStartStop(MESSAGE Msg)
{
    MESSAGE_ID  MsgId   = MSG_ID(Msg);              /* get message id */
    ERRCODE     RValue  = ERR_MSG_NOT_PROCESSED;    /* return value */
    MESSAGE     NewMsg;                             /* to generate show/hide messages */

    /* [OK] pressed+released shortly'? -> ITS FOR US! -> show 'Vehicle State' instead of TimeDate! */
    if (  ( MsgId == MSG_KEY_OK                          )      /* OK key?  */
        &&( MSG_KEY_TRANSITION(Msg) == KEYTRANS_RELEASED )      /* right now released* */
        &&( MSG_KEY_DURATION(Msg) < KEYSAVE              ) )    /* has just shortly been pressed? */
    {
        // start / stop lap timer
        if ( LapCounterState.fActive == 1 )
        {
            LapCounterState.fActive = 0;
            ODS( DBG_SYS, DBG_INFO, "LapCnt manually stopped!");
        }
        else
        {
            LapCounterState.fActive = 1;
            ODS( DBG_SYS, DBG_INFO, "LapCnt manually started!");
        }
    }

    return(RValue);
}




/***********************************************************************
 *  FUNCTION:       LapCntUpdateTime
 *  DESCRIPTION:    If LapCounter currently active, updates time stamp
 *                  of currently focused lap timer
 *                  Will be called cyclicely via timer ISR to enable
 *                  lap counter although not shown on screen!
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        LapCounterState.cCurrentLap (EEPROM value!) indicates current
 *                  focused lap.
 *********************************************************************** */
void LapCntUpdateTime( void )
{
    static UINT8 bLastSecond = 0;       // to prevent multiple time stamp increments
    UINT8        bThisSecond = 0;       // just a time stamp

    // check: LapCounter is active?
    if ( LapCounterState.fActive == 1 )
    {
        TimerGetSys_sec( bThisSecond );             // get current second
        if ( bThisSecond != bLastSecond )           // next second detected?
        {
            int i = LapCounterState.cCurrentLap;    // just a helper variable
            bLastSecond = bThisSecond;              // prevent any retry inside this second
            LapCntTime[i].bSec++;                   // increment lap counter

            // check max time format
            if ( LapCntTime[i].bSec > 59 )          // check seconds
            {
                LapCntTime[i].bSec = 0;             // wrap seconds
                LapCntTime[i].bMin++;               // incr minutes
                if ( LapCntTime[i].bMin > 99 )      // check minutes
                {
                    LapCntTime[i].bMin = 99;        // clip minutes
                    LapCounterState.fActive = 0;    // stop any lapcounter
                    ODS( DBG_SYS, DBG_INFO, "LapCnt stopped due to 99:59 timeout!!");
                }
            }
        }
    }
}



