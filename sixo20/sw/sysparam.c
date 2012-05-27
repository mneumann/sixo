/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-31 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         System Parameter
 *  Purpose:        interface to read / save all parameters in
 *                  EEPROM or NVRAM
 *  Comments:       All stuff to reproduce SIxOs status after power up
 *
 *                  Note1:  System parameter in this context are all
 *                          device parameters saved in eeprom or nvram
 *                          to reproduce device status after power up
 *                          cycle and to save vehicle specific data.
 *
 *                  Note2:  There is no central init function to read back
 *                          ALL eeprom/nvram paramters after power up.
 *                          Each module has to do this for it's own!
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
 * Revision 3.13  2012/05/27 16:01:42  tuberkel
 * All Eeprom/Nvram Variables renamed
 *
 * Revision 3.12  2012/05/24 19:30:03  tuberkel
 * Moved BMP-Logos to 'logos.c'
 *
 * Revision 3.11  2012/05/16 21:06:57  tuberkel
 * New FuelSensor:
 * - now displayed & handled in Maindevice
 * - if enabled: replaces 'NV_FuelDistance'
 * - works well with GPI-0..3 / NVRAM / Reboot / ImpRate = 0
 *
 * Revision 3.10  2012/05/15 20:11:35  tuberkel
 * FuelSensor: BasicSettings enabled & ok (not yet displayed)
 *
 * Revision 3.9  2012/02/27 23:02:29  tuberkel
 * - Eeprom layout changed ==> V3.0.5
 * - new: PID_COOLR_CNTRL, PID_COMPS_CNTRL, PID_LANGUAGE
 * - empty: PID_DEVFLAGS3
 * - CompassDrv corrected
 * - COOLRIDECNTRL_TYPE Bugfix (union)
 * - SysPar- API changed
 *
 * Revision 3.8  2012/02/27 20:46:50  tuberkel
 * - all Coolride GPIs/GPOs correctly set by Eeprom value
 *
 * Revision 3.7  2012/02/26 12:24:55  tuberkel
 * - moved all Eeprom Vlaues physically into 'sysparam' module
 *
 * Revision 3.6  2012/02/21 22:01:56  tuberkel
 * Compass Control/Mode/Eeprom reveiwed/fixed
 *
 * Revision 3.5  2012/02/14 21:08:03  tuberkel
 * - #define COMPASS ==> COMPDRV
 * - Compass SystemParam moved from devFlags2 -> 3
 * - Settings 'Compass' ==> 'Extensions'
 * - all Compass-Display modules enabled by default
 *
 * Revision 3.4  2012/02/10 23:45:22  tuberkel
 * - Survelannce HeatGrip <Info> - if active
 * - Surveillance-API reviewed
 *
 * Revision 3.3  2012/02/08 03:41:30  tuberkel
 * renamed #define MINIEMU ==> KD30_USED
 *
 * Revision 3.2  2011/05/29 21:00:57  tuberkel
 * szSWVersion String extended
 *
 * Revision 3.1  2011/05/29 12:45:04  tuberkel
 * BugFix gwWheelImpulse
 * - Typ korrgiert
 * - jetzt auch im Eeprom gesichert
 *
 * Revision 3.0  2010/11/07 09:30:38  tuberkel
 * V30 Preparations:
 * - BugFix for invalid km-Values from NVRAM
 * - New System Parameters:
 *   - DayLightSavin, Beeper, VehSimulation, Hardcopy
 *   - WheelImpulses/Revolution
 *   - CompassDisplayMode
 *   - Language
 *   - Fuel Capacity/Consumption
 *   - IntroLogoDelay
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
 * Revision 2.3  2009/06/21 21:20:51  tuberkel
 * BugFix: EEPROM max. is 1024 (not 2048) bytes
 *
 * Revision 2.2  2009/04/14 21:01:10  tuberkel
 * Changes done by Arnold:
 * - EEPR_MAGICNUM_ADDR removed
 * - SysPar_CheckMagicNumber() dyn. detects addr. of magic number at end of Eeprom
 *
 * Revision 2.1  2007/03/26 23:27:30  tuberkel
 * changed MOTOBAU version handling
 * - eBikeType -> #define
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.8  2006/03/18 08:52:04  Ralf
 * changed logo handling
 *
 * Revision 1.7  2006/02/18 14:46:30  Ralf
 * - system parameter handling completely reviewed (--> table!)
 * - added new lapcounter / statistic data
 *
 *
 ************************************************************************ */


/* includes */
#include <stdio.h>
#include <string.h>

#include "standard.h"
#include "nvramdrv.h"
#include "eeprdrv.h"
#include "sysparam.h"
#include "measure.h"
#include "debug.h"
#include "timer.h"
#include "display.h"
#include "device.h"
#include "timedate.h"
#include "anain.h"
#include "bitmaps.h"
#include "logos.h"
#include "surveill.h"
#include "resource.h"





/* external symbols */
extern far UINT16   wMilliSecCounter;   /* valid values: 0h .. ffffh */
extern far STRING   szDevName[];        /* debug out stuff */
extern far BOOL     fRTCDefect;         /* surveillance RTC defect detection */



/* ==============================================================

    NVRAM / EEPROM PARAMETER LIST
    -----------------------------

    USAGE:  For each data to be saved cyclicely into NVRAM/EEPROM, there
            has to be
                - the ORIGINAL value used/modified by application
                - the LOCAL COMPARE value to detect changes automatically
                - the DEFAULT value to initialize the NVRAM/EEPROM data

    FUNCTION:   The data configured in 'rgSysParControl' will be
                scanned for any changes cyclicely and with some delay,
                    a) to reduce system load and
                    b) to automatically save any change,
                so that the application does not have to care for that.

    NOTE:   Because we have too many different types to be handled in the
            same table, we use 'void pointers' to data instead of using
            the data itself in the table.

   -------------------------------------------------------------- */

// -------------------------------------------------
// display flags
        DPLFLAGS_TYPE   EE_DisplFlags;                     // original value
static  DPLFLAGS_TYPE   EE_DisplFlags_cmp;                 // compare value
const   DPLFLAGS_TYPE   EE_DisplFlags_def = DEF_DPLFLAGS;  // default value

// -------------------------------------------------
// debug filter flags
        DBGFILT_TYPE    EE_DbgFilter;                      // original value
static  DBGFILT_TYPE    EE_DbgFilter_cmp;                  // compare value
const   DBGFILT_TYPE    EE_DbgFilter_def = DEF_DBGFILT;    // default value
        DBGDETDIR_TYPE  EE_DbgDetails;
static  DBGDETDIR_TYPE  EE_DbgDetails_cmp;
const   DBGDETDIR_TYPE  EE_DbgDetails_def = DEF_DBGDETDIR;

// -------------------------------------------------
/* device flags 1+2+3 */
       DEVFLAGS1_TYPE   EE_DevFlags_1;
static DEVFLAGS1_TYPE   EE_DevFlags_1_cmp;
const  DEVFLAGS1_TYPE   EE_DevFlags_1_def = DEF_DEVFLAGS1;
       DEVFLAGS2_TYPE   EE_DevFlags_2;
static DEVFLAGS2_TYPE   EE_DevFlags_2_cmp;
const  DEVFLAGS2_TYPE   EE_DevFlags_2_def = DEF_DEVFLAGS2;
       DEVFLAGS3_TYPE   EE_DevFlags_3;
static DEVFLAGS3_TYPE   EE_DevFlags_3_cmp;
const  DEVFLAGS3_TYPE   EE_DevFlags_3_def = DEF_DEVFLAGS3;

// -------------------------------------------------
/* Software ID (would never change at application run time) */
       SWVERS_TYPE EE_SWID;
static SWVERS_TYPE EE_SWID_cmp;
const  SWVERS_TYPE EE_SWID_def = DEF_SWID_NUMBER;
       unsigned char szSWVersion[64];       /* buffer to contain formated sw id */

// -------------------------------------------------
/* Bike Logo & Type */
       UINT8 EE_LogoSelect;
static UINT8 EE_LogoSelect_cmp;
#if(BIKE_MOTOBAU==1)
const  UINT8 EE_LogoSelect_def = eLogo_Motobau;
#else // BIKE_MOTOBAU
const  UINT8 EE_LogoSelect_def = eLogo_SIXO;
#endif // BIKE_MOTOBAU
       BIKE_TYPE EE_BikeType;
static BIKE_TYPE EE_BikeType_cmp;
const  BIKE_TYPE EE_BikeType_def = eBIKE_STANDARD;

// -------------------------------------------------
// WheelSize & Impulses
       UINT16 EE_WheelSize;                       // original value in mm
static UINT16 EE_WheelSize_cmp;                   // compare value
const  UINT16 EE_WheelSize_def = DEF_WHEELSIZE;   // default value
       UINT8 EE_Wheel_ImpPRev;                    // wheel impulses/revolution
static UINT8 EE_Wheel_ImpPRev_cmp;
const  UINT8 EE_Wheel_ImpPRev_def = 1;

// -------------------------------------------------
// RPM cylinder correcture factor
       CCF_TYPE EE_CCF;
static CCF_TYPE EE_CCF_cmp;
const  CCF_TYPE EE_CCF_def = DEF_EE_CCF_NOM | DEF_EE_CCF_DENOM;

// -------------------------------------------------
// distances
       DIST_TYPE    NV_VehicDist;
static DIST_TYPE    NV_VehicDist_cmp;
const  DIST_TYPE    NV_VehicDist_def = 0L;
       DIST_TYPE    NV_TripCnt_A;
static DIST_TYPE    NV_TripCnt_A_cmp;
const  DIST_TYPE    NV_TripCnt_A_def = 0L;
       DIST_TYPE    NV_TripCnt_B;
static DIST_TYPE    NV_TripCnt_B_cmp;
const  DIST_TYPE    NV_TripCnt_B_def = 0L;
       DIST_TYPE    NV_TripCom_A;
static DIST_TYPE    NV_TripCom_A_cmp;
const  DIST_TYPE    NV_TripCom_A_def = 0L;
       DIST_TYPE    NV_TripCom_B;
static DIST_TYPE    NV_TripCom_B_cmp;
const  DIST_TYPE    NV_TripCom_B_def = 0L;
       DIST_TYPE    EE_NextSrvKm;
static DIST_TYPE    EE_NextSrvKm_cmp;
const  DIST_TYPE    EE_NextSrvKm_def = 0L;
       DIST_TYPE    NV_FuelDist;
static DIST_TYPE    NV_FuelDist_cmp;
const  DIST_TYPE    NV_FuelDist_def = 0L;

// -------------------------------------------------
// Speed statistics
       SPEED_TYPE   EE_SpeedMax;
static SPEED_TYPE   EE_SpeedMax_cmp;
const  SPEED_TYPE   EE_SpeedMax_def = 0;
       SPEED_TYPE   NV_Speed_AvrM;
static SPEED_TYPE   NV_Speed_AvrM_cmp;
const  SPEED_TYPE   NV_Speed_AvrM_def = 0;
       SPEED_TYPE   NV_Speed_AvrP;
static SPEED_TYPE   NV_Speed_AvrP_cmp;
const  SPEED_TYPE   NV_Speed_AvrP_def = 0;

// -------------------------------------------------
/* Engine speed parameters */
       RPM_TYPE     EE_RPM_Flash;
static RPM_TYPE     EE_RPM_Flash_cmp;
const  RPM_TYPE     EE_RPM_Flash_def = 0;
       RPM_TYPE     EE_RPM_Max;
static RPM_TYPE     EE_RPM_Max_cmp;
const  RPM_TYPE     EE_RPM_Max_def = 0;

// -------------------------------------------------
/* Engine RunTime statistics */
       TIME_TYPE_LD NV_EngRunTime_Srv;
static TIME_TYPE_LD NV_EngRunTime_Srv_cmp;
const  TIME_TYPE_LD NV_EngRunTime_Srv_def = 0L;
       TIME_TYPE_LD NV_EngRunTime_All;
static TIME_TYPE_LD NV_EngRunTime_All_cmp;
const  TIME_TYPE_LD NV_EngRunTime_All_def = 0L;

// -------------------------------------------------
/* voltages */
       UINT16       EE_Volt_Min;
static UINT16       EE_Volt_Min_cmp;
const  UINT16       EE_Volt_Min_def = ANAIN_INVALID_U;
       UINT16       EE_Volt_Max;
static UINT16       EE_Volt_Max_cmp;
const  UINT16       EE_Volt_Max_def = ANAIN_INVALID_U;

// -------------------------------------------------
/* temperatures */
       INT16        EE_TAir_Min;
static INT16        EE_TAir_Min_cmp;
const  INT16        EE_TAir_Min_def = ANAIN_INVALID_S;
       INT16        EE_TAir_Max;
static INT16        EE_TAir_Max_cmp;
const  INT16        EE_TAir_Max_def = ANAIN_INVALID_S;
       INT16        EE_TOil_Min;
static INT16        EE_TOil_Min_cmp;
const  INT16        EE_TOil_Min_def = ANAIN_INVALID_S;
       INT16        EE_TOil_Max;
static INT16        EE_TOil_Max_cmp;
const  INT16        EE_TOil_Max_def = ANAIN_INVALID_S;
       INT16        EE_TWat_Min;
static INT16        EE_TWat_Min_cmp;
const  INT16        EE_TWat_Min_def = ANAIN_INVALID_S;
       INT16        EE_TWat_Max;
static INT16        EE_TWat_Max_cmp;
const  INT16        EE_TWat_Max_def = ANAIN_INVALID_S;

// -------------------------------------------------
/* lap counter */
       TIME_TYPE_LL EE_LapCnt_Time[LAPS_MAX];
static TIME_TYPE_LL EE_LapCnt_Time_cmp[LAPS_MAX];
const  TIME_TYPE_LL EE_LapCnt_Time_def = { 0, 0 };  // reset bMin & bSec
       LCSTATE_TYPE EE_LapCnt_State;
static LCSTATE_TYPE EE_LapCnt_State_cmp;
const  LCSTATE_TYPE EE_LapCnt_State_def = 0;

// -------------------------------------------------
/* Compass */
       COMPASSCNTL_TYPE EE_CompassCtrl;
static COMPASSCNTL_TYPE EE_CompassCtrl_cmp;
const  COMPASSCNTL_TYPE EE_CompassCtrl_def  = 0;    // default: off

// -------------------------------------------------
/* Coolride Heatgrip */
       COOLRIDECNTRL_TYPE     EE_CoolrideCtrl;
static COOLRIDECNTRL_TYPE     EE_CoolrideCtrl_cmp;
const  COOLRIDECNTRL_TYPE     EE_CoolrideCtrl_def = 0;    // default: off


// -------------------------------------------------
/* Language */
       UINT8 EE_LangSelect;         // eeprom value
static UINT8 EE_LangSelect_cmp;
const  UINT8 EE_LangSelect_def = 0; // default: DE

// -------------------------------------------------
/* fuel handling */
       UINT16               EE_FuelCap;              // fuel tank size in 1/10 liters
static UINT16               EE_FuelCap_cmp;
const  UINT16               EE_FuelCap_def = 0;
       UINT8                EE_FuelConsUser;             // fuel consumption in 1/10 liter/100 km
static UINT8                bFuelCons_cmp;
const  UINT8                bFuelCons_def = 0;     
       UINT32               NV_FuelSensImp;            // Fuel sensor Impulses counter since last refuel (NVRAM!)
static UINT32               NV_FuelSensImp_cmp;
const  UINT32               NV_FuelSensImp_def;
       FUELSCNTRL_TYPE      EE_FuelSensCtrl;         // fuel sensor control 
static FUELSCNTRL_TYPE      EE_FuelSensCtrl_cmp;         
const  FUELSCNTRL_TYPE      EE_FuelSensCtrl_def = 0;     


// -------------------------------------------------
/* system start / boot screen */
       UINT8  EE_LogoDelay;      // Boot delay in 1/10 sec
static UINT8  EE_LogoDelay_cmp;
const  UINT8  EE_LogoDelay_def = 20;  // default: 2 sec. delay






/* ========================================================================================== */
/* SYSTEM PARAMETER CONTROL STRUCTURE  - complete NVRAM/EEPROM memory mapping   */
/*                                                                              */
/* NOTE: DO NOT CHANGE ORDER TO KEEP IT COMPATIBLE WITH OLDER SW-VERSIONS!      */

const SYSPARINFO_TYPE  rgSysParControl[] =
{
    /*  Param ID            MemType    Offset   Size of type          addr of data        compare value           default value       */
    /*  ------------------- --------   ------ ----------------------- ------------------  ----------------------  ------------------- */
    {   PID_VEHIC_KM,       NVRAM_SEC,    0,  sizeof(DIST_TYPE),      &NV_VehicDist,      &NV_VehicDist_cmp,      &NV_VehicDist_def      },  // 4 bytes, vehicle overall distance
    {   PID_HOURS_SERV,     NVRAM_SEC,    4,  sizeof(TIME_TYPE_LD),   &NV_EngRunTime_Srv, &NV_EngRunTime_Srv_cmp, &NV_EngRunTime_Srv_def },  // 4 bytes, engine runtime since last service
    {   PID_HOURS_ALL,      NVRAM_SEC,    8,  sizeof(TIME_TYPE_LD),   &NV_EngRunTime_All, &NV_EngRunTime_All_cmp, &NV_EngRunTime_All_def },  // 4 bytes, engine runtime overall
    {   PID_TRIP_A_KM,      NVRAM,       12,  sizeof(DIST_TYPE),      &NV_TripCnt_A,      &NV_TripCnt_A_cmp,      &NV_TripCnt_A_def      },  // 4 bytes, NV_TripCom_Aounter A distance
    {   PID_TRIP_B_KM,      NVRAM,       16,  sizeof(DIST_TYPE),      &NV_TripCnt_B,      &NV_TripCnt_B_cmp,      &NV_TripCnt_B_def      },  // 4 bytes, NV_TripCom_Aounter B distance
    {   PID_TRIP_C_KM,      NVRAM,       20,  sizeof(DIST_TYPE),      &NV_TripCom_A,      &NV_TripCom_A_cmp,      &NV_TripCom_A_def      },  // 4 bytes, NV_TripCom_Aounter C distance
    {   PID_TRIP_D_KM,      NVRAM,       24,  sizeof(DIST_TYPE),      &NV_TripCom_B,      &NV_TripCom_B_cmp,      &NV_TripCom_B_def      },  // 4 bytes, NV_TripCom_Aounter D distance
    {   PID_SPEED_AVR_M,    NVRAM,       28,  sizeof(SPEED_TYPE),     &NV_Speed_AvrM,     &NV_Speed_AvrM_cmp,     &NV_Speed_AvrM_def     },  // 2 bytes, average speed EX-cluding pause times
    {   PID_SPEED_AVR_P,    NVRAM,       30,  sizeof(SPEED_TYPE),     &NV_Speed_AvrP,     &NV_Speed_AvrP_cmp,     &NV_Speed_AvrP_def     },  // 2 bytes, average speed IN-cluding pause times
    {   PID_FUEL_KM,        NVRAM,       32,  sizeof(DIST_TYPE),      &NV_FuelDist,       &NV_FuelDist_cmp,       &NV_FuelDist_def       },  // 4 bytes, distance since last refuel
    {   PID_FUEL_SENSOR,    NVRAM,       36,  sizeof(UINT32),         &NV_FuelSensImp,    &NV_FuelSensImp_cmp,    &NV_FuelSensImp_def    },  // 4 bytes, Fuel sensor Impulses counter since last refuel 
                                       /* ^                                              */
                                       /* |                                              */
                                       /* +---  ATTENTION: MAX 56 BYTES NVRAM (RTC+Battery) AVAILABLE! */

    /*  ------------------- --------   ------ ----------------------- ------------------  ----------------------  ------------------- */

    /*  Param ID            MemType    Offset   Size of type          addr of data        compare value           default value       */
    /*  ------------------- --------   ------ ----------------------- ------------------  ----------------------  ------------------- */
    {   PID_SWVERSION,      EEPROM,      0,   sizeof(UINT16),         &EE_SWID,           &EE_SWID_cmp,           &EE_SWID_def        },  // 2 bytes, 4 software version nibbles
    {   PID_WHEELSIZE,      EEPROM_SEC,  2,   sizeof(UINT16),         &EE_WheelSize,      &EE_WheelSize_cmp,      &EE_WheelSize_def   },  // 2 bytes, wheel circumcistence in mm
    {   PID_CCF,            EEPROM_SEC,  4,   sizeof(CCF_TYPE),       &EE_CCF,            &EE_CCF_cmp,            &EE_CCF_def         },  // 1 byte, two cylinder correcture factor nibbles
    {   PID_DEVFLAGS1,      EEPROM,      5,   sizeof(DEVFLAGS1_TYPE), &EE_DevFlags_1,     &EE_DevFlags_1_cmp,     &EE_DevFlags_1_def  },  // 1 byte, bitfield for system status
    {   PID_DPLFL,          EEPROM_SEC,  6,   sizeof(DPLFLAGS_TYPE),  &EE_DisplFlags,     &EE_DisplFlags_cmp,     &EE_DisplFlags_def  },  // 4 byte, bitfield for display control
    {   PID_DBGFILTFL,      EEPROM,     10,   sizeof(DBGFILT_TYPE),   &EE_DbgFilter,      &EE_DbgFilter_cmp,      &EE_DbgFilter_def   },  // 1 byte, bitfield for debug filters
    {   PID_DBGDETDIRFL,    EEPROM,     11,   sizeof(DBGDETDIR_TYPE), &EE_DbgDetails,     &EE_DbgDetails_cmp,     &EE_DbgDetails_def  },  // 1 byte, bitfield for debug details & direction
    {   PID_LOGO,           EEPROM,     12,   sizeof(UINT8),          &EE_LogoSelect,     &EE_LogoSelect_cmp,     &EE_LogoSelect_def  },  // 1 byte, number of currently selected logo
    {   PID_DEVFLAGS2,      EEPROM,     13,   sizeof(DEVFLAGS2_TYPE), &EE_DevFlags_2,     &EE_DevFlags_2_cmp,     &EE_DevFlags_2_def  },  // 1 byte, trip counter flags
    {   PID_SPEED_MAX,      EEPROM,     14,   sizeof(SPEED_TYPE),     &EE_SpeedMax,       &EE_SpeedMax_cmp,       &EE_SpeedMax_def    },  // 2 bytes, maximum vehicle speed
    {   PID_RPM_MAX,        EEPROM,     16,   sizeof(RPM_TYPE),       &EE_RPM_Max,        &EE_RPM_Max_cmp,        &EE_RPM_Max_def     },  // 2 bytes, maximum engine speed
    {   PID_RPM_FLASH,      EEPROM,     18,   sizeof(RPM_TYPE),       &EE_RPM_Flash,      &EE_RPM_Flash_cmp,      &EE_RPM_Flash_def   },  // 2 bytes, engine speed to enable flash
    {   PID_VOLT_MIN,       EEPROM,     20,   sizeof(UINT16),         &EE_Volt_Min,       &EE_Volt_Min_cmp,       &EE_Volt_Min_def    },  // 2 bytes, battery voltage minimum
    {   PID_VOLT_MAX,       EEPROM,     22,   sizeof(UINT16),         &EE_Volt_Max,       &EE_Volt_Max_cmp,       &EE_Volt_Max_def    }, // 2 bytes, battery voltage maximum
    {   PID_TAIR_MIN,       EEPROM,     24,   sizeof(INT16),          &EE_TAir_Min,       &EE_TAir_Min_cmp,       &EE_TAir_Min_def    },  // 2 bytes, air temperature minimum
    {   PID_TAIR_MAX,       EEPROM,     26,   sizeof(INT16),          &EE_TAir_Max,       &EE_TAir_Max_cmp,       &EE_TAir_Max_def    },  // 2 bytes, air temperature maximum
    {   PID_TOIL_MIN,       EEPROM,     28,   sizeof(INT16),          &EE_TOil_Min,       &EE_TOil_Min_cmp,       &EE_TOil_Min_def    },  // 2 bytes, oil temperature minimum
    {   PID_TOIL_MAX,       EEPROM,     30,   sizeof(INT16),          &EE_TOil_Max,       &EE_TOil_Max_cmp,       &EE_TOil_Max_def    },  // 2 bytes, oil temperature maximum
    {   PID_TWAT_MIN,       EEPROM,     32,   sizeof(INT16),          &EE_TWat_Min,       &EE_TWat_Min_cmp,       &EE_TWat_Min_def    },  // 2 bytes, water temperature minimum
    {   PID_TWAT_MAX,       EEPROM,     34,   sizeof(INT16),          &EE_TWat_Max,       &EE_TWat_Max_cmp,       &EE_TWat_Max_def    },  // 2 bytes, water temperature maximum
    {   PID_BIKETYPE,       EEPROM,     36,   sizeof(UINT8),          &EE_BikeType,       &EE_BikeType_cmp,       &EE_BikeType_def    },  // 1 byte, definition of bike version (enum)
    {   PID_SERV_KM,        EEPROM,     37,   sizeof(DIST_TYPE),      &EE_NextSrvKm,      &EE_NextSrvKm_cmp,      &EE_NextSrvKm_def   },  // 4 bytes, km, at which next service is required
    {   PID_FUEL_CAP,       EEPROM,     41,   sizeof(UINT16),         &EE_FuelCap,        &EE_FuelCap_cmp,        &EE_FuelCap_def     },  // 2 bytes, fuel reservoir in 1/10 liters
    {   PID_FUEL_CONS,      EEPROM,     43,   sizeof(UINT8),          &EE_FuelConsUser,   &bFuelCons_cmp,         &bFuelCons_def      },  // 1 byte,  fuel consumption in 1/10 l/100km
    {   PID_BOOT_DELAY,     EEPROM,     44,   sizeof(UINT8),          &EE_LogoDelay,      &EE_LogoDelay_cmp,      &EE_LogoDelay_def   },  // 1 byte,  delay in 1/10 s at power up
    {   PID_WHEEL_IMP,      EEPROM,     45,   sizeof(UINT8),          &EE_Wheel_ImpPRev,  &EE_Wheel_ImpPRev_cmp,  &EE_Wheel_ImpPRev_def}, // 1 byte,  number of impulses/wheel rotation
    {   PID_DEVFLAGS3,      EEPROM,     46,   sizeof(DEVFLAGS3_TYPE), &EE_DevFlags_3,     &EE_DevFlags_3_cmp,     &EE_DevFlags_3_def  },  // 1 byte,  bitfield for system status
    {   PID_COOLR_CNTRL,    EEPROM,     47,   sizeof(UINT8),          &EE_CoolrideCtrl,   &EE_CoolrideCtrl_cmp,   &EE_CoolrideCtrl_def},  // 1 byte,  bitfield for Coolride Heatgrip Control
    {   PID_COMPS_CNTRL,    EEPROM,     48,   sizeof(UINT8),          &EE_CompassCtrl,    &EE_CompassCtrl_cmp,    &EE_CompassCtrl_def },  // 1 byte,  bitfield for Compass Module Control
    {   PID_LANGUAGE,       EEPROM,     49,   sizeof(UINT8),          &EE_LangSelect,     &EE_LangSelect_cmp,     &EE_LangSelect_def  },  // 1 byte,  language indicator
    {   PID_FUELS_CNTRL,    EEPROM,     50,   sizeof(FUELSCNTRL_TYPE),&EE_FuelSensCtrl,   &EE_FuelSensCtrl_cmp,   &EE_FuelSensCtrl_def }, // 4 byte,  bitfield for FuelSensor Control
    
    {   PID_LAPCSTAT,       EEPROM,    136,   sizeof(LCSTATE_TYPE),   &EE_LapCnt_State,   &EE_LapCnt_State_cmp,   &EE_LapCnt_State_def},  // 1 bytes, lap counter status
    {   PID_LAPC_0 ,        EEPROM,    137,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[0 ],&EE_LapCnt_Time_cmp[0 ],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_1 ,        EEPROM,    139,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[1 ],&EE_LapCnt_Time_cmp[1 ],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_2 ,        EEPROM,    141,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[2 ],&EE_LapCnt_Time_cmp[2 ],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_3 ,        EEPROM,    143,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[3 ],&EE_LapCnt_Time_cmp[3 ],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_4 ,        EEPROM,    145,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[4 ],&EE_LapCnt_Time_cmp[4 ],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_5 ,        EEPROM,    147,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[5 ],&EE_LapCnt_Time_cmp[5 ],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_6 ,        EEPROM,    149,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[6 ],&EE_LapCnt_Time_cmp[6 ],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_7 ,        EEPROM,    151,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[7 ],&EE_LapCnt_Time_cmp[7 ],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_8 ,        EEPROM,    153,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[8 ],&EE_LapCnt_Time_cmp[8 ],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_9 ,        EEPROM,    155,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[9 ],&EE_LapCnt_Time_cmp[9 ],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_10,        EEPROM,    157,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[10],&EE_LapCnt_Time_cmp[10],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_11,        EEPROM,    159,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[11],&EE_LapCnt_Time_cmp[11],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_12,        EEPROM,    161,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[12],&EE_LapCnt_Time_cmp[12],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_13,        EEPROM,    163,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[13],&EE_LapCnt_Time_cmp[13],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_14,        EEPROM,    165,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[14],&EE_LapCnt_Time_cmp[14],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
    {   PID_LAPC_15,        EEPROM,    167,   sizeof(TIME_TYPE_LL),   &EE_LapCnt_Time[15],&EE_LapCnt_Time_cmp[15],&EE_LapCnt_Time_def },  // 2 bytes, lap counter time struct
                                      /* ^                                                 */
                                      /* |                                                 */
                                      /* +---  ATTENTION: MAX 1024 BYTES EEPROM AVAILABLE! */

    /*  ------------------- --------   ------ ----------------------- ------------------  ----------------------  ------------------- */
};


/* number of system parameters in control structure array */
#define NROFSYSPAR  (sizeof(rgSysParControl)/sizeof(SYSPARINFO_TYPE))


/* ========================================================================================== */







/***********************************************************************
 *  FUNCTION:       SysPar_GetSysParamInfo
 *  DESCRIPTION:    returns a pointer to the requested system parameter
 *                  info structure (not the sys parameter itself!)
 *  PARAMETER:      PARAM_ID_TYPE eSearchedID   searched parameter ID
 *  RETURN:         address of requested system parameter info structure
 *                  NULL if not found!
 *  COMMENT:        to be used before reading / writing a system parameter
 *********************************************************************** */
SYSPARINFO_TYPE far * SysPar_GetSysParamInfo(const PARAM_ID_TYPE eSearchedID)
{
    UINT8 bIndex;
    for (bIndex = 0; bIndex < NROFSYSPAR; bIndex++)
    {
        if (rgSysParControl[bIndex].eParamID == eSearchedID)
        {
            return &rgSysParControl[bIndex];
        }
    }
    ODS1(DBG_SYS,DBG_ERROR,"Unknown SystemParameter PID: %u", eSearchedID);
    return NULL;
}



/***********************************************************************
 *  FUNCTION:       SysPar_ReadSysParam
 *  DESCRIPTION:    Reads the requested system variable from EEPROM or NVRAM
 *                  and fills the given buffer
 *  PARAMETER:      PARAM_ID_TYPE   eParamID   enum type of requested system parameter
 *                  void far *      pointer to prepared return buffer
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE SysPar_ReadSysParam(const PARAM_ID_TYPE eParamID, void far * fpParamBuffer)
{
    UINT8                   LocalBuffer[MAXPARAMSIZE];
    SYSPARINFO_TYPE far *   fpParamInfo;
    ERRCODE                 RValue = ERR_PARAM_ERR;

    /* parameter check: */
    fpParamInfo = SysPar_GetSysParamInfo(eParamID);     /* parameter ID exists? */
    if (  (fpParamInfo == NULL  )
        ||(fpParamBuffer == NULL) )
    {
        return ERR_PARAM_ERR;
    }

    /* memory type choice */
    switch (fpParamInfo->eMem)
    {
        case NVRAM:     // intentionally no break here!
        case NVRAM_SEC:
            // read from NVRAM
            RValue = iicNvramRead( fpParamInfo->wStartAddr, fpParamInfo->bSize, LocalBuffer );
            break;
        case EEPROM:     // intentionally no break here!
        case EEPROM_SEC:
            // read from EEPROM
            RValue = iicEepromRead( fpParamInfo->wStartAddr, fpParamInfo->bSize, LocalBuffer );
            break;
        default:
            ODS1(DBG_SYS,DBG_ERROR,"Invalid memory type [%u] detected!", fpParamInfo->eMem);
            break;
    }

    /* make a local copy to prevent near/far ptr compiler errors */
    if (RValue != ERR_OK)
        ODS1(DBG_SYS,DBG_ERROR,"Can not read system parameter PID [%u]!", eParamID);
    else
        memcpy(fpParamBuffer, LocalBuffer, fpParamInfo->bSize);
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       SysPar_WriteSysParam
 *  DESCRIPTION:    Writes the given system variable into EEPROM or NVRAM
 *  PARAMETER:      PARAM_ID_TYPE   eParamID   enum type of given system parameter
 *                  void far *      pointer to given parameter
 *  RETURN:         ERRCODE         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE SysPar_WriteSysParam (const PARAM_ID_TYPE eParamID, void far * fpGivenParam)
{
    UINT8                 LocalBuffer[MAXPARAMSIZE];
    SYSPARINFO_TYPE far * fpParamInfo;
    ERRCODE               RValue = ERR_PARAM_ERR;

    /* parameter check: */
    fpParamInfo = SysPar_GetSysParamInfo(eParamID);     /* parameter ID exists? */
    if (  (fpParamInfo == NULL )
        ||(fpGivenParam == NULL) )
    {
        return ERR_PARAM_ERR;
    }

    /* make a local copy to prevent near/far ptr compiler errors */
    memcpy(LocalBuffer, fpGivenParam, fpParamInfo->bSize);

    /* memory type choice */
    switch (fpParamInfo->eMem)
    {
        case NVRAM:     // intentionally no break here!
        case NVRAM_SEC:
            //write into NVRAM
            RValue = iicNvramWrite( fpParamInfo->wStartAddr, fpParamInfo->bSize, LocalBuffer );
            break;
        case EEPROM:     // intentionally no break here!
        case EEPROM_SEC:
            // write into EEPROM
            RValue = iicEepromWrite( fpParamInfo->wStartAddr, fpParamInfo->bSize, LocalBuffer );
            break;
        default:
            ODS1(DBG_SYS,DBG_ERROR,"Invalid memory type [%u] detected!", fpParamInfo->eMem);
            break;
    }

    /* check for errors */
    if (RValue != ERR_OK)
        ODS1(DBG_SYS,DBG_ERROR,"Can not save system parameter PID %u!", eParamID);
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       SysPar_SetDefaults
 *  DESCRIPTION:    Re-Write all nvram/eeprom parameters with default
 *                  data.
 *  PARAMETER:      fComplete  TRUE:  overwrites WHEELSIZE & EE_CCF & NV_VehicDist too
 *                             FALSE: leaves WHEELSIZE & EE_CCF & NV_VehicDist unchanged
 *  RETURN:         error code
 *  COMMENT:        - To be used after first power up or while
 *                    reseting to 'factory defaults'
 *                  - Manual reset eeprom/nvram by removing the
 *                    'magic number' in eeprom.
 *********************************************************************** */
ERRCODE SysPar_SetDefaults(BOOL fComplete)
{
    ERRCODE RValue = ERR_OK;
    UINT8   cPara;              // just a loop variable

    // loop over all parameters
    for ( cPara = 0; cPara < NROFSYSPAR; cPara++ )
    {
        // check: shall this parameter NOT be cleared with SW update? (e.g. WHEELSIZE, ..)
        if (  ( fComplete == FALSE )
            &&(  ( rgSysParControl[cPara].eMem == NVRAM_SEC )
               ||( rgSysParControl[cPara].eMem == EEPROM_SEC) ) )
        {
            ODS1(DBG_SYS,DBG_ERROR,"  (Parameter [%u] remains unchanged!)", rgSysParControl[cPara].eParamID);
            continue;   // omit this parameter, go on with next...
        }

#ifndef WE_USE_DBGSETTINGS_FROM_EEPROM
        // this code is temporarely disabled, until we can edit this in menue setting
        // -> effect: main.c init-settings will remain active & will never changed!
        if (  ( rgSysParControl[cPara].eParamID == PID_DBGFILTFL   )
            ||( rgSysParControl[cPara].eParamID == PID_DBGDETDIRFL ) )
            continue;   // omit this parameter, go on with next...
#endif // WE_USE_DBGSETTINGS_FROM_EEPROM

        // Update current data & compare value!
        memcpy( rgSysParControl[cPara].fpData,    rgSysParControl[cPara].fpDefault, rgSysParControl[cPara].bSize );
        memcpy( rgSysParControl[cPara].fpCompare, rgSysParControl[cPara].fpDefault, rgSysParControl[cPara].bSize );

        // debug out!
        SysPar_DebugOutParameter( rgSysParControl[cPara].eParamID );

        // write default value into EEPROM/NVRAM
        RValue = SysPar_WriteSysParam( rgSysParControl[cPara].eParamID, rgSysParControl[cPara].fpDefault);
        if (RValue != ERR_OK)
            ODS1(DBG_SYS,DBG_ERROR,"Error while initializing PID:%u", rgSysParControl[cPara].eParamID);

    }
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       SysPar_CheckFirstInit
 *  DESCRIPTION:    Checks the eeprom software ID.
 *                  If its not up2date, it (completely) renews
 *                  eeprom/nvram memory content to defaults
 *  PARAMETER:      -
 *  RETURN:         error code
 *  COMMENT:        resetting the 'magic number' forces complete re-initialize of eeprom
 *********************************************************************** */
ERRCODE SysPar_CheckFirstInit(void)
{
    ERRCODE     RValue  = ERR_OK;
    SWVERS_TYPE SWID    = 0x0;

    /* read eeprom saved software version number */
    RValue = SysPar_ReadSysParam(PID_SWVERSION, &SWID);
    /*ODS4(DBG_SYS,DBG_INFO,"EEPROM Software Version: %u.%u.%u.%c",
            SWID.Fields.apl, SWID.Fields.swv, SWID.Fields.bld, SWID.Fields.spc+APLNUMCOFFS); */

    /* First EEPROM use or user forced eeprom init?
       (indicated by 0xff in complete eeprom memory) */
    if ( SysPar_CheckMagicNumber() == FALSE)
    //if (SWID.Number == 0xFFFF)
    {
        /* renew the COMPLETE nvram & eeprom */
        ODS(DBG_SYS,DBG_INFO,"\nFirst use of EEPROM/NVRAM: Complete Init!");
        RValue = SysPar_SetDefaults(COMPLETE);
    }

    /* check software ID */
    else if (SWID.Number != DEF_SWID_NUMBER)
    {
        /*renew the nvram & eeprom */
        ODS(DBG_SYS,DBG_INFO,"\nNew SW version: Re-Newing EEPROM/NVRAM!");
        ODS(DBG_SYS,DBG_INFO,"(Some basic parameters will remain unchanged!)");
        RValue = SysPar_SetDefaults(PARTIAL);
    }
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       SysPar_CyclicSaveValues
 *  DESCRIPTION:    Cyclicly called routine to asure all necessary
 *                  measurement data to be saved in EEPROM / NVRAM
 *  PARAMETER:      -
 *  RETURN:         error code
 *  COMMENT:        - Does only write into eeprom if any changes made in
 *                    application data.
 *                  - Reduces sytem load by only saving one value every 200 ms.
 *********************************************************************** */
ERRCODE SysPar_CyclicSaveValues(void)
{
    static  BOOL        fInit           = FALSE;            // to check for initialized static vars
    static  UINT8       cCurrentPara    = 0;                // indicates current parameter to be handled
    static  UINT16      wLastCall       = 0;                // time stamp of last NVRAM parameter saved
    UINT16              wThisCall       = 0;                // time stamp of this call
    ERRCODE             RValue          = ERR_OK;           // return value*/

    /* init of compare values at first call only */
    if (fInit == FALSE)
    {
        UINT8 cPara;    // just a loop variable

        // initialize all compare values
        for ( cPara = 0; cPara < NROFSYSPAR; cPara++ )
        {
            // copy current data to compare data
            // NOTE: current data has already been initialized by SysPar_InitSystemPar() before main() loop.
            memcpy( rgSysParControl[cPara].fpCompare, rgSysParControl[cPara].fpData, rgSysParControl[cPara].bSize );
        }
        // never do this again
        fInit = TRUE;
    }

    /* check: time to check one system parameter? */
    TimerGetSys_msec(wThisCall);
    if ( (wThisCall-wLastCall) > PARAM_CYCLE)
    {
        wLastCall = wThisCall;          /* update time stamp */

        // check current parameter
        if ( memcmp( rgSysParControl[cCurrentPara].fpCompare, rgSysParControl[cCurrentPara].fpData, rgSysParControl[cCurrentPara].bSize) != 0 )
        {
            SysPar_DebugOutParameter( rgSysParControl[cCurrentPara].eParamID );

            // Difference found! -> save parameter into EEPROM/NVRAM
            RValue = SysPar_WriteSysParam( rgSysParControl[cCurrentPara].eParamID, rgSysParControl[cCurrentPara].fpData);
            if (RValue != ERR_OK)
                ODS1(DBG_SYS,DBG_ERROR,"Error saving PID:%u", rgSysParControl[cCurrentPara].eParamID);
            // Update compare value too!
            memcpy( rgSysParControl[cCurrentPara].fpCompare, rgSysParControl[cCurrentPara].fpData, rgSysParControl[cCurrentPara].bSize );
        }

        // Increment to next parameter (wrap around, if necessary)
        if ( cCurrentPara < (NROFSYSPAR-1) )
             cCurrentPara++;
        else cCurrentPara = 0;
    }
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       SysPar_InitSystemPar
 *  DESCRIPTION:    Reads all system parameters from eeprom/nvram
 *                  to initialize them as global variables
 *  PARAMETER:      -
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE SysPar_InitSystemPar (void)
{
    ERRCODE RValue = ERR_OK;    // return value
    UINT8   cPara;              // just a loop variable

    // loop over all parameters
    ODS(DBG_SYS,DBG_INFO,"\nReading system init values from EEPROM/NVRAM:");
    for ( cPara = 0; cPara < NROFSYSPAR; cPara++ )
    {

#ifndef WE_USE_DBGSETTINGS_FROM_EEPROM
        // this code is temporarely disabled, until we can edit this in menue setting
        // -> effect: main.c init-settings will remain active & will never changed!
        if (  ( rgSysParControl[cPara].eParamID == PID_DBGFILTFL   )
            ||( rgSysParControl[cPara].eParamID == PID_DBGDETDIRFL ) )
            continue;   // omit this parameter, go on with next...
#endif // WE_USE_DBGSETTINGS_FROM_EEPROM

        // get current parameter from EEPROM/NVRAM
        RValue = SysPar_ReadSysParam(rgSysParControl[cPara].eParamID, rgSysParControl[cPara].fpData);
        if (RValue != ERR_OK)
            ODS1(DBG_SYS,DBG_ERROR,"Error reading PID:%u", rgSysParControl[cPara].eParamID);

        // debug out!
        SysPar_DebugOutParameter( rgSysParControl[cPara].eParamID );
    }

    // setup the SW version
    SysPar_SetupSWVersionStr();

    // check: invalid NVRAM parameters? (may be caused by battery defect)
    if ( NV_VehicDist.km > DIST_MAX_VEHIC )    // invalid?
    {   NV_VehicDist.km = 0;
        fRTCDefect = TRUE;
        ODS(DBG_SYS,DBG_WARNING,"Invalid 'NV_VehicDist', RTC-Battery defect?" );
    }
    if ( NV_TripCnt_A.dkm > DIST_MAX_TRIP )        // invalid?
         NV_TripCnt_A.dkm = 0;
    if ( NV_TripCnt_B.dkm > DIST_MAX_TRIP )        // invalid?
         NV_TripCnt_B.dkm = 0;
    if ( NV_TripCom_A.dkm > DIST_MAX_TRIP )        // invalid?
         NV_TripCom_A.dkm = 0;
    if ( NV_TripCom_B.dkm > DIST_MAX_TRIP )        // invalid?
         NV_TripCom_B.dkm = 0;
    if ( NV_FuelDist.km > DIST_MAX_VEHIC )     // invalid?
         NV_FuelDist.km = 0;
    if ( NV_EngRunTime_All.wHour > SURV_ENGRTIME_ALL_MAX )
         NV_EngRunTime_All.wHour = 0;
    if ( NV_EngRunTime_Srv.wHour > SURV_ENGRTIME_SRV_MAX )
         NV_EngRunTime_Srv.wHour = 0;


    return RValue;
}



/***********************************************************************
 *  FUNCTION:       SysPar_CheckMagicNumber
 *  DESCRIPTION:    Checks existance of 'magic number'
 *                  (a word in EEPROMs most top address) and writes
 *                  the magic number if not found
 *  PARAMETER:      -
 *  RETURN:         TRUE    indicates magic number found
 *                  FALSE   indicates magic number missed
 *  COMMENT:        Used to detect a hardware (eq. EEPROM) which was
 *                  NEVER used before.
 *********************************************************************** */
BOOL SysPar_CheckMagicNumber ( void )
{
    UINT16 wMagicAdr    = 0;
    UINT16 wMagicNumber = 0;

    /* read out current number */
    wMagicAdr = iicEepromSize() - EEPR_MAGICNUM_SIZE;
    iicEepromRead( wMagicAdr, EEPR_MAGICNUM_SIZE, (UINT8 *)(&wMagicNumber) );

    /* check that number: found? */
    if ( wMagicNumber != EEPR_MAGICNUM_VAL )
    {
        /* write into EEPROM */
        wMagicNumber = EEPR_MAGICNUM_VAL;
        iicEepromWrite( wMagicAdr, EEPR_MAGICNUM_SIZE, (UINT8 *)(&wMagicNumber) );
        return FALSE;
    }
    return TRUE;
}



/***********************************************************************
 *  FUNCTION:       SysPar_SetupSWVersionStr
 *  DESCRIPTION:    Setups a string buffer containing the software
 *                  version as formated string
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Objects should use global 'szSWVersion' as reference
 *********************************************************************** */
void SysPar_SetupSWVersionStr( void )
{
    /* get formated sw version */
    szSWVersion[0] = 0x0;
    sprintf( szSWVersion, "V%u.%u.%u", EE_SWID.Fields.apl, EE_SWID.Fields.swv, EE_SWID.Fields.bld);

    /* insert language */
    strcat ( szSWVersion, "-");
    strcat ( szSWVersion, RESTXT_LANG );

    /* add formated debug infos for special versions */
    #if(DEBUG==1)
        strcat ( szSWVersion, RESTXT_SWVER_DEBUG);
    #endif
    #if(KD30_USED==1)
        strcat ( szSWVersion, RESTXT_SWVER_MINIEMU);
    #endif
    #if(COMPASSDRV==1)
        strcat ( szSWVersion, RESTXT_SWVER_COMPASS);
    #endif

    /* limit string to max 21 chars */
    szSWVersion[21] = 0x0;

}



/***********************************************************************
 *  FUNCTION:       SysPar_DebugOutParameter
 *  DESCRIPTION:    Just a helper function to write clear formated
 *                  parameter data to debug out screen, prints out
 *                  current data of given parameter in one line
 *  PARAMETER:      eParamID    ID of parameter to be shown
 *  RETURN:         -
 *  COMMENT:        To be used e.g. after preceding comment line like
 *                  'Now initialzing data: ...'
 *********************************************************************** */
void SysPar_DebugOutParameter( const PARAM_ID_TYPE PID )
{
    switch (PID)                    /* choice of system parameter to be saved */
    {
        case PID_VEHIC_KM:      ODS2(DBG_SYS,DBG_INFO, "- NV NV_VehicDist:  %6lu,%.2lu km",    NV_VehicDist.dkm/100L, NV_VehicDist.dkm-(NV_VehicDist.dkm/100L)*100); break;
        case PID_TRIP_A_KM:     ODS2(DBG_SYS,DBG_INFO, "- NV NV_TripCnt_A:      %6lu,%.2lu km",    NV_TripCnt_A.dkm/100L, NV_TripCnt_A.dkm-(NV_TripCnt_A.dkm/100L)*100); break;
        case PID_TRIP_B_KM:     ODS2(DBG_SYS,DBG_INFO, "- NV NV_TripCnt_B:      %6lu,%.2lu km",    NV_TripCnt_B.dkm/100L, NV_TripCnt_B.dkm-(NV_TripCnt_B.dkm/100L)*100); break;
        case PID_TRIP_C_KM:     ODS2(DBG_SYS,DBG_INFO, "- NV NV_TripCom_A:      %6lu,%.2lu km",    NV_TripCom_A.dkm/100L, NV_TripCom_A.dkm-(NV_TripCom_A.dkm/100L)*100); break;
        case PID_TRIP_D_KM:     ODS2(DBG_SYS,DBG_INFO, "- NV NV_TripCom_B:      %6lu,%.2lu km",    NV_TripCom_B.dkm/100L, NV_TripCom_B.dkm-(NV_TripCom_B.dkm/100L)*100); break;
        case PID_SPEED_AVR_M:   ODS2(DBG_SYS,DBG_INFO, "- NV NV_Speed_AvrM: %6u,%.2u km/h",    NV_Speed_AvrM/100, NV_Speed_AvrM-(NV_Speed_AvrM/100)*100); break;
        case PID_SPEED_AVR_P:   ODS2(DBG_SYS,DBG_INFO, "- NV NV_Speed_AvrP: %6u,%.2u km/h",    NV_Speed_AvrP/100, NV_Speed_AvrP-(NV_Speed_AvrP/100)*100); break;
        case PID_FUEL_KM:       ODS2(DBG_SYS,DBG_INFO, "- NV NV_FuelDist:   %6lu,%.2lu km",    NV_FuelDist.dkm/100L, NV_FuelDist.dkm-(NV_FuelDist.dkm/100L)*100); break;
        case PID_FUEL_SENSOR:   ODS1(DBG_SYS,DBG_INFO, "- NV FuelSens:   %6lu Imp.",        NV_FuelSensImp); break;
        case PID_HOURS_SERV:    ODS3(DBG_SYS,DBG_INFO, "- NV EngTimeSrv: %2u:%02u:%02u",    NV_EngRunTime_Srv.wHour, NV_EngRunTime_Srv.bMin, NV_EngRunTime_Srv.bSec); break;
        case PID_HOURS_ALL:     ODS3(DBG_SYS,DBG_INFO, "- NV EngTimeAll: %2u:%02u:%02u",    NV_EngRunTime_All.wHour, NV_EngRunTime_All.bMin, NV_EngRunTime_All.bSec); break;

        case PID_SWVERSION:     ODS4(DBG_SYS,DBG_INFO, "- EE SW-ID:      %u.%u.%u.%c",      EE_SWID.Fields.apl, EE_SWID.Fields.swv, EE_SWID.Fields.bld, EE_SWID.Fields.spc+APLNUMCOFFS); break;
        case PID_WHEELSIZE:     ODS1(DBG_SYS,DBG_INFO, "- EE WheelSize:  %6u mm",           EE_WheelSize); break;
        case PID_CCF:       ODS2(DBG_SYS,DBG_INFO, "- EE EE_CCF:        %u/%u",            EE_CCF.nibble.nom, EE_CCF.nibble.denom); break;
        case PID_DEVFLAGS1:     ODS2(DBG_SYS,DBG_INFO, "- EE DevFlags1:  DEV:%s MAIND:%u",    szDevName[EE_DevFlags_1.flags.ActDevNr], EE_DevFlags_1.flags.MainDevState); break;
        case PID_DEVFLAGS2:     ODS8(DBG_SYS,DBG_INFO, "- EE DevFlags2:  TC:%u B:%u DL:%u DA:%u M:%u LW:%u VS:%u HC:%u",
                                                                                            EE_DevFlags_2.flags.NV_TripCom_ALongDistUp,
                                                                                            EE_DevFlags_2.flags.BeeperAvail,
                                                                                            EE_DevFlags_2.flags.DLS_Auto,
                                                                                            EE_DevFlags_2.flags.DLS_Active,
                                                                                            EE_DevFlags_2.flags.Metric,
                                                                                            EE_DevFlags_2.flags.LedWarnMode,
                                                                                            EE_DevFlags_2.flags.VehicSimul,
                                                                                            EE_DevFlags_2.flags.Hardcopy   ) ; break;
        case PID_DPLFL:         ODS4(DBG_SYS,DBG_INFO, "- EE DplFlags:   C:%u BLL:%u BLOL:%u Inv:%u",
                                                                                            EE_DisplFlags.flags.ContrLev,
                                                                                            EE_DisplFlags.flags.BacklLev,
                                                                                            EE_DisplFlags.flags.BacklOnLevel,
                                                                                            EE_DisplFlags.flags.fDplInvers);  break;
        case PID_DBGFILTFL:     ODS1(DBG_SYS,DBG_INFO, "- EE DbgFilter:  0x%x ", EE_DbgFilter); break;
        case PID_DBGDETDIRFL:   ODS1(DBG_SYS,DBG_INFO, "- EE DbgDetail:  0x%x", EE_DbgDetails); break;
        case PID_LOGO:          ODS1(DBG_SYS,DBG_INFO, "- EE Logo:       %u", EE_LogoSelect ) ; break;
        case PID_SPEED_MAX:     ODS1(DBG_MEAS,DBG_INFO,"- EE EE_SpeedMax:  %6u km/h", EE_SpeedMax); break;
        case PID_RPM_MAX:       ODS1(DBG_MEAS,DBG_INFO,"- EE RPM Max:    %6u RPM", EE_RPM_Max); break;
        case PID_RPM_FLASH:     ODS1(DBG_MEAS,DBG_INFO,"- EE RPM Flash:  %6u RPM", EE_RPM_Flash); break;
        case PID_VOLT_MIN:      ODS2(DBG_MEAS,DBG_INFO,"- EE Volt Min:   %3u,%.2u V", EE_Volt_Min/100, EE_Volt_Min-(EE_Volt_Min/100)*100); break;
        case PID_VOLT_MAX:      ODS2(DBG_MEAS,DBG_INFO,"- EE Volt Max:   %3u,%.2u V", EE_Volt_Max/100, EE_Volt_Max-(EE_Volt_Max/100)*100); break;
        case PID_TAIR_MIN:      ODS1(DBG_MEAS,DBG_INFO,"- EE TAIR Min:   %5i °C", EE_TAir_Min); break;
        case PID_TAIR_MAX:      ODS1(DBG_MEAS,DBG_INFO,"- EE TAIR Max:   %5i °C", EE_TAir_Max); break;
        case PID_TOIL_MIN:      ODS1(DBG_MEAS,DBG_INFO,"- EE TOIL Min:   %5i °C", EE_TOil_Min); break;
        case PID_TOIL_MAX:      ODS1(DBG_MEAS,DBG_INFO,"- EE TOIL Max:   %5i °C", EE_TOil_Max); break;
        case PID_TWAT_MIN:      ODS1(DBG_MEAS,DBG_INFO,"- EE TWAT Min:   %5i °C", EE_TWat_Min); break;
        case PID_TWAT_MAX:      ODS1(DBG_MEAS,DBG_INFO,"- EE TWAT Max:   %5i °C", EE_TWat_Max); break;
        case PID_BIKETYPE:      ODS1(DBG_SYS,DBG_INFO, "- EE BikeType:   %u", EE_BikeType ) ; break;
        case PID_SERV_KM:       ODS1(DBG_SYS,DBG_INFO, "- EE Service:    %6lu km",  EE_NextSrvKm.km ); break;
        case PID_FUEL_CAP:      ODS2(DBG_SYS,DBG_INFO, "- EE Fuel Cap:   %4u,%1u liter", EE_FuelCap /10, EE_FuelCap -(EE_FuelCap /10)*10); break;
        case PID_FUEL_CONS:     ODS2(DBG_SYS,DBG_INFO, "- EE Fuel Cons:  %4u,%1u l/100", EE_FuelConsUser/10, EE_FuelConsUser-(EE_FuelConsUser/10)*10); break;
        case PID_BOOT_DELAY:    ODS2(DBG_SYS,DBG_INFO, "- EE BootDelay:  %4u,%1u s", EE_LogoDelay/10, EE_LogoDelay-(EE_LogoDelay/10)*10); break;
        case PID_WHEEL_IMP:     ODS1(DBG_SYS,DBG_INFO, "- EE Wheel Imp:  %6u I/Rev", EE_Wheel_ImpPRev); break;
        case PID_DEVFLAGS3:     ODS (DBG_SYS,DBG_INFO, "- EE DevFlags3:  currently not used!"); break;
        case PID_COOLR_CNTRL:   ODS3(DBG_SYS,DBG_INFO, "- EE Coolride:   On:%u GPO:%u GPI:%u",  EE_CoolrideCtrl.flags.CoolrAvail,
                                                                                                EE_CoolrideCtrl.flags.CoolrGPO,
                                                                                                EE_CoolrideCtrl.flags.CoolrGPI ); break;
        case PID_COMPS_CNTRL:   ODS2(DBG_SYS,DBG_INFO, "- EE Compass:    On:%u DPL:%u",         EE_CompassCtrl.flags.CompassAvail,
                                                                                                EE_CompassCtrl.flags.CompassDisplay ); break;
        case PID_LANGUAGE:      ODS1(DBG_SYS,DBG_INFO, "- EE Language:   %u", EE_LangSelect); break;
        case PID_FUELS_CNTRL:   ODS3(DBG_SYS,DBG_INFO, "- EE FuelSensor: On:%u I/l:%u GPI:%u",  EE_FuelSensCtrl.flags.FuelSAvail,
                                                                                                EE_FuelSensCtrl.FuelSImpulseRate,
                                                                                                EE_FuelSensCtrl.flags.FuelSGPI ); break;


        case PID_LAPCSTAT:      ODS2(DBG_MEAS,DBG_INFO,"- EE LapCnt:     Act:%u Lap:%u", EE_LapCnt_State.fActive, EE_LapCnt_State.cCurrentLap); break;
        case PID_LAPC_0 :       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[0 ]: %02u:%02u", EE_LapCnt_Time[0 ].bMin, EE_LapCnt_Time[0 ].bSec); break;
        case PID_LAPC_1 :       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[1 ]: %02u:%02u", EE_LapCnt_Time[1 ].bMin, EE_LapCnt_Time[1 ].bSec); break;
        case PID_LAPC_2 :       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[2 ]: %02u:%02u", EE_LapCnt_Time[2 ].bMin, EE_LapCnt_Time[2 ].bSec); break;
        case PID_LAPC_3 :       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[3 ]: %02u:%02u", EE_LapCnt_Time[3 ].bMin, EE_LapCnt_Time[3 ].bSec); break;
        case PID_LAPC_4 :       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[4 ]: %02u:%02u", EE_LapCnt_Time[4 ].bMin, EE_LapCnt_Time[4 ].bSec); break;
        case PID_LAPC_5 :       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[5 ]: %02u:%02u", EE_LapCnt_Time[5 ].bMin, EE_LapCnt_Time[5 ].bSec); break;
        case PID_LAPC_6 :       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[6 ]: %02u:%02u", EE_LapCnt_Time[6 ].bMin, EE_LapCnt_Time[6 ].bSec); break;
        case PID_LAPC_7 :       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[7 ]: %02u:%02u", EE_LapCnt_Time[7 ].bMin, EE_LapCnt_Time[7 ].bSec); break;
        case PID_LAPC_8 :       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[8 ]: %02u:%02u", EE_LapCnt_Time[8 ].bMin, EE_LapCnt_Time[8 ].bSec); break;
        case PID_LAPC_9 :       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[9 ]: %02u:%02u", EE_LapCnt_Time[9 ].bMin, EE_LapCnt_Time[9 ].bSec); break;
        case PID_LAPC_10:       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[10]: %02u:%02u", EE_LapCnt_Time[10].bMin, EE_LapCnt_Time[10].bSec); break;
        case PID_LAPC_11:       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[11]: %02u:%02u", EE_LapCnt_Time[11].bMin, EE_LapCnt_Time[11].bSec); break;
        case PID_LAPC_12:       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[12]: %02u:%02u", EE_LapCnt_Time[12].bMin, EE_LapCnt_Time[12].bSec); break;
        case PID_LAPC_13:       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[13]: %02u:%02u", EE_LapCnt_Time[13].bMin, EE_LapCnt_Time[13].bSec); break;
        case PID_LAPC_14:       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[14]: %02u:%02u", EE_LapCnt_Time[14].bMin, EE_LapCnt_Time[14].bSec); break;
        case PID_LAPC_15:       ODS2(DBG_MEAS,DBG_INFO,"- EE EE_LapCnt_Timer:  Lap[15]: %02u:%02u", EE_LapCnt_Time[15].bMin, EE_LapCnt_Time[15].bSec); break;

        default:                ODS1(DBG_MEAS,DBG_ERROR,"- Unspecified PID: %u", PID); break;
    }
}



        