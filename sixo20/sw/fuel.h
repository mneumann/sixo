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
 *  This software is copyright � 2001-2004 by N&K Development, Germany
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



/* Fuel Data Structure bundle */
typedef struct 
{
    UINT32 dwConsAct_ml_100;    // Actuel  Fuel Consumption (ml/100km)
    UINT32 dwConsAvr_ml_100;    // Average Fuel Consumption (ml/100km)
    UINT32 dwFuelExh_ml;        // Fuel exhaustion (ml)
    UINT32 dwFuelRem_ml;        // Fuel Remaining (ml)
    UINT32 dwDistExh_km;        // Fuel Distance since last refuel (km)
    UINT32 dwDistRem_km;        // Fuel Distance until next refuel (km)
    BOOL   fSensorAvail;        // FuelSensor available (just a copy for easier use)
    UINT32 dwImpRate;           // FuelSensor Impulsrate (Impulses/Liter)
    UINT32 dwImpulses;          // FuelSensor Impulses 
    UINT32 dwCapacity_ml;       // FuelCapacity (ml)
    UINT32 dwConsUsr_ml_100;    // User given Fuel Consumption (ml/100km)
} FUEL_DATASET_TYPE;


/* public prototypes */
void    Fuel_UpdMeas        (void);
void    Fuel_GetData        (FUEL_DATASET_TYPE * sFuelDataSet);


/* public defines */
#define LITER2ML    1000L       /* helper to convert ml <=> 1/1  Liter */
#define DL2ML       100L        /* helper to convert ml <=> 1/10 Liter */


#endif // _FUEL_H
