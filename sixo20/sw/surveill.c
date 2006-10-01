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
 *      Arnold Neugebauer                   Ralf Krizsan
 *      Stoeckener Str. 115                 Knickstr. 10
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
#include "leddrv.h"
#include "led.h"
#include "surveill.h"
#include "timedate.h"
#include "digoutdr.h"
#include "sysparam.h"





// global data (will be initialized via EEPROM/NVRAM parameter init
RPM_TYPE    RPM_Flash;        // engine speed to enable flash lamp,   1 RPM/bit

RPM_TYPE    RPM_Max;          // max engine speed,                    1 RPM/bit
SPEED_TYPE  Speed_Max;        // max vehicel speed                    1 km/h/bit

UINT16      Volt_Min;         // minimal battery voltage,             0,01 V/bit
UINT16      Volt_Max;         // maximal battery voltage,             0,01 V/bit
INT16       TAir_Min;         // minimal air temperature,             1°C/bit, valid -40°C...215°C
INT16       TAir_Max;         // maximal air temperature,             1°C/bit, valid -40°C...215°C
INT16       TOil_Min;         // minimal oil temperature,             1°C/bit, valid -40°C...215°C
INT16       TOil_Max;         // maximal oil temperature,             1°C/bit, valid -40°C...215°C
INT16       TWat_Min;         // minimal water temperature,           1°C/bit, valid -40°C...215°C
INT16       TWat_Max;         // maximal water temperature,           1°C/bit, valid -40°C...215°C

TIME_TYPE_LD  EngRunTime_Srv;     // 4 bytes, engine runtime since last service
TIME_TYPE_LD  EngRunTime_All;     // 4 bytes, engine runtime overall


// external data
extern UINT16           wMilliSecCounter;       // high resolution short distance timer, ms,  max  65 sec
extern UINT16           wSecCounter;            // low  resolution long  distance timer, sec, max. 18 h
extern BIKE_TYPE        gBikeType;              // bike type
extern far DIST_TYPE    gNextServKm;            // to get/set original value
extern far DIST_TYPE    gNextServKm_def;        // to get/set original value
extern DIGFILTTYPE      DigInFilter[];          // digital filter table for all inputs


// local data
static UINT16      wRPM;                    // RPM signal           1 RPM/bit
static UINT16      wWheelSpeed;             // Wheel speed          1 km/h/bit
static UINT16      wBattSupply;             // battery supply       0,01 V/bit
static UINT16      wAlternator;             // Alternator voltage   0,01 V/bit
static INT16       iTempOil;                // oil temperature      1°C/bit, valid -40°C...215°C
static INT16       iTempWat;                // water temperature    1°C/bit, valid -40°C...215°C
static INT16       iTempAir;                // air temperature      1°C/bit, valid -40°C...215°C
static TIME_TYPE   ClockTime;               // copy of RTC data



/* monitoring / warning variables */
static VEHICLE_STATE_LEVEL  VehicleState[LAST_VEHICLE_STATE_PARM];          /* array with states of all vehicle parameters */
static STRING               szVehicStateDescs[LAST_VEHICLE_STATE_PARM];     /* array with names for all vehicle parameters */
static STRING               szVehicStateAllRight;                           /* standard string if no vehicle parameter in trouble */
static INT8                 CurrentNOKStates;                               /* number of states currently not ok */
static INT8                 CurStateTextIdx = 0;                            /* index of vehicle parameter currently shown */

char szVehicState[STATE_TEXT_LEN+1];                                 /* THE vehicle state string (directly used by 'MainDevice' and 'MonitorDevice') */


#ifdef DEBUG
// just for debug purpose
static const STRING szErrorLevel[] = {  "OK", "INFO", "WARNING", "ERROR" };
#endif



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

    /* initialize vehicle state to all right */
    SurvResetVehicleStates();

    /* assign state description text pointers */
    szVehicStateAllRight                            = (STRING) RESTXT_STATE_ALLRIGHT;
    szVehicStateDescs[VEHICLE_STATE_WATTEMP]        = (STRING) RESTXT_STATE_WATERTEMP;
    szVehicStateDescs[VEHICLE_STATE_OILTEMP]        = (STRING) RESTXT_STATE_OILTEMP;
    szVehicStateDescs[VEHICLE_STATE_ENGINECOLD]     = (STRING) RESTXT_STATE_ENGINE_COLD;
    szVehicStateDescs[VEHICLE_STATE_VOLTAGE_LOW]    = (STRING) RESTXT_STATE_VOLTAGE_LOW;
    szVehicStateDescs[VEHICLE_STATE_VOLTAGE_HIGH]   = (STRING) RESTXT_STATE_VOLTAGE_HIGH;
    szVehicStateDescs[VEHICLE_STATE_ALTERNATOR]     = (STRING) RESTXT_STATE_ALTERNATOR;
    szVehicStateDescs[VEHICLE_STATE_OILPRESS]       = (STRING) RESTXT_STATE_OILPRESS;
    szVehicStateDescs[VEHICLE_STATE_OILSWDEF]       = (STRING) RESTXT_STATE_OILSWDEF;
    szVehicStateDescs[VEHICLE_STATE_FUEL8L]         = (STRING) RESTXT_STATE_FUEL8L;
    szVehicStateDescs[VEHICLE_STATE_FUEL4L]         = (STRING) RESTXT_STATE_FUEL4L;
    szVehicStateDescs[VEHICLE_STATE_ABS]            = (STRING) RESTXT_STATE_ABS;    
    szVehicStateDescs[VEHICLE_STATE_WATTEMPSW]      = (STRING) RESTXT_STATE_WATERTEMP;    
    szVehicStateDescs[VEHICLE_STATE_GLACED]         = (STRING) RESTXT_STATE_GLACED;
    szVehicStateDescs[VEHICLE_STATE_SERVICEKM]      = (STRING) RESTXT_STATE_SERVICEKM;

    /* initial state is "OK" */
    strcpy((STRING) szVehicState, szVehicStateAllRight);

    ODS(DBG_MEAS, DBG_INFO, "SurvInit() Done!");

    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       SurvUpdateData
 *  DESCRIPTION:    controls local updates of analog data
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SurvUpdateData (void)
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
    if (  ( iTempAir != ANAIN_INVALID_S    )
        &&( iTempAir != -ANAIN_TEMP_OFFSET ) )
    {
        if (  ( TAir_Max < iTempAir         )
            ||( TAir_Max == ANAIN_INVALID_S ) )
                TAir_Max = iTempAir;

        if (  ( TAir_Min > iTempAir         )
            ||( TAir_Min == ANAIN_INVALID_S ) )
                TAir_Min = iTempAir;
    }

    // check oil temperature (if valid & connected)
    if (  ( iTempOil != ANAIN_INVALID_S    )
        &&( iTempOil != -ANAIN_TEMP_OFFSET ) )
    {
        if (  ( TOil_Max < iTempOil         )
            ||( TOil_Max == ANAIN_INVALID_S ) )
                TOil_Max = iTempOil;

        if (  ( TOil_Min > iTempOil         )
            ||( TOil_Min == ANAIN_INVALID_S ) )
                TOil_Min = iTempOil;
    }

    // check water temperature (if valid & connected)
    if (  ( iTempWat != ANAIN_INVALID_S )
        &&( iTempWat != -ANAIN_TEMP_OFFSET ) )
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
        EngRunTime_Srv.bSec++;                  // increment both second counters
        EngRunTime_All.bSec++;                  // increment both second counters

        // support Service Engine RunTime:
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

        // support Overall Engine RunTime:
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
        RPMFlash_On = TRUE;                             // lock!
        LED_MSG_TICKS(msg, LEDDRV_INFO, LED_ON, 1, 0);  // create message
        MsgQPostMsg(msg, MSGQ_PRIO_LOW);                // send message

        // special MOTOBAU behaviour
        if ( gBikeType == eBIKE_MOTOBAU )
        {
            // kai want's to enable an additional external lamp/indicator
            GPO0 = 1;                       // enable output GPO_0 too
        }
    }

    // check: Deactivate RPM Flash?
    if (  (  ( RPM_Flash == 0      )      // feature disabled (at runtime)?
           &&( RPMFlash_On == TRUE ) )    // still active?

        ||(  ( RPM_Flash > 0       )      // feature enabled?
           &&( wRPM < RPM_Flash    )      // back under limit?
           &&( RPMFlash_On == TRUE ) ) )  // still active?
    {
        // DISABLE INFO LAMP
        RPMFlash_On = FALSE;                            // unlock!
        LED_MSG_TICKS(msg, LEDDRV_INFO, LED_OFF, 0, 0); // create message
        MsgQPostMsg(msg, MSGQ_PRIO_LOW);                // send message

        // special MOTOBAU behaviour
        if ( gBikeType == eBIKE_MOTOBAU )
        {
            // kai want's to enable an additional external lamp/indicator
            GPO0 = 0;                       // disable output GPO_0 too
        }
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
    DIST_TYPE           VehicDist;      // current vehicle distance
    VEHICLE_STATE_PARM  vstateprm;      // vehicle state parameter
    VEHICLE_STATE_LEVEL vstatelvl;      // vehicle state level

    // default value
    vstateprm = VEHICLE_STATE_SERVICEKM;
    vstatelvl = VEHICLE_STATE_LEVEL_OK;

    // check: current vehicle distance reached service intervall?
    VehicDist = MeasGetVehicDist( MR_KM );
    if (  (gNextServKm.km != gNextServKm_def.km )      // check enabled?
        &&(gNextServKm.km <= VehicDist.km       ) )    // exceeded distance?
        vstatelvl = VEHICLE_STATE_LEVEL_WARNING;

    // update this warning
    SurvSetVehicleState(vstateprm, vstatelvl);
}




/***********************************************************************
 *  FUNCTION:       SurvCheckAllValues
 *  DESCRIPTION:    vehicle surveillance update, will be called
 *                  cyclicly every 20 ms via TimerInterrupt
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:
 *********************************************************************** */
ERRCODE SurvCheckAllValues(void)
{
    // check startup delay ---------------------------
    if ( wSecCounter  >  ANAIN_STARTDELAY )
    {
        SurvUpdateData();               // updates analog data
        SurvCheckAllDigitalWarnings();  // check all digital ports relevant for this bike
        SurvCheckAllAnalogWarnings();   // check all analog ports relevant for this bike
        SurvCheckNextServiceKm();       // check for Service Intervall
        SurvUpdateStatistics();         // updates min/max values
        SurvUpdateEngRunTime();         // updates engine runtime Serv/All
        SurvCheckRPMFlash();            // Updates RPM Flash information
    }

    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       SurvScrollVehicleState
 *  DESCRIPTION:    scrolls currently displayed vehicle state string
 *                  to next upper active vehicle state string
 *  PARAMETER:      VST_SCROLL_UP       ScrollUp
 *                  VST_SCROLL_DOWN     ScrollDown
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SurvScrollVehicleState(BOOL fScrollDir)
{
    INT8    i;
    INT8    ScrollDir = 0;

    /* set scroll dir */
    if ( fScrollDir == VST_SCROLL_UP )
         ScrollDir = 1;
    else ScrollDir = -1;

    //ODS2(DBG_MEAS, DBG_INFO, "SurvScrollVehicleState() Dir:%d #NOKs:%d", ScrollDir, CurrentNOKStates);

    /* update state text line */
    if (CurrentNOKStates == 0)
    {
        strcpy(szVehicState, szVehicStateAllRight);
    }
    else
    {   /* locate next trouble parameter in scroll direction */
        for (i = CurStateTextIdx + ScrollDir;
             i < ARRAY_SIZE(VehicleState) && i >= 0;
             i += ScrollDir)
        {
            if (VehicleState[i] != VEHICLE_STATE_LEVEL_OK)
            {
                strcpy(szVehicState, szVehicStateDescs[i]);
                CurStateTextIdx = i;
                break;
            }
        }
    }
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
    VEHICLE_STATE_PARM  vstateprm;      // vehicle state parameter
    VEHICLE_STATE_LEVEL vstatelvl;      // vehicle state level

    /* Check VEHICLE_STATE_ENGINECOLD -------------------------------------
        default:                                               -> Ok
        if wRPM over warning limit AND (oil OR water to cold): -> Warning! */
    vstateprm = VEHICLE_STATE_ENGINECOLD;
    vstatelvl = VEHICLE_STATE_LEVEL_OK;
    if (  ( wRPM > ENG_COLD_RPM                                                 )
        &&(  (((iTempOil != -ANAIN_TEMP_OFFSET)) && (iTempOil < OIL_TEMP_LOW) )
           ||(((iTempWat != -ANAIN_TEMP_OFFSET)) && (iTempWat < WAT_TEMP_LOW) ) ) )
           vstatelvl = VEHICLE_STATE_LEVEL_WARNING;
    SurvSetVehicleState(vstateprm, vstatelvl);

    /* Check VEHICLE_STATE_GLACED -------------------------------------
        default:                 -> Ok
        if air between -2..+2°C  -> Warning! */
    vstateprm = VEHICLE_STATE_GLACED;
    vstatelvl = VEHICLE_STATE_LEVEL_OK;
    if ( (iTempAir > AIR_TEMP_GLACED_LOW) && (iTempAir < AIR_TEMP_GLACED_HIGH))
           vstatelvl = VEHICLE_STATE_LEVEL_WARNING;
    SurvSetVehicleState(vstateprm, vstatelvl);

    /* Check VEHICLE_STATE_OILTEMP -------------------------------------
        default value:          -> Ok
        if oil temp very high:  -> Warning
        if oil temp too high:   -> Error */
    vstateprm = VEHICLE_STATE_OILTEMP;
    vstatelvl = VEHICLE_STATE_LEVEL_OK;
    if (iTempOil > OIL_TEMP_WARN)
        vstatelvl = VEHICLE_STATE_LEVEL_WARNING;
    if (iTempOil > OIL_TEMP_ERR)
        vstatelvl = VEHICLE_STATE_LEVEL_ERROR;
    SurvSetVehicleState(vstateprm, vstatelvl);

    /* Check VEHICLE_STATE_WATTEMP -------------------------------------
        default value:            -> Ok
        if water temp very high:  -> Warning
        if water temp too high:   -> Error */
    vstateprm = VEHICLE_STATE_WATTEMP;
    vstatelvl = VEHICLE_STATE_LEVEL_OK;
    if (iTempWat > WAT_TEMP_WARN)
        vstatelvl = VEHICLE_STATE_LEVEL_WARNING;
    if (iTempWat > WAT_TEMP_ERR)
        vstatelvl = VEHICLE_STATE_LEVEL_ERROR;
    SurvSetVehicleState(vstateprm, vstatelvl);

    /* Check VEHICLE_STATE_ALTERNATOR -------------------------------
        default value:                                      -> Ok
        if wRPM over warning limit and wAlternator too low: -> Warning! */
    if ( gBikeType == eBIKE_R100GS )
    {   // special BIKE_R100GS checking
        vstateprm = VEHICLE_STATE_ALTERNATOR;
        vstatelvl = SurvGetVehicleState(vstateprm);     // first get current state

        /* Note: The warning will be cleared only, if the ALTW voltage will be
                 permanantly > threshold (or RPM == 0)! */
        if ( vstatelvl == VEHICLE_STATE_LEVEL_OK )      // no warning until now?
        {   if (  ( wRPM        > ALTERN_LOW_RPM)       // check: RPM over check limit?
                &&(wAlternator  < ALTERN_LOW    ) )     //        AND voltage below threshold?
            {
                vstatelvl = VEHICLE_STATE_LEVEL_WARNING;    // SET WARNING!
            }
        }
        else                                            // we already had a warning!
        {   if (  ( wRPM        == 0         )          // no error if motor stands still
                ||( wAlternator > ALTERN_LOW     ) )    // check: voltage higher than threshold?
            {
                vstatelvl = VEHICLE_STATE_LEVEL_OK;     // CLEAR WARNING!
            }
        }
    }
    else
    {
        /* standard behaviour: not used, always ok */
        vstateprm = VEHICLE_STATE_ALTERNATOR;
        vstatelvl = VEHICLE_STATE_LEVEL_OK;
    }
    SurvSetVehicleState(vstateprm, vstatelvl);

    /* Check VEHICLE_STATE_VOLTAGE_LOW -------------------------------
        default value:                                      -> Ok
        if wRPM over warning limit and wBattSupply too low: -> Warning! */
    vstateprm = VEHICLE_STATE_VOLTAGE_LOW;
    vstatelvl = VEHICLE_STATE_LEVEL_OK;
    if (  (wRPM > VOLTAGE_LOW_RPM)
        &&(wBattSupply < VOLTAGE_LOW   ) )
        vstatelvl = VEHICLE_STATE_LEVEL_WARNING;
    SurvSetVehicleState(vstateprm, vstatelvl);

    /* Check VEHICLE_STATE_VOLTAGE_HIGH -------------------------------
        default value:           -> Ok
        if wBattSupply too high: -> Error! */
    vstateprm = VEHICLE_STATE_VOLTAGE_HIGH;
    vstatelvl = VEHICLE_STATE_LEVEL_OK;
    if (wBattSupply > VOLTAGE_HIGH )
        vstatelvl = VEHICLE_STATE_LEVEL_ERROR;
    SurvSetVehicleState(vstateprm, vstatelvl);

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
    switch ( gBikeType )
    {
        /* SPECIAL HANDLING FOR BMW F650 ============================= */
        case eBIKE_F650:
        {
            /* WARNING LED ------------------------------------------- */

            /* fuel 4 l error */
            if ( DF_Fuel_4l_F650 == FALSE )          // low active
                 SurvSetVehicleState(VEHICLE_STATE_FUEL4L, VEHICLE_STATE_LEVEL_WARNING);
            else SurvSetVehicleState(VEHICLE_STATE_FUEL4L, VEHICLE_STATE_LEVEL_OK);

            /* ABS inactive warning */
            if ( DF_ABS_Warn_F650 == 0 )              // low active
                 SurvSetVehicleState(VEHICLE_STATE_ABS, VEHICLE_STATE_LEVEL_WARNING);
            else SurvSetVehicleState(VEHICLE_STATE_ABS, VEHICLE_STATE_LEVEL_OK);

            /* ERROR LED ------------------------------------------- */
            if ( DF_Temp_Warn_F650 == 1)             // high active
                 SurvSetVehicleState(VEHICLE_STATE_WATTEMPSW, VEHICLE_STATE_LEVEL_ERROR);
            else SurvSetVehicleState(VEHICLE_STATE_WATTEMPSW, VEHICLE_STATE_LEVEL_OK);

        } break;




        /* SPECIAL HANDLING FOR HONDA AFRICATWIN ============================= */
        case eBIKE_AFRICATWIN:
        {
            /* WARNING LED ------------------------------------------- */

            /* fuel 8 l warning */
            if ( DF_Fuel_8l_AT == 0)                 // low active
                 SurvSetVehicleState(VEHICLE_STATE_FUEL8L, VEHICLE_STATE_LEVEL_WARNING);
            else SurvSetVehicleState(VEHICLE_STATE_FUEL8L, VEHICLE_STATE_LEVEL_OK);

            /* oil switch defect? */
            if (  ( DF_OILSW == 1 )         // no oil signal
                &&( wRPM     == 0 ) )       // but engine stands still?
                 SurvSetVehicleState(VEHICLE_STATE_OILSWDEF, VEHICLE_STATE_LEVEL_WARNING);
            else SurvSetVehicleState(VEHICLE_STATE_OILSWDEF, VEHICLE_STATE_LEVEL_OK);

            /* ERROR LED ------------------------------------------- */

            /* oil pressure ok? */
            if (  ( DF_OILSW == 0             )      // low active
                &&( wRPM     >  OIL_PRESS_RPM ) )    // and engine running?
                 SurvSetVehicleState(VEHICLE_STATE_OILPRESS, VEHICLE_STATE_LEVEL_ERROR);
            else SurvSetVehicleState(VEHICLE_STATE_OILPRESS, VEHICLE_STATE_LEVEL_OK);

            /* fuel 4 l error */
            if ( DF_Fuel_4l_AT == 0 )               // low active
                 SurvSetVehicleState(VEHICLE_STATE_FUEL4L, VEHICLE_STATE_LEVEL_ERROR);
            else SurvSetVehicleState(VEHICLE_STATE_FUEL4L, VEHICLE_STATE_LEVEL_OK);
        } break;


        /* SPECIAL HANDLING FOR MuZ BAGHIRA ============================= */
        case eBIKE_BAGHIRA:
        {
            /* ERROR LED ------------------------------------------- */

            /* Baghira's does not have oil pressure warning light,
             * but a temperature warning light */
            if ( DF_Temp_Warn_BAGHIRA == 1)          // high active
                 SurvSetVehicleState(VEHICLE_STATE_WATTEMPSW, VEHICLE_STATE_LEVEL_ERROR);
            else SurvSetVehicleState(VEHICLE_STATE_WATTEMPSW, VEHICLE_STATE_LEVEL_OK);
        } break;

        /* DEFAULT HANDLING FOR ALL OTHER BIKES ============================= */
        default:
        {
            /* WARNING LED ------------------------------------------- */

            /* oil switch defect? */
            if (  ( DF_OILSW == 1 )      // no oil signal
                &&( wRPM     == 0 ) )    // but engine stands still?
                 SurvSetVehicleState(VEHICLE_STATE_OILSWDEF, VEHICLE_STATE_LEVEL_WARNING);
            else SurvSetVehicleState(VEHICLE_STATE_OILSWDEF, VEHICLE_STATE_LEVEL_OK);

            /* ERROR LED ------------------------------------------- */

            /* oil pressure ok? */
            if (  ( DF_OILSW == 0             )      // low active
                &&( wRPM     >  OIL_PRESS_RPM ) )    // and engine running?
                 SurvSetVehicleState(VEHICLE_STATE_OILPRESS, VEHICLE_STATE_LEVEL_ERROR);
            else SurvSetVehicleState(VEHICLE_STATE_OILPRESS, VEHICLE_STATE_LEVEL_OK);
        } break;
    }
}




/***********************************************************************
 *  FUNCTION:       SurvResetVehicleStates
 *  DESCRIPTION:    resets any vehicle state to 'all right'
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SurvResetVehicleStates(void)
{
    int i;

    CurrentNOKStates = 0;                               // count of error is 0!
    for (i = 0; i < ARRAY_SIZE(VehicleState); i++)
    {
        SurvSetVehicleState(i, VEHICLE_STATE_LEVEL_OK); // reset each individual vehicle state
    }
}



/***********************************************************************
 *  FUNCTION:       SurvGetNOKStates
 *  DESCRIPTION:    returns number of vehicle states which are
 *                  'NOT OK' (NOK)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
UINT8 SurvGetNOKStates(void)
{
    return (CurrentNOKStates);
}



/***********************************************************************
 *  FUNCTION:       SurvSetVehicleState
 *  DESCRIPTION:    update vehicle state for error/warning text in
 *                  status line and error/warning LEDs
 *  PARAMETER:      VEHICLE_STATE_PARM, VEHICLE_STATE_LEVEL
 *  RETURN:         nothing
 *  COMMENT:        -
 *********************************************************************** */
void SurvSetVehicleState(VEHICLE_STATE_PARM parameter, VEHICLE_STATE_LEVEL level)
{
   int i;
   BOOL info = FALSE;
   BOOL warning = FALSE;
   BOOL error = FALSE;
   MESSAGE msg;

   /* check, wether this call changes anything */
   if (VehicleState[parameter] == level)
        return;         /* no change */
   else ODS2(DBG_MEAS, DBG_INFO, "SurvSetVehicleState() [%s]: %s)", szVehicStateDescs[parameter], szErrorLevel[level]);

   /* update current vehicle state */
   VehicleState[parameter] = level;


   /* new parameter state != ok? -> show it at top! */
   if (level != VEHICLE_STATE_LEVEL_OK)
   {
       /* show new error/warning parameter in state text */
       CurStateTextIdx = parameter;
       strcpy(szVehicState, szVehicStateDescs[parameter]);
   }
   else
   {
       /* parameter cleared -> find next active parameter, if any */

       // just a hack: try to scroll up/down to find next!
       SurvScrollVehicleState(VST_SCROLL_UP);
       SurvScrollVehicleState(VST_SCROLL_DOWN);

   }

   /* check which LEDs should be on */
   CurrentNOKStates = 0;
   for (i = 0; i < ARRAY_SIZE(VehicleState); i++)
   {
      // check each vehicle parameter for its state
      // sum # of NOKState
      switch (VehicleState[i])
      {
         // case VEHICLE_STATE_LEVEL_INFO:  not yet used, Info-LED controlled via different interface!
         case VEHICLE_STATE_LEVEL_WARNING:  CurrentNOKStates++; warning = TRUE; break;
         case VEHICLE_STATE_LEVEL_ERROR:    CurrentNOKStates++; error   = TRUE; break;
         default: break;
      }
   }

   // set default string
   if (CurrentNOKStates == 0)
       strcpy(szVehicState, szVehicStateAllRight);

    /* Generate LED messages */
    //LEDDRV_INFO not yet used, Info-LED controlled via different interface!
    //LED_MSG_MS(msg, LEDDRV_INFO, info, 1, 0);
    //MsgQPostMsg(msg, MSGQ_PRIO_LOW);

    LED_MSG_MS(msg, LEDDRV_WARN, warning, 1, 0);
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);

    LED_MSG_MS(msg, LEDDRV_ERR, error, 1, 0);
    MsgQPostMsg(msg, MSGQ_PRIO_LOW);

    // special MOTOBAU behaviour
    if ( gBikeType == eBIKE_MOTOBAU )
    {
        // kai wants to enable an additional external lamp/indicator
        GPO1 = error | warning;            // enable output GPO_1 too
    }
}



/***********************************************************************
 *  FUNCTION:       SurvGetVehicleState
 *  DESCRIPTION:    returns vehicle state of given parameter
 *  PARAMETER:      VEHICLE_STATE_PARM
 *  RETURN:         VEHICLE_STATE_LEVEL     error/warning/level
 *  COMMENT:        -
 *********************************************************************** */
VEHICLE_STATE_LEVEL SurvGetVehicleState(VEHICLE_STATE_PARM parameter)
{
   /* just return that state */
   return (VehicleState[parameter]);
}



