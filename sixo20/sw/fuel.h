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
 * Revision 1.7  2013/03/26 20:58:25  tuberkel
 * FuelConsumption improved - smoother & more accurately
 *
 * Revision 1.6  2012/06/07 19:14:27  tuberkel
 * Fuel Consumption:
 * - If no motion: Changed from 'l/Min' => 'l/Hour'
 *
 * Revision 1.5  2012/06/05 20:03:42  tuberkel
 * BugFixes:
 * - Actual Consumption
 * - Average Consumption higher resultion
 * - invalid results mapped to '-.-' instead of '99.9'
 *
 * Revision 1.4  2012/06/03 20:37:39  tuberkel
 * - Prepared for 'Actual l/100' and 'l/h'
 * - not yet usable
 *
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


#ifndef _FUEL_H
#define _FUEL_H


/* -------------------------------------------------------- */
/* Complete Fuel-Calculations-Datastructure-Bundle */
typedef struct
{
    UINT32 dwConsAct_ml_hkm;    // Actuel  Fuel Consumption (ml/100km)
    UINT32 dwConsAct_ml_Min;    // Actuel  Fuel Consumption (ml/Min)
    UINT32 dwConsAvr_ml_hkm;    // Average Fuel Consumption (ml/100km)

    UINT32 dwFuelExh_ml;        // Fuel exhaustion (ml)
    UINT32 dwFuelRem_ml;        // Fuel Remaining (ml)

    UINT32 dwDistExh_m;         // Fuel Distance since last refuel (m)
    UINT32 dwDistRem_km;        // Fuel Distance until next refuel (km)

    BOOL   fSensorAvail;        // FuelSensor available (just a copy for easier use)
    UINT32 dwImpRate;           // FuelSensor Impulsrate (Impulses/Liter)
    UINT32 dwImpulses;          // FuelSensor Impulses
    UINT32 dwCapacity_ml;       // FuelCapacity (ml)
    UINT32 dwConsUsr_ml_hkm;    // User given Fuel Consumption (ml/100km)

    UINT16 wLastTime_ms;        // Timestamp of LAST actual consumption sample (ms)
    UINT32 dwLastSample_Imp;    // FuelSensor-Impulses of LAST actual consumption sample (ms)
    UINT32 dwLastDist_m;        // km-reading of of LAST actual consumption sample (m)

} FUEL_DATASET_TYPE;




/* -------------------------------------------------------- */
/* special datasets for 'Actual-Fuel-Consumption' */

#define FUEL_ACT_INTV  1000     // sample interval for actual calculation in ms
#define FUEL_ACT_CNT      3     // number of samples to be used as avarage actual calculation

/* Fuel-Sample-Data-Pair */
typedef struct
{
    UINT32 dwDist_m;            // distance in meters   since last sample
    UINT32 dwImpulses;          // fuel sensor impulses since last sample
    UINT32 wDiffTime_ms;        // elapsed time in ms   since last sample
} FUEL_SAMPLE;



/* -------------------------------------------------------- */
/* to select a calculated fuel data */
typedef enum
{
    FS_DIST_EXH,                // Fuel Distance since last refuel (km)
    FS_DIST_REM,                // Fuel Distance until next refuel (km)
    FS_FUEL_EXH,                // Fuel exhaustion (ml)
    FS_FUEL_REM,                // Fuel Remaining (ml)
                                // only valid, if FuelSensor available:
    FS_CONS_ACT_KM,             // Actuel  Fuel Consumption (ml/100km)
    FS_CONS_ACT_HR,             // Actuel  Fuel Consumption (ml/Hour)
    FS_CONS_ACT_MIN,            // Actuel  Fuel Consumption (ml/Min)
    FS_CONS_AVR                 // Average Fuel Consumption (ml/100km)
} FUEL_SLCT;



/* -------------------------------------------------------- */
/* public prototypes */

void    Fuel_UpdMeas        (void);
void    Fuel_GetData        (FUEL_DATASET_TYPE * sFuelDataSet);
void    Fuel_GetFormStr     (FUEL_SLCT eChoice, STRING szDest, UINT8 bLen );

/* public defines */
#define LITER2ML    1000L       /* helper to convert ml <=> 1/1  Liter */
#define DL2ML       100L        /* helper to convert ml <=> 1/10 Liter */


#endif // _FUEL_H
