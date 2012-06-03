/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-26 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         Measurement Module
 *  Purpose:        All high level measurement stuff for all sensors
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
 * Revision 3.5  2012/06/03 17:45:18  tuberkel
 * Updated API-Function-Name according to Modul-Name
 *
 * Revision 3.4  2012/05/27 17:52:40  tuberkel
 * Corrections for renamed Eeprom/Nvram Variables
 *
 * Revision 3.3  2012/05/27 16:01:40  tuberkel
 * All Eeprom/Nvram Variables renamed
 *
 * Revision 3.2  2012/02/26 12:24:55  tuberkel
 * - moved all Eeprom Vlaues physically into 'sysparam' module
 *
 * Revision 3.1  2011/05/29 12:43:19  tuberkel
 * BugFix gwWheelImpulse
 * - Typ korrgiert
 * - jetzt auch im Eeprom gesichert
 *
 * Revision 3.0  2010/11/07 14:00:50  tuberkel
 * V30 Preparations - no changes
 *
 * Revision 2.2  2010/11/07 11:57:37  tuberkel
 * V30 Preparations:
 * - New: gwWheelImpulse
 * - Devider/Prescaler for WheelSensor Interrupt
 * - if used, has to be supported by HW-changes too
 *
 * Revision 2.1  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.7  2006/03/18 08:50:43  Ralf
 * - TripCounter improved to 4 different counters
 *
 * Revision 1.6  2006/02/18 14:53:39  Ralf
 * - system parameter handling completely reviewed (--> table!)
 * - tripcounter C+D added
 *
 *
 ************************************************************************ */


/* includes */
#include <stdio.h>
#include <string.h>
#include "sfr62p.h"
#include "m16c.h"
#include "standard.h"
#include "timer.h"
#include "debug.h"
#include "measdrv.h"
#include "measure.h"
#include "sysparam.h"




/* external symbols (taken from eeprom/nvram) */
extern  UINT16      wMilliSecCounter;   /* valid values: 0h .. ffffh */
extern  UINT16      EE_WheelSize;       /* wheel size in mm */
extern  UINT16      EE_WheelImpPRev;   /* wheel impulses per revolution */
extern  CCF_TYPE    EE_CCF;             /* RPM cylinder correcture factor */
extern  DIST_TYPE   NV_VehicDist;       /* vehicle distance */
extern  DIST_TYPE   NV_TripCnt_A;       /* TripCounter A */
extern  DIST_TYPE   NV_TripCnt_B;       /* TripCounter B */
extern  DIST_TYPE   NV_TripCom_A;       /* TripCommon A */
extern  DIST_TYPE   NV_TripCom_B;       /* TripCommon B */
extern  DIST_TYPE   NV_FuelDist  ;      /* fuel distance */
extern  SPEED_TYPE  NV_Speed_AvrM;      /* average speed in motion only */
extern  SPEED_TYPE  NV_Speed_AvrP;      /* average speed incl. pauses */







/***********************************************************************
 *  FUNCTION:       Meas_GetSpeed_Wheel
 *  DESCRIPTION:    Interface to calculate the real wheel speed from
 *                  wheel size and wheel sensor impuls period
 *  PARAMETER:      MEASUNITS_TYPE eUnit requested return value units
 *  RETURN:         wheel speed in requested units
 *                  OR Error Code
 *  COMMENT:        Uses this calculation, based on 200µsec/digit of
 *                  wheel sensors period time:
 *
 *                       s     WS (wheel size) [mm]
 *                  v = --- = --------------------------
 *                       t     WP (wheel period) [200µs]
 *
 *
 *                  v [km/h] = (WS * 18) / WP [km/h]
 *
 *                  v [m/s]  = (WS * 5)  / WP [m/s]
 *
 *********************************************************************** */
UINT16 Meas_GetSpeed_Wheel(MEASUNITS_TYPE eUnit)
{
    UINT32  dwScratch;
    UINT16  wWheelPeriod;
    UINT16  RValue = 0;

    /* get averaged wheel period in 200 µsec/digit */
    wWheelPeriod = MeasDrv_GetWheelPeriod(FALSE);

    /* check, wether wheel stands still */
    if (wWheelPeriod == UINT16_MAX)
        return 0;

    /* get the current base value */
    dwScratch = ((UINT32)EE_WheelSize * 18) / wWheelPeriod;

    /* check: slow vehicle speed < ? */
    if (dwScratch < SLOW_WHEELSPEED)
        /* update wheel period with only the last period value */
        wWheelPeriod = MeasDrv_GetWheelPeriod(TRUE);

    /* choice of return value units */
    switch (eUnit)
    {
        case MR_KM_PER_H: dwScratch = ((UINT32)EE_WheelSize *  18) / wWheelPeriod; break;
        case MR_HM_PER_H: dwScratch = ((UINT32)EE_WheelSize * 180) / wWheelPeriod; break;
        case MR_M_PER_S:  dwScratch = ((UINT32)EE_WheelSize *   5) / wWheelPeriod; break;
        case MR_DM_PER_S: dwScratch = ((UINT32)EE_WheelSize *  50) / wWheelPeriod; break;
        case MR_CM_PER_S: dwScratch = ((UINT32)EE_WheelSize * 500) / wWheelPeriod; break;
        default:
        {   ODS1(DBG_MEAS,DBG_ERROR,"Meas_GetSpeed_Wheel(eUnit): Unknown eUnit = %u", eUnit);
            return UINT16_MAX;
        }
    }

    /* cast into correct space */
    if ( dwScratch <= UINT16_MAX )
    {
        if (dwScratch > 0 )
             RValue = (UINT16) dwScratch + 1;   /* TÜV: always assure display to be 1 km/h too fast! */
        else RValue = (UINT16) dwScratch + 0;   /* Zero: keep 0 km/h */
    }
    else
    {
        ODS(DBG_MEAS,DBG_ERROR,"Meas_GetSpeed_Wheel() Overflow! ");
        return UINT16_MAX;
    }


    /* get it ... */
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       Meas_GetSpeed_Engine
 *  DESCRIPTION:    Interface to calculate the real engine speed from
 *                  cylinder correctur factor and RPM sensor impuls period
 *  PARAMETER:      MEASUNITS_TYPE eUnit requested return value units
 *  RETURN:         engine speed in requested units
 *                  OR Error Code
 *  COMMENT:        Uses this calculation, based on 10 µsec/digit of
 *                  RPM sensors period time and the 'Cylinder-Correctur-
 *                  Factor' (EE_CCF):
 *
 *                         # of ignitions per round
 *                  EE_CCF = --------------------------------------
 *                         # of cylinders sharing this ignition
 *
 *
 *                              const 6.000.000 * EE_CCF (Cylinder-Factor)
 *                  n [1/Min] = ---------------------------------------
 *                                   RP (RPM period) [10µs]
 *
 *                               c * EE_CCF     c  * EE_CCF(Nom)
 *                            = --------- = ----------------
 *                                 RP        RP * EE_CCF(Denom)
 *
 *                  n [1/Min] = (c * EE_CCF-Nom) / (RP * EE_CCF-Denom) [1/Min]
 *
 *********************************************************************** */
UINT16 Meas_GetSpeed_Engine(MEASUNITS_TYPE eUnit)
{
    const   UINT32  dwConst = 6000000L;         /* = 60 sec/Min / 10µsec */
    UINT32  dwScratch;
    UINT16  wRPMPeriod;
    UINT16  RValue = 0;

    /* get current rpm period in 10 µsec/digit */
    wRPMPeriod = MeasDrv_GetRPMPeriod(TRUE);

    /* check, wether engine stands still */
    if (wRPMPeriod == UINT16_MAX)
        return 0;

    /* get the RPM base value (units: rounds per minute) */
    dwScratch = (dwConst * EE_CCF.nibble.nom) / ((UINT32)wRPMPeriod * (UINT32)EE_CCF.nibble.denom);

    /* check: Low engine speed, but higher than engine start sequence? */
    if (  (dwScratch < SLOW_ENGSPEED_H)
        &&(dwScratch > SLOW_ENGSPEED_L) )
    {
        /* get a more averaged filter value */
        wRPMPeriod = MeasDrv_GetRPMPeriod(FALSE);
        /* do calculations again */
        dwScratch = (dwConst * EE_CCF.nibble.nom) / ((UINT32)wRPMPeriod * (UINT32)EE_CCF.nibble.denom);
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
            ODS1(DBG_MEAS,DBG_ERROR,"Meas_GetSpeed_Engine(): Unknown eUnit = %u", eUnit);
            return UINT16_MAX;
        }
    }

    /* cast into correct space */
    if (dwScratch <= UINT16_MAX)
        RValue = (UINT16) dwScratch;
    else
    {
        ODS(DBG_MEAS,DBG_ERROR,"Meas_GetSpeed_Engine() Overflow! ");
        return UINT16_MAX;
    }

    /* get it ... */
    return RValue;
}










/***********************************************************************
 *  FUNCTION:       Meas_GetDist_Fuel
 *  DESCRIPTION:    interface to get the current fuel distance counter value
 *  PARAMETER:      eUnits      type of return value
 *  RETURN:         formated distance value
 *  COMMENT:        -
 *********************************************************************** */
UINT16 Meas_GetDist_Fuel( MEASUNITS_TYPE eUnits )
{
    DIST_TYPE ResultDist;

    /* get formated value */
    Meas_ConvertDist( &NV_FuelDist, &ResultDist, eUnits);
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
 *  FUNCTION:       Meas_GetDist_FuelRaw
 *  DESCRIPTION:    interface to get unformated current fuel distance counter value
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE Meas_GetDist_FuelRaw(DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE;
    *fpDist = NV_FuelDist;
    INT_GLOB_ENABLE;
    return ERR_OK;
}

/***********************************************************************
 *  FUNCTION:       Meas_SetDist_Fuel
 *  DESCRIPTION:    Set the current fuel distance counter value
 *  PARAMETER:      far ptr     to src buffer
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE Meas_SetDist_Fuel(DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE;
    NV_FuelDist = *fpDist;
    INT_GLOB_ENABLE;
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       Meas_GetDist_Vehicle
 *  DESCRIPTION:    interface to get the current vehicle distance counter value
 *  PARAMETER:      eUnits      type of return value
 *  RETURN:         formated distance value
 *  COMMENT:        -
 *********************************************************************** */
DIST_TYPE Meas_GetDist_Vehicle( MEASUNITS_TYPE eUnits )
{
    DIST_TYPE ResultDist;

    /* get formated value */
    Meas_ConvertDist( &NV_VehicDist, &ResultDist, eUnits);
    return ResultDist;
}

/***********************************************************************
 *  FUNCTION:       Meas_GetDist_VehicleRaw
 *  DESCRIPTION:    interface to get unformated current vehicle distance counter value
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE Meas_GetDist_VehicleRaw(DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE;
    *fpDist = NV_VehicDist;
    INT_GLOB_ENABLE;
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       Meas_SetDist_Vehicle
 *  DESCRIPTION:    Set the current vehicle distance counter value
 *  PARAMETER:      far ptr     to src buffer
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE Meas_SetDist_Vehicle(DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE;
    NV_VehicDist = *fpDist;
    INT_GLOB_ENABLE;
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       Meas_GetDist_TripCnt
 *  DESCRIPTION:    interface to get the current trip counter value
 *  PARAMETER:      eTripCntID      tripcounter to be read
 *                  eUnits          type of return value
 *  RETURN:         formated distance value
 *  COMMENT:        -
 *********************************************************************** */
UINT16 Meas_GetDist_TripCnt( TRIPC_ID eTripCntID, MEASUNITS_TYPE eUnits )
{
    DIST_TYPE   ResultDist;
    DIST_TYPE * pTripCnt = 0x0;

    /* select trip counter */
    switch (eTripCntID)
    {
        case eTRIPCNT_A: pTripCnt = &NV_TripCnt_A; break;
        case eTRIPCNT_B: pTripCnt = &NV_TripCnt_B; break;
        case eTRIPCOMM_A: pTripCnt = &NV_TripCom_A; break;
        case eTRIPCOMM_B: pTripCnt = &NV_TripCom_B; break;
        default: ODS1(DBG_MEAS,DBG_ERROR,"Illegal TripCounter ID [%u]!", eTripCntID);
            return (0);
            break;
    }

    /* get formated value */
    Meas_ConvertDist( pTripCnt, &ResultDist, eUnits);
    switch (eUnits)
    {
        case MR_KM:         return ResultDist.km; break;
        case MR_HM:         return ResultDist.hm; break;
        case MR_DKM:        return ResultDist.dkm; break;
        case MR_KM_ONLY:    return ResultDist.km_o;  break;
        case MR_HM_ONLY:    return ResultDist.hkm_o; break;
        case MR_DKM_ONLY:   return ResultDist.dkm_o; break;
        default:
            ODS1(DBG_MEAS,DBG_ERROR,"Illegal measurement type [%u]!", eUnits);
            break;
    }
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       Meas_GetDist_TripCntRaw
 *  DESCRIPTION:    interface to get unformated current Trip counter value
 *  PARAMETER:      eTripCntID      tripcounter ID to be read
 *                  fpDist          trip counter return value
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE Meas_GetDist_TripCntRaw( TRIPC_ID eTripCntID, DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE;

    /* select trip counter */
    switch (eTripCntID)
    {
        case eTRIPCNT_A: *fpDist = NV_TripCnt_A; break;
        case eTRIPCNT_B: *fpDist = NV_TripCnt_B; break;
        case eTRIPCOMM_A: *fpDist = NV_TripCom_A; break;
        case eTRIPCOMM_B: *fpDist = NV_TripCom_B; break;
        default: ODS1(DBG_MEAS,DBG_ERROR,"Illegal TripCounter ID [%u]!", eTripCntID);
            return (0);
            break;
    }
    INT_GLOB_ENABLE;
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       Meas_SetDist_TripCnt
 *  DESCRIPTION:    Set the current tripcounter value
 *  PARAMETER:      eTripCntID      tripcounter ID to be set
 *                  fpDist          trip counter set value
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE Meas_SetDist_TripCnt( TRIPC_ID eTripCntID, DIST_TYPE far * fpDist)
{
    INT_GLOB_DISABLE;
    /* select trip counter */
    switch (eTripCntID)
    {
        case eTRIPCNT_A: NV_TripCnt_A = *fpDist; break;
        case eTRIPCNT_B: NV_TripCnt_B = *fpDist; break;
        case eTRIPCOMM_A: NV_TripCom_A = *fpDist; break;
        case eTRIPCOMM_B: NV_TripCom_B = *fpDist; break;
        default: ODS1(DBG_MEAS,DBG_ERROR,"Illegal TripCounter ID [%u]!", eTripCntID);
            return (0);
            break;
    }
    INT_GLOB_ENABLE;
    return ERR_OK;
}






/***********************************************************************
 *  FUNCTION:       Meas_ConvertDist
 *  DESCRIPTION:    Re-Formats the content of the distance variable
 *  PARAMETER:      addr of     source distance value
 *                  addr of     formated target distance value
 *                  format type
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE Meas_ConvertDist( DIST_TYPE far * fpDistSrc, DIST_TYPE far * fpDistTrgt, MEASUNITS_TYPE RUnits)
{
    /* GET value ------------------ */
    UINT32 dwCopy;
    UINT32 dwScratch;
    UINT16 wScratch;

    /* get current value */
    INT_GLOB_DISABLE;
    dwCopy = fpDistSrc->dkm;
    INT_GLOB_ENABLE;

    /* format return value */
    switch (RUnits)
    {
        case MR_KM:         fpDistTrgt->km    = dwCopy / 100L; break;           /* 1 kilometer per digit */
        case MR_HM:         fpDistTrgt->hm    = dwCopy / 10L;  break;           /* 1 hectometer (=0,1 km) per digit */
        case MR_DKM:        fpDistTrgt->dkm   = dwCopy;       break;            /* 1 dekameter (=0,01 km) per digit */
        case MR_KM_ONLY:    fpDistTrgt->km_o  = (UINT16) (dwCopy/100L); break;  /* 1 kilometer per digit */        /* 1 kilometer per digit LIMITED to 65.536 km */
        case MR_HM_ONLY:    dwScratch = dwCopy/10L;                             /* 1 hectometer (=0,1 km) per digit WITHOUT leading km's */
                            fpDistTrgt->hkm_o = (UINT16)(dwScratch - ((dwScratch/10L)*10L)); break;
        case MR_DKM_ONLY:   dwScratch  = dwCopy;                                /* 1 dekameter (=0,01 km) per digit WITHOUT leading km's */
                            fpDistTrgt->dkm_o = (UINT16)(dwScratch - ((dwScratch/100L)*100L)); break;
        default: ODS1(DBG_MEAS,DBG_ERROR,"Unknown measurement type [%u]!", RUnits); break;
    }
    return ERR_OK;
}





 