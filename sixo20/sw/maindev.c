/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-22 by Ralf Schwarzer
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
 * Revision 3.42  2012/06/01 21:46:31  tuberkel
 * Fuel: Layout prepared for 'l/100km' or 'l/Min'
 *
 * Revision 3.41  2012/06/01 20:20:26  tuberkel
 * BugFix FuelAverage - Post-Comma-Calculation
 *
 * Revision 3.40  2012/06/01 20:13:30  tuberkel
 * Fuel: Average Consumption updated
 * - calculation & display based on FuelSensor ok!
 * - reset enabled for FuelCons too
 *
 * Revision 3.39  2012/05/31 20:50:27  tuberkel
 * - MainDev_UpdMeas_Fuel() completely reviewed
 * - Display of To/From Fuelstation now ok (based on user given value)
 * - FuelSensor-Data not yet used!
 *
 * Revision 3.38  2012/05/27 17:52:40  tuberkel
 * Corrections for renamed Eeprom/Nvram Variables
 *
 * Revision 3.37  2012/05/27 16:01:40  tuberkel
 * All Eeprom/Nvram Variables renamed
 *
 * Revision 3.36  2012/05/25 22:38:30  tuberkel
 * small fixes
 *
 * Revision 3.35  2012/05/25 22:29:59  tuberkel
 * simplified Monitor descriptor usage
 *
 * Revision 3.34  2012/05/25 20:46:01  tuberkel
 * Intermediate Version:
 * - Fuel-Consumption reviewed
 * - still Dummy Only
 *
 * Revision 3.33  2012/05/25 20:29:22  tuberkel
 * Intermediate Version:
 * - 3 new Fuel-Display-mode
 * - Dummy Only
 *
 * Revision 3.32  2012/05/25 11:06:23  tuberkel
 * Intermediate Version:
 * - supporting 3 new Fuel-Display-mode
 * - Dummy Only
 *
 * Revision 3.31  2012/05/24 21:14:29  tuberkel
 * - Preparations for 3 new Fuel / Distance / Consumption Display-modes
 * - not yet ready
 *
 * Revision 3.30  2012/05/24 20:00:14  tuberkel
 * BMP renamed
 *
 * Revision 3.29  2012/05/16 21:06:57  tuberkel
 * New FuelSensor:
 * - now displayed & handled in Maindevice
 * - if enabled: replaces 'NV_FuelDistance'
 * - works well with GPI-0..3 / NVRAM / Reboot / ImpRate = 0
 *
 * Revision 3.28  2012/02/27 21:44:16  tuberkel
 * Coolride Heatgrip: Only shon if enabled via eeprom setting
 *
 * Revision 3.27  2012/02/27 20:46:50  tuberkel
 * - all Coolride GPIs/GPOs correctly set by Eeprom value
 *
 * Revision 3.26  2012/02/21 20:58:15  tuberkel
 * all Setdevice ObjectNames reviewed
 *
 * Revision 3.25  2012/02/15 07:32:43  tuberkel
 * Objects-API reviewed (no functional changes)
 *
 * Revision 3.24  2012/02/11 12:21:45  tuberkel
 * dedicated COOLRIDE macros prepared & used
 *
 * Revision 3.23  2012/02/11 09:15:26  tuberkel
 * Coolride HeatGrip now usable
 * - icon shon in right way/time
 * - Key/LED Handling ok
 *
 * Revision 3.22  2012/02/11 00:06:42  tuberkel
 * - HeatGrip Icon shown in upper area
 *
 * Revision 3.21  2012/02/10 23:45:22  tuberkel
 * - Survelannce HeatGrip <Info> - if active
 * - Surveillance-API reviewed
 *
 * Revision 3.20  2012/02/10 22:12:27  tuberkel
 * PWM-Measurement now fully functional
 *
 * Revision 3.19  2012/02/08 23:05:47  tuberkel
 * - GPI PWM calculations improved
 * - renamed DigIn GPI Functions
 * - renamed DigIn Key Functions
 *
 * Revision 3.18  2012/02/08 04:52:34  tuberkel
 * Real HeatGrip measurement activated (but not yet bugfree)
 *
 * Revision 3.17  2012/02/08 03:55:24  tuberkel
 * KEY_TIMING parameter names reviewed/changed
 *
 * Revision 3.16  2012/02/07 17:32:31  tuberkel
 * HeatGrip PWM test animated
 *
 * Revision 3.15  2012/02/07 17:11:16  tuberkel
 * HeatGrip PWM display test now ok
 *
 * Revision 3.14  2012/02/06 21:40:00  tuberkel
 * HeatGrip-Part: Now shows 5 bar-parts
 *
 * Revision 3.13  2012/02/06 21:09:56  tuberkel
 * void MainDev_Show(BOOL fShow)
 * - devided into special subfunctions for Monitor-Part and HeatGrip-Part
 *
 * Revision 3.12  2012/02/06 20:54:14  tuberkel
 * Just renamed all 'Devices' function prefixes for better readability
 *
 * Revision 3.11  2012/02/05 21:08:04  tuberkel
 * New: HeatGrip internal state
 *
 * Revision 3.10  2012/02/05 11:58:01  tuberkel
 * Coolride Makros adapted
 *
 * Revision 3.9  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 3.8  2012/02/04 21:49:42  tuberkel
 * All BeeperDriver functions mapped ==> DigOutDrv()
 *
 * Revision 3.7  2012/02/04 20:38:05  tuberkel
 * Moved all BeeperDriver / LEDDriver stuff ==> 'digoutdrv'
 *
 * Revision 3.6  2012/02/04 08:36:32  tuberkel
 * - MainDev_MsgEntry_VehState() ==> TEST-ACTIVATE PIN_GPO0 for 250 ms for Coolride
 * - Some internal functions renamed
 *
 * Revision 3.5  2012/01/23 04:03:17  tuberkel
 * BugFix Symbol "SystemError"
 *
 * Revision 3.4  2012/01/21 00:14:29  tuberkel
 * - Gearbox & System-Icon moved from Monitor-Part to All-Part
 *
 * Revision 3.3  2012/01/16 05:10:35  tuberkel
 * New: Vertical divider line for Maindevice:Measurement columns
 *
 * Revision 3.2  2012/01/14 08:28:42  tuberkel
 * Message-IDs shortened / reviewed
 *
 * Revision 3.1  2011/05/29 20:57:19  tuberkel
 * - Error/Warning/Info Bitmap Objects simplified
 * - Bitmap bmpEmpty_16x16 to clear Error/Warning/Info icon
 * - Monitor Infos arranged & extended
 * - Gearbox Symbol removable
 *
 * Revision 3.0  2010/11/07 08:58:07  tuberkel
 * V30 Preparations:
 * - Device/Object Handling completely revised & simplified
 * - Device-Substate introduced
 * - Display Appearance reviewed (Gearbox Display, Icons)
 *
 * Revision 2.7  2009/07/19 12:31:03  tuberkel
 * - ObjectInit reviewed
 *
 * Revision 2.6  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
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
#include "digoutdr.h"
#include "beep.h"
#include "led.h"
#include "gpo.h"
#include "timedate.h"
#include "anain.h"
#include "surveill.h"



/* defines for device data */
#define SPEED_STRSIZE   6               /* to contain max. string '333,3' km/h */
#define RPM_STRSIZE     6               /* to contain max. string '13500' U/Min */



/* ============================================================= */
/* Main Device objects */

/* device appearance can be changed via up/down keys */
typedef enum
{
    MD_FIRST,       /* invalid state*/

    MD_MONITOR,     /* monitor sub screen (tempereatures, voltage...) */
    MD_RPM,         /* show engine speed RPM */
    MD_FUEL_FROM,   /* show fuel distance/liter since last refuel */
    MD_FUEL_TO,     /* show fuel distance/liter until next refuel */
    MD_FUEL_CONS,   /* show fuel consumption (only if fuel sensor available) */
    MD_TRIP1,       /* show TripCounter1 distance */
    MD_TRIP2,       /* show TripCounter2 distance */
    MD_VEHDIST,     /* show vehicle overall distance */
    MD_SPEEDMAX,    /* show Speed Max value */
    MD_HEATGRIP,    /* show Coolride HeatGrip with bar */

    MD_LAST,        /* invalid state*/
} MAINDEV_STATE;


// main device control structure (to handle ALL screens)
typedef struct
{   MAINDEV_STATE   eState;         // internal show state
    DEVOBJLIST      List[MD_LAST];  // array of all list control structures
} MAINDEV_CNTRL;

static DEVDATA          MDObj;      // this device
static MAINDEV_CNTRL    MDCntrl;    // special device show state control



/* ============================================================= */
/* external symbols */
extern UINT16               wMilliSecCounter;                   /* valid values: 0h .. ffffh */
extern UINT16               wSecCounter;                        /* valid values: 0h .. ffffh */
extern STRING far           szDevName[];                        /* device names */
extern DEVFLAGS1_TYPE       EE_DevFlags_1;                      /* system parameters */
extern char                 szSurvGlobalState[VEHSTATE_TXT_LEN];/* vehicle state string */
extern COOLRIDECNTRL_TYPE   EE_CoolrideCtrl;                    /* Coolride Control (from eeprom) */
extern FUELSCNTRL_TYPE      EE_FuelSensCtrl;                    /* FuelSensor Control flags (from eeprom) */
extern UINT8                EE_FuelConsUser;                    /* FuelConsumption in 1/10 l/100 km (from eeprom) */
extern UINT16               EE_FuelCap;                         /* FuelCapacity in 1/10 liters (from eeprom) */
extern UINT32               NV_FuelSensImp;                     /* Fuel sensor Impulses counter since last refuel (NVRAM!) */


/* ============================================================= */
/* external Bitmap Raw data */


/* bitmaps for main selection lower area */
extern unsigned char far bmpEnduro_29x16[];     /* enduro symbol  */
extern unsigned char far bmpRPM_16x16[];        /* big RPM symbol */

/* bitmaps for info/warning/error display */
extern unsigned char far bmpInfo_16x16[];       /* INFO symbol */
extern unsigned char far bmpWarning_16x16[];    /* WARNING symbol */
extern unsigned char far bmpError_16x16[];      /* ERROR symbol */
extern unsigned char far bmpEmpty_16x16[];      /* EMPTY symbol (just to clear) */

/* bitmaps for monitor/measuremnt data  */
extern unsigned char far bmpOil_8x8[];          /* oil symbol */
extern unsigned char far bmpWater_8x8[];        /* water symbol */
extern unsigned char far bmpTemp_8x8[];         /* temperature symbol */
extern unsigned char far bmpBattery_8x8[];      /* battery symbol */
extern unsigned char far rgClockSymbol8x8[];    /* clock symbol */
extern unsigned char far bmpRPM_8x8[];          /* RPM symbol samll */
extern unsigned char far bmpFuel_8x8[];         /* fuel symbol small */
extern unsigned char far bmpVertLine_2x20[];    /* vertical line to devide measurements */

/* bitmaps for Coolride heat control */
extern unsigned char far bmpHeatGrip_16x16[];       /* heatgrip icon */
extern unsigned char far bmpHeatBarEmpty_19x8[];    /* heat bargraph - off */
extern unsigned char far bmpHeatBarFull_19x8[];     /* heat bargraph - on */

/* bitmaps for Fuel & Consumption */
extern unsigned char far bmpFuel_16x16[];           /* icon 'fuel' */
extern unsigned char far bmpLiterPer100_16x16[];    /* icon 'l/100' */
extern unsigned char far bmpLiterPerMin_16x16[];    /* icon 'l/Min' */
extern unsigned char far bmpAverage_16x16[];        /* icon 'ø' */

/* bitmaps for gear display  */
extern unsigned char far bmp7Seg_0_16x16[];     /* 7 segment bitmaps for gear display */
extern unsigned char far bmp7Seg_1_16x16[];
extern unsigned char far bmp7Seg_2_16x16[];
extern unsigned char far bmp7Seg_3_16x16[];
extern unsigned char far bmp7Seg_4_16x16[];
extern unsigned char far bmp7Seg_5_16x16[];
extern unsigned char far bmp7Seg_6_16x16[];
extern unsigned char far bmp7Seg_7_16x16[];
extern unsigned char far bmp7Seg_8_16x16[];
extern unsigned char far bmp7Seg_9_16x16[];




/* =================================================== */
/* upper area: speed display */
/* =================================================== */

static OBJ_STEXT   SpeedTxtObj;                /* speed text '000' object */
static OBJ_STEXT   SpeedDescATxtObj;           /* speed descriptor 'km' (or 'mi' for EN) text object */
static OBJ_STEXT   SpeedDescBTxtObj;           /* speed descriptor 'h' text object */
static CHAR        szSpeed[4] = "0";           /* buffer for current speed, max. string '999' */


/* =================================================== */
/* lower area mode  */
/* =================================================== */


/* ------------------------------------ */
/* lower area mode 1: Rounds Per Minute */
static OBJ_STEXT    RPMTxtObj;                  /* rpm text '00000' object */
static OBJ_STEXT    RPMDescTxtObj;              /* rpm descriptor 'U/Min' (or RPM for EN) text object */
static CHAR         szRPM[6] = "0";             /* buffer current eng speed, max. string '13500' */
static OBJ_BMP      RPMBmpObj;                  /* symbol to indicate RPM */

/* ------------------------------------ */
/* lower area mode 2+3+4 common: 'Distance & Liters from/to last/next refueling' */
static OBJ_BMP      FuelBmpObj;                 /* bitmap 'fuel station'  */
static OBJ_STEXT    FuelArrowRTxtObj;           /* Arror-Right to indicate 'to'/'from' refueling  */
static OBJ_STEXT    FuelDistTxtObj;             /* fuel distance text object, e.g. '849km' */
static OBJ_STEXT    FuelLiterTxtObj;            /* fuel liter text object, e.g. '19,2L' */
static CHAR         szFuelDist[10] = "   0km";  /* buffer to contain distance, max. '9999km' */
static CHAR         szFuelLiter[10]=  " 0,0l";  /* buffer to contain fuel,     max. '99,9l' l */
#define LITER2ML    1000L                       /* helper to convert ml <=> 1/1  Liter */
#define DL2ML       100L                        /* helper to convert ml <=> 1/10 Liter */

/* ------------------------------------ */
/* lower area mode 4: Fuel Consumption (only if Fuel Sensor available) */
static OBJ_BMP      ActLiterPerXBmpObj;         /* bitmap for actuel  'l/100' or 'l/Min' */
static OBJ_BMP      AvrLiterPer100BmpObj;       /* bitmap for average 'l/100' */
static OBJ_STEXT    FuelConsActTxtObj;          /* fuel consumption (actual) text object */
static OBJ_STEXT    FuelConsAvrTxtObj;          /* fuel consumption (average) text object */
static CHAR         szFuelConsAct[10] = "--,-"; /* buffer to contain fuel consumption (actual)  max. '99,9' l/100 */
static CHAR         szFuelConsAvr[10] = "--,-"; /* buffer to contain fuel consumption (average) max. '99,9' l/100 */

/* ------------------------------------ */
/* lower area mode 5: Vehicle Distance */
static OBJ_STEXT    DistDescTxtObj;             /* COMMON vehicle & fuel distance decriptor for 'km' or 'mi' */
static OBJ_STEXT    VehDistTxtObj;              /* vehicle distance text object */
static CHAR         szVehDist[10] = "0,0";      /* buffer to contain fuel distance, max. string '9999999,9' */
static OBJ_BMP      VehDistBmpObj;              /* symbol to indicate vehicle distance display mode */

/* ------------------------------------ */
/* lower area mode 6: TripCounter 1 Distance */
static OBJ_STEXT    Trip1DescTxtObj;            /* tripcounter1 descriptor text object 'T1' */
static OBJ_STEXT    Trip1DistTxtObj;            /* tripcounter1 distance text object */
static CHAR         szTrip1Dist[10] = "0,0";    /* buffer to contain tripcounter1 distance, max. string '9999,9' */

/* ------------------------------------ */
/* lower area mode 7: TripCounter 2 Distance */
static OBJ_STEXT    Trip2DescTxtObj;            /* tripcounter1 descriptor text object 'T2' */
static OBJ_STEXT    Trip2DistTxtObj;            /* tripcounter2 distance text object */
static CHAR         szTrip2Dist[10] = "0,0";    /* buffer to contain tripcounter2 distance, max. string '9999,9' */

/* ------------------------------------ */
/* lower area mode 8: Max Speed */
static OBJ_STEXT    SpeedMaxDescTxtObj;         /* SpeedMax descriptor text object for 'v(max)' */
static OBJ_STEXT    SpeedMaxUnitTxtObj;         /* speed max desciptor text object 'km/h' or 'mi/h' */
static OBJ_STEXT    SpeedMaxTxtObj;             /* SpeedMax descriptor text object for '110.0' */
static CHAR         szSpeedMax[4] = "  0";      /* buffer to contain SpeedMax, max. string '999' km/h*/
extern SPEED_TYPE   EE_SpeedMax;                  /* prepared value */

/* ------------------------------------ */
/* lower area mode 7: Coolride Heat Control */
static OBJ_BMP      HeatGripIconBmpObj;         /* symbol to indicate heat at handgrip */
static OBJ_BMP      HeatBarBmpObj;              /* empty/full bar icon - for multiple use */
#define MD_HEATBARPARTS 5                       /* number of bargraph parts */

/* ------------------------------------ */
/* lower area: Date & Time Display */
static OBJ_STEXT    TimeDateTxtObj;             /* time & date output opbject */
static CHAR         szTimeDate[22] = "Mo 01.01.01  00:00:00";   /* buffer for timedate string */


/* --------------------------------------------- */
/* upper area bitmap objects & vehicle state */
static OBJ_BMP      VehStateBmpObj;             /* symbol for current vehicle state */


/* --------------------------------------------- */
/* lower area vehicle state objects */
static OBJ_STEXT    SurvVehStateTxtObj;         /* Vehicle State detected by MonDev_ */
extern INT8         SurvParamListCount;         /* number of states currently not ok */
static INT8         SurvShowVehState = 0;       /* != 0 if vehicle state is to be displayed */


/* --------------------------------------------- */
/* upper area gearbox state */
#if(GEARBOX==1)
static OBJ_BMP      GearSymbolBmpObj;           /* selected gear indicator */
#endif




/* =================================================== */
/* lower area mode 7: Monitor/measurement objects */
/* =================================================== */

/* measure data vertical devider */
static OBJ_BMP      MonVertLineBmpObj;

/* measure data 1: temperature from external air / internal NTC sensor */
static OBJ_BMP      MonAmbientTempBmpObj;
static OBJ_STEXT    MonAmbientTempTxtObj;
static CHAR         szMonAmbientTemp[8];

/* measure data 2: battery voltage */
static OBJ_BMP      MonVoltageBmpObj;
static OBJ_STEXT    MonVoltageTxtObj;
static CHAR         szMonBattVolt[8];

/* measure data 3: motor oil temperature */
static OBJ_BMP      MonOilTempBmpObj;
static OBJ_STEXT    MonOilTempTxtObj;
static CHAR         szMonOilTemp[8];

/* measure data 4: motor RPM (used, if no oil temp sensor connected) */
static OBJ_BMP      MonRPMBmpObj;
static OBJ_STEXT    MonRPMTxtObj;
static CHAR         szMonRPM[8];             

/* measure data 5: water temperature */
static OBJ_BMP      MonWaterTempBmpObj;
static OBJ_STEXT    MonWaterTempTxtObj;
static CHAR         szMonWaterTemp[8];

/* measure data 5: fuel distance (used, if water temp sensor not available) */
static OBJ_BMP      MonFuelDistBmpObj;
static OBJ_STEXT    MonFuelDistTxtObj;
static CHAR         szMonFuelDist[8];



/* ============================================================= */
/* internal function prototypes */
void    MainDev_Show_Monitor            (BOOL fShow);
void    MainDev_Show_Heatgrip           (BOOL fShow);
ERRCODE MainDev_MsgEntry_StateMachine   (MESSAGE Msg);
ERRCODE MainDev_MsgEntry_VehDistRst     (MESSAGE Msg);
ERRCODE MainDev_MsgEntry_VehState       (MESSAGE Msg);
void    MainDev_UpdTimeDate             (void);
void    MainDev_UpdMeas              (void);
void    MainDev_UpdMeas_Mon              (void);
void    MainDev_UpdMeas_Fuel                 (void);
void    MainDev_ObjListInit             (void);
void    MainDev_ShowHorLine             (void);
void    MainDev_Show_VehStateIcon               (void);



/* ============================================================= */
/* bitmap object table of this device */
static const OBJ_BMP_INIT BmpObjInit[] =
{
    /*                              x   y  w   h   raw data              mode     state */
    /* --------------------------- -- --- --- --- --------------------- -------- ----- */

    /* selected info icons */
    /* --------------------------- -- --- --- --- --------------------- -------- ----- */
    { &VehDistBmpObj,               0, 38, 29, 16, bmpEnduro_29x16,     DPLNORM, FALSE },
    { &RPMBmpObj,                   0, 38, 16, 16, bmpRPM_16x16,        DPLNORM, FALSE },

    /* Coolride Heat Control - Symbol & Bar icons (HeatBarBmpObj gets 5x moved & changed!)*/
    /* --------------------------- -- --- --- --- --------------------- -------- ----- */
    { &HeatGripIconBmpObj,          2, 38, 16, 16, bmpHeatGrip_16x16,   DPLNORM, FALSE },
    { &HeatBarBmpObj,              24, 40, 19,  8, bmpHeatBarEmpty_19x8,DPLNORM, OC_DISPL | OC_DYN },

    /* Gear Symbol */
    /* --------------------------- -- --- --- --- --------------------- -------- ----- */
    #if(GEARBOX==1)
    { &GearSymbolBmpObj,            0,  0, 16, 16, bmp7Seg_0_16x16,     DPLNORM, FALSE },
    #endif

    /* error status icons */
    /* --------------------------- -- --- --- --- --------------------- -------- ----- */
    { &VehStateBmpObj,              1, 17, 16, 16, bmpEmpty_16x16,      DPLNORM, FALSE },

    /* monitor symbols  */
    /* --------------------------- -- --- --- --- --------------------- -------- ----- */
    { &MonAmbientTempBmpObj,        2, 37,  8,  8, bmpTemp_8x8,         DPLNORM, FALSE },
    { &MonWaterTempBmpObj,          2, 46,  8,  8, bmpWater_8x8,        DPLNORM, FALSE },
    { &MonFuelDistBmpObj,           2, 46,  8,  8, bmpFuel_8x8,         DPLNORM, FALSE },
    { &MonVoltageBmpObj,           71, 37,  8,  8, bmpBattery_8x8,      DPLNORM, FALSE },
    { &MonOilTempBmpObj,           71, 46,  8,  8, bmpOil_8x8,          DPLNORM, FALSE },
    { &MonRPMBmpObj,               71, 46,  8,  8, bmpRPM_8x8,          DPLNORM, FALSE },
    { &MonVertLineBmpObj,          64, 36,  2, 24, bmpVertLine_2x20,    DPLNORM, FALSE },

    /* fuel consumption symbols  */
    /* --------------------------- -- --- --- --- --------------------- -------- ----- */
    { &FuelBmpObj,                  0, 38, 16, 16, bmpFuel_16x16,       DPLNORM, FALSE },
    { &ActLiterPerXBmpObj,         52, 38, 16, 16, bmpLiterPer100_16x16,DPLNORM, FALSE },
    { &AvrLiterPer100BmpObj,      114, 38, 16, 16, bmpLiterPer100_16x16,DPLNORM, FALSE },

    /* --------------------------- -- --- --- --- --------------------- -------- ----- */
};
#define BMPOBJECTLISTSIZE   (sizeof(BmpObjInit)/sizeof(OBJ_BMP_INIT))


/* ============================================================= */
/* text object table */
static const OBJ_STEXT_INIT TextObjInit[] =
{
    /* UPPER BIG PART: Always show Vehicle Speed + Unit */
    /* pObject                  X    Y  Font            H  Width  Align     Format    string ptr            State      */
    /* ----------------------- ---- --- -------------- --- ----- --------- ---------- -----------------     ---------- */
    { &SpeedTxtObj,             28,  0, DPLFONT_24X32,  1,  3, TXT_RIGHT,  TXT_NORM, szSpeed,               OC_DISPL | OC_DYN   },
    { &SpeedDescATxtObj,       108,  1, DPLFONT_8X16,   1,  2, TXT_LEFT,   TXT_NORM | TXT_UNDERL, RESTXT_SPEEDA_DESC,  OC_DISPL },
    { &SpeedDescBTxtObj,       112, 17, DPLFONT_8X16,   1,  1, TXT_LEFT,   TXT_NORM, RESTXT_SPEEDB_DESC,    OC_DISPL            },

    /* LOWER MEDIUM PART: Selectable Information */
    /* pObject                  X    Y  Font            H  Width  Align     Format    string ptr            State      */
    /* ----------------------- ---- --- -------------- --- ----- --------- ---------- -----------------     ---------- */
    { &RPMTxtObj,               22, 38, DPLFONT_14X16,  1,  5, TXT_RIGHT,  TXT_NORM, szRPM,                 OC_DISPL | OC_DYN   },
    { &RPMDescTxtObj,           96, 38, DPLFONT_8X16,   1,  5, TXT_LEFT,   TXT_NORM, RESTXT_RPM_DESC,       OC_DISPL            },
    { &DistDescTxtObj,         106, 38, DPLFONT_8X16,   1,  2, TXT_LEFT,   TXT_NORM, RESTXT_DIST_DESC,      OC_DISPL            },
    { &VehDistTxtObj,           46, 38, DPLFONT_8X16,   1,  6, TXT_RIGHT,  TXT_NORM, szVehDist,             OC_DISPL | OC_DYN   },
    { &Trip1DescTxtObj,          4, 38, DPLFONT_8X16,   1,  2, TXT_LEFT,   TXT_NORM, RESTXT_TRIP1DESC,      OC_DISPL            },
    { &Trip1DistTxtObj,         24, 38, DPLFONT_14X16,  1,  6, TXT_RIGHT,  TXT_NORM, szTrip1Dist,           OC_DISPL | OC_DYN   },
    { &Trip2DescTxtObj,          4, 38, DPLFONT_8X16,   1,  2, TXT_LEFT,   TXT_NORM, RESTXT_TRIP2DESC,      OC_DISPL            },
    { &Trip2DistTxtObj,         24, 38, DPLFONT_14X16,  1,  6, TXT_RIGHT,  TXT_NORM, szTrip2Dist,           OC_DISPL | OC_DYN   },
    { &SpeedMaxDescTxtObj,       4, 38, DPLFONT_8X16,   1,  3, TXT_LEFT,   TXT_NORM, RESTXT_SPEEDMAX_DESC,  OC_DISPL            },
    { &SpeedMaxUnitTxtObj,      94, 38, DPLFONT_8X16,   1,  4, TXT_LEFT,   TXT_NORM, RESTXT_SPEEDMAX_UNIT,  OC_DISPL            },
    { &SpeedMaxTxtObj,          44, 38, DPLFONT_14X16,  1,  3, TXT_RIGHT,  TXT_NORM, szSpeedMax,            OC_DISPL | OC_DYN   },
    
    { &FuelArrowRTxtObj,         0, 38, DPLFONT_8X16,   1,  1, TXT_RIGHT,  TXT_NORM, RESTXT_MAIN_ARROW_R,   OC_DISPL            },   
    { &FuelDistTxtObj,          28, 38, DPLFONT_8X16,   1,  6, TXT_RIGHT,  TXT_NORM, szFuelDist,            OC_DISPL | OC_DYN   },
    { &FuelLiterTxtObj,         88, 38, DPLFONT_8X16,   1,  5, TXT_RIGHT,  TXT_NORM, szFuelLiter,           OC_DISPL | OC_DYN   },
    { &FuelConsActTxtObj,       20, 38, DPLFONT_8X16,   1,  4, TXT_RIGHT,  TXT_NORM, szFuelConsAct,         OC_DISPL | OC_DYN   },
    { &FuelConsAvrTxtObj,       74, 38, DPLFONT_8X16,   1,  5, TXT_RIGHT,  TXT_NORM, szFuelConsAvr,         OC_DISPL | OC_DYN   },
        
    /* LOWER SMALL PART: Monitor Device Information */
    /* pObject                  X    Y  Font            H  Width  Align     Format    string ptr            State      */
    /* ----------------------- ---- --- -------------- --- ----- --------- ---------- -----------------     ---------- */
    { &MonAmbientTempTxtObj,    24, 38, DPLFONT_6X8,    1,  6, TXT_RIGHT,  TXT_NORM, szMonAmbientTemp,      OC_DISPL | OC_DYN   },
    { &MonWaterTempTxtObj,      24, 47, DPLFONT_6X8,    1,  6, TXT_RIGHT,  TXT_NORM, szMonWaterTemp,        OC_DISPL | OC_DYN   },
    { &MonFuelDistTxtObj,       24, 47, DPLFONT_6X8,    1,  6, TXT_RIGHT,  TXT_NORM, szMonFuelDist,         OC_DISPL | OC_DYN   },
    { &MonVoltageTxtObj,        90, 38, DPLFONT_6X8,    1,  6, TXT_RIGHT,  TXT_NORM, szMonBattVolt,         OC_DISPL | OC_DYN   },
    { &MonOilTempTxtObj,        90, 47, DPLFONT_6X8,    1,  6, TXT_RIGHT,  TXT_NORM, szMonOilTemp,          OC_DISPL | OC_DYN   },
    { &MonRPMTxtObj,            90, 47, DPLFONT_6X8,    1,  6, TXT_RIGHT,  TXT_NORM, szMonRPM,              OC_DISPL | OC_DYN   },

    /* Time and Date OR Vehiclestate text (changed when KEY_OK is pressed) */
    /* pObject                  X    Y  Font            H  Width  Align     Format    string ptr            State      */
    /* ----------------------- ---- --- -------------- --- ----- --------- ---------- -----------------     ---------- */
    { &TimeDateTxtObj,           2, 56, DPLFONT_6X8,    1, 21, TXT_CENTER, TXT_NORM, szTimeDate,            OC_DISPL | OC_DYN   },
    { &SurvVehStateTxtObj,       2, 56, DPLFONT_6X8,    1, 21, TXT_CENTER, TXT_NORM, szSurvGlobalState,     OC_DISPL | OC_DYN   },
    /*------------------------ ---- --- -------------- --- ----- --------- ---------- -----------------     ---------           */
};
#define STEXTOBJ_INITLISTSIZE   (sizeof(TextObjInit)/sizeof(OBJ_STEXT_INIT))



// ---------------------------------------------------------------------
// SCREEN FOCUS ORDER
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// Main device State 'MD_MONITOR' Settings - Screen Focus Order
static const void far * ObjectList_Mon[] =
{
    // objects - shown in every mode
    (void far *) &SpeedTxtObj,          // current speed
    (void far *) &SpeedDescATxtObj,     // speed descriptor A
    (void far *) &SpeedDescBTxtObj,     // speed descriptor A
    #if(GEARBOX==1)
    (void far *) &GearSymbolBmpObj,     // gearbox state
    #endif
    (void far *) &VehStateBmpObj,       // Vehicle State Icon
    (void far *) &TimeDateTxtObj,       // Date & Time
    (void far *) &SurvVehStateTxtObj,   // Vehicle State

    // objects - shown in MD_MONITOR mode only
    (void far *) &MonVoltageTxtObj,
    (void far *) &MonVoltageBmpObj,
    (void far *) &MonAmbientTempTxtObj,
    (void far *) &MonAmbientTempBmpObj,
    (void far *) &MonWaterTempTxtObj,
    (void far *) &MonWaterTempBmpObj,
    (void far *) &MonFuelDistTxtObj,
    (void far *) &MonFuelDistBmpObj,
    (void far *) &MonOilTempBmpObj,
    (void far *) &MonOilTempTxtObj,
    (void far *) &MonRPMTxtObj,
    (void far *) &MonRPMBmpObj,
    (void far *) &MonVertLineBmpObj
};
#define OBJLIST_MON_CNT (sizeof(ObjectList_Mon)/sizeof(OBJ_STATE)/sizeof(void far *))


// ---------------------------------------------------------------------
// Main device State 'MD_RPM' Settings - Screen Focus Order
static const void far * ObjectList_Rpm[] =
{
    // objects - shown in every mode
    (void far *) &SpeedTxtObj,          // current speed
    (void far *) &SpeedDescATxtObj,     // speed descriptor A
    (void far *) &SpeedDescBTxtObj,     // speed descriptor A
    #if(GEARBOX==1)
    (void far *) &GearSymbolBmpObj,     // gearbox state
    #endif
    (void far *) &VehStateBmpObj,       // Vehicle State Icon
    (void far *) &TimeDateTxtObj,       // Date & Time
    (void far *) &SurvVehStateTxtObj,   // Vehicle State

    // objects - shown in MD_RPM mode only
    (void far *) &RPMTxtObj,
    (void far *) &RPMDescTxtObj,
    (void far *) &RPMBmpObj
};
#define OBJLIST_RPM_CNT (sizeof(ObjectList_Rpm)/sizeof(OBJ_STATE)/sizeof(void far *))


// ---------------------------------------------------------------------
// Main device State 'MD_FUEL_FROM' Settings - Screen Focus Order
static const void far * ObjectList_Fuel_From[] =
{
    // objects - shown in every mode
    (void far *) &SpeedTxtObj,          // current speed
    (void far *) &SpeedDescATxtObj,     // speed descriptor A
    (void far *) &SpeedDescBTxtObj,     // speed descriptor A
    #if(GEARBOX==1)
    (void far *) &GearSymbolBmpObj,     // gearbox state
    #endif
    (void far *) &VehStateBmpObj,       // Vehicle State Icon
    (void far *) &TimeDateTxtObj,       // Date & Time
    (void far *) &SurvVehStateTxtObj,   // Vehicle State

    // objects - shown in 'MD_FUEL_FROM' mode only
    (void far *) &FuelBmpObj,           // icon 'fuel'
    (void far *) &FuelArrowRTxtObj,     // arrow left/right of fuel icon
    (void far *) &FuelDistTxtObj,       // fuel km
    (void far *) &FuelLiterTxtObj,      // fuel liters
};
#define OBJLIST_FUEL_FROM_CNT (sizeof(ObjectList_Fuel_From)/sizeof(OBJ_STATE)/sizeof(void far *))

// ---------------------------------------------------------------------
// Main device State 'MD_FUEL_TO' Settings - Screen Focus Order
static const void far * ObjectList_Fuel_To[] =
{
    // objects - shown in every mode
    (void far *) &SpeedTxtObj,          // current speed
    (void far *) &SpeedDescATxtObj,     // speed descriptor A
    (void far *) &SpeedDescBTxtObj,     // speed descriptor A
    #if(GEARBOX==1)
    (void far *) &GearSymbolBmpObj,     // gearbox state
    #endif
    (void far *) &VehStateBmpObj,       // Vehicle State Icon
    (void far *) &TimeDateTxtObj,       // Date & Time
    (void far *) &SurvVehStateTxtObj,   // Vehicle State

    // objects - shown in 'MD_FUEL_TO' mode only
    (void far *) &FuelBmpObj,           // icon 'fuel'
    (void far *) &FuelArrowRTxtObj,     // arrow left/right of fuel icon   
    (void far *) &FuelDistTxtObj,       // fuel km
    (void far *) &FuelLiterTxtObj       // fuel liters
};
#define OBJLIST_FUEL_TO_CNT (sizeof(ObjectList_Fuel_To)/sizeof(OBJ_STATE)/sizeof(void far *))

// ---------------------------------------------------------------------
// Main device State 'MD_FUEL_CONS' Settings - Screen Focus Order
static const void far * ObjectList_Fuel_Cons[] =
{
    // objects - shown in every mode
    (void far *) &SpeedTxtObj,          // current speed
    (void far *) &SpeedDescATxtObj,     // speed descriptor A
    (void far *) &SpeedDescBTxtObj,     // speed descriptor A
    #if(GEARBOX==1)
    (void far *) &GearSymbolBmpObj,     // gearbox state
    #endif
    (void far *) &VehStateBmpObj,       // Vehicle State Icon
    (void far *) &TimeDateTxtObj,       // Date & Time
    (void far *) &SurvVehStateTxtObj,   // Vehicle State

    // objects - shown in 'MD_FUEL_CONS' mode only
    (void far *) &FuelBmpObj,           // icon 'fuel'
    (void far *) &ActLiterPerXBmpObj,   // icon 'l/100' or 'l/Min'
    (void far *) &AvrLiterPer100BmpObj, // icon 'l/100'
    (void far *) &FuelConsActTxtObj,    // actuel  fuel consumption in l/100
    (void far *) &FuelConsAvrTxtObj,    // average fuel consumption in l/100
};
#define OBJLIST_FUEL_CONS_CNT (sizeof(ObjectList_Fuel_Cons)/sizeof(OBJ_STATE)/sizeof(void far *))


// ---------------------------------------------------------------------
// Main device State 'MD_TRIP1' Settings - Screen Focus Order
static const void far * ObjectList_Trip1[] =
{
    // objects - shown in every mode
    (void far *) &SpeedTxtObj,          // current speed
    (void far *) &SpeedDescATxtObj,     // speed descriptor A
    (void far *) &SpeedDescBTxtObj,     // speed descriptor A
    #if(GEARBOX==1)
    (void far *) &GearSymbolBmpObj,     // gearbox state
    #endif
    (void far *) &VehStateBmpObj,       // Vehicle State Icon
    (void far *) &TimeDateTxtObj,       // Date & Time
    (void far *) &SurvVehStateTxtObj,   // Vehicle State

    // objects - shown in 'MD_TRIP1' mode only
    (void far *) &Trip1DescTxtObj,
    (void far *) &Trip1DistTxtObj,
    (void far *) &DistDescTxtObj        // common for Veh/Fuel/Trip1/Trip2

};
#define OBJLIST_TRIP1_CNT (sizeof(ObjectList_Trip1)/sizeof(OBJ_STATE)/sizeof(void far *))


// ---------------------------------------------------------------------
// Main device State 'MD_TRIP2' Settings - Screen Focus Order
static const void far * ObjectList_Trip2[] =
{
    // objects - shown in every mode
    (void far *) &SpeedTxtObj,          // current speed
    (void far *) &SpeedDescATxtObj,     // speed descriptor A
    (void far *) &SpeedDescBTxtObj,     // speed descriptor A
    #if(GEARBOX==1)
    (void far *) &GearSymbolBmpObj,     // gearbox state
    #endif
    (void far *) &VehStateBmpObj,       // Vehicle State Icon
    (void far *) &TimeDateTxtObj,       // Date & Time
    (void far *) &SurvVehStateTxtObj,   // Vehicle State

    // objects - shown in 'MD_TRIP2' mode only
    (void far *) &Trip2DescTxtObj,
    (void far *) &Trip2DistTxtObj,
    (void far *) &DistDescTxtObj        // common for Veh/Fuel/Trip1/Trip2
};
#define OBJLIST_TRIP2_CNT (sizeof(ObjectList_Trip2)/sizeof(OBJ_STATE)/sizeof(void far *))


// ---------------------------------------------------------------------
// Main device State 'MD_VEHDIST' Settings - Screen Focus Order
static const void far * ObjectList_VehDist[] =
{
    // objects - shown in every mode
    (void far *) &SpeedTxtObj,          // current speed
    (void far *) &SpeedDescATxtObj,     // speed descriptor A
    (void far *) &SpeedDescBTxtObj,     // speed descriptor A
    #if(GEARBOX==1)
    (void far *) &GearSymbolBmpObj,     // gearbox state
    #endif
    (void far *) &VehStateBmpObj,       // Vehicle State Icon
    (void far *) &TimeDateTxtObj,       // Date & Time
    (void far *) &SurvVehStateTxtObj,   // Vehicle State

    // objects - shown in 'MD_VEHDIST' mode only
    (void far *) &VehDistTxtObj,
    (void far *) &VehDistBmpObj,
    (void far *) &DistDescTxtObj        // common for Veh/Fuel/Trip1/Trip2
};
#define OBJLIST_VEHDIST_CNT (sizeof(ObjectList_VehDist)/sizeof(OBJ_STATE)/sizeof(void far *))



// ---------------------------------------------------------------------
// Main device State 'MD_SPEEDMAX' Settings - Screen Focus Order
static const void far * ObjectList_SpeedMax[] =
{
    // objects - shown in every mode
    (void far *) &SpeedTxtObj,          // current speed
    (void far *) &SpeedDescATxtObj,     // speed descriptor A
    (void far *) &SpeedDescBTxtObj,     // speed descriptor A
    #if(GEARBOX==1)
    (void far *) &GearSymbolBmpObj,     // gearbox state
    #endif
    (void far *) &VehStateBmpObj,       // Vehicle State Icon
    (void far *) &TimeDateTxtObj,       // Date & Time
    (void far *) &SurvVehStateTxtObj,   // Vehicle State

    // objects - shown in 'MD_SPEEDMAX' mode only
    (void far *) &SpeedMaxDescTxtObj,
    (void far *) &SpeedMaxUnitTxtObj,
    (void far *) &SpeedMaxTxtObj
};
#define OBJLIST_SPEEDMAX_CNT (sizeof(ObjectList_SpeedMax)/sizeof(OBJ_STATE)/sizeof(void far *))


// ---------------------------------------------------------------------
// Main device State 'MD_HEATGRIP' Settings - Screen Focus Order
static const void far * ObjectList_HeatGrip[] =
{
    // objects - shown in every mode
    (void far *) &SpeedTxtObj,          // current speed
    (void far *) &SpeedDescATxtObj,     // speed descriptor A
    (void far *) &SpeedDescBTxtObj,     // speed descriptor A
    #if(GEARBOX==1)
    (void far *) &GearSymbolBmpObj,     // gearbox state
    #endif
    (void far *) &VehStateBmpObj,       // Vehicle State Icon
    (void far *) &TimeDateTxtObj,       // Date & Time
    (void far *) &SurvVehStateTxtObj,   // Vehicle State

    // objects - shown in 'MD_HEATGRIP' mode only
    (void far *) &HeatGripIconBmpObj,   // HeatGrip icon
    (void far *) &HeatBarBmpObj,   // Empty Bar Icon - will be moved & multiplied
};
#define OBJLIST_HEATGRIP_CNT (sizeof(ObjectList_HeatGrip)/sizeof(OBJ_STATE)/sizeof(void far *))



/* ============================================================= */
/* this devices object focus handling - list of all objects */
/* NOTE:    this device does not need any focus, this
            list is for common object handling only! */
static const void far * ObjectList[] =
{
    // objects - shown in every mode
    (void far *) &SpeedTxtObj,
    (void far *) &SpeedDescATxtObj,
    (void far *) &SpeedDescBTxtObj,
    #if(GEARBOX==1)
    (void far *) &GearSymbolBmpObj,
    #endif
    (void far *) &TimeDateTxtObj,
    (void far *) &VehStateBmpObj,
    (void far *) &SurvVehStateTxtObj,

    // objects - shown in MD_MONITOR mode only
    (void far *) &MonVoltageTxtObj,
    (void far *) &MonVoltageBmpObj,
    (void far *) &MonAmbientTempTxtObj,
    (void far *) &MonAmbientTempBmpObj,
    (void far *) &MonWaterTempTxtObj,
    (void far *) &MonWaterTempBmpObj,
    (void far *) &MonOilTempBmpObj,
    (void far *) &MonOilTempTxtObj,
    (void far *) &MonRPMTxtObj,
    (void far *) &MonRPMBmpObj,
    (void far *) &MonVertLineBmpObj,

    // objects for our three Fuel Modes MD_FUEL_FROM / MD_FUEL_TO / MD_FUEL_CONS 
    (void far *) &FuelDistTxtObj,       
    (void far *) &FuelLiterTxtObj,
    (void far *) &FuelArrowRTxtObj,          
    (void far *) &FuelBmpObj,           
    (void far *) &ActLiterPerXBmpObj,
    (void far *) &AvrLiterPer100BmpObj,
    (void far *) &FuelConsActTxtObj,
    (void far *) &FuelConsAvrTxtObj,    
    
    // common object for Veh/Trip1/Trip2
    (void far *) &DistDescTxtObj,

    // objects - shown in MD_RPM mode only
    (void far *) &RPMTxtObj,
    (void far *) &RPMDescTxtObj,
    (void far *) &RPMBmpObj,

    // objects - shown in 'MD_VEHDIST' mode only
    (void far *) &VehDistTxtObj,
    (void far *) &VehDistBmpObj,

    // objects - shown in 'MD_TRIP1/2' mode only
    (void far *) &Trip1DescTxtObj,
    (void far *) &Trip1DistTxtObj,
    (void far *) &Trip2DescTxtObj,
    (void far *) &Trip2DistTxtObj,

    // objects - shown in 'MD_SPEEDMAX' mode only
    (void far *) &SpeedMaxDescTxtObj,
    (void far *) &SpeedMaxUnitTxtObj,
    (void far *) &SpeedMaxTxtObj,

    // objects - shown in 'MD_HEATGRIP' mode only
    (void far *) &HeatGripIconBmpObj,
    (void far *) &HeatBarBmpObj,
};
#define OBJECTLIST_SIZE   (sizeof(ObjectList)/sizeof(OBJ_STATE)/sizeof(void far *))






/***********************************************************************
 *  FUNCTION:       MainDev_Init
 *  DESCRIPTION:    all initial stuff for all objects
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MainDev_Init(void)
{
    int i;

    /* device main data */
    MDObj.eDevID       = DEVID_MAIN;
    MDObj.szDevName    = szDevName[DEVID_MAIN];
    MDObj.fFocused     = FALSE;
    MDObj.fScreenInit  = FALSE;

    /* special MDObj object lists control handling */
    MainDev_ObjListInit();

    /* error check: */
    if (  (EE_DevFlags_1.flags.MainDevState == MD_FIRST)
        ||(EE_DevFlags_1.flags.MainDevState >= MD_LAST ) )
    {   ODS1( DBG_SYS, DBG_ERROR, "MainDev_Init(): Invalid MainDevState %u corrected!", EE_DevFlags_1.flags.MainDevState );
        EE_DevFlags_1.flags.MainDevState = MD_FIRST + 1;
    }
    MDObj.wDevState    = EE_DevFlags_1.flags.MainDevState;

    /* initialize all objects of any type */
    DevObjInit( &MDObj, (void far *)TextObjInit,   STEXTOBJ_INITLISTSIZE,     OBJT_TXT  );
    DevObjInit( &MDObj, (void far *)BmpObjInit,    BMPOBJECTLISTSIZE,      OBJT_BMP  );

    /* initialize this devices objects list */
    MDObj.Objects.ObjList       = ObjectList;
    MDObj.Objects.ObjCount      = OBJECTLIST_SIZE;
    MDObj.Objects.FirstSelObj   = DevObjGetFirstSelectable(&MDObj, ObjectList, OBJECTLIST_SIZE );
    MDObj.Objects.LastSelObj    = DevObjGetLastSelectable (&MDObj, ObjectList, OBJECTLIST_SIZE );

    /* reset focus handling to start values */
    /* NOTE: No focus handling in this device! */
    DevObjFocusReset( &MDObj, ObjectList, OBJECTLIST_SIZE );

    /* return */
    ODS( DBG_SYS, DBG_INFO, "- MainDev_Init() done!");
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       MainDev_ObjListInit
 *  DESCRIPTION:    Initialize special object control structure
 *                  to handle different screens
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MainDev_ObjListInit(void)
{
    // Setup screen object list: Monitor display settings
    MDCntrl.List[MD_MONITOR].ObjList        = ObjectList_Mon;
    MDCntrl.List[MD_MONITOR].ObjCount       = OBJLIST_MON_CNT;
    MDCntrl.List[MD_MONITOR].FirstSelObj    = DevObjGetFirstSelectable(&MDObj, ObjectList_Mon, OBJLIST_MON_CNT);
    MDCntrl.List[MD_MONITOR].LastSelObj     = DevObjGetLastSelectable (&MDObj, ObjectList_Mon, OBJLIST_MON_CNT);

    // Setup screen object list: RPM display settings
    MDCntrl.List[MD_RPM].ObjList            = ObjectList_Mon;
    MDCntrl.List[MD_RPM].ObjCount           = OBJLIST_RPM_CNT;
    MDCntrl.List[MD_RPM].FirstSelObj        = DevObjGetFirstSelectable(&MDObj, ObjectList_Rpm, OBJLIST_RPM_CNT);
    MDCntrl.List[MD_RPM].LastSelObj         = DevObjGetLastSelectable (&MDObj, ObjectList_Rpm, OBJLIST_RPM_CNT);

    // Setup screen object list: Fuel km/liter SINCE LAST refuel display settings
    MDCntrl.List[MD_FUEL_FROM].ObjList      = ObjectList_Mon;
    MDCntrl.List[MD_FUEL_FROM].ObjCount     = OBJLIST_FUEL_FROM_CNT;
    MDCntrl.List[MD_FUEL_FROM].FirstSelObj  = DevObjGetFirstSelectable(&MDObj, ObjectList_Fuel_From, OBJLIST_FUEL_FROM_CNT);
    MDCntrl.List[MD_FUEL_FROM].LastSelObj   = DevObjGetLastSelectable (&MDObj, ObjectList_Fuel_From, OBJLIST_FUEL_FROM_CNT);

    // Setup screen object list: Fuel km/liter UNTIL NEXT refuel display settings
    MDCntrl.List[MD_FUEL_TO].ObjList        = ObjectList_Mon;
    MDCntrl.List[MD_FUEL_TO].ObjCount       = OBJLIST_FUEL_TO_CNT;
    MDCntrl.List[MD_FUEL_TO].FirstSelObj    = DevObjGetFirstSelectable(&MDObj, ObjectList_Fuel_To, OBJLIST_FUEL_TO_CNT);
    MDCntrl.List[MD_FUEL_TO].LastSelObj     = DevObjGetLastSelectable (&MDObj, ObjectList_Fuel_To, OBJLIST_FUEL_TO_CNT);

    // Setup screen object list: Fuel consumption display settings
    MDCntrl.List[MD_FUEL_CONS].ObjList      = ObjectList_Mon;
    MDCntrl.List[MD_FUEL_CONS].ObjCount     = OBJLIST_FUEL_CONS_CNT;
    MDCntrl.List[MD_FUEL_CONS].FirstSelObj  = DevObjGetFirstSelectable(&MDObj, ObjectList_Fuel_To, OBJLIST_FUEL_CONS_CNT);
    MDCntrl.List[MD_FUEL_CONS].LastSelObj   = DevObjGetLastSelectable (&MDObj, ObjectList_Fuel_To, OBJLIST_FUEL_CONS_CNT);

    // Setup screen object list: Tripcounter1 display settings
    MDCntrl.List[MD_TRIP1].ObjList          = ObjectList_Mon;
    MDCntrl.List[MD_TRIP1].ObjCount         = OBJLIST_TRIP1_CNT;
    MDCntrl.List[MD_TRIP1].FirstSelObj      = DevObjGetFirstSelectable(&MDObj, ObjectList_Trip1, OBJLIST_TRIP1_CNT);
    MDCntrl.List[MD_TRIP1].LastSelObj       = DevObjGetLastSelectable (&MDObj, ObjectList_Trip1, OBJLIST_TRIP1_CNT);

    // Setup screen object list: Tripcounter2 display settings
    MDCntrl.List[MD_TRIP2].ObjList          = ObjectList_Mon;
    MDCntrl.List[MD_TRIP2].ObjCount         = OBJLIST_TRIP2_CNT;
    MDCntrl.List[MD_TRIP2].FirstSelObj      = DevObjGetFirstSelectable(&MDObj, ObjectList_Trip2, OBJLIST_TRIP2_CNT);
    MDCntrl.List[MD_TRIP2].LastSelObj       = DevObjGetLastSelectable (&MDObj, ObjectList_Trip2, OBJLIST_TRIP2_CNT);

    // Setup screen object list: Vehicle Distance display settings
    MDCntrl.List[MD_VEHDIST].ObjList        = ObjectList_Mon;
    MDCntrl.List[MD_VEHDIST].ObjCount       = OBJLIST_VEHDIST_CNT;
    MDCntrl.List[MD_VEHDIST].FirstSelObj    = DevObjGetFirstSelectable(&MDObj, ObjectList_VehDist, OBJLIST_VEHDIST_CNT);
    MDCntrl.List[MD_VEHDIST].LastSelObj     = DevObjGetLastSelectable (&MDObj, ObjectList_VehDist, OBJLIST_VEHDIST_CNT);

    // Setup screen object list: SpeedMax display settings
    MDCntrl.List[MD_SPEEDMAX].ObjList       = ObjectList_Mon;
    MDCntrl.List[MD_SPEEDMAX].ObjCount      = OBJLIST_SPEEDMAX_CNT;
    MDCntrl.List[MD_SPEEDMAX].FirstSelObj   = DevObjGetFirstSelectable(&MDObj, ObjectList_SpeedMax, OBJLIST_SPEEDMAX_CNT);
    MDCntrl.List[MD_SPEEDMAX].LastSelObj    = DevObjGetLastSelectable (&MDObj, ObjectList_SpeedMax, OBJLIST_SPEEDMAX_CNT);

    // Setup screen object list: HeatGrip display settings
    MDCntrl.List[MD_HEATGRIP].ObjList       = ObjectList_Mon;
    MDCntrl.List[MD_HEATGRIP].ObjCount      = OBJLIST_HEATGRIP_CNT;
    MDCntrl.List[MD_HEATGRIP].FirstSelObj   = DevObjGetFirstSelectable(&MDObj, ObjectList_HeatGrip, OBJLIST_HEATGRIP_CNT);
    MDCntrl.List[MD_HEATGRIP].LastSelObj    = DevObjGetLastSelectable (&MDObj, ObjectList_HeatGrip, OBJLIST_HEATGRIP_CNT);

    // initial value for current mainDevice display state
    MDCntrl.eState = MD_MONITOR;
}



/***********************************************************************
 *  FUNCTION:       MainDev_Show
 *  DESCRIPTION:    bring updated main device to display
 *                  by calling Show-Fct. of all relevant objects
 *  PARAMETER:      TRUE    show (all/partial) objects,
 *                  FALSE   clear screen
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MainDev_Show(BOOL fShow)
{
    ERRCODE error = ERR_OK;
    MESSAGE NewMsg;             /* for screen fresh message */

    /* ================================================= */
    /* 'show' or 'clear' screen? */
    if (fShow == TRUE)
    {
        /* update of all displayable values */
        MainDev_UpdMeas();

        /* ------------------------------------------ */
        /* do we have to repaint all? */
        if (MDObj.fScreenInit == FALSE)
        {
            /* yes, repaint complete screen */

            /* Clear Screen & set new state */
            DisplClearScreen(0x0);
            MDObj.fScreenInit  = TRUE;

            /* horizontal line between wheel speed & rpm */
            MainDev_ShowHorLine();

            /* always show vehicle speed in upper display part */
            Obj_TextSt_Show( &SpeedTxtObj );
            Obj_TextSt_Show( &SpeedDescATxtObj );
            Obj_TextSt_Show( &SpeedDescBTxtObj );

            /* show lowest display line 'VehicleState' or 'TimeDate' */
            if (SurvShowVehState)
                 Obj_TextSt_Show( &SurvVehStateTxtObj );
            else MainDev_UpdTimeDate(); // initial display only!

            /* ---------------------------------------------------- */
            /* Show Info/Warning/Error Icon */
            MainDev_Show_VehStateIcon();

            /* ---------------------------------------------------- */
            // selected gear */
            #if(GEARBOX==1)
            Obj_Bmp_Show( &GearSymbolBmpObj );
            #endif

            /* ---------------------------------------------------- */
            /* select display mode: which data to be shown below speed? */
            switch (MDObj.wDevState)
            {
                case MD_RPM:
                    Obj_TextSt_Show( &RPMTxtObj );
                    Obj_TextSt_Show( &RPMDescTxtObj );
                    Obj_Bmp_Show( &RPMBmpObj );
                    break;
                case MD_FUEL_FROM:
                    Obj_Bmp_Show(    &FuelBmpObj      );
                    Obj_TextSt_Show( &FuelArrowRTxtObj);                    
                    Obj_TextSt_Show( &FuelDistTxtObj  );
                    Obj_TextSt_Show( &FuelLiterTxtObj );
                    break;
                case MD_FUEL_TO:
                    Obj_Bmp_Show(    &FuelBmpObj      );
                    Obj_TextSt_Show( &FuelArrowRTxtObj);                                        
                    Obj_TextSt_Show( &FuelDistTxtObj );
                    Obj_TextSt_Show( &FuelLiterTxtObj);
                    break;                
                case MD_FUEL_CONS:
                    Obj_Bmp_Show(   &FuelBmpObj          );
                    Obj_Bmp_Show(   &ActLiterPerXBmpObj  );
                    Obj_Bmp_Show(   &AvrLiterPer100BmpObj);
                    Obj_TextSt_Show(&FuelConsActTxtObj   );
                    Obj_TextSt_Show(&FuelConsAvrTxtObj   );
                    break;
                case MD_VEHDIST:
                    Obj_TextSt_Show( &VehDistTxtObj );
                    Obj_TextSt_Show( &DistDescTxtObj );
                    Obj_Bmp_Show( &VehDistBmpObj );
                    break;
                case MD_TRIP1:
                    Obj_TextSt_Show( &Trip1DescTxtObj );
                    Obj_TextSt_Show( &Trip1DistTxtObj );
                    Obj_TextSt_Show( &DistDescTxtObj );
                    break;
                case MD_TRIP2:
                    Obj_TextSt_Show( &Trip2DescTxtObj );
                    Obj_TextSt_Show( &Trip2DistTxtObj );
                    Obj_TextSt_Show( &DistDescTxtObj );
                    break;
                case MD_SPEEDMAX:
                    Obj_TextSt_Show( &SpeedMaxDescTxtObj );
                    Obj_TextSt_Show( &SpeedMaxTxtObj );
                    Obj_TextSt_Show( &SpeedMaxUnitTxtObj );
                    break;
                case MD_HEATGRIP:
                    MainDev_Show_Heatgrip(TRUE);
                    break;
                case MD_MONITOR:
                    MainDev_Show_Monitor(TRUE);
                    break;
                default:
                    ODS1( DBG_SYS, DBG_ERROR,
                          "MainDev_Show(): unknown state: %u", MDObj.wDevState);
            }
        }
        else
        /* ------------------------------------------ */
        {
            /* No, repaint only changed stuff */

            /* show vehicle speed */
            Obj_TextSt_Show( &SpeedTxtObj );

            /* show lowest display line 'VehicleState' or 'TimeDate' */
            if (SurvShowVehState)
                 Obj_TextSt_Show( &SurvVehStateTxtObj );
            // else MainDev_UpdTimeDate(); // NO UPDATE HERE, will be done by special update messages MSG_SECOND_GONE only!

            /* ---------------------------------------------------- */
            /* Show Info/Warning/Error Icon */
            MainDev_Show_VehStateIcon();

            // SIMULATION ONLY: selected gear - show all gears every second */
            #if(GEARBOX==1)
            switch ( wSecCounter % 10 )
            {   case 0: GearSymbolBmpObj.Data.fpucBitmap = bmp7Seg_0_16x16; break;
                case 1: GearSymbolBmpObj.Data.fpucBitmap = bmp7Seg_1_16x16; break;
                case 2: GearSymbolBmpObj.Data.fpucBitmap = bmp7Seg_2_16x16; break;
                case 3: GearSymbolBmpObj.Data.fpucBitmap = bmp7Seg_3_16x16; break;
                case 4: GearSymbolBmpObj.Data.fpucBitmap = bmp7Seg_4_16x16; break;
                case 5: GearSymbolBmpObj.Data.fpucBitmap = bmp7Seg_5_16x16; break;
                case 6: GearSymbolBmpObj.Data.fpucBitmap = bmp7Seg_6_16x16; break;
                case 7: GearSymbolBmpObj.Data.fpucBitmap = bmp7Seg_7_16x16; break;
                case 8: GearSymbolBmpObj.Data.fpucBitmap = bmp7Seg_8_16x16; break;
                case 9: GearSymbolBmpObj.Data.fpucBitmap = bmp7Seg_9_16x16; break;
            }
            Obj_Bmp_Show( &GearSymbolBmpObj );
            #endif

            /* which data to be shown below speed? */
            switch (MDObj.wDevState)
            {
                // show bigger parts
                case MD_RPM:        Obj_TextSt_Show( &RPMTxtObj );          break;
                case MD_FUEL_FROM:
                    Obj_TextSt_Show( &FuelDistTxtObj );
                    Obj_TextSt_Show( &FuelLiterTxtObj);
                    break;
                case MD_FUEL_TO:
                    Obj_TextSt_Show( &FuelDistTxtObj );
                    Obj_TextSt_Show( &FuelLiterTxtObj);
                    break;                
                case MD_FUEL_CONS:
                    Obj_TextSt_Show(&FuelConsActTxtObj   );
                    Obj_TextSt_Show(&FuelConsAvrTxtObj   );
                    break;
                case MD_TRIP1:      Obj_TextSt_Show( &Trip1DistTxtObj );    break;
                case MD_TRIP2:      Obj_TextSt_Show( &Trip2DistTxtObj );    break;
                case MD_VEHDIST:    Obj_TextSt_Show( &VehDistTxtObj );      break;
                case MD_SPEEDMAX:   Obj_TextSt_Show( &SpeedMaxTxtObj );     break;
                case MD_HEATGRIP:
                    MainDev_Show_Heatgrip(FALSE);
                    break;
                case MD_MONITOR:
                    /* call the dedicated subfunction */
                    MainDev_Show_Monitor(FALSE);
                    break;

                default:
                    ODS1( DBG_SYS, DBG_ERROR,
                          "MainDev_Show(): unknown state: %u", MDObj.wDevState);
            }
        }
    }
    else
    /* ================================================= */
    {
        // leave a clear screen for next device
        DisplClearScreen(0x0);

        // reset states of all objects of this device
        DevObjClearState(  &MDObj,
                            MDObj.Objects.ObjList,
                            MDObj.Objects.ObjCount,
                            OS_DISPL | OS_EDIT );

        // reset devices screen state
        MDObj.fScreenInit = FALSE;
    }
}



/***********************************************************************
 *  FUNCTION:       MainDev_Show_VehStateIcon
 *  DESCRIPTION:    Shows the most relevant icon (if any)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Just switches the Bitmap raw data to the adequate ressource
 *********************************************************************** */
void MainDev_Show_VehStateIcon(void)
{
    /* default: no/empty icon */
    VehStateBmpObj.Data.fpucBitmap = bmpEmpty_16x16;

    /* which icon has to be used: Info / Warning / Error ? */
    if      ( Surv_ListGetCount( eSURVST_ERROR ) )
        VehStateBmpObj.Data.fpucBitmap = bmpError_16x16;
    else if ( Surv_ListGetCount( eSURVST_WARNING) )
        VehStateBmpObj.Data.fpucBitmap = bmpWarning_16x16;
    else if ( Surv_ListGetCount( eSURVST_INFO  ) )
    {   VehStateBmpObj.Data.fpucBitmap = bmpInfo_16x16;
    }

    /* Check: No icon shown? -> Heatgrip-icon needed?
       Note: Only if not already shown in MD_HEATGRIP state */
    if (  ( EE_CoolrideCtrl.flags.CoolrAvail == TRUE                )
        &&( VehStateBmpObj.Data.fpucBitmap  == bmpEmpty_16x16  )     // no other icon used?
        &&( MDObj.wDevState                 != MD_HEATGRIP         )     // not already shown below?
        &&( DigInDrv_GPI_GetMeas(EE_CoolrideCtrl.flags.CoolrGPI)->ucPWM > 0 ) )   // PWM is active?
    {   VehStateBmpObj.Data.fpucBitmap = bmpHeatGrip_16x16;
    }

    /* show the selected bmp */
    Obj_Bmp_Show( &VehStateBmpObj );
}


/***********************************************************************
 *  FUNCTION:       MainDev_Show_Monitor
 *  DESCRIPTION:    Special subfunction to show monitor part only
 *                  by calling Show-Fct. of all relevant objects
 *  PARAMETER:      fComplete   TRUE  = show all objects,
 *                              FALSE = show only dynamic stuff
 *  RETURN:         -
 *  COMMENT:        Assumes to be called for MD_MONITOR state only!
 *********************************************************************** */
void MainDev_Show_Monitor(BOOL fComplete)
{
    /* ================================================= */
    /* show complete or refresh dynamic only? */
    if (fComplete == TRUE)
    {
        /* ---------------------------------------------------- */
        // vertical divider line */
        Obj_Bmp_Show( &MonVertLineBmpObj );

        /* ---------------------------------------------------- */
        // show internal/external temperature
        Obj_Bmp_Show   ( &MonAmbientTempBmpObj );
        Obj_TextSt_Show( &MonAmbientTempTxtObj );

        /* ---------------------------------------------------- */
        // show supply voltage
        Obj_Bmp_Show   ( &MonVoltageBmpObj );
        Obj_TextSt_Show( &MonVoltageTxtObj );

        /* ---------------------------------------------------- */
        // Automatic Display Switch between WaterTemp / NV_FuelDistance
        // TBD: Will later be done via Settings, but at the moment we
        //      use this so called 'Arnoldschen Elfenbeinturm' here! ;-)
        // Water temp available?
        if ( AnaInGetWatTemperature() > ANAIN_TEMP_SENSORDETECT )
        {   Obj_Bmp_Show   ( &MonWaterTempBmpObj     );
            Obj_TextSt_Show( &MonWaterTempTxtObj     );
        }
        else /* display fuel distance if no water temp sensor connected */
        {   Obj_Bmp_Show   ( &MonFuelDistBmpObj     );
            Obj_TextSt_Show( &MonFuelDistTxtObj     );
        }

        /* ---------------------------------------------------- */
        // Automatic Display Switch between OilTemp / RPM
        // TBD: Will later be done via Settings, but at the moment we
        //      use this so called 'Arnoldschen Elfenbeinturm' here! ;-)
        // Oil temp available?
        if ( AnaInGetOilTemperature() > ANAIN_TEMP_SENSORDETECT )
        {   Obj_Bmp_Show ( &MonOilTempBmpObj );
            Obj_TextSt_Show( &MonOilTempTxtObj );
        }
        else /* display RPM if no oil temp sensor connected */
        {   Obj_Bmp_Show ( &MonRPMBmpObj );
            Obj_TextSt_Show( &MonRPMTxtObj );
        }
    }
    else
    /* ================================================= */
    {
        /* No, repaint only changed stuff */

        // show external air temp (if n.a.: internal device temp)
        Obj_TextSt_Show( &MonAmbientTempTxtObj );

        // show battery supply voltage
        Obj_TextSt_Show( &MonVoltageTxtObj );

        // Automatic Display Switch between WaterTemp / NV_FuelDistance
        // TBD: Will later be done via Settings, but at the moment we
        //      use this so called 'Arnoldschen Elfenbeinturm' here! ;-)
        // Water temp available?
        if ( AnaInGetWatTemperature() > ANAIN_TEMP_SENSORDETECT )
        {   /* might have been changed! */
            Obj_Bmp_Show   ( &MonWaterTempBmpObj );
            Obj_TextSt_Show( &MonWaterTempTxtObj );
        }
        else
        {   /* might have been changed! */
            Obj_Bmp_Show   ( &MonFuelDistBmpObj  );
            Obj_TextSt_Show( &MonFuelDistTxtObj  );
        }

        // Automatic Display Switch between OilTemp / RPM
        // TBD: Will later be done via Settings, but at the moment we
        //      use this so called 'Arnoldschen Elfenbeinturm' here! ;-)
        // Oil temp available?
        if ( AnaInGetOilTemperature() > ANAIN_TEMP_SENSORDETECT )
        {   /* might have been changed! */
            Obj_Bmp_Show   ( &MonOilTempBmpObj     );
            Obj_TextSt_Show( &MonOilTempTxtObj     );
        }
        else
        {   /* might have been changed! */
            Obj_Bmp_Show   ( &MonRPMBmpObj );
            Obj_TextSt_Show( &MonRPMTxtObj );
        }
    }
}



/***********************************************************************
 *  FUNCTION:       MainDev_Show_Heatgrip
 *  DESCRIPTION:    Special subfunction to show heat-grip part only
 *                  by calling Show-Fct. of all relevant objects
 *  PARAMETER:      fInitial    TRUE  = show all objects,
 *                              FALSE = show only dynamic stuff
 *  RETURN:         -
 *  COMMENT:        Assumes to be called for MD_HEATGRIP state only!
 *
 *                  Uses a SINGLE heat bargraph bmp object to
 *                  generate a COMPLETE 5-part bar:
 *                      - changes the referenced bitmap to 'full'/'empty' if needed
 *                      - changes the origin x-pos to show first bar-part
 *                      - adapts to bmp-obj width to increment x-pos
 *                        of 4 further bar-parts
 *
 *                  Coolride internally can handle 10% PWM steps for ambiant
 *                  temperature compensation, but LEDs (and our display here)
 *                  always show 5 bars = 20% steps!
 *********************************************************************** */
void MainDev_Show_Heatgrip(BOOL fInitial)
{
    UINT8   i;
    OBJ_BMP objBmp    = HeatBarBmpObj;  // we use a copy of that object to manipulate!
    UINT8   ucPwmCurr = 0;              // current PWM value
    UINT8   ucPwmCmp  = 5;              // comparison value to select emty/full bmp

    /* get a fresh PWM value */
    ucPwmCurr = DigInDrv_GPI_GetMeas(EE_CoolrideCtrl.flags.CoolrGPI)->ucPWM;

    /* update leftside icon only at initial state */
    if (fInitial == TRUE)
    {   Obj_Bmp_Show(  &HeatGripIconBmpObj );
    }

    /* loop to generate all 5 bar parts (full/empty) */
    for (i=0; i<MD_HEATBARPARTS; i++)
    {
        /* check: use full/empty bitmap?
           Note: ucPwmCmp level always 5% higher to
                 assure 10% detection accuracy */
        if ( ucPwmCurr > ucPwmCmp )
             objBmp.Data.fpucBitmap = bmpHeatBarFull_19x8;
        else objBmp.Data.fpucBitmap = bmpHeatBarEmpty_19x8;

        /* show the modified bmp object */
        Obj_Bmp_Show( &objBmp );

        /* adapt to next position */
        objBmp.Org.wXPos += objBmp.Data.wWidth + 1;

        /* increase PWM comparison value for next loop */
        ucPwmCmp += 100 / MD_HEATBARPARTS;
    }
}




/***********************************************************************
 *  FUNCTION:       MainDev_MsgEntry
 *  DESCRIPTION:    Receive Message Handler called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MainDev_MsgEntry(MESSAGE GivenMsg)
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
            if ( MDObj.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_MAIN]);
                MSG_BUILD_SETFOCUS(NewMsg,DEVID_MAIN,MSG_SENDER_ID(GivenMsg));  /* build answer message */
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                    /* send answer message */
                MDObj.fFocused = FALSE;                                         /* clear our focus */
                MainDev_Show(FALSE);                                            /* clear our screen */
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
                if (  (MDObj.fFocused       == TRUE             )
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
                MDObj.fFocused = TRUE;                              /* set our focus */
                MainDev_Show(TRUE);                                 /* show our screen */
                EE_DevFlags_1.flags.ActDevNr = DEVID_MAIN;          /* save device# for restore */
                SurvShowVehState = 0;                               /* reset lower VehicleStateString state */
                RValue = ERR_MSG_PROCESSED;
             }
             else
             {
                if ( MDObj.fFocused == TRUE )
                {
                    /*  Some other device should be given the focus,
                        BUT WE'VE GOT THE FOCUS!
                        It should first ask US to give the focus! :-( */
                    ODS3(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but %s had focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[MSG_RECEIVER_ID(GivenMsg)],
                            szDevName[DEVID_MAIN]);
                    MDObj.fFocused = FALSE;                        /* loose our focus */
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
    if (  (MDObj.fFocused == TRUE )
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
                    RValue = MainDev_MsgEntry_VehState(GivenMsg);

                /* Try all distance objects, wether to be reseted or not ([OK]-long) */
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = MainDev_MsgEntry_VehDistRst(GivenMsg);

                /* Try to toggle lower screen part to next information line ([Up/Down]-short) */
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = MainDev_MsgEntry_StateMachine(GivenMsg);

                /* Try to switch to next device ([Up+Down]-short) */
                if (  (RValue == ERR_MSG_NOT_PROCESSED                    )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN))
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYTM_PRESSED_SHORT              ) )
                {
                    /* give focus immediatly to next screen */
                    MDObj.fFocused = FALSE;                                 /* clear our focus */
                    MainDev_Show(FALSE);                                    /* clear our screen */
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_MAIN, DEVID_TRIPCOUNT);/* next: TripCounter */
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_SCREEN_RFRSH:
                MainDev_Show(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;

            /* special messages ------------ */

            /* special handling of vehicle state string in lowest display line */
            case MSG_VEHSTATE_SHOW:  // no break!
            case MSG_VEHSTATE_HIDE:
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = MainDev_MsgEntry_VehState(GivenMsg);
                break;

            /* trigger time / date screen update only */
            case MSG_SECOND_GONE:
                MainDev_UpdTimeDate();
                RValue = ERR_MSG_PROCESSED;
                break;

            default: RValue = ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       MainDev_MsgEntry_StateMachine
 *  DESCRIPTION:    Handle the display states of main device
 *                  and re-init the display state if any changes
 *  PARAMETER:      MESSAGE
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MainDev_MsgEntry_StateMachine(MESSAGE Msg)
{
    MESSAGE_ID  MsgId  = MSG_ID(Msg);                       /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;

    /* -------------------------------------- */
    /* scroll up? */
    if (  (RValue == ERR_MSG_NOT_PROCESSED             )    /* still unpocessed */
        &&(MsgId  == MSG_KEY_UP                        )    /* [UP] */
        &&(MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED ) )  /* not released */
    {
        DevObjClearState(  &MDObj,                          /* reset states of all objects of this device */
                            MDObj.Objects.ObjList,
                            MDObj.Objects.ObjCount,
                            OS_DISPL | OS_SELECT | OS_EDIT );

        /* previous state */
        MDObj.wDevState--;                                  

        /* wrap around? */
        if (MDObj.wDevState == MD_FIRST)                    
            MDObj.wDevState = (MD_LAST-1);
        
        /* jump over Coolride, if n.a.! */
        if (  ( MDObj.wDevState == MD_HEATGRIP )            
            &&( EE_CoolrideCtrl.flags.CoolrAvail == FALSE ) )
            MDObj.wDevState--;

        /* jump over Fuel Consumption, if n.a.! */
        if (  ( MDObj.wDevState == MD_FUEL_CONS )            
            &&( EE_FuelSensCtrl.flags.FuelSAvail == FALSE ) )
            MDObj.wDevState--;

        /* rebuild complete screen right now */
        MDObj.fScreenInit   = FALSE;                        
        MainDev_Show(TRUE);                                 
        RValue = ERR_MSG_PROCESSED;
        ODS1( DBG_SYS, DBG_INFO, "MainDevState: %u", MDObj.wDevState);
    }

    /* -------------------------------------- */
    /* scroll down? */
    if (  (RValue == ERR_MSG_NOT_PROCESSED             )    /* still unpocessed */
        &&(MsgId  == MSG_KEY_DOWN                      )    /* [DOWN] */
        &&(MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED ) )  /* not released */
    {
        DevObjClearState(   &MDObj,                         /* reset states of all objects of this device */
                            MDObj.Objects.ObjList,
                            MDObj.Objects.ObjCount,
                            OS_DISPL | OS_SELECT | OS_EDIT );

        /* next state */
        MDObj.wDevState++;                                         
        
        /* jump over Coolride, if n.a.! */
        if (  ( MDObj.wDevState == MD_HEATGRIP )            
            &&( EE_CoolrideCtrl.flags.CoolrAvail == FALSE ) )
            MDObj.wDevState++;

        /* jump over Fuel Consumption, if n.a.! */
        if (  ( MDObj.wDevState == MD_FUEL_CONS )            
            &&( EE_FuelSensCtrl.flags.FuelSAvail == FALSE ) )
            MDObj.wDevState++;

        /* wrap around? */
        if (MDObj.wDevState == MD_LAST)                     
            MDObj.wDevState = (MD_FIRST+1);

        /* rebuild complete screen right now */
        MDObj.fScreenInit   = FALSE;                        
        MainDev_Show(TRUE);                                 
        RValue = ERR_MSG_PROCESSED;
        ODS1( DBG_SYS, DBG_INFO, "MainDevState: %u", MDObj.wDevState);
    }

    /* -------------------------------------- */
    /* save device state */
    EE_DevFlags_1.flags.MainDevState = MDObj.wDevState;

    return RValue;
}



/***********************************************************************
 *  FUNCTION:       MainDev_MsgEntry_VehDistRst
 *  DESCRIPTION:    Handle the 'distance reset' messages for fuel,
 *                  trip1 and trip2 distance
 *  PARAMETER:      MESSAGE
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MainDev_MsgEntry_VehDistRst(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    static BOOL fLocked = FALSE;                    /* TRUE = key not yet relases */

    // check: main device in state to reset anything?
    if (   ( MDObj.wDevState == MD_FUEL_FROM )
         ||( MDObj.wDevState == MD_FUEL_TO   )
         ||( MDObj.wDevState == MD_FUEL_CONS )
         ||( MDObj.wDevState == MD_TRIP1     )
         ||( MDObj.wDevState == MD_TRIP2     )
         ||( MDObj.wDevState == MD_SPEEDMAX  ) )
    {
        /* [OK] pressed+released long'? -> ITS FOR US! -> Re-enable Reset of distances!
          Note: This code shall prevent a 'flickering info led' if the user doesn't
                release the key which would repeat the distance reset */
        if (  ( MsgId == MSG_KEY_OK                          )      /* OK key?  */
            &&( MSG_KEY_TRANSITION(Msg) == KEYTRANS_RELEASED )      /* right now released* */
            &&( MSG_KEY_DURATION(Msg) > KEYTM_PRESSED_VLONG  )      /* has just shortly been pressed? */
            &&( fLocked == TRUE                              ) )    /* is not already active? */
        {
            fLocked = FALSE;                // Re-enable another Reset of distances
            RValue = ERR_MSG_PROCESSED;     // done!
        }

        /* user presses OK Button > 2 sec: focus & start edit */
        else if (  ( MsgId == MSG_KEY_OK                        )    /* [OK] pressed? */
                 &&( MSG_KEY_TRANSITION(Msg) == KEYTRANS_ON     )    /* still pressed? */
                 &&( MSG_KEY_DURATION(Msg) > KEYTM_PRESSED_VLONG)    /* longer than 2 sec.? */
                 &&( fLocked == FALSE                           ) )  /* key released since last reset? */
        {
            DIST_TYPE Dist;
            Dist.dkm = 0;               /* reset distance value */
            switch (MDObj.wDevState)    
            {   /* same behaviour for all 3 modes: */
                case MD_FUEL_FROM:   
                case MD_FUEL_TO:   
                case MD_FUEL_CONS:
                    Meas_Set_FuelDist( &Dist );                      
                    /* check: Reset FuelConsumption too? */
                    if ( EE_FuelSensCtrl.flags.FuelSAvail == TRUE ) 
                    {   // reset NVRAM value and current FuelSensor impulse counter
                        NV_FuelSensImp = 0L;   
                        DigInDrv_GPI_RstCount( EE_FuelSensCtrl.flags.FuelSGPI );
                    }
                    break;
                case MD_TRIP1:  Meas_Set_TripCnt( eTRIPCOMM_A, &Dist ); break;
                case MD_TRIP2:  Meas_Set_TripCnt( eTRIPCOMM_B, &Dist ); break;
                case MD_SPEEDMAX: EE_SpeedMax = 0; break;
                default: break;
            }
            Beep_SignalOk();                   /* beep ok */
            LED_SignalOk();                    /* LED ok */
            fLocked = TRUE;             /* don't repeat this until key released */
            RValue = ERR_MSG_PROCESSED; /* done */
        }
    }
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       MainDev_MsgEntry_VehState
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
ERRCODE MainDev_MsgEntry_VehState(MESSAGE Msg)
{
    MESSAGE_ID  MsgId   = MSG_ID(Msg);                          /* get message id */
    ERRCODE     RValue  = ERR_MSG_NOT_PROCESSED;                /* return value */
    MESSAGE     NewMsg;                                         /* to generate show/hide messages */

    /* [OK] pressed+released shortly'? -> ITS FOR US! -> show 'Vehicle State' instead of TimeDate! */
    if (  ( MsgId == MSG_KEY_OK                          )      /* OK key?  */
        &&( MSG_KEY_TRANSITION(Msg) == KEYTRANS_RELEASED )      /* right now released* */
        &&( MSG_KEY_DURATION(Msg) < KEYTM_PRESSED_VLONG  ) )    /* has just shortly been pressed? */
    {
        /* SPECIAL USE CASE: KEY-handling for Coolride-Extension: */
        /* Coolride-Extension: If available & in MD_HEATGRIP state: stimulate Coolride Input! */
        if ( MDObj.wDevState == MD_HEATGRIP )
        {
            GPO_SignalCoolRide();           // generate Coolride key input
            LED_SignalAck();                // LED acknowledge for user
            Beep_SignalAck();               // Beeper acknowledge for user (if not disabled by user)
            //GPO_SetNewState( eGPO_1,  COOLR_KEYOUT_SIGNAL );  // JUST FOR TESTs: additionally activate other GPO
        }
        else
        {   /* COMMON USE CASE: */
            /* send message to our self to SHOW vehicle state */
            MSG_BUILD_UINT8(NewMsg, MSG_VEHSTATE_SHOW, 0, 0, 0);
            MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);

            /* prepare delayed message to our self to HIDE vehicle state after 3 seconds */
            MSG_BUILD_UINT8(NewMsg, MSG_VEHSTATE_HIDE, 0, 0, 0);
            SetTimerMsg(NewMsg, SEC2TICKS(3));          /* send delayed message */
            ODS( DBG_SYS, DBG_INFO, "MainDev: [OK] short pressed!");
            RValue = ERR_MSG_PROCESSED;                 /* processed! */
        }
    }

    /* Received special Message to SHOW Vehicle state? */
    else if ( MsgId == MSG_VEHSTATE_SHOW )
    {
        SurvShowVehState = 1;                           /* change view right now! */
        MainDev_Show(TRUE);                             /* don't wait until REFRESH, show right now! */
        ODS( DBG_SYS, DBG_INFO, "MainDev: SHOW VehicleState!");
        RValue = ERR_MSG_PROCESSED;                     /* processed! */
    }

    /* Received special Message to HIDE Vehicle state? */
    else if ( MsgId == MSG_VEHSTATE_HIDE )
    {
        SurvShowVehState = 0;                           /* change view right now! */
        ODS( DBG_SYS, DBG_INFO, "MainDev: HIDE VehicleState!");
        RValue = ERR_MSG_PROCESSED;                     /* processed! */
    }
    else RValue = ERR_MSG_NOT_PROCESSED;

    return(RValue);
}



/***********************************************************************
 *  FUNCTION:       MainDev_UpdMeas
 *  DESCRIPTION:    Just Update/Refresh all values that might be
 *                  displayed inside the display
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MainDev_UpdMeas(void)
{
    UINT16  wWheelSpeed;

    /* update speed & distances */
    wWheelSpeed = MeasGetWheelSpeed(MR_KM_PER_H);
    sprintf( szSpeed,        "%3u",      wWheelSpeed);
    sprintf( szRPM,          "%5u",      MeasGetEngineSpeed(MR_RPM_R10));
    sprintf( szVehDist,      "%06lu",    Meas_Get_VehicDist(MR_KM));
    sprintf( szTrip1Dist,    "%4u%c%1u", Meas_Get_TripCnt( eTRIPCOMM_A, MR_KM_ONLY), RESTXT_DEC_SEPARATOR, Meas_Get_TripCnt( eTRIPCOMM_A, MR_HM_ONLY));
    sprintf( szTrip2Dist,    "%4u%c%1u", Meas_Get_TripCnt( eTRIPCOMM_B, MR_KM_ONLY), RESTXT_DEC_SEPARATOR, Meas_Get_TripCnt( eTRIPCOMM_B, MR_HM_ONLY));
    sprintf( szSpeedMax,     "%3u",      EE_SpeedMax);

    /* update complete monitor measurement stuff */
    MainDev_UpdMeas_Mon();
    
    /* update complete fuel measurement stuff */
    MainDev_UpdMeas_Fuel();    
    
}



/***********************************************************************
 *  FUNCTION:       MainDev_UpdMeas_Mon
 *  DESCRIPTION:    Separate handling of Monitor data
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MainDev_UpdMeas_Mon(void)
{               
    CHAR    szBuff[10];
    
    /* check current display state */
    if (MDObj.wDevState == MD_MONITOR)
    {
        /* update monitor fuel distance anyway */    
        sprintf( szMonFuelDist,  "%3u %s", Meas_Get_FuelDist(MR_KM_ONLY), RESTXT_DIST_DESC );            
        
        /* update temperatures (use external air temp if available, else internal) */
        sprintf( szMonRPM,       "%5u",      MeasGetEngineSpeed(MR_RPM_R10));            
        if ( AnaInGetAirTemperature() > ANAIN_TEMP_SENSORDETECT )
        {   AnaInFormatTemperature(AnaInGetAirTemperature(), szBuff, sizeof(szBuff));  
        }
        else 
        {   AnaInFormatTemperature(AnaInGetTemperature(),    szBuff, sizeof(szBuff));  
        }
        sprintf( (char far *)szMonAmbientTemp, "%s %s", (char far *)szBuff, (char far *)RESTXT_DEGC_DESC );    
        AnaInFormatTemperature(AnaInGetWatTemperature(),szBuff, sizeof(szBuff));
        sprintf( (char far *)szMonWaterTemp, "%s %s", (char far *)szBuff, (char far *)RESTXT_DEGC_DESC );        
        AnaInFormatTemperature(AnaInGetOilTemperature(), szBuff,   sizeof(szBuff));
        sprintf( (char far *)szMonOilTemp, "%s %s", (char far *)szBuff, (char far *)RESTXT_DEGC_DESC );            
        
        /* update voltage  */
        AnaInFormatVoltage(AnaInGetVoltage(), szBuff, sizeof(szBuff));    
        sprintf( (char far *)szMonBattVolt, "%s %s", (char far *)szBuff, (char far *)RESTXT_VOLT_DESC );                    
    }
}





/***********************************************************************
 *  FUNCTION:       MainDev_UpdTimeDate
 *  DESCRIPTION:    Separate handling of TimeDate screen refreshs
 *                  to synchronize display to RTC seconds
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Will be called by special message MSG_SECOND_GONE.
 *********************************************************************** */
void MainDev_UpdTimeDate(void)
{
    CHAR    szBuffer[15];                               // text buffer

    // check conditions to display timedate */
    if (  ( MDObj.fScreenInit == TRUE  )                // screen is ready?
        &&( SurvShowVehState       == FALSE ) )         // no vehicle state string active?
    {
        /* generate complete date/time string (21 chars!):
            DE: 'Mo 01.01.01  00:00:00'
            EN: 'Mo 01.01.01 00:00:00a' */
        TimeDate_GetString( RESENUM_WWDDMMYY,  szBuffer );
        strcpy(szTimeDate, szBuffer);
        strcat(szTimeDate, " ");
        TimeDate_GetString( RESENUM_HHMMSS,  szBuffer );// returns 7! chars!!!
        strcat(szTimeDate, szBuffer);
        Obj_TextSt_Show( &TimeDateTxtObj );
    }
}



/***********************************************************************
 *  FUNCTION:       MainDev_UpdMeas_Fuel
 *  DESCRIPTION:    Separate handling of FuelSensor data
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        We use the 'Fuel' & 'Arrow' icon for multiple
 *                  view mode, so we move them a little bit to correct
 *                  position.
 *
 *                  'GPIx.dwLHCounter' has been initilized by NVRAM value at init time
 *                  'NV_FuelSensImp' is implicitely saved back into NVRAM 
 *                  (for next init time)
 *
 *                  If FuelSensor is available and fuel is consumed - but
 *                  wheel stands still - we change the view from 
 *                  'l/100km' to 'l/Min' by simply exhanging the icons.
 *********************************************************************** */
void MainDev_UpdMeas_Fuel(void)
{           
    UINT32 dwConsAct_ml_100;    // Actuel  Fuel Consumption (ml/100km)
    UINT32 dwConsAvr_ml_100;    // Average Fuel Consumption (ml/100km)
    UINT32 dwFuelExh_ml;        // Fuel exhaustion (ml)
    UINT32 dwFuelRem_ml;        // Fuel Remaining (ml)
    UINT32 dwDistExh_km;        // Fuel Distance since last refuel (km)
    UINT32 dwDistRem_km;        // Fuel Distance until next refuel (km)
    BOOL   fSensorAvail;        // FuelSensor available (just a copy for easier use)
    UINT32 dwImpRate;           // FuelSensor Impulsrate (Impulses/Liter)
    UINT32 dwCapacity_ml;       // FuelCapacity (ml)
    UINT32 dwConsUsr_ml_100;    // User given Fuel Consumption (ml/100km)

    /* ============================================================= */    
    /* retrieve some local scaled copies of EEPROM/NVRAM values */
    /* ============================================================= */    
    fSensorAvail        =         EE_FuelSensCtrl.flags.FuelSAvail;
    dwImpRate           = (UINT32)EE_FuelSensCtrl.FuelSImpulseRate;    
    dwCapacity_ml       = (UINT32)EE_FuelCap      * DL2ML ;
    dwConsUsr_ml_100    = (UINT32)EE_FuelConsUser * DL2ML ;    
    NV_FuelSensImp      = DigInDrv_GPI_GetMeas(EE_FuelSensCtrl.flags.FuelSGPI)->dwLHCounter;

    /* ============================================================= */    
    /* retrieve DISTANCE SINCE LAST REFUELING */
    /* ============================================================= */    
    dwDistExh_km = Meas_Get_FuelDist(MR_KM_ONLY);

    /* ============================================================= */    
    /* calculate ABSOLUTE FUEL EXHAUSTION (dwFuelExh_ml, Milliliter):    
        Fuel(Exh, Liter) = SumOfImpulses (Imp) / Impulsrate (Imp/Liter) */       
    /* ============================================================= */            
    
    /* check: FuelSensor available? */
    if ( fSensorAvail == TRUE ) 
    {                                        
        /* convert FuelImpulses into exhausted fuel ('Liters.DeziLiters', prevent division by zero)*/
        if ( dwImpRate > 0 )
        {   dwFuelExh_ml = (LITER2ML * NV_FuelSensImp) / dwImpRate;
        }
        else
        {   dwFuelExh_ml = 0L;   // invalid user FuelSImpulseRate!
        }           
    }
    else
    {   /* NON-SENSOR-VERSION: calculate fuel exhaustion based on user given l/100 (prevent disivion by zero)*/ 
        if ( dwDistExh_km > 0 )
        {   dwFuelExh_ml = (dwConsUsr_ml_100 * dwDistExh_km) / 100L;        
        }
        else
        {   dwFuelExh_ml = 0L;   // not yet calculable!!
        }                   
    }
    
    /* ============================================================= */    
    /* calculate REMAINING FUEL dwFuelRem_ml (Milliliter, if valid parameters available)        
        Fuel(remain. Liter) = Fuel(Capacity, Liter) - Fuel(Exhaust, Liter) */        
    /* ============================================================= */      
          
    /* check: valid input data available? */
    if ( dwCapacity_ml > dwFuelExh_ml )     // assure a positive result!
    {   dwFuelRem_ml = dwCapacity_ml - dwFuelExh_ml;
    }
    else
    {   dwFuelRem_ml = 0L;           // not yet calculable!
    }

    /* ============================================================= */    
    /* calculate AVERAGE FUEL CONSUMPTION (ml/100km):    
        Fuel(ConsAverage, ml/100km) = 100 * Fuel(Exh, ml) / Dist(Exh, km) */
    /* ============================================================= */    
    
    /* check: FuelSensor available? */
    if ( fSensorAvail == TRUE ) 
    {    
        if (  ( dwDistExh_km > 0 )
            &&( dwFuelExh_ml  > 0 ) )
        {   dwConsAvr_ml_100 = (100L * dwFuelExh_ml) / dwDistExh_km;
        }
        else
        {   /* not yet calcuable => use user given default value */
            dwConsAvr_ml_100 = dwConsUsr_ml_100;      
        }                
    }
    else
    {   /* NON-SENSOR-VERSION: use user given fixed value instead */
        dwConsAvr_ml_100 = dwConsUsr_ml_100;      
    }                
        
    /* ============================================================= */    
    /* calculate REMAINING DISTANCE (km, if valid parameters available)   
        Dist(Remain) = 100 * Fuel(Remain, ml) / Fuel(ConsAverage, ml/100km) */
    /* ============================================================= */    
    
    /* check input parameters first */    
    if (  ( dwConsAvr_ml_100 > 0 )
        &&( dwFuelRem_ml  > 0 ) )
    {   dwDistRem_km = (100L * dwFuelRem_ml)  / dwConsAvr_ml_100;
    }
    else
    {   dwDistRem_km = 0;
    }                       

    /* ============================================================= */    
    /* Update display strings  */       
    
    /* check current display state */
    switch (MDObj.wDevState)
    {
        /*------------------------------------------- */
        /* update distance & liters SINCE LAST refuel */
        case MD_FUEL_FROM:
        {
            UINT8  bFuelExhaust_l;          // Fuel exhaustion - left comma part (liters only)
            UINT8  bFuelExhaust_dl;         // Fuel exhaustion - right comma part (deziliters only)                    
        
            // prepare parts to be displayed
            bFuelExhaust_l   = (UINT8)(  dwFuelExh_ml / LITER2ML );
            bFuelExhaust_dl  = (UINT8)(( dwFuelExh_ml - ((UINT32)bFuelExhaust_l * LITER2ML) )/100 );        

            /* prepare fuel-liters since last refueling */
            if ( bFuelExhaust_l <= 99 )
            {   sprintf( szFuelLiter, "%2u%c%1ul", bFuelExhaust_l, RESTXT_DEC_SEPARATOR, bFuelExhaust_dl );            
            }
            else
            {   sprintf( szFuelLiter, "%--%c-l", RESTXT_DEC_SEPARATOR );            
            }            

            /* calculate & prepare fuel-distance since last refueling */
            if ( dwDistExh_km <= 9999L )
            {   sprintf( szFuelDist,  "%4lu%s", dwDistExh_km, RESTXT_DIST_DESC );                        
            }
            else
            {   sprintf( szFuelDist,  " ---%s", RESTXT_DIST_DESC );                        
            }
            
            /* assure position of fuel icon & arrow right for this mode */
            FuelBmpObj.Org.wXPos        = 0;    // Fuel icon left
            FuelArrowRTxtObj.Org.wXPos  = 18;   // Arrow right of icon
        } break;                

        /*------------------------------------------- */
        /* update distance & liters UNTIL NEXT refuel */            
        case MD_FUEL_TO:
        {
            UINT8  bFuelRemain_l;           // Fuel Remaining - left comma part (liters only)
            UINT8  bFuelRemain_dl;          // Fuel Remaining - right comma part (deziliters only)            
            
            // prepare parts to be displayed
            bFuelRemain_l   = (UINT8)(  dwFuelRem_ml / LITER2ML );
            bFuelRemain_dl  = (UINT8)(( dwFuelRem_ml - ((UINT32)bFuelRemain_l * LITER2ML) )/100);        

            /* prepare fuel-liters since last refueling */
            if ( bFuelRemain_l <= 99 )
            {   sprintf( szFuelLiter, "%2u%c%1ul", bFuelRemain_l, RESTXT_DEC_SEPARATOR, bFuelRemain_dl );            
            }
            else
            {   sprintf( szFuelLiter, "%--%c-l", RESTXT_DEC_SEPARATOR );            
            }
            
            /* calculate & prepare fuel-distance since last refueling */
            if ( dwDistRem_km <= 9999 )
            {   sprintf( szFuelDist,  "%4lu%s", dwDistRem_km, RESTXT_DIST_DESC );                        
            }
            else
            {   sprintf( szFuelDist,  " ---%s", RESTXT_DIST_DESC );                        
            }
            
            /* update position of fuel icon & arrow */
            FuelArrowRTxtObj.Org.wXPos  = 0;            
            FuelBmpObj.Org.wXPos        = 8;                       
        } break;                
    
        /*------------------------------------------- */
        /* update fuel consumption (only if fuel sensor available) */            
        case MD_FUEL_CONS:
        {                          
            /* check: FuelSensor available? */
            if ( fSensorAvail == TRUE ) 
            {                          
                UINT8  bConsAvr_l_100;      // Average Fuel Consumption - left comma part (liters/100 km only)
                UINT8  bConsAvr_dl_100;     // Average Fuel Consumption - right comma part (dl/100 km only)                   
                UINT8  bConsAct_l_100;      // Actuel Fuel Consumption - left comma part (liters/100 km only)
                UINT8  bConsAct_dl_100;     // Actuel Fuel Consumption - right comma part (dl/100 km only)                   

                /* prepare average consumption */
                bConsAvr_l_100  = (UINT8)( dwConsAvr_ml_100 / LITER2ML );
                bConsAvr_dl_100 = (UINT8)( ( dwConsAvr_ml_100 - ((UINT32)bConsAvr_l_100 * LITER2ML)) / DL2ML );
                sprintf( szFuelConsAvr, "\xf8%2u%c%1u", bConsAvr_l_100, RESTXT_DEC_SEPARATOR, bConsAvr_dl_100 );                    
                
                /* prepare actual consumption */
                sprintf( szFuelConsAct, "--,-" );   // not yet used
                //sprintf( szFuelConsAct, "%2u%c%1u", bFuelCons_Liter, RESTXT_DEC_SEPARATOR, wFuelCons_ml );
                
                /* assure correct position of fuel icon (arrow-Object not in obj-list)*/
                FuelBmpObj.Org.wXPos = 0;                             
            }            
            else
            {   // MD_FUEL_CONS will not get called if 
                // (EE_FuelSensCtrl.flags.FuelSAvail == FALSE)
            }            
        } break;
        
        /*------------------------------------------- */
        /* any other Maindevice-State: nothing to do here ... */
        default: 
            break;    
    }
}




/***********************************************************************
 *  FUNCTION:       MainDev_ShowHorLine
 *  DESCRIPTION:    Draws a horizontal line between wheel speed
  *                 lower infos
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MainDev_ShowHorLine(void)
{
    /* horizontal line between wheel speed & rpm */
    DISPLXY Coord = {0,34};                 /* to be removed to an 'LineObject' !!! */
    DisplDrawHorLine(&Coord, 128, 0x03, DPLXOR);
}
