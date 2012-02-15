/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2006-01-03 by Ralf Schwarzer
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
 *      Arnold Neugebauer                   Ralf Schwarzer
 *      Stöckener Str. 115                  Hudeplan 37 A
 *      30419 Hannover                      30453 Hannover
 *      arnold.neugebauer@web.de            ralf.schwarzer@htp-tel.de
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
 * Revision 3.5  2012/02/15 07:32:43  tuberkel
 * Objects-API reviewed (no functional changes)
 *
 * Revision 3.4  2012/02/08 03:55:24  tuberkel
 * KEY_TIMING parameter names reviewed/changed
 *
 * Revision 3.3  2012/02/06 20:54:14  tuberkel
 * Just renamed all 'Devices' function prefixes for better readability
 *
 * Revision 3.2  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 3.1  2012/01/14 08:28:42  tuberkel
 * Message-IDs shortened / reviewed
 *
 * Revision 3.0  2010/11/07 13:55:50  tuberkel
 * V30 Preparations - no changes
 *
 * Revision 2.5  2010/11/07 13:55:15  tuberkel
 * Test re-added - because of bug in CVS
 *
 * Revision 2.3  2009/07/19 12:30:36  tuberkel
 * - ObjectInit reviewed
 *
 * Revision 2.2  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
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
#include "digoutdr.h"
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
static DEVDATA          LCDev_;       // LapCounter Device object itself
extern STRING far       szDevName[];        // device names
extern DEVFLAGS1_TYPE    gDeviceFlags1;      // system parameters

extern UINT16  wMilliSecCounter;        // high resolution short distance timer, ms,  max  65 sec
extern UINT16  wSecCounter;             // low  resolution long  distance timer, sec, max. 18 h



// non public prototypes
void    LCDev_Show(BOOL fShow);
ERRCODE LCDev_StateMachine(MESSAGE Msg);
ERRCODE LCDev_SetFocus(UINT8 bLap);
ERRCODE LCDev_ResetMsg(MESSAGE Msg);
ERRCODE LCDev_StartStop(MESSAGE Msg);


// lap timer text objects
#define LAP_TXTLEN  10                                      // we use 9 bytes for '#16 99:59' (+ NULL char)

static OBJ_TEXTST   LapCntTxtObj[LAPS_MAX];                 // 16 lap objects
static CHAR         szLapCntText[LAPS_MAX][LAP_TXTLEN];     // 16 text buffers a 10 bytes

TIME_TYPE_LL LapCntTime[LAPS_MAX];                   // original values from EEPROM
LCSTATE_TYPE LapCounterState;                        // LapCounter state

static OBJ_TEXTST   TestObject;
static CHAR         szTest;

/* text object table */
static const OBJ_TEXTST_INIT TextObjInit[] =
{
    /*object              X   Y  Font            H   W  align       format    string                 state    */
    /*----------------    -   -  -------------  --- --- ----------- --------- -------------------    -------- */
    { &LapCntTxtObj[0],   0,  0, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[0][0],  OC_DISPL | OC_DYN },
    { &LapCntTxtObj[1],   0,  8, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[1][0],  OC_DISPL | OC_DYN },
    { &LapCntTxtObj[2],   0, 16, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[2][0],  OC_DISPL | OC_DYN },
    { &LapCntTxtObj[3],   0, 24, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[3][0],  OC_DISPL | OC_DYN },
    { &LapCntTxtObj[4],   0, 32, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[4][0],  OC_DISPL | OC_DYN },
    { &LapCntTxtObj[5],   0, 40, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[5][0],  OC_DISPL | OC_DYN },
    { &LapCntTxtObj[6],   0, 48, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[6][0],  OC_DISPL | OC_DYN },
    { &LapCntTxtObj[7],   0, 56, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[7][0],  OC_DISPL | OC_DYN },

    { &LapCntTxtObj[8],  72,  0, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[8][0],  OC_DISPL | OC_DYN },
    { &LapCntTxtObj[9],  72,  8, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[9][0],  OC_DISPL | OC_DYN },
    { &LapCntTxtObj[10], 72, 16, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[10][0], OC_DISPL | OC_DYN },
    { &LapCntTxtObj[11], 72, 24, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[11][0], OC_DISPL | OC_DYN },
    { &LapCntTxtObj[12], 72, 32, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[12][0], OC_DISPL | OC_DYN },
    { &LapCntTxtObj[13], 72, 40, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[13][0], OC_DISPL | OC_DYN },
    { &LapCntTxtObj[14], 72, 48, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[14][0], OC_DISPL | OC_DYN },
    { &LapCntTxtObj[15], 72, 56, DPLFONT_6X8,    1,  9, TXT_LEFT,  TXT_NORM, &szLapCntText[15][0], OC_DISPL | OC_DYN }

};
#define TEXTOBJECTLISTSIZE   (sizeof(TextObjInit)/sizeof(OBJ_TEXTST_INIT))


/* this devices object focus handling - list of all objects */
static const void far * ObjectList[] =
{
    // all text objects
    (void far *) &LapCntTxtObj[0],
    (void far *) &LapCntTxtObj[1],
    (void far *) &LapCntTxtObj[2],
    (void far *) &LapCntTxtObj[3],
    (void far *) &LapCntTxtObj[4],
    (void far *) &LapCntTxtObj[5],
    (void far *) &LapCntTxtObj[6],
    (void far *) &LapCntTxtObj[7],
    (void far *) &LapCntTxtObj[8],
    (void far *) &LapCntTxtObj[9],
    (void far *) &LapCntTxtObj[10],
    (void far *) &LapCntTxtObj[11],
    (void far *) &LapCntTxtObj[12],
    (void far *) &LapCntTxtObj[13],
    (void far *) &LapCntTxtObj[14],
    (void far *) &LapCntTxtObj[15],
};
#define OBJECTLIST_SIZE   (sizeof(ObjectList)/sizeof(OBJ_STATE)/sizeof(void far *))







/***********************************************************************
 *  FUNCTION:       LCDev_Init
 *  DESCRIPTION:    all initial stuff for all objects
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LCDev_Init(void)
{
    int i;
    ERRCODE  RValue = ERR_OK;

    /* device main data */
    LCDev_.eDevID       = DEVID_LAPCNT;
    LCDev_.szDevName    = szDevName[DEVID_LAPCNT];
    LCDev_.fFocused     = FALSE;
    LCDev_.fScreenInit  = FALSE;
    LCDev_.wDevState    = 0;

    // LapCounterState.fActive     = 1;    // default disabled, might be enabled via EEPROM setting
    // LapCounterState.cCurrentLap = 0;    // default start with 0; might be changed via EEPROM setting

    /* initialize all objects of any type */
    DevObjInit( &LCDev_, &TextObjInit,   TEXTOBJECTLISTSIZE, OBJT_TXT  );

    /* initialize this devices objects list */
    LCDev_.Objects.ObjList       = ObjectList;
    LCDev_.Objects.ObjCount      = OBJECTLIST_SIZE;
    LCDev_.Objects.FirstSelObj   = DevObjGetFirstSelectable(&LCDev_,ObjectList,OBJECTLIST_SIZE);
    LCDev_.Objects.LastSelObj    = DevObjGetLastSelectable (&LCDev_,ObjectList,OBJECTLIST_SIZE);

    /* reset focus handling to start values */
    DevObjFocusReset( &LCDev_, ObjectList, OBJECTLIST_SIZE );

    // return
    ODS( DBG_SYS, DBG_INFO, "- LCDev_Init() done!");
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       LCDev_Show
 *  DESCRIPTION:    update devices appearence (new/update/clear)
 *                  by calling Show-Fct. of all relevant objects
 *  PARAMETER:      BOOL    TRUE = show objects,
 *                          FALSE = clear screen
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void LCDev_Show(BOOL fShow)
{
    int     i;
    UINT8   ShowMode;
    //ERRCODE error = ERR_OK;
    //MESSAGE NewMsg;                                         // for screen fresh message

    // update current time
    LCDev_UpdTime();

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
    LCDev_SetFocus(LapCounterState.cCurrentLap);

    // show mode of complete device
    if (fShow == TRUE)                                      // 'show' screen ?
    {
        // show first time? ==> clear screen first!
        if (LCDev_.fScreenInit == FALSE)
        {
            DisplClearScreen(0x00);
            LCDev_.fScreenInit = TRUE;
            ShowMode = SHOW_ALL;                   // repaint all stuff
        }
        else
        {   ShowMode = SHOW_EDIT | SHOW_CURSOR;    // repaint only potential changed stuff
        }

        // FOR DISPLAY OBJECTS TEST ONLY: clear & proof display mode of objects!
        //DisplClearScreen(0xaa);

        /* process complete (active) object to show all objects */
        DevObjShow( &LCDev_,
                    LCDev_.Objects.ObjList,
                    LCDev_.Objects.ObjCount,
                    ShowMode );

    }
    else
    {
        // clear screen
        DisplClearScreen(0x0);

        // reset states of all objects of this device
        DevObjClearState(  &LCDev_,
                            LCDev_.Objects.ObjList,
                            LCDev_.Objects.ObjCount,
                            OS_DISPL | OS_EDIT );

        // set overall device state to 'not init'
        LCDev_.fScreenInit = FALSE;                      // reset devices screen state
    }
}




/***********************************************************************
 *  FUNCTION:       LCDev_MsgEntry
 *  DESCRIPTION:    Receive Message Handler called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LCDev_MsgEntry(MESSAGE GivenMsg)
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
            if ( LCDev_.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_LAPCNT]);
                MSG_BUILD_SETFOCUS(NewMsg,DEVID_SET,MSG_SENDER_ID(GivenMsg));   // build answer message
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                    // send answer message
                LCDev_.fFocused = FALSE;                                     // clear our focus
                LCDev_Show(FALSE);                                           // clear our screen
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
                if (  (LCDev_.fFocused     == TRUE        )
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
                LCDev_.fFocused = TRUE;                           // set our focus
                LCDev_Show(TRUE);                                 // show our screen immediatly
                gDeviceFlags1.flags.ActDevNr = DEVID_LAPCNT;             // save device# for restore
                RValue = ERR_MSG_PROCESSED;
             }
             else
             {
                if ( LCDev_.fFocused == TRUE )
                {
                    // Some other device should be given the focus,
                    // BUT WE'VE GOT THE FOCUS!
                    // It should first ask US to give the focus! :-(
                    ODS3(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but %s had focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[MSG_RECEIVER_ID(GivenMsg)],
                            szDevName[DEVID_LAPCNT]);
                    LCDev_.fFocused = FALSE;                        // loose our focus
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
    if (  (LCDev_.fFocused == TRUE )
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
                    RValue = LCDev_StartStop(GivenMsg);                         // try start/stop timer

                if( RValue == ERR_MSG_NOT_PROCESSED )
                    RValue = LCDev_StateMachine(GivenMsg);                // try move focus

                if( RValue == ERR_MSG_NOT_PROCESSED )
                    RValue = LCDev_ResetMsg(GivenMsg);                    // try reset all counters


                if (  (RValue == ERR_MSG_NOT_PROCESSED                    )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN))     // try to give msg to next device
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYTM_PRESSED_SHORT              ) )
                {
                    // give focus immediatly to next screen
                    LCDev_.fFocused = FALSE;                              // clear our focus
                    LCDev_Show(FALSE);                                    // clear our screen
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_LAPCNT, DEVID_MONITOR);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }

                LCDev_Show(TRUE);     // update screen anyway
                break;

            //case MSG_SCREEN_RFRSH:        /* standard refresh cycle */
            case MSG_SECOND_GONE:  /* additional blink lap trigger */
                LCDev_Show(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;

            default: return ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}







/***********************************************************************
 *  FUNCTION:       LCDev_StateMachine
 *  DESCRIPTION:    focus handles over all selectable objects
 *  PARAMETER:      message id (up/down)
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LCDev_StateMachine(MESSAGE Msg)
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
        LCDev_SetFocus(LapCounterState.cCurrentLap);      // set focus
        LCDev_Show(TRUE);                                 // immedeately show result
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
        LCDev_SetFocus(LapCounterState.cCurrentLap);      // set focus
        LCDev_Show(TRUE);                                 // immedeately show result
        RValue = ERR_MSG_PROCESSED;                             // processed!
        ODS1( DBG_SYS, DBG_INFO, "LapCnt incr. to lap %u!", LapCounterState.cCurrentLap+1);
    }
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       LCDev_SetFocus
 *  DESCRIPTION:    focus handles over all selectable objects
 *  PARAMETER:      lap to be focused
 *  RETURN:         -
 *  COMMENT:        This is aspecial focus handler, which lets the
 *                  object (current lap) blink without beeing in edit mode!
 *********************************************************************** */
ERRCODE LCDev_SetFocus(UINT8 bLap)
{
    int       i;
    ERRCODE RValue = ERR_MSG_NOT_PROCESSED;

    // clear any focus before setting new one
    for (i = 0; i < ARRAY_SIZE(TextObjInit); i++)
        TextObjInit[i].fpObject->State.bits.fSelected = 0;

    // set focus to current lap
    TextObjInit[bLap].fpObject->State.bits.fSelected = 1;

    // let the focus blink with 1 Hz, if lap counter active
    TextObjInit[bLap].fpObject->State.bits.fSelected = 1;
    if (  (  LapCounterState.fActive  == 1 )
        &&( (LapCntTime[bLap].bSec%2) == 0 ) )
             TextObjInit[bLap].fpObject->State.bits.fSelected = 0;

    return (RValue);
}


/***********************************************************************
 *  FUNCTION:       LCDev_ResetMsg
 *  DESCRIPTION:    Handle the 'reset' messages for all lap timers
 *  PARAMETER:      MESSAGE
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LCDev_ResetMsg(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    static BOOL fLocked = FALSE;                    /* TRUE = key not yet relases */

    /* [OK] pressed+released long'? -> ITS FOR US! -> Re-enable Reset!
      Note: This code shall prevent a 'flickering info led' if the user doesn't
            release the key which would repeat the distance reset */
    if (  ( MsgId == MSG_KEY_OK                          )      /* OK key?  */
        &&( MSG_KEY_TRANSITION(Msg) == KEYTRANS_RELEASED )      /* right now released* */
        &&( MSG_KEY_DURATION(Msg) > KEYTM_PRESSED_VLONG              )      /* has just shortly been pressed? */
        &&( fLocked == TRUE                              ) )    /* is not already active? */
    {
        fLocked = FALSE;                // Re-enable another Reset of distances
        RValue = ERR_MSG_PROCESSED;     // done!
    }

    /* user presses OK Button > 2 sec: focus & start edit */
    else if (  ( MsgId == MSG_KEY_OK                    )    /* [OK] pressed? */
             &&( MSG_KEY_TRANSITION(Msg) == KEYTRANS_ON )    /* still pressed? */
             &&( MSG_KEY_DURATION(Msg) > KEYTM_PRESSED_VLONG        )    /* longer than 2 sec.? */
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
        Beep_SignalOk();                   /* beep ok */
        LED_SignalOk();                    /* LED ok */
        fLocked = TRUE;             /* don't repeat this until key released */

        ODS( DBG_SYS, DBG_INFO, "LapCnt reseted!");

        // ready
        RValue = ERR_MSG_PROCESSED; /* done */
    }
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       LCDev_StartStop
 *  DESCRIPTION:    Handle messages for start/stop the lap timer
 *  PARAMETER:      MESSAGE
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        This function tries to detect a short-pressed-OK-key!
 *                  This is done via detection of pressed- and relased
 *                  states. Only if the OK key is released after short
 *                  time, it is detected.
 *********************************************************************** */
ERRCODE LCDev_StartStop(MESSAGE Msg)
{
    MESSAGE_ID  MsgId   = MSG_ID(Msg);              /* get message id */
    ERRCODE     RValue  = ERR_MSG_NOT_PROCESSED;    /* return value */
    MESSAGE     NewMsg;                             /* to generate show/hide messages */

    /* [OK] pressed+released shortly'? -> ITS FOR US! -> show 'Vehicle State' instead of TimeDate! */
    if (  ( MsgId == MSG_KEY_OK                          )      /* OK key?  */
        &&( MSG_KEY_TRANSITION(Msg) == KEYTRANS_RELEASED )      /* right now released* */
        &&( MSG_KEY_DURATION(Msg) < KEYTM_PRESSED_VLONG              ) )    /* has just shortly been pressed? */
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
 *  FUNCTION:       LCDev_UpdTime
 *  DESCRIPTION:    If LapCounter currently active, updates time stamp
 *                  of currently focused lap timer
 *                  Will be called cyclicely via timer ISR to enable
 *                  lap counter although not shown on screen!
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        LapCounterState.cCurrentLap (EEPROM value!) indicates current
 *                  focused lap.
 *********************************************************************** */
void LCDev_UpdTime( void )
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



