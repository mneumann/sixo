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
 * Revision 2.2  2009/07/22 12:36:35  tuberkel
 * Device Object Focus handling simplified:
 * - use DevObjFocusMove() / DevObjFocusReset() inside any device
 * - DevObjFocusReset() to set focus to default state
 * - use DevObjFocusMove() to move focus based on UP/DOWN key
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

extern DPLFLAGS_TYPE    gDisplayFlags;  //orginal display values
extern SYSFLAGS_TYPE    gSystemFlags;   //system parameters
extern UINT32           dwSystemTime;   // high resolution long  distance timer, ms,  max. 49 days


/* device names in plain text for diagnostic purpose */
/* (index is same as DEVICE_ID) */
const STRING szDevName[] =
{   "UnknownDev",
    "AllDevices",
    "MainDev",
    "IntroDev",
    "TripCDev",
    "SetDev",
    "MenuDev",
    "TestDev",
    "DiagnDev",
    "DbgDev",
    "WarnDev",
    "MonDev",
    "StatDev",
    "LapCDev",
    "HWTestDev"
};


/***********************************************************************
 *  FUNCTION:       DevCyclicRefresh
 *  DESCRIPTION:    helps any device refreshing the display content
 *                  every MAX_REFRESH_MS ms by sending the
 *                  MSG_SCREEN_REFRESH message.
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
    if (wThisCall - wLastCall > MAX_REFRESH_MS)
    {
        wLastCall = wThisCall;                                  /* update time stamp */
        MSG_BUILD_REFRESH(Msg, DEVID_UNKNOWN, DEVID_ALL);       /* from unknown to all */
        MsgQPostMsg(Msg, MSGQ_PRIO_LOW);                        /* post-it..*/

        /* backlight automatic only, if we are NOT in Settings/HWtest Screen
           they have to handle it on their own (AlwaysOn/EditMode)
           And: After Start, backlight is ON, we delay automatic switch off
           for 3 seconds to prevent flickering if it's night */
        if (  (gSystemFlags.flags.ActDevNr != DEVID_SET   )
            &&(gSystemFlags.flags.ActDevNr != DEVID_HWTEST)
            &&(dwSystemTime                >  3000        ) )
        {
            /* cyclic check: backlight automatic */
            LCDDrvSetBacklightLevel(    DisplBacklightCheckOn(gDisplayFlags.flags.BacklOnLevel),
                                        gDisplayFlags.flags.BacklLev );
        }
    }
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       DevObjFocusMove
 *  DESCRIPTION:    Moves focus to next/prev. object
 *  PARAMETER:      fpDevData       device to support focus
 *                  ObjList         list og selectable(!) objects of the device
 *                  FocusListSize   number of objects inside the list
 *                  Msg             to be processed/move focus
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE DevObjFocusMove(    DEVDATA far *       fpDevData,
                            void far * far *    ObjList,
                            UINT8               FocusListSize,
                            MESSAGE             Msg)
{
    ERRCODE                 RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID              MsgId;
    UINT8                   i;
    OBJSTATE far * far *    FocusObjList;

    /* extract and check message parameters: UP/DOWN and 1st/longer TIME? */
    MsgId = MSG_ID(Msg);
    if (  (  (MsgId                   == MSG_KEY_DOWN )
           ||(MsgId                   == MSG_KEY_UP   ) )
        &&(  (MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED )     // right now pressed?
           ||(MSG_KEY_TRANSITION(Msg) == KEYTRANS_ON      ) ) ) // or key repitition active?
    {
        /* convert the object list ptr for eassier access */
        FocusObjList = (OBJSTATE far * far *) ObjList;

        /* clear focus of all objects per default */
        for (i = 0; i < FocusListSize; i++)
            FocusObjList[i]->bits.fSelected = FALSE;

        /* move focus down (incl. wrap around) */
        if ( MsgId == MSG_KEY_DOWN )
        {   if ( fpDevData->bFocusObj < (FocusListSize-1) )
                 fpDevData->bFocusObj++;
            else fpDevData->bFocusObj = 0;
        }

        /* try to move focus up (incl. wrap around) */
        if ( MsgId == MSG_KEY_UP )
        {   if ( fpDevData->bFocusObj > 0 )
                 fpDevData->bFocusObj--;
            else fpDevData->bFocusObj = FocusListSize-1;
        }

        /* activate focus of current object */
        FocusObjList[fpDevData->bFocusObj]->bits.fSelected = TRUE;
        ODS1( DBG_SYS, DBG_INFO, "OBJ-FOCUS moved to %u", fpDevData->bFocusObj);

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
 *                  the focus fr the first object
 *  PARAMETER:      fpDevData       device to support focus
 *                  ObjList         list og selectable(!) objects of the device
 *                  FocusListSize   number of objects inside the list
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE DevObjFocusReset(   DEVDATA far *       fpDevData,
                            void far * far *    ObjList,
                            UINT8               FocusListSize )
{
    OBJSTATE far * far *    FocusObjList;
    UINT8           i;

    /* convert the object list ptr for eassier access */
    FocusObjList = (OBJSTATE far * far *) ObjList;

    /* clear focus of all objects per default */
    for (i = 0; i < FocusListSize; i++)
        FocusObjList[i]->bits.fSelected = FALSE;

    /* activate focus of most first object */
    fpDevData->bFocusObj = 0;
    FocusObjList[fpDevData->bFocusObj]->bits.fSelected = TRUE;
    ODS1( DBG_SYS, DBG_INFO, "OBJ-FOCUS reseted to %u", fpDevData->bFocusObj);
    return (ERR_OK);
}




