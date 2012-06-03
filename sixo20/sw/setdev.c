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
 *  Module:         settings device
 *  Purpose:        handles the settings screen device
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
 *      or try http:// www.sixo.de
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
 * Revision 3.35  2012/06/03 17:45:18  tuberkel
 * Updated API-Function-Name according to Modul-Name
 *
 * Revision 3.34  2012/05/31 20:48:22  tuberkel
 * Fuel-Tank & Consumption enabled
 *
 * Revision 3.33  2012/05/28 12:47:31  tuberkel
 * Corrections for renamed Eeprom/Nvram Variables
 *
 * Revision 3.32  2012/05/27 17:52:40  tuberkel
 * Corrections for renamed Eeprom/Nvram Variables
 *
 * Revision 3.31  2012/05/27 16:01:42  tuberkel
 * All Eeprom/Nvram Variables renamed
 *
 * Revision 3.30  2012/05/15 20:11:35  tuberkel
 * FuelSensor: BasicSettings enabled & ok (not yet displayed)
 *
 * Revision 3.29  2012/03/01 20:48:25  tuberkel
 * Extensions: all - except Coolride - disabled for next Release
 *
 * Revision 3.28  2012/02/27 23:05:46  tuberkel
 * - Compass: Information moved from EE_DevFlags_3  ==> EE_CompassCtrl
 * - CompassDrv corrected
 * - SysPar- API changed
 *
 * Revision 3.27  2012/02/27 20:46:50  tuberkel
 * - all Coolride GPIs/GPOs correctly set by Eeprom value
 *
 * Revision 3.26  2012/02/26 12:24:55  tuberkel
 * - moved all Eeprom Vlaues physically into 'sysparam' module
 *
 * Revision 3.25  2012/02/26 11:47:16  tuberkel
 * Objects Order reviewed
 *
 * Revision 3.24  2012/02/26 10:11:52  tuberkel
 * - All GearInfo Settings available
 *
 * Revision 3.23  2012/02/25 20:43:01  tuberkel
 * - All ChainOiler Settings available
 *
 * Revision 3.22  2012/02/25 20:14:35  tuberkel
 * - All FuelSensor Settings available
 * - Tab-order rearranged
 *
 * Revision 3.21  2012/02/25 17:25:05  tuberkel
 * All Coolride Settings available
 *
 * Revision 3.20  2012/02/25 16:48:30  tuberkel
 * 5 new Objects in ExtensionsScreen
 *
 * Revision 3.19  2012/02/21 22:01:56  tuberkel
 * Compass Control/Mode/Eeprom reveiwed/fixed
 *
 * Revision 3.18  2012/02/21 21:14:34  tuberkel
 * SourceCode reformated
 *
 * Revision 3.17  2012/02/21 20:58:15  tuberkel
 * all Setdevice ObjectNames reviewed
 *
 * Revision 3.16  2012/02/19 11:23:13  tuberkel
 * BugFix BOOL Obj - R-mode-Left aligned
 *
 * Revision 3.15  2012/02/15 07:32:43  tuberkel
 * Objects-API reviewed (no functional changes)
 *
 * Revision 3.14  2012/02/14 22:36:22  tuberkel
 * Functions reviewed
 *
 * Revision 3.13  2012/02/14 21:08:03  tuberkel
 * - #define COMPASS ==> COMPDRV
 * - Compass SystemParam moved from devFlags2 -> 3
 * - Settings 'Compass' ==> 'Extensions'
 * - all Compass-Display modules enabled by default
 *
 * Revision 3.12  2012/02/12 12:44:34  tuberkel
 * New: small manual to add new screens/objects
 *
 * Revision 3.11  2012/02/12 12:17:38  tuberkel
 * partial COMPASSDRV CompilerFix
 *
 * Revision 3.10  2012/02/11 12:21:45  tuberkel
 * dedicated COOLRIDE macros prepared & used
 *
 * Revision 3.9  2012/02/10 23:45:22  tuberkel
 * - Survelannce HeatGrip <Info> - if active
 * - Surveillance-API reviewed
 *
 * Revision 3.8  2012/02/08 03:55:24  tuberkel
 * KEY_TIMING parameter names reviewed/changed
 *
 * Revision 3.7  2012/02/06 20:54:14  tuberkel
 * Just renamed all 'Devices' function prefixes for better readability
 *
 * Revision 3.6  2012/01/21 19:52:49  tuberkel
 * Additional Logos for 'TeneristI and 'Coolride'
 *
 * Revision 3.5  2012/01/21 00:35:39  tuberkel
 * BugFix: BIKE_TYPE order now equals displayed list
 *
 * Revision 3.4  2012/01/16 20:00:09  tuberkel
 * V301-RC: LED-Dimmer & ScreenDump Option disabled
 *
 * Revision 3.3  2012/01/14 08:28:42  tuberkel
 * Message-IDs shortened / reviewed
 *
 * Revision 3.2  2011/05/29 21:00:22  tuberkel
 * - Compass Settings removable
 * - ScreenDump only if DEBUG active
 * - all Objects not ready set to 'not selectable'
 *
 * Revision 3.1  2011/05/29 12:43:19  tuberkel
 * BugFix gwWheelImpulse
 * - Typ korrgiert
 * - jetzt auch im Eeprom gesichert
 *
 * Revision 3.0  2010/11/07 09:04:45  tuberkel
 * V30 Preparations:
 * - Device/Object Handling completely revised & simplified
 * - Device-Substate (devided into multiple Subscreens)
 * - diverse new Parameters
 *
 * Revision 2.13  2009/07/22 12:45:08  tuberkel
 * Device Object Focus handling simplified:
 * - now used standard DevObjFocusSet() / DevObjFocusReset() functions
 *
 * Revision 2.12  2009/07/19 12:32:59  tuberkel
 * - ObjectInit reviewed
 *
 * Revision 2.11  2009/07/15 08:53:29  tuberkel
 * Just comments
 *
 * Revision 2.10  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.9  2009/07/08 21:41:17  tuberkel
 * All compiler defines reviewed:
 * - set to unique usage: set define to 0 or 1
 * - default values set, if not used
 * - see 'Project Editor' for details
 *
 * Revision 2.8  2009/06/24 21:12:27  tuberkel
 * CompilerFix: correct use of #define COMPASSDRV
 *
 * Revision 2.7  2009/06/21 21:23:18  tuberkel
 * Changes by AN and SCHW:
 * - new Compass layout inside TripCounter Module
 * - can be disabled via user settings 'Tripcounter/upperlower:
 * Bitcoded:
 * - bit0: LongDistance:     1=upside (like roadbook), 0=downside
 * - bit1: CompassShowHead: 1=show, 0=off
 * - bit2: CompassShowBar:   1=show, 0=off
 * CompassHeading ist shown in footer line
 * CompassCompassBar as graphic
 *
 * Revision 2.6  2009/04/20 18:26:04  tuberkel
 * Compiler Fix SetDev_StateMachine()
 *
 * Revision 2.5  2007/03/30 10:05:13  tuberkel
 * changed NumObj_WheelSize size 11 -> 13 chars
 *
 * Revision 2.4  2007/03/26 23:11:55  tuberkel
 * changed MOTOBAU version handling
 * - eBikeType -> #define
 *
 * Revision 2.3  2007/03/09 08:38:36  niezeithat
 * - Added additional Motorbike Version, Africatwin RD07, no Fule Level Sensors!
 *
 * Revision 2.2  2006/10/01 22:22:53  tuberkel
 * new: DigInDrv_FilterInit();
 * - essential: adapt vehicle specific digital filter settings too!
 *
 * Revision 2.1  2006/07/20 23:03:24  tuberkel
 * added F650 version
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.12  2006/02/18 14:27:13  Ralf
 * - adaption to BIKE_MOTOBAU
 * - system variables handling improved
 * - eeprom parameter for lapcounter and statistics screen new
 *
 ************************************************************************ */



/* =================================================================

    HOW TO INSERT A NEW SETUP SUB SCREEN:
    -------------------------------------

    1. Add new SetDevice state in SETDEV_STATE enum list
    2. Add new defintions of display objects (text/bool/select) in objects definition part
    3. Add new external data objects (e.g. SystemParameter) if needed
    4. Add new Screen Headline OBJ_STEXT   STxtObj_HL_xxx;
    5. Add new Ressource text string constants in 'resource_xx.h'
    6. Add new initial object data in static object array (e.g. STxtObj_InitList[], /bool/select)
    7. Add new defintion ObjectList_xxx,  for all the above objects part of that screen (e.g. ObjectList_Ext1[])
    8. Add this ObjectList_xxx to function SetDev_ObjListInit()
    9. Add new handler functions for these objects to SetDev_CheckChanges()
   10. Add new handler functions for these objects to SetDev_ValuesUpdate()
   11. Add new handler functions for these objects to SetDev_ValuesInit(void)

  ================================================================== */

// include
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
#include "displdrv.h"
#include "timedate.h"
#include "bitmaps.h"
#include "surveill.h"
#include "compassdrv.h"
#include "fonts.h"


// defines for device data


// ----------------------------------------------------------------
// SETTINGS DEVICE DATA
// ----------------------------------------------------------------
// special types to handle SDObj internal display states
//
// NOTE:    Supports completely different device appearance in order
//          to keep parameters together in different screens

// Sub-Screen indicator (current diplay state)
typedef enum
{                   // currently shows screen with
    SD_VEHIC,       //   vehicle setup
    SD_DEVICE,      //   device setup
    SD_OUT,         //   LED/LCD setup
    SD_EXT1,        //   Extensions1 setup

    SD_LAST         // INVALID LAST STATE!
} SETDEV_STATE;


// set device control structure (to handle ALL screens)
typedef struct
{   SETDEV_STATE    eState;         // Setdevice show state
    DEVOBJLIST      List[SD_LAST];  // array of lists of display objects
} SETDEV_CNTRL;


static DEVDATA          SDObj;              // settings device object itself
static SETDEV_CNTRL     SDCntrl;            // special set device show state control

// Static Headline Text Objects
static OBJ_STEXT        STxtObj_HL_Vehicle;
static OBJ_STEXT        STxtObj_HL_Device;
static OBJ_STEXT        STxtObj_HL_Out;
static OBJ_STEXT        STxtObj_HL_Ext1;



// ----------------------------------------------------------------
// externals
extern STRING far           szDevName[];        // device names
extern DEVFLAGS1_TYPE       EE_DevFlags_1;      // system parameters
extern unsigned char        szSWVersion[];      // formated sw version
extern UINT16               wMilliSecCounter;   // valid values: 0h .. ffffh


// ----------------------------------------------------------------
// universal buffers for all edit objects
static BOOL     fEditBuffer;                //  1 bit edit buffer
static UINT8    bEditBuffer;                //  8 bit edit buffer
static UINT16   wEditBuffer;                // 16 bit edit buffer
static UINT32   dwEditBuffer;               // 32 bit edit buffer





// ===================================================================
// VEHICLE-SETTINGS - OBJECTS
// ===================================================================

// ----------------------------------------------------------------
// BikeType:
extern BIKE_TYPE    EE_BikeType;            // BikeType (from eeprom)
static OBJ_SELECT   SlctObj_BikeType;       // BikeType - edit object
static const STRING pszSelectBike[RESTXT_SET_BIKE_CNT] =
                        {   RESTXT_SET_BIKE_STANDARD    ,
                            RESTXT_SET_BIKE_R100GS      ,
                            RESTXT_SET_BIKE_AFRICATWIN  ,
                            RESTXT_SET_BIKE_BAGHIRA     ,
                            RESTXT_SET_BIKE_F650        ,
                        };

// ------------------------------------------
// CylinderCorrectureFactor EE_CCF:
// Note: EE_CCF is a bitfield structure - can therefore NOT directly be adressed by edit objects,
//       So we need local copy 'CCFNom'/'CCFDenom' to compare/save changes!
extern CCF_TYPE     EE_CCF;                 // EE_CCF value (with Nom/Denom nibbles) from eeprom (can not be address
static UINT8        CCFNom;                 // copy of EE_CCF.Nom as reference for edit object
static UINT8        CCFDenom;               // copy of EE_CCF.Denom as reference for edit object
static OBJ_NUM      NumObj_CCFNom;          // nominator edit object
static OBJ_NUM      NumObj_CCFDenom;        // denominator edit object

// ------------------------------------------
// WheelSize / WheelImpulse objects
extern UINT16       EE_WheelSize;           // wheel size from eeprom in mm
extern UINT8        EE_WheelImpPRev;        // wheel impulse from eeprom
static OBJ_NUM      NumObj_WheelSize;       // wheel size edit object
static OBJ_NUM      NumObj_WheelImpPRev;    // wheel impulse edit object

// ------------------------------------------
// EngineRunTime objects:
extern TIME_TYPE_LD NV_EngRunTime_Srv;      // engine runtime in h - since last service (NVRAM, 4 bytes)
extern TIME_TYPE_LD NV_EngRunTime_All;      // engine runtime in h - overall (NVRAM, 4 bytes)
static OBJ_NUM      NumObj_EngRunSrv;       // EnginRunTime Service - edit object
static OBJ_NUM      NumObj_EngRunAll;       // EnginRunTime All     - edit object

// ------------------------------------------
// Vehicle / Service km objects:
// Note: 'VehicDist' is Interrupt-driven and API protected - can therefore NOT directly be adressed by edit objects,
//       So we need local copy 'VehicDist' to compare/save changes!
extern DIST_TYPE    EE_NextSrvKm;           // Next km-Service (from eeprom)
static UINT32       dwServKm;               // original vehicle distance in km only
static DIST_TYPE    VehicDist;              // VehicDist-Copy returned by Measurement API
static UINT32       dwVehicDist;            // VehicDist-Copy - in km only
static OBJ_NUM      NumObj_ServKm;          // Serv-km  - edit object
static OBJ_NUM      NumObj_VehicDist;       // Vehic-km - edit object

// ----------------------------------------------------------------
// TankCapacity / FuelConsumption objects:
extern UINT16       EE_FuelCap;             // FuelCapacity in 1/10 liters (from eeprom)
extern UINT8        EE_FuelConsUser;        // FuelConsumption in 1/10 l/100 km (from eeprom)
static OBJ_NUM      NumObj_TankCap;         // FuelCapacity    - edit object
static OBJ_NUM      NumObj_FuelCons;        // FuelConsumption - edit object

// ----------------------------------------------------------------
// BikeLogo (NOTE: All entries derived from 'LOGO_TYPE')
extern UINT8        EE_LogoDelay;            // StartLogoDelay (from eeprom)
extern UINT8        EE_LogoSelect;         // BikeLogo (from eeprom)
static OBJ_NUM      NumObj_LogoDelay;       // StartLogoDelay - edit object
static OBJ_SELECT   SlctObj_Logo;           // BikeLogo - edit object
static const STRING pszSelectLogo[RESTXT_SET_LOGO_CNT] =
                        {   RESTXT_SET_LOGO_NKDEV     ,
                            RESTXT_SET_LOGO_BMW       ,
                            RESTXT_SET_LOGO_HUSQV     ,
                            RESTXT_SET_LOGO_BAGHIRA   ,
                            RESTXT_SET_LOGO_SIXO      ,
                            RESTXT_SET_LOGO_AFRICATWIN,
                            RESTXT_SET_LOGO_BMW_1100GS,
                            RESTXT_SET_LOGO_KTM       ,
                            RESTXT_SET_LOGO_HUSQVRS   ,
                            RESTXT_SET_LOGO_HPN       ,
                            RESTXT_SET_LOGO_MOTOBAU   ,
                            RESTXT_SET_LOGO_VFORCE    ,
                            RESTXT_SET_LOGO_TENERISTI ,
                            RESTXT_SET_LOGO_COOLRIDE
                        };





// ===================================================================
// DISPLAY-SETTINGS - OBJECTS
// ===================================================================

// ----------------------------------------------------------------
// DisplayControl objects:
// Note: 'EE_DisplFlags' is a bitfield structure - can therefore NOT directly be adressed by edit objects,
//       So we need local copies to compare/save changes!
extern DPLFLAGS_TYPE  EE_DisplFlags;        // All Display Control Flags (from eeprom)

// ----------------------------------------------------------------
// LCD settings
static OBJ_STEXT    STxtObj_Lcd_Desc;       // static text 'LCD'
static OBJ_NUM      NumObj_BacklOL;         // LCD BacklightOnLevel - edit object
static UINT8        bBacklOnLevel;          // LCD BacklightOnLevel - local edit value
static OBJ_NUM      NumObj_BacklLvl;        // LCD BacklightLevel - edit object
static UINT8        bBacklLev;              // LCD BacklightLevel - local edit value
static OBJ_NUM      NumObj_ContrLvl;        // LCD DisplyContrast - edit object
static UINT8        bContrLev;              // LCD DisplyContrast - local edit value

// ----------------------------------------------------------------
// LED settings
static OBJ_STEXT    STxtObj_Led_Desc;       // static text 'LED'
static OBJ_NUM      NumObj_LEDDimm;         // LED Dimmer - edit object
static UINT8        bLEDDimmLev;            // LED Dimmer - local edit value
static OBJ_NUM      NumObj_RPMFlash;        // LED RPM-High-Flash - edit object,
extern RPM_TYPE     EE_RPM_Flash;              // LED RPM-High-Flash, 1 RPM/bit (from EEPROM)
static OBJ_SELECT   SlctObj_LEDWM;          // LED WarningMode - edit object
static BOOL         fLedWarnMode;           // LED WarningMode - copy eeprom value
static const STRING pszSelectLedWM[RESTXT_SET_LEDWM_CNT] =
                        {   RESTXT_SET_LEDWM_SIXO,
                            RESTXT_SET_LEDWM_STD
                        };





// ===================================================================
// DEVICE-SETTINGS - OBJECTS
// ===================================================================

// ----------------------------------------------------------------
// Device Flags 2+3:
// Note: 'EE_DevFlags_2/3' are a bitfield structure - can therefore NOT directly be adressed by edit objects,
//       So we need local copies to compare/save changes!
extern DEVFLAGS2_TYPE       EE_DevFlags_2;
extern DEVFLAGS3_TYPE       EE_DevFlags_3;

// ----------------------------------------------------------------
// Date objects
static DATE_TYPE    RTCDateCopy;            // buffer to get current RTC data
static UINT8        bDate;                  // copy of RTC day as reference
static UINT8        bMonth;                 // copy of RTC day as reference
static UINT8        bYear;                  // copy of RTC day as reference
static OBJ_NUM      NumObj_ClkDate;         // day   - edit object
static OBJ_NUM      NumObj_ClkMonth;        // month - edit object
static OBJ_NUM      NumObj_ClkYear;         // year  - edit object

// ----------------------------------------------------------------
// Time objects
static TIME_TYPE    RTCTimeCopy;            // buffer to get current RTC data
static UINT8        bHour;                  // copy of RTC hour as reference
static UINT8        bMin;                   // copy of RTC minute as reference
static UINT8        bSec;                   // copy of RTC second as reference
static OBJ_NUM      NumObj_ClkHour;         // hour    -  edit object
static OBJ_NUM      NumObj_ClkMin;          // minute  -  edit object
static OBJ_NUM      NumObj_ClkSec;          // seconds -  edit object

// ----------------------------------------------------------------
// ClockDriverCalibration '±168'
static OBJ_NUM      NumObj_ClkCal;          // ClockCalibration - edit object
static UINT8        bRTCCalib;              // buffer to get current RTC value
static UINT8        bClkCalib;              // copy of RTC clock calibration as reference

// ----------------------------------------------------------------
// DayLightSaving DLS
static OBJ_BOOL     BoolObj_DLSave;         // DaylightSaving - edit object
BOOL                fDaylightSaveOn;         // Copy of eeprom value.struct element

// ----------------------------------------------------------------
// Language Selection
static OBJ_SELECT   SlctObj_Lang;           // Language - edit object
extern UINT8        EE_LangSelect;          // Language (from eeprom)
static const STRING pszSelectLang[RESTXT_SET_LANG_CNT] =
                        {   RESTXT_SET_LANG_DE,
                            RESTXT_SET_LANG_EN,
                            RESTXT_SET_LANG_NL
                        };

// ----------------------------------------------------------------
// TripCounterDisplayMode (up/down)
static OBJ_SELECT   SlctObj_TripCntLUp;     // TripCounterDisplayMode - edit object
static BOOL         fTripCntLUp;            // TripCounterDisplayMode - copy of eeprom value
static const STRING pszSelectTrip[RESTXT_SET_TRIP_CNT] =
                        {   RESTXT_SET_TRIP_VST,
                            RESTXT_SET_TRIP_VSB
                        };

// ----------------------------------------------------------------
// SIxO fMetricKm (miles/km)
static BOOL         fMetricKm;                // fMetricKm - copy of eeprom value
static OBJ_SELECT   SlctObj_Metric;         // fMetricKm edit object
static const STRING pszSelectMetric[RESTXT_SET_METRIC_CNT] =
                        {   RESTXT_SET_METRIC_KM,
                            RESTXT_SET_METRIC_MILES
                        };

// ----------------------------------------------------------------
// Vehicle simulation object (on/off, from eeprom)
static OBJ_BOOL     BoolObj_VehSim;         // Simulation - edit object
BOOL                fVehicSimul;            // Simulation On/off switch (copy of eeprom)

// ----------------------------------------------------------------
// BMP-Hardcopy available via UART and HIGH_BEAM available object (on/off, from eeprom)
static OBJ_BOOL     BoolObj_ScrDmp;         // fHardcopyAvail - edit object
BOOL                fHardcopyAvail;         // fHardcopyAvail On/off switch (copy of eeprom)

// ----------------------------------------------------------------
// Beeper Control (taken from EE_DevFlags_2, on/off, from eeprom)
static OBJ_BOOL     BoolObj_BeepCtrl;       // Beeper available On/off - edit object
BOOL                fBeeperAvail;           // Beeper available On/off switch (copy of eeprom)

// ----------------------------------------------------------------
// Eeprom Reset object (on = reset)
static OBJ_BOOL     BoolObj_EeprRst;        // Eeprom Reset - edit Object
BOOL                fEepromReset;           // Eeprom Reset

// ----------------------------------------------------------------
// SIxO DebugOut Control daylight saving value (on/off, from eeprom)
static OBJ_NUM      NumObj_DbgOut;          // SIxO-DebugOut control - edit object
extern DBGFILT_TYPE EE_DbgFilter;           // SIxO-DebugOut control (level & module, from eeprom



// ===================================================================
// EXTENSIONS SETTINGS - OBJECTS
// ===================================================================

// ----------------------------------------------------------------
// COMPASS OBJECTS
extern COMPASSCNTL_TYPE     EE_CompassCtrl;     // Eeprom value
static UINT8                bCompassDispl;      // to support enum display mode
static OBJ_SELECT           SlctObj_CompassD;   // compass calibration state object
static const STRING pszSelectCompD[RESTXT_SET_COMPD_CNT] =
                        {   RESTXT_SET_COMPD_NA  ,
                            RESTXT_SET_COMPD_HD  ,
                            RESTXT_SET_COMPD_GR  ,
                            RESTXT_SET_COMPD_HDGR
                        };
static OBJ_SELECT   SlctObj_CompassC;           // compass display mode object
static UINT8        bCompassCal;                // to detect user pressed buttons/activity
static const STRING pszSelectCompC[RESTXT_SET_COMPC_CNT] =
                        {   RESTXT_SET_COMPC_NA   ,
                            RESTXT_SET_COMPC_WORK ,
                            RESTXT_SET_COMPC_HHOLD,
                            RESTXT_SET_COMPC_HTURN,
                            RESTXT_SET_COMPC_HSAVE,
                            RESTXT_SET_COMPC_VHOLD,
                            RESTXT_SET_COMPC_VTURN,
                            RESTXT_SET_COMPC_VSAVE
                        };
static OBJ_BOOL BoolObj_CompAvail;          // complete boolean Compass main switch Object
BOOL            fCompassAvail;                // local value

// ----------------------------------------------------------------
// COOLRIDE HEATGRIP OBJECTS
extern COOLRIDECNTRL_TYPE   EE_CoolrideCtrl; // Eeprom saved Coolride Control Flags
static OBJ_BOOL BoolObj_CoolrAvail;         // main switch to enable 'Coolride' extension
static OBJ_NUM  NumObj_CoolrIn;             // Number of SIxO-GPI - to measure PWM of heatgrip
static OBJ_NUM  NumObj_CoolrOut;            // Number of SIxO-GPO - to provide Key-Action to Coolride
BOOL            fCoolrideAvail;               // parts of the 'EE_CoolrideCtrl' structure
UINT8           gbCoolrGPI;
UINT8           gbCoolrGPO;

// ----------------------------------------------------------------
// FUEL SENSOR OBJECTS
extern FUELSCNTRL_TYPE EE_FuelSensCtrl;   // Eeprom saved Control flags
extern UINT32   NV_FuelSensImp;                // NVram Fuel sensor Impulses counter since last refuel 
static OBJ_BOOL BoolObj_FuelSAvail;         // main switch to enable 'FuelSensor' extension
static OBJ_NUM  NumObj_FuelSIn;             // Number of SIxO-GPI - to count impulses of fuel sensor
static OBJ_NUM  NumObj_FuelSImp;            // Number of Impulses/Liter of fuel sensor
BOOL            fFuelSensAvail;            // parts of the 'EE_FuelSensCtrl' structure
UINT8           gbFuelSGPI;                 // edit value for GPI
UINT16          gwFuelSImp;                 // edit value for Impulse/Litre

// ----------------------------------------------------------------
// CHAIN OILER OBJECTS
static OBJ_BOOL BoolObj_ChainOAvail;        // main switch to enable 'Chain Oiler' extension
static OBJ_NUM  NumObj_ChainOOut;           // Number of SIxO-GPO - to activate ChainOiler
static OBJ_NUM  NumObj_ChainODur;           // ChainOiler - duration of Oiler in sec. every n km
static OBJ_NUM  NumObj_ChainOkm;            // ChainOiler - km to reactivate forn sec.
BOOL            fChainOilAvail;
UINT8           gbChainOGPO;
UINT8           gbChainODur;
UINT8           gbChainOkm;

// ----------------------------------------------------------------
// GEARBOX OBJECTS
static OBJ_BOOL     BoolObj_GearIAvail;     // main switch to enable 'GearInfo' extension
static OBJ_SELECT   SlctObj_GearIMode;      // gearinfo - input mode
static const STRING pszSelectGearIM[RESTXT_SET_GEARIM_CNT] =
                        {   RESTXT_SET_GEARIM_NA   ,
                            RESTXT_SET_GEARIM_AUTO ,
                            RESTXT_SET_GEARIM_GPI
                        };
static UINT8        bGearIMode;
BOOL                fGearInfoAvail;


// ----------------------------------------------------------------
// GPS MOUSE OBJECTS
static OBJ_BOOL BoolObj_GPSAvail;
BOOL            fGPSAvail;







/* ----------------------------------------------------------- */
/* STATIC HEADLINE & OTHER TEXT OBJECTS */
/* ----------------------------------------------------------- */

static const OBJ_STEXT_INIT STxtObj_InitList[] =
{
    /* Headline Objects for all Screens: */
    /*pObject                X   Y   Font         H  Width  Align       Format      string ptr             State  */
    /*--------------------- ---- --- ------------ -- -----  ----------- ----------- ---------------------- --------- */
    {&STxtObj_HL_Vehicle,   C01, R1, DPLFONT_6X8, 1, 21,    TXT_CENTER, TXT_INVERS, RESTXT_SET_HL_VEHICLE, OC_DISPL},
    {&STxtObj_HL_Device,    C01, R1, DPLFONT_6X8, 1, 21,    TXT_CENTER, TXT_INVERS, RESTXT_SET_HL_DEVICE,  OC_DISPL},
    {&STxtObj_HL_Out,       C01, R1, DPLFONT_6X8, 1, 21,    TXT_CENTER, TXT_INVERS, RESTXT_SET_HL_DISPLAY, OC_DISPL},
    {&STxtObj_HL_Ext1,      C01, R1, DPLFONT_6X8, 1, 21,    TXT_CENTER, TXT_INVERS, RESTXT_SET_HL_EXTENS1, OC_DISPL},
  //{&STxtObj_HL_IOPorts,   C01, R1, DPLFONT_6X8, 1, 21,    TXT_CENTER, TXT_INVERS, RESTXT_SET_HL_IOPORTS, OC_DISPL},
  //{&STxtObj_HL_Warnings,  C01, R1, DPLFONT_6X8, 1, 21,    TXT_CENTER, TXT_INVERS, RESTXT_SET_HL_WARNINGS,OC_DISPL},

    /* Led/Lcd Descriptors */
    /*pObject                X   Y   Font         H  Width  Align       Format      string ptr             State  */
    /*--------------------- ---- --- ------------ -- -----  ----------- ----------- ---------------------- --------- */
    {&STxtObj_Lcd_Desc,     C01, R2, DPLFONT_6X8, 1, 4,     TXT_LEFT,   TXT_NORM,   RESTXT_SET_LCD,        OC_DISPL},
    {&STxtObj_Led_Desc,     C01, R5, DPLFONT_6X8, 1, 4,     TXT_LEFT,   TXT_NORM,   RESTXT_SET_LED,        OC_DISPL},
    /*--------------------- ---- --- ------------ -- -----  ----------- ----------- ---------------------- --------- */
};
#define STEXTOBJ_INITLISTSIZE   (sizeof(STxtObj_InitList)/sizeof(OBJ_STEXT_INIT))



/* ----------------------------------------------------------- */
/* BOOL OBJECTS INIT TABLE */
/* ----------------------------------------------------------- */

// DebugMode ==> ScreenDumps activable! (see table below)
#if(DEBUG==1)
    #define OC_SCREENDUMP_OPT  OC_DISPL | OC_SELECT | OC_EDIT
#else
    #define OC_SCREENDUMP_OPT  OC_DISPL
#endif

static const OBJ_BOOL_INIT BoolObj_InitList[] =
{   /*pObject                X    Y  Font         Width  Data                 EditBuffer    DescriptorL             DescriptorR              State      */
    /*-------------------- ---- ---- ------------ -----  -------------------- ------------- ----------------------- -----------------------  --------------------------------- */
    {&BoolObj_DLSave,      C16,  R3, DPLFONT_6X8,    6,  &fDaylightSaveOn,    &fEditBuffer, RESTXT_SET_RTC_DLS,     RESTXT_EMPTY_TXT,        OC_DISPL | OC_SELECT | OC_EDIT },
    {&BoolObj_BeepCtrl,    C16,  R4, DPLFONT_6X8,    6,  &fBeeperAvail,       &fEditBuffer, RESTXT_SET_BEEP_ON,     RESTXT_EMPTY_TXT,        OC_DISPL | OC_SELECT | OC_EDIT },
    {&BoolObj_VehSim,      C16,  R5, DPLFONT_6X8,    6,  &fVehicSimul,        &fEditBuffer, RESTXT_SET_VEHICSIM,    RESTXT_EMPTY_TXT,        OC_DISPL | OC_SELECT | OC_EDIT },
    {&BoolObj_ScrDmp,      C16,  R6, DPLFONT_6X8,    6,  &fHardcopyAvail,     &fEditBuffer, RESTXT_SET_HARDCOPY,    RESTXT_EMPTY_TXT,        OC_SCREENDUMP_OPT              },
    {&BoolObj_EeprRst,     C16,  R7, DPLFONT_6X8,    6,  &fEepromReset,       &fEditBuffer, RESTXT_SET_RESETEEPROM, RESTXT_EMPTY_TXT,        OC_DISPL | OC_SELECT | OC_EDIT },
    /*-------------------- ---- ---- ------------ -----  -------------------- ------------- ----------------------- -----------------------  --------------------------------- */
    {&BoolObj_CompAvail,   C01,  R2, DPLFONT_6X8,   10,  &fCompassAvail,      &fEditBuffer, RESTXT_EMPTY_TXT,       RESTXT_SET_COMPASS,      OC_DISPL | OC_SELECT  },
    {&BoolObj_CoolrAvail,  C01,  R3, DPLFONT_6X8,   14,  &fCoolrideAvail,     &fEditBuffer, RESTXT_EMPTY_TXT,       RESTXT_SET_COOLRIDE,     OC_DISPL | OC_SELECT | OC_EDIT },
    {&BoolObj_FuelSAvail,  C01,  R4, DPLFONT_6X8,   12,  &fFuelSensAvail,     &fEditBuffer, RESTXT_EMPTY_TXT,       RESTXT_SET_FUELSENSOR,   OC_DISPL | OC_SELECT | OC_EDIT },
    {&BoolObj_ChainOAvail, C01,  R5, DPLFONT_6X8,    8,  &fChainOilAvail,     &fEditBuffer, RESTXT_EMPTY_TXT,       RESTXT_SET_CHAINOILER,   OC_DISPL | OC_SELECT  },
    {&BoolObj_GearIAvail,  C01,  R6, DPLFONT_6X8,   21,  &fGearInfoAvail,     &fEditBuffer, RESTXT_EMPTY_TXT,       RESTXT_SET_GEARINFO,     OC_DISPL | OC_SELECT  },
    {&BoolObj_GPSAvail,    C01,  R7, DPLFONT_6X8,   21,  &fGPSAvail,          &fEditBuffer, RESTXT_EMPTY_TXT,       RESTXT_SET_GPS,          OC_DISPL | OC_SELECT  },
    /*-------------------- ---- ---- ------------ -----  -------------------- ------------- ----------------------- -----------------------  --------------------------------- */
};
#define BOOLOBJ_INITLISTSIZE   (sizeof(BoolObj_InitList)/sizeof(OBJ_BOOL_INIT))




/* ----------------------------------------------------------- */
/* SELECT OBJECTS INIT TABLE */
/* ----------------------------------------------------------- */
static const OBJ_SLCT_INIT SlctObj_InitList[] =
{
    /* fpObject           OrgX    OrgY  Font         Width pValue                        Max                    WorkValue       szDescr                      SelectTxtList     SelectTxtWidth           State*/
    /* ------------------ ------ ------ ------------ ----- ----------------------------- ------------------     --------------- ---------------------------- ----------------  ------------------------ --------------------------------- */
    { &SlctObj_BikeType,    C01,   R2,  DPLFONT_6X8,   12, (UINT8 far *)&EE_BikeType,    RESTXT_SET_BIKE_CNT,   &bEditBuffer,   RESTXT_SET_BIKE_DESC,        pszSelectBike,    RESTXT_SET_BIKE_WIDTH,   OC_DISPL | OC_SELECT | OC_EDIT   },
    /* ------------------ ------ ------ ------------ ----- ----------------------------- ------------------     --------------- ---------------------------- ----------------  ------------------------ --------------------------------- */
    { &SlctObj_TripCntLUp,  C01,   R4,  DPLFONT_6X8,   12, (UINT8 far *)&fTripCntLUp,    RESTXT_SET_TRIP_CNT,   &bEditBuffer,   RESTXT_SET_TRIP_DESC,        pszSelectTrip,    RESTXT_SET_TRIP_WIDTH,   OC_DISPL | OC_SELECT | OC_EDIT   },
    { &SlctObj_Lang,        C01,   R5,  DPLFONT_6X8,   12, (UINT8 far *)&EE_LangSelect,  RESTXT_SET_LANG_CNT,   &bEditBuffer,   RESTXT_SET_LANG_DESC,        pszSelectLang,    RESTXT_SET_LANG_WIDTH,   OC_DISPL                         },
    { &SlctObj_Metric,      C01,   R6,  DPLFONT_6X8,   12, (UINT8 far *)&fMetricKm,      RESTXT_SET_METRIC_CNT, &bEditBuffer,   RESTXT_SET_METRIC_DESC,      pszSelectMetric,  RESTXT_SET_METRIC_WIDTH, OC_DISPL                         },
    /* ------------------ ------ ------ ------------ ----- ----------------------------- ------------------     --------------- ---------------------------- ----------------  ------------------------ --------------------------------- */
    { &SlctObj_LEDWM,       C05,   R6,  DPLFONT_6X8,   17, (UINT8 far *)&fLedWarnMode,   RESTXT_SET_LEDWM_CNT,  &bEditBuffer,   RESTXT_SET_LEDWM_DESC,       pszSelectLedWM,   RESTXT_SET_LEDWM_WIDTH,  OC_DISPL | OC_SELECT | OC_EDIT   },
    /* ------------------ ------ ------ ------------ ----- ----------------------------- ------------------     --------------- ---------------------------- ----------------  ------------------------ --------------------------------- */
    { &SlctObj_Logo,        C01,   R7,  DPLFONT_6X8,   12, (UINT8 far *)&EE_LogoSelect,  RESTXT_SET_LOGO_CNT,   &bEditBuffer,   RESTXT_SET_LOGO_DESC,        pszSelectLogo,    RESTXT_SET_LOGO_WIDTH,   OC_DISPL | OC_SELECT | OC_EDIT   },
    /* ------------------ ------ ------ ------------ ----- ----------------------------- ------------------     --------------- ---------------------------- ----------------  ------------------------ --------------------------------- */
    { &SlctObj_CompassD,    C11,   R2,  DPLFONT_6X8,    5, (UINT8 far *)&bCompassDispl,  RESTXT_SET_COMPD_CNT,  &bEditBuffer,   RESTXT_SET_COMPD_DESC,       pszSelectCompD,   RESTXT_SET_COMPD_WIDTH,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &SlctObj_CompassC,    C17,   R2,  DPLFONT_6X8,    5, (UINT8 far *)&bCompassCal,    RESTXT_SET_COMPC_CNT,  &bEditBuffer,   RESTXT_SET_COMPC_DESC,       pszSelectCompC,   RESTXT_SET_COMPC_WIDTH,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &SlctObj_GearIMode,   C16,   R6,  DPLFONT_6X8,    6, (UINT8 far *)&bGearIMode,     RESTXT_SET_GEARIM_CNT, &bEditBuffer,   RESTXT_SET_GEARIM_DESC,      pszSelectGearIM,  RESTXT_SET_GEARIM_WIDTH, OC_DISPL | OC_SELECT | OC_EDIT   },
    /* ------------------ ------ ------ ------------ ----- ----------------------------- ------------------     --------------- ---------------------------- ----------------  ------------------------ --------------------------------- */

};
#define SLCTOBJ_INITLISTSIZE   (sizeof(SlctObj_InitList)/sizeof(OBJ_SLCT_INIT))



/* ----------------------------------------------------------- */
/* NUMBER INIT TABLE */
/* ----------------------------------------------------------- */
static const OBJ_NUM_INIT NumObj_InitList[] =
{
    /* VEHICLE SETTINGS */
    /* fpObject           OrgX    OrgY  Font         Width  pNumber                 pWorkNumber     Type   Min  Max    Step DplType  Mode     C  zDescr                      zUnit                       L   Capabilities                     */
    /* ------------------ ------ ------ ------------ ----- -----------------------  --------------- ------ ---- ------- --- -------  -------- - ---------------------------- --------------------------- -- ----------------------------------- */
    { &NumObj_CCFNom,       C15,   R2,  DPLFONT_6X8,     6, &CCFNom,                &bEditBuffer,   eUCHAR, 1L,     9L,  0L, eDez,   eColumn, 0, RESTXT_SET_CCFNOM_DESC,     RESTXT_SET_CCFNOM_UNIT,     1,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_CCFDenom,     C21,   R2,  DPLFONT_6X8,     1, &CCFDenom,              &bEditBuffer,   eUCHAR, 1L,     9L,  0L, eDez,   eColumn, 0, RESTXT_EMPTY_TXT,           RESTXT_EMPTY_TXT,           1,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_WheelSize,    C01,   R3,  DPLFONT_6X8,    12, &EE_WheelSize,          &wEditBuffer,   eUINT,  0L,  9999L,  0L, eDez,   eColumn, 0, RESTXT_SET_WHEELSIZE_DESC,  RESTXT_SET_WHEELSIZE_UNIT,  4,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_WheelImpPRev, C16,   R3,  DPLFONT_6X8,     6, &EE_WheelImpPRev,       &wEditBuffer,   eUCHAR, 1L,    99L,  0L, eDez,   eColumn, 0, RESTXT_SET_WHEELIMP_DESC,   RESTXT_EMPTY_TXT,           2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_VehicDist,    C01,   R4,  DPLFONT_6X8,    12, &dwVehicDist,           &dwEditBuffer,  eULONG, 0L,999999L,  0L, eDez,   eColumn, 0, RESTXT_SET_VEHICKM_DESC,    RESTXT_SET_VEHICKM_UNIT,    6,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_EngRunAll,    C16,   R4,  DPLFONT_6X8,     6, &NV_EngRunTime_All.wHour,&wEditBuffer,  eUINT,  0L, 65535L,  0L, eDez,   eColumn, 0, RESTXT_EMPTY_TXT,           RESTXT_SET_ERT_UNIT,        5,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_ServKm,       C01,   R5,  DPLFONT_6X8,    12, &dwServKm,              &dwEditBuffer,  eULONG, 0L,999999L,  0L, eDez,   eColumn, 0, RESTXT_SET_SERVKM_DESC,     RESTXT_SET_VEHICKM_UNIT,    6,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_EngRunSrv,    C16,   R5,  DPLFONT_6X8,     6, &NV_EngRunTime_Srv.wHour,&wEditBuffer,  eUINT,  0L, 65535L,  0L, eDez,   eColumn, 0, RESTXT_EMPTY_TXT,           RESTXT_SET_ERT_UNIT,        5,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_TankCap,      C01,   R6,  DPLFONT_6X8,    12, &EE_FuelCap,            &dwEditBuffer,  eUINT,  0L,   999L,  0L, eDez,   eColumn, 1, RESTXT_SET_TANKCAP_DESC,    RESTXT_SET_TANKCAP_UNIT,    4,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_FuelCons,     C15,   R6,  DPLFONT_6X8,     7, &EE_FuelConsUser,       &bEditBuffer,   eUCHAR, 0L,   255L,  0L, eDez,   eColumn, 1, RESTXT_EMPTY_TXT,           RESTXT_SET_FUELCONS_UNIT,   4,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_LogoDelay,    C18,   R7,  DPLFONT_6X8,     4, &EE_LogoDelay,          &bEditBuffer,   eUCHAR, 0L,    99L,  0L, eDez,   eColumn, 1, "",                         RESTXT_SET_LOGODELAY_UNIT,  3,  OC_DISPL | OC_SELECT | OC_EDIT   },

    /* DEVICE SETTINGS */
    /* fpObject           OrgX    OrgY  Font         Width  pNumber                 pWorkNumber     Type   Min  Max    Step DplType  Mode     C  zDescr                      zUnit                       L   Capabilities                     */
    /* ------------------ ------ ------ ------------ ----- -----------------------  --------------- ------ ---- ------- --- -------  -------- - ---------------------------- --------------------------- -- ----------------------------------- */
    { &NumObj_ClkHour,      C01,   R2,  DPLFONT_6X8,     8, &bHour,                 &bEditBuffer,   eUCHAR, 0L,    23L,  0L, eDez,   eColumn, 0, RESTXT_SET_RTC_TIME,        RESTXT_EMPTY_TXT,           2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_ClkMin,       C09,   R2,  DPLFONT_6X8,     3, &bMin,                  &bEditBuffer,   eUCHAR, 0L,    59L,  0L, eDez,   eColumn, 0, RESTXT_TIMESEPERATOR,       RESTXT_EMPTY_TXT,           2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_ClkSec,       C12,   R2,  DPLFONT_6X8,     3, &bSec,                  &bEditBuffer,   eUCHAR, 0L,    59L,  0L, eDez,   eColumn, 0, RESTXT_TIMESEPERATOR,       RESTXT_EMPTY_TXT,           2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_ClkCal,       C16,   R2,  DPLFONT_6X8,     6, &bClkCalib,             &bEditBuffer,   eUCHAR, 0L,   255L,  0L, eDez,   eColumn, 0, RESTXT_SET_RTC_CALIB,       RESTXT_EMPTY_TXT,           3,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_ClkDate,      C01,   R3,  DPLFONT_6X8,     8, &bDate,                 &bEditBuffer,   eUCHAR, 1L,    31L,  0L, eDez,   eColumn, 0, RESTXT_SET_RTC_DATE,        RESTXT_EMPTY_TXT,           2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_ClkMonth,     C09,   R3,  DPLFONT_6X8,     3, &bMonth,                &bEditBuffer,   eUCHAR, 1L,    12L,  0L, eDez,   eColumn, 0, RESTXT_DAYSEPERATOR,        RESTXT_EMPTY_TXT,           2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_ClkYear,      C12,   R3,  DPLFONT_6X8,     3, &bYear,                 &bEditBuffer,   eUCHAR, 0L,    99L,  0L, eDez,   eColumn, 0, RESTXT_DAYSEPERATOR,        RESTXT_EMPTY_TXT,           2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_DbgOut,       C01,   R7,  DPLFONT_6X8,    12, &EE_DbgFilter,          &bEditBuffer,   eUCHAR, 0L,   255L,  0L, eHex,   eColumn, 0, RESTXT_SET_DBGDESC,         RESTXT_EMPTY_TXT,           2,  OC_DISPL                         },

    /* LED/LCD SETTINGS */
    /* fpObject           OrgX    OrgY  Font         Width  pNumber                 pWorkNumber     Type   Min  Max    Step DplType  Mode     C  zDescr                      zUnit                       L   Capabilities                     */
    /* ------------------ ------ ------ ------------ ----- -----------------------  --------------- ------ ---- ------- --- -------  -------- - ---------------------------- --------------------------- -- ----------------------------------- */
    { &NumObj_BacklOL,      C05,   R2,  DPLFONT_6X8,    17, &bBacklOnLevel,         &bEditBuffer,   eUCHAR, 0L,     7L,  0L, eDez,   eColumn, 0, RESTXT_SET_LCD_BL_DESC,     RESTXT_EMPTY_TXT,           1,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_BacklLvl,     C05,   R3,  DPLFONT_6X8,    17, &bBacklLev,             &bEditBuffer,   eUCHAR, 0L,    63L,  1L, eDez,   eStep,   0, RESTXT_SET_LCD_BR_DESC,     RESTXT_EMPTY_TXT,           2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_ContrLvl,     C05,   R4,  DPLFONT_6X8,    17, &bContrLev,             &bEditBuffer,   eUCHAR, 0L,    63L,  1L, eDez,   eStep,   0, RESTXT_SET_LCD_CNT_DESC,    RESTXT_EMPTY_TXT,           2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_LEDDimm,      C05,   R5,  DPLFONT_6X8,    17, &bLEDDimmLev,           &bEditBuffer,   eUCHAR, 0L,     7L,  1L, eDez,   eStep,   0, RESTXT_SET_LED_DIM_DESC,    RESTXT_EMPTY_TXT,           1,  OC_DISPL | OC_SELECT             },
    { &NumObj_RPMFlash,     C05,   R7,  DPLFONT_6X8,    17, &EE_RPM_Flash,          &wEditBuffer,   eUINT,  0L, 30000L,  0L, eDez,   eColumn, 0, RESTXT_SET_RPMFL_DESC,      RESTXT_EMPTY_TXT,           5,  OC_DISPL | OC_SELECT | OC_EDIT   },

    /* EXTENSIONS SETTINGS */
    /* fpObject           OrgX    OrgY  Font         Width  pNumber                 pWorkNumber     Type   Min  Max    Step DplType  Mode     C  zDescr                      zUnit                       L   Capabilities                     */
    /* ------------------ ------ ------ ------------ ----- -----------------------  --------------- ------ ---- ------- --- -------  -------- - ---------------------------- --------------------------- -- ----------------------------------- */
    { &NumObj_CoolrOut,     C15,   R3,  DPLFONT_6X8,     3, &gbCoolrGPO,            &bEditBuffer,   eUCHAR, 0L,     1L,  0L, eDez,   eColumn, 0, RESTXT_SET_COOLR_OUT,       RESTXT_EMPTY_TXT,           1,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_CoolrIn,      C19,   R3,  DPLFONT_6X8,     3, &gbCoolrGPI,            &bEditBuffer,   eUCHAR, 0L,     3L,  0L, eDez,   eColumn, 0, RESTXT_SET_COOLR_IN,        RESTXT_EMPTY_TXT,           1,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_FuelSImp,     C10,   R4,  DPLFONT_6X8,     8, &gwFuelSImp,            &wEditBuffer,   eUINT,  0L, 64000L,  0L, eDez,   eColumn, 0, RESTXT_EMPTY_TXT,           RESTXT_SET_FUELS_IMP,       5,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_FuelSIn,      C19,   R4,  DPLFONT_6X8,     3, &gbFuelSGPI,            &bEditBuffer,   eUCHAR, 0L,     3L,  0L, eDez,   eColumn, 0, RESTXT_SET_FUELS_IN,        RESTXT_EMPTY_TXT,           1,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_ChainODur,    C09,   R5,  DPLFONT_6X8,     3, &gbChainODur,           &bEditBuffer,   eUCHAR, 0L,    99L,  0L, eDez,   eColumn, 0, RESTXT_EMPTY_TXT,           RESTXT_SET_CHAINO_DUR,      2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_ChainOkm,     C13,   R5,  DPLFONT_6X8,     5, &gbChainOkm,            &bEditBuffer,   eUCHAR, 0L,   999L,  0L, eDez,   eColumn, 0, RESTXT_EMPTY_TXT,           RESTXT_SET_CHAINO_KM,       3,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &NumObj_ChainOOut,    C19,   R5,  DPLFONT_6X8,     3, &gbChainOGPO,           &bEditBuffer,   eUCHAR, 0L,     1L,  0L, eDez,   eColumn, 0, RESTXT_SET_CHAINO_OUT,      RESTXT_EMPTY_TXT,           1,  OC_DISPL | OC_SELECT | OC_EDIT   },

    /* ------------------ ------ ------ ------------ ----- -----------------------  --------------- ------ ---- ------- --- ------- -------- - ------------------------ ----------------------- -- -------------------------------------------- */
};
#define NUMOBJ_INITLISTSIZE   (sizeof(NumObj_InitList)/sizeof(OBJ_NUM_INIT))




// ---------------------------------------------------------------------
// SCREEN FOCUS ORDER
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// Vehicle-Settings - Screen Focus Order
static const void far * ObjectList_Veh[] =
{
    (void far *) &STxtObj_HL_Vehicle,   // 0 - Headline (not selectable)
    (void far *) &SlctObj_BikeType,     // 01
    (void far *) &NumObj_CCFNom,        // 02
    (void far *) &NumObj_CCFDenom,      // 03
    (void far *) &NumObj_WheelSize,     // 04
    (void far *) &NumObj_WheelImpPRev, // 05
    (void far *) &NumObj_VehicDist,     // 06
    (void far *) &NumObj_EngRunAll,     // 07
    (void far *) &NumObj_ServKm,        // 08
    (void far *) &NumObj_EngRunSrv,     // 09
    (void far *) &NumObj_TankCap,       // 10
    (void far *) &NumObj_FuelCons,      // 11
    (void far *) &SlctObj_Logo,         // 12
    (void far *) &NumObj_LogoDelay      // 13

};
#define OBJLIST_VEH_CNT (sizeof(ObjectList_Veh)/sizeof(OBJ_STATE)/sizeof(void far *))


// -------------------------------------------------------------------------
// Device-Settings - Screen Focus Order
static const void far * ObjectList_Dev[] =
{
    (void far *) &STxtObj_HL_Device,    // 0 - Headline (not selectable)
    (void far *) &NumObj_ClkHour,       // 01
    (void far *) &NumObj_ClkMin,        // 02
    (void far *) &NumObj_ClkSec,        // 03
    (void far *) &NumObj_ClkCal,        // 04
    (void far *) &NumObj_ClkDate,       // 05
    (void far *) &NumObj_ClkMonth,      // 06
    (void far *) &NumObj_ClkYear,       // 07
    (void far *) &BoolObj_DLSave,       // 08
    (void far *) &SlctObj_TripCntLUp,   // 09
    (void far *) &BoolObj_BeepCtrl,     // 10
    (void far *) &SlctObj_Lang,         // 11
    (void far *) &BoolObj_VehSim,       // 12
    (void far *) &SlctObj_Metric,       // 13
    (void far *) &BoolObj_ScrDmp,       // 14
    (void far *) &NumObj_DbgOut,        // 15
    (void far *) &BoolObj_EeprRst,      // 16
};
#define OBJLIST_DEV_CNT (sizeof(ObjectList_Dev)/sizeof(OBJ_STATE)/sizeof(void far *))


// -------------------------------------------------------------------------
// LED/LCD Settings Screen Tab order
static const void far * ObjectList_Out[] =
{
    (void far *) &STxtObj_HL_Out,       // 0 - Headline (not selectable)
    (void far *) &STxtObj_Lcd_Desc,     // 1
    (void far *) &NumObj_BacklOL,       // 2
    (void far *) &NumObj_BacklLvl,      // 3
    (void far *) &NumObj_ContrLvl,      // 4
    (void far *) &STxtObj_Led_Desc,     // 5
    (void far *) &NumObj_LEDDimm,       // 6
    (void far *) &SlctObj_LEDWM,        // 7
    (void far *) &NumObj_RPMFlash,      // 8
};
#define OBJLIST_OUT_CNT   (sizeof(ObjectList_Out)/sizeof(OBJ_STATE)/sizeof(void far *))


// -------------------------------------------------------------------------
// Extensions Screen Tab Order
static const void far * ObjectList_Ext1[] =
{
    (void far *) &STxtObj_HL_Ext1,      // 0 - Headline (not selectable)
    (void far *) &BoolObj_CompAvail,    // 1
    (void far *) &SlctObj_CompassD,     // 2
    (void far *) &SlctObj_CompassC,     // 3
    (void far *) &BoolObj_CoolrAvail,   // 4
    (void far *) &NumObj_CoolrOut,
    (void far *) &NumObj_CoolrIn,
    (void far *) &BoolObj_FuelSAvail,   //
    (void far *) &NumObj_FuelSImp,
    (void far *) &NumObj_FuelSIn,
    (void far *) &BoolObj_ChainOAvail,  //
    (void far *) &NumObj_ChainODur,
    (void far *) &NumObj_ChainOkm,
    (void far *) &NumObj_ChainOOut,
    (void far *) &BoolObj_GearIAvail,   //
    (void far *) &SlctObj_GearIMode,
    (void far *) &BoolObj_GPSAvail      //
};
#define OBJLIST_EXT1_CNT (sizeof(ObjectList_Ext1)/sizeof(OBJ_STATE)/sizeof(void far *))






// ---------------------------------------------------------------------
// non public prototypes
void    SetDev_Show           (BOOL fShow);
ERRCODE SetDev_TryObjects     (MESSAGE GivenMsg);
void    SetDev_CheckChanges   (void);
void    SetDev_ValuesUpdate   (void);
void    SetDev_ValuesInit     (void);
void    SetDev_CheckState    (MESSAGE GivenMsg);
void    SetDev_ObjListInit    (void);






/***********************************************************************
 *  FUNCTION:       SetDev_Init
 *  DESCRIPTION:    all initial stuff for all objects
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        These initial settings are updated at any time
 *                  the values are edited/changed.
 *********************************************************************** */
ERRCODE SetDev_Init(void)
{
    int i;
    ERRCODE  RValue;

    /* device main data */
    SDObj.eDevID       = DEVID_SET;
    SDObj.szDevName    = szDevName[DEVID_SET];
    SDObj.fFocused     = FALSE;
    SDObj.fScreenInit  = FALSE;

    /* initialize all kind of object types */
    DevObjInit( &SDObj, (void far *)STxtObj_InitList,   STEXTOBJ_INITLISTSIZE,  OBJT_TXT   );
    DevObjInit( &SDObj, (void far *)NumObj_InitList,    NUMOBJ_INITLISTSIZE,    OBJT_ENUM  );
    DevObjInit( &SDObj, (void far *)SlctObj_InitList,   SLCTOBJ_INITLISTSIZE,   OBJT_SLCT  );
    DevObjInit( &SDObj, (void far *)BoolObj_InitList,   BOOLOBJ_INITLISTSIZE,   OBJT_EBOOL );

    /* special SDObj object lists control handling */
    SetDev_ObjListInit();

    /* initialize edit values */
    SetDev_ValuesInit();

    /* reset focus handling for current screen  */
    DevObjFocusReset(   &SDObj,
                        SDCntrl.List[SDCntrl.eState].ObjList,
                        SDCntrl.List[SDCntrl.eState].ObjCount );

    // return
    ODS( DBG_SYS, DBG_INFO, "- SetDev_Init() done!");
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       SetDev_Show
 *  DESCRIPTION:    update devices appearence (new/update/clear)
 *                  by calling Show-Fct. of all relevant objects
 *  PARAMETER:      BOOL    TRUE = show objects, FALSE = clear screen
 *  RETURN:         -
 *  COMMENT:        Some editor buffers directly access the original
 *                  value, so they do not have to be refreshed
 *********************************************************************** */
void SetDev_Show(BOOL fShow)
{
    MESSAGE NewMsg;
    UINT8   ShowMode;
    UINT8   i;

    // show mode of complete device
    if (fShow == TRUE)
    {
        // 'SHOW' SCREEN

        // update all object values
        SetDev_ValuesUpdate();

        // do we have to repaint all?
        if (SDObj.fScreenInit == FALSE)
        {   ShowMode = SHOW_ALL;                   // repaint all stuff
            SDObj.fScreenInit = TRUE;              // afterwards: initailized
        }
        else
        {   ShowMode = SHOW_EDIT | SHOW_CURSOR;    // repaint only potential changed stuff
        }

        // FOR DISPLAY OBJECTS TEST ONLY: clear & proof display mode of objects!
        //DisplClearScreen(0xaa);

        /* process complete (active) object to show all objects */
        DevObjShow( &SDObj,
                    SDCntrl.List[SDCntrl.eState].ObjList,
                    SDCntrl.List[SDCntrl.eState].ObjCount,
                    ShowMode );

    }
    else
    {
        // 'CLEAR' SCREEN
        DisplClearScreen(0x0);      // leave a clear screen for next device

        // reset states of all objects of this device
        // NOTE: Do NOT clear OS_SELECT here, object has to focused again
        //       after re-entering this device!
        DevObjClearState(   &SDObj,
                            SDCntrl.List[SDCntrl.eState].ObjList,
                            SDCntrl.List[SDCntrl.eState].ObjCount,
                            OS_DISPL | OS_EDIT );

        // set overall device state to 'not init'
        SDObj.fScreenInit  = FALSE;

    }
}



/***********************************************************************
 *  FUNCTION:       SetDev_MsgEntry
 *  DESCRIPTION:    Receive Message Handler called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE SetDev_MsgEntry(MESSAGE GivenMsg)
{
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID  MsgId;
    MESSAGE     NewMsg;


    // analyze message: it's for us?
    MsgId = MSG_ID(GivenMsg);                               // get message id
    switch (MsgId)
    {
        case MSG_GET_FOCUS:
        {
            // Some device want's to get the focus:
            // If we've currently got the focus, we'll
            // answer to SET his focus!
            if ( SDObj.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_SET]);
                MSG_BUILD_SETFOCUS(NewMsg,DEVID_SET,MSG_SENDER_ID(GivenMsg));   // build answer message
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);// send answer message
                SetDev_Show(FALSE);                         // clear our screen
                SDObj.fScreenInit = FALSE;                  // reset init state
                SDObj.fFocused    = FALSE;                  // clear our focus
                RValue = ERR_MSG_PROCESSED;
            }
        } break;
        case MSG_SET_FOCUS:
        {
            // messsage for us?
            if (MSG_RECEIVER_ID(GivenMsg) == DEVID_SET)
            {
                // Someone wants us to take the focus?
                // We assume, that nobody else has the focus
                // and we've got the the screen now for us!
                if (  (SDObj.fFocused        == TRUE      )
                    &&(MSG_RECEIVER_ID(GivenMsg) == DEVID_SET) )
                    ODS2(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but it already has focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_SET]);
                else /*
                    ODS2(   DBG_SYS, DBG_INFO,
                            "FOCUS: %s -> %s!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_SET]) */;
                SetDev_Show(TRUE);                          // show our screen immediatly
                SDObj.fScreenInit = TRUE;                   // reset init state
                SDObj.fFocused    = TRUE;                   // set our focus
                EE_DevFlags_1.flags.ActDevNr = DEVID_SET;   // save device# for restore
                RValue = ERR_MSG_PROCESSED;
             }
             else
             {
                if ( SDObj.fFocused == TRUE )
                {
                    // Some other device should be given the focus,
                    // BUT WE'VE GOT THE FOCUS!
                    // It should first ask US to give the focus! :-(
                    ODS3(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but %s had focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[MSG_RECEIVER_ID(GivenMsg)],
                            szDevName[DEVID_SET]);
                    SDObj.fFocused = FALSE;                 // loose our focus
                    ODS1(   DBG_SYS, DBG_WARNING,
                            "%s now loosing focus :-( ",
                            szDevName[DEVID_SET]);
                    RValue = ERR_MSG_NOT_PROCESSED;
                }
             }
        } break;
    }

    // ----------------------------------------------------------------
    // this part is only for FOCUSED time AND msg not already processed
    if (  (SDObj.fFocused == TRUE )
        &&(RValue == ERR_MSG_NOT_PROCESSED ) )
    {
        switch (MsgId)
        {
            case MSG_KEYS_PRESSED:
            case MSG_KEY_OK:
            case MSG_KEY_UP:
            case MSG_KEY_DOWN:
            case MSG_DPL_FLASH_ON:
            case MSG_DPL_FLASH_OFF:

                /* first, let one if the list objects try the msg */
                if( RValue == ERR_MSG_NOT_PROCESSED )
                {
                    /* try all objects of current list */
                    RValue = DevObjMsg( &SDObj,
                                        SDCntrl.List[SDCntrl.eState].ObjList,
                                        SDCntrl.List[SDCntrl.eState].ObjCount,
                                        GivenMsg);
                    /* Any change? check & execute & show it! */
                    if( RValue == ERR_MSG_PROCESSED )
                    {   SetDev_CheckChanges();
                        SetDev_Show(TRUE);
                    }
                }

                /* try to move focus to next object (if possible) */
                if( RValue == ERR_MSG_NOT_PROCESSED )
                {
                    /* try move focus through object list */
                    RValue = DevObjFocusSet( &SDObj,
                                             SDCntrl.List[SDCntrl.eState].ObjList,
                                             SDCntrl.List[SDCntrl.eState].ObjCount,
                                             GivenMsg);
                    /* Focus moved? => check to change to next/previous screen! */
                    if (RValue == ERR_MSG_PROCESSED)
                    {   SetDev_CheckState(GivenMsg);
                    }
                    /* show changes anyway */
                    SetDev_Show(TRUE);
                }

                /* try to give focus to next device */
                if (  (RValue == ERR_MSG_NOT_PROCESSED                    )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN))
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYTM_PRESSED_SHORT              ) )
                {
                    /* give focus immediatly to next screen */
                    SDObj.fScreenInit = FALSE;                  // reset init state
                    SDObj.fFocused    = FALSE;                  // clear our focus
                    SetDev_Show(FALSE);                         // clear our screen
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_SET, DEVID_INTRO);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_SCREEN_RFRSH:
                /* backlight automatic,
                   only if Backlight-Switch and -Level object not in edit mode */
                if (  (NumObj_BacklOL.State.bits.fEditActive    == FALSE)
                    &&(NumObj_BacklLvl.State.bits.fEditActive == FALSE) )
                    LCDDrvSetBacklightLevel(    DisplBacklightCheckOn(EE_DisplFlags.flags.BacklOnLevel),
                                                EE_DisplFlags.flags.BacklLev );
                SetDev_Show(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;
            default: return ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       SetDev_CheckChanges_Vehicle
 *  DESCRIPTION:    Subfunction of SetDev_CheckChanges() (see for details)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        WE HERE HANDLE ONLY THOSE DATA, WHICH CAN _NOT_DIRECTLY_
 *                  BE ACCESSED BY THE EDIT OBJECT VIA AN ADDRESS!
 *********************************************************************** */
void SetDev_CheckChanges_Vehicle( void )
{
    // ============================================================
    // VEHICLE SETTINGS
    // ============================================================
 
    // CCF value was changed? ----------------------
    if (  (EE_CCF.nibble.nom   != CCFNom  )
        ||(EE_CCF.nibble.denom != CCFDenom) )
    {   EE_CCF.nibble.nom   = CCFNom;           // save global -> auto eeprom update!
        EE_CCF.nibble.denom = CCFDenom;
    }

    // Service km changed? -------------------
    if( EE_NextSrvKm.km != dwServKm )
    {   EE_NextSrvKm.km  = dwServKm;            // give back km into dkm structure
    }

    // Vehicle Distance changed? -------------------
    if( VehicDist.km != dwVehicDist )
    {   VehicDist.km = dwVehicDist * 100L;      // give back km into dkm structure
        Meas_SetDist_Vehicle( &VehicDist );       // save into system variable -> auto eeprom update!
    }

    // RPM flash setting changed? ------------------
    // no check necessary, we directly access global data when editor finishes!

    // WHEEL SIZE setting changed? ------------------
    // no check necessary, we directly access global data when editor finishes!

    // Bike LOGO changed? ------------------
    // no check necessary, we directly access global data
}



/***********************************************************************
 *  FUNCTION:       SetDev_CheckChanges_Device
 *  DESCRIPTION:    Subfunction of SetDev_CheckChanges() (see for details)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        WE HERE HANDLE ONLY THOSE DATA, WHICH CAN _NOT_DIRECTLY_
 *                  BE ACCESSED BY THE EDIT OBJECT VIA AN ADDRESS!
 *********************************************************************** */
void SetDev_CheckChanges_Device( void )
{
    // ============================================================
    // DEVICE SETTINGS
    // ============================================================

    // Eeprom Reset required by user? -----------------
    if( fEepromReset == 1 )        // saved with changes?
    {
        fEepromReset  = 0;                 // clear for next use
        SetDev_Show(FALSE);                 // clear screen
        SysPar_SetDefaults(PARTIAL);        // reset ALL parameters
        Delay_ms(500);                      // wait, simulate a reset
        SetDev_Show(TRUE);                  // rebuild screen
    }

    // Beeper Usage was changed? -----------------
    if( EE_DevFlags_2.flags.fBeeperAvail != fBeeperAvail )         // saved with changes?
    {   EE_DevFlags_2.flags.fBeeperAvail  = fBeeperAvail;          // save global -> auto eeprom update!
    }

    // Daylight Saving was changed? -----------------
    // NOTE:    We do not change current time here! We interprete current time
    //          as being correct right now, independently of switching DLS on/off.
    //          BUT we update the USGAE of the CEST flag for further automatic checking.
    if( EE_DevFlags_2.flags.fDLSAuto != fDaylightSaveOn )// saved with changes?
    {
        EE_DevFlags_2.flags.fDLSAuto  = fDaylightSaveOn; // save global -> auto eeprom update!
        TimeDate_UpdateCEST();                                  // update update CEST state here too!
    }

    // VehicleSimulation was changed? -----------------
    if( EE_DevFlags_2.flags.fVehicSimul  != fVehicSimul )  // saved with changes?
    {   EE_DevFlags_2.flags.fVehicSimul   = fVehicSimul;   // save global -> auto eeprom update!
    }

    // fHardcopyAvail Usage was changed? -----------------
    if( EE_DevFlags_2.flags.fHardcopyAvail  != fHardcopyAvail )           // saved with changes?
    {   EE_DevFlags_2.flags.fHardcopyAvail   = fHardcopyAvail;            // save global -> auto eeprom update!
    }

    // TripCntFlag was changed? -----------------
    if( EE_DevFlags_2.flags.fTripCntLUp != fTripCntLUp )         // compare bits only
    {   EE_DevFlags_2.flags.fTripCntLUp  = fTripCntLUp;          // save global -> auto eeprom update!
    }

    // fMetricKm was changed? -----------------
    if( EE_DevFlags_2.flags.fMetricKm != fMetricKm )                     // compare bits only
    {   EE_DevFlags_2.flags.fMetricKm  = fMetricKm;                      // save global -> auto eeprom update!
    }
}



/***********************************************************************
 *  FUNCTION:       SetDev_CheckChanges_TimeDate
 *  DESCRIPTION:    Subfunction of SetDev_CheckChanges() (see for details)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        WE HERE HANDLE ONLY THOSE DATA, WHICH CAN _NOT_DIRECTLY_
 *                  BE ACCESSED BY THE EDIT OBJECT VIA AN ADDRESS!
 *********************************************************************** */
void SetDev_CheckChanges_TimeDate( void )
{
    // ============================================================
    // TIME/DATE SETTINGS
    // ============================================================

    // Any Date changed? -------------------------------
    //
    // NOTE: As all date settings use the same API function, so
    //       all date fields are to be handled together here!
    TimeDate_GetDate( &RTCDateCopy );                           // get date update
    if ( RTCDateCopy.bDate != bDate )                           // saved date with changes?
    {   RTCDateCopy.bDate = bDate;                              // copy changes
        TimeDate_SetDate( &RTCDateCopy );                       // save changes in RTC (and correct it)
        bDate = RTCDateCopy.bDate;                              // read back (corrected) value
    }
    else
    {   if ( RTCDateCopy.bMonth != bMonth )                     // OR saved month with changes?
        {   RTCDateCopy.bMonth = bMonth;                        // copy changes
            TimeDate_SetDate( &RTCDateCopy );                   // save changes in RTC (and correct it)
            bMonth = RTCDateCopy.bMonth;                        // read back (corrected) value
        }
        else
        {   if ( RTCDateCopy.bYear != bYear )                   // OR saved year with changes?
            {   RTCDateCopy.bYear = bYear;                      // copy changes
                TimeDate_SetDate( &RTCDateCopy );               // save changes in RTC (and correct it)
                bYear = RTCDateCopy.bYear;                      // read back (corrected) value
            }
        }
    }
    TimeDate_GetDate( &RTCDateCopy );                           // get current date to update display at any time
    bDate  = RTCDateCopy.bDate;
    bMonth = RTCDateCopy.bMonth;
    bYear  = RTCDateCopy.bYear;

    // Any Time changed? -------------------------------
    //
    // NOTE1: As all time settings use the same API function, so
    //        all time fields are to be handled together here!
    // NOTE2: To prevent detecting 'permanent changes' in time
    //        settings while refreshing values, comparisons are
    //        only done, while object is focused!
    // NOTE3: If DaylightSaving is enabled, we additonally check CET/CEST
    //        state too, but we don not change the hour value!
    TimeDate_GetTime( &RTCTimeCopy );                           // get time update to detect changes
    if (  (NumObj_ClkHour.State.bits.fSelected == TRUE)         // Hours focused?
        &&(RTCTimeCopy.bHour != bHour              ) )          // AND saved hours with changes?
    {   RTCTimeCopy.bHour = bHour;                              // copy changes
        TimeDate_SetTime( &RTCTimeCopy );                       // save changes in RTC (and correct it)
        bHour = RTCTimeCopy.bHour;                              // read back (corrected) value
        if( EE_DevFlags_2.flags.fDLSAuto == TRUE )      // DaylightSaving Automatic enabled?
        {   EE_DevFlags_2.flags.fDLSActive = TimeDate_GetCEST();// set CEST state too
        }
    }
    else
    {   if (  (NumObj_ClkMin.State.bits.fSelected == TRUE)      // Minutes focused?
            &&(RTCTimeCopy.bMin != bMin               ) )       // AND saved month with changes?
        {   RTCTimeCopy.bMin = bMin;                            // copy changes
            TimeDate_SetTime( &RTCTimeCopy );                   // save changes in RTC (and correct it)
            bMin = RTCTimeCopy.bMin;                            // read back (corrected) value
        }
        else
        {   if (  (NumObj_ClkSec.State.bits.fSelected == TRUE)  // Seconds focused?
                //&&(NumObj_ClkSec.State.bits.fEditActive == FALSE)  // AND not in edit mode?
                &&(RTCTimeCopy.bSec != bSec               ) )   // AND saved seconds with changes?
            {   RTCTimeCopy.bSec = bSec;                        // copy changes
                TimeDate_SetTime( &RTCTimeCopy );               // save changes in RTC (and correct it)
                bSec = RTCTimeCopy.bSec;                        // read back (corrected) value
            }
        }
    }
    // update date fields anyway
    TimeDate_GetTime( &RTCTimeCopy );                           // get current date to update display at any time
    bHour = RTCTimeCopy.bHour;
    bMin  = RTCTimeCopy.bMin;
    bSec  = RTCTimeCopy.bSec;

    // clock calibration changed? ------------------
    TimeDate_GetCalibDirect( &bRTCCalib );                      // get current calibration value
    if ( bRTCCalib != bClkCalib )                               // saved year with changes?
    {   bRTCCalib = bClkCalib;                                  // copy changes
        TimeDate_SetCalibDirect( &bClkCalib );                  // save changes in RTC
    }
}



/***********************************************************************
 *  FUNCTION:       SetDev_CheckChanges_LCDLED
 *  DESCRIPTION:    Subfunction of SetDev_CheckChanges() (see for details)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        WE HERE HANDLE ONLY THOSE DATA, WHICH CAN _NOT_DIRECTLY_
 *                  BE ACCESSED BY THE EDIT OBJECT VIA AN ADDRESS!
 *********************************************************************** */
void SetDev_CheckChanges_LCDLED( void )
{
    // ============================================================
    // LCD/LED SETTINGS
    // ============================================================

    // Backlight on/off and Level ------------------
    if( EE_DisplFlags.flags.BacklOnLevel != bBacklOnLevel )     // saved with changes?
    {   EE_DisplFlags.flags.BacklOnLevel = bBacklOnLevel;       // save global -> auto eeprom update!
    }

    // BacklightLevel was changed? -----------------
    if( EE_DisplFlags.flags.BacklLev != bBacklLev )             // saved with changes?
    {   EE_DisplFlags.flags.BacklLev = bBacklLev;               // save global -> auto eeprom update!
    }

    // NOTE: As BacklightSwitch and -Level are to be set in one
    //       API function, they are to be handled together here!
    // BacklightSwitch in edit mode? -----------
    if( NumObj_BacklOL.State.bits.fEditActive == TRUE )               // edit mode active?
    {   LCDDrvSetBacklightLevel( DisplBacklightCheckOn(bEditBuffer),  // use CURRENT EDIT VALUE of BacklightOnlevel!!!
                                 EE_DisplFlags.flags.BacklLev );
    }
    else
    {  // BacklightLevel in edit mode? -----------
       if( NumObj_BacklLvl.State.bits.fEditActive == TRUE )         // NO: edit mode level active?
       {    LCDDrvSetBacklightLevel(  DisplBacklightCheckOn(EE_DisplFlags.flags.BacklOnLevel),
                                      bEditBuffer );                // use CURRENT EDIT VALUE of BacklightLevel!!!
       }
       else
       {    // ELSE: Set current Backlight-Switch-Level with current backlight level
            LCDDrvSetBacklightLevel(  DisplBacklightCheckOn(EE_DisplFlags.flags.BacklOnLevel),
                                      EE_DisplFlags.flags.BacklLev ); //set global values
       }
    }

    // LED Warning Mode was changed? -----------------
    if( EE_DevFlags_2.flags.fLedWarnModeStd != fLedWarnMode )           // compare bits only
    {   EE_DevFlags_2.flags.fLedWarnModeStd  = fLedWarnMode;            // save global -> auto eeprom update!
    }

    // Display Contrast changed? --------------------
    if( EE_DisplFlags.flags.ContrLev != bContrLev ){            // saved with changes?
        EE_DisplFlags.flags.ContrLev = bContrLev;               // save global -> auto eeprom update!
    }

    // Display Contrast in edit mode? --------------
    if( NumObj_ContrLvl.State.bits.fEditActive == FALSE ){      // edit mode NOT active?
        LCDDrvSetContrastLevel( EE_DisplFlags.flags.ContrLev ); // set global contrast value
    }
    else{
        LCDDrvSetContrastLevel(bEditBuffer);                    // execute changes immediatly!
    }
}



/***********************************************************************
 *  FUNCTION:       SetDev_CheckChanges_Extension
 *  DESCRIPTION:    Subfunction of SetDev_CheckChanges() (see for details)
 *                  Used to detect any changes, once after EDIT mode was finished!
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        WE HERE HANDLE ONLY THOSE DATA, WHICH CAN _NOT_DIRECTLY_
 *                  BE ACCESSED BY THE EDIT OBJECT VIA AN ADDRESS!
 *********************************************************************** */
void SetDev_CheckChanges_Extension( void )
{
    // -------------------------------------------------------
    // COMPASS SETTINGS
    // -------------------------------------------------------

    // Compass Display Mode was changed ?
    if ( SlctObj_CompassD.State.bits.fEditActive == FALSE ) // edit mode NOT active?
    {   EE_CompassCtrl.flags.CompassDisplay = bCompassDispl;// just assure eeprom value equals local copy
    }

    // -------------------------------------------------------
    // Compass Calibration State in edit mode?
    if (SlctObj_CompassC.State.bits.fEditActive == TRUE )    // edit mode active?
    {
        if(bEditBuffer  < bCompassCal)          // decr. state NOT allowed! -> fix old value!
        {   bEditBuffer = bCompassCal;
        }
        else if(bEditBuffer > bCompassCal+1)    // incr. >1 NOT allowed! -> limit new value!
        {   bEditBuffer = bCompassCal+1;
        }
        else if (bEditBuffer != bCompassCal)    // still a user activity detected?
        {
            bCompassCal++;                      // should indicate the real compass driver calibration state
            switch(bCompassCal)
            {   case 0: ODS( DBG_SYS, DBG_INFO, "Waiting for Compass Calibration start.."); break;
                case 1: ODS( DBG_SYS, DBG_INFO, "Step 1: Hold compass horizontal and keep still!"); break;
                case 2: ODS( DBG_SYS, DBG_INFO, "Step 2: Multiple rotate compass horizontal!"); break;
                case 3: ODS( DBG_SYS, DBG_INFO, "Step 3: Save horizontal measurement!"); break;
                case 4: ODS( DBG_SYS, DBG_INFO, "Step 4: Hold compass vertical and keep still!"); break;
                case 5: ODS( DBG_SYS, DBG_INFO, "Step 5: Multiple rotate compass vertical!"); break;
                case 6: ODS( DBG_SYS, DBG_INFO, "Step 6: Save vertical measurement!"); break;
            }
            #if (COMPASSDRV==1)
            CompDrv_Cmd_IncCalState();          // ==> activate next calibration step!
            #endif
        }
        else
        {   // nothing changed ==> nothing to do!
        }
    }
    else    // currently not (no longer) in edit mode
    {   // check for changed value after editing finished with ESC / OK
        #if (COMPASSDRV==1)
        if (bCompassCal == 0)                   // user pressed ESC ?
        {   ODS( DBG_SYS, DBG_INFO, "Calibration aborted => Reset Calibration Mode!");
            CompDrv_Cmd_Reset();                // ==> reset calibration process!
        }
        if (bCompassCal < 6)                    // user OK before end 'state 6'?
        {   ODS( DBG_SYS, DBG_INFO, "Calibration not completed => Reset Calibration Mode!");
            CompDrv_Cmd_Reset();                // ==> reset calibration process!
        }
        else
        {   // success finished calibration state!
            ODS( DBG_SYS, DBG_INFO, "Calibration successfully completed!");
        }
        #endif
        bCompassCal = 0;                        // reset to default state
    }

    // -------------------------------------------------------
    // COOLRIDE HEATGRIP SETTINGS
    // -------------------------------------------------------

    // Coolride enable/disable changed?
    if ( BoolObj_CoolrAvail.State.bits.fEditActive == FALSE )   // edit mode NOT active?
    {   EE_CoolrideCtrl.flags.CoolrAvail = fCoolrideAvail;        // just assure eeprom value equals local copy
    }

    // Coolride GPO Settings changed?
    if ( NumObj_CoolrOut.State.bits.fEditActive == FALSE )      // edit mode NOT active?
    {   EE_CoolrideCtrl.flags.CoolrGPO = gbCoolrGPO;            // just assure eeprom value equals local copy
    }

    // Coolride GPI Settings changed?
    if ( NumObj_CoolrIn.State.bits.fEditActive == FALSE )       // edit mode NOT active?
    {   // check: GPI has been changed?
        if (EE_CoolrideCtrl.flags.CoolrGPI != gbCoolrGPI)
        {   // save new value & initialize new GPI with valid Coolride-PWM-Measurement settings
            EE_CoolrideCtrl.flags.CoolrGPI = gbCoolrGPI;
            DigInDrv_GPI_SetupMeas( EE_CoolrideCtrl.flags.CoolrGPI, COOLR_PWMIN_LOGIC, COOLR_PWMIN_TO );
        }
    }

    // -------------------------------------------------------
    // FUEL SENSOR SETTINGS
    // -------------------------------------------------------

    // FuelSensor enable/disable changed?
    if ( BoolObj_FuelSAvail.State.bits.fEditActive == FALSE )   // edit mode NOT active?
    {   EE_FuelSensCtrl.flags.FuelSAvail = fFuelSensAvail;     // just assure eeprom value equals local copy
    }

    // FuelSensor ImpulsRate changed?
    if ( NumObj_FuelSImp.State.bits.fEditActive == FALSE )      // edit mode NOT active?
    {   EE_FuelSensCtrl.FuelSImpulseRate = gwFuelSImp;          // just assure eeprom value equals local copy
    }

    // FuelSensor GPI Settings changed?
    if ( NumObj_FuelSIn.State.bits.fEditActive == FALSE )       // edit mode NOT active?
    {   // check: GPI has been changed?
        if (EE_FuelSensCtrl.flags.FuelSGPI != gbFuelSGPI );
        {   // save new value & initialize new GPI with valid FuelSensor-PWM-Measurement settings
            EE_FuelSensCtrl.flags.FuelSGPI = gbFuelSGPI;
            DigInDrv_GPI_SetupMeas( EE_FuelSensCtrl.flags.FuelSGPI, FUELS_PWMIN_LOGIC, FUELS_PWMIN_TO );
        }
    }

}



/***********************************************************************
 *  FUNCTION:       SetDev_CheckChanges
 *  DESCRIPTION:    Gets called, after OK button pressed:
 *                  Compares all screen object data for any change.
 *                  If any, the global data sources will be updated too
 *                  and the changed value will be executed immedeatly.
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        WE HERE HANDLE ONLY THOSE DATA, WHICH CAN _NOT_DIRECTLY_
 *                  BE ACCESSED BY THE EDIT OBJECT VIA AN ADDRESS!
 *
 *                  Special handling: Some changed values will be executed
 *                  while edit mode is STILL ACTIVE (like Contrast Level),
 *                  in order to give the user the chance to 'view' what he
 *                  is doing!
 *
 *                  Note that file-local-copies of global data (e.g.
 *                  local 'CCFNom' of 'CCF.nibble.nom' are changed by
 *                  edit object only if 'Save' button was pressed! So,
 *                  while edit process is still active, these local values
 *                  won't change!
 *
 *                  BUT: If you like to track the current(!) edit value
 *                  while edit mode is active, you should use the EDIT
 *                  BUFFER value instead! (e.g. DisplayContrast)
 *
 *********************************************************************** */
void SetDev_CheckChanges( void )
{
    SetDev_CheckChanges_Vehicle();          // VEHICLE SETTINGS
    SetDev_CheckChanges_Device();           // DEVICE SETTINGS
    SetDev_CheckChanges_TimeDate();         // TIME/DATE SETTINGS
    SetDev_CheckChanges_LCDLED();           // LCD/LED SETTINGS
    SetDev_CheckChanges_Extension();        // EXTENSIONS SETTINGS
}




/***********************************************************************
 *  FUNCTION:       SetDev_ValuesUpdate
 *  DESCRIPTION:    Updates all Setdevice Objects in order to be
 *                  shown on screen immediatly.
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        We here handle only those data, which can not directly
 *                  be accessed by the edit object via an address.
 *
 *                  Special handling: Some changed values will be executed
 *                  while edit mode is STILL ACTIVE (like Contrast Level),
 *                  in order to give the user the chance to 'view' what he
 *                  is doing!
 *
 *                  Note that file-local-copies of global data (e.g.
 *                  local 'CCFNom' of 'CCF.nibble.nom') are changed by
 *                  edit object only if 'Save' button was pressed! So,
 *                  while edit process is still active, these local values
 *                  won't change!
 *
 *                  BUT: If you like to track the current(!) edit value
 *                  while edit mode is active, you should use the EDIT
 *                  BUFFER value instead! (e.g. DisplayContrast)
 *********************************************************************** */
void SetDev_ValuesUpdate(void)
{

    // REFRESH ALL DYNAMIC VALUES - ONLY IF NOT IN EDIT MODE

    // ============================================================
    // VEHICLE SETTINGS
    // ============================================================

    // Cylinder Correctur Factor
    if (NumObj_CCFNom.State.bits.fEditActive == FALSE)
        CCFNom = EE_CCF.nibble.nom;
    if (NumObj_CCFDenom.State.bits.fEditActive == FALSE)
        CCFDenom = EE_CCF.nibble.denom;

    // vehicle km
    if (NumObj_VehicDist.State.bits.fEditActive == FALSE)
    {   VehicDist = Meas_GetDist_Vehicle(MR_KM);          // get fresh value
        dwVehicDist = VehicDist.km;                      // get km only
    }

    // service km
    if (NumObj_ServKm.State.bits.fEditActive == FALSE)
    {   dwServKm = EE_NextSrvKm.km;                         // get km only
    }

    // ============================================================
    // DEVICE SETTINGS
    // ============================================================

    // tripcounter state
    if (SlctObj_TripCntLUp.State.bits.fEditActive == FALSE)
    {   fTripCntLUp = EE_DevFlags_2.flags.fTripCntLUp;
    }

    // metric state
    if (SlctObj_Metric.State.bits.fEditActive == FALSE)
    {   fMetricKm = EE_DevFlags_2.flags.fMetricKm;
    }

    // BeeperUsage
    if( BoolObj_BeepCtrl.State.bits.fEditActive == FALSE)
    {   fBeeperAvail = EE_DevFlags_2.flags.fBeeperAvail;
    }

    // DaylightSaving Usage
    if( BoolObj_DLSave.State.bits.fEditActive == FALSE)
    {   fDaylightSaveOn = EE_DevFlags_2.flags.fDLSAuto;
    }

    // Vehicle Simulation
    if( BoolObj_VehSim.State.bits.fEditActive == FALSE)
    {   fVehicSimul = EE_DevFlags_2.flags.fVehicSimul;
    }

    // fHardcopyAvail Usage
    if( BoolObj_ScrDmp.State.bits.fEditActive == FALSE)
    {   fHardcopyAvail = EE_DevFlags_2.flags.fHardcopyAvail;
    }

    // ============================================================
    // TIME/DATE SETTINGS
    // ============================================================

    // date
    TimeDate_GetDate( &RTCDateCopy );
    if (NumObj_ClkDate.State.bits.fEditActive == FALSE)
        bDate = RTCDateCopy.bDate;
    if (NumObj_ClkMonth.State.bits.fEditActive == FALSE)
        bMonth = RTCDateCopy.bMonth;
    if (NumObj_ClkYear.State.bits.fEditActive == FALSE)
        bYear = RTCDateCopy.bYear;

    // time
    TimeDate_GetTime( &RTCTimeCopy );
    if (NumObj_ClkHour.State.bits.fEditActive == FALSE)
        bHour = RTCTimeCopy.bHour;
    if (NumObj_ClkMin.State.bits.fEditActive == FALSE)
        bMin = RTCTimeCopy.bMin;
    if (NumObj_ClkSec.State.bits.fEditActive == FALSE)
        bSec = RTCTimeCopy.bSec;

    // RTC calibration value
    if (NumObj_ClkCal.State.bits.fEditActive == FALSE)
        TimeDate_GetCalibDirect( &bClkCalib );

    // ============================================================
    // LED/LCD SETTINGS
    // ============================================================

    // Led Warning Mode state
    if (SlctObj_LEDWM.State.bits.fEditActive == FALSE)
    {   fLedWarnMode    = EE_DevFlags_2.flags.fLedWarnModeStd;
    }

    // Backlight Level
    if (NumObj_BacklOL.State.bits.fEditActive == FALSE)
        bBacklOnLevel   = EE_DisplFlags.flags.BacklOnLevel;
    if (NumObj_BacklLvl.State.bits.fEditActive == FALSE)
        bBacklLev       = EE_DisplFlags.flags.BacklLev;
    if (NumObj_ContrLvl.State.bits.fEditActive == FALSE)
        bContrLev       = EE_DisplFlags.flags.ContrLev;

    // ============================================================
    // EXTENSIONS SETTINGS
    // ============================================================

    // compass state
    if (SlctObj_CompassC.State.bits.fEditActive == FALSE)
    {
        #if (COMPASSDRV==1)
        COMPDRV_HEADINFO *ptHeadingInfo;
        ptHeadingInfo   = CompDrv_GetHeadingInfo();
        bCompassCal     = ptHeadingInfo->ucCalState;
        #endif
    }

    // Coolride enable/disable changed?
    if ( BoolObj_CoolrAvail.State.bits.fEditActive == FALSE )   // edit mode NOT active?
        fCoolrideAvail    = EE_CoolrideCtrl.flags.CoolrAvail;

    // Coolride GPO Settings changed?
    if ( NumObj_CoolrOut.State.bits.fEditActive == FALSE )      // edit mode NOT active?
        gbCoolrGPO      = EE_CoolrideCtrl.flags.CoolrGPO;

    // Coolride GPI Settings changed?
    if ( NumObj_CoolrIn.State.bits.fEditActive == FALSE )       // edit mode NOT active?
        gbCoolrGPI      = EE_CoolrideCtrl.flags.CoolrGPI;

    // FuelSensor enable/disable changed?
    if ( BoolObj_FuelSAvail.State.bits.fEditActive == FALSE )   // edit mode NOT active?
    {   fFuelSensAvail = EE_FuelSensCtrl.flags.FuelSAvail;      
    }

    // FuelSensor ImpulsRate changed?
    if ( NumObj_FuelSImp.State.bits.fEditActive == FALSE )      // edit mode NOT active?
    {   gwFuelSImp = EE_FuelSensCtrl.FuelSImpulseRate;           
    }

    // FuelSensor GPI Settings changed?
    if ( NumObj_FuelSIn.State.bits.fEditActive == FALSE )       // edit mode NOT active?
    {   gbFuelSGPI = EE_FuelSensCtrl.flags.FuelSGPI;
    }
    
}



/***********************************************************************
 *  FUNCTION:       SetDev_ValuesInit
 *  DESCRIPTION:    get local copies of global unions
 *                  to handle edit objects
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        WE HERE HANDLE ONLY THOSE DATA, WHICH CAN _NOT_DIRECTLY_
 *                  BE ACCESSED BY THE EDIT OBJECT VIA AN ADDRESS!
 *********************************************************************** */
void SetDev_ValuesInit(void)
{
    // cylinder correctur factor
    CCFNom          = EE_CCF.nibble.nom;
    CCFDenom        = EE_CCF.nibble.denom;

    // display
    bBacklOnLevel   = EE_DisplFlags.flags.BacklOnLevel;
    bBacklLev       = EE_DisplFlags.flags.BacklLev;
    bContrLev       = EE_DisplFlags.flags.ContrLev;

    // compass state & display mode
    bCompassCal     = 0;
    bCompassDispl   = EE_CompassCtrl.flags.CompassDisplay;

    // Coolride settings
    fCoolrideAvail    = EE_CoolrideCtrl.flags.CoolrAvail;
    gbCoolrGPO      = EE_CoolrideCtrl.flags.CoolrGPO;
    gbCoolrGPI      = EE_CoolrideCtrl.flags.CoolrGPI;
    
    // FuelSensor settings
    fFuelSensAvail = EE_FuelSensCtrl.flags.FuelSAvail;      
    gwFuelSImp      = EE_FuelSensCtrl.FuelSImpulseRate;           
    gbFuelSGPI      = EE_FuelSensCtrl.flags.FuelSGPI;   

    // time/date
    TimeDate_GetDate( &RTCDateCopy );       // get current date
    bDate  = RTCDateCopy.bDate;
    bMonth = RTCDateCopy.bMonth;
    bYear  = RTCDateCopy.bYear;
    TimeDate_GetTime( &RTCTimeCopy );       // get current date
    bHour  = RTCTimeCopy.bHour;
    bMin   = RTCTimeCopy.bMin;
    bSec   = RTCTimeCopy.bSec;
    TimeDate_GetCalibDirect( &bClkCalib );  // get calibration value
}



/***********************************************************************
 *  FUNCTION:       SetDev_CheckState
 *  DESCRIPTION:    Checks, wether UP/DOWN was treated as Wraparound
 *                  of focus inside current object list. If so, the
 *                  next/previous object LIST is activated by setting
 *                  the internal state.
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SetDev_CheckState(MESSAGE GivenMsg)
{
    ERRCODE                 RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID              MsgId;
    OBJ_DUMMY far *       DummyObj;
    void far * far *        ObjList;
    DEVOBJLIST far *        ListCntrl;
    BOOL                    fChanged = FALSE;

    // analyze message: Up/down?
    MsgId = MSG_ID(GivenMsg);

    /* --------------------------------------------------------------- */
    /* check: we went UP, but the focus is on LAST? => WrapAround!
              ==> change Screen to next upper one!
              ==> change Focus to LAST one! */
    if (  ( MsgId == MSG_KEY_UP )
        &&( SDCntrl.eState > SD_VEHIC )
        &&( SDObj.bFocusObj == SDCntrl.List[SDCntrl.eState].LastSelObj) )
    {
        // set new SetDev_ state/screen + focus + flag
        SDCntrl.eState--;
        SDObj.bFocusObj = SDCntrl.List[SDCntrl.eState].LastSelObj;
        fChanged = TRUE;
    }

    /* --------------------------------------------------------------- */
    /* check: we went DOWN, but the focus is on FIRST? => WrapAround!
              ==> change Screen to next upper one!
              ==> change Focus to LAST one! */
    if (  ( MsgId == MSG_KEY_DOWN)
        &&( SDCntrl.eState < (SD_LAST-1) )
        &&( SDObj.bFocusObj == SDCntrl.List[SDCntrl.eState].FirstSelObj) )
    {
        // set new SetDev_ state/screen + focus + flag
        SDCntrl.eState++;
        SDObj.bFocusObj = SDCntrl.List[SDCntrl.eState].FirstSelObj;
        fChanged = TRUE;
    }

    /* --------------------------------------------------------------- */
    /* process changes (if any) */
    if (fChanged == TRUE)
    {
        // reset state of all objects in new list
        DevObjClearState(   &SDObj,
                            SDCntrl.List[SDCntrl.eState].ObjList,
                            SDCntrl.List[SDCntrl.eState].ObjCount,
                            OS_DISPL | OS_SELECT | OS_EDIT );

        // activate focus of new selected object
        ListCntrl   = &(SDCntrl.List[SDCntrl.eState]);
        ObjList     = ListCntrl->ObjList;
        DummyObj    = (OBJ_DUMMY far *)ObjList[SDObj.bFocusObj];
        DummyObj->State.bits.fSelected = TRUE;

        // reset screen state
        SDObj.fScreenInit = FALSE;

        // clear screen
        SetDev_Show(FALSE);
        ODS3( DBG_SYS, DBG_WARNING, "[%s] New Screen/Focus: %u/%u", SDObj.szDevName, SDCntrl.eState, SDObj.bFocusObj);
    }
}




/***********************************************************************
 *  FUNCTION:       SetDev_ObjListInit
 *  DESCRIPTION:    Initialize special object control structure
 *                  to handle different screens
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SetDev_ObjListInit(void)
{
    // Setup 1 st screen object list: Vehicle settings
    SDCntrl.List[SD_VEHIC].ObjList       = ObjectList_Veh;
    SDCntrl.List[SD_VEHIC].ObjCount      = OBJLIST_VEH_CNT;
    SDCntrl.List[SD_VEHIC].FirstSelObj   = DevObjGetFirstSelectable(&SDObj, ObjectList_Veh, OBJLIST_VEH_CNT);
    SDCntrl.List[SD_VEHIC].LastSelObj    = DevObjGetLastSelectable (&SDObj, ObjectList_Veh, OBJLIST_VEH_CNT);

    // Setup 2nd screen object list: Device settings
    SDCntrl.List[SD_DEVICE].ObjList      = ObjectList_Dev;
    SDCntrl.List[SD_DEVICE].ObjCount     = OBJLIST_DEV_CNT;
    SDCntrl.List[SD_DEVICE].FirstSelObj  = DevObjGetFirstSelectable(&SDObj, ObjectList_Dev, OBJLIST_DEV_CNT);
    SDCntrl.List[SD_DEVICE].LastSelObj   = DevObjGetLastSelectable (&SDObj, ObjectList_Dev, OBJLIST_DEV_CNT);

    // Setup 3rd screen object list: Output settings
    SDCntrl.List[SD_OUT].ObjList         = ObjectList_Out;
    SDCntrl.List[SD_OUT].ObjCount        = OBJLIST_OUT_CNT;
    SDCntrl.List[SD_OUT].FirstSelObj     = DevObjGetFirstSelectable(&SDObj, ObjectList_Out, OBJLIST_OUT_CNT);
    SDCntrl.List[SD_OUT].LastSelObj      = DevObjGetLastSelectable (&SDObj, ObjectList_Out, OBJLIST_OUT_CNT);

    // Setup 4th screen object list: Extensions settings
    SDCntrl.List[SD_EXT1].ObjList        = ObjectList_Ext1;
    SDCntrl.List[SD_EXT1].ObjCount       = OBJLIST_EXT1_CNT;
    SDCntrl.List[SD_EXT1].FirstSelObj    = DevObjGetFirstSelectable(&SDObj, ObjectList_Ext1, OBJLIST_EXT1_CNT);
    SDCntrl.List[SD_EXT1].LastSelObj     = DevObjGetLastSelectable (&SDObj, ObjectList_Ext1, OBJLIST_EXT1_CNT);

    // Default state: Show verhicle settings
    SDCntrl.eState = SD_VEHIC;
}

