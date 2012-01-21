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
 * - Bitmap rgEmptySymbol16x16 to clear Error/Warning/Info icon
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
#include "beep.h"
#include "led.h"
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
    MD_FUEL,        /* show fuel distance */
    MD_TRIP1,       /* show TripCounter1 distance */
    MD_TRIP2,       /* show TripCounter2 distance */
    MD_VEHDIST,   /* show vehicle overall distance */
    MD_SPEEDMAX,    /* show Speed Max value */

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
extern UINT16           wMilliSecCounter;                   /* valid values: 0h .. ffffh */
extern UINT16           wSecCounter;                        /* valid values: 0h .. ffffh */
extern STRING far       szDevName[];                        /* device names */
extern DEVFLAGS1_TYPE   gDeviceFlags1;                      /* system parameters */
extern char             szSurvGlobalState[VEHSTATE_TXT_LEN];/* vehicle state string */



/* ============================================================= */
/* external Bitmap Raw data */


/* bitmaps for main selection lower area */
extern unsigned char far rgEnduroSymbol29x16[];     /* enduro symbol  */
extern unsigned char far rgRPMSymbo16x16[];         /* big RPM symbol */
extern unsigned char far rgFuelSymbo16x16[];        /* fuel symbol */

/* bitmaps for info/warning/error display */
extern unsigned char far rgInfoSymbol16x16[];       /* INFO symbol */
extern unsigned char far rgWarningSymbol16x16[];    /* WARNING symbol */
extern unsigned char far rgErrorSymbol16x16[];      /* ERROR symbol */
extern unsigned char far rgEmptySymbol16x16[];      /* EMPTY symbol (just to clear) */

/* bitmaps for monitor/measuremnt data  */
extern unsigned char far rgOilSymbol8x8[];          /* oil symbol */
extern unsigned char far rgWaterSymbol8x8[];        /* water symbol */
extern unsigned char far rgTempSymbol8x8[];         /* temperature symbol */
extern unsigned char far rgBatterySymbol8x8[];      /* battery symbol */
extern unsigned char far rgClockSymbol8x8[];        /* clock symbol */
extern unsigned char far rgRPMSymbol8x8[];          /* RPM symbol samll */
extern unsigned char far rgFuelSymbol8x8[];         /* fuel symbol small */
extern unsigned char far rgVertLine_2x20[];         /* vertical line to devide measurements */

/* bitmaps for gear display  */
extern unsigned char far rg7Seg_0_16x16[];          /* 7 segment bitmaps for gear display */
extern unsigned char far rg7Seg_1_16x16[];
extern unsigned char far rg7Seg_2_16x16[];
extern unsigned char far rg7Seg_3_16x16[];
extern unsigned char far rg7Seg_4_16x16[];
extern unsigned char far rg7Seg_5_16x16[];
extern unsigned char far rg7Seg_6_16x16[];
extern unsigned char far rg7Seg_7_16x16[];
extern unsigned char far rg7Seg_8_16x16[];
extern unsigned char far rg7Seg_9_16x16[];



/* =================================================== */
/* upper area: speed display */
/* =================================================== */

static TEXTOBJECT   SpeedTxtObj;                /* speed text '000' object */
static TEXTOBJECT   SpeedDescATxtObj;           /* speed descriptor 'km' (or 'mi' for EN) text object */
static TEXTOBJECT   SpeedDescBTxtObj;           /* speed descriptor 'h' text object */
static CHAR         szSpeed[4] = "0";           /* buffer for current speed, max. string '999' */


/* =================================================== */
/* lower area mode  */
/* =================================================== */

/* lower area mode 1: Rounds Per Minute */
static TEXTOBJECT   RPMTxtObj;                  /* rpm text '00000' object */
static TEXTOBJECT   RPMDescTxtObj;              /* rpm descriptor 'U/Min' (or RPM for DE) text object */
static CHAR         szRPM[6] = "0";             /* buffer current eng speed, max. string '13500' */
static BMPOBJECT    RPMBmpObj;                  /* symbol to indicate RPM */

/* lower area mode 2: Fuel Distance */
static TEXTOBJECT   FuelDistTxtObj;             /* fuel distance text object */
static CHAR         szFuelDist[10] = "0,0";     /* buffer to contain fuel distance, max. string '9999999,9' */
static BMPOBJECT    FuelDistBmpObj;             /* symbol to indicate fuel distance display mode */

/* lower area mode 3: Vehicle Distance */
static TEXTOBJECT   VehDistTxtObj;              /* vehicle distance text object */
static CHAR         szVehDist[10] = "0,0";      /* buffer to contain fuel distance, max. string '9999999,9' */
static BMPOBJECT    VehDistBmpObj;              /* symbol to indicate vehicle distance display mode */

/* lower area mode 4: TripCounter 1 Distance */
static TEXTOBJECT   Trip1DescTxtObj;            /* tripcounter1 descriptor text object 'T1' */
static TEXTOBJECT   Trip1DistTxtObj;            /* tripcounter1 distance text object */
static CHAR         szTrip1Dist[10] = "0,0";    /* buffer to contain tripcounter1 distance, max. string '9999,9' */

/* lower area mode 5: TripCounter 2 Distance */
static TEXTOBJECT   Trip2DescTxtObj;            /* tripcounter1 descriptor text object 'T2' */
static TEXTOBJECT   Trip2DistTxtObj;            /* tripcounter2 distance text object */
static CHAR         szTrip2Dist[10] = "0,0";    /* buffer to contain tripcounter2 distance, max. string '9999,9' */

/* lower area mode 2..5: common distance descriptor */
static TEXTOBJECT   DistDescTxtObj;             /* COMMON vehicle & fuel distance decriptor for 'km' or 'mi' */


/* lower area mode 6: Max Speed */
static TEXTOBJECT   SpeedMaxDescTxtObj;         /* SpeedMax descriptor text object for 'v(max)' */
static TEXTOBJECT   SpeedMaxUnitTxtObj;         /* speed max desciptor text object 'km/h' or 'mi/h' */
static TEXTOBJECT   SpeedMaxTxtObj;             /* SpeedMax descriptor text object for '110.0' */
static CHAR         szSpeedMax[4] = "  0";      /* buffer to contain SpeedMax, max. string '999' km/h*/
extern SPEED_TYPE   Speed_Max;                  /* prepared value */

/* lower area: Date & Time Display */
static TEXTOBJECT   TimeDateTxtObj;             /* time & date output opbject */
static CHAR         szTimeDate[22] = "Mo 01.01.01  00:00:00";   /* buffer for timedate string */


/* --------------------------------------------- */
/* upper area bitmap objects & vehicle state */
static BMPOBJECT    VehStateBmpObj;             /* symbol for current vehicle state */


/* --------------------------------------------- */
/* lower area vehicle state objects */
static TEXTOBJECT   SurvVehStateTxtObj;         /* Vehicle State detected by MonitorDevice */
extern INT8         SurvParamListCount;         /* number of states currently not ok */
static INT8         SurvShowVehState = 0;       /* != 0 if vehicle state is to be displayed */


/* --------------------------------------------- */
/* upper area gearbox state */
#if(GEARBOX==1)
static BMPOBJECT    GearSymbolBmpObj;           /* selected gear indicator */
#endif



/* =================================================== */
/* lower area mode 7: Monitor/measurement objects */
/* =================================================== */

/* measure data vertical devider */
static BMPOBJECT    MonVertLineBmpObj;

/* measure data 1: temperature from external air / internal NTC sensor */
static BMPOBJECT    MonAmbientTempBmpObj;
static TEXTOBJECT   MonAmbientTempTxtObj;
static TEXTOBJECT   MonAmbientTempDescTxtObj;
static CHAR         szTemp[5];

/* measure data 2: battery voltage */
static BMPOBJECT    MonVoltageBmpObj;
static TEXTOBJECT   MonVoltageTxtObj;
static TEXTOBJECT   MonVoltageDescTxtObj;
static CHAR         szVoltage[5];

/* measure data 3: motor oil temperature */
static TEXTOBJECT   MonOilTempTxtObj;
static TEXTOBJECT   MonOilTempDescTxtObj;
static BMPOBJECT    MonOilTempBmpObj;
static CHAR         szOilTemp[5];

/* measure data 4: motor RPM (used, if no oil temp sensor connected) */
static TEXTOBJECT   MonRPMTxtObj;
static TEXTOBJECT   MonRPMDescTxtObj;
static BMPOBJECT    MonRPMBmpObj;

/* measure data 5: water temperature */
static TEXTOBJECT   MonWaterTempTxtObj;
static TEXTOBJECT   MonWaterTempDescTxtObj;
static CHAR         szWaterTemp[5];
static BMPOBJECT    MonWaterTempBmpObj;

/* measure data 5: fuel distance (used, if water temp sensor not available) */
static TEXTOBJECT   MonFuelTxtObj;
static TEXTOBJECT   MonFuelDescTxtObj;
static BMPOBJECT    MonFuelBmpObj;




/* ============================================================= */
/* internal prototypes */
ERRCODE MainDeviceStateMachine      (MESSAGE Msg);
ERRCODE MainDeviceResetMsg          (MESSAGE Msg);
ERRCODE MainDeviceShowVehicStateMsg (MESSAGE Msg);
void    MainDeviceUpdateTimeDate    (void);
void    MainDeviceUpdateMeasValues  (void);
void    MainDeviceObjListInit       (void);
void    MainDeviceShowHorLine       (void);



/* ============================================================= */
/* bitmap object table of this device */
static const BMPOBJECT_INITTYPE BmpObjInit[] =
{
    /*                              x   y  w   h   raw data              mode     state */
    /* --------------------------- -- --- --- --- --------------------- -------- ----- */

    /* selected info icons */
    /* --------------------------- -- --- --- --- --------------------- -------- ----- */
    { &FuelDistBmpObj,              0, 38, 16, 16, rgFuelSymbo16x16,     DPLNORM, FALSE },
    { &VehDistBmpObj,               0, 38, 29, 16, rgEnduroSymbol29x16,  DPLNORM, FALSE },
    { &RPMBmpObj,                   0, 38, 16, 16, rgRPMSymbo16x16,      DPLNORM, FALSE },

    /* Gear Symbol */
    /* --------------------------- -- --- --- --- --------------------- -------- ----- */
    #if(GEARBOX==1)
    { &GearSymbolBmpObj,            0,  0, 16, 16, rg7Seg_0_16x16,       DPLNORM, FALSE },
    #endif

    /* error status icons */
    /* --------------------------- -- --- --- --- --------------------- -------- ----- */
    { &VehStateBmpObj,              1, 17, 16, 16, rgEmptySymbol16x16,   DPLNORM, FALSE },

    /* monitor symbols  */
    /* --------------------------- -- --- --- --- --------------------- -------- ----- */
    { &MonAmbientTempBmpObj,        2, 37,  8,  8, rgTempSymbol8x8,      DPLNORM, FALSE },
    { &MonWaterTempBmpObj,          2, 46,  8,  8, rgWaterSymbol8x8,     DPLNORM, FALSE },
    { &MonFuelBmpObj,               2, 46,  8,  8, rgFuelSymbol8x8,      DPLNORM, FALSE },
    { &MonVoltageBmpObj,           71, 37,  8,  8, rgBatterySymbol8x8,   DPLNORM, FALSE },
    { &MonOilTempBmpObj,           71, 46,  8,  8, rgOilSymbol8x8,       DPLNORM, FALSE },
    { &MonRPMBmpObj,               71, 46,  8,  8, rgRPMSymbol8x8,       DPLNORM, FALSE },
    { &MonVertLineBmpObj,          64, 36,  2, 24, rgVertLine_2x20,      DPLNORM, FALSE }
    /* --------------------------- -- --- --- --- --------------------- -------- ----- */
};
#define BMPOBJECTLISTSIZE   (sizeof(BmpObjInit)/sizeof(BMPOBJECT_INITTYPE))


/* ============================================================= */
/* text object table */
static const TEXTOBJECT_INITTYPE TextObjInit[] =
{
    /* BIG Vehicle Speed + Unit */
    /* pObject                  X    Y  Font            H  Width  Align     Format    string ptr            State      */
    /* ----------------------- ---- --- -------------- --- ----- --------- ---------- -----------------     ---------- */
    { &SpeedTxtObj,             28,  0, DPLFONT_24X32,  1,  3, TXT_RIGHT,  TXT_NORM, szSpeed,               OC_DISPL | OC_DYN   },
    { &SpeedDescATxtObj,       108,  1, DPLFONT_8X16,   1,  2, TXT_LEFT,   TXT_NORM | TXT_UNDERL, RESTXT_SPEEDA_DESC,  OC_DISPL },
    { &SpeedDescBTxtObj,       112, 17, DPLFONT_8X16,   1,  1, TXT_LEFT,   TXT_NORM, RESTXT_SPEEDB_DESC,    OC_DISPL            },

    /* Selected BIG Information */
    /* pObject                  X    Y  Font            H  Width  Align     Format    string ptr            State      */
    /* ----------------------- ---- --- -------------- --- ----- --------- ---------- -----------------     ---------- */
    { &RPMTxtObj,               22, 38, DPLFONT_14X16,  1,  5, TXT_RIGHT,  TXT_NORM, szRPM,                 OC_DISPL | OC_DYN   },
    { &RPMDescTxtObj,           96, 38, DPLFONT_8X16,   1,  5, TXT_LEFT,   TXT_NORM, RESTXT_RPM_DESC,       OC_DISPL            },
    { &DistDescTxtObj,         106, 38, DPLFONT_8X16,   1,  2, TXT_LEFT,   TXT_NORM, RESTXT_DIST_DESC,      OC_DISPL            },
    { &FuelDistTxtObj,          24, 38, DPLFONT_14X16,  1,  6, TXT_RIGHT,  TXT_NORM, szFuelDist,            OC_DISPL | OC_DYN   },
    { &VehDistTxtObj,           46, 38, DPLFONT_8X16,   1,  6, TXT_RIGHT,  TXT_NORM, szVehDist,             OC_DISPL | OC_DYN   },
    { &Trip1DescTxtObj,          4, 38, DPLFONT_8X16,   1,  2, TXT_LEFT,   TXT_NORM, RESTXT_TRIP1DESC,      OC_DISPL            },
    { &Trip1DistTxtObj,         24, 38, DPLFONT_14X16,  1,  6, TXT_RIGHT,  TXT_NORM, szTrip1Dist,           OC_DISPL | OC_DYN   },
    { &Trip2DescTxtObj,          4, 38, DPLFONT_8X16,   1,  2, TXT_LEFT,   TXT_NORM, RESTXT_TRIP2DESC,      OC_DISPL            },
    { &Trip2DistTxtObj,         24, 38, DPLFONT_14X16,  1,  6, TXT_RIGHT,  TXT_NORM, szTrip2Dist,           OC_DISPL | OC_DYN   },
    { &SpeedMaxDescTxtObj,       4, 38, DPLFONT_8X16,   1,  3, TXT_LEFT,   TXT_NORM, RESTXT_SPEEDMAX_DESC,  OC_DISPL            },
    { &SpeedMaxUnitTxtObj,      94, 38, DPLFONT_8X16,   1,  4, TXT_LEFT,   TXT_NORM, RESTXT_SPEEDMAX_UNIT,  OC_DISPL            },
    { &SpeedMaxTxtObj,          44, 38, DPLFONT_14X16,  1,  3, TXT_RIGHT,  TXT_NORM, szSpeedMax,            OC_DISPL | OC_DYN   },

    /* Monitor Device Information */
    /* pObject                  X    Y  Font            H  Width  Align     Format    string ptr            State      */
    /* ----------------------- ---- --- -------------- --- ----- --------- ---------- -----------------     ---------- */
    { &MonAmbientTempTxtObj,    18, 38, DPLFONT_6X8,    1,  4, TXT_RIGHT,  TXT_NORM, szTemp,                OC_DISPL | OC_DYN   },
    { &MonAmbientTempDescTxtObj,48, 38, DPLFONT_6X8,    1,  2, TXT_RIGHT,  TXT_NORM, RESTXT_DEGC_DESC,      OC_DISPL            },
    { &MonWaterTempTxtObj,      18, 47, DPLFONT_6X8,    1,  4, TXT_RIGHT,  TXT_NORM, szWaterTemp,           OC_DISPL | OC_DYN   },
    { &MonWaterTempDescTxtObj,  48, 47, DPLFONT_6X8,    1,  2, TXT_RIGHT,  TXT_NORM, RESTXT_DEGC_DESC,      OC_DISPL            },
    { &MonFuelTxtObj,           18, 47, DPLFONT_6X8,    1,  4, TXT_RIGHT,  TXT_NORM, szFuelDist,            OC_DISPL | OC_DYN   },
    { &MonFuelDescTxtObj,       48, 47, DPLFONT_6X8,    1,  2, TXT_RIGHT,  TXT_NORM, RESTXT_DIST_DESC,      OC_DISPL            },

    { &MonVoltageTxtObj,        90, 38, DPLFONT_6X8,    1,  4, TXT_RIGHT,  TXT_NORM, szVoltage,             OC_DISPL | OC_DYN   },
    { &MonVoltageDescTxtObj,   114, 38, DPLFONT_6X8,    1,  2, TXT_RIGHT,  TXT_NORM, RESTXT_VOLT_DESC,      OC_DISPL            },
    { &MonOilTempTxtObj,        90, 47, DPLFONT_6X8,    1,  4, TXT_RIGHT,  TXT_NORM, szOilTemp,             OC_DISPL | OC_DYN   },
    { &MonOilTempDescTxtObj,   114, 47, DPLFONT_6X8,    1,  2, TXT_RIGHT,  TXT_NORM, RESTXT_DEGC_DESC,      OC_DISPL            },
    { &MonRPMTxtObj,            96, 47, DPLFONT_6X8,    1,  5, TXT_RIGHT,  TXT_NORM, szRPM,                 OC_DISPL | OC_DYN   },
    { &MonRPMDescTxtObj,       114, 47, DPLFONT_6X8,    1,  2, TXT_RIGHT,  TXT_NORM, RESTXT_EMPTY_TXT,      OC_DISPL            },

    /* Time and Date OR Vehiclestate text (changed when KEY_OK is pressed) */
    /* pObject                  X    Y  Font            H  Width  Align     Format    string ptr            State      */
    /* ----------------------- ---- --- -------------- --- ----- --------- ---------- -----------------     ---------- */
    { &TimeDateTxtObj,           2, 56, DPLFONT_6X8,    1, 21, TXT_CENTER, TXT_NORM, szTimeDate,            OC_DISPL | OC_DYN   },
    { &SurvVehStateTxtObj,       2, 56, DPLFONT_6X8,    1, 21, TXT_CENTER, TXT_NORM, szSurvGlobalState,     OC_DISPL | OC_DYN   },
    /*------------------------ ---- --- -------------- --- ----- --------- ---------- -----------------     ---------           */
};
#define TEXTOBJECTLISTSIZE   (sizeof(TextObjInit)/sizeof(TEXTOBJECT_INITTYPE))



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
    (void far *) &MonVoltageDescTxtObj,
    (void far *) &MonVoltageBmpObj,
    (void far *) &MonAmbientTempTxtObj,
    (void far *) &MonAmbientTempDescTxtObj,
    (void far *) &MonAmbientTempBmpObj,
    (void far *) &MonWaterTempTxtObj,
    (void far *) &MonWaterTempDescTxtObj,
    (void far *) &MonWaterTempBmpObj,
    (void far *) &MonFuelTxtObj,
    (void far *) &MonFuelDescTxtObj,
    (void far *) &MonFuelBmpObj,
    (void far *) &MonOilTempBmpObj,
    (void far *) &MonOilTempTxtObj,
    (void far *) &MonOilTempDescTxtObj,
    (void far *) &MonRPMTxtObj,
    (void far *) &MonRPMDescTxtObj,
    (void far *) &MonRPMBmpObj,
    (void far *) &MonVertLineBmpObj
};
#define OBJLIST_MON_CNT (sizeof(ObjectList_Mon)/sizeof(OBJSTATE)/sizeof(void far *))


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
#define OBJLIST_RPM_CNT (sizeof(ObjectList_Rpm)/sizeof(OBJSTATE)/sizeof(void far *))


// ---------------------------------------------------------------------
// Main device State 'MD_FUEL' Settings - Screen Focus Order
static const void far * ObjectList_Fuel[] =
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

    // objects - shown in 'MD_FUEL' mode only
    (void far *) &FuelDistTxtObj,
    (void far *) &FuelDistBmpObj,
    (void far *) &DistDescTxtObj        // common for Veh/Fuel/Trip1/Trip2
};
#define OBJLIST_FUEL_CNT (sizeof(ObjectList_Fuel)/sizeof(OBJSTATE)/sizeof(void far *))


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
#define OBJLIST_TRIP1_CNT (sizeof(ObjectList_Trip1)/sizeof(OBJSTATE)/sizeof(void far *))


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
#define OBJLIST_TRIP2_CNT (sizeof(ObjectList_Trip2)/sizeof(OBJSTATE)/sizeof(void far *))


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
#define OBJLIST_VEHDIST_CNT (sizeof(ObjectList_VehDist)/sizeof(OBJSTATE)/sizeof(void far *))



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
#define OBJLIST_SPEEDMAX_CNT (sizeof(ObjectList_SpeedMax)/sizeof(OBJSTATE)/sizeof(void far *))


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
    (void far *) &MonVoltageDescTxtObj,
    (void far *) &MonVoltageBmpObj,

    (void far *) &MonAmbientTempTxtObj,
    (void far *) &MonAmbientTempDescTxtObj,
    (void far *) &MonAmbientTempBmpObj,

    (void far *) &MonWaterTempTxtObj,
    (void far *) &MonWaterTempDescTxtObj,
    (void far *) &MonWaterTempBmpObj,

    (void far *) &MonFuelTxtObj,
    (void far *) &MonFuelDescTxtObj,
    (void far *) &MonFuelBmpObj,

    (void far *) &MonOilTempBmpObj,
    (void far *) &MonOilTempTxtObj,
    (void far *) &MonOilTempDescTxtObj,
    (void far *) &MonRPMTxtObj,
    (void far *) &MonRPMDescTxtObj,
    (void far *) &MonRPMBmpObj,
    (void far *) &MonVertLineBmpObj,

    // common object for Veh/Fuel/Trip1/Trip2
    (void far *) &DistDescTxtObj,

    // objects - shown in MD_RPM mode only
    (void far *) &RPMTxtObj,
    (void far *) &RPMDescTxtObj,
    (void far *) &RPMBmpObj,

    // objects - shown in 'MD_FUEL' mode only
    (void far *) &FuelDistTxtObj,
    (void far *) &FuelDistBmpObj,

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
    (void far *) &SpeedMaxTxtObj

};
#define OBJECTLIST_SIZE   (sizeof(ObjectList)/sizeof(OBJSTATE)/sizeof(void far *))






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
    MDObj.eDevID       = DEVID_MAIN;
    MDObj.szDevName    = szDevName[DEVID_MAIN];
    MDObj.fFocused     = FALSE;
    MDObj.fScreenInit  = FALSE;

    /* special MDObj object lists control handling */
    MainDeviceObjListInit();

    /* error check: */
    if (  (gDeviceFlags1.flags.MainDevState == MD_FIRST)
        ||(gDeviceFlags1.flags.MainDevState >= MD_LAST ) )
    {   ODS1( DBG_SYS, DBG_ERROR, "MainDeviceInit(): Invalid MainDevState %u corrected!", gDeviceFlags1.flags.MainDevState );
        gDeviceFlags1.flags.MainDevState = MD_FIRST + 1;
    }
    MDObj.wDevState    = gDeviceFlags1.flags.MainDevState;

    /* initialize all objects of any type */
    DevObjInit( &MDObj, (void far *)TextObjInit,   TEXTOBJECTLISTSIZE,     OBJT_TXT  );
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
    ODS( DBG_SYS, DBG_INFO, "- MainDeviceInit() done!");
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       MainDeviceObjListInit
 *  DESCRIPTION:    Initialize special object control structure
 *                  to handle different screens
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MainDeviceObjListInit(void)
{
    // Setup screen object list: Monitor display settings
    MDCntrl.List[MD_MONITOR].ObjList       = ObjectList_Mon;
    MDCntrl.List[MD_MONITOR].ObjCount      = OBJLIST_MON_CNT;
    MDCntrl.List[MD_MONITOR].FirstSelObj   = DevObjGetFirstSelectable(&MDObj, ObjectList_Mon, OBJLIST_MON_CNT);
    MDCntrl.List[MD_MONITOR].LastSelObj    = DevObjGetLastSelectable (&MDObj, ObjectList_Mon, OBJLIST_MON_CNT);

    // Setup screen object list: RPM display settings
    MDCntrl.List[MD_RPM].ObjList       = ObjectList_Mon;
    MDCntrl.List[MD_RPM].ObjCount      = OBJLIST_RPM_CNT;
    MDCntrl.List[MD_RPM].FirstSelObj   = DevObjGetFirstSelectable(&MDObj, ObjectList_Rpm, OBJLIST_RPM_CNT);
    MDCntrl.List[MD_RPM].LastSelObj    = DevObjGetLastSelectable (&MDObj, ObjectList_Rpm, OBJLIST_RPM_CNT);

    // Setup screen object list: Fuel display settings
    MDCntrl.List[MD_FUEL].ObjList       = ObjectList_Mon;
    MDCntrl.List[MD_FUEL].ObjCount      = OBJLIST_FUEL_CNT;
    MDCntrl.List[MD_FUEL].FirstSelObj   = DevObjGetFirstSelectable(&MDObj, ObjectList_Fuel, OBJLIST_FUEL_CNT);
    MDCntrl.List[MD_FUEL].LastSelObj    = DevObjGetLastSelectable (&MDObj, ObjectList_Fuel, OBJLIST_FUEL_CNT);

    // Setup screen object list: Tripcounter1 display settings
    MDCntrl.List[MD_TRIP1].ObjList       = ObjectList_Mon;
    MDCntrl.List[MD_TRIP1].ObjCount      = OBJLIST_TRIP1_CNT;
    MDCntrl.List[MD_TRIP1].FirstSelObj   = DevObjGetFirstSelectable(&MDObj, ObjectList_Trip1, OBJLIST_TRIP1_CNT);
    MDCntrl.List[MD_TRIP1].LastSelObj    = DevObjGetLastSelectable (&MDObj, ObjectList_Trip1, OBJLIST_TRIP1_CNT);

    // Setup screen object list: Tripcounter2 display settings
    MDCntrl.List[MD_TRIP2].ObjList       = ObjectList_Mon;
    MDCntrl.List[MD_TRIP2].ObjCount      = OBJLIST_TRIP2_CNT;
    MDCntrl.List[MD_TRIP2].FirstSelObj   = DevObjGetFirstSelectable(&MDObj, ObjectList_Trip2, OBJLIST_TRIP2_CNT);
    MDCntrl.List[MD_TRIP2].LastSelObj    = DevObjGetLastSelectable (&MDObj, ObjectList_Trip2, OBJLIST_TRIP2_CNT);

    // Setup screen object list: Vehicle Distance display settings
    MDCntrl.List[MD_VEHDIST].ObjList       = ObjectList_Mon;
    MDCntrl.List[MD_VEHDIST].ObjCount      = OBJLIST_VEHDIST_CNT;
    MDCntrl.List[MD_VEHDIST].FirstSelObj   = DevObjGetFirstSelectable(&MDObj, ObjectList_Trip2, OBJLIST_VEHDIST_CNT);
    MDCntrl.List[MD_VEHDIST].LastSelObj    = DevObjGetLastSelectable (&MDObj, ObjectList_Trip2, OBJLIST_VEHDIST_CNT);

    // Setup screen object list: SpeedMax display settings
    MDCntrl.List[MD_SPEEDMAX].ObjList       = ObjectList_Mon;
    MDCntrl.List[MD_SPEEDMAX].ObjCount      = OBJLIST_SPEEDMAX_CNT;
    MDCntrl.List[MD_SPEEDMAX].FirstSelObj   = DevObjGetFirstSelectable(&MDObj, ObjectList_Trip2, OBJLIST_SPEEDMAX_CNT);
    MDCntrl.List[MD_SPEEDMAX].LastSelObj    = DevObjGetLastSelectable (&MDObj, ObjectList_Trip2, OBJLIST_SPEEDMAX_CNT);

    MDCntrl.eState = MD_MONITOR;    // initial value
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

    /* ================================================= */
    /* 'show' or 'clear' screen? */
    if (fShow == TRUE)
    {
        /* update of all displayable values */
        MainDeviceUpdateMeasValues();

        /* ------------------------------------------ */
        /* do we have to repaint all? */
        if (MDObj.fScreenInit == FALSE)
        {
            /* yes, repaint complete screen */

            /* Clear Screen & set new state */
            DisplClearScreen(0x0);
            MDObj.fScreenInit  = TRUE;

            /* horizontal line between wheel speed & rpm */
            MainDeviceShowHorLine();

            /* always show vehicle speed in upper display part */
            ObjTextShow( &SpeedTxtObj );
            ObjTextShow( &SpeedDescATxtObj );
            ObjTextShow( &SpeedDescBTxtObj );

            /* show lowest display line 'VehicleState' or 'TimeDate' */
            if (SurvShowVehState)
                 ObjTextShow( &SurvVehStateTxtObj );
            else MainDeviceUpdateTimeDate(); // initial display only!

            /* ---------------------------------------------------- */
            /* Info/Warning/Error Icon
               (Most important icon is dominant),
               just switch the Bitmap raw data to the adequate ressource */
            // default: empty symbol
            VehStateBmpObj.Data.fpucBitmap = rgEmptySymbol16x16;
            if ( SurvListGetCount( SURVST_ERR ) )
                VehStateBmpObj.Data.fpucBitmap = rgErrorSymbol16x16;
            else if ( SurvListGetCount( SURVST_WARN ) )
                VehStateBmpObj.Data.fpucBitmap = rgWarningSymbol16x16;
            else if ( SurvListGetCount( SURVST_INFO ) )
                VehStateBmpObj.Data.fpucBitmap = rgInfoSymbol16x16;
            // show selected symbol
            ObjBmpShow( &VehStateBmpObj );

            /* ---------------------------------------------------- */
            // selected gear */
            #if(GEARBOX==1)
            ObjBmpShow( &GearSymbolBmpObj );
            #endif

            /* ---------------------------------------------------- */
            /* select display mode: which data to be shown below speed? */
            switch (MDObj.wDevState)
            {
                case MD_RPM:
                    ObjTextShow( &RPMTxtObj );
                    ObjTextShow( &RPMDescTxtObj );
                    ObjBmpShow( &RPMBmpObj );
                break;
                case MD_FUEL:
                    ObjTextShow( &FuelDistTxtObj );
                    ObjTextShow( &DistDescTxtObj );
                    ObjBmpShow( &FuelDistBmpObj );
                break;
                case MD_VEHDIST:
                    ObjTextShow( &VehDistTxtObj );
                    ObjTextShow( &DistDescTxtObj );
                    ObjBmpShow( &VehDistBmpObj );
                break;
                case MD_TRIP1:
                    ObjTextShow( &Trip1DescTxtObj );
                    ObjTextShow( &Trip1DistTxtObj );
                    ObjTextShow( &DistDescTxtObj );
                break;
                case MD_TRIP2:
                    ObjTextShow( &Trip2DescTxtObj );
                    ObjTextShow( &Trip2DistTxtObj );
                    ObjTextShow( &DistDescTxtObj );
                break;
                case MD_SPEEDMAX:
                    ObjTextShow( &SpeedMaxDescTxtObj );
                    ObjTextShow( &SpeedMaxTxtObj );
                    ObjTextShow( &SpeedMaxUnitTxtObj );
                break;
                case MD_MONITOR:
                {
                    /* ---------------------------------------------------- */
                    // vertical divider line */
                    ObjBmpShow( &MonVertLineBmpObj );

                    /* ---------------------------------------------------- */
                    // show internal/external temperature
                    ObjBmpShow ( &MonAmbientTempBmpObj );
                    ObjTextShow( &MonAmbientTempTxtObj );
                    ObjTextShow( &MonAmbientTempDescTxtObj );

                    /* ---------------------------------------------------- */
                    // show supply voltage
                    ObjBmpShow ( &MonVoltageBmpObj );
                    ObjTextShow( &MonVoltageTxtObj );
                    ObjTextShow( &MonVoltageDescTxtObj );

                    /* ---------------------------------------------------- */
                    // Automatic Display Switch between WaterTemp / FuelDistance
                    // TBD: Will later be done via Settings, but at the moment we
                    //      use this so called 'Arnoldschen Elfenbeinturm' here! ;-)
                    // Water temp available?
                    if ( AnaInGetWatTemperature() > ANAIN_TEMP_SENSORDETECT )
                    {   ObjBmpShow ( &MonWaterTempBmpObj     );
                        ObjTextShow( &MonWaterTempTxtObj     );
                        ObjTextShow( &MonWaterTempDescTxtObj );
                    }
                    else /* display fuel distance if no water temp sensor connected */
                    {   ObjTextShow( &MonFuelTxtObj     );
                        ObjTextShow( &MonFuelDescTxtObj );
                        ObjBmpShow ( &MonFuelBmpObj     );
                    }

                    /* ---------------------------------------------------- */
                    // Automatic Display Switch between OilTemp / RPM
                    // TBD: Will later be done via Settings, but at the moment we
                    //      use this so called 'Arnoldschen Elfenbeinturm' here! ;-)
                    // Oil temp available?
                    if ( AnaInGetOilTemperature() > ANAIN_TEMP_SENSORDETECT )
                    {   ObjBmpShow ( &MonOilTempBmpObj );
                        ObjTextShow( &MonOilTempTxtObj );
                        ObjTextShow( &MonOilTempDescTxtObj );
                    }
                    else /* display RPM if no oil temp sensor connected */
                    {   ObjBmpShow ( &MonRPMBmpObj );
                        ObjTextShow( &MonRPMTxtObj );
                        ObjTextShow( &MonRPMDescTxtObj );
                    }
                } break;

                // case eMainClock:
                // break;
                default:
                    ODS1( DBG_SYS, DBG_ERROR,
                          "MainDeviceShow(): unknown state: %u", MDObj.wDevState);
            }
        }
        else
        /* ------------------------------------------ */
        {
            /* No, repaint only changed stuff */

            /* show vehicle speed */
            ObjTextShow( &SpeedTxtObj );

            /* show lowest display line 'VehicleState' or 'TimeDate' */
            if (SurvShowVehState)
                 ObjTextShow( &SurvVehStateTxtObj );
            // else MainDeviceUpdateTimeDate(); // NO UPDATE HERE, will be done by special update messages only!

            /* which icon has to be used: Info / Warning / Error ?
               (Most important icon is dominant),
               just switch the Bitmap raw data to the adequate ressource */
            VehStateBmpObj.Data.fpucBitmap = rgEmptySymbol16x16;    // default
            if ( SurvListGetCount( SURVST_ERR ) )
                VehStateBmpObj.Data.fpucBitmap = rgErrorSymbol16x16;
            else if ( SurvListGetCount( SURVST_WARN ) )
                VehStateBmpObj.Data.fpucBitmap = rgWarningSymbol16x16;
            else if ( SurvListGetCount( SURVST_INFO ) )
                VehStateBmpObj.Data.fpucBitmap = rgInfoSymbol16x16;
            ObjBmpShow( &VehStateBmpObj );

            // SIMULATION ONLY: selected gear - show all gears every second */
            #if(GEARBOX==1)
            switch ( wSecCounter % 10 )
            {   case 0: GearSymbolBmpObj.Data.fpucBitmap = rg7Seg_0_16x16; break;
                case 1: GearSymbolBmpObj.Data.fpucBitmap = rg7Seg_1_16x16; break;
                case 2: GearSymbolBmpObj.Data.fpucBitmap = rg7Seg_2_16x16; break;
                case 3: GearSymbolBmpObj.Data.fpucBitmap = rg7Seg_3_16x16; break;
                case 4: GearSymbolBmpObj.Data.fpucBitmap = rg7Seg_4_16x16; break;
                case 5: GearSymbolBmpObj.Data.fpucBitmap = rg7Seg_5_16x16; break;
                case 6: GearSymbolBmpObj.Data.fpucBitmap = rg7Seg_6_16x16; break;
                case 7: GearSymbolBmpObj.Data.fpucBitmap = rg7Seg_7_16x16; break;
                case 8: GearSymbolBmpObj.Data.fpucBitmap = rg7Seg_8_16x16; break;
                case 9: GearSymbolBmpObj.Data.fpucBitmap = rg7Seg_9_16x16; break;
            }
            ObjBmpShow( &GearSymbolBmpObj );
            #endif

            /* which data to be shown below speed? */
            switch (MDObj.wDevState)
            {
                // show bigger parts
                case MD_RPM:        ObjTextShow( &RPMTxtObj );          break;
                case MD_FUEL:       ObjTextShow( &FuelDistTxtObj );     break;
                case MD_TRIP1:      ObjTextShow( &Trip1DistTxtObj );    break;
                case MD_TRIP2:      ObjTextShow( &Trip2DistTxtObj );    break;
                case MD_VEHDIST:    ObjTextShow( &VehDistTxtObj );      break;
                case MD_SPEEDMAX:   ObjTextShow( &SpeedMaxTxtObj );     break;

                // special case: so small monitor information
                case MD_MONITOR:
                {
                    // show external air temp (if n.a.: internal device temp)
                    ObjTextShow( &MonAmbientTempTxtObj );

                    // show battery supply voltage
                    ObjTextShow( &MonVoltageTxtObj );

                    // Automatic Display Switch between WaterTemp / FuelDistance
                    // TBD: Will later be done via Settings, but at the moment we
                    //      use this so called 'Arnoldschen Elfenbeinturm' here! ;-)
                    // Water temp available?
                    if ( AnaInGetWatTemperature() > ANAIN_TEMP_SENSORDETECT )
                    {   /* might have been changed! */
                        ObjBmpShow ( &MonWaterTempBmpObj     );
                        ObjTextShow( &MonWaterTempTxtObj     );
                        ObjTextShow( &MonWaterTempDescTxtObj );
                    }
                    else
                    {   /* might have been changed! */
                        ObjTextShow( &MonFuelTxtObj     );
                        ObjTextShow( &MonFuelDescTxtObj );
                        ObjBmpShow ( &MonFuelBmpObj     );
                    }

                    // Automatic Display Switch between OilTemp / RPM
                    // TBD: Will later be done via Settings, but at the moment we
                    //      use this so called 'Arnoldschen Elfenbeinturm' here! ;-)
                    // Oil temp available?
                    if ( AnaInGetOilTemperature() > ANAIN_TEMP_SENSORDETECT )
                    {   /* might have been changed! */
                        ObjBmpShow ( &MonOilTempBmpObj     );
                        ObjTextShow( &MonOilTempTxtObj     );
                        ObjTextShow( &MonOilTempDescTxtObj );
                    }
                    else
                    {   /* might have been changed! */
                        ObjBmpShow ( &MonRPMBmpObj );
                        ObjTextShow( &MonRPMTxtObj );
                        ObjTextShow( &MonRPMDescTxtObj );
                    }

                } break;
                // case eMainClock:
                //      Clock & Date are always visible and therefore independent of state machine
                // break;
                default:
                    ODS1( DBG_SYS, DBG_ERROR,
                          "MainDeviceShow(): unknown state: %u", MDObj.wDevState);
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
            if ( MDObj.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_MAIN]);
                MSG_BUILD_SETFOCUS(NewMsg,DEVID_MAIN,MSG_SENDER_ID(GivenMsg));   /* build answer message */
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                     /* send answer message */
                MDObj.fFocused = FALSE;                                     /* clear our focus */
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
                MDObj.fFocused = TRUE;                             /* set our focus */
                MainDeviceShow(TRUE);                                   /* show our screen */
                gDeviceFlags1.flags.ActDevNr = DEVID_MAIN;               /* save device# for restore */
                SurvShowVehState = 0;                                   /* reset lower VehicleStateString state */
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
                    MDObj.fFocused = FALSE;                              /* clear our focus */
                    MainDeviceShow(FALSE);                                    /* clear our screen */
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_MAIN, DEVID_TRIPCOUNT);  /* next: TripCounter */
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_SCREEN_RFRSH:
                MainDeviceShow(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;

            /* special messages ------------ */

            /* special handling of vehicle state string in lowest display line */
            case MSG_VEHSTATE_SHOW:  // no break!
            case MSG_VEHSTATE_HIDE:
                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = MainDeviceShowVehicStateMsg(GivenMsg);
                break;

            /* trigger time / date screen update only */
            case MSG_SECOND_GONE:
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
    MESSAGE_ID  MsgId = MSG_ID(Msg);                        /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;

    /* -------------------------------------- */
    /* scroll up? */
    if (  (RValue == ERR_MSG_NOT_PROCESSED             )    /* still unpocessed */
        &&(MsgId  == MSG_KEY_UP                        )    /* [UP] */
        &&(MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED ) )  /* not released */
    {
        DevObjClearState(   &MDObj,                         /* reset states of all objects of this device */
                            MDObj.Objects.ObjList,
                            MDObj.Objects.ObjCount,
                            OS_DISPL | OS_SELECT | OS_EDIT );
        MDObj.wDevState--;                                  /* previous state */
        if (MDObj.wDevState == MD_FIRST)                    /* wrap around? */
            MDObj.wDevState = (MD_LAST-1);
        MDObj.fScreenInit   = FALSE;                        /* next time rebuild complete screen */
        MainDeviceShow(TRUE);                               /* rebuild screen right now */
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
        MDObj.wDevState++;                                  /* next state */
        if (MDObj.wDevState == MD_LAST)                     /* wrap around? */
            MDObj.wDevState = (MD_FIRST+1);
        MDObj.fScreenInit   = FALSE;                        /* next time rebuild complete screen */
        MainDeviceShow(TRUE);                               /* rebuild screen right now */
        RValue = ERR_MSG_PROCESSED;
        ODS1( DBG_SYS, DBG_INFO, "MainDevState: %u", MDObj.wDevState);
    }

    /* -------------------------------------- */
    /* save device state */
    gDeviceFlags1.flags.MainDevState = MDObj.wDevState;

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
    if (   ( MDObj.wDevState == MD_FUEL  )
         ||( MDObj.wDevState == MD_TRIP1 )
         ||( MDObj.wDevState == MD_TRIP2 )
         ||( MDObj.wDevState == MD_SPEEDMAX) )
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
            switch (MDObj.wDevState) /* set into memory */
            {
                case MD_FUEL:   MeasSetFuelDist( &Dist );  break;
                case MD_TRIP1:  MeasSetTripCnt( eTRIPC_C, &Dist ); break;
                case MD_TRIP2:  MeasSetTripCnt( eTRIPC_D, &Dist ); break;
                case MD_SPEEDMAX: Speed_Max = 0; break;
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
        MSG_BUILD_UINT8(NewMsg, MSG_VEHSTATE_SHOW, 0, 0, 0);
        MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);

        /* prepare delayed message to our self to HIDE vehicle state after 3 seconds */
        MSG_BUILD_UINT8(NewMsg, MSG_VEHSTATE_HIDE, 0, 0, 0);
        SetTimerMsg(NewMsg, SEC2TICKS(3));          /* send delayed message */
        ODS( DBG_SYS, DBG_INFO, "MainDev: [OK] short pressed!");
        RValue = ERR_MSG_PROCESSED;                 /* processed! */
    }

    /* Received special Message to SHOW Vehicle state? */
    else if ( MsgId == MSG_VEHSTATE_SHOW )
    {
        SurvShowVehState = 1;                       /* change view right now! */
        MainDeviceShow(TRUE);                       /* don't wait until REFRESH, show right now! */
        ODS( DBG_SYS, DBG_INFO, "MainDev: SHOW VehicleState!");
        RValue = ERR_MSG_PROCESSED;                 /* processed! */
    }

    /* Received special Message to HIDE Vehicle state? */
    else if ( MsgId == MSG_VEHSTATE_HIDE )
    {
        SurvShowVehState = 0;                       /* change view right now! */
        ODS( DBG_SYS, DBG_INFO, "MainDev: HIDE VehicleState!");
        RValue = ERR_MSG_PROCESSED;                 /* processed! */
    }
    else RValue = ERR_MSG_NOT_PROCESSED;

    return(RValue);
}



/***********************************************************************
 *  FUNCTION:       MainDeviceUpdateMeasValues
 *  DESCRIPTION:    Just Update/Refresh all values that might be
 *                  displayed inside the display
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MainDeviceUpdateMeasValues(void)
{
    UINT16  wWheelSpeed;

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
    if (  ( MDObj.fScreenInit == TRUE  )       // screen is ready?
        &&( SurvShowVehState       == FALSE ) )     // no vehicle state string active?
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



/***********************************************************************
 *  FUNCTION:       MainDeviceShowHorLine
 *  DESCRIPTION:    Draws a horizontal line between wheel speed
  *                 lower infos
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MainDeviceShowHorLine(void)
{
    /* horizontal line between wheel speed & rpm */
    DISPLXY Coord = {0,34};                 /* to be removed to an 'LineObject' !!! */
    DisplDrawHorLine(&Coord, 128, 0x03, DPLXOR);
}
