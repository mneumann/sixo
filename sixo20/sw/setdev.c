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
 * CompilerFix: correct use of #define COMPASS
 *
 * Revision 2.7  2009/06/21 21:23:18  tuberkel
 * Changes by AN and SCHW:
 * - new Compass layout inside TripCounter Module
 * - can be disabled via user settings 'Tripcounter/upperlower:
 * Bitcoded:
 * - bit0: LongDistance:     1=upside (like roadbook), 0=downside
 * - bit1: ShowCompassValue: 1=show, 0=off
 * - bit2: ShowCompassBar:   1=show, 0=off
 * CompassValue ist shown in footer line
 * CompassCompassBar as graphic
 *
 * Revision 2.6  2009/04/20 18:26:04  tuberkel
 * Compiler Fix SetDeviceStateMachine()
 *
 * Revision 2.5  2007/03/30 10:05:13  tuberkel
 * changed EditWheelSizeObj size 11 -> 13 chars
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
// DEVICE OBJECT DATA
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
    #if(COMPASS==1)
    SD_COMP,        //   compass setup
    #endif

  //SD_PORTS,       //   general input/output setup
  //SD_WARN,        //   warning setup
  //SD_DEVLIST,     //   device list setup

    SD_LAST         // INVALID LAST STATE!
} SETDEV_STATE;


// set device control structure (to handle ALL screens)
typedef struct
{   SETDEV_STATE    eState;         // internal show state
    DEVOBJLIST      List[SD_LAST];  // array of all list control structures
} SETDEV_CNTRL;


static DEVDATA          SDObj;              // settings device object itself
static SETDEV_CNTRL     SDCntrl;            // special set device show state control

extern STRING far       szDevName[];        // device names
extern DEVFLAGS1_TYPE   gDeviceFlags1;      // system parameters
extern unsigned char    szSWVersion[];      /* formated sw version */


// ----------------------------------------------------------------
// universal edit buffers for screen objects
static BOOL     fEditBuffer;                //  1 bit edit buffer
static UINT8    bEditBuffer;                //  8 bit edit buffer
static UINT16   wEditBuffer;                // 16 bit edit buffer
static UINT32   dwEditBuffer;               // 32 bit edit buffer



// ----------------------------------------------------------------
// OBJECTS TO BE HANDLED IN EDIT MODE
// ----------------------------------------------------------------

// ----------------------------------------------------------------
// wheel size / impulse objects
extern UINT16           wWheelSize;                     // original wheel size from eeprom in mm
static EDITNUMBEROBJECT EditWheelSizeObj;               // complete wheel size object
extern UINT8            gbWheelImpulse;                 // original wheel impulse from eeprom
static EDITNUMBEROBJECT EditWheelImpulseObj;            // complete wheel impulse object



// ----------------------------------------------------------------
// cylinder correcture factor:
extern CCF_TYPE         CCF;                            // original CCF value (with Nom/Denom nibbles) from eeprom
static EDITNUMBEROBJECT EditCCFNomObj;                  // complete nominator number object
static EDITNUMBEROBJECT EditCCFDenomObj;                // complete denominator number object
static UINT8            CCFNom;                         // copy of CCF.Nom as reference
static UINT8            CCFDenom;                       // copy of CCF.Denom as reference


// ----------------------------------------------------------------
// EngineRunTime objects:
extern TIME_TYPE_LD     EngRunTime_Srv;                 // original values from NVRAM, 4 bytes, engine runtime since last service
extern TIME_TYPE_LD     EngRunTime_All;                 // original values from NVRAM, 4 bytes, engine runtime overall
static EDITNUMBEROBJECT EditEngRunSrvObj;               // complete EnginRunTime Service object
static EDITNUMBEROBJECT EditEngRunAllObj;               // complete EnginRunTime All object


// ----------------------------------------------------------------
// vehicle / service km objects:
extern DIST_TYPE        gNextServKm;                    // to get/set original value
static UINT32           dwServKm;                       // original vehicle distance in km only
static EDITNUMBEROBJECT EditServKmObj;                  // complete number object
static DIST_TYPE        VehicDist;                      // to get/set original value
static UINT32           dwVehicDist;                    // original vehicle distance in km only
static EDITNUMBEROBJECT EditVehicDistObj;               // complete number object


// ----------------------------------------------------------------
// tank capacity / fuel consumption objects:
extern UINT16           gwFuelCap;                      // to get/set original value, in 1/10 liters
static EDITNUMBEROBJECT EditTankCapObj;                 // complete number object
extern UINT8            gbFuelCons;                     // to get/set original value, in 1/10 l/100 km
static EDITNUMBEROBJECT EditFuelConsObj;                // complete number object


// ----------------------------------------------------------------
// Backlight objects:
extern DPLFLAGS_TYPE    gDisplayFlags;                  // orginal display values

// BacklightOnLevel: 'an/aus/auto: 0..63'
static EDITNUMBEROBJECT EditBacklObj;                   // complete backlight object
static UINT8            bBacklOnLevel;                  // copy of gDisplayFlags.flags.BacklOnLevel as reference

// Backlight Level: 'Helligkeit: 63'
static EDITNUMBEROBJECT EditBacklLevObj;                // complete backlight level object
static UINT8            bBacklLev;                      // copy of gDisplayFlags.flags.BacklLev as reference

// Display Contrast:
static EDITNUMBEROBJECT EditContrLevObj;                // complete contrast level object
static UINT8            bContrLev;                      // copy of gDisplayFlags.flags.ContLev as reference

// LEDs Control
static EDITNUMBEROBJECT EditLedDimmObj;                 // complete LED dimmer object
static UINT8            bLEDDimmLev;                    // copy of gDisplayFlags.flags.BacklLev as reference

// Display Contrast:
static EDITNUMBEROBJECT EditDbgOutObj;                  // complete DebugOut control object (level & module)
extern DBGFILT_TYPE     gDebugFilter;                   // original value


// ----------------------------------------------------------------
// RPM Flash Light:
extern RPM_TYPE         RPM_Flash;                      // original values from EEPROM, engine speed to enable flash lamp,   1 RPM/bit
static EDITNUMBEROBJECT EditRPMFlashObj;                // complete RPM Flashlight object


// ----------------------------------------------------------------
// BikeType:
static SELECTOBJECT     SelectBikeObj;                  // complete bike type object
extern BIKE_TYPE        gBikeType;                      // EEprom value
static BIKE_TYPE        LocalBikeType;                  // local edit buffer
static const STRING     pszSelectBike[RESTXT_SET_BIKE_CNT] =
                        {   RESTXT_SET_BIKE_STANDARD    ,
                            RESTXT_SET_BIKE_R100GS      ,
                            RESTXT_SET_BIKE_AFRICATWIN  ,
                            RESTXT_SET_BIKE_BAGHIRA     ,
                            RESTXT_SET_BIKE_F650        ,
                        };

// ----------------------------------------------------------------
// BikeLogo
/* NOTE: All entries derived from 'LOGO_TYPE' */
static SELECTOBJECT     SelectLogoObj;                  // complete logo type object
extern UINT8            gLogoSelection;                 // Eeprom value
static const STRING     pszSelectLogo[RESTXT_SET_LOGO_CNT] =
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
static EDITNUMBEROBJECT EditLogoDelayObj;               // complete logo delay object
       UINT8            gbLogoDelay;                    // Eeprom value;



// ----------------------------------------------------------------
// Compass Display Mode Object:
static SELECTOBJECT     SelectCompDplObj;               // compass calibration state object
extern UINT8            gCompDplMode;                   // Eeprom value
static const STRING     pszSelectCompD[RESTXT_SET_COMPD_CNT] =
                        {   RESTXT_SET_COMPD_NA  ,
                            RESTXT_SET_COMPD_HD  ,
                            RESTXT_SET_COMPD_GR  ,
                            RESTXT_SET_COMPD_HDGR
                        };


// ----------------------------------------------------------------
// Compass Calibration State Object (temporary while calibration mode is active):
static SELECTOBJECT     SelectCompCalStObj;             // compass display mode object
static UINT8            bCmpCalState;                   // to detect user pressed buttons/activity
static const STRING     pszSelectCompC[RESTXT_SET_COMPC_CNT] =
                        {   RESTXT_SET_COMPC_NA   ,
                            RESTXT_SET_COMPC_WORK ,
                            RESTXT_SET_COMPC_HHOLD,
                            RESTXT_SET_COMPC_HTURN,
                            RESTXT_SET_COMPC_HSAVE,
                            RESTXT_SET_COMPC_VHOLD,
                            RESTXT_SET_COMPC_VTURN,
                            RESTXT_SET_COMPC_VSAVE
                        };



// ----------------------------------------------------------------
// Language Selection Object
static SELECTOBJECT     SelectLangObj;                  // Language Selection Object
extern UINT8            gLanguage;                      // eeprom value
static const STRING     pszSelectLang[RESTXT_SET_LANG_CNT] =
                        {   RESTXT_SET_LANG_DE,
                            RESTXT_SET_LANG_EN,
                            RESTXT_SET_LANG_NL
                        };


// ----------------------------------------------------------------
// Device Flags 2:
extern DEVFLAGS2_TYPE   gDeviceFlags2;

static SELECTOBJECT     SelectTripObj;                // trip counter flag object (taken from gDeviceFlags2 )
static const STRING     pszSelectTrip[RESTXT_SET_TRIP_CNT] =
                        {   RESTXT_SET_TRIP_VST,
                            RESTXT_SET_TRIP_VSB
                        };
static BOOL             fTripCntLongUp;               // copy of trip counter flag as reference



// ----------------------------------------------------------------
// Device Flags 3:
extern DEVFLAGS3_TYPE   gDeviceFlags3;

static SELECTOBJECT     SelectMetricObj;              // Metric selection: METER/INCH (taken from gDeviceFlags3 )
static const STRING     pszSelectMetric[RESTXT_SET_METRIC_CNT] =
                        {   RESTXT_SET_METRIC_KM,
                            RESTXT_SET_METRIC_MILES
                        };
static BOOL             fMetric;                        // copy of Metric flag as reference

static SELECTOBJECT     SelectLedWMObj;           // Led Warning Mode Selection: SIxO-like / common mode (leds on at ignition), taken from gDeviceFlags3 )
static const STRING     pszSelectLedWM[RESTXT_SET_LEDWM_CNT] =
                        {   RESTXT_SET_LEDWM_SIXO,
                            RESTXT_SET_LEDWM_STD
                        };
static BOOL             fLedWarnMode;                   // copy of Metric flag as reference



// ----------------------------------------------------------------
// Date field objects
static DATE_TYPE        RTCDateCopy;                    // buffer to get current RTC data
static EDITNUMBEROBJECT EditDayObj;                     // complete day object
static UINT8            bDate;                          // copy of RTC day as reference
static EDITNUMBEROBJECT EditMonthObj;                   // complete month object
static UINT8            bMonth;                         // copy of RTC day as reference
static EDITNUMBEROBJECT EditYearObj;                    // complete year object
static UINT8            bYear;                          // copy of RTC day as reference

// ----------------------------------------------------------------
// time field objects
static TIME_TYPE        RTCTimeCopy;                    // buffer to get current RTC data
static EDITNUMBEROBJECT EditHourObj;                    // complete hour object
static UINT8            bHour;                          // copy of RTC hour as reference
static EDITNUMBEROBJECT EditMinObj;                     // complete minute object
static UINT8            bMin;                           // copy of RTC minute as reference
static EDITNUMBEROBJECT EditSecObj;                     // complete seconds object
static UINT8            bSec;                           // copy of RTC second as reference

// ----------------------------------------------------------------
// clock driver calibration value '±168'
static EDITNUMBEROBJECT EditClkCalibObj;                // complete calibration object
static UINT8            bRTCCalib;                      // buffer to get current RTC value
static UINT8            bClkCalib;                      // copy of RTC clock calibration as reference

// ----------------------------------------------------------------
// daylight saving value (on/off, from eeprom)
static EDITBOOLOBJECT   EditBoolDLSaveObj;              // complete boolean DaylightSavingseconds object
       BOOL             gfDaylightSave;                 // original value from eeprom


// ----------------------------------------------------------------
// Device Settings objects

// Vehicle simulation object (on/off, from eeprom)
static EDITBOOLOBJECT   EditBoolVehSimObj;              // complete boolean VehicleSimualtion edit Object
       BOOL             gfVehicSimulation;              // original value from eeprom

// BMP-Hardcopy via UART and HIGH_BEAM available object (on/off, from eeprom)
static EDITBOOLOBJECT   EditBoolScrDmpObj;              // complete boolean Hardcopy edit Object
       BOOL             gfHardcopy;                   // original value from eeprom

// Eeprom Reset object (on = reset)
static EDITBOOLOBJECT   EditBoolEeprRstObj;             // complete boolean Eeprom reset Object
       BOOL             gfEepromReset;                  // local value to start reset

// Beeper Control (taken from gDeviceFlags2 )
static EDITBOOLOBJECT   EditBoolBeepCtrlObj;            // complete boolean Beeper control Object
       BOOL             gfBeepCtrl;                     // local value

// Compass Available Control
static EDITBOOLOBJECT   EditBoolCompAvailObj;           // complete boolean Compass main switch Object
       BOOL             gfCompAvail;                    // local value


// ----------------------------------------------------------------
// other external symbols
extern UINT16           wMilliSecCounter;               // valid values: 0h .. ffffh





/* ----------------------------------------------------------- */
/* STATIC HEADLINE & OTHER TEXT OBJECTS */
/* ----------------------------------------------------------- */
static TEXTOBJECT   TextObj_HL_Vehicle;
static TEXTOBJECT   TextObj_HL_Device;
static TEXTOBJECT   TextObj_HL_Out;
static TEXTOBJECT   TextObj_HL_Compass;
//static TEXTOBJECT   TextObj_HL_IOPorts;
//static TEXTOBJECT   TextObj_HL_Warnings;

static TEXTOBJECT   TextObj_Comp_Calib;
static TEXTOBJECT   TextObj_Comp_Error;

static TEXTOBJECT   TextObj_Led_Desc;
static TEXTOBJECT   TextObj_Lcd_Desc;

//static TEXTOBJECT   TextObj_Dev_SWVers;
//static TEXTOBJECT   TextObj_Dev_SWVersDsc;


static const TEXTOBJECT_INITTYPE TextObj[] =
{
    /* Headline Objects for all Screens: */
    /*pObject                X   Y   Font         H  Width  Align       Format      string ptr             State  */
    /*--------------------- ---- --- ------------ -- -----  ----------- ----------- ---------------------- --------- */
    {&TextObj_HL_Vehicle,   C01, R1, DPLFONT_6X8, 1, 21,    TXT_CENTER, TXT_INVERS, RESTXT_SET_HL_VEHICLE, OC_DISPL},
    {&TextObj_HL_Device,    C01, R1, DPLFONT_6X8, 1, 21,    TXT_CENTER, TXT_INVERS, RESTXT_SET_HL_DEVICE,  OC_DISPL},
    {&TextObj_HL_Out,       C01, R1, DPLFONT_6X8, 1, 21,    TXT_CENTER, TXT_INVERS, RESTXT_SET_HL_DISPLAY, OC_DISPL},
    {&TextObj_HL_Compass,   C01, R1, DPLFONT_6X8, 1, 21,    TXT_CENTER, TXT_INVERS, RESTXT_SET_HL_COMPASS, OC_DISPL},
  //{&TextObj_HL_IOPorts,   C01, R1, DPLFONT_6X8, 1, 21,    TXT_CENTER, TXT_INVERS, RESTXT_SET_HL_IOPORTS, OC_DISPL},
  //{&TextObj_HL_Warnings,  C01, R1, DPLFONT_6X8, 1, 21,    TXT_CENTER, TXT_INVERS, RESTXT_SET_HL_WARNINGS,OC_DISPL},

    /* Device Descriptors */
    /*pObject                X   Y   Font         H  Width  Align       Format      string ptr             State  */
    /*--------------------- ---- --- ------------ -- -----  ----------- ----------- ---------------------- --------- */
  //{&TextObj_Dev_SWVersDsc,C01, R7, DPLFONT_6X8, 1,  3,    TXT_LEFT,   TXT_NORM,   RESTXT_SET_SW_VERS,    OC_DISPL},
  //{&TextObj_Dev_SWVers,   C04, R7, DPLFONT_6X8, 1, 10,    TXT_LEFT,   TXT_NORM,   szSWVersion,           OC_DISPL},

    /* Compass Descriptors */
    /*pObject                X   Y   Font         H  Width  Align       Format      string ptr             State  */
    /*--------------------- ---- --- ------------ -- -----  ----------- ----------- ---------------------- --------- */
    {&TextObj_Comp_Calib,   C01, R4, DPLFONT_6X8, 1, 21,    TXT_LEFT,   TXT_NORM,   RESTXT_SET_COMPC_DESC, OC_DISPL},
    {&TextObj_Comp_Error,   C01, R6, DPLFONT_6X8, 1, 21,    TXT_LEFT,   TXT_NORM,   RESTXT_SET_COMP_ERR,   OC_DISPL},

    /* Led/Lcd Descriptors */
    /*pObject                X   Y   Font         H  Width  Align       Format      string ptr             State  */
    /*--------------------- ---- --- ------------ -- -----  ----------- ----------- ---------------------- --------- */
    {&TextObj_Led_Desc,     C01, R2, DPLFONT_6X8, 1, 4,     TXT_LEFT,   TXT_NORM,   RESTXT_SET_LCD,        OC_DISPL},
    {&TextObj_Lcd_Desc,     C01, R5, DPLFONT_6X8, 1, 4,     TXT_LEFT,   TXT_NORM,   RESTXT_SET_LED,        OC_DISPL},
    /*--------------------- ---- --- ------------ -- -----  ----------- ----------- ---------------------- --------- */
};
#define TEXTOBJECTLISTSIZE   (sizeof(TextObj)/sizeof(TEXTOBJECT_INITTYPE))



/* ----------------------------------------------------------- */
/* EDIT BOOL OBJECTS INIT TABLE */
/* ----------------------------------------------------------- */

// DebugMode ==> ScreenDumps activable! (see table below)
#if(DEBUG==1)
    #define OC_SCREENDUMP_OPT  OC_DISPL | OC_SELECT | OC_EDIT
#else
    #define OC_SCREENDUMP_OPT  OC_DISPL
#endif

static const EDITBOOL_INITTYPE EditBoolObj[] =
{   /*pObject                X    Y  Font         Width  Data                 EditBuffer    Descriptor              State      */
    /*-------------------- ---- ---- ------------ -----  -------------------- ------------- ----------------------- --------------------------------- */
    {&EditBoolDLSaveObj,   C16,  R3, DPLFONT_6X8,    6,  &gfDaylightSave,     &fEditBuffer, RESTXT_SET_RTC_DLS,     OC_DISPL | OC_SELECT | OC_EDIT },
    {&EditBoolBeepCtrlObj, C16,  R4, DPLFONT_6X8,    6,  &gfBeepCtrl,         &fEditBuffer, RESTXT_SET_BEEP_ON,     OC_DISPL | OC_SELECT | OC_EDIT },
    {&EditBoolVehSimObj,   C16,  R5, DPLFONT_6X8,    6,  &gfVehicSimulation,  &fEditBuffer, RESTXT_SET_VEHICSIM,    OC_DISPL | OC_SELECT | OC_EDIT },
    {&EditBoolScrDmpObj,   C16,  R6, DPLFONT_6X8,    6,  &gfHardcopy,         &fEditBuffer, RESTXT_SET_HARDCOPY,    OC_SCREENDUMP_OPT              },
    {&EditBoolEeprRstObj,  C16,  R7, DPLFONT_6X8,    6,  &gfEepromReset,      &fEditBuffer, RESTXT_SET_RESETEEPROM, OC_DISPL | OC_SELECT | OC_EDIT },
    /*-------------------- ---- ---- ------------ -----  -------------------- ------------- ----------------------- --------------------------------- */
    {&EditBoolCompAvailObj,C01,  R2, DPLFONT_6X8,   21,  &gfCompAvail,        &fEditBuffer, RESTXT_SET_COMPASS,     OC_DISPL | OC_SELECT | OC_EDIT },
    /*-------------------- ---- ---- ------------ -----  -------------------- ------------- ----------------------- --------------------------------- */
};
#define EDITBOOLOBJECTLISTSIZE   (sizeof(EditBoolObj)/sizeof(EDITBOOL_INITTYPE))




/* ----------------------------------------------------------- */
/* SELECT OBJECTS INIT TABLE */
/* ----------------------------------------------------------- */
static const SELECT_INITTYPE SelectObj[] =
{
    /* fpObject           OrgX    OrgY  Font         Width pValue                        Max                    WorkValue       szDescr                      SelectTxtList     SelectTxtWidth           State*/
    /* ------------------ ------ ------ ------------ ----- ----------------------------- ------------------     --------------- ---------------------------- ----------------  ------------------------ --------------------------------- */
    { &SelectBikeObj,       C01,   R2,  DPLFONT_6X8,   12, (UINT8 far *)&LocalBikeType,  RESTXT_SET_BIKE_CNT,   &bEditBuffer,   RESTXT_SET_BIKE_DESC,        pszSelectBike,    RESTXT_SET_BIKE_WIDTH,   OC_DISPL | OC_SELECT | OC_EDIT   },
    /* ------------------ ------ ------ ------------ ----- ----------------------------- ------------------     --------------- ---------------------------- ----------------  ------------------------ --------------------------------- */
    { &SelectTripObj,       C01,   R4,  DPLFONT_6X8,   12, (UINT8 far *)&fTripCntLongUp, RESTXT_SET_TRIP_CNT,   &bEditBuffer,   RESTXT_SET_TRIP_DESC,        pszSelectTrip,    RESTXT_SET_TRIP_WIDTH,   OC_DISPL | OC_SELECT | OC_EDIT   },
    { &SelectLangObj,       C01,   R5,  DPLFONT_6X8,   12, (UINT8 far *)&gLanguage,      RESTXT_SET_LANG_CNT,   &bEditBuffer,   RESTXT_SET_LANG_DESC,        pszSelectLang,    RESTXT_SET_LANG_WIDTH,   OC_DISPL                         },
    { &SelectMetricObj,     C01,   R6,  DPLFONT_6X8,   12, (UINT8 far *)&fMetric,        RESTXT_SET_METRIC_CNT, &bEditBuffer,   RESTXT_SET_METRIC_DESC,      pszSelectMetric,  RESTXT_SET_METRIC_WIDTH, OC_DISPL                         },
    /* ------------------ ------ ------ ------------ ----- ----------------------------- ------------------     --------------- ---------------------------- ----------------  ------------------------ --------------------------------- */
    { &SelectLedWMObj,      C05,   R6,  DPLFONT_6X8,   17, (UINT8 far *)&fLedWarnMode,   RESTXT_SET_LEDWM_CNT,  &bEditBuffer,   RESTXT_SET_LEDWM_DESC,       pszSelectLedWM,   RESTXT_SET_LEDWM_WIDTH,  OC_DISPL | OC_SELECT | OC_EDIT   },
    /* ------------------ ------ ------ ------------ ----- ----------------------------- ------------------     --------------- ---------------------------- ----------------  ------------------------ --------------------------------- */
    { &SelectLogoObj,       C01,   R7,  DPLFONT_6X8,   12, (UINT8 far *)&gLogoSelection, RESTXT_SET_LOGO_CNT,   &bEditBuffer,   RESTXT_SET_LOGO_DESC,        pszSelectLogo,    RESTXT_SET_LOGO_WIDTH,   OC_DISPL | OC_SELECT | OC_EDIT   },
    /* ------------------ ------ ------ ------------ ----- ----------------------------- ------------------     --------------- ---------------------------- ----------------  ------------------------ --------------------------------- */
    { &SelectCompDplObj,    C01,   R3,  DPLFONT_6X8,   21, (UINT8 far *)&gCompDplMode,   RESTXT_SET_COMPD_CNT,  &bEditBuffer,   RESTXT_SET_COMPD_DESC,       pszSelectCompD,   RESTXT_SET_COMPD_WIDTH,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &SelectCompCalStObj,  C01,   R5,  DPLFONT_6X8,   21, (UINT8 far *)&bCmpCalState,   RESTXT_SET_COMPC_CNT,  &bEditBuffer,   "",                          pszSelectCompC,   RESTXT_SET_COMPC_WIDTH,  OC_DISPL | OC_SELECT | OC_EDIT   },
    /* ------------------ ------ ------ ------------ ----- ----------------------------- ------------------     --------------- ---------------------------- ----------------  ------------------------ --------------------------------- */

};
#define SELECTOBJECTLISTSIZE   (sizeof(SelectObj)/sizeof(SELECT_INITTYPE))



/* ----------------------------------------------------------- */
/* EDIT NUMBER INIT TABLE */
/* ----------------------------------------------------------- */
static const EDITNUMBER_INITTYPE EditNumObj[] =
{
    /* VEHICLE SETTINGS */
    /* fpObject           OrgX    OrgY  Font         Width  pNumber                 pWorkNumber     Type   Min  Max    Step DplType  Mode     C  zDescr                      zUnit                       L   State                              */
    /* ------------------ ------ ------ ------------ ----- -----------------------  --------------- ------ ---- ------- --- -------  -------- - ---------------------------- --------------------------- -- ----------------------------------- */
    { &EditCCFNomObj,       C15,   R2,  DPLFONT_6X8,     6, &CCFNom,                &bEditBuffer,   eUCHAR, 1L,     9L,  0L, eDez,   eColumn, 0, RESTXT_SET_CCFNOM_DESC,     RESTXT_SET_CCFNOM_UNIT,     1,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditCCFDenomObj,     C21,   R2,  DPLFONT_6X8,     1, &CCFDenom,              &bEditBuffer,   eUCHAR, 1L,     9L,  0L, eDez,   eColumn, 0, "",                         "",                         1,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditWheelSizeObj,    C01,   R3,  DPLFONT_6X8,    12, &wWheelSize,            &wEditBuffer,   eUINT,  0L,  9999L,  0L, eDez,   eColumn, 0, RESTXT_SET_WHEELSIZE_DESC,  RESTXT_SET_WHEELSIZE_UNIT,  4,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditWheelImpulseObj, C16,   R3,  DPLFONT_6X8,     6, &gbWheelImpulse,        &wEditBuffer,   eUCHAR, 1L,    99L,  0L, eDez,   eColumn, 0, RESTXT_SET_WHEELIMP_DESC,   "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditVehicDistObj,    C01,   R4,  DPLFONT_6X8,    12, &dwVehicDist,           &dwEditBuffer,  eULONG, 0L,999999L,  0L, eDez,   eColumn, 0, RESTXT_SET_VEHICKM_DESC,    RESTXT_SET_VEHICKM_UNIT,    6,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditEngRunAllObj,    C16,   R4,  DPLFONT_6X8,     6, &EngRunTime_All.wHour,  &wEditBuffer,   eUINT,  0L, 65535L,  0L, eDez,   eColumn, 0, "",                         RESTXT_SET_ERT_UNIT,        5,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditServKmObj,       C01,   R5,  DPLFONT_6X8,    12, &dwServKm,              &dwEditBuffer,  eULONG, 0L,999999L,  0L, eDez,   eColumn, 0, RESTXT_SET_SERVKM_DESC,     RESTXT_SET_VEHICKM_UNIT,    6,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditEngRunSrvObj,    C16,   R5,  DPLFONT_6X8,     6, &EngRunTime_Srv.wHour,  &wEditBuffer,   eUINT,  0L, 65535L,  0L, eDez,   eColumn, 0, "",                         RESTXT_SET_ERT_UNIT,        5,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditTankCapObj,      C01,   R6,  DPLFONT_6X8,    12, &gwFuelCap,             &dwEditBuffer,  eUINT,  0L,   999L,  0L, eDez,   eColumn, 1, RESTXT_SET_TANKCAP_DESC,    RESTXT_SET_TANKCAP_UNIT,    4,  OC_DISPL                         },
    { &EditFuelConsObj,     C15,   R6,  DPLFONT_6X8,     7, &gbFuelCons,            &bEditBuffer,   eUCHAR, 0L,   255L,  0L, eDez,   eColumn, 1, RESTXT_SET_FUELCONS_DESC,   RESTXT_SET_FUELCONS_UNIT,   4,  OC_DISPL                         },
    { &EditLogoDelayObj,    C18,   R7,  DPLFONT_6X8,     4, &gbLogoDelay,           &bEditBuffer,   eUCHAR, 0L,    99L,  0L, eDez,   eColumn, 1, "",                         RESTXT_SET_LOGODELAY_UNIT,  3,  OC_DISPL | OC_SELECT | OC_EDIT   },

    /* DEVICE SETTINGS */
    /* fpObject           OrgX    OrgY  Font         Width  pNumber                 pWorkNumber     Type   Min  Max    Step DplType  Mode     C  zDescr                      zUnit                       L   State                              */
    /* ------------------ ------ ------ ------------ ----- -----------------------  --------------- ------ ---- ------- --- -------  -------- - ---------------------------- --------------------------- -- ----------------------------------- */
    { &EditHourObj,         C01,   R2,  DPLFONT_6X8,     8, &bHour,                 &bEditBuffer,   eUCHAR, 0L,    23L,  0L, eDez,   eColumn, 0, RESTXT_SET_RTC_TIME,        "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditMinObj,          C09,   R2,  DPLFONT_6X8,     3, &bMin,                  &bEditBuffer,   eUCHAR, 0L,    59L,  0L, eDez,   eColumn, 0, RESTXT_TIMESEPERATOR,       "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditSecObj,          C12,   R2,  DPLFONT_6X8,     3, &bSec,                  &bEditBuffer,   eUCHAR, 0L,    59L,  0L, eDez,   eColumn, 0, RESTXT_TIMESEPERATOR,       "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditClkCalibObj,     C16,   R2,  DPLFONT_6X8,     6, &bClkCalib,             &bEditBuffer,   eUCHAR, 0L,   255L,  0L, eDez,   eColumn, 0, RESTXT_SET_RTC_CALIB,       "",                         3,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditDayObj,          C01,   R3,  DPLFONT_6X8,     8, &bDate,                 &bEditBuffer,   eUCHAR, 1L,    31L,  0L, eDez,   eColumn, 0, RESTXT_SET_RTC_DATE,        "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditMonthObj,        C09,   R3,  DPLFONT_6X8,     3, &bMonth,                &bEditBuffer,   eUCHAR, 1L,    12L,  0L, eDez,   eColumn, 0, RESTXT_DAYSEPERATOR,        "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditYearObj,         C12,   R3,  DPLFONT_6X8,     3, &bYear,                 &bEditBuffer,   eUCHAR, 0L,    99L,  0L, eDez,   eColumn, 0, RESTXT_DAYSEPERATOR,        "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditDbgOutObj,       C01,   R7,  DPLFONT_6X8,    12, &gDebugFilter,          &bEditBuffer,   eUCHAR, 0L,   255L,  0L, eHex,   eColumn, 0, RESTXT_DBGOUTDESCR,         "",                         2,  OC_DISPL                         },

    /* LED/LCD SETTINGS */
    /* fpObject           OrgX    OrgY  Font         Width  pNumber                 pWorkNumber     Type   Min  Max    Step DplType  Mode     C  zDescr                      zUnit                       L   State                              */
    /* ------------------ ------ ------ ------------ ----- -----------------------  --------------- ------ ---- ------- --- -------  -------- - ---------------------------- --------------------------- -- ----------------------------------- */
    { &EditBacklObj,        C05,   R2,  DPLFONT_6X8,    17, &bBacklOnLevel,         &bEditBuffer,   eUCHAR, 0L,     7L,  0L, eDez,   eColumn, 0, RESTXT_SET_LCD_BL_DESC,     "",                         1,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditBacklLevObj,     C05,   R3,  DPLFONT_6X8,    17, &bBacklLev,             &bEditBuffer,   eUCHAR, 0L,    63L,  1L, eDez,   eStep,   0, RESTXT_SET_LCD_BR_DESC,     "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditContrLevObj,     C05,   R4,  DPLFONT_6X8,    17, &bContrLev,             &bEditBuffer,   eUCHAR, 0L,    63L,  1L, eDez,   eStep,   0, RESTXT_SET_LCD_CNT_DESC,    "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT   },
    { &EditLedDimmObj,      C05,   R5,  DPLFONT_6X8,    17, &bLEDDimmLev,           &bEditBuffer,   eUCHAR, 0L,     7L,  1L, eDez,   eStep,   0, RESTXT_SET_LED_DIM_DESC,    "",                         1,  OC_DISPL | OC_SELECT             },
    { &EditRPMFlashObj,     C05,   R7,  DPLFONT_6X8,    17, &RPM_Flash,             &wEditBuffer,   eUINT,  0L, 30000L,  0L, eDez,   eColumn, 0, RESTXT_SET_RPMFL_DESC,      "",                         5,  OC_DISPL | OC_SELECT | OC_EDIT   },

    /* ------------------ ------ ------ ------------ ----- -----------------------  --------------- ------ ---- ------- --- ------- -------- - ------------------------ ----------------------- -- -------------------------------------------- */
};
#define EDITNUMOBJECTLISTSIZE   (sizeof(EditNumObj)/sizeof(EDITNUMBER_INITTYPE))




// ---------------------------------------------------------------------
// SCREEN FOCUS ORDER
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// Vehicle-Settings - Screen Focus Order
static const void far * ObjectList_Veh[] =
{
    (void far *) &TextObj_HL_Vehicle,   // 00
    (void far *) &SelectBikeObj,        // 01
    (void far *) &EditCCFNomObj,        // 02
    (void far *) &EditCCFDenomObj,      // 03
    (void far *) &EditWheelSizeObj,     // 04
    (void far *) &EditWheelImpulseObj,  // 05
    (void far *) &EditVehicDistObj,     // 06
    (void far *) &EditEngRunAllObj,     // 07
    (void far *) &EditServKmObj,        // 08
    (void far *) &EditEngRunSrvObj,     // 09
    (void far *) &EditTankCapObj,       // 10
    (void far *) &EditFuelConsObj,      // 11
    (void far *) &SelectLogoObj,        // 12
    (void far *) &EditLogoDelayObj      // 13

};
#define OBJLIST_VEH_CNT (sizeof(ObjectList_Veh)/sizeof(OBJSTATE)/sizeof(void far *))


// -------------------------------------------------------------------------
// Device-Settings - Screen Focus Order
static const void far * ObjectList_Dev[] =
{
    (void far *) &TextObj_HL_Device,    // 00
    (void far *) &EditHourObj,          // 01
    (void far *) &EditMinObj,           // 02
    (void far *) &EditSecObj,           // 03
    (void far *) &EditClkCalibObj,      // 04
    (void far *) &EditDayObj,           // 05
    (void far *) &EditMonthObj,         // 06
    (void far *) &EditYearObj,          // 07
    (void far *) &EditBoolDLSaveObj,    // 08
    (void far *) &SelectTripObj,        // 09
    (void far *) &EditBoolBeepCtrlObj,  // 10
    (void far *) &SelectLangObj,        // 11
    (void far *) &EditBoolVehSimObj,    // 12
    (void far *) &SelectMetricObj,      // 13
    (void far *) &EditBoolScrDmpObj,    // 14
    (void far *) &EditDbgOutObj,        // 15
    (void far *) &EditBoolEeprRstObj,   // 16
};
#define OBJLIST_DEV_CNT (sizeof(ObjectList_Dev)/sizeof(OBJSTATE)/sizeof(void far *))


// -------------------------------------------------------------------------
// LED/LCD Settings Screen Tab order
static const void far * ObjectList_Out[] =
{
    (void far *) &TextObj_HL_Out,       // 0
    (void far *) &TextObj_Lcd_Desc,     // 1
    (void far *) &EditBacklObj,         // 2
    (void far *) &EditBacklLevObj,      // 3
    (void far *) &EditContrLevObj,      // 4
    (void far *) &TextObj_Led_Desc,     // 5
    (void far *) &EditLedDimmObj,       // 6
    (void far *) &SelectLedWMObj,       // 7
    (void far *) &EditRPMFlashObj,      // 8
};
#define OBJLIST_OUT_CNT   (sizeof(ObjectList_Out)/sizeof(OBJSTATE)/sizeof(void far *))


// -------------------------------------------------------------------------
// Compass Screen Tab order
static const void far * ObjectList_Compass[] =
{
    (void far *) &TextObj_HL_Compass,   // 1
    (void far *) &EditBoolCompAvailObj, // 2
    (void far *) &SelectCompDplObj,     // 3
    (void far *) &TextObj_Comp_Calib,   // 4
    (void far *) &SelectCompCalStObj,   // 5
    (void far *) &TextObj_Comp_Error,   // 6
};
#define OBJLIST_CMP_CNT (sizeof(ObjectList_Compass)/sizeof(OBJSTATE)/sizeof(void far *))






// ---------------------------------------------------------------------
// non public prototypes
void    SetDeviceShow           (BOOL fShow);
ERRCODE SetDeviceTryObjects     (MESSAGE GivenMsg);
void    SetDeviceValuesChanges   (void);
void    SetDeviceValuesUpdate   (void);
void    SetDeviceValuesInit     (void);
void    SetDeviceCheckState    (MESSAGE GivenMsg);
void    SetDeviceObjListInit    (void);






/***********************************************************************
 *  FUNCTION:       SetDeviceInit
 *  DESCRIPTION:    all initial stuff for all objects
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        These initial settings are updated at any time
 *                  the values are edited/changed.
 *********************************************************************** */
ERRCODE SetDeviceInit(void)
{
    int i;
    ERRCODE  RValue;

    /* device main data */
    SDObj.eDevID       = DEVID_SET;
    SDObj.szDevName    = szDevName[DEVID_SET];
    SDObj.fFocused     = FALSE;
    SDObj.fScreenInit  = FALSE;

    /* initialize all kind of object types */
    DevObjInit( &SDObj, (void far *)TextObj,        TEXTOBJECTLISTSIZE,     OBJT_TXT   );
    DevObjInit( &SDObj, (void far *)EditNumObj,     EDITNUMOBJECTLISTSIZE,  OBJT_ENUM  );
    DevObjInit( &SDObj, (void far *)SelectObj,      SELECTOBJECTLISTSIZE,   OBJT_SLCT  );
    DevObjInit( &SDObj, (void far *)EditBoolObj,    EDITBOOLOBJECTLISTSIZE, OBJT_EBOOL );

    /* special SDObj object lists control handling */
    SetDeviceObjListInit();

    /* initialize edit values */
    SetDeviceValuesInit();

    /* reset focus handling for current screen  */
    DevObjFocusReset(   &SDObj,
                        SDCntrl.List[SDCntrl.eState].ObjList,
                        SDCntrl.List[SDCntrl.eState].ObjCount );

    // return
    ODS( DBG_SYS, DBG_INFO, "- SetDeviceInit() done!");
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       SetDeviceShow
 *  DESCRIPTION:    update devices appearence (new/update/clear)
 *                  by calling Show-Fct. of all relevant objects
 *  PARAMETER:      BOOL    TRUE = show objects, FALSE = clear screen
 *  RETURN:         -
 *  COMMENT:        Some editor buffers directly access the original
 *                  value, so they do not have to be refreshed
 *********************************************************************** */
void SetDeviceShow(BOOL fShow)
{
    MESSAGE NewMsg;
    UINT8   ShowMode;
    UINT8   i;

    // show mode of complete device
    if (fShow == TRUE)
    {
        // 'SHOW' SCREEN

        // update all object values
        SetDeviceValuesUpdate();

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
 *  FUNCTION:       SetDeviceMsgEntry
 *  DESCRIPTION:    Receive Message Handler called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE SetDeviceMsgEntry(MESSAGE GivenMsg)
{
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID  MsgId;
    MESSAGE     NewMsg;


    // analyze message: it's for us?
    MsgId = MSG_ID(GivenMsg);                                                   // get message id
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
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                    // send answer message
                SetDeviceShow(FALSE);                                           // clear our screen
                SDObj.fScreenInit = FALSE;                                  // reset init state
                SDObj.fFocused    = FALSE;                                  // clear our focus
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
                SetDeviceShow(TRUE);                                    // show our screen immediatly
                SDObj.fScreenInit = TRUE;                           // reset init state
                SDObj.fFocused    = TRUE;                           // set our focus
                gDeviceFlags1.flags.ActDevNr = DEVID_SET;                // save device# for restore
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
                    SDObj.fFocused = FALSE;                        // loose our focus
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
                    {   SetDeviceValuesChanges();
                        SetDeviceShow(TRUE);
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
                    {   SetDeviceCheckState(GivenMsg);
                    }
                    /* show changes anyway */
                    SetDeviceShow(TRUE);
                }

                /* try to give focus to next device */
                if (  (RValue == ERR_MSG_NOT_PROCESSED                    )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN))
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYSHORT              ) )
                {
                    /* give focus immediatly to next screen */
                    SDObj.fScreenInit = FALSE;                         // reset init state
                    SDObj.fFocused    = FALSE;                         // clear our focus
                    SetDeviceShow(FALSE);                              // clear our screen
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_SET, DEVID_INTRO);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_SCREEN_RFRSH:
                /* backlight automatic,
                   only if Backlight-Switch and -Level object not in edit mode */
                if (  (EditBacklObj.State.bits.fEditActive    == FALSE)
                    &&(EditBacklLevObj.State.bits.fEditActive == FALSE) )
                    LCDDrvSetBacklightLevel(    DisplBacklightCheckOn(gDisplayFlags.flags.BacklOnLevel),
                                                gDisplayFlags.flags.BacklLev );
                SetDeviceShow(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;
            default: return ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       SetDeviceValuesChanges
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
 *********************************************************************** */
void SetDeviceValuesChanges( void )
{
    // Wheel Size changed? -------------------------
    // no check necessary, we directly access global data when editor finishes!

    // CCF value was changed? ----------------------
    if (  (CCF.nibble.nom   != CCFNom  )
        ||(CCF.nibble.denom != CCFDenom) )
    {   CCF.nibble.nom   = CCFNom;                              // save global -> auto eeprom update!
        CCF.nibble.denom = CCFDenom;
    }

    // Eeprom Reset required by user? -----------------
    if( gfEepromReset == 1 )        // saved with changes?
    {
        gfEepromReset  = 0;                     // clear for next use
        SetDeviceShow(FALSE);                   // clear screen
        ParSetDefaults(PARTIAL);                // reset ALL parameters
        Delay_ms(500);                          // wait, simulate a reset
        SetDeviceShow(TRUE);                    // rebuild screen
    }

    // Beeper Usage was changed? -----------------
    if( gDeviceFlags2.flags.BeeperAvail != gfBeepCtrl )         // saved with changes?
    {   gDeviceFlags2.flags.BeeperAvail  = gfBeepCtrl;          // save global -> auto eeprom update!
    }

    // Daylight Saving was changed? -----------------
    // NOTE:    We do not change current time here! We interprete current time
    //          as being correct right now, independently of switching DLS on/off.
    //          BUT we update the USGAE of the CEST flag for further automatic checking.
    if( gDeviceFlags2.flags.DaylightSaveAuto != gfDaylightSave )// saved with changes?
    {
        gDeviceFlags2.flags.DaylightSaveAuto  = gfDaylightSave; // save global -> auto eeprom update!
        TimeDate_UpdateCEST();                                  // update update CEST state here too!
    }

    // VehicleSimulation was changed? -----------------
    if( gDeviceFlags2.flags.VehicSimul  != gfVehicSimulation )  // saved with changes?
    {   gDeviceFlags2.flags.VehicSimul   = gfVehicSimulation;   // save global -> auto eeprom update!
    }

    // Hardcopy Usage was changed? -----------------
    if( gDeviceFlags2.flags.Hardcopy  != gfHardcopy )           // saved with changes?
    {   gDeviceFlags2.flags.Hardcopy   = gfHardcopy;            // save global -> auto eeprom update!
    }

    // Backlight on/off and Level ------------------
    if( gDisplayFlags.flags.BacklOnLevel != bBacklOnLevel )     // saved with changes?
    {   gDisplayFlags.flags.BacklOnLevel = bBacklOnLevel;       // save global -> auto eeprom update!
    }

    // BacklightLevel was changed? -----------------
    if( gDisplayFlags.flags.BacklLev != bBacklLev )             // saved with changes?
    {   gDisplayFlags.flags.BacklLev = bBacklLev;               // save global -> auto eeprom update!
    }

    // NOTE: As BacklightSwitch and -Level are to be set in one
    //       API function, they are to be handled together here!
    // BacklightSwitch in edit mode? -----------
    if( EditBacklObj.State.bits.fEditActive == TRUE )               // edit mode active?
    {   LCDDrvSetBacklightLevel( DisplBacklightCheckOn(bEditBuffer),  // use CURRENT EDIT VALUE of BacklightOnlevel!!!
                                 gDisplayFlags.flags.BacklLev );
    }
    else
    {  // BacklightLevel in edit mode? -----------
       if( EditBacklLevObj.State.bits.fEditActive == TRUE )         // NO: edit mode level active?
       {    LCDDrvSetBacklightLevel(  DisplBacklightCheckOn(gDisplayFlags.flags.BacklOnLevel),
                                      bEditBuffer );                // use CURRENT EDIT VALUE of BacklightLevel!!!
       }
       else
       {    // ELSE: Set current Backlight-Switch-Level with current backlight level
            LCDDrvSetBacklightLevel(  DisplBacklightCheckOn(gDisplayFlags.flags.BacklOnLevel),
                                      gDisplayFlags.flags.BacklLev ); //set global values
       }
    }

    // Service km changed? -------------------
    if( gNextServKm.km != dwServKm )
    {    gNextServKm.km = dwServKm;                       // give back km into dkm structure
    }

#if (COMPASS==1)
    // CompassCalib State in edit mode? -----------------
    if (EditCompassCalibObj.State.bits.fEditActive == TRUE )    // edit mode active?
    {
        if(bEditBuffer < bCurrentCmpCalState)           // decr. state NOT allowed! -> fix old value!
        {   bEditBuffer = bCurrentCmpCalState;
        }
        else if(bEditBuffer > bCurrentCmpCalState+1)    // incr. >1 NOT allowed! -> limit new value!
        {   bEditBuffer = bCurrentCmpCalState+1;
        }
        else if (bEditBuffer != bCurrentCmpCalState)    // still a user activity detected?
        {
            bCurrentCmpCalState++;                      // should indicate the real compass driver calibration state
            switch(bCurrentCmpCalState)
            {   case 0: ODS( DBG_SYS, DBG_INFO, "Waiting for Compass Calibration start.."); break;
                case 1: ODS( DBG_SYS, DBG_INFO, "Step 1: Hold compass horizontal and keep still!"); break;
                case 2: ODS( DBG_SYS, DBG_INFO, "Step 2: Multiple rotate compass horizontal!"); break;
                case 3: ODS( DBG_SYS, DBG_INFO, "Step 3: Save horizontal measurement!"); break;
                case 4: ODS( DBG_SYS, DBG_INFO, "Step 4: Hold compass vertical and keep still!"); break;
                case 5: ODS( DBG_SYS, DBG_INFO, "Step 5: Multiple rotate compass vertical!"); break;
                case 6: ODS( DBG_SYS, DBG_INFO, "Step 6: Save vertical measurement!"); break;
            }
            CompassCmdIncCalState();                    // ==> activate next calibration step!
        }
        else
        {   // nothing changed ==> nothing to do!
        }
    }
    else    // currently not (no longer) in edit mode
    {   // check for changed value after editing finished with ESC / OK
        if (bCmpCalState == 0)                      // user pressed ESC ?
        {   ODS( DBG_SYS, DBG_INFO, "Calibration aborted => Reset Calibration Mode!");
            CompassCmdReset();                      // ==> reset calibration process!
        }
        if (bCmpCalState < 6)                      // user OK before end 'state 6'?
        {   ODS( DBG_SYS, DBG_INFO, "Calibration not completed => Reset Calibration Mode!");
            CompassCmdReset();                      // ==> reset calibration process!
        }
        else
        {   // success finished calibration state!
            ODS( DBG_SYS, DBG_INFO, "Calibration successfully completed!");
        }
        bCmpCalState   = 0;                         // reset to default state
        bCurrentCmpCalState = 0;
    }
#endif // COMPASS

    // TripCntFlag was changed? -----------------
    if( gDeviceFlags2.flags.TripCLongDistUp != fTripCntLongUp )     // compare bits only
    {   gDeviceFlags2.flags.TripCLongDistUp  = fTripCntLongUp;      // save global -> auto eeprom update!
    }

    // Metric was changed? -----------------
    if( gDeviceFlags3.flags.Metric != fMetric )                     // compare bits only
    {   gDeviceFlags3.flags.Metric  = fMetric;                      // save global -> auto eeprom update!
    }

    // LED Warning Mode was changed? -----------------
    if( gDeviceFlags3.flags.LedWarnMode != fLedWarnMode )           // compare bits only
    {   gDeviceFlags3.flags.LedWarnMode  = fLedWarnMode;            // save global -> auto eeprom update!
    }

    // Vehicle Distance changed? -------------------
    if( VehicDist.km != dwVehicDist )
    {   VehicDist.km = dwVehicDist * 100L;          // give back km into dkm structure
        MeasSetVehicDist( &VehicDist );             // save into system variable -> auto eeprom update!
    }

    // RPM flash setting changed? ------------------
    // no check necessary, we directly access global data when editor finishes!

    // Bike TYPE changed? ------------------
    if ( gBikeType != LocalBikeType )
    {    gBikeType  = LocalBikeType;                // save that new value
        // essential: reset all vehicle states to 'all right' too! */
        SurvResetAllParameters();
        // essential: adapt vehicle specific digital filter settings too! */
        DigInDrv_FilterInit();
    }

    // Bike LOGO changed? ------------------
    // no check necessary, we directly access global data
    // 'gLogoSelection' when editor finishes and auto eeprom update!


    // Display Contrast changed? --------------------
    if( gDisplayFlags.flags.ContrLev != bContrLev ){            // saved with changes?
        gDisplayFlags.flags.ContrLev = bContrLev;               // save global -> auto eeprom update!
    }

    // Display Contrast in edit mode? --------------
    if( EditContrLevObj.State.bits.fEditActive == FALSE ){      // edit mode NOT active?
        LCDDrvSetContrastLevel( gDisplayFlags.flags.ContrLev ); // set global contrast value
    }
    else{
        LCDDrvSetContrastLevel(bEditBuffer);                    // execute changes immediatly!
    }

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
    if (  (EditHourObj.State.bits.fSelected == TRUE)            // Hours focused?
        &&(RTCTimeCopy.bHour != bHour              ) )          // AND saved hours with changes?
    {   RTCTimeCopy.bHour = bHour;                              // copy changes
        TimeDate_SetTime( &RTCTimeCopy );                       // save changes in RTC (and correct it)
        bHour = RTCTimeCopy.bHour;                              // read back (corrected) value
        if( gDeviceFlags2.flags.DaylightSaveAuto == TRUE )      // DaylightSaving Automatic enabled?
        {   gDeviceFlags2.flags.CESTActive = TimeDate_GetCEST();// set CEST state too
        }
    }
    else
    {   if (  (EditMinObj.State.bits.fSelected == TRUE)         // Minutes focused?
            &&(RTCTimeCopy.bMin != bMin               ) )       // AND saved month with changes?
        {   RTCTimeCopy.bMin = bMin;                            // copy changes
            TimeDate_SetTime( &RTCTimeCopy );                   // save changes in RTC (and correct it)
            bMin = RTCTimeCopy.bMin;                            // read back (corrected) value
        }
        else
        {   if (  (EditSecObj.State.bits.fSelected == TRUE)     // Seconds focused?
                //&&(EditSecObj.State.bits.fEditActive == FALSE)  // AND not in edit mode?
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
 *  FUNCTION:       SetDeviceValuesUpdate
 *  DESCRIPTION:    Updates all SDObj parameters in order to be
 *                  shown immediatly.
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
 *                  local 'CCFNom' of 'CCF.nibble.nom' are changed by
 *                  edit object only if 'Save' button was pressed! So,
 *                  while edit process is still active, these local values
 *                  won't change!
 *
 *                  BUT: If you like to track the current(!) edit value
 *                  while edit mode is active, you should use the EDIT
 *                  BUFFER value instead! (e.g. DisplayContrast)
 *********************************************************************** */
void SetDeviceValuesUpdate(void)
{

    // REFRESH ALL DYNAMIC VALUES - ONLY IF NOT IN EDIT MODE

    // vehicle km
    if (EditVehicDistObj.State.bits.fEditActive == FALSE)
    {   VehicDist = MeasGetVehicDist(MR_KM);            // get fresh value
        dwVehicDist = VehicDist.km;                     // get km only
    }

    // service km
    if (EditServKmObj.State.bits.fEditActive == FALSE)
    {   dwServKm = gNextServKm.km;                      // get km only
    }

    // compass state
#if (COMPASS==1)
    if (EditCompassCalibObj.State.bits.fEditActive == FALSE)
    {   tCompassHeadingInfo *ptHeadingInfo;
        ptHeadingInfo = CompassGetHeadingInfo();
        bCmpCalState  = ptHeadingInfo->ucCalState;      // get current driver state
    }
#endif //COMPASS

    // tripcounter state
    if (SelectTripObj.State.bits.fEditActive == FALSE)
    {   fTripCntLongUp = gDeviceFlags2.flags.TripCLongDistUp;
    }

    // metric state
    if (SelectMetricObj.State.bits.fEditActive == FALSE)
    {   fMetric = gDeviceFlags3.flags.Metric;
    }

    // Led Warning Mode state
    if (SelectLedWMObj.State.bits.fEditActive == FALSE)
    {   fLedWarnMode = gDeviceFlags3.flags.LedWarnMode;
    }

    // Cylinder Correctur Factor
    if (EditCCFNomObj.State.bits.fEditActive == FALSE)
        CCFNom = CCF.nibble.nom;
    if (EditCCFDenomObj.State.bits.fEditActive == FALSE)
        CCFDenom = CCF.nibble.denom;

    // backlight level
    if (EditBacklObj.State.bits.fEditActive == FALSE)
        bBacklOnLevel = gDisplayFlags.flags.BacklOnLevel;
    if (EditBacklLevObj.State.bits.fEditActive == FALSE)
        bBacklLev = gDisplayFlags.flags.BacklLev;
    if (EditContrLevObj.State.bits.fEditActive == FALSE)
        bContrLev   = gDisplayFlags.flags.ContrLev;

    // date
    TimeDate_GetDate( &RTCDateCopy );       // get current date
    if (EditDayObj.State.bits.fEditActive == FALSE)
        bDate = RTCDateCopy.bDate;
    if (EditMonthObj.State.bits.fEditActive == FALSE)
        bMonth = RTCDateCopy.bMonth;
    if (EditYearObj.State.bits.fEditActive == FALSE)
        bYear = RTCDateCopy.bYear;

    // time
    TimeDate_GetTime( &RTCTimeCopy );       // get current date
    if (EditHourObj.State.bits.fEditActive == FALSE)
        bHour = RTCTimeCopy.bHour;
    if (EditMinObj.State.bits.fEditActive == FALSE)
        bMin = RTCTimeCopy.bMin;
    if (EditSecObj.State.bits.fEditActive == FALSE)
        bSec = RTCTimeCopy.bSec;

    // RTC calibration value
    if (EditClkCalibObj.State.bits.fEditActive == FALSE)
        TimeDate_GetCalibDirect( &bClkCalib );  // get calibration value

    // BeeperUsage
    if( EditBoolBeepCtrlObj.State.bits.fEditActive == FALSE)
    {   gfBeepCtrl = gDeviceFlags2.flags.BeeperAvail;
    }

    // DaylightSaving Usage
    if( EditBoolDLSaveObj.State.bits.fEditActive == FALSE)
    {   gfDaylightSave = gDeviceFlags2.flags.DaylightSaveAuto;
    }

    // Vehicle Simulation
    if( EditBoolVehSimObj.State.bits.fEditActive == FALSE)
    {   gfVehicSimulation = gDeviceFlags2.flags.VehicSimul;
    }

    // Hardcopy Usage
    if( EditBoolScrDmpObj.State.bits.fEditActive == FALSE)
    {   gfHardcopy = gDeviceFlags2.flags.Hardcopy;
    }

}



/***********************************************************************
 *  FUNCTION:       SetDeviceValuesInit
 *  DESCRIPTION:    get local copies of global unions
 *                  to handle edit objects
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SetDeviceValuesInit(void)
{
    // cylinder correctur factor
    CCFNom          = CCF.nibble.nom;
    CCFDenom        = CCF.nibble.denom;

    // display
    bBacklOnLevel   = gDisplayFlags.flags.BacklOnLevel;
    bBacklLev       = gDisplayFlags.flags.BacklLev;
    bContrLev       = gDisplayFlags.flags.ContrLev;

    // bike type
    LocalBikeType   = gBikeType;

    // compass state
    bCmpCalState    = 0;

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
 *  FUNCTION:       SetDeviceCheckState
 *  DESCRIPTION:    Checks, wether UP/DOWN was treated as Wraparound
 *                  of focus inside current object list. If so, the
 *                  next/previous object LIST is activated by setting
 *                  the internal state.
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SetDeviceCheckState(MESSAGE GivenMsg)
{
    ERRCODE                 RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID              MsgId;
    DUMMYOBJECT far *       DummyObj;
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
        // set new SetDevice state/screen + focus + flag
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
        // set new SetDevice state/screen + focus + flag
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
        DummyObj    = (DUMMYOBJECT far *)ObjList[SDObj.bFocusObj];
        DummyObj->State.bits.fSelected = TRUE;

        // reset screen state
        SDObj.fScreenInit = FALSE;

        // clear screen
        SetDeviceShow(FALSE);
        ODS3( DBG_SYS, DBG_WARNING, "[%s] New Screen/Focus: %u/%u", SDObj.szDevName, SDCntrl.eState, SDObj.bFocusObj);
    }
}




/***********************************************************************
 *  FUNCTION:       SetDeviceObjListInit
 *  DESCRIPTION:    Initialize special object control structure
 *                  to handle different screens
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SetDeviceObjListInit(void)
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

    // Setup 4th screen object list: Compass settings
    #if(COMPASS==1)
    SDCntrl.List[SD_COMP].ObjList        = ObjectList_Compass;
    SDCntrl.List[SD_COMP].ObjCount       = OBJLIST_CMP_CNT;
    SDCntrl.List[SD_COMP].FirstSelObj    = DevObjGetFirstSelectable(&SDObj, ObjectList_Compass, OBJLIST_CMP_CNT);
    SDCntrl.List[SD_COMP].LastSelObj     = DevObjGetLastSelectable (&SDObj, ObjectList_Compass, OBJLIST_CMP_CNT);
    #endif //(COMPASS==1)

    // Default state: Show verhicle settings
    SDCntrl.eState = SD_VEHIC;
}

