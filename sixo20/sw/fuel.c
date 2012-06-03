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
 * Revision 1.3  2012/06/03 12:46:04  tuberkel
 * Moved all Fuel-Formating from maindev.c ==> fuel.c
 *
 * Revision 1.2  2012/06/03 11:41:14  tuberkel
 * Moved all Fuel-Calculations from maindev.c ==> fuel.c
 *
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
#include "digindrv.h"
#include "measure.h"


// ------------------------------------------------------
/* external symbols (taken from eeprom/nvram) */
extern far UINT16           wMilliSecCounter;       /* high resolution short distance timer, ms,  max  65 sec */
extern far UINT16           wSecCounter;            /* low  resolution long  distance timer, sec, max. 18 h */
extern FUELSCNTRL_TYPE      EE_FuelSensCtrl;        /* FuelSensor Control flags (from eeprom) */
extern UINT8                EE_FuelConsUser;        /* FuelConsumption in 1/10 l/100 km (from eeprom) */
extern UINT16               EE_FuelCap;             /* FuelCapacity in 1/10 liters (from eeprom) */
extern UINT32               NV_FuelSensImp;         /* Fuel sensor Impulses counter since last refuel (NVRAM!) */


// ------------------------------------------------------
/* local symbols */
FUEL_DATASET_TYPE   sFuel;


// ------------------------------------------------------
/* local define */






/***********************************************************************
 *  FUNCTION:       Fuel_UpdMeas
 *  DESCRIPTION:    Updates complete fuel Measurement
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Call this function at at least once, before
 *                  using one of the API functions to return result.
 *
 *                  'GPIx.dwLHCounter' has been initilized by NVRAM value at init time
 *                  'NV_FuelSensImp' is implicitely saved back into NVRAM 
 *                  (for next init time)
 *
 *                  If FuelSensor is available and fuel is consumed - but
 *                  wheel stands still - we change the view from 
 *                  'l/100km' to 'l/Min' by simply exhanging the icons.
 *********************************************************************** */
void Fuel_UpdMeas(void)
{   
    /* Update the NVRAM value of FuelSensor-Impulses (see note above) */
    NV_FuelSensImp = DigInDrv_GPI_GetMeas(EE_FuelSensCtrl.flags.FuelSGPI)->dwLHCounter;
    
    /* get a fresh copy of EEPROM/NVRAM values (might have been changed) */
    sFuel.fSensorAvail      =         EE_FuelSensCtrl.flags.FuelSAvail;
    sFuel.dwImpRate         = (UINT32)EE_FuelSensCtrl.FuelSImpulseRate;    
    sFuel.dwCapacity_ml     = (UINT32)EE_FuelCap      * DL2ML ;
    sFuel.dwConsUsr_ml_100  = (UINT32)EE_FuelConsUser * DL2ML ;    
    sFuel.dwDistExh_km      = Meas_Get_FuelDist(MR_KM_ONLY);
    sFuel.dwImpulses        = NV_FuelSensImp;   

    /* ============================================================= */    
    /* calculate ABSOLUTE FUEL EXHAUSTION (dwFuelExh_ml, Milliliter):    
        Fuel(Exh, Liter) = SumOfImpulses (Imp) / Impulsrate (Imp/Liter) */       
    /* ============================================================= */            
    
    /* check: FuelSensor available? */
    if ( sFuel.fSensorAvail == TRUE ) 
    {                                        
        /* convert FuelImpulses into exhausted fuel ('Liters.DeziLiters', prevent division by zero)*/
        if ( sFuel.dwImpRate > 0 )
        {   sFuel.dwFuelExh_ml = (LITER2ML * sFuel.dwImpulses) / sFuel.dwImpRate;
        }
        else
        {   sFuel.dwFuelExh_ml = 0L;   // invalid user FuelSImpulseRate!
        }           
    }
    else
    {   /* NON-SENSOR-VERSION: calculate fuel exhaustion based on user given l/100 (prevent disivion by zero)*/ 
        if ( sFuel.dwDistExh_km > 0 )
        {   sFuel.dwFuelExh_ml = (sFuel.dwConsUsr_ml_100 * sFuel.dwDistExh_km) / 100L;        
        }
        else
        {   sFuel.dwFuelExh_ml = 0L;   // not yet calculable!!
        }                   
    }
    
    /* ============================================================= */    
    /* calculate REMAINING FUEL dwFuelRem_ml (Milliliter, if valid parameters available)        
        Fuel(remain. Liter) = Fuel(Capacity, Liter) - Fuel(Exhaust, Liter) */        
    /* ============================================================= */      
          
    /* check: valid input data available? */
    if ( sFuel.dwCapacity_ml > sFuel.dwFuelExh_ml )     // assure a positive result!
    {   sFuel.dwFuelRem_ml = sFuel.dwCapacity_ml - sFuel.dwFuelExh_ml;
    }
    else
    {   sFuel.dwFuelRem_ml = 0L;           // not yet calculable!
    }

    /* ============================================================= */    
    /* calculate AVERAGE FUEL CONSUMPTION (ml/100km):    
        Fuel(ConsAverage, ml/100km) = 100 * Fuel(Exh, ml) / Dist(Exh, km) */
    /* ============================================================= */    
    
    /* check: FuelSensor available? */
    if ( sFuel.fSensorAvail == TRUE ) 
    {    
        if (  ( sFuel.dwDistExh_km > 0 )
            &&( sFuel.dwFuelExh_ml  > 0 ) )
        {   sFuel.dwConsAvr_ml_100 = (100L * sFuel.dwFuelExh_ml) / sFuel.dwDistExh_km;
        }
        else
        {   /* not yet calcuable => use user given default value */
            sFuel.dwConsAvr_ml_100 = sFuel.dwConsUsr_ml_100;      
        }                
    }
    else
    {   /* NON-SENSOR-VERSION: use user given fixed value instead */
        sFuel.dwConsAvr_ml_100 = sFuel.dwConsUsr_ml_100;      
    }                
        
    /* ============================================================= */    
    /* calculate REMAINING DISTANCE (km, if valid parameters available)   
        Dist(Remain) = 100 * Fuel(Remain, ml) / Fuel(ConsAverage, ml/100km) */
    /* ============================================================= */    
    
    /* check input parameters first */    
    if (  ( sFuel.dwConsAvr_ml_100 > 0 )
        &&( sFuel.dwFuelRem_ml  > 0 ) )
    {   sFuel.dwDistRem_km = (100L * sFuel.dwFuelRem_ml)  / sFuel.dwConsAvr_ml_100;
    }
    else
    {   sFuel.dwDistRem_km = 0;
    }                       
}





/***********************************************************************
 *  FUNCTION:       Fuel_GetData
 *  DESCRIPTION:    Returns a copy of complete fuel data set 
 *  PARAMETER:      dwImpulseRate   fuel sensor impulsrate (Imp/L)
 *                  dwImpulses      fuel sensor impulses
 *                  0               if not calculable
 *  RETURN:         Exhausted fuel in ml
 *  COMMENT:        Assure to call 'Fuel_UpdMeas()' at least once before
 *                  retrieving a copy of this data set.
 *********************************************************************** */
void Fuel_GetData (FUEL_DATASET_TYPE * pFuelDataSet)
{    
    /* assure fresh calculated data before returning them */
    Fuel_UpdMeas();
    
    /* simply copy our local data */
    memcpy( pFuelDataSet, &sFuel, sizeof(FUEL_DATASET_TYPE) );
}
    
    

/***********************************************************************
 *  FUNCTION:       Fuel_GetFormatedString
 *  DESCRIPTION:    Returns a formated string for the requested data
 *  PARAMETER:      eChoice     indicator for requested data
 *                  szDest      destination buffer to insert formated string
 *                  bLen        max length of reqested string (incl. last '\x00)
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void Fuel_GetFormStr (FUEL_SLCT eChoice, STRING szDest, UINT8 bLen )
{      
    /* check requested data */
    switch (eChoice)
    {       
        /* -------------------------------------------------- */
        case FS_DIST_EXH:  // Fuel Distance since last refuel (km) 
        {
            /* calculate & prepare fuel-distance since last refueling */
            if  ( sFuel.dwDistExh_km <= 9999L )
            {   sprintf( szDest,  "%4lu%s", sFuel.dwDistExh_km, RESTXT_DIST_DESC );                        
            }
            else
            {   sprintf( szDest,  "9999%s", RESTXT_DIST_DESC );                        
            }
            
        } break;
    
        /* -------------------------------------------------- */
        case FS_DIST_REM:  // Fuel Distance until next refuel (km)   
        {
            /* calculate & prepare fuel-distance since last refueling */
            if ( sFuel.dwDistRem_km <= 9999L )
            {   sprintf( szDest,  "%4lu%s", sFuel.dwDistRem_km, RESTXT_DIST_DESC );                        
            }
            else
            {   sprintf( szDest,  "9999%s", RESTXT_DIST_DESC );                        
            }                        
        } break;
    
        /* -------------------------------------------------- */
        case FS_FUEL_EXH:  // Fuel exhaustion (ml)
        {
            UINT8  bFuelExhaust_l;          // Fuel exhaustion - left comma part (liters only)
            UINT8  bFuelExhaust_dl;         // Fuel exhaustion - right comma part (deziliters only)                    
        
            // prepare parts to be displayed
            bFuelExhaust_l   = (UINT8)(  sFuel.dwFuelExh_ml / LITER2ML );
            bFuelExhaust_dl  = (UINT8)(( sFuel.dwFuelExh_ml - ((UINT32)bFuelExhaust_l * LITER2ML) )/100 );        

            /* prepare fuel-liters since last refueling */
            if ( bFuelExhaust_l <= 99 )
            {   sprintf( szDest, "%2u%c%1ul", bFuelExhaust_l, RESTXT_DEC_SEPARATOR, bFuelExhaust_dl );            
            }
            else
            {   sprintf( szDest, "99%c9l", RESTXT_DEC_SEPARATOR );            
            }                        
        } break;
    
        /* -------------------------------------------------- */
        case FS_FUEL_REM:  // Fuel Remaining (ml)
        {
            UINT8  bFuelRemain_l;           // Fuel Remaining - left comma part (liters only)
            UINT8  bFuelRemain_dl;          // Fuel Remaining - right comma part (deziliters only)            
            
            // prepare parts to be displayed
            bFuelRemain_l   = (UINT8)(  sFuel.dwFuelRem_ml / LITER2ML );
            bFuelRemain_dl  = (UINT8)(( sFuel.dwFuelRem_ml - ((UINT32)bFuelRemain_l * LITER2ML) )/100);        

            /* prepare fuel-liters since last refueling */
            if ( bFuelRemain_l <= 99 )
            {   sprintf( szDest, "%2u%c%1ul", bFuelRemain_l, RESTXT_DEC_SEPARATOR, bFuelRemain_dl );            
            }
            else
            {   sprintf( szDest, "99%c9l", RESTXT_DEC_SEPARATOR );            
            }
            
        } break;
    
        /* -------------------------------------------------- */
        case FS_CONS_ACT:  // if FuelSensor available: Actuel  Fuel Consumption (ml/100km)
        {
            UINT8  bConsAct_l_100;      // Actuel Fuel Consumption - left comma part (liters/100 km only)
            UINT8  bConsAct_dl_100;     // Actuel Fuel Consumption - right comma part (dl/100 km only)                   
            
            /* prepare actual consumption */
            sprintf( szDest, "--,-" );   // not yet used
            //sprintf( szDest, "%2u%c%1u", bFuelCons_Liter, RESTXT_DEC_SEPARATOR, wFuelCons_ml );
            
        } break;
    
        /* -------------------------------------------------- */
        case FS_CONS_AVR:  // if FuelSensor available: Average Fuel Consumption (ml/100km)
        {
            UINT8  bConsAvr_l_100;      // Average Fuel Consumption - left comma part (liters/100 km only)
            UINT8  bConsAvr_dl_100;     // Average Fuel Consumption - right comma part (dl/100 km only)                   

            /* prepare average consumption */
            bConsAvr_l_100  = (UINT8)(   sFuel.dwConsAvr_ml_100 / LITER2ML );
            bConsAvr_dl_100 = (UINT8)( ( sFuel.dwConsAvr_ml_100 - ((UINT32)bConsAvr_l_100 * LITER2ML)) / DL2ML );
            sprintf( szDest, "\xf8%2u%c%1u", bConsAvr_l_100, RESTXT_DEC_SEPARATOR, bConsAvr_dl_100 );                                               
        } break;
    
        /* -------------------------------------------------- */
        default: szDest[0] = 'x'; break;   // Parameter error!
    }
}



        
