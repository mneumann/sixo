/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-31 by Ralf Krizsan
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



/* external symbols */
extern UINT16           wMilliSecCounter; /* valid values: 0h .. ffffh */
extern far STRING       szDevName[];      /* debug out stuff */


/* ==============================================================

    NVRAM / EEPROM PARAMETER LIST
    -----------------------------

    USAGE:  For each data to be saved cyclicely into NVRAM/EEPROM, there
            has to be
                - the original data used/modified by application
                - the local compare value to detect changes automatically
                - the default value to initialize the NVRAM/EEPROM data

    FUNCTION:   The data configured in 'rgSysParControllist' will be
                scanned for any changes cyclicely and with some delay,
                    a) to reduce system load and
                    b) to automatically save any change,
                so that the application does not have to care for that.

    NOTE:   Because we have to many different types to be handled in the
            same table, we use 'void pointers' to data instead of using
            the data itself in the table.

   -------------------------------------------------------------- */

// display flags
extern  DPLFLAGS_TYPE   gDisplayFlags;                     // original value
static  DPLFLAGS_TYPE   gDisplayFlags_cmp;                 // compare value
const   DPLFLAGS_TYPE   gDisplayFlags_def = DEF_DPLFLAGS;  // default value

// debug filter flags
extern  DBGFILT_TYPE    gDebugFilter;                      // original value
static  DBGFILT_TYPE    gDebugFilter_cmp;                  // compare value
const   DBGFILT_TYPE    gDebugFilter_def = DEF_DBGFILT;    // default value
extern  DBGDETDIR_TYPE  gDebugDetails;
static  DBGDETDIR_TYPE  gDebugDetails_cmp;
const   DBGDETDIR_TYPE  gDebugDetails_def = DEF_DBGDETDIR;

/* system flags */
       SYSFLAGS_TYPE   gSystemFlags;
static SYSFLAGS_TYPE   gSystemFlags_cmp;
const  SYSFLAGS_TYPE   gSystemFlags_def = DEF_SYSFLAGS;

/* Software ID (would never change at application run time) */
       SWVERS_TYPE gSWID;
static SWVERS_TYPE gSWID_cmp;
const  SWVERS_TYPE gSWID_def = DEF_SWID_NUMBER;

/* selected logo */
       UINT8 gLogoSelection;
static UINT8 gLogoSelection_cmp;
#ifdef BIKE_MOTOBAU
    const  UINT8 gLogoSelection_def = eLogo_Motobau;
#else // BIKE_MOTOBAU
    const  UINT8 gLogoSelection_def = eLogo_SIXO;
#endif // BIKE_MOTOBAU

/* tripcounter settings */
       TRPCNTFL_TYPE gTripCntFlags;
static TRPCNTFL_TYPE gTripCntFlags_cmp;
const  TRPCNTFL_TYPE gTripCntFlags_def = DEF_TRPCNTFL;

/* bike type settings */
       BIKE_TYPE gBikeType;
static BIKE_TYPE gBikeType_cmp;

// special MOTOBAU behaviour
// Note:    This is the one and only MOTOBAU compiler switch!
//          Any other setting/behaviour will be at runtime via gBikeType
#ifdef BIKE_MOTOBAU
    const  BIKE_TYPE gBikeType_def = eBIKE_MOTOBAU;     // will not be editable! Fixed to MOTOBAU
#else // BIKE_MOTOBAU
    const  BIKE_TYPE gBikeType_def = eBIKE_STANDARD;    // will never be MOTOBAU version!
#endif // BIKE_MOTOBAU

// wheel size in mm
extern  UINT16 wWheelSize;                      // original value
static  UINT16 wWheelSize_cmp;                  // compare value
const   UINT16 wWheelSize_def = DEF_WHEELSIZE;  // default value

// RPM cylinder correcture factor
extern  CCF_TYPE CCF;
static  CCF_TYPE CCF_cmp;
const   CCF_TYPE CCF_def = DEF_CCF_NOM | DEF_CCF_DENOM;

// vehicle distance
extern DIST_TYPE   VehicDist;
static DIST_TYPE   VehicDist_cmp;
const  DIST_TYPE   VehicDist_def = 0L;

// TripCounter A..D
extern DIST_TYPE   TripA;
static DIST_TYPE   TripA_cmp;
const  DIST_TYPE   TripA_def = 0L;

extern DIST_TYPE   TripB;
static DIST_TYPE   TripB_cmp;
const  DIST_TYPE   TripB_def = 0L;

extern DIST_TYPE   TripC;
static DIST_TYPE   TripC_cmp;
const  DIST_TYPE   TripC_def = 0L;

extern DIST_TYPE   TripD;
static DIST_TYPE   TripD_cmp;
const  DIST_TYPE   TripD_def = 0L;

       DIST_TYPE   gNextServKm;
static DIST_TYPE   gNextServKm_cmp;
const  DIST_TYPE   gNextServKm_def = 0L;


// Fuel Distance
extern DIST_TYPE   FuelDist;
static DIST_TYPE   FuelDist_cmp;
const  DIST_TYPE   FuelDist_def = 0L;

// Speed statistics
extern SPEED_TYPE  Speed_Max;
static SPEED_TYPE  Speed_Max_cmp;
const  SPEED_TYPE  Speed_Max_def = 0;

extern SPEED_TYPE  Speed_AvrM;
static SPEED_TYPE  Speed_AvrM_cmp;
const  SPEED_TYPE  Speed_AvrM_def = 0;

extern SPEED_TYPE  Speed_AvrP;
static SPEED_TYPE  Speed_AvrP_cmp;
const  SPEED_TYPE  Speed_AvrP_def = 0;

/* Engine speed parameters */
extern RPM_TYPE RPM_Flash;
static RPM_TYPE RPM_Flash_cmp;
const  RPM_TYPE RPM_Flash_def = 0;

extern RPM_TYPE RPM_Max;
static RPM_TYPE RPM_Max_cmp;
const  RPM_TYPE RPM_Max_def = 0;


/* Engine RunTime statistics */
extern TIME_TYPE_LD  EngRunTime_Srv;
static TIME_TYPE_LD  EngRunTime_Srv_cmp;
const  TIME_TYPE_LD  EngRunTime_Srv_def = 0L;

extern TIME_TYPE_LD  EngRunTime_All;
static TIME_TYPE_LD  EngRunTime_All_cmp;
const  TIME_TYPE_LD  EngRunTime_All_def = 0L;


/* voltages */
extern UINT16 Volt_Min;
static UINT16 Volt_Min_cmp;
const  UINT16 Volt_Min_def = ANAIN_INVALID_U;

extern UINT16 Volt_Max;
static UINT16 Volt_Max_cmp;
const  UINT16 Volt_Max_def = ANAIN_INVALID_U;


/* temperatures */
extern INT16 TAir_Min;
static INT16 TAir_Min_cmp;
const  INT16 TAir_Min_def = ANAIN_INVALID_S;
extern INT16 TAir_Max;
static INT16 TAir_Max_cmp;
const  INT16 TAir_Max_def = ANAIN_INVALID_S;

extern INT16 TOil_Min;
static INT16 TOil_Min_cmp;
const  INT16 TOil_Min_def = ANAIN_INVALID_S;
extern INT16 TOil_Max;
static INT16 TOil_Max_cmp;
const  INT16 TOil_Max_def = ANAIN_INVALID_S;

extern INT16 TWat_Min;
static INT16 TWat_Min_cmp;
const  INT16 TWat_Min_def = ANAIN_INVALID_S;
extern INT16 TWat_Max;
static INT16 TWat_Max_cmp;
const  INT16 TWat_Max_def = ANAIN_INVALID_S;

extern TIME_TYPE_LL LapCntTime[LAPS_MAX];
static TIME_TYPE_LL LapCntTime_cmp[LAPS_MAX];
const  TIME_TYPE_LL LapCntTime_def = { 0, 0 };  // reset bMin & bSec

extern LCSTATE_TYPE LapCounterState;
static LCSTATE_TYPE LapCounterState_cmp;
const  LCSTATE_TYPE LapCounterState_def = 0;



/* ========================================================================================== */
/* SYSTEM PARAMETER CONTROL STRUCTURE  - complete NVRAM/EEPROM memory mapping */

const SYSPARINFO_TYPE  rgSysParControl[] =
{
    /*  Param ID            MemType    Offset   Size of type          addr of data        compare value           default value       */
    /*  ------------------- --------   ------ ----------------------- ------------------  ----------------------  ------------------- */
    {   PID_VEHIC_KM,       NVRAM_SEC,    0,  sizeof(DIST_TYPE),      &VehicDist,         &VehicDist_cmp,         &VehicDist_def      },  // 4 bytes, vehicle overall distance
    {   PID_HOURS_SERV,     NVRAM_SEC,    4,  sizeof(TIME_TYPE_LD),   &EngRunTime_Srv,    &EngRunTime_Srv_cmp,    &EngRunTime_Srv_def },  // 4 bytes, engine runtime since last service
    {   PID_HOURS_ALL,      NVRAM_SEC,    8,  sizeof(TIME_TYPE_LD),   &EngRunTime_All,    &EngRunTime_All_cmp,    &EngRunTime_All_def },  // 4 bytes, engine runtime overall
    {   PID_TRIP_A_KM,      NVRAM,       12,  sizeof(DIST_TYPE),      &TripA,             &TripA_cmp,             &TripA_def          },  // 4 bytes, TripCounter A distance
    {   PID_TRIP_B_KM,      NVRAM,       16,  sizeof(DIST_TYPE),      &TripB,             &TripB_cmp,             &TripB_def          },  // 4 bytes, TripCounter B distance
    {   PID_TRIP_C_KM,      NVRAM,       20,  sizeof(DIST_TYPE),      &TripC,             &TripC_cmp,             &TripC_def          },  // 4 bytes, TripCounter C distance
    {   PID_TRIP_D_KM,      NVRAM,       24,  sizeof(DIST_TYPE),      &TripD,             &TripD_cmp,             &TripD_def          },  // 4 bytes, TripCounter D distance
    {   PID_SPEED_AVR_M,    NVRAM,       28,  sizeof(SPEED_TYPE),     &Speed_AvrM,        &Speed_AvrM_cmp,        &Speed_AvrM_def     },  // 2 bytes, average speed EX-cluding pause times
    {   PID_SPEED_AVR_P,    NVRAM,       30,  sizeof(SPEED_TYPE),     &Speed_AvrP,        &Speed_AvrP_cmp,        &Speed_AvrP_def     },  // 2 bytes, average speed IN-cluding pause times
    {   PID_FUEL_KM,        NVRAM,       32,  sizeof(DIST_TYPE),      &FuelDist,          &FuelDist_cmp,          &FuelDist_def       },  // 4 bytes, distance since last refuel
    /*                                   |                          */
    /*                                   +---  max 56 bytes NVRAM available! */
    /*  ------------------- --------   ------ ----------------------- ------------------  ----------------------  ------------------- */

    /*  Param ID            MemType    Offset   Size of type          addr of data        compare value           default value       */
    /*  ------------------- --------   ------ ----------------------- ------------------  ----------------------  ------------------- */
    {   PID_SWVERSION,      EEPROM,      0,   sizeof(UINT16),         &gSWID,             &gSWID_cmp,             &gSWID_def          },  // 2 bytes, 4 software version nibbles
    {   PID_WHEELSIZE,      EEPROM_SEC,  2,   sizeof(UINT16),         &wWheelSize,        &wWheelSize_cmp,        &wWheelSize_def     },  // 2 bytes, wheel circumcistence in mm
    {   PID_RPM_CCF,        EEPROM_SEC,  4,   sizeof(CCF_TYPE),       &CCF,               &CCF_cmp,               &CCF_def            },  // 1 byte, two cylinder correcture factor nibbles
    {   PID_SYSFL,          EEPROM_SEC,  5,   sizeof(SYSFLAGS_TYPE),  &gSystemFlags,      &gSystemFlags_cmp,      &gSystemFlags_def   },  // 1 byte, bitfield for system status
    {   PID_DPLFL,          EEPROM_SEC,  6,   sizeof(DPLFLAGS_TYPE),  &gDisplayFlags,     &gDisplayFlags_cmp,     &gDisplayFlags_def  },  // 4 byte, bitfield for display control
    {   PID_DBGFILTFL,      EEPROM_SEC, 10,   sizeof(DBGFILT_TYPE),   &gDebugFilter,      &gDebugFilter_cmp,      &gDebugFilter_def   },  // 1 byte, bitfield for debug filters
    {   PID_DBGDETDIRFL,    EEPROM_SEC, 11,   sizeof(DBGDETDIR_TYPE), &gDebugDetails,     &gDebugDetails_cmp,     &gDebugDetails_def  },  // 1 byte, bitfield for debug details & direction
    {   PID_LOGO,           EEPROM_SEC, 12,   sizeof(UINT8),          &gLogoSelection,    &gLogoSelection_cmp,    &gLogoSelection_def },  // 1 byte, number of currently selected logo
    {   PID_TRIPCNTFL,      EEPROM,     13,   sizeof(TRPCNTFL_TYPE),  &gTripCntFlags,     &gTripCntFlags_cmp,     &gTripCntFlags_def  },  // 1 byte, trip counter flags
    {   PID_SPEED_MAX,      EEPROM,     14,   sizeof(SPEED_TYPE),     &Speed_Max,         &Speed_Max_cmp,         &Speed_Max_def      },  // 2 bytes, maximum vehicle speed
    {   PID_RPM_MAX,        EEPROM,     16,   sizeof(RPM_TYPE),       &RPM_Max,           &RPM_Max_cmp,           &RPM_Max_def        },  // 2 bytes, maximum engine speed
    {   PID_RPM_FLASH,      EEPROM,     18,   sizeof(RPM_TYPE),       &RPM_Flash,         &RPM_Flash_cmp,         &RPM_Flash_def      },  // 2 bytes, engine speed to enable flash
    {   PID_VOLT_MIN,       EEPROM,     20,   sizeof(UINT16),         &Volt_Min,          &Volt_Min_cmp,          &Volt_Min_def       },  // 2 bytes, battery voltage minimum
    {   PID_VOLT_MAX,       EEPROM,     22,   sizeof(UINT16),         &Volt_Max,          &Volt_Max_cmp,          &Volt_Max_def       },  // 2 bytes, battery voltage maximum
    {   PID_TAIR_MIN,       EEPROM,     24,   sizeof(INT16),          &TAir_Min,          &TAir_Min_cmp,          &TAir_Min_def       },  // 2 bytes, air temperature minimum
    {   PID_TAIR_MAX,       EEPROM,     26,   sizeof(INT16),          &TAir_Max,          &TAir_Max_cmp,          &TAir_Max_def       },  // 2 bytes, air temperature maximum
    {   PID_TOIL_MIN,       EEPROM,     28,   sizeof(INT16),          &TOil_Min,          &TOil_Min_cmp,          &TOil_Min_def       },  // 2 bytes, oil temperature minimum
    {   PID_TOIL_MAX,       EEPROM,     30,   sizeof(INT16),          &TOil_Max,          &TOil_Max_cmp,          &TOil_Max_def       },  // 2 bytes, oil temperature maximum
    {   PID_TWAT_MIN,       EEPROM,     32,   sizeof(INT16),          &TWat_Min,          &TWat_Min_cmp,          &TWat_Min_def       },  // 2 bytes, water temperature minimum
    {   PID_TWAT_MAX,       EEPROM,     34,   sizeof(INT16),          &TWat_Max,          &TWat_Max_cmp,          &TWat_Max_def       },  // 2 bytes, water temperature maximum
    {   PID_BIKETYPE,       EEPROM,     36,   sizeof(UINT8),          &gBikeType,         &gBikeType_cmp,         &gBikeType_def      },  // 1 byte, definition of bike version (enum)
    {   PID_SERV_KM,        EEPROM,     37,   sizeof(DIST_TYPE),      &gNextServKm,       &gNextServKm_cmp,       &gNextServKm_def    },  // 4 bytes, km, at which next service is required

    {   PID_LAPCSTAT,       EEPROM,    136,   sizeof(LCSTATE_TYPE),   &LapCounterState,   &LapCounterState_cmp,   &LapCounterState_def},  // 1 bytes, lap counter status
    {   PID_LAPC_0 ,        EEPROM,    137,   sizeof(TIME_TYPE_LL),   &LapCntTime[0 ],    &LapCntTime_cmp[0 ],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_1 ,        EEPROM,    139,   sizeof(TIME_TYPE_LL),   &LapCntTime[1 ],    &LapCntTime_cmp[1 ],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_2 ,        EEPROM,    141,   sizeof(TIME_TYPE_LL),   &LapCntTime[2 ],    &LapCntTime_cmp[2 ],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_3 ,        EEPROM,    143,   sizeof(TIME_TYPE_LL),   &LapCntTime[3 ],    &LapCntTime_cmp[3 ],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_4 ,        EEPROM,    145,   sizeof(TIME_TYPE_LL),   &LapCntTime[4 ],    &LapCntTime_cmp[4 ],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_5 ,        EEPROM,    147,   sizeof(TIME_TYPE_LL),   &LapCntTime[5 ],    &LapCntTime_cmp[5 ],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_6 ,        EEPROM,    149,   sizeof(TIME_TYPE_LL),   &LapCntTime[6 ],    &LapCntTime_cmp[6 ],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_7 ,        EEPROM,    151,   sizeof(TIME_TYPE_LL),   &LapCntTime[7 ],    &LapCntTime_cmp[7 ],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_8 ,        EEPROM,    153,   sizeof(TIME_TYPE_LL),   &LapCntTime[8 ],    &LapCntTime_cmp[8 ],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_9 ,        EEPROM,    155,   sizeof(TIME_TYPE_LL),   &LapCntTime[9 ],    &LapCntTime_cmp[9 ],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_10,        EEPROM,    157,   sizeof(TIME_TYPE_LL),   &LapCntTime[10],    &LapCntTime_cmp[10],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_11,        EEPROM,    159,   sizeof(TIME_TYPE_LL),   &LapCntTime[11],    &LapCntTime_cmp[11],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_12,        EEPROM,    161,   sizeof(TIME_TYPE_LL),   &LapCntTime[12],    &LapCntTime_cmp[12],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_13,        EEPROM,    163,   sizeof(TIME_TYPE_LL),   &LapCntTime[13],    &LapCntTime_cmp[13],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_14,        EEPROM,    165,   sizeof(TIME_TYPE_LL),   &LapCntTime[14],    &LapCntTime_cmp[14],    &LapCntTime_def     },  // 2 bytes, lap counter time struct
    {   PID_LAPC_15,        EEPROM,    167,   sizeof(TIME_TYPE_LL),   &LapCntTime[15],    &LapCntTime_cmp[15],    &LapCntTime_def     },  // 2 bytes, lap counter time struct

    /*  EEPROM: max. 2048 bytes allowed  */
    /*  ------------------- --------   ------ ----------------------- ------------------  ----------------------  ------------------- */
};


/* number of system parameters in control structure array */
#define NROFSYSPAR  (sizeof(rgSysParControl)/sizeof(SYSPARINFO_TYPE))


/* ========================================================================================== */







/***********************************************************************
 *  FUNCTION:       ParGetSysParamInfo
 *  DESCRIPTION:    returns a pointer to the requested system parameter
 *                  info structure (not the sys parameter itself!)
 *  PARAMETER:      PARAM_ID_TYPE eSearchedID   searched parameter ID
 *  RETURN:         address of requested system parameter info structure
 *                  NULL if not found!
 *  COMMENT:        to be used before reading / writing a system parameter
 *********************************************************************** */
SYSPARINFO_TYPE far * ParGetSysParamInfo(const PARAM_ID_TYPE eSearchedID)
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
 *  FUNCTION:       ParReadSysParam
 *  DESCRIPTION:    Reads the requested system variable from EEPROM or NVRAM
 *                  and fills the given buffer
 *  PARAMETER:      PARAM_ID_TYPE   eParamID   enum type of requested system parameter
 *                  void far *      pointer to prepared return buffer
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ParReadSysParam(const PARAM_ID_TYPE eParamID, void far * fpParamBuffer)
{
    UINT8                   LocalBuffer[MAXPARAMSIZE];
    SYSPARINFO_TYPE far *   fpParamInfo;
    ERRCODE                 RValue = ERR_PARAM_ERR;

    /* parameter check: */
    fpParamInfo = ParGetSysParamInfo(eParamID);     /* parameter ID exists? */
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
 *  FUNCTION:       ParWriteSysParam
 *  DESCRIPTION:    Writes the given system variable into EEPROM or NVRAM
 *  PARAMETER:      PARAM_ID_TYPE   eParamID   enum type of given system parameter
 *                  void far *      pointer to given parameter
 *  RETURN:         ERRCODE         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ParWriteSysParam (const PARAM_ID_TYPE eParamID, void far * fpGivenParam)
{
    UINT8                 LocalBuffer[MAXPARAMSIZE];
    SYSPARINFO_TYPE far * fpParamInfo;
    ERRCODE               RValue = ERR_PARAM_ERR;

    /* parameter check: */
    fpParamInfo = ParGetSysParamInfo(eParamID);     /* parameter ID exists? */
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
 *  FUNCTION:       ParSetDefaults
 *  DESCRIPTION:    Re-Write all nvram/eeprom parameters with default
 *                  data.
 *  PARAMETER:      fComplete  TRUE:  overwrites WHEELSIZE & CCF & VehicDist too
 *                             FALSE: leaves WHEELSIZE & CCF & VehicDist unchanged
 *  RETURN:         error code
 *  COMMENT:        - To be used after first power up or while
 *                    reseting to 'factory defaults'
 *                  - Manual reset eeprom/nvram by removing the
 *                    'magic number' in eeprom.
 *********************************************************************** */
ERRCODE ParSetDefaults(BOOL fComplete)
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
        ParDebugOutParameter( rgSysParControl[cPara].eParamID );

        // write default value into EEPROM/NVRAM
        RValue = ParWriteSysParam( rgSysParControl[cPara].eParamID, rgSysParControl[cPara].fpDefault);
        if (RValue != ERR_OK)
            ODS1(DBG_SYS,DBG_ERROR,"Error while initializing PID:%u", rgSysParControl[cPara].eParamID);

    }
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       ParCheckFirstInit
 *  DESCRIPTION:    Checks the eeprom software ID.
 *                  If its not up2date, it (completely) renews
 *                  eeprom/nvram memory content to defaults
 *  PARAMETER:      -
 *  RETURN:         error code
 *  COMMENT:        resetting the 'magic number' forces complete re-initialize of eeprom
 *********************************************************************** */
ERRCODE ParCheckFirstInit(void)
{
    ERRCODE     RValue  = ERR_OK;
    SWVERS_TYPE SWID    = 0x0;

    /* read eeprom saved software version number */
    RValue = ParReadSysParam(PID_SWVERSION, &SWID);
    /*ODS4(DBG_SYS,DBG_INFO,"EEPROM Software Version: %u.%u.%u.%c",
            SWID.Fields.apl, SWID.Fields.swv, SWID.Fields.bld, SWID.Fields.spc+APLNUMCOFFS); */

    /* First EEPROM use or user forced eeprom init?
       (indicated by 0xff in complete eeprom memory) */
    if ( ParCheckMagicNumber() == FALSE)
    //if (SWID.Number == 0xFFFF)
    {
        /* renew the COMPLETE nvram & eeprom */
        ODS(DBG_SYS,DBG_INFO,"\nFirst use of EEPROM/NVRAM: Complete Init!");
        RValue = ParSetDefaults(COMPLETE);
    }

    /* check software ID */
    else if (SWID.Number != DEF_SWID_NUMBER)
    {
        /*renew the nvram & eeprom */
        ODS(DBG_SYS,DBG_INFO,"\nNew SW version: Re-Newing EEPROM/NVRAM!");
        ODS(DBG_SYS,DBG_INFO,"(Some basic parameters will remain unchanged!)");
        RValue = ParSetDefaults(PARTIAL);
    }
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       ParCyclicSaveValues
 *  DESCRIPTION:    Cyclicly called routine to asure all necessary
 *                  measurement data to be saved in EEPROM / NVRAM
 *  PARAMETER:      -
 *  RETURN:         error code
 *  COMMENT:        - Does only write into eeprom if any changes made in
 *                    application data.
 *                  - Reduces sytem load by only saving one value every 200 ms.
 *********************************************************************** */
ERRCODE ParCyclicSaveValues(void)
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
            // NOTE: current data has already been initialized by ParInitSystemPar() before main() loop.
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
            ParDebugOutParameter( rgSysParControl[cCurrentPara].eParamID );

            // Difference found! -> save parameter into EEPROM/NVRAM
            RValue = ParWriteSysParam( rgSysParControl[cCurrentPara].eParamID, rgSysParControl[cCurrentPara].fpData);
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
 *  FUNCTION:       ParInitSystemPar
 *  DESCRIPTION:    Reads all system parameters from eeprom/nvram
 *                  to initialize them as global variables
 *  PARAMETER:      -
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ParInitSystemPar (void)
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
        RValue = ParReadSysParam(rgSysParControl[cPara].eParamID, rgSysParControl[cPara].fpData);
        if (RValue != ERR_OK)
            ODS1(DBG_SYS,DBG_ERROR,"Error reading PID:%u", rgSysParControl[cPara].eParamID);

        // debug out!
        ParDebugOutParameter( rgSysParControl[cPara].eParamID );

    }
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       ParCheckMagicNumber
 *  DESCRIPTION:    Checks existance of 'magic number'
 *                  (a word in EEPROMs most top address) and writes
 *                  the magic number if not found
 *  PARAMETER:      -
 *  RETURN:         TRUE    indicates magic number found
 *                  FALSE   indicates magic number missed
 *  COMMENT:        Used to detect a hardware (eq. EEPROM) which was
 *                  NEVER used before.
 *********************************************************************** */
BOOL ParCheckMagicNumber ( void )
{
    UINT16 wMagicNumber = 0;

    /* read out current number */
    iicEepromRead( EEPR_MAGICNUM_ADDR, EEPR_MAGICNUM_SIZE, (UINT8 *)&wMagicNumber );

    /* check that number: found? */
    if ( wMagicNumber != EEPR_MAGICNUM_VAL )
    {
        /* write into EEPROM */
        wMagicNumber = EEPR_MAGICNUM_VAL;
        iicEepromWrite( EEPR_MAGICNUM_ADDR, EEPR_MAGICNUM_SIZE, (UINT8 *)&wMagicNumber );
        return FALSE;
    }
    return TRUE;
}



/***********************************************************************
 *  FUNCTION:       ParDebugOutParameter
 *  DESCRIPTION:    Just a helper function to write clear formated
 *                  parameter data to debug out screen, prints out
 *                  current data of given parameter in one line
 *  PARAMETER:      eParamID    ID of parameter to be shown
 *  RETURN:         -
 *  COMMENT:        To be used e.g. after preceding comment line like
 *                  'Now initialzing data: ...'
 *********************************************************************** */
void ParDebugOutParameter( const PARAM_ID_TYPE PID )
{
    switch (PID)                    /* choice of system parameter to be saved */
    {
        case PID_VEHIC_KM:      ODS2(DBG_SYS,DBG_INFO, "- NV VehicDist:  %6lu,%.2lu km",     VehicDist.dkm/100L, VehicDist.dkm-(VehicDist.dkm/100L)*100); break;
        case PID_TRIP_A_KM:     ODS2(DBG_SYS,DBG_INFO, "- NV TripA:      %6lu,%.2lu km",     TripA.dkm/100L, TripA.dkm-(TripA.dkm/100L)*100); break;
        case PID_TRIP_B_KM:     ODS2(DBG_SYS,DBG_INFO, "- NV TripB:      %6lu,%.2lu km",     TripB.dkm/100L, TripB.dkm-(TripB.dkm/100L)*100); break;
        case PID_TRIP_C_KM:     ODS2(DBG_SYS,DBG_INFO, "- NV TripC:      %6lu,%.2lu km",     TripC.dkm/100L, TripC.dkm-(TripC.dkm/100L)*100); break;
        case PID_TRIP_D_KM:     ODS2(DBG_SYS,DBG_INFO, "- NV TripD:      %6lu,%.2lu km",     TripD.dkm/100L, TripD.dkm-(TripD.dkm/100L)*100); break;
        case PID_SPEED_AVR_M:   ODS2(DBG_SYS,DBG_INFO, "- NV Speed_AvrM: %6u,%.2u km/h",     Speed_AvrM/100, Speed_AvrM-(Speed_AvrM/100)*100); break;
        case PID_SPEED_AVR_P:   ODS2(DBG_SYS,DBG_INFO, "- NV Speed_AvrP: %6u,%.2u km/h",     Speed_AvrP/100, Speed_AvrP-(Speed_AvrP/100)*100); break;
        case PID_FUEL_KM:       ODS2(DBG_SYS,DBG_INFO, "- NV FuelDist:   %6lu,%.2lu km",     FuelDist.dkm/100L, FuelDist.dkm-(FuelDist.dkm/100L)*100); break;
        case PID_HOURS_SERV:    ODS3(DBG_SYS,DBG_INFO, "- NV EngTimeSrv: %2u:%02u:%02u",    EngRunTime_Srv.wHour, EngRunTime_Srv.bMin, EngRunTime_Srv.bSec); break;
        case PID_HOURS_ALL:     ODS3(DBG_SYS,DBG_INFO, "- NV EngTimeAll: %2u:%02u:%02u",    EngRunTime_All.wHour, EngRunTime_All.bMin, EngRunTime_All.bSec); break;

        case PID_SWVERSION:     ODS4(DBG_SYS,DBG_INFO, "- EE SW-ID:      %u.%u.%u.%c",      gSWID.Fields.apl, gSWID.Fields.swv, gSWID.Fields.bld, gSWID.Fields.spc+APLNUMCOFFS); break;
        case PID_WHEELSIZE:     ODS1(DBG_SYS,DBG_INFO, "- EE WheelSize:  %6u mm",            wWheelSize); break;
        case PID_RPM_CCF:       ODS2(DBG_SYS,DBG_INFO, "- EE CCF:        %u/%u",            CCF.nibble.nom, CCF.nibble.denom); break;
        case PID_SYSFL:         ODS2(DBG_SYS,DBG_INFO, "- EE SysFlags:   AD: %s MD: %u", szDevName[gSystemFlags.flags.ActDevNr], gSystemFlags.flags.MainDevState); break;
        case PID_DPLFL:         ODS4(DBG_SYS,DBG_INFO, "- EE DplFlags:   C:%u BLL:%u BLOL:%u Inv:%u", gDisplayFlags.flags.ContrLev, gDisplayFlags.flags.BacklLev, gDisplayFlags.flags.BacklOnLevel, gDisplayFlags.flags.fDplInvers); break;
        case PID_DBGFILTFL:     ODS1(DBG_SYS,DBG_INFO, "- EE DbgFilter:  0x%x ", gDebugFilter); break;
        case PID_DBGDETDIRFL:   ODS1(DBG_SYS,DBG_INFO, "- EE DbgDetail:  0x%x", gDebugDetails); break;
        case PID_LOGO:          ODS1(DBG_SYS,DBG_INFO, "- EE Logo:       %u", gLogoSelection ) ; break;
        case PID_TRIPCNTFL:     ODS1(DBG_SYS,DBG_INFO, "- EE TripCntFl:  %u", gTripCntFlags.flags.LongDistUp ) ; break;
        case PID_SPEED_MAX:     ODS1(DBG_MEAS,DBG_INFO,"- EE Speed_Max:  %6u km/h", Speed_Max); break;
        case PID_RPM_MAX:       ODS1(DBG_MEAS,DBG_INFO,"- EE RPM Max:    %6u RPM", RPM_Max); break;
        case PID_RPM_FLASH:     ODS1(DBG_MEAS,DBG_INFO,"- EE RPM Flash:  %6u RPM", RPM_Flash); break;
        case PID_VOLT_MIN:      ODS2(DBG_MEAS,DBG_INFO,"- EE Volt Min:   %3u,%.2u V", Volt_Min/100, Volt_Min-(Volt_Min/100)*100); break;
        case PID_VOLT_MAX:      ODS2(DBG_MEAS,DBG_INFO,"- EE Volt Max:   %3u,%.2u V", Volt_Max/100, Volt_Max-(Volt_Max/100)*100); break;
        case PID_TAIR_MIN:      ODS1(DBG_MEAS,DBG_INFO,"- EE TAIR Min:   %5i °C", TAir_Min); break;
        case PID_TAIR_MAX:      ODS1(DBG_MEAS,DBG_INFO,"- EE TAIR Max:   %5i °C", TAir_Max); break;
        case PID_TOIL_MIN:      ODS1(DBG_MEAS,DBG_INFO,"- EE TOIL Min:   %5i °C", TOil_Min); break;
        case PID_TOIL_MAX:      ODS1(DBG_MEAS,DBG_INFO,"- EE TOIL Max:   %5i °C", TOil_Max); break;
        case PID_TWAT_MIN:      ODS1(DBG_MEAS,DBG_INFO,"- EE TWAT Min:   %5i °C", TWat_Min); break;
        case PID_TWAT_MAX:      ODS1(DBG_MEAS,DBG_INFO,"- EE TWAT Max:   %5i °C", TWat_Max); break;
        case PID_BIKETYPE:      ODS1(DBG_SYS,DBG_INFO, "- EE BikeType:   %u", gBikeType ) ; break;
        case PID_SERV_KM:       ODS1(DBG_SYS,DBG_INFO, "- EE Service:    %6lu km",  gNextServKm.km ); break;

        case PID_LAPCSTAT:      ODS2(DBG_MEAS,DBG_INFO,"- EE LapCnt:     Act:%u Lap:%u", LapCounterState.fActive, LapCounterState.cCurrentLap); break;
        case PID_LAPC_0 :       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[0 ]: %02u:%02u", LapCntTime[0 ].bMin, LapCntTime[0 ].bSec); break;
        case PID_LAPC_1 :       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[1 ]: %02u:%02u", LapCntTime[1 ].bMin, LapCntTime[1 ].bSec); break;
        case PID_LAPC_2 :       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[2 ]: %02u:%02u", LapCntTime[2 ].bMin, LapCntTime[2 ].bSec); break;
        case PID_LAPC_3 :       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[3 ]: %02u:%02u", LapCntTime[3 ].bMin, LapCntTime[3 ].bSec); break;
        case PID_LAPC_4 :       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[4 ]: %02u:%02u", LapCntTime[4 ].bMin, LapCntTime[4 ].bSec); break;
        case PID_LAPC_5 :       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[5 ]: %02u:%02u", LapCntTime[5 ].bMin, LapCntTime[5 ].bSec); break;
        case PID_LAPC_6 :       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[6 ]: %02u:%02u", LapCntTime[6 ].bMin, LapCntTime[6 ].bSec); break;
        case PID_LAPC_7 :       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[7 ]: %02u:%02u", LapCntTime[7 ].bMin, LapCntTime[7 ].bSec); break;
        case PID_LAPC_8 :       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[8 ]: %02u:%02u", LapCntTime[8 ].bMin, LapCntTime[8 ].bSec); break;
        case PID_LAPC_9 :       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[9 ]: %02u:%02u", LapCntTime[9 ].bMin, LapCntTime[9 ].bSec); break;
        case PID_LAPC_10:       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[10]: %02u:%02u", LapCntTime[10].bMin, LapCntTime[10].bSec); break;
        case PID_LAPC_11:       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[11]: %02u:%02u", LapCntTime[11].bMin, LapCntTime[11].bSec); break;
        case PID_LAPC_12:       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[12]: %02u:%02u", LapCntTime[12].bMin, LapCntTime[12].bSec); break;
        case PID_LAPC_13:       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[13]: %02u:%02u", LapCntTime[13].bMin, LapCntTime[13].bSec); break;
        case PID_LAPC_14:       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[14]: %02u:%02u", LapCntTime[14].bMin, LapCntTime[14].bSec); break;
        case PID_LAPC_15:       ODS2(DBG_MEAS,DBG_INFO,"- EE LapCntTimer:  Lap[15]: %02u:%02u", LapCntTime[15].bMin, LapCntTime[15].bSec); break;

        default:                ODS1(DBG_MEAS,DBG_ERROR,"- Unspecified PID: %u", PID); break;
    }
}



        