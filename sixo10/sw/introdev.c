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
 *  Created:        24-11-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         intro devices
 *  Purpose:        handles the intro screen device with all
 *                  subsequent potential screens
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
#include "sysparam.h"




/* external symbols */
extern UINT16               wMilliSecCounter;           /* valid values: 0h .. ffffh */
extern STRING far           szDevName[];                /* device names */
extern SYSFLAGS_TYPE        gSystemFlags;               /* system parameters */
extern unsigned char far    rgSIxOLogo128x56[];         /* logo bitmap data */
extern SWVERS_TYPE          gSWID;                      /* software id from eeprom */


/* device static objects */
static DEVDATA      IntroScreenDev;         /* this device */
static BMPOBJECT    SixoLogoBmpObj;         /* logo object */
static TEXTOBJECT   SWVersionTxtObj;        /* version text object */
static TEXTOBJECT   SWDebugVersTxtObj;      /* debug version text object */

static unsigned char szSWVersion[15];       /* buffer to contain formated sw id */
static unsigned char szSWDbgVers[15];       /* buffer to contain formated debug infos */




/* internal prototypes */





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

    /* get formated sw version */
    sprintf( szSWVersion, "V%u.%u.%u.%c",
                         gSWID.Fields.apl,
                         gSWID.Fields.swv,
                         gSWID.Fields.bld,
                         gSWID.Fields.spc+APLNUMCOFFS);

    /* get formated debug infos */
    szSWDbgVers[0] = 0x0;
    #ifdef DEBUG
    strcat ( szSWDbgVers, "DBG");
    #endif
    #ifdef MINIEMU
    strcat ( szSWDbgVers, " MINIEMU");
    #endif

    /* init objects */
    ObjBmpInit( &SixoLogoBmpObj,                    /* logo bitmap */
                0, 0, 128, 56,
                rgSIxOLogo128x56,
                DPLNORM, FALSE);

    ObjTextInit( &SWVersionTxtObj,                  /* sw version id */
                 2, 48, DPLFONT_6X8,
                 1, 21, TXT_RIGHT, TXT_NORM,
                 szSWVersion,
                 OC_DISPL | OS_INIT | OS_DISPL);

    ObjTextInit( &SWDebugVersTxtObj,                /* sw debug info */
                 2, 40, DPLFONT_6X8,
                 1, 21, TXT_RIGHT, TXT_NORM,
                 szSWDbgVers,
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
        /* do we have to repaint all? */
        if (IntroScreenDev.fScreenInit == FALSE)
        {
            error = ObjBmpShow( &SixoLogoBmpObj );
            error = ObjTextShow( &SWVersionTxtObj );
            error = ObjTextShow( &SWDebugVersTxtObj );
            IntroScreenDev.fScreenInit  = TRUE;
        }
        else
        {
            // no refresh stuff to be done here !!!
            // (all static data)
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
            case MSG_DPL_FLASH_ON:
            case MSG_DPL_FLASH_OFF:
            case MSG_KEY_OK:
            default: return ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}




