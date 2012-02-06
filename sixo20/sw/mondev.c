/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2004-04-10 by Markus Mueller
 *  Project:        SIxO
 *  Module:         monitor device
 *  Purpose:        handles the monitor device screen
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
 * Revision 3.4  2012/02/06 20:54:14  tuberkel
 * Just renamed all 'Devices' function prefixes for better readability
 *
 * Revision 3.3  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 3.2  2012/01/14 10:26:44  tuberkel
 * LED PWM handling changed:
 * - no longer Msgs/TimerMsgs used (inaccurate optic)
 * - instead use TimerISR to control PWM
 * - Granulartiy is SystemTicks (20 ms)
 * - works well
 * - prevent direct LEDDrv access (if possible)
 *
 * Revision 3.1  2012/01/14 08:28:42  tuberkel
 * Message-IDs shortened / reviewed
 *
 * Revision 3.0  2010/11/07 09:12:09  tuberkel
 * V30 Preparations:
 * - Device/Object Handling completely revised & simplified
 *
 * Revision 2.6  2009/07/19 12:31:34  tuberkel
 * - ObjectInit reviewed
 * - dyn. text handling disabled
 *
 * Revision 2.5  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.4  2009/07/08 21:41:17  tuberkel
 * All compiler defines reviewed:
 * - set to unique usage: set define to 0 or 1
 * - default values set, if not used
 * - see 'Project Editor' for details
 *
 * Revision 2.3  2009/06/21 17:55:56  tuberkel
 * Changes done by AN:
 * DisplDrawHorLine() new Mode-Parameter
 *
 * Revision 2.2  2007/03/30 10:06:51  tuberkel
 * BugFix: added missing DisplayClear at Init
 *
 * Revision 2.1  2007/03/26 23:29:59  tuberkel
 * changed MOTOBAU version handling
 * - eBikeType -> #define
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.6  2006/02/18 14:57:16  Ralf
 * - BIKE_MOTOBAU: this device disabled
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
#include "digoutdr.h"
#include "digindrv.h"
#include "sysparam.h"
#include "anaindrv.h"
#include "anain.h"
#include "led.h"
#include "surveill.h"
#include "measure.h"
#include "beep.h"
#include "timedate.h"



/* defines */




/* external symbols */
extern UINT16               wMilliSecCounter;           /* valid values: 0h .. ffffh */
extern STRING far           szDevName[];                /* device names */
extern DEVFLAGS1_TYPE        gDeviceFlags1;               /* system parameters */

extern BIKE_TYPE        gBikeType;          /* bike type selcetion */

extern UINT16       Volt_Min;
extern far UINT16   Volt_Min_def;
extern UINT16       Volt_Max;
extern far UINT16   Volt_Max_def;
extern INT16        TAir_Min;           // NOTE:    We have to declare the default values
extern INT16 far    TAir_Min_def;       //          with 'far', because they are defined
extern INT16        TAir_Max;           //          as 'const' in ROM in another module.
extern INT16 far    TAir_Max_def;
extern INT16        TOil_Min;
extern INT16 far    TOil_Min_def;
extern INT16        TOil_Max;
extern INT16 far    TOil_Max_def;
extern INT16        TWat_Min;
extern INT16 far    TWat_Min_def;
extern INT16        TWat_Max;
extern INT16 far    TWat_Max_def;

extern SPEED_TYPE       Speed_Max;
extern SPEED_TYPE  far  Speed_Max_def;
extern RPM_TYPE         RPM_Max;
extern RPM_TYPE    far  RPM_Max_def;

extern TIME_TYPE_LD  EngRunTime_Srv;
extern TIME_TYPE_LD  EngRunTime_All;






/* MONITOR DEVICE APPEARANCE */

/*   ....!....!....!....!.
    +---------------------+    This is MOTOBAU apearance
    |Batterie:       12.2V|    of the monitor device
    |Luft:+10C  -10C  +35C|    'C' stands for '°C'
    |Öl:  105C  -13C +130C|    special character
    |H2O:  80C   -2C +105C|
    |DRZ-max:  10200 U/Min|
    |v-max:      195 km/h |
    |Serv. 65h  Ges. 1295h|
    |     < Alles ok >    |
    +---------------------+ */

/*   ....!....!....!....!.
    +---------------------+    This is standard apearance
    |Gerät    33°C        |    of the monitor device
    |Luft     34°C |  max |
    |Wasser  112°C | 130°C|
    |Öl       80°C | 140°C|
    |Batt.   14,0V | 15,9V|
    |---------------------|
    |   >> Alles ok <<    |
    +---------------------+ */




/* device static objects */
static DEVDATA      MonitorScreenDev;       /* this device */

static TEXTOBJECT   BattObj;
static char         szBatt[STAT_TXT_LEN];
static TEXTOBJECT   DevTempObj;
static char         szDevTemp[STAT_TXT_LEN];
static TEXTOBJECT   AirTempObj;
static char         szAirTemp[STAT_TXT_LEN];
static TEXTOBJECT   OilTempObj;
static char         szOilTemp[STAT_TXT_LEN];
static TEXTOBJECT   WatTempObj;
static char         szWatTemp[STAT_TXT_LEN];
static TEXTOBJECT   RPMmaxObj;
static char         szRPMmax[STAT_TXT_LEN];
static TEXTOBJECT   VmaxObj;
static char         szVmax[STAT_TXT_LEN];
static TEXTOBJECT   RunTimeObj;
static char         szRunTime[STAT_TXT_LEN];
static TEXTOBJECT   StatusObj;

extern char         szSurvGlobalState[VEHSTATE_TXT_LEN];    /* vehicle state string */



// textobjects type for this device ----------------------------



static TEXTOBJECT_INITTYPE TextObjInit[] =
{

#if(BIKE_MOTOBAU==1)

    // SPECIAL MOTOBAU Monitor device resources ----------------------------
    /*pObject            X   Y  Font          H  Width  Align     Format    string ptr   State      */
    /*----------------- --- --- ------------ --- ----- --------- ---------- ------------ ---------- */
    { &BattObj,          0,  0, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szBatt,      OC_DISPL | OC_DYN },
    { &AirTempObj,       0,  8, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szAirTemp,   OC_DISPL | OC_DYN },
    { &OilTempObj,       0, 16, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szOilTemp,   OC_DISPL | OC_DYN },
    { &WatTempObj,       0, 24, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szWatTemp,   OC_DISPL | OC_DYN },
    { &RPMmaxObj,        0, 32, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szRPMmax,    OC_DISPL | OC_DYN },
    { &VmaxObj,          0, 40, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szVmax,      OC_DISPL | OC_DYN },
    { &RunTimeObj,       0, 48, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szRunTime,   OC_DISPL | OC_DYN },
    { &StatusObj,        0, 56, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szSurvGlobalState,OC_DISPL | OC_DYN }
    /*----------------- --- --- ------------ --- ----- --------- ---------- ------------ ---------- */

#else // BIKE_MOTOBAU

    // STANDARD Monitor device resources ----------------------------
    /*pObject            X   Y  Font          H  Width  Align     Format    string ptr   State      */
    /*----------------- --- --- ------------ --- ----- --------- ---------- ------------ ---------- */
    { &DevTempObj,       0,  0, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szDevTemp,   OC_DISPL | OC_DYN  },
    { &AirTempObj,       0, 10, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szAirTemp,   OC_DISPL | OC_DYN  },
    { &WatTempObj,       0, 20, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szWatTemp,   OC_DISPL | OC_DYN  },
    { &OilTempObj,       0, 30, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szOilTemp,   OC_DISPL | OC_DYN  },
    { &BattObj,          0, 40, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szBatt,      OC_DISPL | OC_DYN  },
    { &StatusObj,        0, 56, DPLFONT_6X8,  1,   21, TXT_LEFT, TXT_NORM,  szSurvGlobalState,OC_DISPL | OC_DYN  }
    /*----------------- --- --- ------------ --- ----- --------- ---------- ------------ ---------- */

#endif // BIKE_MOTOBAU
};
#define TEXTOBJECTLISTSIZE   (sizeof(TextObjInit)/sizeof(TEXTOBJECT_INITTYPE))


/* this devices object focus handling - list of all objects */
/* NOTE:    this device does not need any focus, this
            list is for common object handling only! */
static const void far * ObjectList[] =
{
    // all text objects
#if(BIKE_MOTOBAU==1)

    (void far *) &BattObj,
    (void far *) &AirTempObj,
    (void far *) &OilTempObj,
    (void far *) &WatTempObj,
    (void far *) &RPMmaxObj,
    (void far *) &VmaxObj,
    (void far *) &RunTimeObj,
    (void far *) &StatusObj,

#else // BIKE_MOTOBAU

    (void far *) &DevTempObj,
    (void far *) &AirTempObj,
    (void far *) &WatTempObj,
    (void far *) &OilTempObj,
    (void far *) &BattObj,
    (void far *) &StatusObj,

#endif // BIKE_MOTOBAU
};
#define OBJECTLIST_SIZE   (sizeof(ObjectList)/sizeof(OBJSTATE)/sizeof(void far *))


/* internal prototypes */
void MonDev_UpdateStrings ( void );
ERRCODE MonDev_MsgEntry_Reset(MESSAGE Msg);



/***********************************************************************
 *  FUNCTION:       MonDev_Init
 *  DESCRIPTION:    all initial stuff for all objects of
 *                  'monitor screen device'
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MonDev_Init(void)
{
    int i;

    /* device main data */
    MonitorScreenDev.eDevID       = DEVID_MONITOR;
    MonitorScreenDev.szDevName    = szDevName[DEVID_MONITOR];
    MonitorScreenDev.fFocused     = FALSE;
    MonitorScreenDev.fScreenInit  = FALSE;

    /* initialize all objects of any type */
    DevObjInit( &MonitorScreenDev, (void far *)TextObjInit,   TEXTOBJECTLISTSIZE,     OBJT_TXT   );

    // initialize this devices objects list
    MonitorScreenDev.Objects.ObjList       = ObjectList;
    MonitorScreenDev.Objects.ObjCount      = OBJECTLIST_SIZE;
    MonitorScreenDev.Objects.FirstSelObj   = DevObjGetFirstSelectable(&MonitorScreenDev, ObjectList, OBJECTLIST_SIZE );
    MonitorScreenDev.Objects.LastSelObj    = DevObjGetLastSelectable (&MonitorScreenDev, ObjectList, OBJECTLIST_SIZE );

    /* reset focus handling to start values */
    DevObjFocusReset( &MonitorScreenDev, ObjectList, OBJECTLIST_SIZE );

    /* return */
    ODS( DBG_SYS, DBG_INFO, "- MonDev_Init() done!");
    return ERR_OK;
}

/***********************************************************************
 *  FUNCTION:       MonDev_Show
 *  DESCRIPTION:    bring updated 'monitor screen device' to display
 *                  by calling Show-Fct. of all objects
 *  PARAMETER:      BOOL    TRUE = show objects, FALSE = clear screen
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MonDev_Show(BOOL fShow)
{
    UINT8   ShowMode;
    ERRCODE error = ERR_OK;
    int i;

    /* its on screen? */
    if (fShow == TRUE)
    {
        /* update all monitor strings */
        MonDev_UpdateStrings();

        /* do we have to repaint all? */
        if (MonitorScreenDev.fScreenInit == FALSE)
        {
            // repaint all stuff
            DisplClearScreen(0x00);
            MonitorScreenDev.fScreenInit = TRUE;
            ShowMode = SHOW_ALL;

            /* special NOT MOTOBAU behaviour */
            #if(BIKE_MOTOBAU==0)
            /* horizontal line between value list and status lines */
            {   DISPLXY Coord = {0,50};                 /* to be removed to an 'LineObject' !!! */
                DisplDrawHorLine(&Coord, 128, 0x03, DPLXOR);
            }
            #endif // BIKE_MOTOBAU
        }
        else
        {   ShowMode = SHOW_EDIT | SHOW_CURSOR;    // repaint only potential changed stuff
        }

        // FOR DISPLAY OBJECTS TEST ONLY: clear & proof display mode of objects!
        //DisplClearScreen(0xaa);

        /* process complete (active) object to show all objects */
        DevObjShow(&MonitorScreenDev,
                    MonitorScreenDev.Objects.ObjList,
                    MonitorScreenDev.Objects.ObjCount,
                    ShowMode );
    }
    else // parameter FALSE: clear screen only!
    {
        // clear screen
        DisplClearScreen(0x0);

        // reset states of all objects of this device
        DevObjClearState(  &MonitorScreenDev,
                            MonitorScreenDev.Objects.ObjList,
                            MonitorScreenDev.Objects.ObjCount,
                            OS_DISPL | OS_EDIT );

        // set overall device state to 'not init'
        MonitorScreenDev.fScreenInit  = FALSE;
    }
}


/***********************************************************************
 *  FUNCTION:       MonDev_MsgEntry
 *  DESCRIPTION:    Receive Message Handler of 'monitor screen' device
 *                  called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MonDev_MsgEntry(MESSAGE GivenMsg)
{
    ERRCODE      RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID   MsgId;
    MESSAGE      NewMsg;

    /* analyze message: it's for us? */
    MsgId = MSG_ID(GivenMsg);        /* get message id */
    switch (MsgId)
    {
        case MSG_GET_FOCUS:
        {
            /*  Some device want's to get the focus:
                If we've currently got the focus, we'll
                answer to SET his focus! */
            if ( MonitorScreenDev.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_MONITOR]);
                MSG_BUILD_SETFOCUS(NewMsg, DEVID_MONITOR, MSG_SENDER_ID(GivenMsg));   /* build answer message */
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                            /* send answer message */
                MonitorScreenDev.fFocused = FALSE;                                        /* clear our focus */
                MonDev_Show(FALSE);                                                 /* clear our screen */
                RValue = ERR_MSG_PROCESSED;
            }
        } break;
        case MSG_SET_FOCUS:
        {
            /* messsage for us? */
            if (MSG_RECEIVER_ID(GivenMsg) == DEVID_MONITOR)
            {
                /*  Someone wants us to take the focus?
                    We assume, that nobody else has the focus
                    and we've got the the screen now for us! */
                if (  (MonitorScreenDev.fFocused   == TRUE             )
                    &&(MSG_RECEIVER_ID(GivenMsg) == DEVID_MONITOR) )
                    ODS2(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but it already has focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_MONITOR]);
                else /*
                    ODS2(   DBG_SYS, DBG_INFO,
                            "FOCUS: %s -> %s!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_MONITOR]) */ ;
                MonitorScreenDev.fFocused = TRUE;                         /* set our focus */
                MonDev_Show(TRUE);                                  /* show our screen */
                gDeviceFlags1.flags.ActDevNr = DEVID_MONITOR;              /* save device# for restore */
                RValue = ERR_MSG_PROCESSED;
             }
             else
             {
                if ( MonitorScreenDev.fFocused == TRUE )
                {
                    /*  Some other device should be given the focus,
                        BUT WE'VE GOT THE FOCUS!
                        It should first ask US to give the focus! :-( */
                    ODS3(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but %s had focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[MSG_RECEIVER_ID(GivenMsg)],
                            szDevName[DEVID_MONITOR]);
                    MonitorScreenDev.fFocused = FALSE;                        /* loose our focus */
                    ODS1(   DBG_SYS, DBG_WARNING,
                            "%s now loosing focus :-( ",
                            szDevName[DEVID_MONITOR]);
                    RValue = ERR_MSG_NOT_PROCESSED;
                }
             }
        } break;
    }

    /* this part is only for FOCUSED time
       AND msg not already processed */
    if (  (MonitorScreenDev.fFocused == TRUE )
        &&(RValue == ERR_MSG_NOT_PROCESSED ) )
    {
        switch (MsgId)
        {
            case MSG_SCREEN_RFRSH:
                MonDev_Show(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;
            case MSG_KEYS_PRESSED:
                /* check if UP&DOWN are pressed short: */
                if (  (RValue == ERR_MSG_NOT_PROCESSED                    )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN))
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYSHORT              ) )
                {
                    /* give focus immediatly to next device  */
                    MonitorScreenDev.fFocused = FALSE;                                        /* clear our focus */
                    MonDev_Show(FALSE);                                                 /* clear our screen */
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_MONITOR, DEVID_SET);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_KEY_UP:
               if (MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED)
               {
                   /* VehicleSurveillance: scroll back in list of active status messages */
                   SurvScrollVehicleState(VST_SCROLL_UP);
                   MonDev_Show(TRUE);     // force refresh!
                   RValue = ERR_MSG_PROCESSED;
               }
               break;
            case MSG_KEY_DOWN:
               if (MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED)
               {
                   /* VehicleSurveillance: scroll forward in list of active status messages */
                   SurvScrollVehicleState(VST_SCROLL_DOWN);
                   MonDev_Show(TRUE);     // force refresh!
                   RValue = ERR_MSG_PROCESSED;
               }
               break;
            case MSG_DPL_FLASH_ON:
            case MSG_DPL_FLASH_OFF:
            case MSG_KEY_OK:
                /* Try to reset all Min/Max values (if [OK]-long) */
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = MonDev_MsgEntry_Reset(GivenMsg);
                break;

            default: return ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}


/***********************************************************************
 *  FUNCTION:       MonDev_UpdateStrings
 *  DESCRIPTION:    Generates fresh information for all strings
 *                  shown in this device
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MonDev_UpdateStrings ( void )
{
    char   szDataBuff[5];
    char   szDataBuff_Min[5];
    char   szDataBuff_Max[5];
    UINT16 wBuffer;
    INT16  iBuffer;

    // select special behaviour
    #if(BIKE_MOTOBAU==1)
    {
        // special MOTOBAU apearance =========================================

        // battery string:    |Batterie:       12.2V|
        AnaInFormatVoltage( AnaInGetVoltage(), szDataBuff, sizeof(szDataBuff));
        sprintf (   (char far *)szBatt,
                    "%-16s%4s%s",
                    RESTXT_STAT_BATT,
                    (char far *)szDataBuff,
                    RESTXT_STAT_BATT_DESC);

        // air temperature string   |Luft: 10°C (10..35°C)|
        AnaInFormatTemperature( AnaInGetAirTemperature(), szDataBuff,     sizeof(szDataBuff));
        AnaInFormatTemperature( TAir_Min,                 szDataBuff_Min, sizeof(szDataBuff_Min));
        AnaInFormatTemperature( TAir_Max,                 szDataBuff_Max, sizeof(szDataBuff_Max));
        sprintf (   (char far *)szAirTemp,
                    "%s%s\x8f %s\x8f\x8c %s\x8f\x8b",   // \x8f = °C, \x8c = Down-Arrow, \x8b = Up-Arrow,
                    RESTXT_STAT_TEMP_AIR,
                    (char far *)szDataBuff,
                    (char far *)szDataBuff_Min,
                    (char far *)szDataBuff_Max );

        // oil temperature string   |Öl:  105°C  (..130°C)|
        AnaInFormatTemperature( AnaInGetOilTemperature(), szDataBuff,     sizeof(szDataBuff));
        AnaInFormatTemperature( TOil_Min,                 szDataBuff_Min, sizeof(szDataBuff_Min));
        AnaInFormatTemperature( TOil_Max,                 szDataBuff_Max, sizeof(szDataBuff_Max));
        sprintf (   (char far *)szOilTemp,
                    "%s%s\x8f %s\x8f\x8c %s\x8f\x8b",     // \x8f = °C, \x8c = Down-Arrow, \x8b = Up-Arrow,
                    //"%s%s\x8f       %s\x8f\x8b",            // Minimum disabled
                    RESTXT_STAT_TEMP_OIL,
                    (char far *)szDataBuff,
                    (char far *)szDataBuff_Min,           // Minimum disabled
                    (char far *)szDataBuff_Max );

        // water temperature string |H2O:  80°C  (..105°C)|
        AnaInFormatTemperature( AnaInGetWatTemperature(), szDataBuff,     sizeof(szDataBuff));
        AnaInFormatTemperature( TWat_Min,                   szDataBuff_Min, sizeof(szDataBuff_Min));
        AnaInFormatTemperature( TWat_Max,                   szDataBuff_Max, sizeof(szDataBuff_Max));
        sprintf (   (char far *)szWatTemp,
                    "%s%s\x8f %s\x8f\x8c %s\x8f\x8b",     // \x8f = °C, \x8c = Down-Arrow, \x8b = Up-Arrow,
                    //"%s%s\x8f       %s\x8f\x8b",            // Minimum disabled
                    RESTXT_STAT_TEMP_WAT,
                    (char far *)szDataBuff,
                    (char far *)szDataBuff_Min,             // Minimum disabled
                    (char far *)szDataBuff_Max );

        // RPM max string           |DRZ-max:  10200 U/Min|
        sprintf (   (char far *) szRPMmax,
                    "%-10s%5u %s",
                    RESTXT_STAT_RPMMAX_DESC,
                    RPM_Max,
                    RESTXT_STAT_RPMMAX_UNIT );

         // v max string             |v-max:      195 km/h |
        sprintf (   (char far *) szVmax,
                    "%-10s%5u %s ",
                    RESTXT_STAT_VMAX_DESC,
                    Speed_Max,
                    RESTXT_STAT_VMAX_UNIT );

        // runtime string           |Serv. 65h  Ges. 1295h|
        sprintf (   (char far *) szRunTime,
                    "%s%3uh  %s%5uh",
                    RESTXT_STAT_H_SERV,
                    EngRunTime_Srv.wHour,
                    RESTXT_STAT_H_ALL,
                    EngRunTime_All.wHour );
    }
    #else // BIKE_MOTOBAU
    {
        // STANDARD SIXO BEHAVIOUR ======================================

        // dev temperature string   |Gerät    33°C        |
        AnaInFormatTemperature( AnaInGetTemperature(), szDataBuff,     sizeof(szDataBuff));
        sprintf (   (char far *)szDevTemp,
                    "%s%3s%s",
                    RESTXT_MON_TEMP_DEV,
                    (char far *)szDataBuff,
                    RESTXT_DEGC_DESC );

        // air temperature string   |Luft     34°C |  Max |
        AnaInFormatTemperature( AnaInGetAirTemperature(), szDataBuff,     sizeof(szDataBuff));
        sprintf (   (char far *)szAirTemp,
                    "%s%3s%s |  Max",
                    RESTXT_MON_TEMP_AIR,
                    (char far *)szDataBuff,
                    RESTXT_DEGC_DESC );

        // water temperature string |Wasser  112°C | 130°C|
        AnaInFormatTemperature( AnaInGetWatTemperature(), szDataBuff,     sizeof(szDataBuff));
        AnaInFormatTemperature( TWat_Max,                 szDataBuff_Max, sizeof(szDataBuff_Max));
        sprintf (   (char far *)szWatTemp,
                    "%s%3s%s | %3s%s",
                    RESTXT_MON_TEMP_WAT,
                    (char far *)szDataBuff,
                    RESTXT_DEGC_DESC,
                    (char far *)szDataBuff_Max,
                    RESTXT_DEGC_DESC  );

        // oil temperature string   |Öl       80°C | 140°C|
        AnaInFormatTemperature( AnaInGetOilTemperature(), szDataBuff,     sizeof(szDataBuff));
        AnaInFormatTemperature( TOil_Max,                 szDataBuff_Max, sizeof(szDataBuff_Max));
        sprintf (   (char far *)szOilTemp,
                    "%s%3s%s | %3s%s",
                    RESTXT_MON_TEMP_OIL,
                    (char far *)szDataBuff,
                    RESTXT_DEGC_DESC,
                    (char far *)szDataBuff_Max,
                    RESTXT_DEGC_DESC  );

        // battery string:    |Batt.   14,0V | 15,9V|
        AnaInFormatVoltage( AnaInGetVoltage(), szDataBuff, sizeof(szDataBuff));
        AnaInFormatVoltage( Volt_Max, szDataBuff_Max, sizeof(szDataBuff_Max));
        sprintf (   (char far *)szBatt,
                    "%s%4s%s | %4s%s",
                    RESTXT_MON_BATT,
                    (char far *)szDataBuff,
                    RESTXT_VOLT_DESC,
                    (char far *)szDataBuff_Max,
                    RESTXT_VOLT_DESC);

        // END OF SPECIAL BEHAVIOUR ======================================
    }
    #endif // BIKE_MOTOBAU
}


/***********************************************************************
 *  FUNCTION:       MonDev_MsgEntry_Reset
 *  DESCRIPTION:    Handles the 'all reset' messages for all
 *                  min/max values
 *  PARAMETER:      MESSAGE
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MonDev_MsgEntry_Reset(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    static BOOL fLocked = FALSE;                    /* TRUE = key not yet relases */

    /* [OK] pressed+released long'? -> ITS FOR US! -> Re-enable Reset of distances!
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
        // RESET ALL!
        ODS( DBG_SYS, DBG_INFO, "Reset all Min/Max values!");

        // using default values from 'sysparam.c'
        TAir_Min = TAir_Min_def;
        TAir_Max = TAir_Max_def;
        TOil_Min = TOil_Min_def;
        TOil_Max = TOil_Max_def;
        TWat_Min = TWat_Min_def;
        TWat_Max = TWat_Max_def;

        /* special MOTOBAU behaviour */
        #if(BIKE_MOTOBAU==1)
        {   Speed_Max = Speed_Max_def;
            RPM_Max   = RPM_Max_def;
        }
        #else // BIKE_MOTOBAU
        {   Volt_Min = Volt_Min_def;          /* standard behaviour */
            Volt_Max = Volt_Max_def;
        }
        #endif // BIKE_MOTOBAU
        Beep_SignalOk();                   /* beep ok */
        LED_SignalOk();                    /* LED ok */
        fLocked = TRUE;             /* don't repeat this until key released */
        RValue = ERR_MSG_PROCESSED; /* done */
    }
}




