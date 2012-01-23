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
 * SurvCheckAllDigitalWarnings()
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





// global data (will be initialized via EEPROM/NVRAM parameter init
RPM_TYPE    RPM_Flash;          // engine speed to enable flash lamp,   1 RPM/bit

RPM_TYPE    RPM_Max;            // max engine speed,                    1 RPM/bit
SPEED_TYPE  Speed_Max;          // max vehicel speed                    1 km/h/bit

UINT16      Volt_Min;           // minimal battery voltage,             0,01 V/bit
UINT16      Volt_Max;           // maximal battery voltage,             0,01 V/bit
INT16       TAir_Min;           // minimal air temperature,             1°C/bit, valid -40°C...215°C
INT16       TAir_Max;           // maximal air temperature,             1°C/bit, valid -40°C...215°C
INT16       TOil_Min;           // minimal oil temperature,             1°C/bit, valid -40°C...215°C
INT16       TOil_Max;           // maximal oil temperature,             1°C/bit, valid -40°C...215°C
INT16       TWat_Min;           // minimal water temperature,           1°C/bit, valid -40°C...215°C
INT16       TWat_Max;           // maximal water temperature,           1°C/bit, valid -40°C...215°C

TIME_TYPE_LD  EngRunTime_Srv;   // 4 bytes, engine runtime since last service
TIME_TYPE_LD  EngRunTime_All;   // 4 bytes, engine runtime overall

UINT16      gwFuelCap;          // tank size in 1/10 liters
UINT8       gbFuelCons;         // motor fuel consumption in 1/10 liter/100 km

BOOL        fCESTChanged = FALSE;   // DaylaightSaving 'CET/CEST changed flag'
BOOL        fRTCDefect = FALSE;     // RTC defect detection


// external data
extern far UINT16           wMilliSecCounter;       // high resolution short distance timer, ms,  max  65 sec
extern far UINT16           wSecCounter;            // low  resolution long  distance timer, sec, max. 18 h
extern far DEVFLAGS2_TYPE   gDeviceFlags2;          // device status parameters
extern far DEVFLAGS3_TYPE   gDeviceFlags3;          // device status parameters
extern far BIKE_TYPE        gBikeType;              // bike type
extern far DIST_TYPE        gNextServKm;            // to get/set original value
extern far DIST_TYPE        gNextServKm_def;        // to detect wether enabled/not
extern far TIME_TYPE_LD     EngRunTime_Srv_def;     // to detect wether enabled/not
extern far DIGFILTTYPE      DigInFilter[];          // digital filter table for all inputs


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
/* NOTE: must be adequate to enum type SURV_PARAM_ID_TYPE! */
static const STRING szSurvParamDesc[] =
{
    RESTXT_STATE_ALLRIGHT    ,  // 00
    RESTXT_STATE_OILTEMP     ,  // 01
    RESTXT_STATE_WATERTEMP   ,  // 02
    RESTXT_STATE_VOLTAGE_LOW ,  // 03
    RESTXT_STATE_VOLTAGE_HIGH,  // 04
    RESTXT_STATE_ALTERNATOR  ,  // 05
    RESTXT_STATE_OILPRESS    ,  // 06
    RESTXT_STATE_OILSWDEF    ,  // 07
    RESTXT_STATE_ENGINE_COLD ,  // 08
    RESTXT_STATE_FUEL8L      ,  // 09
    RESTXT_STATE_FUEL4L      ,  // 10
    RESTXT_STATE_ABS         ,  // 11
    RESTXT_STATE_WATTEMPSW   ,  // 12
    RESTXT_STATE_GLACED      ,  // 13
    RESTXT_STATE_SERVICEKM   ,  // 14
    RESTXT_STATE_SERVICEHOUR ,  // 15
    RESTXT_STATE_SIMULATION  ,  // 16
    RESTXT_STATE_HARDCOPY    ,  // 17
    RESTXT_STATE_DLS_SUMMER  ,  // 18
    RESTXT_STATE_DLS_WINTER  ,  // 19
    RESTXT_STATE_RTC_BATT       // 20
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
static SURV_PARAM_TYPE  SurvParamList[SURV_PARAM_MAX];
/* ----------------------------------------------------- */




/* other surveillance helpers */
UINT8           SurvParamListCount;                     /* number of states currently not ok */

static INT8     CurStateTextIdx = 0;                    /* index of vehicle parameter currently shown */
       UINT8    eSurvGlobalState;                       /* Most top parameter state, used for Icon in 'MainDevice' */
       char     szSurvGlobalState[VEHSTATE_TXT_LEN+5];  /* Most top parameter string, used by 'MainDevice' and 'MonitorDevice')
                                                           (+ some security chars) */







/***********************************************************************
 *  FUNCTION:       SurvInit
 *  DESCRIPTION:    initilizes vehicle surveillance
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:
 *********************************************************************** */
ERRCODE SurvInit(void)
{
    UINT8 i;

    /* completely clear all surveilled parameters */
    SurvResetAllParameters();

    /* show our empty list */
    SurvListShow();

    /* default global surrveilance state (before starting surveillance after x seconds */
    SurvSetGlobalState( 0 );

    /* done */
    ODS(DBG_MEAS, DBG_INFO, "SurvInit() Done!");
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       SurvUpdateMeasData
 *  DESCRIPTION:    Update of all surveilled measurement data
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SurvUpdateMeasData (void)
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
 *  FUNCTION:       SurvUpdateStatistics
 *  DESCRIPTION:    updates min/max values
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Updates Min/Max values under these conditions:
 *                      - the analog data must be valid
 *                      - the analog data exceeds old limit (> Max / < Min)
 *                      - the old limit has never been initialized (ANAIN_INVALID_x)
 *********************************************************************** */
void SurvUpdateStatistics (void)
{
    // check battery voltage (if valid):
    if ( wBattSupply != ANAIN_INVALID_U )
    {
        if (  ( Volt_Max < wBattSupply     )
            ||( Volt_Max == ANAIN_INVALID_U ) )
                Volt_Max = wBattSupply;

        if (  ( Volt_Min > wBattSupply )
            ||( Volt_Min == ANAIN_INVALID_U ) )
                Volt_Min = wBattSupply;
    }

    // check air temperature (if valid & connected)
    if (  ( iTempAir != ANAIN_INVALID_S        )
        &&( iTempAir > ANAIN_TEMP_SENSORDETECT ) )
    {
        if (  ( TAir_Max < iTempAir         )
            ||( TAir_Max == ANAIN_INVALID_S ) )
                TAir_Max = iTempAir;

        if (  ( TAir_Min > iTempAir         )
            ||( TAir_Min == ANAIN_INVALID_S ) )
                TAir_Min = iTempAir;
    }

    // check oil temperature (if valid & connected)
    if (  ( iTempOil != ANAIN_INVALID_S        )
        &&( iTempOil > ANAIN_TEMP_SENSORDETECT ) )
    {
        if (  ( TOil_Max < iTempOil         )
            ||( TOil_Max == ANAIN_INVALID_S ) )
                TOil_Max = iTempOil;

        if (  ( TOil_Min > iTempOil         )
            ||( TOil_Min == ANAIN_INVALID_S ) )
                TOil_Min = iTempOil;
    }

    // check water temperature (if valid & connected)
    if (  ( iTempWat != ANAIN_INVALID_S        )
        &&( iTempWat > ANAIN_TEMP_SENSORDETECT ) )
    {
        if (  ( TWat_Max < iTempWat         )
            ||( TWat_Max == ANAIN_INVALID_S ) )
                TWat_Max = iTempWat;

        if (  ( TWat_Min > iTempWat )
            ||( TWat_Min == ANAIN_INVALID_S ) )
                TWat_Min = iTempWat;
    }

    // check RPM max:
    if ( RPM_Max < wRPM )
         RPM_Max = wRPM;

    // check Speed max
    if ( Speed_Max < wWheelSpeed )
         Speed_Max = wWheelSpeed;

}



/***********************************************************************
 *  FUNCTION:       SurvUpdateEngRunTime
 *  DESCRIPTION:    Updates EngineRunTime (All/Service) every second,
 *                  if engine runs, checks service km intervall
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Engine RunTimes will be saved in NVRAM  automatically
 *********************************************************************** */
void SurvUpdateEngRunTime ( void )
{
    static UINT8 bLastSecond = 0;               // to prevent multiple time stamp increments

    // check basic conditions
    if (  ( wRPM           > ENGRUNTIME_RPM )   // engine runs?
        &&( ClockTime.bSec != bLastSecond   ) ) // RTC second has wrapped around?
    {
        bLastSecond = ClockTime.bSec;           // lock!

        // support Service Engine RunTime:
        // Note: Motobau wants to incr. ServiceHours only, no warning, but
        //       default version uses ServiceHours to detect service limit
        #if (BIKE_MOTOBAU == 1)
        EngRunTime_Srv.bSec++;
        if (EngRunTime_Srv.bSec > 59)
        {
            EngRunTime_Srv.bSec = 0;
            EngRunTime_Srv.bMin++;
            if (EngRunTime_Srv.bMin > 59)
            {
                EngRunTime_Srv.bMin = 0;
                EngRunTime_Srv.wHour++;
                if (EngRunTime_Srv.wHour > ENGRUNTIME_SRV_MAX)
                    EngRunTime_Srv.wHour = ENGRUNTIME_SRV_MAX;  // limit time
            }
        }
        #endif // BIKE_MOTOBAU

        // support Overall Engine RunTime:
        EngRunTime_All.bSec++;
        if (EngRunTime_All.bSec > 59)
        {
            EngRunTime_All.bSec = 0;
            EngRunTime_All.bMin++;
            if (EngRunTime_All.bMin > 59)
            {
                EngRunTime_All.bMin = 0;
                EngRunTime_All.wHour++;
                if (EngRunTime_All.wHour > ENGRUNTIME_ALL_MAX)
                    EngRunTime_All.wHour = ENGRUNTIME_ALL_MAX;  // limit time
            }
        }
    }
}


/***********************************************************************
 *  FUNCTION:       SurvCheckRPMFlash
 *  DESCRIPTION:    Controls the RPM Flash light info & GPO_0 output,
 *                  switches on both, if current RPM value exceeds
 *                  setting in eeprom.
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        RPM Flash lamp is enabled, if eeprom value > 0 RPM!
 *********************************************************************** */
void SurvCheckRPMFlash ( void )
{
    static BOOL RPMFlash_On = FALSE;
    MESSAGE msg;

    // check: Activate RPM Flash?
    if (  ( RPM_Flash > 0        )      // feature enabled?
        &&( wRPM > RPM_Flash     )      // limit exceeded?
        &&( RPMFlash_On == FALSE ) )    // not already activated?
    {
        // ENABLE INFO LAMP
        RPMFlash_On = TRUE;                         // lock!
        LEDSetNewState( LED_INFO, LED_PERM_ON );    // permanent on

        // special MOTOBAU behaviour
        #if(BIKE_MOTOBAU==1)
        // kai want's to enable an additional external lamp/indicator
        GPO0 = 1;                       // enable output GPO_0 too
        #endif // BIKE_MOTOBAU
    }

    // check: Deactivate RPM Flash?
    if (  (  ( RPM_Flash == 0      )      // feature disabled (at runtime)?
           &&( RPMFlash_On == TRUE ) )    // still active?

        ||(  ( RPM_Flash > 0       )      // feature enabled?
           &&( wRPM < RPM_Flash    )      // back under limit?
           &&( RPMFlash_On == TRUE ) ) )  // still active?
    {
        // DISABLE INFO LAMP
        RPMFlash_On = FALSE;                        // unlock!
        LEDSetNewState( LED_INFO, LED_PERM_OFF );   // permannet off

        // special MOTOBAU behaviour
        #if(BIKE_MOTOBAU==1)
        // kai want's to enable an additional external lamp/indicator
        GPO0 = 0;                       // disable output GPO_0 too
        #endif // BIKE_MOTOBAU
    }
}


/***********************************************************************
 *  FUNCTION:       SurvCheckNextServiceKm
 *  DESCRIPTION:    Simple check wether ServiceIntervall (km) reached
 *                  (Eeprom setting)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SurvCheckNextServiceKm ( void )
{
    DIST_TYPE       VehicDist;      // current vehicle distance
    SURV_PARAM_ID_TYPE vstateprm;      // vehicle state parameter
    SURV_PARAM_STATE_TYPE vstatelvl;      // vehicle state level

    // default value: ok
    vstateprm = eSURVP_SERVICEKM;
    vstatelvl = eSURVST_OK;

    // check: current vehicle distance reached service intervall?
    VehicDist = MeasGetVehicDist( MR_KM );
    if (  (gNextServKm.km != gNextServKm_def.km )      // check enabled?
        &&(gNextServKm.km <= VehicDist.km       ) )    // exceeded distance?
        vstatelvl = eSURVST_WARNING;

    // update this warning
    SurvListSetParamState(vstateprm, vstatelvl);
}


/***********************************************************************
 *  FUNCTION:       SurvCheckNextServiceHours
 *  DESCRIPTION:    Check wether ServiceIntervall (hours) reached
 *                  (Eeprom setting)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SurvCheckNextServiceHours ( void )
{
    SURV_PARAM_ID_TYPE vstateprm;      // vehicle state parameter
    SURV_PARAM_STATE_TYPE vstatelvl;      // vehicle state level

    // default value: ok!
    vstateprm = eSURVP_SERVICEHOUR;
    vstatelvl = eSURVST_OK;

    // check: current vehicle runtime hours reached service intervall hours?
    if (  (EngRunTime_Srv.wHour != EngRunTime_Srv_def.wHour )      // check enabled?
        &&(EngRunTime_Srv.wHour <= EngRunTime_All.wHour     ) )    // exceeded hours?
        vstatelvl = eSURVST_WARNING;

    // update this warning
    SurvListSetParamState(vstateprm, vstatelvl);
}




/***********************************************************************
 *  FUNCTION:       SurvProcessAll
 *  DESCRIPTION:    vehicle surveillance update, will be called
 *                  cyclicly every 20 ms via TimerInterrupt
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        We use a short start delay SURV_STARTDELAY to prevent
 *                  unpredictable messages at ADC start behavior.
 *********************************************************************** */
ERRCODE SurvProcessAll(void)
{
    // check startup delay ---------------------------
    if ( wSecCounter  >  SURV_STARTDELAY )
    {
        SurvUpdateMeasData ();          // updates analog data
        SurvUpdateStatistics();         // updates min/max values
        SurvUpdateEngRunTime();         // updates engine runtime Serv/All

        SurvCheckAllDigitalWarnings();  // check all digital ports relevant for this bike
        SurvCheckAllAnalogWarnings();   // check all analog ports relevant for this bike
        SurvCheckNextServiceKm();       // check for km   Service Intervall
        SurvCheckNextServiceHours();    // check for hour Service Intervall
        SurvCheckAllDeviceWarnings();   // check all device internal warnings
        SurvCheckRPMFlash();            // Updates RPM Flash information

        //SurvSetLEDState();              // update Info/Warn/Err LED status
    }

    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       SurvScrollVehicleState
 *  DESCRIPTION:    scrolls currently displayed vehicle state string
 *                  to next upper/lower active vehicle state string
 *                  if possible and available
 *  PARAMETER:      VST_SCROLL_UP       ScrollUp   = TRUE
 *                  VST_SCROLL_DOWN     ScrollDown = FALSE
 *  RETURN:         -
 *  COMMENT:        Controls global surveillance status + string too!
 *********************************************************************** */
void SurvScrollVehicleState(BOOL fScrollDir)
{
    static  UINT8   ucIdx = 0;      // last used read list idx
            UINT8   ucCount;        // overall error count

    /* check: currently no parameter active? */
    ucCount = SurvListGetCount(eSURVST_ALL);

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
    SurvSetGlobalState( ucIdx );

}




/***********************************************************************
 *  FUNCTION:       SurvCheckAllAnalogWarnings
 *  DESCRIPTION:    check all analog values and update monitor states
 *  PARAMETER:      -
 *  RETURN:         nothing
 *  COMMENT:
 *********************************************************************** */
void SurvCheckAllAnalogWarnings(void)
{
    SURV_PARAM_ID_TYPE      vstateprm;      // vehicle state parameter
    SURV_PARAM_STATE_TYPE   vstatelvl;      // vehicle state level

    /* Check eSURVP_ENGINECOLD -------------------------------------
        default:                                               -> Ok
        if wRPM over warning limit AND (oil OR water to cold): -> Warning! */
    vstateprm = eSURVP_ENGINECOLD;
    vstatelvl = eSURVST_OK;
    if (  ( wRPM > ENG_COLD_RPM                         )
        &&(  (  (iTempOil > ANAIN_TEMP_SENSORDETECT )
              &&(iTempOil < OIL_TEMP_LOW            ) )
           ||(  (iTempWat > ANAIN_TEMP_SENSORDETECT )
              &&(iTempWat < WAT_TEMP_LOW            ) ) ) )
           vstatelvl = eSURVST_INFO;
    /* now add this resh state to our surveillance list */
    SurvListSetParamState(vstateprm, vstatelvl);

    /* Check eSURVP_GLACED -------------------------------------
        default:                 -> Ok
        if air between -2..+2°C  -> Warning! */
    vstateprm = eSURVP_GLACED;
    vstatelvl = eSURVST_OK;
    if (  ( iTempAir > AIR_TEMP_GLACED_LOW  )
        &&( iTempAir < AIR_TEMP_GLACED_HIGH ) )
           vstatelvl = eSURVST_INFO;
    /* now add this resh state to our surveillance list */
    SurvListSetParamState(vstateprm, vstatelvl);

    /* Check eSURVP_OILTEMP -------------------------------------
        default value:          -> Ok
        if oil temp very high:  -> Warning
        if oil temp too high:   -> Error */
    vstateprm = eSURVP_OILTEMP;
    vstatelvl = eSURVST_OK;
    if (iTempOil  > OIL_TEMP_WARN)
        vstatelvl = eSURVST_WARNING;
    if (iTempOil  > OIL_TEMP_ERR)
        vstatelvl = eSURVST_ERROR;
    /* now add this resh state to our surveillance list */
    SurvListSetParamState(vstateprm, vstatelvl);

    /* Check eSURVP_WATTEMP -------------------------------------
        default value:            -> Ok
        if water temp very high:  -> Warning
        if water temp too high:   -> Error */
    vstateprm = eSURVP_WATTEMP;
    vstatelvl = eSURVST_OK;
    if (iTempWat  > WAT_TEMP_WARN)
        vstatelvl = eSURVST_WARNING;
    if (iTempWat  > WAT_TEMP_ERR)
        vstatelvl = eSURVST_ERROR;
    /* now add this resh state to our surveillance list */
    SurvListSetParamState(vstateprm, vstatelvl);

    /* Check eSURVP_ALTERNATOR -------------------------------
        default value:                                      -> Ok
        if wRPM over warning limit and wAlternator too low: -> Warning! */
    if ( gBikeType == eBIKE_R100GS )
    {
        // special BIKE_R100GS checking
        vstateprm = eSURVP_ALTERNATOR;
        vstatelvl = SurvListGetParamState(vstateprm);     // first get current state

        /* Note: The warning will be cleared only, if the ALTW voltage will be
                 permanantly > threshold (or RPM == 0)! */
        if ( vstatelvl == eSURVST_OK )                  // no warning until now?
        {   if (  ( wRPM        > ALTERN_LOW_RPM)       // check: RPM over check limit?
                &&(wAlternator  < ALTERN_LOW    ) )     //        AND voltage below threshold?
            {   vstatelvl = eSURVST_WARNING;            // SET WARNING!
            }
        }
        else                                            // we already had a warning!
        {   if (  ( wRPM        == 0         )          // no error if motor stands still
                ||( wAlternator > ALTERN_LOW ) )        // check: voltage higher than threshold?
            {   vstatelvl = eSURVST_OK;                 // CLEAR WARNING!
            }
        }
        // check: if not using Sixo-Warnmode, setup LED directly here
        if ( gDeviceFlags3.flags.LedWarnMode == SURV_LWM_STD )
        {
            if ( vstatelvl != eSURVST_OK )
                 LEDSetNewState(LED_WARN, LED_PERM_ON );
            else LEDSetNewState(LED_WARN, LED_PERM_OFF);
        }
    }
    else
    {   /* standard behaviour: not used, always ok */
        vstateprm = eSURVP_ALTERNATOR;
        vstatelvl = eSURVST_OK;
    }

    /* now add this resh state to our surveillance list */
    SurvListSetParamState(vstateprm, vstatelvl);

    /* Check eSURVP_VOLTAGE_LOW -------------------------------
        default value:                                      -> Ok
        if wRPM over warning limit and wBattSupply too low: -> Warning! */
    vstateprm = eSURVP_VOLTAGE_LOW;
    vstatelvl = eSURVST_OK;
    if (  (wRPM > VOLTAGE_LOW_RPM)
        &&(wBattSupply < VOLTAGE_LOW   ) )
        vstatelvl = eSURVST_WARNING;
    /* now add this resh state to our surveillance list */
    SurvListSetParamState(vstateprm, vstatelvl);

    /* Check eSURVP_VOLTAGE_HIGH -------------------------------
        default value:           -> Ok
        if wBattSupply too high: -> Error! */
    vstateprm = eSURVP_VOLTAGE_HIGH;
    vstatelvl = eSURVST_OK;
    if (wBattSupply > VOLTAGE_HIGH )
        vstatelvl = eSURVST_ERROR;
    /* now add this resh state to our surveillance list */
    SurvListSetParamState(vstateprm, vstatelvl);

}



/***********************************************************************
 *  FUNCTION:       SurvCheckAllDeviceWarnings
 *  DESCRIPTION:    check all device internal states
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SurvCheckAllDeviceWarnings(void)
{
    /* ============================================================== */
    /* COMMON WARNINGS */

    /* User Info: do not drive with vehicle simulation on! */
    if (gDeviceFlags2.flags.VehicSimul == TRUE)
         SurvListSetParamState(eSURVP_SIMULATION, eSURVST_INFO);
    else SurvListSetParamState(eSURVP_SIMULATION, eSURVST_OK);

    /* User warning: Hardcopy via HBEAM-switch active? */
    if (gDeviceFlags2.flags.Hardcopy == TRUE)
         SurvListSetParamState(eSURVP_HARDCOPY, eSURVST_INFO);
    else SurvListSetParamState(eSURVP_HARDCOPY, eSURVST_OK);

    /* User Warning: Automatic RTC Clock change because of summer/winter time? */
    if (  ( gDeviceFlags2.flags.DaylightSaveAuto == TRUE )
        &&( fCESTChanged                         == TRUE ) )
    {
        fCESTChanged = FALSE;               // reset for next change detection
        if ( TimeDate_GetCEST() == TRUE )   // select msg
             SurvListSetParamState(eSURVP_DLS_SUMMER, eSURVST_INFO);
        else SurvListSetParamState(eSURVP_DLS_WINTER, eSURVST_INFO);
    }
    else
    {   SurvListSetParamState(eSURVP_DLS_SUMMER, eSURVST_OK);
        SurvListSetParamState(eSURVP_DLS_WINTER, eSURVST_OK);
    }

    /* User Warning: NVRAM delivered invalid vales -> RTC Batterie defect? */
    if (  fRTCDefect == TRUE )
         SurvListSetParamState(eSURVP_RTC_BATT, eSURVST_WARNING);
    else SurvListSetParamState(eSURVP_RTC_BATT, eSURVST_OK);

}



/***********************************************************************
 *  FUNCTION:       SurvCheckAllDigitalWarnings
 *  DESCRIPTION:    check all other digital in ports and sets
 *                  vehicle state
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        We use digital filtered values only
 *********************************************************************** */
void SurvCheckAllDigitalWarnings(void)
{
    /* ============================================================== */
    /* COMMON WARNINGS */
    // nothing tbd. yet

    /* ============================================================== */
    /* BIKE SPECIFIC WARNINGS */
    switch ( gBikeType )
    {
        /* SPECIAL HANDLING FOR BMW F650 ============================= */
        case eBIKE_F650:
        {
            /* --------------------------------------------- */
            /* F650 - Fuel 4 l Switch (low active) - Warning */
            if ( DF_Fuel_4l_F650 == FALSE )
                 SurvListSetParamState(eSURVP_FUEL4L, eSURVST_WARNING);
            else SurvListSetParamState(eSURVP_FUEL4L, eSURVST_OK);

            /* --------------------------------------------- */
            /* F650 - ABS inactive signal (low active) - Error */
            if ( DF_ABS_Warn_F650 == 0 )
                 SurvListSetParamState(eSURVP_ABS, eSURVST_ERROR);
            else SurvListSetParamState(eSURVP_ABS, eSURVST_OK);
            // check: if not using Sixo-Warnmode, setup Error-LED directly here
            if ( gDeviceFlags3.flags.LedWarnMode == SURV_LWM_STD )
            {   if ( DF_ABS_Warn_F650 == 0 )
                     LEDSetNewState(LED_WARN, LED_PERM_ON );
                else LEDSetNewState(LED_WARN, LED_PERM_OFF);
            }

            /* --------------------------------------------- */
            /* F650 - Watertemp High Switch (high active) - Error */
            if ( DF_Temp_Warn_F650 == 1)
                 SurvListSetParamState(eSURVP_WATTEMPSW, eSURVST_ERROR);
            else SurvListSetParamState(eSURVP_WATTEMPSW, eSURVST_OK);
            // check: if not using Sixo-Warnmode, setup LED directly here
            if ( gDeviceFlags3.flags.LedWarnMode == SURV_LWM_STD )
            {   if ( DF_ABS_Warn_F650 == 0 )
                     LEDSetNewState(LED_ERR, LED_PERM_ON );
                else LEDSetNewState(LED_ERR, LED_PERM_OFF);
            }
        } break;

        /* SPECIAL HANDLING FOR HONDA AFRICATWIN ============================= */
        case eBIKE_AFRICATWIN:
        {
            /* --------------------------------------------- */
            /* AfricaTwin - Fuel 8 l - Warning */
            if ( DF_Fuel_8l_AT == 0)                 // low active
                 SurvListSetParamState(eSURVP_FUEL8L, eSURVST_WARNING);
            else SurvListSetParamState(eSURVP_FUEL8L, eSURVST_OK);

            /* --------------------------------------------- */
            /* AfricaTwin - Oil switch defect? - Warning */
            if (  ( DF_OILSW == 1 )         // no oil signal
                &&( wRPM     == 0 ) )       // but engine stands still?
                 SurvListSetParamState(eSURVP_OILSWDEF, eSURVST_WARNING);
            else SurvListSetParamState(eSURVP_OILSWDEF, eSURVST_OK);
            // check: if not using Sixo-Warnmode, setup LED directly here
            if ( gDeviceFlags3.flags.LedWarnMode == SURV_LWM_STD )
            {   if ( DF_Fuel_4l_AT == 0 )      // low active
                     LEDSetNewState(LED_WARN, LED_PERM_ON );
                else LEDSetNewState(LED_WARN, LED_PERM_OFF );
            }

            /* --------------------------------------------- */
            /* AfricaTwin - Oil pressure ok? - Error (low active) */
            if (  ( DF_OILSW == 0             )      // low active
                &&( wRPM     >  OIL_PRESS_RPM ) )    // and engine running?
                 SurvListSetParamState(eSURVP_OILPRESS, eSURVST_ERROR);
            else SurvListSetParamState(eSURVP_OILPRESS, eSURVST_OK);
            // check: if not using Sixo-Warnmode, setup LED directly here
            if ( gDeviceFlags3.flags.LedWarnMode == SURV_LWM_STD )
            {   if ( DF_OILSW == 0 )      // low active
                     LEDSetNewState(LED_ERR, LED_PERM_ON );
                else LEDSetNewState(LED_ERR, LED_PERM_OFF );
            }

            /* --------------------------------------------- */
            /* AfricaTwin - Fuel 4 l - Error (low active)*/
            if ( DF_Fuel_4l_AT == 0 )
                 SurvListSetParamState(eSURVP_FUEL4L, eSURVST_ERROR);
            else SurvListSetParamState(eSURVP_FUEL4L, eSURVST_OK);

        } break;


        /* SPECIAL HANDLING FOR MuZ BAGHIRA ============================= */
        case eBIKE_BAGHIRA:
        {
            /* --------------------------------------------- */
            /* Baghira - Water-Temperatur - Error (high active) */
            if ( DF_Temp_Warn_BAGHIRA == 1)          // high active
                 SurvListSetParamState(eSURVP_WATTEMPSW, eSURVST_ERROR);
            else SurvListSetParamState(eSURVP_WATTEMPSW, eSURVST_OK);
            // check: if not using Sixo-Warnmode, setup LED directly here
            if ( gDeviceFlags3.flags.LedWarnMode == SURV_LWM_STD )
            {   if ( DF_Temp_Warn_BAGHIRA == 1)          // high active
                     LEDSetNewState(LED_ERR, LED_PERM_ON );
                else LEDSetNewState(LED_ERR, LED_PERM_OFF );
            }

        } break;

        /* DEFAULT HANDLING FOR ALL OTHER BIKES ============================= */
        default:
        {
            /* --------------------------------------------- */
            /* Standard - oil switch defect? - Warning */
            if (  ( DF_OILSW == 1 )      // no oil signal
                &&( wRPM     == 0 ) )    // but engine stands still?
                 SurvListSetParamState(eSURVP_OILSWDEF, eSURVST_WARNING);
            else SurvListSetParamState(eSURVP_OILSWDEF, eSURVST_OK);

            /* --------------------------------------------- */
            /* Standard - Oil pressure ok? */
            if (  ( DF_OILSW == 0             )      // low active
                &&( wRPM     >  OIL_PRESS_RPM ) )    // and engine running?
                 SurvListSetParamState(eSURVP_OILPRESS, eSURVST_ERROR);
            else SurvListSetParamState(eSURVP_OILPRESS, eSURVST_OK);
            // check: if not using Sixo-Warnmode, setup LED directly here
            if ( gDeviceFlags3.flags.LedWarnMode == SURV_LWM_STD )
            {   if ( DF_OILSW == 0 )      // low active
                     LEDSetNewState(LED_ERR, LED_PERM_ON );
                else LEDSetNewState(LED_ERR, LED_PERM_OFF );
            }

        } break;
    }
}




/***********************************************************************
 *  FUNCTION:       SurvResetAllParameters
 *  DESCRIPTION:    Resets any surveilled parameter by clearing complete
 *                  list.
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SurvResetAllParameters(void)
{
    // clear our current parameter list
    memset (&SurvParamList, 0x0, sizeof(SurvParamList) );

    // list is empty, count of currently detected info/warn/error is 0!
    SurvParamListCount = 0;
}



/***********************************************************************
 *  FUNCTION:       SurvListSetParamState
 *  DESCRIPTION:    Inserts/Remove given surveillance parameter.
 *                  - state = eSURVST_OK: delete parameter from list
 *                  - state > eSURVST_OK: add/update parameter at sorted position
 *  PARAMETER:      SURV_PARAM_ID_TYPE     parameter
 *                  SURV_PARAM_STATE_TYPE  state
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
void SurvListSetParamState( SURV_PARAM_ID_TYPE      GivenParam,
                            SURV_PARAM_STATE_TYPE   GivenState   )
{
    UINT8   bListIndex;     // index of parameter inside surveillance list

    // try to find inside our list (if not already in list: bListIndex = SURV_PARAM_MAX )
    bListIndex = SurvListGetIndex( GivenParam );

    // check: parameter never inserted and still ok -> irrelevant!
    if (  ( bListIndex == SURV_PARAM_MAX )
        &&( GivenState == eSURVST_OK     ) )
        return;

    // check: parameter exists but unchanged -> nothing to do!
    if (  ( bListIndex <  SURV_PARAM_MAX                  )
        &&( GivenState == SurvParamList[bListIndex].state ) )
        return;

    // check: parameter is set back to 'ok'?
    if( GivenState == eSURVST_OK )
    {
        /* ok, remove from list */
        SurvListRemoveParam ( bListIndex );
    }
    else
    {
        // check: already inside list?
        if (bListIndex < SURV_PARAM_MAX )
        {
            // ok, replace & resort parameter
            SurvListUpdateParam ( bListIndex, GivenState );
        }
        else
        {
            // ok, add new parameter sorted
            SurvListAddParam( GivenParam, GivenState );
        }
    }

    /* show our updated list */
    SurvListShow();

    /* update 'GlobalSurvState' = most top surv.-parameter (index 0)
       for MainDevice-Status field */
    SurvSetGlobalState(0);    // use most important (top) list entry

    /* update our led states */
    SurvSetLEDState();
}



/***********************************************************************
 *  FUNCTION:       SurvListGetParamState
 *  DESCRIPTION:    returns state of given parameter
 *  PARAMETER:      SURV_PARAM_ID_TYPE
 *  RETURN:         SURV_PARAM_STATE_TYPE     error/warning/level
 *  COMMENT:        If parameter not found (inactive),
 *                  then eSURVST_OK is returned
 *********************************************************************** */
SURV_PARAM_STATE_TYPE SurvListGetParamState(SURV_PARAM_ID_TYPE parameter)
{
    SURV_PARAM_STATE_TYPE   RValue;
    UINT8                   idx;

    /* try to find parameter */
    idx = SurvListGetIndex( parameter );

    /* check: not found? */
    if ( idx == SURV_PARAM_MAX  )
    {   RValue = eSURVST_OK;                // not found => ok!
    }
    else
    {   RValue = SurvParamList[idx].state;  // found: return state of parameter
    }

    // return state of parameter
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       SurvListGetIndex
 *  DESCRIPTION:    Tries to find the given surveillance parameter
 *                  in the current list.
 *  PARAMETER:      parameter       SurvParametere to be found/returned
 *  RETURN:         index           of parameter (if found)
 *                  SURV_PARAM_MAX  if not found
 *  COMMENT:        Just to find out presence and state of given parameter.
 *                  If parameter does NOT have an info/warning/error state
 *                  (means, it is 'ok' or 'inactive', the parameter will
 *                  NOT be found!
 *********************************************************************** */
UINT8 SurvListGetIndex( SURV_PARAM_ID_TYPE parameter )
{
    UINT8 i;

    // scan complete list - until found
    for ( i=0; i<SURV_PARAM_MAX; i++ )
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
 *  FUNCTION:       SurvListGetCount
 *  DESCRIPTION:    Returns number of current entries in parameter list
 *  PARAMETER:      u8GetMask       expects one of the SURVST_xxx bitmasks
 *  RETURN:         count           sum of active requested parameters
 *  COMMENT:        -
 *********************************************************************** */
UINT8 SurvListGetCount( UINT8 statemask )
{
    UINT8 i;
    UINT8 RValue = 0;

    // scan complete list - until found
    for ( i=0; i<SURV_PARAM_MAX; i++ )
    {
        // check: row is used AND state is one of our searched?
        if (  ( SurvParamList[i].param != eSURVP_NOENTRY )
            &&( SurvParamList[i].state &  statemask      ) )
            RValue++;
    }
    // return overall sum of all requested active parameters
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       SurvListRemoveParam
 *  DESCRIPTION:    Removes the given entry from by moving all following
 *                  entries one row up.
 *  PARAMETER:      bListIndex      Index to be removed from list
 *  RETURN:         -
 *  COMMENT:        Assumes the given parameter already to be found
 *                  via SurvListGetIndex()
 *********************************************************************** */
void SurvListRemoveParam ( UINT8 bListIndex )
{
    UINT8   i;

    ODS1(DBG_MEAS, DBG_INFO, "SurvListRemoveParam('%s')", szSurvParamDesc[SurvParamList[bListIndex].param]);

    // move all following entries 1 step up
    //  - start with given index
    //  - end one entry before(!) list end
    for ( i=bListIndex; i<(SURV_PARAM_MAX-1); i++ )
    {
        // move lower entry one row up
        SurvParamList[i].param = SurvParamList[i+1].param;
        SurvParamList[i].state = SurvParamList[i+1].state;
    }

    // clear last entry
    SurvParamList[SURV_PARAM_MAX-1].param = eSURVP_NOENTRY;
    SurvParamList[SURV_PARAM_MAX-1].state = eSURVST_OK;
}



/***********************************************************************
 *  FUNCTION:       SurvListUpdateParam
 *  DESCRIPTION:    Updates given parameter by first removing it
 *                  and the re-inserting (incl. correct sorting).
 *  PARAMETER:      bListIndex      entry to be updated
 *                  state           new state to be set
 *  RETURN:         -
 *  COMMENT:        Assumes the given parameter already to be found
 *                  via SurvListGetIndex()
 *********************************************************************** */
void SurvListUpdateParam ( UINT8 bListIndex, SURV_PARAM_STATE_TYPE state )
{
    SURV_PARAM_TYPE     TempParam;

    ODS1(DBG_MEAS, DBG_INFO, "SurvListUpdateParam('%s')", szSurvParamDesc[SurvParamList[bListIndex].param]);

    // check: parameter already has correct state?
    if ( SurvParamList[bListIndex].state == state )
        return;     // unchanged, just return

    // first save parameter - with new state!
    TempParam.param = SurvParamList[bListIndex].param;
    TempParam.state = state;

    // now clear old parameter from list
    SurvListRemoveParam ( bListIndex );

    // then re-insert changed parameter
    SurvListAddParam( TempParam.param, TempParam.state );
}



/***********************************************************************
 *  FUNCTION:       SurvListAddParam
 *  DESCRIPTION:    Adds a new parameter to the list of active parameters
 *  PARAMETER:      parameter       parameter to be added
 *                  state           state of that parameter
 *  RETURN:         -
 *  COMMENT:        Inserts parameter at most top position, but with
 *                  priority of info/warning/error.
 *                  If list is complete full, the least important entry
 *                  will be removed.
 *********************************************************************** */
void SurvListAddParam( SURV_PARAM_ID_TYPE parameter, SURV_PARAM_STATE_TYPE state )
{
    UINT8               i;
    UINT8               count;
    SURV_PARAM_TYPE     NewParam;   // just to insert new one
    SURV_PARAM_TYPE     LastParam;  // just to save old last one to re-insert

    ODS1(DBG_MEAS, DBG_INFO, "SurvListAddParam('%s')", szSurvParamDesc[parameter]);

    // check: given parameter is != ok?
    if ( state == eSURVST_OK )
        return;     // our list does only contain parameters > ok --> just return

    // generate a new temporary parameter to be inserted
    NewParam.param = parameter;
    NewParam.state = state;

    // save last entry, might be re-inserted if list full & new one less important
    LastParam = SurvParamList[SURV_PARAM_MAX-1];

    // completely move current list one row down
    for ( i=(SURV_PARAM_MAX-1); i>0; i-- )
        SurvParamList[i] = SurvParamList[i-1];

    // now try to insert new parameter -
    //  - beginning from upper side
    //  - stop one before end
    for ( i=0; i<(SURV_PARAM_MAX-1); i++ )
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
    if ( i == (SURV_PARAM_MAX-1) )
    {
        // re-insert previous last parameter
        SurvParamList[i] = LastParam;
        ODS(DBG_MEAS, DBG_WARNING, "List full, new entry discarded!" );
    }
}



/***********************************************************************
 *  FUNCTION:       SurvListShow
 *  DESCRIPTION:    just shows the current content of the list
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        just a debug helper: printout our new sorted list
 *********************************************************************** */
void SurvListShow( void )
{
    UINT8               i;
    UINT8               count;

    ODS(DBG_MEAS, DBG_INFO, "Surv-ParamList Content:" );
    count = SurvListGetCount(eSURVST_ALL);
    for ( i=0; i<SURV_PARAM_MAX; i++ )
    {
        ODS4(DBG_MEAS, DBG_INFO, "%u/%u: '%18s' : '%s'",
                i+1, count,
                szSurvParamDesc[SurvParamList[i].param],
                szErrorLevel   [SurvParamList[i].state] );
    }
}



/***********************************************************************
 *  FUNCTION:       SurvSetGlobalState
 *  DESCRIPTION:    Updates global state & string
 *  PARAMETER:      ucIdx       List entry to be shown (if possible)
 *  RETURN:         -
 *  COMMENT:        global surveillance status + string are used by
 *                  MainDevice and MonitorDevice
 *********************************************************************** */
void SurvSetGlobalState( UINT8 ucIdx )
{
    UINT8   ucCount;        // overall error count

    /* check: how many error parameters active? */
    ucCount = SurvListGetCount(eSURVST_ALL);

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
 *  FUNCTION:       SurvSetLEDState
 *  DESCRIPTION:    Updates global LED status for Info/Warn/Error,
 *                  based on our SurvList (instead of direct input switch)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Should cyclicly be called to to update LED status
 *                  at any time.
 *                  Toggle/Invert activation mode of each Info/Warn/Err LED
 *                  separately, BUT only if necessary!
 *********************************************************************** */
void SurvSetLEDState( void )
{
    MESSAGE msg;            // for LED event message

    /* main switch: Use special SIxO warning Mode? */
    if ( gDeviceFlags3.flags.LedWarnMode == SURV_LWM_SIXO )
    {
        /* INFO-LED --------------------------------------  */
        if (  ( SurvListGetCount(eSURVST_INFO) >  0     )
            &&( LEDGetState(LED_INFO) == FALSE ) )
        {   LEDSetNewState( LED_INFO, LED_PERM_ON );
        }
        if (  ( SurvListGetCount(eSURVST_INFO) == 0     )
            &&( LEDGetState(LED_INFO)     == TRUE ) )
        {   LEDSetNewState( LED_INFO, LED_PERM_OFF );
        }

        /* WARN-LED --------------------------------------  */
        if (  ( SurvListGetCount(eSURVST_WARNING) >  0     )
            &&( LEDGetState(LED_WARN)     == FALSE ) )
        {   LEDSetNewState( LED_WARN, LED_PERM_ON );
        }
        if (  ( SurvListGetCount(eSURVST_WARNING) == 0     )
            &&( LEDGetState(LED_WARN)     == TRUE ) )
        {   LEDSetNewState( LED_WARN, LED_PERM_OFF );
        }

        /* ERROR-LED --------------------------------------  */
        if (  ( SurvListGetCount(eSURVST_ERROR) >  0     )
            &&( LEDGetState(LED_ERR)     == FALSE ) )
        {   LEDSetNewState( LED_ERR, LED_PERM_ON );
        }
        if (  ( SurvListGetCount(eSURVST_ERROR) == 0     )
            &&( LEDGetState(LED_ERR)     == TRUE ) )
        {   LEDSetNewState( LED_ERR, LED_PERM_OFF );
        }
    }

    // special MOTOBAU behaviour
    #if(BIKE_MOTOBAU==1)
    // kai wants to enable an additional external lamp/indicator
    //  - enable output GPO_1 too for Warning/Error only
    GPO1 = SurvListGetCount(eSURVST_WARNING | eSURVST_ERR );
    #endif // BIKE_MOTOBAU
}
