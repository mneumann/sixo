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
 *  Module:         devices
 *  Purpose:        all devices interfaces
 *  Comments:
 *                  NOTE: *** Meaning of devices ***
 *
 *                  Devices in this systems context are single and autark
 *                  working modules to solve a special feature, f.e. the
 *                  'TripCounter' or the 'MainMenu'.
 *
 ************************************************************************ */

#include "standard.h"
#include "msgqueue.h"
#include "device.h"
#include "resource.h"
#include "timer.h"


/* external stuff */
extern  UINT16  wMilliSecCounter;       // valid values: 0h .. ffffh
extern  UINT16  wSecCounter;            // valid values: 0h .. ffffh



/* device names in plain text for diagnostic purpose */
/* (index is same as DEVICE_ID) */
const STRING szDevName[] =
{   "Unknown_Device",
    "All_Devices",
    "Main_Device",
    "Intro_Device",
    "Tripcounter_Device",
    "Settings_Device",
    "Menu_Device",
    "Test_Device",
    "Diagnose_Device",
    "Debug_Device",
    "Warning_Device"
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
    }
    return ERR_OK;
}


