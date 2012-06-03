/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2012-06-02 by Ralf Schwarzer
 *  Project:        SIxO 2.0
 *  Module:         fuel.c
 *  Purpose:        Fuel calculation & service module
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
 * Revision 1.1  2012/06/03 07:59:34  tuberkel
 * added to cvs
 *
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
#include "debug.h"
#include "anain.h"
#include "resource.h"
#include "fuel.h"
#include "timedate.h"
#include "sysparam.h"




// ------------------------------------------------------
/* external symbols (taken from eeprom/nvram) */
extern far UINT16           wMilliSecCounter;       // high resolution short distance timer, ms,  max  65 sec
extern far UINT16           wSecCounter;            // low  resolution long  distance timer, sec, max. 18 h
extern FUELSCNTRL_TYPE      EE_FuelSensCtrl;        /* FuelSensor Control flags (from eeprom) */
extern UINT8                EE_FuelConsUser;        /* FuelConsumption in 1/10 l/100 km (from eeprom) */
extern UINT16               EE_FuelCap;             /* FuelCapacity in 1/10 liters (from eeprom) */
extern UINT32               NV_FuelSensImp;         /* Fuel sensor Impulses counter since last refuel (NVRAM!) */


// ------------------------------------------------------
/* local symbols */




/***********************************************************************
 *  FUNCTION:       Fuel_CalcAll
 *  DESCRIPTION:    General cyclic fuel calcualtion
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void Fuel_CalcAll(void)
{
    UINT32 dwConsAct_ml_100;    // Actuel  Fuel Consumption (ml/100km)
    UINT32 dwConsAvr_ml_100;    // Average Fuel Consumption (ml/100km)
    UINT32 dwFuelExh_ml;        // Fuel exhaustion (ml)
    UINT32 dwFuelRem_ml;        // Fuel Remaining (ml)
    UINT32 dwDistExh_km;        // Fuel Distance since last refuel (km)
    UINT32 dwDistRem_km;        // Fuel Distance until next refuel (km)
    BOOL   fSensorAvail;        // FuelSensor available (just a copy for easier use)
    UINT32 dwImpRate;           // FuelSensor Impulsrate (Impulses/Liter)
    UINT32 dwCapacity_ml;       // FuelCapacity (ml)
    UINT32 dwConsUsr_ml_100;    // User given Fuel Consumption (ml/100km)    
    
    /* retrieve some local scaled copies of EEPROM/NVRAM values */
    fSensorAvail        =         EE_FuelSensCtrl.flags.FuelSAvail;
    dwImpRate           = (UINT32)EE_FuelSensCtrl.FuelSImpulseRate;    
    dwCapacity_ml       = (UINT32)EE_FuelCap      * DL2ML ;
    dwConsUsr_ml_100    = (UINT32)EE_FuelConsUser * DL2ML ;    
    NV_FuelSensImp      = DigInDrv_GPI_GetMeas(EE_FuelSensCtrl.flags.FuelSGPI)->dwLHCounter;

    /* retrieve DISTANCE SINCE LAST REFUELING */
    dwDistExh_km = Meas_Get_FuelDist(MR_KM_ONLY);

    /* ============================================================= */    
    /* calculate ABSOLUTE FUEL EXHAUSTION (dwFuelExh_ml, Milliliter):    
        Fuel(Exh, Liter) = SumOfImpulses (Imp) / Impulsrate (Imp/Liter) */       
    /* ============================================================= */            
    
    /* check: FuelSensor available? */
    if ( fSensorAvail == TRUE ) 
    {                                        
        /* convert FuelImpulses into exhausted fuel ('Liters.DeziLiters', prevent division by zero)*/
        if ( dwImpRate > 0 )
        {   dwFuelExh_ml = (LITER2ML * NV_FuelSensImp) / dwImpRate;
        }
        else
        {   dwFuelExh_ml = 0L;   // invalid user FuelSImpulseRate!
        }           
    }
    else
    {   /* NON-SENSOR-VERSION: calculate fuel exhaustion based on user given l/100 (prevent disivion by zero)*/ 
        if ( dwDistExh_km > 0 )
        {   dwFuelExh_ml = (dwConsUsr_ml_100 * dwDistExh_km) / 100L;        
        }
        else
        {   dwFuelExh_ml = 0L;   // not yet calculable!!
        }                   
    }
    
    /* ============================================================= */    
    /* calculate REMAINING FUEL dwFuelRem_ml (Milliliter, if valid parameters available)        
        Fuel(remain. Liter) = Fuel(Capacity, Liter) - Fuel(Exhaust, Liter) */        
    /* ============================================================= */      
          
    /* check: valid input data available? */
    if ( dwCapacity_ml > dwFuelExh_ml )     // assure a positive result!
    {   dwFuelRem_ml = dwCapacity_ml - dwFuelExh_ml;
    }
    else
    {   dwFuelRem_ml = 0L;           // not yet calculable!
    }

    /* ============================================================= */    
    /* calculate AVERAGE FUEL CONSUMPTION (ml/100km):    
        Fuel(ConsAverage, ml/100km) = 100 * Fuel(Exh, ml) / Dist(Exh, km) */
    /* ============================================================= */    
    
    /* check: FuelSensor available? */
    if ( fSensorAvail == TRUE ) 
    {    
        if (  ( dwDistExh_km > 0 )
            &&( dwFuelExh_ml  > 0 ) )
        {   dwConsAvr_ml_100 = (100L * dwFuelExh_ml) / dwDistExh_km;
        }
        else
        {   /* not yet calcuable => use user given default value */
            dwConsAvr_ml_100 = dwConsUsr_ml_100;      
        }                
    }
    else
    {   /* NON-SENSOR-VERSION: use user given fixed value instead */
        dwConsAvr_ml_100 = dwConsUsr_ml_100;      
    }                
        
    /* ============================================================= */    
    /* calculate REMAINING DISTANCE (km, if valid parameters available)   
        Dist(Remain) = 100 * Fuel(Remain, ml) / Fuel(ConsAverage, ml/100km) */
    /* ============================================================= */    
    
    /* check input parameters first */    
    if (  ( dwConsAvr_ml_100 > 0 )
        &&( dwFuelRem_ml  > 0 ) )
    {   dwDistRem_km = (100L * dwFuelRem_ml)  / dwConsAvr_ml_100;
    }
    else
    {   dwDistRem_km = 0;
    }                       

    /* ============================================================= */    
    /* Update display strings  */       
    
    /* check current display state */
    switch (MDObj.wDevState)
    {
        /*------------------------------------------- */
        /* update distance & liters SINCE LAST refuel */
        case MD_FUEL_FROM:
        {
            UINT8  bFuelExhaust_l;          // Fuel exhaustion - left comma part (liters only)
            UINT8  bFuelExhaust_dl;         // Fuel exhaustion - right comma part (deziliters only)                    
        
            // prepare parts to be displayed
            bFuelExhaust_l   = (UINT8)(  dwFuelExh_ml / LITER2ML );
            bFuelExhaust_dl  = (UINT8)(( dwFuelExh_ml - ((UINT32)bFuelExhaust_l * LITER2ML) )/100 );        

            /* prepare fuel-liters since last refueling */
            if ( bFuelExhaust_l <= 99 )
            {   sprintf( szFuelLiter, "%2u%c%1ul", bFuelExhaust_l, RESTXT_DEC_SEPARATOR, bFuelExhaust_dl );            
            }
            else
            {   sprintf( szFuelLiter, "%--%c-l", RESTXT_DEC_SEPARATOR );            
            }            

            /* calculate & prepare fuel-distance since last refueling */
            if ( dwDistExh_km <= 9999L )
            {   sprintf( szFuelDist,  "%4lu%s", dwDistExh_km, RESTXT_DIST_DESC );                        
            }
            else
            {   sprintf( szFuelDist,  " ---%s", RESTXT_DIST_DESC );                        
            }
            
            /* assure position of fuel icon & arrow right for this mode */
            FuelBmpObj.Org.wXPos        = 0;    // Fuel icon left
            FuelArrowRTxtObj.Org.wXPos  = 18;   // Arrow right of icon
        } break;                

        /*------------------------------------------- */
        /* update distance & liters UNTIL NEXT refuel */            
        case MD_FUEL_TO:
        {
            UINT8  bFuelRemain_l;           // Fuel Remaining - left comma part (liters only)
            UINT8  bFuelRemain_dl;          // Fuel Remaining - right comma part (deziliters only)            
            
            // prepare parts to be displayed
            bFuelRemain_l   = (UINT8)(  dwFuelRem_ml / LITER2ML );
            bFuelRemain_dl  = (UINT8)(( dwFuelRem_ml - ((UINT32)bFuelRemain_l * LITER2ML) )/100);        

            /* prepare fuel-liters since last refueling */
            if ( bFuelRemain_l <= 99 )
            {   sprintf( szFuelLiter, "%2u%c%1ul", bFuelRemain_l, RESTXT_DEC_SEPARATOR, bFuelRemain_dl );            
            }
            else
            {   sprintf( szFuelLiter, "%--%c-l", RESTXT_DEC_SEPARATOR );            
            }
            
            /* calculate & prepare fuel-distance since last refueling */
            if ( dwDistRem_km <= 9999 )
            {   sprintf( szFuelDist,  "%4lu%s", dwDistRem_km, RESTXT_DIST_DESC );                        
            }
            else
            {   sprintf( szFuelDist,  " ---%s", RESTXT_DIST_DESC );                        
            }
            
            /* update position of fuel icon & arrow */
            FuelArrowRTxtObj.Org.wXPos  = 0;            
            FuelBmpObj.Org.wXPos        = 8;                       
        } break;                
    
        /*------------------------------------------- */
        /* update fuel consumption (only if fuel sensor available) */            
        case MD_FUEL_CONS:
        {                          
            /* check: FuelSensor available? */
            if ( fSensorAvail == TRUE ) 
            {                          
                UINT8  bConsAvr_l_100;      // Average Fuel Consumption - left comma part (liters/100 km only)
                UINT8  bConsAvr_dl_100;     // Average Fuel Consumption - right comma part (dl/100 km only)                   
                UINT8  bConsAct_l_100;      // Actuel Fuel Consumption - left comma part (liters/100 km only)
                UINT8  bConsAct_dl_100;     // Actuel Fuel Consumption - right comma part (dl/100 km only)                   

                /* prepare average consumption */
                bConsAvr_l_100  = (UINT8)( dwConsAvr_ml_100 / LITER2ML );
                bConsAvr_dl_100 = (UINT8)( ( dwConsAvr_ml_100 - ((UINT32)bConsAvr_l_100 * LITER2ML)) / DL2ML );
                sprintf( szFuelConsAvr, "\xf8%2u%c%1u", bConsAvr_l_100, RESTXT_DEC_SEPARATOR, bConsAvr_dl_100 );                    
                
                /* prepare actual consumption */
                sprintf( szFuelConsAct, "--,-" );   // not yet used
                //sprintf( szFuelConsAct, "%2u%c%1u", bFuelCons_Liter, RESTXT_DEC_SEPARATOR, wFuelCons_ml );
                
                /* assure correct position of fuel icon (arrow-Object not in obj-list)*/
                FuelBmpObj.Org.wXPos = 0;                             
            }            
            else
            {   // MD_FUEL_CONS will not get called if 
                // (EE_FuelSensCtrl.flags.FuelSAvail == FALSE)
            }            
        } break;
        
        /*------------------------------------------- */
        /* any other Maindevice-State: nothing to do here ... */
        default: 
            break;    
    }




/***********************************************************************
 *  FUNCTION:       Fuel_FuelExhaust
 *  DESCRIPTION:    calculates absolute fuel exhaustion in ml,
 *                  based on fuel sensor parameter OR Eeprom value
 *  PARAMETER:      dwImpulseRate   fuel sensor impulsrate (Imp/L)
 *                  dwImpulses      fuel sensor impulses
 *                  0               if not calculable
 *  RETURN:         Exhausted fuel in ml
 *  COMMENT:        Fuel(Exh, Liter) = SumOfImpulses (Imp) / Impulsrate (Imp/Liter)
 *********************************************************************** */
UINT32 Fuel_FuelExhaust ( void )
{    
    UINT32 RValue; 

    /* prepare input values */    
    BOOL   fSensorAvail     =         EE_FuelSensCtrl.flags.FuelSAvail;
    UINT32 dwImpRate        = (UINT32)EE_FuelSensCtrl.FuelSImpulseRate;    
    UINT32 dwCapacity_ml    = (UINT32)EE_FuelCap      * DL2ML ;
    UINT32 dwConsUsr_ml_100 = (UINT32)EE_FuelConsUser * DL2ML ;    
    UINT32 dwSensImp        = DigInDrv_GPI_GetMeas(EE_FuelSensCtrl.flags.FuelSGPI)->dwLHCounter;
    UINT32 dwDistExh_km     = Meas_Get_FuelDist(MR_KM_ONLY);
        
    /* check: FuelSensor available? */
    if ( fSensorAvail == TRUE ) 
    {                                        
        /* FUEL-SENSOR-VERSION: convert FuelImpulses into exhausted fuel 
           (prevent division by zero)*/
        if ( dwImpRate > 0 )
        {   RValue = (LITER2ML * dwSensImp / dwImpRate;
        }
        else
        {   RValue = 0L;   // invalid user FuelSImpulseRate!
        }           
    }
    else
    {   /* NON-SENSOR-VERSION: calculate fuel exhaustion based on user given l/100 (prevent disivion by zero)*/ 
        if ( dwDistExh_km > 0 )
        {   RValue = (dwConsUsr_ml_100 * dwDistExh_km) / 100L;        
        }
        else
        {   RValue = 0L;   // not yet calculable!!
        }                   
    }
    return (RValue);
}

