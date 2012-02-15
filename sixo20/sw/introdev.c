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
 * Revision 3.4  2012/02/15 07:32:43  tuberkel
 * Objects-API reviewed (no functional changes)
 *
 * Revision 3.3  2012/02/08 03:55:24  tuberkel
 * KEY_TIMING parameter names reviewed/changed
 *
 * Revision 3.2  2012/02/06 20:54:14  tuberkel
 * Just renamed all 'Devices' function prefixes for better readability
 *
 * Revision 3.1  2012/01/14 08:28:42  tuberkel
 * Message-IDs shortened / reviewed
 *
 * Revision 3.0  2010/11/07 13:45:46  tuberkel
 * V30 Preparations:
 * - Device/Object Handling completely revised & simplified
 * - changed BIKE Type logo handling
 * - SW Version String adapted
 *
 * Revision 2.9  2009/07/19 12:30:08  tuberkel
 * - ObjectInit reviewed
 *
 * Revision 2.8  2009/07/15 08:55:11  tuberkel
 * additional includes for global #defines
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
 * Revision 2.5  2009/06/21 17:51:47  tuberkel
 * Changes done by AN:
 * Compass Version uses add. 'C' indicator
 *
 * Revision 2.4  2007/03/30 10:09:22  tuberkel
 * added language string part to szSWVersion
 *
 * Revision 2.3  2007/03/26 23:16:22  tuberkel
 * BugFix:
 * - more reliable buffer size szSWVersion[] 22 -> 64 chars
 * Changed MOTOBAU version handling
 * - eBikeType -> #define
 *
 * Revision 2.2  2007/03/09 08:38:36  niezeithat
 * - Added additional Motorbike Version, Africatwin RD07, no Fule Level Sensors!
 *
 * Revision 2.1  2006/07/20 23:00:42  tuberkel
 * added F650 version
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
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
#include "measdrv.h"
#include "vehicsim.h"



/* external symbols */
extern UINT16           wMilliSecCounter;   /* valid values: 0h .. ffffh */
extern STRING far       szDevName[];        /* device names */
extern DEVFLAGS1_TYPE   gDeviceFlags1;      /* system parameters */
extern SWVERS_TYPE      gSWID;              /* software id from eeprom */
extern UINT8            gLogoSelection;     /* selected logo */
extern BIKE_TYPE        gBikeType;          /* bike type selcetion */
extern unsigned char    szSWVersion[];      /* formated sw version */

/* local device data */
static DEVDATA      IntroScreenDev;         /* this device */

/* text objects */
static OBJ_TEXTST   SWVersionTxtObj;        /* version text object */


/* text object table of this device */
static const OBJ_TEXTST_INIT TextObjInit[] =
{   /*pObject           X  Y   Font         H  Width  Align     Format    string ptr   State  */
    { &SWVersionTxtObj, 0, 57, DPLFONT_6X8, 1, 21,    TXT_LEFT, TXT_NORM, szSWVersion, OC_DISPL }
};
#define TEXTOBJECTLISTSIZE   (sizeof(TextObjInit)/sizeof(OBJ_TEXTST_INIT))


/* bitmap object */
extern const unsigned char far *    fpBikeLogos[];
static OBJ_BMP                    SixoLogoBmpObj;         /* logo object */


/* bitmap object table of this device */
static const OBJ_BMP_INIT     BmpObjInit[] =          /* init data */
{   /* object         x  y  w    h   raw data   mode     state */
    {&SixoLogoBmpObj, 0, 0, 128, 56, NULL,      DPLNORM, FALSE  }
};
#define BMPOBJECTLISTSIZE   (sizeof(BmpObjInit)/sizeof(OBJ_BMP_INIT))


/* this devices object focus handling - list of all objects */
/* NOTE:    this device does not need any focus, this
            list is for common object handling only! */
static const void far * ObjectList[] =
{
    // all bitmap objects
    (void far *) &SixoLogoBmpObj,
    // all text objects
    (void far *) &SWVersionTxtObj,
};
#define OBJECTLIST_SIZE   (sizeof(ObjectList)/sizeof(OBJ_STATE)/sizeof(void far *))




/* internal prototypes */
ERRCODE IntroDev_ChangeLogo(MESSAGE Msg);





/***********************************************************************
 *  FUNCTION:       IntroDev_Init
 *  DESCRIPTION:    all initial stuff for all objects of
 *                  'intro screen device'
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE IntroDev_Init(void)
{
    /* device main data */
    IntroScreenDev.eDevID       = DEVID_INTRO;
    IntroScreenDev.szDevName    = szDevName[DEVID_INTRO];
    IntroScreenDev.fFocused     = FALSE;
    IntroScreenDev.fScreenInit  = FALSE;

    /* initialize all objects of any type */
    DevObjInit( &IntroScreenDev, (void far *)TextObjInit,   TEXTOBJECTLISTSIZE, OBJT_TXT  );
    DevObjInit( &IntroScreenDev, (void far *)BmpObjInit,    BMPOBJECTLISTSIZE,  OBJT_BMP  );

    /* initialize this devices objects list */
    IntroScreenDev.Objects.ObjList       = ObjectList;
    IntroScreenDev.Objects.ObjCount      = OBJECTLIST_SIZE;
    IntroScreenDev.Objects.FirstSelObj   = DevObjGetFirstSelectable(&IntroScreenDev,ObjectList,OBJECTLIST_SIZE);
    IntroScreenDev.Objects.LastSelObj    = DevObjGetLastSelectable (&IntroScreenDev,ObjectList,OBJECTLIST_SIZE);

    /* reset focus handling to start values */
    DevObjFocusReset( &IntroScreenDev, ObjectList, OBJECTLIST_SIZE );

    /* return */
    ODS( DBG_SYS, DBG_INFO, "- IntroDev_Init() done!");
    return ERR_OK;
}







/***********************************************************************
 *  FUNCTION:       IntroDev_Show
 *  DESCRIPTION:    bring updated 'intro screen device' to display
 *                  by calling Show-Fct. of all objects
 *  PARAMETER:      BOOL    TRUE = show objects, FALSE = clear screen
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void IntroDev_Show(BOOL fShow)
{
    ERRCODE error = ERR_OK;
    /* its on screen? */
    if (fShow == TRUE)
    {
        /* Update values */
        #if(BIKE_MOTOBAU==1)
            // always use the Motobau logo for this version
            SixoLogoBmpObj.Data.fpucBitmap = (unsigned char far *)fpBikeLogos[eLogo_Motobau];   // fixed to MOTOBAU logo
        #else // BIKE_MOTOBAU
            // use the logo selected by user
            SixoLogoBmpObj.Data.fpucBitmap = (unsigned char far *)fpBikeLogos[gLogoSelection];
        #endif // BIKE_MOTOBAU

        /* NOTE:    Lower screen part is reserved for software + bike
                    version information.
                    We have up to 21 characters space. Example:
                    'V12.13.11 BAGHIRA EDS' (E=Emulator, D=Debug, S=VehicleSimulation */


        /* do we have to repaint all? */
        if (IntroScreenDev.fScreenInit == FALSE)
        {
            error = Obj_Bmp_Show ( &SixoLogoBmpObj );
            error = Obj_TextSt_Show( &SWVersionTxtObj );
            IntroScreenDev.fScreenInit  = TRUE;
        }
        else
        {
            //bitmap could have been changed!
            error = Obj_Bmp_Show( &SixoLogoBmpObj );
        }
    }
    else
    {
        // clear screen
        DisplClearScreen(0x0);

        // reset states of all objects of this device
        DevObjClearState(  &IntroScreenDev,
                            IntroScreenDev.Objects.ObjList,
                            IntroScreenDev.Objects.ObjCount,
                            OS_DISPL | OS_EDIT );

        // reset devices screen state
        IntroScreenDev.fScreenInit  = FALSE;
    }
}


/***********************************************************************
 *  FUNCTION:       IntroDev_MsgEntry
 *  DESCRIPTION:    Receive Message Handler of 'intro screen' device
 *                  called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE IntroDev_MsgEntry(MESSAGE GivenMsg)
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
                IntroDev_Show(FALSE);                                                 /* clear our screen */
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
                IntroDev_Show(TRUE);                                  /* show our screen */
                gDeviceFlags1.flags.ActDevNr = DEVID_INTRO;              /* save device# for restore */
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
            case MSG_SCREEN_RFRSH:
                IntroDev_Show(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;
            case MSG_KEYS_PRESSED:
                /* check if UP&DOWN are pressed short: */
                if (  (RValue == ERR_MSG_NOT_PROCESSED                    )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN))
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYTM_PRESSED_SHORT              ) )
                {
                    /* give focus immediatly to next device  */
                    IntroScreenDev.fFocused = FALSE;                                        /* clear our focus */
                    IntroDev_Show(FALSE);                                                 /* clear our screen */
                    #if(TESTSCREEN==1)
                        MSG_BUILD_SETFOCUS(NewMsg, DEVID_INTRO, DEVID_TESTSCREEN);
                    #else
                        MSG_BUILD_SETFOCUS(NewMsg, DEVID_INTRO, DEVID_MAIN);
                    #endif
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_KEY_UP:
            case MSG_KEY_DOWN:
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = IntroDev_ChangeLogo(GivenMsg);         /* try to change BMP */
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
 *  FUNCTION:       IntroDev_ChangeLogo
 *  DESCRIPTION:    UP / DOWN key will change SIxO logo, if key pressed
 *                  long.
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE IntroDev_ChangeLogo(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                        /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    UINT16          wActTime;
    static UINT16   wLastChange = 0;

    /* get current time stamp */
    #define CHANGE_LOGO_DELAY  1000    // in ms
    TimerGetSys_msec(wActTime);

    /* user presses UP/DOWN Button > 2 sec: change logo! */
    if (  (MsgId == MSG_KEY_UP                   )                /* [UP] */
        &&(MSG_KEY_DURATION(Msg) > KEYTM_PRESSED_VLONG       )                /* pressed 'long'? */
        &&( (wActTime - wLastChange) > CHANGE_LOGO_DELAY) )              /* delay? */
    {
        if ( gLogoSelection > BIKELOGO_FIRST)
             gLogoSelection--;
        else gLogoSelection = BIKELOGOG_LAST;       // set to last element
        TimerGetSys_msec(wLastChange);
        RValue = ERR_MSG_PROCESSED;
    }
    else if (  (MsgId == MSG_KEY_DOWN                  )           /* [DOWN] */
             &&(MSG_KEY_DURATION(Msg) > KEYTM_PRESSED_VLONG        )           /* pressed 'long'? */
             &&( (wActTime - wLastChange) > CHANGE_LOGO_DELAY ) )         /* delay? */
    {
        if ( gLogoSelection < BIKELOGOG_LAST )
             gLogoSelection++;
        else gLogoSelection = BIKELOGO_FIRST;       // st to first element
        TimerGetSys_msec(wLastChange);
        RValue = ERR_MSG_PROCESSED;
    }

    return RValue;
}


