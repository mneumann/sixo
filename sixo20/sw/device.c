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
 *  Module:         devices
 *  Purpose:        all devices interfaces
 *  Comments:
 *                  NOTE: *** Meaning of devices ***
 *
 *                  Devices in this systems context are single and autark
 *                  working modules to solve a special feature, f.e. the
 *                  'TripCounter' or the 'MainMenu'.
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
 * Revision 3.6  2012/05/27 17:52:39  tuberkel
 * Corrections for renamed Eeprom/Nvram Variables
 *
 * Revision 3.5  2012/05/27 16:01:37  tuberkel
 * All Eeprom/Nvram Variables renamed
 *
 * Revision 3.4  2012/02/21 20:58:15  tuberkel
 * all Setdevice ObjectNames reviewed
 *
 * Revision 3.3  2012/02/15 07:32:43  tuberkel
 * Objects-API reviewed (no functional changes)
 *
 * Revision 3.2  2012/01/14 08:28:42  tuberkel
 * Message-IDs shortened / reviewed
 *
 * Revision 3.1  2011/05/29 20:55:25  tuberkel
 * just comment
 *
 * Revision 3.0  2010/11/07 13:28:26  tuberkel
 * V30 Preparations:
 * - Device Handling completely reviewed & simplified
 * - Support of 'ObjectLists' inside selected Device
 * - automatic focus of selected object in device
 *
 * Revision 2.2  2009/07/22 12:36:35  tuberkel
 * Device Object Focus handling simplified:
 * - use DevObjFocusSet() / DevObjFocusReset() inside any device
 * - DevObjFocusReset() to set focus to default state
 * - use DevObjFocusSet() to move focus based on UP/DOWN key
 *
 * Revision 2.1  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 * Revision 1.8  2006/03/18 08:45:10  Ralf
 * BugFix: no flickering backlight after booting in darkness
 *
 * Revision 1.7  2006/02/18 14:43:30  Ralf
 * - BIKE_MOTOBAU: lapcounter and statistic devices added
 *
 *
 ************************************************************************ */


/* includes */
#include "standard.h"
#include "msgqueue.h"
#include "device.h"
#include "resource.h"
#include "timer.h"
#include "displdrv.h"
#include "display.h"
#include "sysparam.h"
#include "objects.h"
#include "digindrv.h"
#include "debug.h"




/* external stuff */
extern  UINT16  wMilliSecCounter;       // valid values: 0h .. ffffh
extern  UINT16  wSecCounter;            // valid values: 0h .. ffffh

extern DPLFLAGS_TYPE    EE_DisplFlags;  //orginal display values
extern DEVFLAGS1_TYPE   EE_DevFlags_1;  //system parameters
extern UINT32           dwSystemTime;   // high resolution long  distance timer, ms,  max. 49 days


/* device names in plain text for diagnostic purpose */
/* NOTE_ Always assure same order like DEVICE_ID!!! */
const STRING szDevName[] =
{   "UnknownDev",

    // these devices are currently supported
    "AllDevices",
    "MainDev",
    "IntroDev",
    "TripCntDev",
    "SetDev",
    "StatDev",
    "LapCDev",
    "MonDev",
    "HWTestDev",
    "TestDev",

    // these device IDs are prepared for future use
    "MenuDev",
    "DiagnDev",
    "DbgDev",
    "WarnDev",

    "INVALID"
};


/***********************************************************************
 *  FUNCTION:       DevCyclicRefresh
 *  DESCRIPTION:    helps any device refreshing the display content
 *                  every MAX_REFRESH_MS ms by sending the
 *                  MSG_SCREEN_RFRSH message.
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        Registered as timer function, will be called from
 *                  timer interrupt
 *********************************************************************** */
ERRCODE DevCyclicRefresh(void)
{
    static  UINT16  wLastCall   = 0;                /* time stamp of last call */
            UINT16  wThisCall   = 0;                /* time stamp of this call */
            MESSAGE Msg;                            /* for building messages */

    /* check: time to check one system parameter? */
    TimerGetSys_msec(wThisCall);
    if ((wThisCall - wLastCall) > MAX_REFRESH_MS)
    {
        /* --------------------------------------------- */
        /* generate a cyclic refersh message */
        wLastCall = wThisCall;                                  /* update time stamp */
        MSG_BUILD_REFRESH(Msg, DEVID_UNKNOWN, DEVID_ALL);       /* from unknown to all */
        MsgQPostMsg(Msg, MSGQ_PRIO_LOW);                        /* post-it..*/

        /* --------------------------------------------- */
        /* support backlight automatic */
        /* NOTE: Support this only, if
                    - we are NOT in Settings/HWtest Screen
                    - if at least 3 seconds have gone
                 This assures, that backlight is correctly handled while
                 editing the backlight mode, and at init time the backlight is
                 ON for at least n seconds. */
        if (  (EE_DevFlags_1.flags.ActDevNr != DEVID_SET   )
            &&(EE_DevFlags_1.flags.ActDevNr != DEVID_HWTEST)
            &&(dwSystemTime                >  3000        ) )
        {
            /* support backlight automatic */
            LCDDrvSetBacklightLevel(    DisplBacklightCheckOn(EE_DisplFlags.flags.BacklOnLevel),
                                        EE_DisplFlags.flags.BacklLev );
        }
    }
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       DevObjFocusSet
 *  DESCRIPTION:    Moves focus to next/prev. object
 *  PARAMETER:      fpDevData       device to support focus
 *                  ObjList         list og selectable(!) objects of the device
 *                  ListSize        number of objects inside the list
 *                  Msg             to be processed/move focus
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE DevObjFocusSet( DEVDATA far *       fpDevData,
                        void far * far *    GivenList,
                        UINT8               ListSize,
                        MESSAGE             Msg)
{
    OBJ_DUMMY far * far * ObjList;  // dummy object list to handle common object properties
    ERRCODE                 RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID              MsgId;
    BOOL                    fFound = FALSE;
    UINT8                   i;

    /* extract and check message parameters: UP/DOWN and 1st/longer TIME? */
    MsgId = MSG_ID(Msg);
    if (  (  (MsgId                   == MSG_KEY_DOWN     )
           ||(MsgId                   == MSG_KEY_UP       ) )
        &&(  (MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED )     // right now pressed?
           ||(MSG_KEY_TRANSITION(Msg) == KEYTRANS_ON      ) ) ) // or key repitition active?
    {
        /* convert the given list into an array of dummy objects to get access to it */
        ObjList = (OBJ_DUMMY far * far *) GivenList;

        /* clear focus of all objects per default */
        for (i = 0; i < ListSize; i++)
            ObjList[i]->State.bits.fSelected = FALSE;

        /* ------------------------------------------------- */
        /* check: DOWN pressed? -> move focus to NEXT object */
        if ( MsgId == MSG_KEY_DOWN )
        {
            /* try to find next SELECTABLE object */
            i = fpDevData->bFocusObj;       // start with current focus object index
            while (fFound == FALSE)
            {
                /* 1st: move focus down (incl. wrap around) */
                if ( i < (ListSize-1) )
                     i++;
                else i = 0;

                /* 2nd: object selectable? -> ok! */
                if (ObjList[i]->State.bits.fSelectable == TRUE)
                {   fFound = TRUE;          // ok, break here
                }
                else
                {   ODS2( DBG_SYS, DBG_WARNING, "[%s] Unselectable object %u", fpDevData->szDevName, i);
                }

                /* 3rd: all objects checked? -> error! */
                if (i == fpDevData->bFocusObj)
                {   fFound = TRUE;          // error, but break here
                    ODS1( DBG_SYS, DBG_WARNING, "[%s] NO selectable object found!", fpDevData->szDevName );
                }
            }
            /* ok, save result */
            fpDevData->bFocusObj = i;
        }

        /* ------------------------------------------------- */
        /* check: UP pressed? -> move focus to NEXT object */
        if ( MsgId == MSG_KEY_UP )
        {
            /* try to find next SELECTABLE object */
            i = fpDevData->bFocusObj;       // start with current focus object index
            while (fFound == FALSE)
            {
                /* 1st: move focus down (incl. wrap around) */
                if ( i > 0 )
                     i--;
                else i = ListSize-1;

                /* 2nd: object selectable? -> ok! */
                if (ObjList[i]->State.bits.fSelectable == TRUE)
                {   fFound = TRUE;          // ok, break here
                }
                else
                {   ODS2( DBG_SYS, DBG_WARNING, "[%s] Unselectable object %u", fpDevData->szDevName, i);
                }

                /* 3rd: all objects checked? -> error! */
                if (i == fpDevData->bFocusObj)
                {   fFound = TRUE;          // error, but break here
                    ODS1( DBG_SYS, DBG_WARNING, "[%s] NO selectable object found!", fpDevData->szDevName );
                }
            }
            /* ok, save result */
            fpDevData->bFocusObj = i;
        }

        /* activate focus of current object */
        ObjList[fpDevData->bFocusObj]->State.bits.fSelected = TRUE;
        ODS2( DBG_SYS, DBG_INFO, "[%s] Focus moved to %u", fpDevData->szDevName, fpDevData->bFocusObj);

        // msg processed anyway
        RValue = ERR_MSG_PROCESSED;
    }
    else
    {   // irrelevant message
    }

    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       DevObjFocusReset
 *  DESCRIPTION:    resets focus of all selectable objects and activates
 *                  the focus fr the first selectable object (if any)
 *  PARAMETER:      fpDevData       device to support focus
 *                  GivenList       list of objects of the device
 *                  ListSize        number of objects inside the list
 *  RETURN:         ERR_OK          always
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE DevObjFocusReset(   DEVDATA far *       fpDevData,
                            void far * far *    GivenList,
                            UINT8               FocusListSize )
{
    OBJ_DUMMY far * far * ObjList;  // dummy object list to handle common object properties
    BOOL                    fFound = FALSE;
    UINT8                   i;

    /* convert the given list into an array of dummy objects to get access to it */
    ObjList = (OBJ_DUMMY far * far *) GivenList;

    /* clear focus of all objects per default */
    for (i = 0; i < FocusListSize; i++)
        ObjList[i]->State.bits.fSelected = FALSE;

    /* try to find first SELECTABLE object */
    for (i = 0; i < FocusListSize; i++)
    {
        /* check: object IS selectable? -> select it and break here! */
        if (ObjList[i]->State.bits.fSelectable == TRUE)
        {   /* ok, save result */
            fpDevData->bFocusObj = i;
            ObjList[fpDevData->bFocusObj]->State.bits.fSelected = TRUE;
            ODS2( DBG_SYS, DBG_INFO, "[%s] Focus reset to %u", fpDevData->szDevName, fpDevData->bFocusObj);
            break;
        }
        else
        {   //ODS2( DBG_SYS, DBG_WARNING, "[%s] Non selectable object %u", fpDevData->szDevName, i);
        }
    }
    /* check: no object selectable at all? -> warning! */
    if (i == FocusListSize)
    {   ODS1( DBG_SYS, DBG_WARNING, "[%s] NO selectable object found!", fpDevData->szDevName );
    }
    return (ERR_OK);
}



/***********************************************************************
 *  FUNCTION:       DevObjGetFirstSelectable
 *  DESCRIPTION:    Returns the FIRST selectable object in the list
 *  PARAMETER:      fpDevData       device to support focus
 *                  ObjList         list og selectable(!) objects of the device
 *                  ListSize        number of objects inside the list
 *  RETURN:         0..n            number of first selectable object in list
 *                  ListSize        if NO object selectable
 *  COMMENT:        -
 *********************************************************************** */
UINT8 DevObjGetFirstSelectable( DEVDATA far *       fpDevData,
                                void far * far *    GivenList,
                                UINT8               ListSize   )
{
    OBJ_DUMMY far * far * ObjList;  // dummy object list to handle common object properties
    BOOL                    fFound = FALSE;
    UINT8                   i;

    /* convert the given list into an array of dummy objects to get access to it */
    ObjList = (OBJ_DUMMY far * far *) GivenList;

    /* check 'Selectable' capability of all objects - until first found */
    for (i = 0; i < ListSize; i++)
    {
        if ( ObjList[i]->State.bits.fSelectable == TRUE )
            break;
    }
    ODS3( DBG_SYS, DBG_INFO, "[%s] First selectable is %u/%u", fpDevData->szDevName, i, ListSize );
    return (i);
}


/***********************************************************************
 *  FUNCTION:       DevObjGetLastSelectable
 *  DESCRIPTION:    Returns the LAST selectable object in the list
 *  PARAMETER:      fpDevData       device to support focus
 *                  ObjList         list og selectable(!) objects of the device
 *                  ListSize        number of objects inside the list
 *  RETURN:         0..n            number of last selectable object in list
 *                  ListSize        if NO object selectable
 *  COMMENT:        -
 *********************************************************************** */
UINT8 DevObjGetLastSelectable(  DEVDATA far *       fpDevData,
                                void far * far *    GivenList,
                                UINT8               ListSize   )
{
    OBJ_DUMMY far * far * ObjList;  // dummy object list to handle common object properties
    BOOL                    fFound = FALSE;
    UINT8                   i;

    /* convert the given list into an array of dummy objects to get access to it */
    ObjList = (OBJ_DUMMY far * far *) GivenList;

    /* check 'Selectable' capability of all objects - reverse, until last found */
    for (i = ListSize; i > 0; i--)
    {
        if ( ObjList[i]->State.bits.fSelectable == TRUE )
            break;
    }
    ODS3( DBG_SYS, DBG_INFO, "[%s] Last selectable is %u/%u", fpDevData->szDevName, i, ListSize );
    return (i);
}



/***********************************************************************
 *  FUNCTION:       DevObjShow
 *  DESCRIPTION:    Universal function to show all objects of any
 *                  kind inside a list of objects.
 *  PARAMETER:      fpDevData       device to support focus
 *                  GivenList       list of objects of the device
 *                  ListSize        number of objects inside the list
 *                  UpdateMode      for edit objects only: kind of show method
 *  RETURN:         ERR_OK          always
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE DevObjShow( DEVDATA far *       fpDevData,
                    void far * far *    GivenList,
                    UINT8               ListSize,
                    UINT8               UpdateMode )
{
    OBJ_DUMMY far * far * ObjList;  // dummy object list to handle common object properties
    UINT8           i;

    /* convert the given list into an array of dummy objects to get access to it */
    ObjList = (OBJ_DUMMY far * far *) GivenList;

    /* process all objects of list */
    for (i = 0; i < ListSize; i++)
    {
        /* select correct kind of method */
        switch (ObjList[i]->eType)
        {
            case OBJT_BMP:   Obj_Bmp_Show      ( (OBJ_BMP far *)        (ObjList[i]) ); break;
            case OBJT_TXT:   Obj_TextSt_Show     ( (OBJ_STEXT far *)       (ObjList[i]) ); break;
            case OBJT_ETXT:  Obj_Text_Show ( (OBJ_TEXT far *)   (ObjList[i]), UpdateMode ); break;
            case OBJT_ENUM:  Obj_Num_Show  ( (OBJ_NUM far *) (ObjList[i]), UpdateMode ); break;
            case OBJT_EBOOL: Obj_Bool_Show ( (OBJ_BOOL far *)   (ObjList[i]), UpdateMode ); break;
            case OBJT_SLCT:  Obj_Select_Show   ( (OBJ_SELECT far *)     (ObjList[i]), UpdateMode ); break;
            default: ODS1( DBG_SYS, DBG_WARNING, "DevObjShow(): Unknown ObjType %u!", ObjList[i]->eType); break;
        }
    }
}



/***********************************************************************
 *  FUNCTION:       DevObjSetState
 *  DESCRIPTION:    Universal function to INSERT one of the OBJ_STATE
 *                  bits into all objects insde the list
 *  PARAMETER:      fpDevData       device to support focus
 *                  GivenList       list of objects of the device
 *                  ListSize        number of objects inside the list
 *                  OBJ_STATE        state to be set
 *  RETURN:         ERR_OK          always
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE DevObjSetState( DEVDATA far *       fpDevData,
                        void far * far *    GivenList,
                        UINT8               ListSize,
                        UINT8               GivenState )
{
    OBJ_DUMMY far * far * ObjList;  // dummy object list to handle common object properties
    UINT8           i;

    /* convert the given list into an array of dummy objects to get access to it */
    ObjList = (OBJ_DUMMY far * far *) GivenList;

    /* process all objects of list */
    for (i = 0; i < ListSize; i++)
        ObjList[i]->State.byte |= GivenState;

    ODS2( DBG_SYS, DBG_INFO, "[%s] All object states 0x%0x SET", fpDevData->szDevName, GivenState);
}



/***********************************************************************
 *  FUNCTION:       DevObjClearState
 *  DESCRIPTION:    Universal function to CLEAR one of the OBJ_STATE
 *                  bits into all objects off the list
 *  PARAMETER:      fpDevData       device to support focus
 *                  GivenList       list of objects of the device
 *                  ListSize        number of objects inside the list
 *                  OBJ_STATE        state to be set
 *  RETURN:         ERR_OK          always
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE DevObjClearState(   DEVDATA far *       fpDevData,
                            void far * far *    GivenList,
                            UINT8               ListSize,
                            UINT8               GivenState )
{
    OBJ_DUMMY far * far * ObjList;  // dummy object list to handle common object properties
    UINT8           i;

    /* convert the given list into an array of dummy objects to get access to it */
    ObjList = (OBJ_DUMMY far * far *) GivenList;

    /* process all objects of list */
    for (i = 0; i < ListSize; i++)
        ObjList[i]->State.byte &= ~GivenState;

    ODS2( DBG_SYS, DBG_INFO, "[%s] All object states 0x%0x CLEARED!", fpDevData->szDevName, GivenState);
}



/***********************************************************************
 *  FUNCTION:       DevObjInit
 *  DESCRIPTION:    Universal function to initiliaze all objects of any
 *                  kind inside a list of objects.
 *  PARAMETER:      fpDevData       device to support focus
 *                  GivenList       list of objects of the device
 *                  ListSize        number of objects inside the list
 *                  eType           object type to initialized
 *  RETURN:         ERR_OK          always
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE DevObjInit( DEVDATA far *       fpDevData,
                    void far *          ObjList,
                    UINT8               ListSize,
                    UINT8               eType     )
{
    UINT8   i;

    /* process all objects of list */
    for (i = 0; i < ListSize; i++)
    {
        /* select correct kind of method */
        switch ((OBJ_TYPE) eType)
        {
            case OBJT_TXT:   Obj_TextSt_Init     ( &((OBJ_STEXT_INIT far *)ObjList)[i]); break;
            case OBJT_BMP:   Obj_Bmp_Init      ( &((OBJ_BMP_INIT far *) ObjList)[i]); break;
            case OBJT_ETXT:  Obj_Text_Init ( &((OBJ_TEXT_INIT far *)  ObjList)[i]); break;
            case OBJT_ENUM:  Obj_Num_Init  ( &((OBJ_NUM_INIT far *)ObjList)[i]); break;
            case OBJT_EBOOL: Obj_Bool_Init ( &((OBJ_BOOL_INIT far *)  ObjList)[i]); break;
            case OBJT_SLCT:  Obj_Select_Init   ( &((OBJ_SLCT_INIT far *)    ObjList)[i]); break;
            default: ODS1( DBG_SYS, DBG_WARNING, "DevObjInit(): Unknown ObjType %u!", eType); break;
        }
    }
}



/***********************************************************************
 *  FUNCTION:       DevObjMsg
 *  DESCRIPTION:    Provides all objects given in list with msg.
 *  PARAMETER:      fpDevData       device to support focus
 *                  ObjList         list og selectable(!) objects of the device
 *                  ListSize        number of objects inside the list
 *                  Msg             to be processed/move focus
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE DevObjMsg(  DEVDATA far *       fpDevData,
                    void far * far *    GivenList,
                    UINT8               ListSize,
                    MESSAGE             GivenMsg )

{
    OBJ_DUMMY far * far * ObjList;    // dummy object list to handle common object properties
    ERRCODE                 RValue = ERR_MSG_NOT_PROCESSED;
    UINT8                   i;

    /* convert the given list into an array of dummy objects to get access to it */
    ObjList = (OBJ_DUMMY far * far *) GivenList;

    /* process all objects of list */
    for (i = 0; i < ListSize; i++)
    {
        /* select correct kind of method */
        switch (ObjList[i]->eType)
        {
            case OBJT_BMP:   break; // does not have a msg entry fct.
            case OBJT_TXT:   break; // does not have a msg entry fct.
            case OBJT_ETXT:  RValue = Obj_Text_MsgEntry ( (OBJ_TEXT far *)   (ObjList[i]), GivenMsg ); break;
            case OBJT_ENUM:  RValue = Obj_Num_MsgEntry  ( (OBJ_NUM far *) (ObjList[i]), GivenMsg ); break;
            case OBJT_EBOOL: RValue = Obj_Bool_MsgEntry ( (OBJ_BOOL far *)   (ObjList[i]), GivenMsg ); break;
            case OBJT_SLCT:  RValue = Obj_Select_MsgEntry   ( (OBJ_SELECT far *)     (ObjList[i]), GivenMsg ); break;
            default: ODS1( DBG_SYS, DBG_WARNING, "DevObjShow(): Unknown ObjType %u!", ObjList[i]->eType); break;
        }

        /* break here, if object processed the msg */
        if (RValue == ERR_MSG_PROCESSED)
            break;
    }

    /* return result */
    return RValue;
}



