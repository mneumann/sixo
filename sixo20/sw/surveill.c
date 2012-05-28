/***********************************************************************
 *
 *  $RCSfile: surveill.c
 *
 *  $Revision$
 *
 *  $Date$
 *
 *  Author: Markus Mueller, mm@rsj.de
 *
 *  --------------------------------------------------------------------
 *  Created:        03-30-2004 by Markus Mueller
 *  Project:        SIxO 2.0
 *  Module:         surveill.c
 *  Purpose:        vehicle surveillance module
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
 *      Stoeckener Str. 115                 Hudeplan 37 A
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
 * Revision 3.19  2012/05/28 12:47:31  tuberkel
 * Corrections for renamed Eeprom/Nvram Variables
 *
 * Revision 3.18  2012/05/27 17:52:40  tuberkel
 * Corrections for renamed Eeprom/Nvram Variables
 *
 * Revision 3.17  2012/05/27 16:01:42  tuberkel
 * All Eeprom/Nvram Variables renamed
 *
 * Revision 3.16  2012/05/24 19:30:03  tuberkel
 * Moved BMP-Logos to 'logos.c'
 *
 * Revision 3.15  2012/03/02 22:44:33  tuberkel
 * Surv_CheckDevice():
 * - VehicSim-Warning only if not debugging!
 *
 * Revision 3.14  2012/03/01 20:49:59  tuberkel
 * Surv_CheckDigitalPorts():
 * - Oilswitch defect Detection disabled for all
 * - Oilswitch defect Detection enabled for AT & BMW only
 *
 * Revision 3.13  2012/02/27 21:43:18  tuberkel
 * Surv_CheckDevice(): removed check of Coolride Heatgrips
 *
 * Revision 3.12  2012/02/27 20:46:50  tuberkel
 * - all Coolride GPIs/GPOs correctly set by Eeprom value
 *
 * Revision 3.11  2012/02/26 12:24:55  tuberkel
 * - moved all Eeprom Vlaues physically into 'sysparam' module
 *
 * Revision 3.10  2012/02/14 21:08:03  tuberkel
 * - #define COMPASS ==> COMPDRV
 * - Compass SystemParam moved from devFlags2 -> 3
 * - Settings 'Compass' ==> 'Extensions'
 * - all Compass-Display modules enabled by default
 *
 * Revision 3.9  2012/02/11 12:21:45  tuberkel
 * dedicated COOLRIDE macros prepared & used
 *
 * Revision 3.8  2012/02/11 09:16:14  tuberkel
 * Disabled HeatGrip as 'VehState'
 *
 * Revision 3.7  2012/02/10 23:45:22  tuberkel
 * - Survelannce HeatGrip <Info> - if active
 * - Surveillance-API reviewed
 *
 * Revision 3.6  2012/02/06 20:54:14  tuberkel
 * Just renamed all 'Devices' function prefixes for better readability
 *
 * Revision 3.5  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 3.4  2012/02/04 21:49:42  tuberkel
 * All BeeperDriver functions mapped ==> DigOutDrv()
 *
 * Revision 3.3  2012/01/23 04:04:16  tuberkel
 * BugFix Symbol "SystemError"
 *
 * Revision 3.2  2012/01/21 19:52:49  tuberkel
 * Additional Logos for 'TeneristI and 'Coolride'
 *
 * Revision 3.1  2012/01/14 10:26:44  tuberkel
 * LED PWM handling changed:
 * - no longer Msgs/TimerMsgs used (inaccurate optic)
 * - instead use TimerISR to control PWM
 * - Granulartiy is SystemTicks (20 ms)
 * - works well
 * - prevent direct LEDDrv access (if possible)
 *
 * Revision 3.0  2010/11/07 09:24:42  tuberkel
 * V30 Preparations:
 * - Surveillance completely reviewed
 * - new: sorted list of any Info/warning/error
 * - new: informational states (e.g. SummerTime activated)
 * - new: internal states (e.g. RTC defect)
 * - new: API to retrieve currently active StateList
 * . moved some features from 'MOTO_BAU' version to general version
 *
 * Revision 2.8  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.7  2009/07/08 21:41:17  tuberkel
 * All compiler defines reviewed:
 * - set to unique usage: set define to 0 or 1
 * - default values set, if not used
 * - see 'Project Editor' for details
 *
 * Revision 2.6  2007/03/30 10:03:49  tuberkel
 * just changed constant name VEHSTATE_TXT_LEN
 *
 * Revision 2.5  2007/03/26 23:06:06  tuberkel
 * changed MOTOBAU version handling
 * - eBiketye -> #define
 *
 * Revision 2.4  2006/10/19 18:51:30  tuberkel
 * BugFix: ANAIN_TEMP_OFFSET -> ANAIN_TEMP_SENSORDETECT
 * - now uses values <-20° to detect missing temp sensor
 *
 * Revision 2.3  2006/10/01 22:19:02  tuberkel
 * Surv_CheckDigitalPorts()
 * - now uses digital filtered values only
 *
 * Revision 2.2  2006/07/20 23:13:43  tuberkel
 * BugFix in V2.1:
 * - ALTW-warning now cleared correctly
 *
 * Revision 2.1  2006/07/20 23:10:50  tuberkel
 * added special F650 handling:
 * - fuel 4 L. warning
 * - ABS warning
 * - WaterTempSwitch
 * Now distinguish between analog TempoSensor & TempSwitch
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.9  2006/02/18 14:36:04  Ralf
 * - adaption to BIKE_MOTOBAU
 * - added surveillance of Temps/EngSpeed/Distances
 *
 *
 ************************************************************************ */



// includes
#include <stdio.h>
#include <string.h>

#include "standard.h"
#include "sfr62p.h"
#include "msgqueue.h"
#include "timer.h"
#include "digoutdr.h"
#include "digindrv.h"
#include "debug.h"
#include "anain.h"
#include "device.h"
#include "measure.h"
#include "resource.h"
#include "led.h"
#include "surveill.h"
#include "timedate.h"
#include "digoutdr.h"
#include "sysparam.h"




// ------------------------------------------------------
/* external symbols (taken from eeprom/nvram) */
extern  far UINT16          wMilliSecCounter;   // high resolution short distance timer, ms,  max  65 sec
extern  far UINT16          wSecCounter;        // low  resolution long  distance timer, sec, max. 18 h
extern  far DEVFLAGS2_TYPE  EE_DevFlags_2;      // device status parameters
extern  far DEVFLAGS3_TYPE  EE_DevFlags_3;      // device status parameters
extern  far BIKE_TYPE       EE_BikeType;          // bike type
extern  far DIST_TYPE       EE_NextSrvKm;        // to get/set original value
extern  far DIST_TYPE       EE_NextSrvKm_def;    // to detect wether enabled/not
extern  far TIME_TYPE_LD    NV_EngRunTime_Srv_def; // to detect wether enabled/not
extern  far DIGFILTTYPE     DigInFilter[];      // digital filter table for all inputs
extern  far RPM_TYPE        EE_RPM_Flash;          // engine speed to enable flash lamp,   1 RPM/bit
extern  far RPM_TYPE        EE_RPM_Max;            // max engine speed,                    1 RPM/bit
extern  far SPEED_TYPE      EE_SpeedMax;          // max vehicel speed                    1 km/h/bit
extern  far TIME_TYPE_LD    NV_EngRunTime_Srv;     // 4 bytes, engine runtime since last service
extern  far TIME_TYPE_LD    NV_EngRunTime_All;     // 4 bytes, engine runtime overall
extern  far UINT16          EE_FuelCap;          // tank size in 1/10 liters
extern  far UINT8           EE_FuelConsUser;         // motor fuel consumption in 1/10 liter/100 km

extern  far UINT16          EE_Volt_Min;           // minimal battery voltage,     0,01 V/bit
extern  far UINT16          EE_Volt_Max;           // maximal battery voltage,     0,01 V/bit
extern  far INT16           EE_TAir_Min;           // minimal air temperature,     1°C/bit, valid -40°C...215°C
extern  far INT16           EE_TAir_Max;           // maximal air temperature,     1°C/bit, valid -40°C...215°C
extern  far INT16           EE_TOil_Min;           // minimal oil temperature,     1°C/bit, valid -40°C...215°C
extern  far INT16           EE_TOil_Max;           // maximal oil temperature,     1°C/bit, valid -40°C...215°C
extern  far INT16           EE_TWat_Min;           // minimal water temperature,   1°C/bit, valid -40°C...215°C
extern  far INT16           EE_TWat_Max;           // maximal water temperature,   1°C/bit, valid -40°C...215°C


/* clock control */
BOOL    fCESTChanged = FALSE;   // DaylaightSaving 'CET/CEST changed flag'
BOOL    fRTCDefect = FALSE;     // RTC defect detection


// locally surveilled data
static UINT16       wRPM;                   // RPM signal           1 RPM/bit
static UINT16       wWheelSpeed;            // Wheel speed          1 km/h/bit
static UINT16       wBattSupply;            // battery supply       0,01 V/bit
static UINT16       wAlternator;            // Alternator voltage   0,01 V/bit
static INT16        iTempOil;               // oil temperature      1°C/bit, valid -40°C...215°C
static INT16        iTempWat;               // water temperature    1°C/bit, valid -40°C...215°C
static INT16        iTempAir;               // air temperature      1°C/bit, valid -40°C...215°C
static TIME_TYPE    ClockTime;              // copy of RTC data



/* const array with parameter names */
/* NOTE: MUST be adequate to enum type SURVP_ID! */
static const STRING szSurvParamDesc[] =
{
    RESTXT_STATE_ALLRIGHT       ,  // 00
    RESTXT_STATE_OILTEMP        ,  // 01
    RESTXT_STATE_WATERTEMP      ,  // 02
    RESTXT_STATE_BATT_LOW  ,  // 03
    RESTXT_STATE_BATT_HIGH ,  // 04
    RESTXT_STATE_ALTERNATOR     ,  // 05
    RESTXT_STATE_OILPRESS       ,  // 06
    RESTXT_STATE_OILSWDEF       ,  // 07
    RESTXT_STATE_ENGINE_COLD    ,  // 08
    RESTXT_STATE_FUEL8L         ,  // 09
    RESTXT_STATE_FUEL4L         ,  // 10
    RESTXT_STATE_ABS            ,  // 11
    RESTXT_STATE_WATTEMPSW      ,  // 12
    RESTXT_STATE_GLACED         ,  // 13
    RESTXT_STATE_SERVICEKM      ,  // 14
    RESTXT_STATE_SERVICEHOUR    ,  // 15
    RESTXT_STATE_SIMULATION     ,  // 16
    RESTXT_STATE_HARDCOPY       ,  // 17
    RESTXT_STATE_DLS_SUMMER     ,  // 18
    RESTXT_STATE_DLS_WINTER     ,  // 19
    RESTXT_STATE_RTC_BATT       ,  // 20
    RESTXT_STATE_COOLRIDE          // 21
};

// just for debug purpose
static const STRING szErrorLevel[] =
{
    RESTXT_STATE_LVL_OK,
    RESTXT_STATE_LVL_INFO,
    RESTXT_STATE_LVL_WARN,
    RESTXT_STATE_LVL_ERR,
};


/* ----------------------------------------------------- */
/* THE Surveillance List - empty at startup */

/* NOTE: Any new surveillance parameter with state != ok will be inserted
         into this list. If the state returns to '== ok', it will be removed.
         At runtime, this list conatins a sorted list (error first, infos last)
         of all currently available surveillance parameters. */
static SURVP_TYPE  SurvParamList[SURVP_MAX];
/* ----------------------------------------------------- */




/* other surveillance helpers */
UINT8           SurvParamListCount;                     /* number of states currently not ok */

static INT8     CurStateTextIdx = 0;                    /* index of vehicle parameter currently shown */
       UINT8    eSurvGlobalState;                       /* Most top parameter state, used for Icon in 'MainDevice' */
       char     szSurvGlobalState[VEHSTATE_TXT_LEN+5];  /* Most top parameter string, used by 'MainDevice' and 'MonDev_')
                                                           (+ some security chars) */







/***********************************************************************
 *  FUNCTION:       Surv_Init
 *  DESCRIPTION:    initilizes vehicle surveillance
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:
 *********************************************************************** */
ERRCODE Surv_Init(void)
{
    UINT8 i;

    /* completely clear all surveilled parameters */
    Surv_ResetAllParameters();

    /* show our empty list */
    Surv_ListShow();

    /* default global surrveilance state (before starting surveillance after x seconds */
    Surv_SetGlobalState( 0 );

    /* done */
    ODS(DBG_MEAS, DBG_INFO, "Surv_Init() Done!");
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       Surv_UpdateMeasData
 *  DESCRIPTION:    Update of all surveilled measurement data
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void Surv_UpdateMeasData (void)
{
    // values should now be valid ---------------------
    iTempAir    = AnaInGetAirTemperature();   // if -ANAIN_TEMP_OFFSET -> not available!
    iTempWat    = AnaInGetWatTemperature();   // if -ANAIN_TEMP_OFFSET -> not available!
    iTempOil    = AnaInGetOilTemperature();   // if -ANAIN_TEMP_OFFSET -> not available!
    wBattSupply = AnaInGetVoltage();
    wAlternator = AnaInGetAltwVoltage();
    wRPM        = MeasGetEngineSpeed(MR_RPM_R10);
    wWheelSpeed = MeasGetWheelSpeed(MR_KM_PER_H);
    TimeDate_GetTime( &ClockTime );           // get current RTC time
}



/***********************************************************************
 *  FUNCTION:       Surv_UpdateStatistics
 *  DESCRIPTION:    updates min/max values
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Updates Min/Max values under these conditions:
 *                      - the analog data must be valid
 *                      - the analog data exceeds old limit (> Max / < Min)
 *                      - the old limit has never been initialized (ANAIN_INVALID_x)
 *********************************************************************** */
void Surv_UpdateStatistics (void)
{
    // check battery voltage (if valid):
    if ( wBattSupply != ANAIN_INVALID_U )
    {
        if (  ( EE_Volt_Max < wBattSupply     )
            ||( EE_Volt_Max == ANAIN_INVALID_U ) )
                EE_Volt_Max = wBattSupply;

        if (  ( EE_Volt_Min > wBattSupply )
            ||( EE_Volt_Min == ANAIN_INVALID_U ) )
                EE_Volt_Min = wBattSupply;
    }

    // check air temperature (if valid & connected)
    if (  ( iTempAir != ANAIN_INVALID_S        )
        &&( iTempAir > ANAIN_TEMP_SENSORDETECT ) )
    {
        if (  ( EE_TAir_Max < iTempAir         )
            ||( EE_TAir_Max == ANAIN_INVALID_S ) )
                EE_TAir_Max = iTempAir;

        if (  ( EE_TAir_Min > iTempAir         )
            ||( EE_TAir_Min == ANAIN_INVALID_S ) )
                EE_TAir_Min = iTempAir;
    }

    // check oil temperature (if valid & connected)
    if (  ( iTempOil != ANAIN_INVALID_S        )
        &&( iTempOil > ANAIN_TEMP_SENSORDETECT ) )
    {
        if (  ( EE_TOil_Max < iTempOil         )
            ||( EE_TOil_Max == ANAIN_INVALID_S ) )
                EE_TOil_Max = iTempOil;

        if (  ( EE_TOil_Min > iTempOil         )
            ||( EE_TOil_Min == ANAIN_INVALID_S ) )
                EE_TOil_Min = iTempOil;
    }

    // check water temperature (if valid & connected)
    if (  ( iTempWat != ANAIN_INVALID_S        )
        &&( iTempWat > ANAIN_TEMP_SENSORDETECT ) )
    {
        if (  ( EE_TWat_Max < iTempWat         )
            ||( EE_TWat_Max == ANAIN_INVALID_S ) )
                EE_TWat_Max = iTempWat;

        if (  ( EE_TWat_Min > iTempWat )
            ||( EE_TWat_Min == ANAIN_INVALID_S ) )
                EE_TWat_Min = iTempWat;
    }

    // check RPM max:
    if ( EE_RPM_Max < wRPM )
         EE_RPM_Max = wRPM;

    // check Speed max
    if ( EE_SpeedMax < wWheelSpeed )
         EE_SpeedMax = wWheelSpeed;

}



/***********************************************************************
 *  FUNCTION:       Surv_UpdateEngRunTime
 *  DESCRIPTION:    Updates EngineRunTime (All/Service) every second,
 *                  if engine runs, checks service km intervall
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Engine RunTimes will be saved in NVRAM  automatically
 *********************************************************************** */
void Surv_UpdateEngRunTime ( void )
{
    static UINT8 bLastSecond = 0;               // to prevent multiple time stamp increments

    // check basic conditions
    if (  ( wRPM           > SURV_ENGRTIME_RPM )   // engine runs?
        &&( ClockTime.bSec != bLastSecond   ) ) // RTC second has wrapped around?
    {
        bLastSecond = ClockTime.bSec;           // lock!

        // support Service Engine RunTime:
        // Note: Motobau wants to incr. ServiceHours only, no warning, but
        //       default version uses ServiceHours to detect service limit
        #if (BIKE_MOTOBAU == 1)
        NV_EngRunTime_Srv.bSec++;
        if (NV_EngRunTime_Srv.bSec > 59)
        {
            NV_EngRunTime_Srv.bSec = 0;
            NV_EngRunTime_Srv.bMin++;
            if (NV_EngRunTime_Srv.bMin > 59)
            {
                NV_EngRunTime_Srv.bMin = 0;
                NV_EngRunTime_Srv.wHour++;
                if (NV_EngRunTime_Srv.wHour > SURV_ENGRTIME_SRV_MAX)
                    NV_EngRunTime_Srv.wHour = SURV_ENGRTIME_SRV_MAX;  // limit time
            }
        }
        #endif // BIKE_MOTOBAU

        // support Overall Engine RunTime:
        NV_EngRunTime_All.bSec++;
        if (NV_EngRunTime_All.bSec > 59)
        {
            NV_EngRunTime_All.bSec = 0;
            NV_EngRunTime_All.bMin++;
            if (NV_EngRunTime_All.bMin > 59)
            {
                NV_EngRunTime_All.bMin = 0;
                NV_EngRunTime_All.wHour++;
                if (NV_EngRunTime_All.wHour > SURV_ENGRTIME_ALL_MAX)
                    NV_EngRunTime_All.wHour = SURV_ENGRTIME_ALL_MAX;  // limit time
            }
        }
    }
}


/***********************************************************************
 *  FUNCTION:       Surv_CheckRPMFlash
 *  DESCRIPTION:    Controls the RPM Flash light info & eGPO_0 output,
 *                  switches on both, if current RPM value exceeds
 *                  setting in eeprom.
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        RPM Flash lamp is enabled, if eeprom value > 0 RPM!
 *********************************************************************** */
void Surv_CheckRPMFlash ( void )
{
    static BOOL RPMFlash_On = FALSE;
    MESSAGE msg;

    // check: Activate RPM Flash?
    if (  ( EE_RPM_Flash > 0        )      // feature enabled?
        &&( wRPM > EE_RPM_Flash     )      // limit exceeded?
        &&( RPMFlash_On == FALSE ) )    // not already activated?
    {
        // ENABLE INFO LAMP
        RPMFlash_On = TRUE;                 // lock!
        LED_SetNewState( eLED_INFO, LED_ON ); // permanent on

        // special MOTOBAU behaviour
        #if(BIKE_MOTOBAU==1)
        // kai want's to enable an additional external lamp/indicator
        PIN_GPO0 = 1;                       // enable output eGPO_0 too
        #endif // BIKE_MOTOBAU
    }

    // check: Deactivate RPM Flash?
    if (  (  ( EE_RPM_Flash == 0      )      // feature disabled (at runtime)?
           &&( RPMFlash_On == TRUE ) )    // still active?

        ||(  ( EE_RPM_Flash > 0       )      // feature enabled?
           &&( wRPM < EE_RPM_Flash    )      // back under limit?
           &&( RPMFlash_On == TRUE ) ) )  // still active?
    {
        // DISABLE INFO LAMP
        RPMFlash_On = FALSE;                    // unlock!
        LED_SetNewState( eLED_INFO, LED_OFF );    // permanent off

        // special MOTOBAU behaviour
        #if(BIKE_MOTOBAU==1)
        // kai want's to enable an additional external lamp/indicator
        PIN_GPO0 = 0;                       // disable output eGPO_0 too
        #endif // BIKE_MOTOBAU
    }
}



/***********************************************************************
 *  FUNCTION:       Surv_CheckService
 *  DESCRIPTION:    Check wether ServiceIntervall (hours) reached
 *                  (Eeprom setting)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void Surv_CheckService ( void )
{
    DIST_TYPE   NV_VehicDist;      // current vehicle distance
    SURVP_ID    vstateprm;      // vehicle state parameter
    SURVP_STATE vstatelvl;      // vehicle state level

    // default value: ok!
    vstateprm = eSURVID_SERVICEHOUR;
    vstatelvl = eSURVST_OK;

    // check: current vehicle runtime hours reached service intervall hours?
    if (  (NV_EngRunTime_Srv.wHour != NV_EngRunTime_Srv_def.wHour )      // check enabled?
        &&(NV_EngRunTime_Srv.wHour <= NV_EngRunTime_All.wHour     ) )    // exceeded hours?
        vstatelvl = eSURVST_WARNING;

    // update this warning
    Surv_ListSetParamState(vstateprm, vstatelvl);

    // default value: ok
    vstateprm = eSURVID_SERVICEKM;
    vstatelvl = eSURVST_OK;

    // check: current vehicle distance reached service intervall?
    NV_VehicDist = Meas_Get_VehicDist( MR_KM );
    if (  (EE_NextSrvKm.km != EE_NextSrvKm_def.km )      // check enabled?
        &&(EE_NextSrvKm.km <= NV_VehicDist.km       ) )    // exceeded distance?
        vstatelvl = eSURVST_WARNING;

    // update this warning
    Surv_ListSetParamState(vstateprm, vstatelvl);

}




/***********************************************************************
 *  FUNCTION:       Surv_ProcessAll
 *  DESCRIPTION:    vehicle surveillance update, will be called
 *                  cyclicly every 20 ms via TimerInterrupt
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        We use a short start delay SURV_STARTDELAY to prevent
 *                  unpredictable messages at ADC start behavior.
 *********************************************************************** */
ERRCODE Surv_ProcessAll(void)
{
    // check startup delay ---------------------------
    if ( wSecCounter  >  SURV_STARTDELAY )
    {
        Surv_UpdateMeasData ();         // updates analog data
        Surv_UpdateStatistics();        // updates min/max values
        Surv_UpdateEngRunTime();        // updates engine runtime Serv/All

        Surv_CheckDigitalPorts();       // check all digital ports relevant for this bike
        Surv_CheckAnalogPorts();        // check all analog ports relevant for this bike
        Surv_CheckService();            // check for hour Service Intervall
        Surv_CheckDevice();             // check all device internal infos/warnings/errors
        Surv_CheckRPMFlash();           // Updates RPM Flash information

        //Surv_SetLEDState();              // update Info/Warn/Err LED status
    }

    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       Surv_ScrollVehicleState
 *  DESCRIPTION:    scrolls currently displayed vehicle state string
 *                  to next upper/lower active vehicle state string
 *                  if possible and available
 *  PARAMETER:      VST_SCROLL_UP       ScrollUp   = TRUE
 *                  VST_SCROLL_DOWN     ScrollDown = FALSE
 *  RETURN:         -
 *  COMMENT:        Controls global surveillance status + string too!
 *********************************************************************** */
void Surv_ScrollVehicleState(BOOL fScrollDir)
{
    static  UINT8   ucIdx = 0;      // last used read list idx
            UINT8   ucCount;        // overall error count

    /* check: currently no parameter active? */
    ucCount = Surv_ListGetCount(eSURVST_ALL);

    /* clip current index to max. */
    if ( ucCount == 0 )
        ucIdx = 0;                      // no error -> reste index
    else if ( ucIdx >= ucCount )
        ucIdx = ucCount-1;              // always one less than max

    /* try to setup next upper index */
    if (  ( fScrollDir == VST_SCROLL_UP )
        &&( ucIdx      >  0             ) )
            ucIdx--;                    // sroll up if possible

    /* OR try setup next lower possible index */
    if (  ( fScrollDir == VST_SCROLL_DOWN )
        &&( ucIdx      <  (ucCount-1)     ) )
            ucIdx++;                    // scroll down if possible
    ODS1(DBG_MEAS, DBG_INFO, "Scrolled to %u", ucIdx );

    /* update our global state */
    Surv_SetGlobalState( ucIdx );

}




/***********************************************************************
 *  FUNCTION:       Surv_CheckAnalogPorts
 *  DESCRIPTION:    check all analog values and update monitor states
 *  PARAMETER:      -
 *  RETURN:         nothing
 *  COMMENT:
 *********************************************************************** */
void Surv_CheckAnalogPorts(void)
{
    SURVP_ID      vstateprm;      // vehicle state parameter
    SURVP_STATE   vstatelvl;      // vehicle state level

    /* Check eSURVID_ENGINECOLD -------------------------------------
        default:                                               -> Ok
        if wRPM over warning limit AND (oil OR water to cold): -> Warning! */
    vstateprm = eSURVID_ENGINECOLD;
    vstatelvl = eSURVST_OK;
    if (  ( wRPM > SURV_ENGCOLD_RPM                         )
        &&(  (  (iTempOil > ANAIN_TEMP_SENSORDETECT )
              &&(iTempOil < SURV_OILTEMP_LOW            ) )
           ||(  (iTempWat > ANAIN_TEMP_SENSORDETECT )
              &&(iTempWat < SURV_WATTEMP_LOW            ) ) ) )
           vstatelvl = eSURVST_INFO;
    /* now add this resh state to our surveillance list */
    Surv_ListSetParamState(vstateprm, vstatelvl);

    /* Check eSURVID_GLACED -------------------------------------
        default:                 -> Ok
        if air between -2..+2°C  -> Warning! */
    vstateprm = eSURVID_GLACED;
    vstatelvl = eSURVST_OK;
    if (  ( iTempAir > SURV_AIRTEMP_GLACED_LOW  )
        &&( iTempAir < SURV_AIRTEMP_GLACED_HIGH ) )
           vstatelvl = eSURVST_INFO;
    /* now add this resh state to our surveillance list */
    Surv_ListSetParamState(vstateprm, vstatelvl);

    /* Check eSURVID_OILTEMP -------------------------------------
        default value:          -> Ok
        if oil temp very high:  -> Warning
        if oil temp too high:   -> Error */
    vstateprm = eSURVID_OILTEMP;
    vstatelvl = eSURVST_OK;
    if (iTempOil  > SURV_OILTEMP_WARN)
        vstatelvl = eSURVST_WARNING;
    if (iTempOil  > SURV_OILTEMP_ERR)
        vstatelvl = eSURVST_ERROR;
    /* now add this resh state to our surveillance list */
    Surv_ListSetParamState(vstateprm, vstatelvl);

    /* Check eSURVID_WATTEMP -------------------------------------
        default value:            -> Ok
        if water temp very high:  -> Warning
        if water temp too high:   -> Error */
    vstateprm = eSURVID_WATTEMP;
    vstatelvl = eSURVST_OK;
    if (iTempWat  > SURV_WATTEMP_WARN)
        vstatelvl = eSURVST_WARNING;
    if (iTempWat  > SURV_WATTEMP_ERR)
        vstatelvl = eSURVST_ERROR;
    /* now add this resh state to our surveillance list */
    Surv_ListSetParamState(vstateprm, vstatelvl);

    /* Check eSURVID_ALTERNATOR -------------------------------
        default value:                                      -> Ok
        if wRPM over warning limit and wAlternator too low: -> Warning! */
    if ( EE_BikeType == eBIKE_R100GS )
    {
        // special BIKE_R100GS checking
        vstateprm = eSURVID_ALTERNATOR;
        vstatelvl = Surv_ListGetParamState(vstateprm);     // first get current state

        /* Note: The warning will be cleared only, if the ALTW voltage will be
                 permanantly > threshold (or RPM == 0)! */
        if ( vstatelvl == eSURVST_OK )                  // no warning until now?
        {   if (  ( wRPM        > SURV_ALTERN_LOW_RPM)       // check: RPM over check limit?
                &&(wAlternator  < SURV_ALTERN_LOW    ) )     //        AND voltage below threshold?
            {   vstatelvl = eSURVST_WARNING;            // SET WARNING!
            }
        }
        else                                            // we already had a warning!
        {   if (  ( wRPM        == 0         )          // no error if motor stands still
                ||( wAlternator > SURV_ALTERN_LOW ) )        // check: voltage higher than threshold?
            {   vstatelvl = eSURVST_OK;                 // CLEAR WARNING!
            }
        }
        // check: if not using Sixo-Warnmode, setup LED directly here
        if ( EE_DevFlags_2.flags.fLedWarnModeStd == SURV_LWM_STD )
        {
            if ( vstatelvl != eSURVST_OK )
                 LED_SetNewState(eLED_WARN, LED_ON );
            else LED_SetNewState(eLED_WARN, LED_OFF);
        }
    }
    else
    {   /* standard behaviour: not used, always ok */
        vstateprm = eSURVID_ALTERNATOR;
        vstatelvl = eSURVST_OK;
    }

    /* now add this fresh state to our surveillance list */
    Surv_ListSetParamState(vstateprm, vstatelvl);

    /* Check eSURVID_SURV_BATT_LOW -------------------------------
        default value:                                      -> Ok
        if wRPM over warning limit and wBattSupply too low: -> Warning! */
    vstateprm = eSURVID_SURV_BATT_LOW;
    vstatelvl = eSURVST_OK;
    if (  (wRPM > SURV_BATT_LOW_RPM)
        &&(wBattSupply < SURV_BATT_LOW   ) )
        vstatelvl = eSURVST_WARNING;
    /* now add this resh state to our surveillance list */
    Surv_ListSetParamState(vstateprm, vstatelvl);

    /* Check eSURVID_SURV_BATT_HIGH -------------------------------
        default value:           -> Ok
        if wBattSupply too high: -> Error! */
    vstateprm = eSURVID_SURV_BATT_HIGH;
    vstatelvl = eSURVST_OK;
    if (wBattSupply > SURV_BATT_HIGH )
        vstatelvl = eSURVST_ERROR;
    /* now add this resh state to our surveillance list */
    Surv_ListSetParamState(vstateprm, vstatelvl);

}



/***********************************************************************
 *  FUNCTION:       Surv_CheckDevice
 *  DESCRIPTION:    check all device internal states
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void Surv_CheckDevice(void)
{
    /* User Info: do not drive with vehicle simulation on! */
    if (EE_DevFlags_2.flags.fVehicSimul == TRUE)
         Surv_ListSetParamState(eSURVID_SIMULATION, eSURVST_INFO);
    else Surv_ListSetParamState(eSURVID_SIMULATION, eSURVST_OK);

    /* User warning: fHardcopyAvail via HBEAM-switch active? */
    /* Note: If in Debugger-Mode KD30, Prevent warning for undisturbed screenshots */
#if(KD30_USED==0)
    if (EE_DevFlags_2.flags.fHardcopyAvail == TRUE)
         Surv_ListSetParamState(eSURVID_HARDCOPY, eSURVST_INFO);
    else Surv_ListSetParamState(eSURVID_HARDCOPY, eSURVST_OK);
#endif  // KD30_USED

    /* User Warning: Automatic RTC Clock change because of summer/winter time? */
    if (  ( EE_DevFlags_2.flags.fDLSAuto == TRUE )
        &&( fCESTChanged                         == TRUE ) )
    {
        fCESTChanged = FALSE;               // reset for next change detection
        if ( TimeDate_GetCEST() == TRUE )   // select msg
             Surv_ListSetParamState(eSURVID_DLS_SUMMER, eSURVST_INFO);
        else Surv_ListSetParamState(eSURVID_DLS_WINTER, eSURVST_INFO);
    }
    else
    {   Surv_ListSetParamState(eSURVID_DLS_SUMMER, eSURVST_OK);
        Surv_ListSetParamState(eSURVID_DLS_WINTER, eSURVST_OK);
    }

    /* User Warning: NVRAM delivered invalid vales -> RTC Batterie defect? */
    if (  fRTCDefect == TRUE )
         Surv_ListSetParamState(eSURVID_RTC_BATT, eSURVST_WARNING);
    else Surv_ListSetParamState(eSURVID_RTC_BATT, eSURVST_OK);

}



/***********************************************************************
 *  FUNCTION:       Surv_CheckDigitalPorts
 *  DESCRIPTION:    check all other digital in ports and sets
 *                  vehicle state
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        We use digital filtered values only
 *********************************************************************** */
void Surv_CheckDigitalPorts(void)
{
    /* ============================================================== */
    /* COMMON WARNINGS */
    // nothing tbd. yet

    /* ============================================================== */
    /* BIKE SPECIFIC WARNINGS */
    switch ( EE_BikeType )
    {
        /* SPECIAL HANDLING FOR BMW F650 ============================= */
        case eBIKE_F650:
        {
            /* --------------------------------------------- */
            /* F650 - Fuel 4 l Switch (low active) - Warning */
            if ( DF_Fuel_4l_F650 == FALSE )
                 Surv_ListSetParamState(eSURVID_FUEL4L, eSURVST_WARNING);
            else Surv_ListSetParamState(eSURVID_FUEL4L, eSURVST_OK);

            /* --------------------------------------------- */
            /* F650 - ABS inactive signal (low active) - Error */
            if ( DF_ABS_Warn_F650 == 0 )
                 Surv_ListSetParamState(eSURVID_ABS, eSURVST_ERROR);
            else Surv_ListSetParamState(eSURVID_ABS, eSURVST_OK);
            // check: if not using Sixo-Warnmode, setup Error-LED directly here
            if ( EE_DevFlags_2.flags.fLedWarnModeStd == SURV_LWM_STD )
            {   if ( DF_ABS_Warn_F650 == 0 )
                     LED_SetNewState(eLED_WARN, LED_ON );
                else LED_SetNewState(eLED_WARN, LED_OFF);
            }

            /* --------------------------------------------- */
            /* F650 - Watertemp High Switch (high active) - Error */
            if ( DF_Temp_Warn_F650 == 1)
                 Surv_ListSetParamState(eSURVID_WATTEMPSW, eSURVST_ERROR);
            else Surv_ListSetParamState(eSURVID_WATTEMPSW, eSURVST_OK);
            // check: if not using Sixo-Warnmode, setup LED directly here
            if ( EE_DevFlags_2.flags.fLedWarnModeStd == SURV_LWM_STD )
            {   if ( DF_ABS_Warn_F650 == 0 )
                     LED_SetNewState(eLED_ERROR, LED_ON );
                else LED_SetNewState(eLED_ERROR, LED_OFF);
            }
        } break;

        /* SPECIAL HANDLING FOR HONDA AFRICATWIN ============================= */
        case eBIKE_AFRICATWIN:
        {
            /* --------------------------------------------- */
            /* AfricaTwin - Fuel 8 l - Warning */
            if ( DF_Fuel_8l_AT == 0)                 // low active
                 Surv_ListSetParamState(eSURVID_FUEL8L, eSURVST_WARNING);
            else Surv_ListSetParamState(eSURVID_FUEL8L, eSURVST_OK);

            /* --------------------------------------------- */
            /* AfricaTwin - Oil switch defect? - Warning */
            if (  ( DF_OILSW == 1 )         // no oil signal
                &&( wRPM     == 0 ) )       // but engine stands still?
                 Surv_ListSetParamState(eSURVID_OILSWDEF, eSURVST_WARNING);
            else Surv_ListSetParamState(eSURVID_OILSWDEF, eSURVST_OK);
            // check: if not using Sixo-Warnmode, setup LED directly here
            if ( EE_DevFlags_2.flags.fLedWarnModeStd == SURV_LWM_STD )
            {   if ( DF_Fuel_4l_AT == 0 )      // low active
                     LED_SetNewState(eLED_WARN, LED_ON );
                else LED_SetNewState(eLED_WARN, LED_OFF );
            }

            /* --------------------------------------------- */
            /* AfricaTwin - Oil pressure ok? - Error (low active) */
            if (  ( DF_OILSW == 0             )      // low active
                &&( wRPM     >  SURV_OILPRESS_RPM ) )    // and engine running?
                 Surv_ListSetParamState(eSURVID_OILPRESS, eSURVST_ERROR);
            else Surv_ListSetParamState(eSURVID_OILPRESS, eSURVST_OK);
            // check: if not using Sixo-Warnmode, setup LED directly here
            if ( EE_DevFlags_2.flags.fLedWarnModeStd == SURV_LWM_STD )
            {   if ( DF_OILSW == 0 )      // low active
                     LED_SetNewState(eLED_ERROR, LED_ON );
                else LED_SetNewState(eLED_ERROR, LED_OFF );
            }

            /* --------------------------------------------- */
            /* AfricaTwin - Fuel 4 l - Error (low active)*/
            if ( DF_Fuel_4l_AT == 0 )
                 Surv_ListSetParamState(eSURVID_FUEL4L, eSURVST_ERROR);
            else Surv_ListSetParamState(eSURVID_FUEL4L, eSURVST_OK);

        } break;


        /* SPECIAL HANDLING FOR MuZ BAGHIRA ============================= */
        case eBIKE_BAGHIRA:
        {
            /* --------------------------------------------- */
            /* Baghira - Water-Temperatur - Error (high active) */
            if ( DF_Temp_Warn_BAGHIRA == 1)          // high active
                 Surv_ListSetParamState(eSURVID_WATTEMPSW, eSURVST_ERROR);
            else Surv_ListSetParamState(eSURVID_WATTEMPSW, eSURVST_OK);
            // check: if not using Sixo-Warnmode, setup LED directly here
            if ( EE_DevFlags_2.flags.fLedWarnModeStd == SURV_LWM_STD )
            {   if ( DF_Temp_Warn_BAGHIRA == 1)          // high active
                     LED_SetNewState(eLED_ERROR, LED_ON );
                else LED_SetNewState(eLED_ERROR, LED_OFF );
            }

        } break;

        /* SPECIAL HANDLING FOR BMW R100GS ============================= */
        case eBIKE_R100GS:
        {
            /* --------------------------------------------- */
            /* R100GS - Oil switch defect? - Warning */
            if (  ( DF_OILSW == 1 )      // no oil signal
                &&( wRPM     == 0 ) )    // but engine stands still?
                 Surv_ListSetParamState(eSURVID_OILSWDEF, eSURVST_WARNING);
            else Surv_ListSetParamState(eSURVID_OILSWDEF, eSURVST_OK);

            /* --------------------------------------------- */
            /* R100GS  - Oil pressure ok? */
            if (  ( DF_OILSW == 0             )      // low active
                &&( wRPM     >  SURV_OILPRESS_RPM ) )    // and engine running?
                 Surv_ListSetParamState(eSURVID_OILPRESS, eSURVST_ERROR);
            else Surv_ListSetParamState(eSURVID_OILPRESS, eSURVST_OK);
            // check: if not using Sixo-Warnmode, setup LED directly here
            if ( EE_DevFlags_2.flags.fLedWarnModeStd == SURV_LWM_STD )
            {   if ( DF_OILSW == 0 )      // low active
                     LED_SetNewState(eLED_ERROR, LED_ON );
                else LED_SetNewState(eLED_ERROR, LED_OFF );
            }

        }

        /* DEFAULT HANDLING FOR ALL OTHER BIKES ============================= */
        default:
        {
            /* --------------------------------------------- */
            /* nothing to check here in general.. */

        } break;
    }
}




/***********************************************************************
 *  FUNCTION:       Surv_ResetAllParameters
 *  DESCRIPTION:    Resets any surveilled parameter by clearing complete
 *                  list.
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void Surv_ResetAllParameters(void)
{
    // clear our current parameter list
    memset (&SurvParamList, 0x0, sizeof(SurvParamList) );

    // list is empty, count of currently detected info/warn/error is 0!
    SurvParamListCount = 0;
}



/***********************************************************************
 *  FUNCTION:       Surv_ListSetParamState
 *  DESCRIPTION:    Inserts/Remove given surveillance parameter.
 *                  - state = eSURVST_OK: delete parameter from list
 *                  - state > eSURVST_OK: add/update parameter at sorted position
 *  PARAMETER:      SURVP_ID     parameter
 *                  SURVP_STATE  state
 *  RETURN:         nothing
 *  COMMENT:        The generated list is sorted:
 *                      Most top: all eSURVST_ERROR
 *                      then:     all eSURVST_WARNING
 *                      then:     all eSURVST_INFO
 *                  Parameters with state eSURVST_OK do not exist,
 *                  they will be removed.
 *                  Each parameter can occur only ONCE inside the list.
 *                  If a new state != eSURVST_OK is given, the parameter
 *                  will be removed and re-inserted at correct position.
 *                  If list is full, the least important parameter will
 *                  be removed.
 *********************************************************************** */
void Surv_ListSetParamState( SURVP_ID      GivenParam,
                            SURVP_STATE   GivenState   )
{
    UINT8   bListIndex;     // index of parameter inside surveillance list

    // try to find inside our list (if not already in list: bListIndex = SURVP_MAX )
    bListIndex = Surv_ListGetIndex( GivenParam );

    // check: parameter never inserted and still ok -> irrelevant!
    if (  ( bListIndex == SURVP_MAX )
        &&( GivenState == eSURVST_OK     ) )
        return;

    // check: parameter exists but unchanged -> nothing to do!
    if (  ( bListIndex <  SURVP_MAX                  )
        &&( GivenState == SurvParamList[bListIndex].state ) )
        return;

    // check: parameter is set back to 'ok'?
    if( GivenState == eSURVST_OK )
    {
        /* ok, remove from list */
        Surv_ListRemoveParam ( bListIndex );
    }
    else
    {
        // check: already inside list?
        if (bListIndex < SURVP_MAX )
        {
            // ok, replace & resort parameter
            Surv_ListUpdateParam ( bListIndex, GivenState );
        }
        else
        {
            // ok, add new parameter sorted
            Surv_ListAddParam( GivenParam, GivenState );
        }
    }

    /* show our updated list */
    Surv_ListShow();

    /* update 'GlobalSurvState' = most top surv.-parameter (index 0)
       for MainDevice-Status field */
    Surv_SetGlobalState(0);    // use most important (top) list entry

    /* update our led states */
    Surv_SetLEDState();
}



/***********************************************************************
 *  FUNCTION:       Surv_ListGetParamState
 *  DESCRIPTION:    returns state of given parameter
 *  PARAMETER:      SURVP_ID
 *  RETURN:         SURVP_STATE     error/warning/level
 *  COMMENT:        If parameter not found (inactive),
 *                  then eSURVST_OK is returned
 *********************************************************************** */
SURVP_STATE Surv_ListGetParamState(SURVP_ID parameter)
{
    SURVP_STATE   RValue;
    UINT8                   idx;

    /* try to find parameter */
    idx = Surv_ListGetIndex( parameter );

    /* check: not found? */
    if ( idx == SURVP_MAX  )
    {   RValue = eSURVST_OK;                // not found => ok!
    }
    else
    {   RValue = SurvParamList[idx].state;  // found: return state of parameter
    }

    // return state of parameter
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       Surv_ListGetIndex
 *  DESCRIPTION:    Tries to find the given surveillance parameter
 *                  in the current list.
 *  PARAMETER:      parameter       SurvParametere to be found/returned
 *  RETURN:         index           of parameter (if found)
 *                  SURVP_MAX  if not found
 *  COMMENT:        Just to find out presence and state of given parameter.
 *                  If parameter does NOT have an info/warning/error state
 *                  (means, it is 'ok' or 'inactive', the parameter will
 *                  NOT be found!
 *********************************************************************** */
UINT8 Surv_ListGetIndex( SURVP_ID parameter )
{
    UINT8 i;

    // scan complete list - until found
    for ( i=0; i<SURVP_MAX; i++ )
    {   if ( SurvParamList[i].param == parameter )
        {   break;      // found!
        }
        else
        {   // just go on..
        }
    }

    // return index/not found
    return i;
}



/***********************************************************************
 *  FUNCTION:       Surv_ListGetCount
 *  DESCRIPTION:    Returns number of current entries in parameter list
 *  PARAMETER:      u8GetMask       expects one of the SURVST_xxx bitmasks
 *  RETURN:         count           sum of active requested parameters
 *  COMMENT:        -
 *********************************************************************** */
UINT8 Surv_ListGetCount( UINT8 statemask )
{
    UINT8 i;
    UINT8 RValue = 0;

    // scan complete list - until found
    for ( i=0; i<SURVP_MAX; i++ )
    {
        // check: row is used AND state is one of our searched?
        if (  ( SurvParamList[i].param != eSURVID_NOENTRY )
            &&( SurvParamList[i].state &  statemask      ) )
            RValue++;
    }
    // return overall sum of all requested active parameters
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       Surv_ListRemoveParam
 *  DESCRIPTION:    Removes the given entry from by moving all following
 *                  entries one row up.
 *  PARAMETER:      bListIndex      Index to be removed from list
 *  RETURN:         -
 *  COMMENT:        Assumes the given parameter already to be found
 *                  via Surv_ListGetIndex()
 *********************************************************************** */
void Surv_ListRemoveParam ( UINT8 bListIndex )
{
    UINT8   i;

    ODS1(DBG_MEAS, DBG_INFO, "Surv_ListRemoveParam('%s')", szSurvParamDesc[SurvParamList[bListIndex].param]);

    // move all following entries 1 step up
    //  - start with given index
    //  - end one entry before(!) list end
    for ( i=bListIndex; i<(SURVP_MAX-1); i++ )
    {
        // move lower entry one row up
        SurvParamList[i].param = SurvParamList[i+1].param;
        SurvParamList[i].state = SurvParamList[i+1].state;
    }

    // clear last entry
    SurvParamList[SURVP_MAX-1].param = eSURVID_NOENTRY;
    SurvParamList[SURVP_MAX-1].state = eSURVST_OK;
}



/***********************************************************************
 *  FUNCTION:       Surv_ListUpdateParam
 *  DESCRIPTION:    Updates given parameter by first removing it
 *                  and the re-inserting (incl. correct sorting).
 *  PARAMETER:      bListIndex      entry to be updated
 *                  state           new state to be set
 *  RETURN:         -
 *  COMMENT:        Assumes the given parameter already to be found
 *                  via Surv_ListGetIndex()
 *********************************************************************** */
void Surv_ListUpdateParam ( UINT8 bListIndex, SURVP_STATE state )
{
    SURVP_TYPE     TempParam;

    ODS1(DBG_MEAS, DBG_INFO, "Surv_ListUpdateParam('%s')", szSurvParamDesc[SurvParamList[bListIndex].param]);

    // check: parameter already has correct state?
    if ( SurvParamList[bListIndex].state == state )
        return;     // unchanged, just return

    // first save parameter - with new state!
    TempParam.param = SurvParamList[bListIndex].param;
    TempParam.state = state;

    // now clear old parameter from list
    Surv_ListRemoveParam ( bListIndex );

    // then re-insert changed parameter
    Surv_ListAddParam( TempParam.param, TempParam.state );
}



/***********************************************************************
 *  FUNCTION:       Surv_ListAddParam
 *  DESCRIPTION:    Adds a new parameter to the list of active parameters
 *  PARAMETER:      parameter       parameter to be added
 *                  state           state of that parameter
 *  RETURN:         -
 *  COMMENT:        Inserts parameter at most top position, but with
 *                  priority of info/warning/error.
 *                  If list is complete full, the least important entry
 *                  will be removed.
 *********************************************************************** */
void Surv_ListAddParam( SURVP_ID parameter, SURVP_STATE state )
{
    UINT8               i;
    UINT8               count;
    SURVP_TYPE     NewParam;   // just to insert new one
    SURVP_TYPE     LastParam;  // just to save old last one to re-insert

    ODS1(DBG_MEAS, DBG_INFO, "Surv_ListAddParam('%s')", szSurvParamDesc[parameter]);

    // check: given parameter is != ok?
    if ( state == eSURVST_OK )
        return;     // our list does only contain parameters > ok --> just return

    // generate a new temporary parameter to be inserted
    NewParam.param = parameter;
    NewParam.state = state;

    // save last entry, might be re-inserted if list full & new one less important
    LastParam = SurvParamList[SURVP_MAX-1];

    // completely move current list one row down
    for ( i=(SURVP_MAX-1); i>0; i-- )
        SurvParamList[i] = SurvParamList[i-1];

    // now try to insert new parameter -
    //  - beginning from upper side
    //  - stop one before end
    for ( i=0; i<(SURVP_MAX-1); i++ )
    {
        // check: new parameter-state is >= next lower parameter-state?
        if ( NewParam.state  >= SurvParamList[i+1].state )
        {
            // This is the right position -> insert & stop our loop here!
            SurvParamList[i] = NewParam;
            break;      // list update complete
        }
        else
        {   // no, just move lower entry on row up (old position) and go on..
            SurvParamList[i] = SurvParamList[i+1];
        }
    }

    // check: we reached the last row? (new one is less important than all existing entries)
    if ( i == (SURVP_MAX-1) )
    {
        // re-insert previous last parameter
        SurvParamList[i] = LastParam;
        ODS(DBG_MEAS, DBG_WARNING, "List full, new entry discarded!" );
    }
}



/***********************************************************************
 *  FUNCTION:       Surv_ListShow
 *  DESCRIPTION:    just shows the current content of the list
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        just a debug helper: printout our new sorted list
 *********************************************************************** */
void Surv_ListShow( void )
{
    UINT8               i;
    UINT8               count;

    ODS(DBG_MEAS, DBG_INFO, "Surv-ParamList Content:" );
    count = Surv_ListGetCount(eSURVST_ALL);
    for ( i=0; i<SURVP_MAX; i++ )
    {
        ODS4(DBG_MEAS, DBG_INFO, "%u/%u: '%18s' : '%s'",
                i+1, count,
                szSurvParamDesc[SurvParamList[i].param],
                szErrorLevel   [SurvParamList[i].state] );
    }
}



/***********************************************************************
 *  FUNCTION:       Surv_SetGlobalState
 *  DESCRIPTION:    Updates global state & string
 *  PARAMETER:      ucIdx       List entry to be shown (if possible)
 *  RETURN:         -
 *  COMMENT:        global surveillance status + string are used by
 *                  MainDevice and MonDev_
 *********************************************************************** */
void Surv_SetGlobalState( UINT8 ucIdx )
{
    UINT8   ucCount;        // overall error count

    /* check: how many error parameters active? */
    ucCount = Surv_ListGetCount(eSURVST_ALL);

    /* clip idx to limit (=0 OR < max) */
    if (ucCount == 0)
        ucIdx = 0;
    else if (ucIdx >= ucCount )
        ucIdx = ucCount;

    /* update our global surv status */
    eSurvGlobalState = SurvParamList[ucIdx].state;
    sprintf( szSurvGlobalState, "%u/%u %s", (ucCount==0)?0:ucIdx+1, ucCount, szSurvParamDesc[SurvParamList[ucIdx].param] );
}



/***********************************************************************
 *  FUNCTION:       Surv_SetLEDState
 *  DESCRIPTION:    Updates global LED status for Info/Warn/Error,
 *                  based on our SurvList (instead of direct input switch)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Should cyclicly be called to to update LED status
 *                  at any time.
 *                  Toggle/Invert activation mode of each Info/Warn/Err LED
 *                  separately, BUT only if necessary!
 *********************************************************************** */
void Surv_SetLEDState( void )
{
    MESSAGE msg;            // for LED event message

    /* main switch: Use special SIxO warning Mode? */
    if ( EE_DevFlags_2.flags.fLedWarnModeStd == SURV_LWM_SIXO )
    {
        /* INFO-LED --------------------------------------  */
        if (  ( Surv_ListGetCount(eSURVST_INFO) >  0     )
            &&( LED_GetState(eLED_INFO) == FALSE ) )
        {   LED_SetNewState( eLED_INFO, LED_ON );
        }
        if (  ( Surv_ListGetCount(eSURVST_INFO) == 0     )
            &&( LED_GetState(eLED_INFO)     == TRUE ) )
        {   LED_SetNewState( eLED_INFO, LED_OFF );
        }

        /* WARN-LED --------------------------------------  */
        if (  ( Surv_ListGetCount(eSURVST_WARNING) >  0     )
            &&( LED_GetState(eLED_WARN)     == FALSE ) )
        {   LED_SetNewState( eLED_WARN, LED_ON );
        }
        if (  ( Surv_ListGetCount(eSURVST_WARNING) == 0     )
            &&( LED_GetState(eLED_WARN)     == TRUE ) )
        {   LED_SetNewState( eLED_WARN, LED_OFF );
        }

        /* ERROR-LED --------------------------------------  */
        if (  ( Surv_ListGetCount(eSURVST_ERROR) >  0     )
            &&( LED_GetState(eLED_ERROR)     == FALSE ) )
        {   LED_SetNewState( eLED_ERROR, LED_ON );
        }
        if (  ( Surv_ListGetCount(eSURVST_ERROR) == 0     )
            &&( LED_GetState(eLED_ERROR)     == TRUE ) )
        {   LED_SetNewState( eLED_ERROR, LED_OFF );
        }
    }

    // special MOTOBAU behaviour
    #if(BIKE_MOTOBAU==1)
    // kai wants to enable an additional external lamp/indicator
    //  - enable output eGPO_1 too for Warning/Error only
    PIN_GPO1 = Surv_ListGetCount(eSURVST_WARNING | eSURVST_ERR );
    #endif // BIKE_MOTOBAU
}
