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
 *  Purpose:        All high level measurement api export header
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
 *      St�ckener Str. 115                  Hudeplan 37 A
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
 * Revision 3.3  2012/06/03 17:45:18  tuberkel
 * Updated API-Function-Name according to Modul-Name
 *
 * Revision 3.2  2012/05/27 17:52:40  tuberkel
 * Corrections for renamed Eeprom/Nvram Variables
 *
 * Revision 3.1  2012/05/27 16:01:41  tuberkel
 * All Eeprom/Nvram Variables renamed
 *
 * Revision 3.0  2010/11/07 13:40:55  tuberkel
 * V30 Preparations:
 * -  new DEF_WHEELIMPULSE
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
 * Revision 1.6  2006/02/18 14:55:46  Ralf
 * - tripcounter C+D added
 *
 *
 ************************************************************************ */

#ifndef _MEASURE_H
#define _MEASURE_H


/* common defines */


/* default values to initialize eeprom/nvram */
#define DEF_WHEELSIZE    2100   /* wheel circumdistance in Millimeters */
#define DEF_WHEELIMPULSE    1   /* number of impulses / wheel rotation */
#define DEF_CCF_NOM      0x01   /* low nibble: cylinder correktur factor nominator   (default: 1) */
#define DEF_CCF_DENOM    0x10   /* high nibble cylinder correktur factor denominator (default: 1) */




/* measurement limits */
#define MM_OVFL           9999  /* 10 m overflow limit in mm */
#define SLOW_ENGSPEED_L   800L  /* in RPM, lower  limit to begin with stronger filter */
#define SLOW_ENGSPEED_H  1500L  /* in RPM, higher limit to end   with stronger filter */
#define SLOW_WHEELSPEED    15L  /* in km/h, limit under which unaveraged values are used */


/* distance type - just a common interface type for API */
typedef union
{
    UINT32  dkm;    /* dekameter,        10 m/digit, maximum:    42.946.972,95 km */
    UINT32  hm;     /* hektometer,      100 m/digit, maximum:    42.946.972,9  km */
    UINT32  km;     /* kilometer,      1000 m/digit, maximum:    42.946.972    km */
    UINT16  km_o;   /* kilometer only, 1000 m/digit, range:   0    .. 65536    km */
    UINT16  dkm_o;  /* dekameter only,   10 m/digit, range:   0,00 ..     0,99 km */
    UINT16  hkm_o;  /* hektometer only, 100 m/digit, range:   0,0  ..     0,9  km */
} DIST_TYPE;

/* distance max values */
#define DIST_MAX_FUEL     99999L  /* max fuel distance 999.99 km in 0.01 km/bit */
#define DIST_MAX_TRIP   6500000L  /* max tripcounter distance 65000.00 km in 0.01 km/bit */
#define DIST_MAX_VEHIC 99999999L  /* max vehicle distance 999999.99 km in 0.01 km/bit */



/* vehicle speed structure:
    - f.e. used for maximum speed
    - resolution:    0,01 km/h
    - maximum:     655,35 km/h */
typedef UINT16 SPEED_TYPE;
#define SPEED_MAX 35000           /* max. plausible speed: 350 km/h */


/* engine speed structure:
    - f.e. used for maximum RPM (Rounds-per-Minute)
    - resolution:      1 RPM
    - maximum:     65535 RPM */
typedef UINT16 RPM_TYPE;
#define RPM_MAX 30000           /* max. plausible RPM: 30000 RPM */


/* cylinder correcture factor (EE_CCF):
    - used to adapt ignition impulses to number of cylinders sharing one impulse
    - devided into two parts: nominator & denominator
    - see 'Meas_GetSpeed_Engine()' in measure.c for details */
typedef union
{
    UINT8 byte;
    struct
    {
        unsigned char   nom:4;      /* nominator:   # of ignitions per round, range 1..15 */
        unsigned char   denom:4;    /* denominator: # of cylinders sharing this ignition, range 1..15  */
    } nibble;
} CCF_TYPE;




/* enum type to manipulate one of
   the trip counters */
typedef enum
{
    eTRIPCNT_A,    // trip counter A
    eTRIPCNT_B,    // trip counter B
    eTRIPCOMM_A,   // trip common A
    eTRIPCOMM_B,   // trip common B
} TRIPC_ID;



/* enum type simply to define return value units of measurement function */
/* (used to get more after comma information) */
typedef enum
{
    MR_KM_PER_H,        /* result in 1 kilo-meter/hour per digit */
    MR_HM_PER_H,        /* result in 1 hecto-meter/hour per digit (= 10*km/h) */
    MR_M_PER_S,         /* result in 1 meter/second per digit */
    MR_DM_PER_S,        /* result in 1 deci-meter/second per digit (=10*m/s)*/
    MR_CM_PER_S,        /* result in 1 centi-meter/second per digit (=100*m/s)*/
    MR_RPS,             /* result in 1 RoundPerSecond per digit (= RPM/60) */
    MR_RPM,             /* result in 1 RoundPerMinute per digit */
    MR_RPM_R10,         /* result in 1 RoundPerMinute per digit rounded to ten's */
    MR_KM,              /* result in 1 kilometer per digit */
    MR_HM,              /* result in 1 hectometer (=0,1 km) per digit */
    MR_DKM,             /* result in 1 dekameter (=0,01 km) per digit */
    MR_KM_ONLY,         /* result in 1 kilometer per digit LIMITED to 65.536 km */
    MR_HM_ONLY,         /* result in 1 hectometer (=0,1 km) per digit WITHOUT leading km's */
    MR_DKM_ONLY         /* result in 1 dekameter (=0,01 km) per digit WITHOUT leading km's */
} MEASUNITS_TYPE;





/* non public prototypes */
ERRCODE Meas_ConvertDist( DIST_TYPE far * fpDistSrc, DIST_TYPE far * fpDistTrgt, MEASUNITS_TYPE RUnits);



/* get/set distance measuremant data*/
#define SET  TRUE
#define GET  FALSE


/* public prototypes */
UINT16      Meas_GetSpeed_Wheel       ( MEASUNITS_TYPE eUnit);
UINT16      Meas_GetSpeed_Engine      ( MEASUNITS_TYPE eUnit);

DIST_TYPE   Meas_GetDist_Vehicle      ( MEASUNITS_TYPE eUnits );
ERRCODE     Meas_SetDist_Vehicle      ( DIST_TYPE far * fpDist);
ERRCODE     Meas_GetDist_VehicleRaw  ( DIST_TYPE far * fpDist);

UINT16      Meas_GetDist_Fuel       ( MEASUNITS_TYPE eUnits );
ERRCODE     Meas_SetDist_Fuel       ( DIST_TYPE far * fpDist);
ERRCODE     Meas_GetDist_FuelRaw   ( DIST_TYPE far * fpDist);

UINT16      Meas_GetDist_TripCnt        ( TRIPC_ID eTripCntID, MEASUNITS_TYPE eUnits );
ERRCODE     Meas_GetDist_TripCntRaw    ( TRIPC_ID eTripCntID, DIST_TYPE far * fpDist);
ERRCODE     Meas_SetDist_TripCnt        ( TRIPC_ID eTripCntID, DIST_TYPE far * fpDist);




#endif // _MEASURE_H

