/***********************************************************************
 *
 *  $RCSfile$
 *
 *  $Revision$
 *
 *  $Date$
 *
 *  $Author$
 *
 *  --------------------------------------------------------------------
 *  Company:        N&K Development, all rights reserved
 *
 *                  Neugebauer & Krizsan Cooperation, Germany
 *                  contact:    arnold.neugebauer@web.de
 *                              ralf.krizsan@web.de
 *  --------------------------------------------------------------------
 *  Created:        26-03-2002 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         Measurement Module
 *  Purpose:        All high level measurement stuff for all sensors
 *  Comments:       -
 *
 ************************************************************************ */

/* includes */
#include <stdio.h>
#include <string.h>
#include "standard.h"
#include "timer.h"
#include "debug.h"
#include "measdrv.h"
#include "measure.h"
#include "sysparam.h"


/* external symbols */
extern  UINT16  wMilliSecCounter; // valid values: 0h .. ffffh


/* non public module symbols */
UINT16       wWheelSize  = DEF_WHEELSIZE;                    /* wheel size in mm */
CCF_TYPE     CCF         = DEF_CCF_NOM | DEF_CCF_DENOM;      /* RPM cylinder correcture factor */
DIST_TYPE    VehicDist   = 0x0;                              /* vehicle distance */
DIST_TYPE    TripA       = 0x0;                              /* TripCounter A */
DIST_TYPE    TripB       = 0x0;                              /* TripCounter B */
DIST_TYPE    FuelDist    = 0x0;                              /* fuel distance */
SPEED_TYPE   SpeedMax    = 0x0;                              /* max speed */
SPEED_TYPE   SpeedAvrM   = 0x0;                              /* average speed in motion only */
SPEED_TYPE   SpeedAvrP   = 0x0;                              /* average speed incl. pauses */




/***********************************************************************
 *  FUNCTION:       MeasGetWheelSpeed
 *  DESCRIPTION:    Interface to calculate the real wheel speed from
 *                  wheel size and wheel sensor impuls period
 *  PARAMETER:      MEASUNITS_TYPE eUnit requested return value units
 *  RETURN:         wheel speed in requested units
 *                  OR Error Code
 *  COMMENT:        Uses this calculation, based on 200탎ec/digit of
 *                  wheel sensors period time:
 *
 *                       s     WS (wheel size) [mm]
 *                  v = --- = --------------------------
 *                       t     WP (wheel period) [200탎]
 *
 *
 *                  v [km/h] = (WS * 18) / WP [km/h]
 *
 *                  v [m/s]  = (WS * 5)  / WP [m/s]
 *
 *********************************************************************** */
UINT16 MeasGetWheelSpeed(MEASUNITS_TYPE eUnit)
{
    UINT32  dwScratch;
    UINT16  wWheelPeriod;
    UINT16  RValue = 0;

    /* get averaged wheel period in 200 탎ec/digit */
    wWheelPeriod = MeasDrvGetWheelPeriod(FALSE);

    /* check, wether wheel stands still */
    if (wWheelPeriod == UINT16_MAX)
        return 0;

    /* get the current base value */
    dwScratch = ((UINT32)wWheelSize * 18) / wWheelPeriod;

    /* check: slow vehicle speed < ? */
    if (dwScratch < SLOW_WHEELSPEED)
        /* update wheel period with only the last period value */
        wWheelPeriod = MeasDrvGetWheelPeriod(TRUE);

    /* choice of return value units */
    switch (eUnit)
    {
        case MR_KM_PER_H:
        {
            dwScratch = ((UINT32)wWheelSize * 18) / wWheelPeriod;
        } break;
        case MR_HM_PER_H:
        {
            dwScratch = ((UINT32)wWheelSize * 180) / wWheelPeriod;
        } break;
        case MR_M_PER_S:
        {
            dwScratch = ((UINT32)wWheelSize * 5) / wWheelPeriod;
        } break;
        case MR_DM_PER_S:
        {
            dwScratch = ((UINT32)wWheelSize * 50) / wWheelPeriod;
        } break;
        case MR_CM_PER_S:
        {
            dwScratch = ((UINT32)wWheelSize * 500) / wWheelPeriod;
        } break;
        default:
        {
            ODS1(DBG_MEAS,DBG_ERROR,"MeasGetWheelSpeed(eUnit): Unknown eUnit = %u", eUnit);
            return UINT16_MAX;
        }
    }

    /* cast into correct space */
    if (dwScratch <= UINT16_MAX)
        RValue = (UINT16) dwScratch;
    else
    {
        ODS(DBG_MEAS,DBG_ERROR,"MeasGetWheelSpeed() Overflow! ");
        return UINT16_MAX;
    }


    /* get it ... */
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       MeasGetEngineSpeed
 *  DESCRIPTION:    Interface to calculate the real engine speed from
 *                  cylinder correctur factor and RPM sensor impuls period
 *  PARAMETER:      MEASUNITS_TYPE eUnit requested return value units
 *  RETURN:         engine speed in requested units
 *                  OR Error Code
 *  COMMENT:        Uses this calculation, based on 10 탎ec/digit of
 *                  RPM sensors period time and the 'Cylinder-Correctur-
 *                  Factor' (CCF):
 *
 *                         # of ignitions per round
 *                  CCF = --------------------------------------
 *                         # of cylinders sharing this ignition
 *
 *
 *                              const 6.000.000 * CCF (Cylinder-Factor)
 *                  n [1/Min] = ---------------------------------------
 *                                   RP (RPM period) [10탎]
 *
 *                               c * CCF     c  * CCF(Nom)
 *                            = --------- = ----------------
 *                                 RP        RP * CCF(Denom)
 *
 *                  n [1/Min] = (c * CCF-Nom) / (RP * CCF-Denom) [1/Min]
 *
 *********************************************************************** */
UINT16 MeasGetEngineSpeed(MEASUNITS_TYPE eUnit)
{
    const   UINT32  dwConst = 6000000L;         /* = 60 sec/Min / 10탎ec */
    UINT32  dwScratch;
    UINT16  wRPMPeriod;
    UINT16  RValue = 0;

    /* get current rpm period in 10 탎ec/digit */
    wRPMPeriod = MeasDrvGetRPMPeriod(TRUE);

    /* check, wether engine stands still */
    if (wRPMPeriod == UINT16_MAX)
        return 0;

    /* get the RPM base value (units: rounds per minute) */
    dwScratch = (dwConst * CCF.nibble.nom) / ((UINT32)wRPMPeriod * (UINT32)CCF.nibble.denom);

    /* check: Low engine speed? */
    if (dwScratch < SLOW_ENGSPEED)
    {
        /* get a more averaged filter value */
        wRPMPeriod = MeasDrvGetRPMPeriod(FALSE);
        /* do calculations again */
        dwScratch = (dwConst * CCF.nibble.nom) / ((UINT32)wRPMPeriod * (UINT32)CCF.nibble.denom);
    }


    /* choice of return value units */
    switch (eUnit)
    {
        case MR_RPS:    /* rounds per second */
        {
            dwScratch = dwScratch / 60;
        } break;
        case MR_RPM:    /* rounds per minute */
        {
                        /* nothing to do, value already computed! */
        } break;
        case MR_RPM_R10:/* rounds per minute rounded to ten's */
        {
            dwScratch = (dwScratch / 10) * 10;
        } break;
        default:
        {
            ODS1(DBG_MEAS,DBG_ERROR,"MeasGetEngineSpeed(): Unknown eUnit = %u", eUnit);
            return UINT16_MAX;
        }
    }

    /* cast into correct space */
    if (dwScratch <= UINT16_MAX)
        RValue = (UINT16) dwScratch;
    else
    {
        ODS(DBG_MEAS,DBG_ERROR,"MeasGetEngineSpeed() Overflow! ");
        return UINT16_MAX;
    }

    /* get it ... */
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       MeasCyclicSaveValues
 *  DESCRIPTION:    Cyclicly called routine to asure all necessary
 *                  measurement data to be saved in NVRAM
 *  PARAMETER:      -
 *  RETURN:         error code
 *  COMMENT:        - Does only write into nvram if any changes made in value
 *                  - Reduces sytem load by only saving one value every 200 ms.
 *                  - init values are valid after reading initial reading
 *                    from nvram.
 *********************************************************************** */
ERRCODE MeasCyclicSaveValues(void)
{
    static  BOOL            fInit       = FALSE;            /* to check for initialized static vars */
    static  PARAM_ID_TYPE   PID         = PID_NVRAM_START;
    static  UINT16          wLastCall   = 0;
            UINT16          wThisCall   = 0;
            ERRCODE         RValue      = ERR_OK;
    static  DIST_TYPE       VehicDistCopy;
    static  DIST_TYPE       TripACopy;
    static  DIST_TYPE       TripBCopy;
    static  DIST_TYPE       FuelDistCopy;
    static  SPEED_TYPE      SpeedMaxCopy;
    static  SPEED_TYPE      SpeedAvrMCopy;
    static  SPEED_TYPE      SpeedAvrPCopy;

    /* first init local copies at first call */
    if (fInit == FALSE)
    {
        VehicDistCopy   = VehicDist;
        TripACopy       = TripA;
        TripBCopy       = TripB;
        SpeedMaxCopy    = SpeedMax;
        SpeedAvrMCopy   = SpeedAvrM;
        SpeedAvrPCopy   = SpeedAvrP;
        FuelDistCopy    = FuelDist;
        fInit           = TRUE;
    }

    /* check: time to save one system parameter? */
    TimerGetSys_msec(wThisCall);
    if (wThisCall - wLastCall > NVRAM_CYCLE)
    {
        wLastCall = wThisCall;          /* update time stamp */
        switch (PID)                    /* choice of system parameter to be saved */
        {
            case PID_VEHIC_KM:
                if (VehicDistCopy.dkm != VehicDist.dkm)
                {
                    RValue = ParWriteSysParam( PID, &VehicDist);
                    VehicDistCopy = VehicDist;
                    ODS2(DBG_MEAS,DBG_INFO,"VehicDist: %lu,%.2lu km", VehicDist.dkm/100L, VehicDist.dkm-(VehicDist.dkm/100L)*100);
                } break;
            case PID_TRIPA_KM:
                if (TripACopy.dkm != TripA.dkm)
                {
                    RValue = ParWriteSysParam( PID,  &TripA);
                    TripACopy = TripA;
                    ODS2(DBG_MEAS,DBG_INFO,"TripA: %lu,%.2lu km", TripA.dkm/100L, TripA.dkm-(TripA.dkm/100L)*100);
                } break;
            case PID_TRIPB_KM:
                if (TripBCopy.dkm != TripB.dkm)
                {
                    RValue = ParWriteSysParam( PID,  &TripB);
                    TripBCopy = TripB;
                    ODS2(DBG_MEAS,DBG_INFO,"TripB: %lu,%.2lu km", TripB.dkm/100L, TripB.dkm-(TripB.dkm/100L)*100);
                } break;
            case PID_FUEL_KM:
                if (FuelDistCopy.dkm != FuelDist.dkm)
                {
                    RValue = ParWriteSysParam( PID, &FuelDist);
                    FuelDistCopy = FuelDist;
                    ODS2(DBG_MEAS,DBG_INFO,"FuelDist: %lu,%.2lu km", FuelDist.dkm/100L, FuelDist.dkm-(FuelDist.dkm/100L)*100);
                } break;
            case PID_SPEED_MAX:
                if (SpeedMaxCopy != SpeedMax)
                {
                    RValue = ParWriteSysParam( PID, &SpeedMax);
                    SpeedMaxCopy = SpeedMax;
                    ODS2(DBG_MEAS,DBG_INFO,"SpeedMax: %u,%.2u km/h", SpeedMax/100, SpeedMax-(SpeedMax/100)*100);
                } break;
            case PID_SPEED_AVR_M:
                if (SpeedAvrMCopy != SpeedAvrM)
                {
                    RValue = ParWriteSysParam( PID, &SpeedAvrM);
                    SpeedAvrMCopy = SpeedAvrM;
                    ODS2(DBG_MEAS,DBG_INFO,"SpeedAvrM: %u,%.2u km/h", SpeedAvrM/100, SpeedAvrM-(SpeedAvrM/100)*100);
                } break;
            case PID_SPEED_AVR_P:
                if (SpeedAvrPCopy != SpeedAvrP)
                {
                    RValue = ParWriteSysParam( PID, &SpeedAvrP);
                    SpeedAvrPCopy = SpeedAvrP;
                    ODS2(DBG_MEAS,DBG_INFO,"SpeedAvrP: %u,%.2u km/h", SpeedAvrP/100, SpeedAvrP-(SpeedAvrP/100)*100);
                } break;
            default:
                ODS1(DBG_MEAS,DBG_ERROR,"Unknown PID: %u", PID);
                break;
        }

        /* next system parameter for next call */
        PID++;                                      /* inkr PID */
        if (PID >= PID_NVRAM_END)
           PID = PID_NVRAM_START;                   /* start at the begining again */
    }
    return RValue;
}






/***********************************************************************
 *  FUNCTION:       MeasGetFuelDist
 *  DESCRIPTION:    interface to get the current fuel distance counter value
 *  PARAMETER:      eUnits      type of return value
 *  RETURN:         formated distance value
 *  COMMENT:        -
 *********************************************************************** */
UINT16 MeasGetFuelDist( MEASUNITS_TYPE eUnits )
{
    DIST_TYPE ResultDist;

    /* get formated value */
    GetFormatedDist( &FuelDist, &ResultDist, eUnits);
    switch (eUnits)
    {
        case MR_KM_ONLY:    return ResultDist.km_o;  break;
        case MR_HM_ONLY:    return ResultDist.hkm_o; break;
        case MR_DKM_ONLY:   return ResultDist.dkm_o; break;
        default: ODS1(DBG_MEAS,DBG_ERROR,"Illegal measurement type [%u]!", eUnits); break;
    }
    return ERR_OK;
}

/***********************************************************************
 *  FUNCTION:       MeasGetRawFuelDist
 *  DESCRIPTION:    interface to get unformated current fuel distance counter value
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MeasGetRawFuelDist(DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE
    *fpDist = FuelDist;
    INT_GLOB_ENABLE
    return ERR_OK;
}

/***********************************************************************
 *  FUNCTION:       MeasSetFuelDist
 *  DESCRIPTION:    Set the current fuel distance counter value
 *  PARAMETER:      far ptr     to src buffer
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MeasSetFuelDist(DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE
    FuelDist = *fpDist;
    INT_GLOB_ENABLE
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       MeasGetVehicDist
 *  DESCRIPTION:    interface to get the current vehicle distance counter value
 *  PARAMETER:      eUnits      type of return value
 *  RETURN:         formated distance value
 *  COMMENT:        -
 *********************************************************************** */
DIST_TYPE MeasGetVehicDist( MEASUNITS_TYPE eUnits )
{
    DIST_TYPE ResultDist;

    /* get formated value */
    GetFormatedDist( &VehicDist, &ResultDist, eUnits);
    return ResultDist;
}

/***********************************************************************
 *  FUNCTION:       MeasGetRawVehicDist
 *  DESCRIPTION:    interface to get unformated current vehicle distance counter value
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MeasGetRawVehicDist(DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE
    *fpDist = VehicDist;
    INT_GLOB_ENABLE
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       MeasSetVehicDist
 *  DESCRIPTION:    Set the current vehicle distance counter value
 *  PARAMETER:      far ptr     to src buffer
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MeasSetVehicDist(DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE
    VehicDist = *fpDist;
    INT_GLOB_ENABLE
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       MeasGetTripA
 *  DESCRIPTION:    interface to get the current TRIP A counter value
 *  PARAMETER:      eUnits      type of return value
 *  RETURN:         formated distance value
 *  COMMENT:        -
 *********************************************************************** */
UINT16 MeasGetTripA( MEASUNITS_TYPE eUnits )
{
    DIST_TYPE ResultDist;

    /* get formated value */
    GetFormatedDist( &TripA, &ResultDist, eUnits);
    switch (eUnits)
    {
        case MR_KM_ONLY:    return ResultDist.km_o;  break;
        case MR_HM_ONLY:    return ResultDist.hkm_o; break;
        case MR_DKM_ONLY:   return ResultDist.dkm_o; break;
        default: ODS1(DBG_MEAS,DBG_ERROR,"Illegal measurement type [%u]!", eUnits); break;
    }
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       MeasGetRawTripA
 *  DESCRIPTION:    interface to get unformated current TripA counter value
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MeasGetRawTripA(DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE
    *fpDist = TripA;
    INT_GLOB_ENABLE
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       MeasSetTripA
 *  DESCRIPTION:    Set the current fuel distance counter value
 *  PARAMETER:      far ptr     to src buffer
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MeasSetTripA(DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE
    TripA = *fpDist;
    INT_GLOB_ENABLE
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       MeasGetTripB
 *  DESCRIPTION:    interface to get the current TRIP B counter value
 *  PARAMETER:      eUnits      type of return value
 *  RETURN:         formated distance value
 *  COMMENT:        -
 *********************************************************************** */
UINT16 MeasGetTripB( MEASUNITS_TYPE eUnits )
{
    DIST_TYPE ResultDist;

    /* get formated value */
    GetFormatedDist( &TripB, &ResultDist, eUnits);
    switch (eUnits)
    {
        case MR_KM_ONLY:    return ResultDist.km_o;  break;
        case MR_HM_ONLY:    return ResultDist.hkm_o; break;
        case MR_DKM_ONLY:   return ResultDist.dkm_o; break;
        default: ODS1(DBG_MEAS,DBG_ERROR,"Illegal measurement type [%u]!", eUnits); break;
    }
    return ERR_OK;
}

/***********************************************************************
 *  FUNCTION:       MeasGetRawTripB
 *  DESCRIPTION:    interface to get unformated current TripB counter value
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MeasGetRawTripB(DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE
    *fpDist = TripB;
    INT_GLOB_ENABLE
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       MeasSetTripB
 *  DESCRIPTION:    Set the current fuel distance counter value
 *  PARAMETER:      far ptr     to src buffer
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MeasSetTripB(DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE
    TripB = *fpDist;
    INT_GLOB_ENABLE
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       GetFormatedDist
 *  DESCRIPTION:    Re-Formats the content of the distance variable
 *  PARAMETER:      addr of     source distance value
 *                  addr of     formated target distance value
 *                  format type
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE GetFormatedDist( DIST_TYPE far * fpDistSrc, DIST_TYPE far * fpDistTrgt, MEASUNITS_TYPE RUnits)
{
    /* GET value ------------------ */
    UINT32 dwCopy;
    UINT16 wScratch;

    /* get current value */
    INT_GLOB_DISABLE
    dwCopy = fpDistSrc->dkm;
    INT_GLOB_ENABLE

    /* format return value */
    switch (RUnits)
    {
        case MR_KM:         fpDistTrgt->km    = dwCopy / 100L; break;              /* 1 kilometer per digit */
        case MR_HM:         fpDistTrgt->hm    = dwCopy / 10L;  break;              /* 1 hectometer (=0,1 km) per digit */
        case MR_DKM:        fpDistTrgt->dkm   = dwCopy;       break;              /* 1 dekameter (=0,01 km) per digit */
        case MR_KM_ONLY:    fpDistTrgt->km_o  = (UINT16) (dwCopy/100L); break;    /* 1 kilometer per digit */        /* 1 kilometer per digit LIMITED to 65.536 km */
        case MR_HM_ONLY:    wScratch      = (UINT16) (dwCopy/10L);                     /* 1 hectometer (=0,1 km) per digit WITHOUT leading km's */
                            fpDistTrgt->hkm_o = wScratch - ((wScratch/10)*10); break;
        case MR_DKM_ONLY:   wScratch      = (UINT16) dwCopy;                          /* 1 dekameter (=0,01 km) per digit WITHOUT leading km's */
                            fpDistTrgt->dkm_o = wScratch - ((wScratch/100)*100); break;
        default: ODS1(DBG_MEAS,DBG_ERROR,"Unknown measurement type [%u]!", RUnits); break;
    }
    return ERR_OK;
}





 