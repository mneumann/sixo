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
 *  Module:         intro devices
 *  Purpose:        handles the intro screen device with all
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
 *  This software is copyright � 2001-2004 by N&K Development, Germany
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
 *      St�ckener Str. 115                  Knickstr. 10
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
 * Revision 1.1  2006/03/18 21:53:07  tuberkel
 * Initial revision
 *
 * Revision 1.8  2006/03/18 08:46:08  Ralf
 * changed logo handling
 *
 * Revision 1.7  2006/02/18 14:44:44  Ralf
 * - added MOTOBAU logo
 *
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
#include "sysparam.h"
#include "bitmaps.h"




/* external symbols */
extern UINT16           wMilliSecCounter;   /* valid values: 0h .. ffffh */
extern STRING far       szDevName[];        /* device names */
extern SYSFLAGS_TYPE    gSystemFlags;       /* system parameters */
extern SWVERS_TYPE      gSWID;              /* software id from eeprom */
extern UINT8            gLogoSelection;     /* selected logo */
extern BIKE_TYPE        gBikeType;          /* bike type selcetion */

/* external bitmaps */
extern const unsigned char far * fpBikeLogos[];


/* device static objects */
static DEVDATA      IntroScreenDev;         /* this device */
static BMPOBJECT    SixoLogoBmpObj;         /* logo object */
static TEXTOBJECT   SWVersionTxtObj;        /* version text object */
static unsigned char szSWVersion[22];       /* buffer to contain formated sw id */




/* internal prototypes */
ERRCODE IntroScreenChangeLogo(MESSAGE Msg);



/***********************************************************************
 *  FUNCTION:       IntroScreenInit
 *  DESCRIPTION:    all initial stuff for all objects of
 *                  'intro screen device'
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE IntroScreenInit(void)
{
    /* device main data */
    IntroScreenDev.eDevID       = DEVID_INTRO;
    IntroScreenDev.szDevName    = szDevName[DEVID_INTRO];
    IntroScreenDev.fFocused     = FALSE;
    IntroScreenDev.fScreenInit  = FALSE;

    /* init objects */
    ObjBmpInit( &SixoLogoBmpObj,                    /* logo bitmap */
                0, 0, 128, 56,
                fpBikeLogos[0],                     /* per default, will later be changed */
                DPLNORM, FALSE);

    ObjTextInit( &SWVersionTxtObj,                  /* sw version id */
                 0, 57, DPLFONT_6X8,
                 1, 21, TXT_LEFT, TXT_NORM,
                 szSWVersion,
                 OC_DISPL | OS_INIT | OS_DISPL);

    /* return */
    return ERR_OK;
}

/***********************************************************************
 *  FUNCTION:       IntroScreenShow
 *  DESCRIPTION:    bring updated 'intro screen device' to display
 *                  by calling Show-Fct. of all objects
 *  PARAMETER:      BOOL    TRUE = show objects, FALSE = clear screen
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void IntroScreenShow(BOOL fShow)
{
    ERRCODE error = ERR_OK;
    /* its on screen? */
    if (fShow == TRUE)
    {
        /* Update values */
        SixoLogoBmpObj.Data.fpucBitmap = (unsigned char far *)fpBikeLogos[gLogoSelection];

        /* NOTE:    Lower screen part is reserved for software + bike
                    version information.
                    We have up to 21 characters space. Example:
                    'V12.13.11 BAGHIRA EDS' (E=Emulator, D=Debug, S=VehicleSimulation */

        /* get formated sw version */
        szSWVersion[0] = 0x0;
        sprintf( szSWVersion, "V%u.%u.%u", gSWID.Fields.apl, gSWID.Fields.swv, gSWID.Fields.bld);
        strcat ( szSWVersion, " ");

        /* add formated bike version string depending on special compiler settings */
        switch ( gBikeType )
        {
            case eBIKE_AFRICATWIN:  strcat ( szSWVersion, RESTXT_BIKESTRING_AT); break;
            case eBIKE_R100GS:      strcat ( szSWVersion, RESTXT_BIKESTRING_R100GS); break;
            case eBIKE_R1100GS:     strcat ( szSWVersion, RESTXT_BIKESTRING_R1100GS); break;
            case eBIKE_BAGHIRA:     strcat ( szSWVersion, RESTXT_BIKESTRING_BAGHIRA); break;
            case eBIKE_HUSQV:       strcat ( szSWVersion, RESTXT_BIKESTRING_HUSQV); break;
            case eBIKE_HUSQVRS:     strcat ( szSWVersion, RESTXT_BIKESTRING_HUSQVRS); break;
            case eBIKE_KTM:         strcat ( szSWVersion, RESTXT_BIKESTRING_KTM); break;
            case eBIKE_MOTOBAU:     strcat ( szSWVersion, RESTXT_BIKESTRING_MOTOBAU); break;
            default:                strcat ( szSWVersion, RESTXT_BIKESTRING_DEFAULT); break;
        }
        strcat ( szSWVersion, " ");

        /* add formated debug infos */
        #ifdef DEBUG
            strcat ( szSWVersion, RESTXT_SWVER_DEBUG);
        #endif
        #ifdef MINIEMU
            strcat ( szSWVersion, RESTXT_SWVER_MINIEMU);
        #endif
        #ifdef VEHICSIM
            strcat ( szSWVersion, RESTXT_SWVER_VEHICSIM);
        #endif
        #ifdef HARDCOPY
            strcat ( szSWVersion, RESTXT_SWVER_HARDCOPY);
        #endif

        /* do we have to repaint all? */
        if (IntroScreenDev.fScreenInit == FALSE)
        {
            error = ObjBmpShow( &SixoLogoBmpObj );
            error = ObjTextShow( &SWVersionTxtObj );
            IntroScreenDev.fScreenInit  = TRUE;
        }
        else
        {
            //bitmap could have been changed!
            error = ObjBmpShow( &SixoLogoBmpObj );
        }
    }
    else
    {
        DisplClearScreen(0x0);
        IntroScreenDev.fScreenInit  = FALSE;
    }
}


/***********************************************************************
 *  FUNCTION:       IntroScreenMsgEntry
 *  DESCRIPTION:    Receive Message Handler of 'intro screen' device
 *                  called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE IntroScreenMsgEntry(MESSAGE GivenMsg)
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
            if ( IntroScreenDev.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_INTRO]);
                MSG_BUILD_SETFOCUS(NewMsg,DEVID_INTRO,MSG_SENDER_ID(GivenMsg));   /* build answer message */
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                            /* send answer message */
                IntroScreenDev.fFocused = FALSE;                                        /* clear our focus */
                IntroScreenShow(FALSE);                                                 /* clear our screen */
                RValue = ERR_MSG_PROCESSED;
            }
        } break;
        case MSG_SET_FOCUS:
        {
            /* messsage for us? */
            if (MSG_RECEIVER_ID(GivenMsg) == DEVID_INTRO)
            {
                /*  Someone wants us to take the focus?
                    We assume, that nobody else has the focus
                    and we've got the the screen now for us! */
                if (  (IntroScreenDev.fFocused   == TRUE             )
                    &&(MSG_RECEIVER_ID(GivenMsg) == DEVID_INTRO) )
                    ODS2(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but it already has focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_INTRO]);
                else /*
                    ODS2(   DBG_SYS, DBG_INFO,
                            "FOCUS: %s -> %s!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_INTRO]) */ ;
                IntroScreenDev.fFocused = TRUE;                         /* set our focus */
                IntroScreenShow(TRUE);                                  /* show our screen */
                gSystemFlags.flags.ActDevNr = DEVID_INTRO;                /* save device# for restore */
                RValue = ERR_MSG_PROCESSED;
             }
             else
             {
                if ( IntroScreenDev.fFocused == TRUE )
                {
                    /*  Some other device should be given the focus,
                        BUT WE'VE GOT THE FOCUS!
                        It should first ask US to give the focus! :-( */
                    ODS3(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but %s had focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[MSG_RECEIVER_ID(GivenMsg)],
                            szDevName[DEVID_INTRO]);
                    IntroScreenDev.fFocused = FALSE;                        /* loose our focus */
                    ODS1(   DBG_SYS, DBG_WARNING,
                            "%s now loosing focus :-( ",
                            szDevName[DEVID_INTRO]);
                    RValue = ERR_MSG_NOT_PROCESSED;
                }
             }
        } break;
    }

    /* this part is only for FOCUSED time
       AND msg not already processed */
    if (  (IntroScreenDev.fFocused == TRUE )
        &&(RValue == ERR_MSG_NOT_PROCESSED ) )
    {
        switch (MsgId)
        {
            case MSG_SCREEN_REFRESH:
                IntroScreenShow(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;
            case MSG_KEYS_PRESSED:
                /* check if UP&DOWN are pressed short: */
                if (  (RValue == ERR_MSG_NOT_PROCESSED                    )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN))
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYSHORT              ) )
                {
                    /* give focus immediatly to next device  */
                    IntroScreenDev.fFocused = FALSE;                                        /* clear our focus */
                    IntroScreenShow(FALSE);                                                 /* clear our screen */
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_INTRO, DEVID_MAIN);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_KEY_UP:
            case MSG_KEY_DOWN:
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = IntroScreenChangeLogo(GivenMsg);         /* try to change BMP */
                break;
            case MSG_DPL_FLASH_ON:
            case MSG_DPL_FLASH_OFF:
            case MSG_KEY_OK:
            default: return ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       IntroScreenChangeLogo
 *  DESCRIPTION:    UP / DOWN key will change SIxO logo, if key pressed
 *                  long.
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE IntroScreenChangeLogo(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                        /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    UINT16          wActTime;
    static UINT16   wLastChange = 0;

    /* get current time stamp */
    #define LOGO_DELAY  1000    // in ms
    TimerGetSys_msec(wActTime);

    /* user presses UP/DOWN Button > 2 sec: change logo! */
    if (  (MsgId == MSG_KEY_UP                   )                /* [UP] */
        &&(MSG_KEY_DURATION(Msg) > KEYSAVE       )                /* pressed 'long'? */
        &&( (wActTime - wLastChange) > LOGO_DELAY) )              /* delay? */
    {
        if ( gLogoSelection > BIKELOGO_FIRST)
             gLogoSelection--;
        else gLogoSelection = BIKELOGOG_LAST;       // set to last element
        TimerGetSys_msec(wLastChange);
        RValue = ERR_MSG_PROCESSED;
    }
    else if (  (MsgId == MSG_KEY_DOWN                  )           /* [DOWN] */
             &&(MSG_KEY_DURATION(Msg) > KEYSAVE        )           /* pressed 'long'? */
             &&( (wActTime - wLastChange) > LOGO_DELAY ) )         /* delay? */
    {
        if ( gLogoSelection < BIKELOGOG_LAST )
             gLogoSelection++;
        else gLogoSelection = BIKELOGO_FIRST;       // st to first element
        TimerGetSys_msec(wLastChange);
        RValue = ERR_MSG_PROCESSED;
    }

    return RValue;
}

