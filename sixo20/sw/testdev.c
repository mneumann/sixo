/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-24 by Ralf Schwarzer
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
 * Revision 2.2  2009/07/15 09:05:29  tuberkel
 * - completely reviewed for GUI tests
 *
 * Revision 2.1  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
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


/* main switch for this module */
#if(TESTSCREEN==1)


/* device static objects */
static DEVDATA      TestScreenDev;


/* static headline text object */
static TEXTOBJECT   TextObj_Headline;


/* edit text object */
static EDITTEXTOBJECT   EditTextObj;
static CHAR             szEditTextDescr[]    = "EditText:";
static CHAR             szEditTextBuffer[20] = "abcdefgh";


/* edit number object */
static EDITNUMBEROBJECT EditNumObj;                 /* complete number object */
static UINT32           dwEditNumValue = 99000;     /* object value */


/* edit bool object */
static EDITBOOLOBJECT   EditBoolObj;                /* complete boolean object */
static BOOL             bEditBoolValue = FALSE;     /* object value */


/* external symbols */
extern UINT16           wMilliSecCounter;       // valid values: 0h .. ffffh
extern STRING far       szDevName[];            // device names
extern SYSFLAGS_TYPE    gSystemFlags;           /* system parameters */


/* some helper / buffer variables */
static UINT32           dwEditBuffer;
static CHAR             szBuffer[32];
static BOOL             bEditBoolBuffer;

/* internal prototypes */
ERRCODE TestScreenSetFocus(MESSAGE GivenMsg);





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
    TestScreenDev.eDevID       = DEVID_TESTSCREEN;
    TestScreenDev.szDevName    = szDevName[DEVID_TESTSCREEN];
    TestScreenDev.fFocused     = FALSE;
    TestScreenDev.fScreenInit  = FALSE;

    /* Headline */
    ObjTextInit(    &TextObj_Headline,
                    0, 0, DPLFONT_6X8, 1, 21, TXT_CENTER, TXT_INVERS,
                    RESTXT_TEST_GUITEST,
                    OC_DISPL );

    /* edit text field */
    ObjEditTextInit(    &EditTextObj,       // object to be initilized
                        0, 10,              // X / Y origin
                        DPLFONT_6X8,        // used font
                        21,                 // overall window width in chars
                        szEditTextBuffer,   // pointer to original value
                        RESTXT_TEST_ET_DSC, // descriptor string
                        10,                 // edit field length in chars
                        CHARL_LOWER,        // available character list
                        OC_DISPL | OC_SELECT | OC_EDIT );   // init states

    /* number object */
    ObjEditNumInit( &EditNumObj,            // object to be initilized
                    0, 20,                  // X / Y origin
                    DPLFONT_6X8,            // used font
                    21,                     // overall window width in chars
                    &dwEditNumValue,        // pointer to original value
                    &dwEditBuffer,          // pointer to work copy buffer
                    eULONG,0L, 999999L, 0L, // type of number, min, max Limits, stepsize
                    eDez, eColumn, 0,       // dez display format, column edit mode, comma pos
                    RESTXT_TEST_EN_DSC,     // descriptor string
                    RESTXT_TEST_EN_UNIT,    // units string
                    6,                      // edit field length in chars
                    OC_DISPL | OC_SELECT | OC_EDIT ); // init states

    /* number object */
    ObjEditBoolInit(&EditBoolObj,           // object to be initilized
                    0, 30,                  // X / Y origin
                    DPLFONT_6X8,            // used font
                    21,                     // overall window width in chars
                    &bEditBoolValue,        // pointer to original value
                    &bEditBoolBuffer,       // pointer to work copy buffer
                    RESTXT_TEST_EB_DSC,     // descriptor string
                    OC_DISPL | OC_SELECT | OC_EDIT ); // init states

    /* set edit text object focused per default */
    EditTextObj.State.bits.fSelected = TRUE;

    return ERR_OK;
}

/***********************************************************************
 *  FUNCTION:       TestScreenShow
 *  DESCRIPTION:    bring updated 'test screen device' to display
 *                  by calling Show-Fct. of all objects
 *  PARAMETER:      fShow   if TRUE, then shows all
 *                          if FALSE, then clears all
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void TestScreenShow(BOOL fShow)
{
    /* its on screen? */
    if (fShow == TRUE)
    {
        /* show objects */
        ObjTextShow    ( &TextObj_Headline );
        ObjEditTextShow( &EditTextObj, SHOW_ALL);
        ObjEditNumShow ( &EditNumObj, SHOW_ALL);
        ObjEditBoolShow( &EditBoolObj, SHOW_ALL);
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
                        szDevName[DEVID_TESTSCREEN]);
                MSG_BUILD_SETFOCUS(NewMsg,DEVID_TESTSCREEN,MSG_SENDER_ID(GivenMsg));   /* build answer message */
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                     /* send answer message */
                TestScreenDev.fFocused = FALSE;                                  /* clear our focus */
                TestScreenShow(TRUE);                                            /* clear our screen */
                RValue = ERR_MSG_PROCESSED;
            }
        } break;
        case MSG_SET_FOCUS:
        {
            /* messsage for us? */
            if (MSG_RECEIVER_ID(GivenMsg) == DEVID_TESTSCREEN)
            {
                /*  Someone wants us to take the focus?
                    We assume, that nobody else has the focus
                    and we've got the the screen now for us! */
                if (  (TestScreenDev.fFocused   == TRUE             )
                    &&(MSG_RECEIVER_ID(GivenMsg) == DEVID_TESTSCREEN) )
                    ODS2(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but it already has focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_TESTSCREEN]);
                else /*
                    ODS2(   DBG_SYS, DBG_INFO,
                            "FOCUS: %s -> %s!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_TESTSCREEN]) */ ;
                TestScreenDev.fFocused = TRUE;                          /* set our focus */
                TestScreenShow(FALSE);                                  /* show our screen */
                gSystemFlags.flags.ActDevNr = DEVID_TESTSCREEN;               /* save device# for restore */
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
                            szDevName[DEVID_TESTSCREEN]);
                    TestScreenDev.fFocused = FALSE;                        /* loose our focus */
                    ODS1(   DBG_SYS, DBG_WARNING,
                            "%s now loosing focus :-( ",
                            szDevName[DEVID_TESTSCREEN]);
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
            /* ------------------------------------------------------------ */
            /* The system tells focused device to refresh its screen! */
            case MSG_SCREEN_REFRESH:
                TestScreenShow(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;

            /* ------------------------------------------------------------ */
            /* all these messages handled by objects */
            case MSG_DPL_FLASH_ON:
            case MSG_DPL_FLASH_OFF:
            case MSG_KEY_OK:
            case MSG_KEYS_PRESSED:
            case MSG_KEY_UP:
            case MSG_KEY_DOWN:

                /* let one of the objects use the msg */
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = ObjEditTextMsgEntry(&EditTextObj, GivenMsg);
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = ObjEditNumMsgEntry(&EditNumObj, GivenMsg);
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = ObjEditBoolMsgEntry(&EditBoolObj, GivenMsg);

                /* try to move focus to next object */
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = TestScreenSetFocus(GivenMsg);

                /* try to give focus to next device */
                if (  (RValue == ERR_MSG_NOT_PROCESSED      )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN))
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYSHORT              ) )
                {
                    /* give focus immediatly to IntroScreen */
                    TestScreenDev.fFocused = FALSE;                         /* clear our focus */
                    TestScreenShow(FALSE);                                  /* clear our screen */

                    // just test: stay inside DEVID_TESTSCREEN!
                    //MSG_BUILD_SETFOCUS(NewMsg, DEVID_TESTSCREEN, DEVID_TESTSCREEN);
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_TESTSCREEN, DEVID_INTRO);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }

                /* something changed -> Refresh! */
                TestScreenShow(TRUE);
                break;

            /* ------------------------------------------------------------ */
            default:
                return ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}

/***********************************************************************
 *  FUNCTION:       TestScreenSetFocus
 *  DESCRIPTION:    focus handles over all selectable objects
 *  PARAMETER:      new state to be set
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE TestScreenSetFocus(MESSAGE GivenMsg)
{
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID  MsgId;
    MsgId = MSG_ID(GivenMsg);        /* get message id */

    // check: use only first pressed msg!
    if(MSG_KEY_TRANSITION(GivenMsg) != KEYTRANS_PRESSED)
        return (RValue);

    // check: none of the objects is still in edit mode?
    if (  (EditTextObj.State.bits.fEditActive == FALSE)
        &&(EditNumObj.State.bits.fEditActive  == FALSE)
        &&(EditBoolObj.State.bits.fEditActive == FALSE) )
    {
        // ok: try to move the focus down:
        if (MsgId == MSG_KEY_DOWN)
        {
            // msg processed anyway
            RValue = ERR_MSG_PROCESSED;

            // try to move from EditTextObj => EditNumObj
            if (EditTextObj.State.bits.fSelected == TRUE)
            {   EditTextObj.State.bits.fSelected = FALSE;
                EditNumObj.State.bits.fSelected  = TRUE;
                EditBoolObj.State.bits.fSelected = FALSE;
            }
            // try to move from EditNumObj => EditBoolObj
            else if (EditNumObj.State.bits.fSelected == TRUE)
            {   EditTextObj.State.bits.fSelected = FALSE;
                EditNumObj.State.bits.fSelected  = FALSE;
                EditBoolObj.State.bits.fSelected = TRUE;
            }
            else
            {   // already reached lowest element
            }
        }
        // ok: try to move the focus down:
        else if (MsgId == MSG_KEY_UP)
        {
            // msg processed anyway
            RValue = ERR_MSG_PROCESSED;

            // try to move from EditBoolObj => EditNumObj
            if (EditBoolObj.State.bits.fSelected == TRUE)
            {   EditTextObj.State.bits.fSelected = FALSE;
                EditNumObj.State.bits.fSelected  = TRUE;
                EditBoolObj.State.bits.fSelected = FALSE;
            }
            // try to move from EditNumObj => EditTextObj
            else if (EditNumObj.State.bits.fSelected == TRUE)
            {   EditTextObj.State.bits.fSelected = TRUE;
                EditNumObj.State.bits.fSelected  = FALSE;
                EditBoolObj.State.bits.fSelected = FALSE;
            }
            else
            {   // already reached most top element
            }
        }
        else
        {   // not a UP/DOWN msg!
        }
    }
    return (RValue);
}




#endif // TESTSCREEN
