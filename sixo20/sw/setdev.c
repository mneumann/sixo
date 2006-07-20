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
 *      Arnold Neugebauer                   Ralf Krizsan
 *      Stöckener Str. 115                  Knickstr. 10
 *      30419 Hannover                      30890 Barsinghausen
 *      arnold.neugebauer@web.de            ralf.krizsan@web.de
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



// defines for device data


// Screen Appearance:  +----.----.----.----.-+
// (21 Chars width)    |Radumfang:    2200 mm|
//                     |Zyl.-Fakt.:    1/1   |
//     <= V2.3.5       |Lichtsteuerung:  0   |
//                     |Helligkeit:     63   |
//                     |Kontrast:       36   |
//                     |Gesamt:    112.500 km|
//                     |24.12.04 13:24:25 ±20|
//                     +----.----.----.----.-+

// Screen Appearance:  +----.----.----.----.-+
// (21 Chars width)    |Rad:2200 mm   ZKF:1/1|
//                     |Display A:0 H:63 K:36|
//     >= V2.4.0       |Schaltbl.:10200 U/Min|
//                     |Serv.:999h Ges.:9999h|
//                     |                     | später: |Typ:__R1100GS Spr.:DE|
//                     |Gesamt:    112.500 km|
//                     |24.12.04 13:24:25 ±20|
//                     +----.----.----.----.-+

// Screen Appearance:  +----.----.----.----.-+
// (21 Chars width)    |Rad:2200 mm   ZKF:1/1|
//                     |Display A:0 H:63 K:36|
//     >= V2.4.2       |Schaltbl.:10200 U/Min|  Motobau:
//                     |Mrd-Typ:0   TripCnt:0|
//                     |Serv. bei  999.999 km|  |Serv.:999h Ges.:9999h|
//                     |Gesamt:    112.500 km|
//                     |24.12.04 13:24:25 ±20|
//                     +----.----.----.----.-+



// device states = focus handling
// NOTE:    these enums have to be in the same order like the
//          objects in EditNumObj[]
typedef enum
{
    eSetFirst,          // INVALID STATE
    eSetWheelsize,      // wheel size
    eSetCCFNom,         // CCF nominator
    eSetCCFDenom,       // CCF denominator
    eSetDisplLight,     // display backlight
    eSetDisplHellgk,    // backlight level
    eSetDisplContr,     // display contrast
    eSetRPMFlash,       // RPM flash light

    eSetBikeType,       // bike type selection
    eSetTripCntFl,      // tripCounter Flag

    // special MOTOBAU selection
    eSetServHours,      // EngineRuntimeHours Service
    eSetAllHours,       // EngineRuntimeHours Overall

    // standard SIXO selection
    eSetServKm,         // next service required at km x

    eSetVehicDist,      // vehicle distance
    eSetDay,            // RTC day
    eSetMonth,          // RTC month
    eSetYear,           // RTC year
    eSetHour,           // RTC hours
    eSetMin,            // RTC minutes
    eSetSec,            // RTC seconds
    eSetClkCalib,       // RTC calibration
    eSetLast,           // INVALID STATE
} SETDEVSTATE;


// device object data
static DEVDATA          SetDevice;                                  // settings device object itself
extern STRING far       szDevName[];                                // device names
extern SYSFLAGS_TYPE    gSystemFlags;                               // system parameters


// buffers for screen objects to make edit process on
static BOOL     fEditBuffer;                                        //  1 bit edit buffer
static UINT8    bEditBuffer;                                        //  8 bit edit buffer
static UINT16   wEditBuffer;                                        // 16 bit edit buffer
static UINT32   dwEditBuffer;                                       // 32 bit edit buffer


// wheelsize object 'Radumfang:   2200 mm'
extern UINT16           wWheelSize;                                 // original wheel size from eeprom in mm
static EDITNUMBEROBJECT EditWheelSizeObj;                           // complete wheel size object


// cylinder correcture factor: 'Zyl.-Fakt.: 1/1'
extern CCF_TYPE         CCF;                                        // original CCF value (with Nom/Denom nibbles) from eeprom
static EDITNUMBEROBJECT EditCCFNomObj;                              // complete nominator number object
static EDITNUMBEROBJECT EditCCFDenomObj;                            // complete denominator number object
static UINT8            CCFNom;                                     // copy of CCF.Nom as reference
static UINT8            CCFDenom;                                   // copy of CCF.Denom as reference


// Backlight objects:
extern DPLFLAGS_TYPE    gDisplayFlags;                              // orginal display values

// BacklightOnLevel: 'an/aus/auto: 0..63'
static EDITNUMBEROBJECT EditBacklObj;                               // complete backlight object
static UINT8            bBacklOnLevel;                              // copy of gDisplayFlags.flags.BacklOnLevel as reference
static CHAR             szBacklDescr[] = RESTXT_SET_DPLLIGHTDESC;       // descriptor string

// Backlight Level: 'Helligkeit: 63'
static EDITNUMBEROBJECT EditBacklLevObj;                            // complete backlight level object
static UINT8            bBacklLev;                                  // copy of gDisplayFlags.flags.BacklLev as reference
static CHAR             szBacklLevDescr[] = RESTXT_SET_DPLHELLGKDESC;   // descriptor string

// Display Contrast: 'Kontrast: 63'
static EDITNUMBEROBJECT EditContrLevObj;                            // complete contrast level object
static UINT8            bContrLev;                                  // copy of gDisplayFlags.flags.ContLev as reference

// RPM FlashLight:
extern RPM_TYPE         RPM_Flash;                                  // original values from EEPROM, engine speed to enable flash lamp,   1 RPM/bit
static EDITNUMBEROBJECT EditRPMFlashObj;                            // complete RPM Flashlight object

// BikeType: |Mrd-Typ:0   TripCnt:0|
static EDITNUMBEROBJECT EditBikeTypeObj;                            // complete bike type object
extern BIKE_TYPE        gBikeType;
static BIKE_TYPE        LocalBikeType;
extern UINT8            gLogoSelection;                             // selected bke logo

// TripCounterFlag: |Mrd-Typ:0   TripCnt:0|
static EDITNUMBEROBJECT EditTripCntFlObj;                            // complete trip counter flag object
static UINT8            bTripCntFl;                                  // copy of trip counter flag as reference
extern TRPCNTFL_TYPE    gTripCntFlags;

// EngineRunTime objects:
extern TIME_TYPE_LD  EngRunTime_Srv;                                // original values from NVRAM, 4 bytes, engine runtime since last service
extern TIME_TYPE_LD  EngRunTime_All;                                // original values from NVRAM, 4 bytes, engine runtime overall
static EDITNUMBEROBJECT EditEngRunSrvObj;                           // complete EnginRunTime Service object
static EDITNUMBEROBJECT EditEngRunAllObj;                           // complete EnginRunTime All object


// service km object |Serv. bei  999.999 km|
extern DIST_TYPE        gNextServKm;                                // to get/set original value
static UINT32           dwServKm;                                   // original vehicle distance in km only
static EDITNUMBEROBJECT EditServKmObj;                              // complete number object



// vehicle distance object 'Gesamt: 112.500 km'
static DIST_TYPE        VehicDist;                                  // to get/set original value
static UINT32           dwVehicDist;                                // original vehicle distance in km only
static EDITNUMBEROBJECT EditVehicDistObj;                           // complete number object


// Date field objects '24.12.04'
static DATE_TYPE        RTCDateCopy;                                // buffer to get current RTC data
static EDITNUMBEROBJECT EditDayObj;                                 // complete day object
static EDITNUMBEROBJECT EditMonthObj;                               // complete month object
static EDITNUMBEROBJECT EditYearObj;                                // complete year object
static UINT8            bDate;                                      // copy of RTC day as reference
static UINT8            bMonth;                                     // copy of RTC day as reference
static UINT8            bYear;                                      // copy of RTC day as reference


// time field objects '13:12:24'
static TIME_TYPE     RTCTimeCopy;                                // buffer to get current RTC data
static EDITNUMBEROBJECT EditHourObj;                                // complete hour object
static EDITNUMBEROBJECT EditMinObj;                                 // complete minute object
static EDITNUMBEROBJECT EditSecObj;                                 // complete seconds object
static UINT8            bHour;                                      // copy of RTC hour as reference
static UINT8            bMin;                                       // copy of RTC minute as reference
static UINT8            bSec;                                       // copy of RTC second as reference

// clock driver calibration value '±68'
static EDITNUMBEROBJECT EditClkCalibObj;                            // complete calibration object
static UINT8            bRTCCalib;                                  // buffer to get current RTC value
static UINT8            bClkCalib;                                  // copy of RTC clock calibration as reference



// other external symbols
extern UINT16           wMilliSecCounter;                           // valid values: 0h .. ffffh



// non public prototypes
void    SetDeviceShow(BOOL fShow);
ERRCODE SetDeviceTryObjects(MESSAGE GivenMsg);
ERRCODE SetDeviceStateMachine(MESSAGE Msg);
void    SetDeviceCheckChanges(void);
ERRCODE SetDeviceSetFocus(SETDEVSTATE eState);


/* EDIT NUMBER INIT TABLE */
static const EDITNUMBER_INITTYPE EditNumObj[] =
{
    /* fpObject           OrgX    OrgY  Font         Width  pNumber                 pWorkNumber     Type   Min  Max    Step DplType Mode     C  zDescr                      zUnit                       L   State                                       */
    /* ------------------ ------ ------ ------------ ----- -----------------------  --------------- ------ ---- ------- --- ------- -------- - ---------------------------- --------------------------- -- -------------------------------------------- */
    { &EditWheelSizeObj,      0,    0,  DPLFONT_6X8,    11, &wWheelSize,            &wEditBuffer,   eUINT,  0L,  9999L,  0L, eDez,   eColumn, 0, RESTXT_SET_WHEELSIZEDESC,   RESTXT_SET_WHEELSIZEUNIT,   4,  OC_DISPL | OC_SELECT | OC_EDIT | OS_SELECT  },
    { &EditCCFNomObj,        84,    0,  DPLFONT_6X8,     6, &CCFNom,                &bEditBuffer,   eUCHAR, 1L,     9L,  0L, eDez,   eColumn, 0, RESTXT_SET_CCFNOMDESC,      RESTXT_SET_CCFNOMUNIT,      1,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditCCFDenomObj,     120,    0,  DPLFONT_6X8,     1, &CCFDenom,              &bEditBuffer,   eUCHAR, 1L,     9L,  0L, eDez,   eColumn, 0, "",                         "",                         1,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditBacklObj,          0,    9,  DPLFONT_6X8,    11, &bBacklOnLevel,         &bEditBuffer,   eUCHAR, 0L,     7L,  0L, eDez,   eColumn, 0, szBacklDescr,               "",                         1,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditBacklLevObj,      72,    9,  DPLFONT_6X8,     4, &bBacklLev,             &bEditBuffer,   eUCHAR, 0L,    63L,  0L, eDez,   eColumn, 0, RESTXT_SET_DPLHELLGKDESC,   "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditContrLevObj,     102,    9,  DPLFONT_6X8,     4, &bContrLev,             &bEditBuffer,   eUCHAR, 0L,    63L,  0L, eDez,   eColumn, 0, RESTXT_SET_DPLCONTRDESC,    "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditRPMFlashObj,       0,   18,  DPLFONT_6X8,    21, &RPM_Flash,             &wEditBuffer,   eUINT,  0L, 30000L,  0L, eDez,   eColumn, 0, RESTXT_SET_RPMFLASH,        RESTXT_SET_RPMFLASHUNIT,    5,  OC_DISPL | OC_SELECT | OC_EDIT              },
      // bike type will modified in init routine for BIKE_MOTOBAU  version
    { &EditBikeTypeObj,       0,   27,  DPLFONT_6X8,     9, &LocalBikeType,         &bEditBuffer,   eUCHAR, 0L,     7L,  0L, eDez,   eColumn, 0, RESTXT_SET_BIKETYPE,        "",                         1,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditTripCntFlObj,     72,   27,  DPLFONT_6X8,     9, &bTripCntFl,            &bEditBuffer,   eUCHAR, 0L,     1L,  0L, eDez,   eColumn, 0, RESTXT_SET_TRIPCNTFL,       "",                         1,  OC_DISPL | OC_SELECT | OC_EDIT              },
      // EngRun EngServ will modified in init routine for BIKE_MOTOBAU version
    { &EditEngRunSrvObj,      0,   36,  DPLFONT_6X8,    10, &EngRunTime_Srv.wHour,  &wEditBuffer,   eUINT,  0L,   999L,  0L, eDez,   eColumn, 0, RESTXT_SET_ERT_SRV,         RESTXT_SET_ERT_UNIT,        3,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditEngRunAllObj,     66,   36,  DPLFONT_6X8,    10, &EngRunTime_All.wHour,  &wEditBuffer,   eUINT,  0L,  9999L,  0L, eDez,   eColumn, 0, RESTXT_SET_ERT_ALL,         RESTXT_SET_ERT_UNIT,        4,  OC_DISPL | OC_SELECT | OC_EDIT              },
      // Servkm will modified in init routine for NOT BIKE_MOTOBAU version
    { &EditServKmObj,         0,   36,  DPLFONT_6X8,    21, &dwServKm,              &dwEditBuffer,  eULONG, 0L, 999999L, 0L, eDez,   eColumn, 3, RESTXT_SET_SERVKM,          RESTXT_SET_VEHICKMUNIT,     7,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditVehicDistObj,      0,   45,  DPLFONT_6X8,    21, &dwVehicDist,           &dwEditBuffer,  eULONG, 0L, 999999L, 0L, eDez,   eColumn, 3, RESTXT_SET_VEHICKMDESC,     RESTXT_SET_VEHICKMUNIT,     7,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditDayObj,            0,   54,  DPLFONT_6X8,     2, &bDate,                 &bEditBuffer,   eUCHAR, 1L,    31L,  0L, eDez,   eColumn, 0, "",                         "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditMonthObj,         12,   54,  DPLFONT_6X8,     4, &bMonth,                &bEditBuffer,   eUCHAR, 1L,    12L,  0L, eDez,   eColumn, 0, RESTXT_DAYSEPERATOR,        RESTXT_DAYSEPERATOR,        2,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditYearObj,          36,   54,  DPLFONT_6X8,     2, &bYear,                 &bEditBuffer,   eUCHAR, 0L,    99L,  0L, eDez,   eColumn, 0, "",                         "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditHourObj,          54,   54,  DPLFONT_6X8,     2, &bHour,                 &bEditBuffer,   eUCHAR, 0L,    23L,  0L, eDez,   eColumn, 0, "",                         "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditMinObj,           66,   54,  DPLFONT_6X8,     4, &bMin,                  &bEditBuffer,   eUCHAR, 0L,    59L,  0L, eDez,   eColumn, 0, RESTXT_TIMESEPERATOR,       RESTXT_TIMESEPERATOR,       2,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditSecObj,           90,   54,  DPLFONT_6X8,     2, &bSec,                  &bEditBuffer,   eUCHAR, 0L,    59L,  0L, eDez,   eColumn, 0, "",                         "",                         2,  OC_DISPL | OC_SELECT | OC_EDIT              },
    { &EditClkCalibObj,     102,   54,  DPLFONT_6X8,     4, &bClkCalib,             &bEditBuffer,   eUCHAR, 0L,   255L,  0L, eDez,   eColumn, 0, RESTXT_CLKCALIBDESC,        "",                         3,  OC_DISPL | OC_SELECT | OC_EDIT              }
    /* ------------------ ------ ------ ------------ ----- -----------------------  --------------- ------ ---- ------- --- ------- -------- - ------------------------ ----------------------- -- -------------------------------------------- */
};










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

    // device main data
    SetDevice.eDevID       = DEVID_SET;
    SetDevice.szDevName    = szDevName[DEVID_SET];
    SetDevice.fFocused     = FALSE;
    SetDevice.fScreenInit  = FALSE;
    SetDevice.wDevState    = eSetWheelsize;

    // ----------------------------------
    // get local copies of global unions to handle EditNumObjects
    CCFNom          = CCF.nibble.nom;
    CCFDenom        = CCF.nibble.denom;
    bBacklOnLevel   = gDisplayFlags.flags.BacklOnLevel;
    bBacklLev       = gDisplayFlags.flags.BacklLev;
    bContrLev       = gDisplayFlags.flags.ContrLev;
    LocalBikeType   = gBikeType;

    TimeDate_GetDate( &RTCDateCopy );       // get current date
    bDate  = RTCDateCopy.bDate;
    bMonth = RTCDateCopy.bMonth;
    bYear  = RTCDateCopy.bYear;

    TimeDate_GetTime( &RTCTimeCopy );       // get current date
    bHour  = RTCTimeCopy.bHour;
    bMin   = RTCTimeCopy.bMin;
    bSec   = RTCTimeCopy.bSec;

    TimeDate_GetCalibDirect( &bClkCalib );  // get calibration value

    /* initialize all edit num objects */
    for (i = 0; i < ARRAY_SIZE(EditNumObj); i++)
    {
        RValue = ObjEditNumInit( EditNumObj[i].fpObject,
                                 EditNumObj[i].wOrgPosX,
                                 EditNumObj[i].wOrgPosY,
                                 EditNumObj[i].eFont,
                                 EditNumObj[i].bWindWidth,
                                 EditNumObj[i].fpNumber,
                                 EditNumObj[i].fpWorkNumber,
                                 EditNumObj[i].eType,
                                 EditNumObj[i].lMin,
                                 EditNumObj[i].lMax,
                                 EditNumObj[i].lStepSize,
                                 EditNumObj[i].eDplType,
                                 EditNumObj[i].eMode,
                                 EditNumObj[i].bComma,
                                 EditNumObj[i].szDescr,
                                 EditNumObj[i].szUnit,
                                 EditNumObj[i].bLength,
                                 EditNumObj[i].bState);

        // special MOTOBAU behaviour post-init-manipulations
        if ( gBikeType == eBIKE_MOTOBAU )
        {
            EditBikeTypeObj.State.bits.fEditable  = 0;          // BikeType: locked to MOTOBAU, will not be editable, but visible
            EditServKmObj.State.bits.fDisplayable = 0;          // ServKm: will not be visible
        }

        // special STANDARD behaviour post-init-manipulations
        if ( gBikeType != eBIKE_MOTOBAU )
        {
            EditBikeTypeObj.Limits.lMax = (UINT32) (eBIKE_MOTOBAU-1);   // BikeType: MOTOBAU version not available
            EditEngRunSrvObj.State.bits.fDisplayable = 0;               // EngRunServHours: will not be visible
            EditEngRunAllObj.State.bits.fDisplayable = 0;               // EngRunAllHours: will not be visible
        }

        // check init process
        if ( RValue != ERR_OK )
        {
            ODS1(   DBG_SYS, DBG_WARNING, "SetDeviceInit(): EditNumObj[%u] not initialized!", i);
        }

    }

    // return
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
    int     i;
    ERRCODE error = ERR_OK;
    MESSAGE NewMsg;                                         // for screen fresh message

    // show mode of complete device
    if (fShow == TRUE)                                      // 'show' screen ?
    {
        // refresh dynamic values (only if not in edit mode)
        if (EditVehicDistObj.State.bits.fEditActive == FALSE)
        {   VehicDist = MeasGetVehicDist(MR_KM);            // get fresh value
            dwVehicDist = VehicDist.km;                     // get km only
        }

        if (EditServKmObj.State.bits.fEditActive == FALSE)
        {   dwServKm = gNextServKm.km;                      // get km only
        }

        if (EditTripCntFlObj.State.bits.fEditActive == FALSE)
        {   bTripCntFl = gTripCntFlags.flags.LongDistUp;
        }

        if (EditCCFNomObj.State.bits.fEditActive == FALSE)
            CCFNom = CCF.nibble.nom;
        if (EditCCFDenomObj.State.bits.fEditActive == FALSE)
            CCFDenom = CCF.nibble.denom;

        if (EditBacklObj.State.bits.fEditActive == FALSE)
            bBacklOnLevel = gDisplayFlags.flags.BacklOnLevel;
        if (EditBacklLevObj.State.bits.fEditActive == FALSE)
            bBacklLev = gDisplayFlags.flags.BacklLev;
        if (EditContrLevObj.State.bits.fEditActive == FALSE)
            bContrLev   = gDisplayFlags.flags.ContrLev;

        TimeDate_GetDate( &RTCDateCopy );       // get current date
        if (EditDayObj.State.bits.fEditActive == FALSE)
            bDate = RTCDateCopy.bDate;
        if (EditMonthObj.State.bits.fEditActive == FALSE)
            bMonth = RTCDateCopy.bMonth;
        if (EditYearObj.State.bits.fEditActive == FALSE)
            bYear = RTCDateCopy.bYear;

        TimeDate_GetTime( &RTCTimeCopy );       // get current date
        if (EditHourObj.State.bits.fEditActive == FALSE)
            bHour = RTCTimeCopy.bHour;
        if (EditMinObj.State.bits.fEditActive == FALSE)
            bMin = RTCTimeCopy.bMin;
        if (EditSecObj.State.bits.fEditActive == FALSE)
            bSec = RTCTimeCopy.bSec;

        if (EditClkCalibObj.State.bits.fEditActive == FALSE)
            TimeDate_GetCalibDirect( &bClkCalib );  // get calibration value

        // do we have to repaint all?
        if (SetDevice.fScreenInit == FALSE)
        {
            /* initialize all edit num objects */
            for (i = 0; i < ARRAY_SIZE(EditNumObj); i++)
                ObjEditNumShow( EditNumObj[i].fpObject, SHOW_ALL );

            SetDevice.fScreenInit = TRUE;                   // reset init state
        }
        else
        {
            // NO, repaint only potential changed stuff

            // repaint edit num objects
            for (i = 0; i < ARRAY_SIZE(EditNumObj); i++)
                ObjEditNumShow( EditNumObj[i].fpObject, SHOW_EDIT | SHOW_CURSOR );
        }
    }
    else                                                    // 'clear' screen!
    {
        DisplClearScreen(0x0);                              // leave a clear screen for next device
        SetDevice.fScreenInit = FALSE;                      // reset devices screen state
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
            if ( SetDevice.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_SET]);
                MSG_BUILD_SETFOCUS(NewMsg,DEVID_SET,MSG_SENDER_ID(GivenMsg));   // build answer message
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                    // send answer message
                SetDevice.fFocused = FALSE;                                     // clear our focus
                SetDeviceShow(FALSE);                                           // clear our screen
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
                if (  (SetDevice.fFocused        == TRUE      )
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
                SetDevice.fFocused = TRUE;                              // set our focus
                SetDeviceShow(TRUE);                                    // show our screen immediatly
                gSystemFlags.flags.ActDevNr = DEVID_SET;                // save device# for restore
                RValue = ERR_MSG_PROCESSED;
             }
             else
             {
                if ( SetDevice.fFocused == TRUE )
                {
                    // Some other device should be given the focus,
                    // BUT WE'VE GOT THE FOCUS!
                    // It should first ask US to give the focus! :-(
                    ODS3(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but %s had focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[MSG_RECEIVER_ID(GivenMsg)],
                            szDevName[DEVID_SET]);
                    SetDevice.fFocused = FALSE;                        // loose our focus
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
    if (  (SetDevice.fFocused == TRUE )
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
                if( RValue == ERR_MSG_NOT_PROCESSED )
                    RValue = SetDeviceTryObjects(GivenMsg);                 // try all objects
                if( RValue == ERR_MSG_NOT_PROCESSED )
                    RValue = SetDeviceStateMachine(GivenMsg);               // try move focus
                if (  (RValue == ERR_MSG_NOT_PROCESSED                    )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN)) // try to give msg to next device
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYSHORT              ) )
                {
                    // give focus immediatly to next screen
                    SetDevice.fFocused = FALSE;                            // clear our focus
                    SetDeviceShow(FALSE);                                  // clear our screen
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_SET, DEVID_INTRO);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_SCREEN_REFRESH:
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
 *  FUNCTION:       SetDeviceTryObjects
 *  DESCRIPTION:    message loop over all device objects until one
 *                  objects used the message
 *  PARAMETER:      message given by device
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_PROCESSED
 *  COMMENT:        TBD: We could improve behaviour, if we use the
 *                  'focus' info of this device!
 *********************************************************************** */
ERRCODE SetDeviceTryObjects(MESSAGE GivenMsg)
{
    int     i;
    ERRCODE RValue = ERR_MSG_NOT_PROCESSED;

    // try all objects to use that message
    for (i = 0; i < ARRAY_SIZE(EditNumObj); i++)
    {
        // try to give that message to object
        RValue = ObjEditNumMsgEntry( EditNumObj[i].fpObject, GivenMsg);

        // object used that message? -> ready!
        if ( RValue == ERR_MSG_PROCESSED)
            break;
    }

    // check & execute changes
    SetDeviceCheckChanges();
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       SetDeviceStateMachine
 *  DESCRIPTION:    focus handles over all selectable objects
 *  PARAMETER:      message id (up/down)
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE SetDeviceStateMachine(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                            // get message id
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;

    // scroll up? --------------------------------------------------------------
    if (  (RValue == ERR_MSG_NOT_PROCESSED                )     // still unpocessed
        &&(MsgId  == MSG_KEY_UP                           )     // [UP]
        &&(  (MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED )     // now pressed
           ||(MSG_KEY_TRANSITION(Msg) == KEYTRANS_ON      ) ) ) // or longer pressed?
    {
        SetDevice.wDevState--;                          // previous selection state

        if (SetDevice.wDevState == eSetFirst)           // wrap around?
            SetDevice.wDevState = (eSetLast-1);

        if (  (gBikeType == eBIKE_MOTOBAU        )      // special MOTOBAU behaviour
            &&(SetDevice.wDevState == eSetServKm ) )    // do not select 'service km'
            SetDevice.wDevState = eSetAllHours;         // jump over it!

        if (  (gBikeType != eBIKE_MOTOBAU          )    // special NOT MOTOBAU behaviour
            &&(SetDevice.wDevState == eSetAllHours ) )  // do not select 'eSetAllHours'
            SetDevice.wDevState = eSetTripCntFl;        // jump over it!

        SetDeviceSetFocus(SetDevice.wDevState);         // now use this focus...
        SetDeviceShow(TRUE);
        RValue = ERR_MSG_PROCESSED;
        ODS1( DBG_SYS, DBG_INFO, "SetDevState: %u", SetDevice.wDevState);
    }

    // scroll down? ------------------------------------------------------------
    if (  (RValue == ERR_MSG_NOT_PROCESSED                )     // still unpocessed
        &&(MsgId  == MSG_KEY_DOWN                         )     // [DOWN]
        &&(  (MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED )     // now pressed
           ||(MSG_KEY_TRANSITION(Msg) == KEYTRANS_ON      ) ) ) // or longer pressed?
    {
        SetDevice.wDevState++;                          // next selection state

        if (SetDevice.wDevState  == eSetLast)           // wrap around?
            SetDevice.wDevState = (eSetFirst+1);

        if (  (gBikeType == eBIKE_MOTOBAU        )      // special MOTOBAU behaviour
            &&(SetDevice.wDevState == eSetServKm ) )    // do not select 'service km'
            SetDevice.wDevState = eSetVehicDist;        // jump over it!

        if (  (gBikeType != eBIKE_MOTOBAU          )    // special NOT MOTOBAU behaviour
            &&(SetDevice.wDevState == eSetServHours) )  // do not select 'eSetServHours'
            SetDevice.wDevState = eSetServKm;           // jump over it!

        SetDeviceSetFocus(SetDevice.wDevState);         // now use this focus...
        SetDeviceShow(TRUE);
        RValue = ERR_MSG_PROCESSED;
        ODS1( DBG_SYS, DBG_INFO, "SetDevState: %u", SetDevice.wDevState);
    }
    return RValue;
}


/***********************************************************************
 *  FUNCTION:       SetDeviceStateMachine
 *  DESCRIPTION:    focus handles over all selectable objects
 *  PARAMETER:      new state to be set
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE SetDeviceSetFocus(SETDEVSTATE eState)
{
    int     i;
    ERRCODE RValue = ERR_MSG_NOT_PROCESSED;

    // clear any focus before setting next one
    EditWheelSizeObj.State.bits.fSelected = FALSE;
    EditCCFNomObj.State.bits.fSelected    = FALSE;
    EditCCFDenomObj.State.bits.fSelected  = FALSE;
    EditBacklObj.State.bits.fSelected     = FALSE;
    EditVehicDistObj.State.bits.fSelected = FALSE;
    EditRPMFlashObj.State.bits.fSelected  = FALSE;
    EditBikeTypeObj.State.bits.fSelected  = FALSE;
    EditTripCntFlObj.State.bits.fSelected = FALSE;
    EditEngRunSrvObj.State.bits.fSelected = FALSE;
    EditEngRunAllObj.State.bits.fSelected = FALSE;
    EditServKmObj.State.bits.fSelected    = FALSE;
    EditBacklLevObj.State.bits.fSelected  = FALSE;
    EditContrLevObj.State.bits.fSelected  = FALSE;
    EditDayObj.State.bits.fSelected       = FALSE;
    EditMonthObj.State.bits.fSelected     = FALSE;
    EditYearObj.State.bits.fSelected      = FALSE;
    EditHourObj.State.bits.fSelected      = FALSE;
    EditMinObj.State.bits.fSelected       = FALSE;
    EditSecObj.State.bits.fSelected       = FALSE;
    EditClkCalibObj.State.bits.fSelected  = FALSE;

    // enable visible focus for selected object:
    switch (eState)
    {
        case eSetWheelsize:     EditWheelSizeObj.State.bits.fSelected = TRUE; break;
        case eSetCCFNom:        EditCCFNomObj.State.bits.fSelected    = TRUE; break;
        case eSetCCFDenom:      EditCCFDenomObj.State.bits.fSelected  = TRUE; break;
        case eSetDisplLight:    EditBacklObj.State.bits.fSelected     = TRUE; break;
        case eSetVehicDist:     EditVehicDistObj.State.bits.fSelected = TRUE; break;
        case eSetDisplHellgk:   EditBacklLevObj.State.bits.fSelected  = TRUE; break;
        case eSetDisplContr:    EditContrLevObj.State.bits.fSelected  = TRUE; break;
        case eSetRPMFlash:      EditRPMFlashObj.State.bits.fSelected  = TRUE; break;
        case eSetBikeType:      EditBikeTypeObj.State.bits.fSelected  = TRUE; break;
        case eSetTripCntFl:     EditTripCntFlObj.State.bits.fSelected = TRUE; break;
        case eSetServHours:     EditEngRunSrvObj.State.bits.fSelected = TRUE; break;
        case eSetAllHours:      EditEngRunAllObj.State.bits.fSelected = TRUE; break;
        case eSetServKm:        EditServKmObj.State.bits.fSelected    = TRUE; break;
        case eSetDay:           EditDayObj.State.bits.fSelected       = TRUE; break;
        case eSetMonth:         EditMonthObj.State.bits.fSelected     = TRUE; break;
        case eSetYear:          EditYearObj.State.bits.fSelected      = TRUE; break;
        case eSetHour:          EditHourObj.State.bits.fSelected      = TRUE; break;
        case eSetMin:           EditMinObj.State.bits.fSelected       = TRUE; break;
        case eSetSec:           EditSecObj.State.bits.fSelected       = TRUE; break;
        case eSetClkCalib:      EditClkCalibObj.State.bits.fSelected  = TRUE; break;

        default: ODS1( DBG_SYS, DBG_INFO, "Unknown DevState: %u", SetDevice.wDevState);
    }
}


/***********************************************************************
 *  FUNCTION:       SetDeviceCheckChanges
 *  DESCRIPTION:    compares all screen object data for any change.
 *                  If any, the global data sources will be updated too
 *                  and the changed value will be executed immedeatly.
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
void SetDeviceCheckChanges( void )
{
    // Wheel Size changed? -------------------------
    // no check necessary, we directly access global data when editor finishes!

    // CCF value was changed? ----------------------
    if (  (CCF.nibble.nom   != CCFNom  )
        ||(CCF.nibble.denom != CCFDenom) ){
        CCF.nibble.nom   = CCFNom;                              // save global -> auto eeprom update!
        CCF.nibble.denom = CCFDenom;
    }

    // Backlight on/off and Level ------------------
    if( gDisplayFlags.flags.BacklOnLevel != bBacklOnLevel ){    // saved with changes?
       gDisplayFlags.flags.BacklOnLevel = bBacklOnLevel;        // save global -> auto eeprom update!
    }

    // BacklightLevel was changed? -----------------
    if( gDisplayFlags.flags.BacklLev != bBacklLev ){            // saved with changes?
       gDisplayFlags.flags.BacklLev = bBacklLev;                // save global -> auto eeprom update!
    }

    // NOTE: As BacklightSwitch and -Level are to be set in one
    //       API function, they are to be handled together here!
    // BacklightSwitch in edit mode? -----------
    if( EditBacklObj.State.bits.fEditActive == TRUE ){              // edit mode active?
      LCDDrvSetBacklightLevel( DisplBacklightCheckOn(bEditBuffer),  // use CURRENT EDIT VALUE of BacklightOnlevel!!!
                               gDisplayFlags.flags.BacklLev );

    }
    else{
       // BacklightLevel in edit mode? -----------
       if( EditBacklLevObj.State.bits.fEditActive == TRUE ){        // NO: edit mode level active?
         LCDDrvSetBacklightLevel(  DisplBacklightCheckOn(gDisplayFlags.flags.BacklOnLevel),
                                   bEditBuffer );                   // use CURRENT EDIT VALUE of BacklightLevel!!!

       }
       else{
          // ELSE: Set current Backlight-Switch-Level with current backlight level
         LCDDrvSetBacklightLevel(  DisplBacklightCheckOn(gDisplayFlags.flags.BacklOnLevel),
                                    gDisplayFlags.flags.BacklLev ); //set global values
       }
    }

    // Service km changed? -------------------
    if( gNextServKm.km != dwServKm )
    {    gNextServKm.km = dwServKm;                       // give back km into dkm structure
    }

    // TripCntFlag was changed? -----------------
    if( gTripCntFlags.flags.LongDistUp != bTripCntFl )
    {  gTripCntFlags.flags.LongDistUp = bTripCntFl;                // save global -> auto eeprom update!
    }

    // Vehicle Distance changed? -------------------
    if( VehicDist.km != dwVehicDist )
    {   VehicDist.km = dwVehicDist * 100L;                      // give back km into dkm structure
        MeasSetVehicDist( &VehicDist );                         // save into system variable -> auto eeprom update!
    }

    // RPM flash setting changed? ------------------
    // no check necessary, we directly access global data when editor finishes!

    // BikeType changed? ------------------
    if ( gBikeType != LocalBikeType )
    {   gBikeType = LocalBikeType;  // save that new value
        switch (gBikeType)          // change logo too!
        {   case eBIKE_STANDARD:    gLogoSelection = eLogo_SIXO;        break;
            case eBIKE_R100GS:      gLogoSelection = eLogo_BMW;         break;
            case eBIKE_R1100GS:     gLogoSelection = eLogo_BMW_1100GS;  break;
            case eBIKE_F650:        gLogoSelection = eLogo_BMW;  	break;
            case eBIKE_AFRICATWIN:  gLogoSelection = eLogo_AfricaTwin;  break;
            case eBIKE_BAGHIRA:     gLogoSelection = eLogo_BAGHIRA;     break;
            case eBIKE_HUSQV:       gLogoSelection = eLogo_HUSQV;       break;
            case eBIKE_HUSQVRS:     gLogoSelection = eLogo_HUSQVRS;     break;
            case eBIKE_KTM:         gLogoSelection = eLogo_KTM;         break;
            case eBIKE_MOTOBAU:     gLogoSelection = eLogo_Motobau;     break;
            default:                gLogoSelection = eLogo_SIXO;        break;
        }
        SurvResetVehicleStates();   // essential: reset all vehicle states to 'all right' too! */
    }

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
    TimeDate_GetTime( &RTCTimeCopy );                           // get time update to detect changes
    if (  (EditHourObj.State.bits.fSelected == TRUE)            // Hours focused?
        &&(RTCTimeCopy.bHour != bHour              ) )          // AND saved hours with changes?
    {   RTCTimeCopy.bHour = bHour;                              // copy changes
        TimeDate_SetTime( &RTCTimeCopy );                       // save changes in RTC (and correct it)
        bHour = RTCTimeCopy.bHour;                              // read back (corrected) value
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
