/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-22 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         main devices
 *  Purpose:        handles the main screen (speed & rpm) device
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
 *      Arnold Neugebauer                   Ralf Krizsan
 *      Stöckener Str. 115                  Knickstr. 10
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
 * Revision 2.5  2009/06/24 21:11:08  tuberkel
 * just removed a comma in string
 *
 * Revision 2.4  2009/06/21 17:55:27  tuberkel
 * Changes done by AN:
 * DisplDrawHorLine() new Mode-Parameter
 *
 * Revision 2.3  2007/03/30 10:08:05  tuberkel
 * - Matthias Otto stuff:  language specific display content
 * - Added error check for wrong system init values
 *
 * Revision 2.2  2007/03/26 23:17:31  tuberkel
 * just comments
 *
 * Revision 2.1  2006/10/19 18:56:15  tuberkel
 * BugFix: ANAIN_TEMP_OFFSET -> ANAIN_TEMP_SENSORDETECT
 * - now uses values <-20° to detect missing temp sensor
 * BugFix:
 * - Reset-Beeper only if necessary
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 * Revision 1.18  2006/03/18 09:00:57  Ralf
 * - TripCounter improved to 4 different counters
 *
 *
 *  --------------------------------------------------------------------
 */


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
#include "measure.h"
#include "sysparam.h"
#include "beep.h"
#include "led.h"
#include "timedate.h"
#include "anain.h"
#include "surveill.h"



/* defines for device data */
#define SPEED_STRSIZE   6               /* to contain max. string '333,3' km/h */
#define RPM_STRSIZE     6               /* to contain max. string '13500' U/Min */


/* main device states
   defines, which object to be shown under
   line of tachometer */
typedef enum
{
    eMainFirst,         /* invalid state*/
    eMainMonitor,       /* monitor sub screen (tempereatures, voltage...) */
    eMainRPM,           /* show engine speed RPM */
    eMainFuelKm,        /* show fuel distance */
    eMainTrip1Km,       /* show TripCounter1 distance */
    eMainTrip2Km,       /* show TripCounter2 distance */
    eMainVehKm,         /* show vehicle overall distance */
    eMainSpeedMax,      /* show Speed Max value */
    eMainLast,          /* invalid state*/
} MAINDEVSTATE;

/* external symbols */
extern UINT16            wMilliSecCounter;                  /* valid values: 0h .. ffffh */
extern STRING far        szDevName[];                       /* device names */
extern SYSFLAGS_TYPE     gSystemFlags;                      /* system parameters */
extern char              szVehicState[VEHSTATE_TXT_LEN];    /* vehicle state string */


/* external bitmaps */
extern unsigned char far rgEnduroSymbol29x16[];     /* enduro symbol  */
extern unsigned char far rgFuelSymbo16x16[];        /* fuel symbol */
extern unsigned char far rgFuelSymbol8x8[];         /* fuel symbol small */
extern unsigned char far rgInfoSymbol16x16[];       /* info symbol */
extern unsigned char far rgHazardSymbol16x16[];     /* Hazard symbol */
extern unsigned char far rgOilSymbol8x8[];          /* oil symbol */
extern unsigned char far rgWaterSymbol8x8[];        /* water symbol */
extern unsigned char far rgTempSymbol8x8[];         /* temperature symbol */
extern unsigned char far rgBatterySymbol8x8[];      /* battery symbol */
extern unsigned char far rgClockSymbol8x8[];        /* clock symbol */
extern unsigned char far rgRPMSymbol8x8[];          /* RPM symbol */


/* device static objects */
static DEVDATA      MainDevice;                 /* this device */

static TEXTOBJECT   SpeedTxtObj;                /* speed text object */
static TEXTOBJECT   SpeedDescTxtObj;            /* speed descriptor 'km/h' text object */
static CHAR         szSpeed[6] = "0,0";         /* buffer for current speed, max. string '333,3' */

static TEXTOBJECT   RPMTxtObj;                  /* rpm text object */
static TEXTOBJECT   RPMDescTxtObj;              /* rpm descriptor 'U/Min' text object */
static CHAR         szRPM[6] = "0";             /* buffer current eng speed, max. string '13500' */

static TEXTOBJECT   FuelDistTxtObj;             /* fuel distance text object */
static CHAR         szFuelDist[10] = "0,0";     /* buffer to contain fuel distance, max. string '9999999,9' */
static BMPOBJECT    FuelSymbolBmpObj;           /* symbol to indicate fuel distance display mode */

static TEXTOBJECT   VehDistTxtObj;              /* vehicle distance text object */
static CHAR         szVehDist[10] = "0,0";      /* buffer to contain fuel distance, max. string '9999999,9' */
static BMPOBJECT    VehicSymbolBmpObj;          /* symbol to indicate vehicle distance display mode */

static TEXTOBJECT   Trip1DescTxtObj;            /* tripcounter1 descriptor text object 'T1' */
static TEXTOBJECT   Trip1DistTxtObj;            /* tripcounter1 distance text object */
static CHAR         szTrip1Dist[10] = "0,0";    /* buffer to contain tripcounter1 distance, max. string '9999,9' */

static TEXTOBJECT   Trip2DescTxtObj;            /* tripcounter1 descriptor text object 'T2' */
static TEXTOBJECT   Trip2DistTxtObj;            /* tripcounter2 distance text object */
static CHAR         szTrip2Dist[10] = "0,0";    /* buffer to contain tripcounter2 distance, max. string '9999,9' */

static TEXTOBJECT   DistDescTxtObj;             /* COMMON vehicle & fuel distance decriptor for 'km' */

static TEXTOBJECT   SpeedMaxDescTxtObj;         /* SpeedMax descriptor text object for 'v(max)' */
static TEXTOBJECT   SpeedMaxUnitTxtObj;         /* speed max desciptor text object 'km/h' */
static TEXTOBJECT   SpeedMaxTxtObj;             /* SpeedMax descriptor text object for '110.0' */
static CHAR         szSpeedMax[4] = "  0";      /* buffer to contain SpeedMax, max. string '999' km/h*/
extern SPEED_TYPE   Speed_Max;                  /* prepared value */

static TEXTOBJECT   TimeDateTxtObj;             /* time & date output opbject */
static CHAR         szTimeDate[22] = "Mo  01.01.01 00:00:00";   /* buffer for timedate string */

static BMPOBJECT    InfoSymbolBmpObj;           /* info symbol for monitor */
static BMPOBJECT    HazardSymbolBmpObj;         /* hazard symbol for monitor */
static BMPOBJECT    BatterySymbolBmpObj;        /* battery symbol for monitor */
static BMPOBJECT    TempSymbolBmpObj;           /* temp symbol for monitor */

static TEXTOBJECT   TempTxtObj;                 /* temperature from external air / internal NTC sensor */
static TEXTOBJECT   TempDescTxtObj;
static CHAR         szTemp[5];

static TEXTOBJECT   VehicStateTxtObj;           /* Vehicle State detected by MonitorDevice */

static TEXTOBJECT   VoltageTxtObj;              /* system voltage */
static TEXTOBJECT   VoltageDescTxtObj;
static CHAR         szVoltage[5];

static TEXTOBJECT   OilTempTxtObj;              /* oil temp */
static TEXTOBJECT   OilTempDescTxtObj;
static BMPOBJECT    OilTempSymbolBmpObj;        /* oil temp symbol for monitor */
static CHAR         szOilTemp[5];
/* RPM displayed in monitor mode if no oil temp available */
static TEXTOBJECT   RPM2TxtObj;                 /* RPM */
static BMPOBJECT    RPMSymbolBmpObj;

static TEXTOBJECT   WaterTempTxtObj;            /* water temp */
static TEXTOBJECT   WaterTempDescTxtObj;
static CHAR         szWaterTemp[5];
static BMPOBJECT    WaterTempSymbolBmpObj;      /* water temp symbol for monitor */
/* fuel distance displayed in monitor mode if water temp not available */
static TEXTOBJECT   FuelDist2TxtObj;            /* fuel distance text object */
static BMPOBJECT    Fuel2SymbolBmpObj;

extern INT8         CurrentNOKStates;           /* number of states currently not ok */
static INT8         ShowVehicleState = 0;       /* != 0 if vehicle state is to be displayed */

/* internal prototypes */
ERRCODE MainDeviceStateMachine(MESSAGE Msg);
ERRCODE MainDeviceResetMsg(MESSAGE Msg);
ERRCODE MainDeviceShowVehicStateMsg(MESSAGE Msg);
void MainDeviceUpdateTimeDate(void);


/* bitmap object table */
static const BMPOBJECT_INITTYPE BmpObjects[] =
{
    { &FuelSymbolBmpObj,           4, 38, 16, 16, rgFuelSymbo16x16,     DPLNORM, FALSE },
    { &VehicSymbolBmpObj,          0, 38, 29, 16, rgEnduroSymbol29x16,  DPLNORM, FALSE },

    /* monitor symbols ------------------------ */
    { &InfoSymbolBmpObj,           2, 37, 16, 16, rgInfoSymbol16x16,    DPLNORM, FALSE },
    { &HazardSymbolBmpObj,         2, 37, 16, 16, rgHazardSymbol16x16,  DPLNORM, FALSE },
    { &BatterySymbolBmpObj,       77, 37,  8,  8, rgBatterySymbol8x8,   DPLNORM, FALSE },
    { &TempSymbolBmpObj,          25, 37,  8,  8, rgTempSymbol8x8,      DPLNORM, FALSE },
    { &WaterTempSymbolBmpObj,     24, 46,  8,  8, rgWaterSymbol8x8,     DPLNORM, FALSE },
    { &Fuel2SymbolBmpObj,         24, 46,  8,  8, rgFuelSymbol8x8,      DPLNORM, FALSE },
    { &OilTempSymbolBmpObj,       77, 46,  8,  8, rgOilSymbol8x8,       DPLNORM, FALSE },
    { &RPMSymbolBmpObj,           77, 46,  8,  8, rgRPMSymbol8x8,       DPLNORM, FALSE }
};


/* text object table */
static const TEXTOBJECT_INITTYPE TextObjects[] =
{
    /*pObject                   X    Y  Font            H  Width  Align     Format    string ptr        State      */
    /* speed ----------------- ---- --- -------------- --- ----- --------- ---------- ----------------- ---------- */
    { &SpeedTxtObj,              8,  0, DPLFONT_24X32,  1,  3, TXT_RIGHT,  TXT_NORM, szSpeed,           OC_DISPL },
    { &SpeedDescTxtObj,         90,  8, DPLFONT_8X16,   1,  4, TXT_LEFT,   TXT_NORM, RESTXT_SPEED_DESC, OC_DISPL },

    /* rpm ------------------- ---- --- -------------- --- ----- --------- ---------- ----------------- ---------- */
    { &RPMTxtObj,                8, 38, DPLFONT_14X16,  1,  5, TXT_RIGHT,  TXT_NORM, szRPM,             OC_DISPL },
    { &RPMDescTxtObj,           86, 38, DPLFONT_8X16,   1,  5, TXT_LEFT,   TXT_NORM, RESTXT_RPM_DESC,   OC_DISPL },

    /* fuel / vehicle distance ---- --- -------------- --- ----- --------- ---------- ----------------- ---------- */
    { &DistDescTxtObj,         106, 38, DPLFONT_8X16,   1,  2, TXT_LEFT,   TXT_NORM, RESTXT_DIST_DESC,  OC_DISPL },
    { &FuelDistTxtObj,          24, 38, DPLFONT_14X16,  1,  6, TXT_RIGHT,  TXT_NORM, szFuelDist,        OC_DISPL },
    { &VehDistTxtObj,           46, 38, DPLFONT_8X16,   1,  6, TXT_RIGHT,  TXT_NORM, szVehDist,         OC_DISPL },
    { &Trip1DescTxtObj,          4, 38, DPLFONT_8X16,   1,  2, TXT_LEFT,   TXT_NORM, RESTXT_TRIP1DESC,  OC_DISPL },
    { &Trip1DistTxtObj,         24, 38, DPLFONT_14X16,  1,  6, TXT_RIGHT,  TXT_NORM, szTrip1Dist,       OC_DISPL },
    { &Trip2DescTxtObj,          4, 38, DPLFONT_8X16,   1,  2, TXT_LEFT,   TXT_NORM, RESTXT_TRIP2DESC,  OC_DISPL },
    { &Trip2DistTxtObj,         24, 38, DPLFONT_14X16,  1,  6, TXT_RIGHT,  TXT_NORM, szTrip2Dist,       OC_DISPL },
    { &SpeedMaxDescTxtObj,       4, 38, DPLFONT_8X16,   1,  3, TXT_LEFT,   TXT_NORM, RESTXT_SPEEDMAX_DESC, OC_DISPL },
    { &SpeedMaxUnitTxtObj,      94, 38, DPLFONT_8X16,   1,  4, TXT_LEFT,   TXT_NORM, RESTXT_SPEED_DESC, OC_DISPL },
    { &SpeedMaxTxtObj,          44, 38, DPLFONT_14X16,  1,  3, TXT_RIGHT,  TXT_NORM, szSpeedMax,        OC_DISPL },

    /* temperature from internal sensor -------------- --- ----- --------- ---------- ----------------- ---------- */
    { &TempTxtObj,              36, 38, DPLFONT_6X8,    1,  3, TXT_RIGHT,  TXT_NORM, szTemp,            OC_DISPL },
    { &TempDescTxtObj,          55, 38, DPLFONT_6X8,    1,  2, TXT_RIGHT,  TXT_NORM, RESTXT_DEGC_DESC,  OC_DISPL },

    /* system voltage -------- ---- --- -------------- --- ----- --------- ---------- ----------------- ---------- */
    { &VoltageTxtObj,           90, 38, DPLFONT_6X8,    1,  4, TXT_RIGHT,  TXT_NORM, szVoltage,         OC_DISPL },
    { &VoltageDescTxtObj,      115, 38, DPLFONT_6X8,    1,  2, TXT_RIGHT,  TXT_NORM, RESTXT_VOLT_DESC,  OC_DISPL },

    /* water temp ------------ ---- --- -------------- --- ----- --------- ---------- ----------------- ---------- */
    /* fuel distance display if no water temp available */
    { &WaterTempTxtObj,         36, 47, DPLFONT_6X8,    1,  3, TXT_RIGHT,  TXT_NORM, szWaterTemp,       OC_DISPL },
    { &WaterTempDescTxtObj,     55, 47, DPLFONT_6X8,    1,  2, TXT_RIGHT,  TXT_NORM, RESTXT_DEGC_DESC,  OC_DISPL },
    { &FuelDist2TxtObj,         30, 47, DPLFONT_6X8,    1,  6, TXT_RIGHT,  TXT_NORM, szFuelDist,        OC_DISPL },

    /* oil temp -------------- ---- --- -------------- --- ----- --------- ---------- ----------------- ---------- */
    /* RPM display if no oil temp available */
    { &OilTempTxtObj,           97, 47, DPLFONT_6X8,    1,  3, TXT_RIGHT,  TXT_NORM, szOilTemp,         OC_DISPL },
    { &OilTempDescTxtObj,      115, 47, DPLFONT_6X8,    1,  2, TXT_RIGHT,  TXT_NORM, RESTXT_DEGC_DESC,  OC_DISPL },
    { &RPM2TxtObj,              97, 47, DPLFONT_6X8,    1,  5, TXT_RIGHT,  TXT_NORM, szRPM,             OC_DISPL },

    /* Time and Date + vehicle state text (changed when KEY_OK is pressed) ---------- ----------------- ---------- */
    { &TimeDateTxtObj,           2, 56, DPLFONT_6X8,    1, 21, TXT_CENTER, TXT_NORM, szTimeDate,        OC_DISPL },
    { &VehicStateTxtObj,         2, 56, DPLFONT_6X8,    1, 21, TXT_CENTER, TXT_NORM, szVehicState,      OC_DISPL },
    /*------------------------ ---- --- -------------- --- ----- --------- ---------- ----------------- ---------- */
};


/***********************************************************************
 *  FUNCTION:       MainDeviceInit
 *  DESCRIPTION:    all initial stuff for all objects
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MainDeviceInit(void)
{
    int i;

    /* device main data */
    MainDevice.eDevID       = DEVID_MAIN;
    MainDevice.szDevName    = szDevName[DEVID_MAIN];
    MainDevice.fFocused     = FALSE;
    MainDevice.fScreenInit  = FALSE;

    /* error check: */
    if (  (gSystemFlags.flags.MainDevState == eMainFirst)
        ||(gSystemFlags.flags.MainDevState >= eMainLast ) )
    {   ODS1( DBG_SYS, DBG_ERROR, "MainDeviceInit(): Invalid MainDevState %u corrected!", gSystemFlags.flags.MainDevState );
        gSystemFlags.flags.MainDevState = eMainFirst + 1;
    }
    MainDevice.wDevState    = gSystemFlags.flags.MainDevState;

    /* initialize bitmap objects */
    for (i = 0; i < ARRAY_SIZE(BmpObjects); i++)
    {
        ObjBmpInit(BmpObjects[i].pObject,
                   BmpObjects[i].wOrgPosX,
                   BmpObjects[i].wOrgPosY,
                   BmpObjects[i].wWidth,
                   BmpObjects[i].wHeight,
                   BmpObjects[i].rgBMPRawData,
                   BmpObjects[i].bMode,
                   BmpObjects[i].bState);
    }

    /* initialize text objects */
    for (i = 0; i < ARRAY_SIZE(TextObjects); i++)
    {
        ObjTextInit(TextObjects[i].pObject,
                    TextObjects[i].wOrgPosX,
                    TextObjects[i].wOrgPosY,
                    TextObjects[i].eFont,
                    TextObjects[i].bWindHeight,
                    TextObjects[i].bWindWidth,
                    TextObjects[i].eAlign,
                    TextObjects[i].eFormat,
                    TextObjects[i].szText,
                    TextObjects[i].bState);
    }

    /* return */
    return ERR_OK;
}

/***********************************************************************
 *  FUNCTION:       MainDeviceShow
 *  DESCRIPTION:    bring updated main device to display
 *                  by calling Show-Fct. of all relevant objects
 *  PARAMETER:      TRUE    show (all/partial) objects,
 *                  FALSE   clear screen
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MainDeviceShow(BOOL fShow)
{
    ERRCODE error = ERR_OK;
    MESSAGE NewMsg;             /* for screen fresh message */
    UINT16  wWheelSpeed;

    /* 'show' or 'clear' screen? */
    if (fShow == TRUE)
    {
        /* update of all displayable values */
        wWheelSpeed = MeasGetWheelSpeed(MR_KM_PER_H);
        sprintf( szSpeed,        "%3u",      wWheelSpeed);
        sprintf( szRPM,          "%5u",      MeasGetEngineSpeed(MR_RPM_R10));
        sprintf( szFuelDist,     "%4u%c%1u", MeasGetFuelDist(MR_KM_ONLY), RESTXT_DEC_SEPARATOR, MeasGetFuelDist(MR_HM_ONLY));
        sprintf( szVehDist,      "%06lu",    MeasGetVehicDist(MR_KM));
        sprintf( szTrip1Dist,    "%4u%c%1u", MeasGetTripCnt( eTRIPC_C, MR_KM_ONLY), RESTXT_DEC_SEPARATOR, MeasGetTripCnt( eTRIPC_C, MR_HM_ONLY));
        sprintf( szTrip2Dist,    "%4u%c%1u", MeasGetTripCnt( eTRIPC_D, MR_KM_ONLY), RESTXT_DEC_SEPARATOR, MeasGetTripCnt( eTRIPC_D, MR_HM_ONLY));
        sprintf( szSpeedMax,     "%3u",      Speed_Max);

        /* analog input values are already formated */
        if ( AnaInGetAirTemperature() > ANAIN_TEMP_SENSORDETECT )
             AnaInFormatTemperature(AnaInGetAirTemperature(), szTemp, sizeof(szTemp));  // use external air temp if available
        else AnaInFormatTemperature(AnaInGetTemperature(),    szTemp, sizeof(szTemp));  // use internal device temp else
        AnaInFormatTemperature(AnaInGetWatTemperature(), szWaterTemp, sizeof(szWaterTemp));
        AnaInFormatVoltage    (AnaInGetVoltage(),        szVoltage,   sizeof(szVoltage));
        AnaInFormatTemperature(AnaInGetOilTemperature(), szOilTemp,   sizeof(szOilTemp));

        /* do we have to repaint all? */
        if (MainDevice.fScreenInit == FALSE)
        {
            /* yes, repaint complete screen */
            DisplClearScreen(0x0);

            /* set new state */
            MainDevice.fScreenInit  = TRUE;

            /* horizontal line between wheel speed & rpm */
            {
                DISPLXY Coord = {0,34};                 /* to be removed to an 'LineObject' !!! */
                DisplDrawHorLine(&Coord, 128, 0x03, DPLXOR);
            }

            /* show always vehicle speed in upper display part */
            ObjTextShow( &SpeedTxtObj );
            ObjTextShow( &SpeedDescTxtObj );

            /* show lowest display line 'VehicleState' or 'TimeDate' */
            if (ShowVehicleState)
                 ObjTextShow( &VehicStateTxtObj );
            else MainDeviceUpdateTimeDate(); // initial display only!


            /* which data to be shown below speed? */
            switch (MainDevice.wDevState)
            {
                case eMainRPM:
                    ObjTextShow( &RPMTxtObj );
                    ObjTextShow( &RPMDescTxtObj );
                break;
                case eMainFuelKm:
                    ObjTextShow( &FuelDistTxtObj );
                    ObjTextShow( &DistDescTxtObj );
                    ObjBmpShow( &FuelSymbolBmpObj );
                break;
                case eMainVehKm:
                    ObjTextShow( &VehDistTxtObj );
                    ObjTextShow( &DistDescTxtObj );
                    ObjBmpShow( &VehicSymbolBmpObj );
                break;
                case eMainTrip1Km:
                    ObjTextShow( &Trip1DescTxtObj );
                    ObjTextShow( &Trip1DistTxtObj );
                    ObjTextShow( &DistDescTxtObj );
                break;
                case eMainTrip2Km:
                    ObjTextShow( &Trip2DescTxtObj );
                    ObjTextShow( &Trip2DistTxtObj );
                    ObjTextShow( &DistDescTxtObj );
                break;
                case eMainSpeedMax:
                    ObjTextShow( &SpeedMaxDescTxtObj );
                    ObjTextShow( &SpeedMaxTxtObj );
                    ObjTextShow( &SpeedMaxUnitTxtObj );
                break;
                case eMainMonitor:
                {
                    /* Markus Monitor-Part-in-Main-Devíce-Mode :-) */

                    // which icon has to be used: Hazard or Info?
                    ObjBmpShow( SurvGetNOKStates()? &HazardSymbolBmpObj : &InfoSymbolBmpObj );

                    // show supply voltage
                    ObjBmpShow( &BatterySymbolBmpObj );
                    ObjTextShow( &VoltageTxtObj );
                    ObjTextShow( &VoltageDescTxtObj );

                    // show internal/external temperature
                    ObjBmpShow( &TempSymbolBmpObj );
                    ObjTextShow( &TempTxtObj );
                    ObjTextShow( &TempDescTxtObj );
                    }

                    // Automatic Display Switch between WaterTemp / FuelDistance
                    // TBD: Will later be done via Settings, but at the moment we
                    //      use this so called 'Arnoldschen Elfenbeinturm' here! ;-)
                    // Water temp available?
                    if ( AnaInGetWatTemperature() > ANAIN_TEMP_SENSORDETECT )
                    {
                        ObjBmpShow( &WaterTempSymbolBmpObj );
                        ObjTextShow( &WaterTempTxtObj );
                        ObjTextShow( &WaterTempDescTxtObj );
                    }
                    else /* display fuel distance if no water temp sensor connected */
                    {
                        ObjTextShow( &FuelDist2TxtObj );
                        ObjBmpShow( &Fuel2SymbolBmpObj );
                    }

                    // Automatic Display Switch between OilTemp / RPM
                    // TBD: Will later be done via Settings, but at the moment we
                    //      use this so called 'Arnoldschen Elfenbeinturm' here! ;-)
                    // Oil temp available?
                    if ( AnaInGetOilTemperature() > ANAIN_TEMP_SENSORDETECT )
                    {
                        ObjBmpShow( &OilTempSymbolBmpObj );
                        ObjTextShow( &OilTempTxtObj );
                        ObjTextShow( &OilTempDescTxtObj );
                    }
                    else /* display RPM if no oil temp sensor connected */
                    {
                        ObjBmpShow( &RPMSymbolBmpObj );
                        ObjTextShow( &RPM2TxtObj );
                    }
                break;
                // case eMainClock:
                // break;
                default:
                    ODS1( DBG_SYS, DBG_ERROR,
                          "MainDeviceShow(): unknown state: %u", MainDevice.wDevState);
            }
        }
        else
        {
            /* No, repaint only changed stuff */
            ObjTextShow( &SpeedTxtObj );

            /* show lowest display line 'VehicleState' or 'TimeDate' */
            if (ShowVehicleState)
                 ObjTextShow( &VehicStateTxtObj );
            // else MainDeviceUpdateTimeDate(); // NO UPDATE HERE, will be done by special update messages only!

            /* which data to be shown below speed? */
            switch (MainDevice.wDevState)
            {
                case eMainRPM:      ObjTextShow( &RPMTxtObj );          break;
                case eMainFuelKm:   ObjTextShow( &FuelDistTxtObj );     break;
                case eMainTrip1Km:  ObjTextShow( &Trip1DistTxtObj );    break;
                case eMainTrip2Km:  ObjTextShow( &Trip2DistTxtObj );    break;
                case eMainVehKm:    ObjTextShow( &VehDistTxtObj );      break;
                case eMainSpeedMax: ObjTextShow( &SpeedMaxTxtObj );     break;
                case eMainMonitor:
                {
                    /* Markus Monitor-Part-in-Main-Devíce-Mode :-) */

                    // which icon has to be used: Hazard or Info?
                    ObjBmpShow( SurvGetNOKStates() ? &HazardSymbolBmpObj : &InfoSymbolBmpObj );

                    // show external air temp (if n.a.: internal device temp)
                    ObjTextShow( &TempTxtObj );

                    // show battery supply voltage
                    ObjTextShow( &VoltageTxtObj );

                    // Automatic Display Switch between WaterTemp / FuelDistance
                    // TBD: Will later be done via Settings, but at the moment we
                    //      use this so called 'Arnoldschen Elfenbeinturm' here! ;-)
                    // Water temp available?
                    if ( AnaInGetWatTemperature() > ANAIN_TEMP_SENSORDETECT )
                    {
                        /* might have been changed! */
                        ObjBmpShow( &WaterTempSymbolBmpObj );
                        ObjTextShow( &WaterTempTxtObj );
                        ObjTextShow( &WaterTempDescTxtObj );
                    }
                    else
                    {
                        /* might have been changed! */
                        ObjTextShow( &FuelDist2TxtObj );
                        ObjBmpShow( &Fuel2SymbolBmpObj );
                    }

                    // Automatic Display Switch between OilTemp / RPM
                    // TBD: Will later be done via Settings, but at the moment we
                    //      use this so called 'Arnoldschen Elfenbeinturm' here! ;-)
                    // Oil temp available?
                    if ( AnaInGetOilTemperature() > ANAIN_TEMP_SENSORDETECT )
                    {
                        /* might have been changed! */
                        ObjBmpShow( &OilTempSymbolBmpObj );
                        ObjTextShow( &OilTempTxtObj );
                        ObjTextShow( &OilTempDescTxtObj );
                    }
                    else
                    {
                        /* might have been changed! */
                        ObjBmpShow( &RPMSymbolBmpObj );
                        ObjTextShow( &RPM2TxtObj );
                    }

                } break;
                // case eMainClock:
                //      Clock & Date are always visible and therefore independent of state machine
                // break;
                default:
                    ODS1( DBG_SYS, DBG_ERROR,
                          "MainDeviceShow(): unknown state: %u", MainDevice.wDevState);
            }
        }
    }
    else
    {
        DisplClearScreen(0x0);              /* leave a clear screen for next device */
        MainDevice.fScreenInit = FALSE;     /* reset devices screen state */
    }
}


/***********************************************************************
 *  FUNCTION:       MainDeviceMsgEntry
 *  DESCRIPTION:    Receive Message Handler called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MainDeviceMsgEntry(MESSAGE GivenMsg)
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
            if ( MainDevice.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_MAIN]);
                MSG_BUILD_SETFOCUS(NewMsg,DEVID_MAIN,MSG_SENDER_ID(GivenMsg));   /* build answer message */
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                     /* send answer message */
                MainDevice.fFocused = FALSE;                                     /* clear our focus */
                MainDeviceShow(FALSE);                                           /* clear our screen */
                RValue = ERR_MSG_PROCESSED;
            }
        } break;
        case MSG_SET_FOCUS:
        {
            /* messsage for us? */
            if (MSG_RECEIVER_ID(GivenMsg) == DEVID_MAIN)
            {
                /*  Someone wants us to take the focus?
                    We assume, that nobody else has the focus
                    and we've got the the screen now for us! */
                if (  (MainDevice.fFocused       == TRUE             )
                    &&(MSG_RECEIVER_ID(GivenMsg) == DEVID_MAIN) )
                    ODS2(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but it already has focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_MAIN]);
                else /*
                    ODS2(   DBG_SYS, DBG_INFO,
                            "FOCUS: %s -> %s!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_MAIN]) */;
                MainDevice.fFocused = TRUE;                             /* set our focus */
                MainDeviceShow(TRUE);                                   /* show our screen */
                gSystemFlags.flags.ActDevNr = DEVID_MAIN;               /* save device# for restore */
                ShowVehicleState = 0;                                   /* reset lower VehicleStateString state */
                RValue = ERR_MSG_PROCESSED;
             }
             else
             {
                if ( MainDevice.fFocused == TRUE )
                {
                    /*  Some other device should be given the focus,
                        BUT WE'VE GOT THE FOCUS!
                        It should first ask US to give the focus! :-( */
                    ODS3(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but %s had focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[MSG_RECEIVER_ID(GivenMsg)],
                            szDevName[DEVID_MAIN]);
                    MainDevice.fFocused = FALSE;                        /* loose our focus */
                    ODS1(   DBG_SYS, DBG_WARNING,
                            "%s now loosing focus :-( ",
                            szDevName[DEVID_MAIN]);
                    RValue = ERR_MSG_NOT_PROCESSED;
                }
             }
        } break;
    }

    /* this part is only for FOCUSED time
       AND msg not already processed */
    if (  (MainDevice.fFocused == TRUE )
        &&(RValue == ERR_MSG_NOT_PROCESSED ) )
    {
        switch (MsgId)
        {
            case MSG_KEY_UP:
            case MSG_KEY_DOWN:
            case MSG_KEY_OK:
            case MSG_DPL_FLASH_ON:
            case MSG_DPL_FLASH_OFF:
            case MSG_KEYS_PRESSED:
                /* Try to show vehicle state instead of Time/Date ([OK]-short) */
                /* THIS MESSAGE HANLDER FIRST! (forwards handled messages) */
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = MainDeviceShowVehicStateMsg(GivenMsg);

                /* Try all distance objects, wether to be reseted or not ([OK]-long) */
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = MainDeviceResetMsg(GivenMsg);

                /* Try to toggle lower screen part to next information line ([Up/Down]-short) */
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = MainDeviceStateMachine(GivenMsg);

                /* Try to switch to next device ([Up+Down]-short) */
                if (  (RValue == ERR_MSG_NOT_PROCESSED                    )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN))
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYSHORT              ) )
                {
                    /* give focus immediatly to next screen */
                    MainDevice.fFocused = FALSE;                              /* clear our focus */
                    MainDeviceShow(FALSE);                                    /* clear our screen */
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_MAIN, DEVID_TRIPCOUNT);  /* next: TripCounter */
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_SCREEN_REFRESH:
                MainDeviceShow(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;

            /* special messages ------------ */

            /* special handling of vehicle state string in lowest display line */
            case MSG_MAINDEV_VEHIC_STATE_SHOW:  // no break!
            case MSG_MAINDEV_VEHIC_STATE_HIDE:
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = MainDeviceShowVehicStateMsg(GivenMsg);
                break;

            /* trigger time / date screen update only */
            case MSG_TIMEDATE_SECOND_GONE:
                MainDeviceUpdateTimeDate();
                RValue = ERR_MSG_PROCESSED;
                break;

            default: RValue = ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       MainDeviceStateMachine
 *  DESCRIPTION:    Handle the display states of main device
 *                  and re-init the display state if any changes
 *  PARAMETER:      MESSAGE
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MainDeviceStateMachine(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                            /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;

    /* scroll up? */
    if (  (RValue == ERR_MSG_NOT_PROCESSED             )        /* still unpocessed */
        &&(MsgId  == MSG_KEY_UP                        )        /* [UP] */
        &&(MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED ) )      /* not released */
    {
        MainDevice.wDevState--;                                 /* previous state */
        if (MainDevice.wDevState == eMainFirst)                 /* wrap around? */
            MainDevice.wDevState = (eMainLast-1);
        MainDevice.fScreenInit   = FALSE;                       /* next time rebuild complete screen */
        MainDeviceShow(TRUE);                                   /* rebuild screen right now */
        RValue = ERR_MSG_PROCESSED;
        ODS1( DBG_SYS, DBG_INFO, "MainDevState: %u", MainDevice.wDevState);
    }

    /* scroll down? */
    if (  (RValue == ERR_MSG_NOT_PROCESSED             )        /* still unpocessed */
        &&(MsgId  == MSG_KEY_DOWN                      )        /* [DOWN] */
        &&(MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED ) )      /* not released */
    {
        MainDevice.wDevState++;                                 /* next state */
        if (MainDevice.wDevState == eMainLast)                  /* wrap around? */
            MainDevice.wDevState = (eMainFirst+1);
        MainDevice.fScreenInit   = FALSE;                       /* next time rebuild complete screen */
        MainDeviceShow(TRUE);                                   /* rebuild screen right now */
        RValue = ERR_MSG_PROCESSED;
        ODS1( DBG_SYS, DBG_INFO, "MainDevState: %u", MainDevice.wDevState);
    }

    /* save device state */
    gSystemFlags.flags.MainDevState = MainDevice.wDevState;

    return RValue;
}



/***********************************************************************
 *  FUNCTION:       MainDeviceResetMsg
 *  DESCRIPTION:    Handle the 'distance reset' messages for fuel,
 *                  trip1 and trip2 distance
 *  PARAMETER:      MESSAGE
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MainDeviceResetMsg(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    static BOOL fLocked = FALSE;                    /* TRUE = key not yet relases */

    // check: main device in state to reset anything?
    if (   ( MainDevice.wDevState == eMainFuelKm  )
         ||( MainDevice.wDevState == eMainTrip1Km )
         ||( MainDevice.wDevState == eMainTrip2Km )
         ||( MainDevice.wDevState == eMainSpeedMax) )
    {
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
            DIST_TYPE Dist;
            Dist.dkm = 0;               /* reset distance value */
            switch (MainDevice.wDevState) /* set into memory */
            {
                case eMainFuelKm:   MeasSetFuelDist( &Dist );  break;
                case eMainTrip1Km:  MeasSetTripCnt( eTRIPC_C, &Dist ); break;
                case eMainTrip2Km:  MeasSetTripCnt( eTRIPC_D, &Dist ); break;
                case eMainSpeedMax: Speed_Max = 0; break;
                default: break;
            }
            BeepOk();                   /* beep ok */
            LEDOk();                    /* LED ok */
            fLocked = TRUE;             /* don't repeat this until key released */
            RValue = ERR_MSG_PROCESSED; /* done */
        }
    }
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       MainDeviceShowVehicStateMsg
 *  DESCRIPTION:    Handle messages for the monitor sub screen, makes
 *                  lower screen part showing 'vehicle state' instead of
 *                  'Time+Date' for a defined period of time.
 *  PARAMETER:      MESSAGE
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        This function tries to detect a short-pressed-OK-key!
 *                  This is done via detection of pressed- and relased
 *                  states. Only if the OK key is released after short
 *                  time, it is detected.
 *                  Reaction is, that two messages are send to our
 *                  Maindevice: One to SHOW the vehicle state  right now,
 *                  and one to HIDE the vehicle state (eq. show Time+Date)
 *                  after n seconds (via SetTimerMsg() ).
 *********************************************************************** */
ERRCODE MainDeviceShowVehicStateMsg(MESSAGE Msg)
{
    MESSAGE_ID  MsgId   = MSG_ID(Msg);              /* get message id */
    ERRCODE     RValue  = ERR_MSG_NOT_PROCESSED;    /* return value */
    MESSAGE     NewMsg;                             /* to generate show/hide messages */

    /* [OK] pressed+released shortly'? -> ITS FOR US! -> show 'Vehicle State' instead of TimeDate! */
    if (  ( MsgId == MSG_KEY_OK                          )      /* OK key?  */
        &&( MSG_KEY_TRANSITION(Msg) == KEYTRANS_RELEASED )      /* right now released* */
        &&( MSG_KEY_DURATION(Msg) < KEYSAVE              ) )    /* has just shortly been pressed? */
    {
        /* send message to our self to SHOW vehicle state */
        MSG_BUILD_UINT8(NewMsg, MSG_MAINDEV_VEHIC_STATE_SHOW, 0, 0, 0);
        MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);

        /* prepare delayed message to our self to HIDE vehicle state after 3 seconds */
        MSG_BUILD_UINT8(NewMsg, MSG_MAINDEV_VEHIC_STATE_HIDE, 0, 0, 0);
        SetTimerMsg(NewMsg, SEC2TICKS(3));          /* send delayed message */
        ODS( DBG_SYS, DBG_INFO, "MainDev: [OK] short pressed!");
        RValue = ERR_MSG_PROCESSED;                 /* processed! */
    }

    /* Received special Message to SHOW Vehicle state? */
    else if ( MsgId == MSG_MAINDEV_VEHIC_STATE_SHOW )
    {
        ShowVehicleState = 1;                       /* change view right now! */
        MainDeviceShow(TRUE);                       /* don't wait until REFRESH, show right now! */
        ODS( DBG_SYS, DBG_INFO, "MainDev: SHOW VehicleState!");
        RValue = ERR_MSG_PROCESSED;                 /* processed! */
    }

    /* Received special Message to HIDE Vehicle state? */
    else if ( MsgId == MSG_MAINDEV_VEHIC_STATE_HIDE )
    {
        ShowVehicleState = 0;                       /* change view right now! */
        ODS( DBG_SYS, DBG_INFO, "MainDev: HIDE VehicleState!");
        RValue = ERR_MSG_PROCESSED;                 /* processed! */
    }
    else RValue = ERR_MSG_NOT_PROCESSED;

    return(RValue);
}



/***********************************************************************
 *  FUNCTION:       MainDeviceUpdateTimeDate
 *  DESCRIPTION:    Separate handling of TimeDate screen refreshs
 *                  to synchronize display to RTC seconds
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MainDeviceUpdateTimeDate(void)
{
    CHAR    szBuffer[15];                           // text buffer

    // check conditions to display timedate */
    if (  ( MainDevice.fScreenInit == TRUE  )       // screen is ready?
        &&( ShowVehicleState       == FALSE ) )     // no vehicle state string active?
    {
        /* generate complete date/time string (21 chars!):
            DE: 'Mo 01.01.01  00:00:00'
            EN: 'Mo 01.01.01 00:00:00a' */
        TimeDate_GetString( RESENUM_WWDDMMYY,  szBuffer );
        strcpy(szTimeDate, szBuffer);
        strcat(szTimeDate, " ");
        TimeDate_GetString( RESENUM_HHMMSS,  szBuffer );  // returns 7! chars!!!
        strcat(szTimeDate, szBuffer);
        ObjTextShow( &TimeDateTxtObj );
    }
}

