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
 *  Module:         surveill
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
 * Revision 1.1  2006/03/29 06:22:46  tuberkel
 * Initial revision
 *
 * Revision 1.5  2006/02/18 15:05:14  Ralf
 * - engine runtime surveillance added
 *
 *
 ************************************************************************ */


#ifndef _SURVEILL_H
#define _SURVEILL_H



/* surveillance limits */
#define OIL_TEMP_LOW      50    /* °C */
#define OIL_TEMP_WARN    110    /* °C */
#define OIL_TEMP_ERR     120    /* °C */

#define OIL_PRESS_RPM    500   /* U/Min */

#define ENG_COLD_RPM    4000    /* U/Min */

#define WAT_TEMP_LOW      75    /* °C */
#define WAT_TEMP_WARN    100    /* °C */
#define WAT_TEMP_ERR     110    /* °C */

#define AIR_TEMP_GLACED_LOW   -2 /* °C */
#define AIR_TEMP_GLACED_HIGH   2 /* °C */

#define VOLTAGE_LOW      1050   /* 10,5V */
#define VOLTAGE_LOW_RPM  1700   /* U/Min */
#define VOLTAGE_HIGH     1550   /* 15,5V */

#define ALTERN_LOW        800   /* 8V */
#define ALTERN_LOW_RPM   2000   /* U/Min */

#define ENGRUNTIME_RPM       500    /* U/Min */
#define ENGRUNTIME_ALL_MAX  9999    /* h */
#define ENGRUNTIME_SRV_MAX   999    /* h */





/* all potential vehicle state parameters for any bike */
typedef enum
{
   VEHICLE_STATE_OILTEMP,       // oil temp sensor
   VEHICLE_STATE_WATTEMP,       // water temp sensor
   VEHICLE_STATE_VOLTAGE_LOW,   // RPM + device power supply
   VEHICLE_STATE_VOLTAGE_HIGH,  // device power supply
   VEHICLE_STATE_ALTERNATOR,    // RPM + external alternator input
   VEHICLE_STATE_OILPRESS,      // RPM + oil switch or oil sensor
   VEHICLE_STATE_OILSWDEF,      // RPM + oil switch defekt?
   VEHICLE_STATE_ENGINECOLD,    // RPM + oil/water temp sensor
   VEHICLE_STATE_FUEL8L,        // digital input
   VEHICLE_STATE_FUEL4L,        // digital input
   VEHICLE_STATE_GLACED,        // air temp sensor
   VEHICLE_STATE_SERVICEKM,     // general vehicle service intervall
   LAST_VEHICLE_STATE_PARM      // --- invalid state --- */
} VEHICLE_STATE_PARM;


/* all states for vehicle parameters */
typedef enum
{
   VEHICLE_STATE_LEVEL_OK,          // indicates 'no error'
   VEHICLE_STATE_LEVEL_INFO,        // indicates 'just info, no problem!'
   VEHICLE_STATE_LEVEL_WARNING,     // indicates 'might be a problem!'
   VEHICLE_STATE_LEVEL_ERROR,       // indicates 'we have a serious problem!'

   VEHICLE_STATE_LEVEL_INVALID      // invalid level
} eVEHICLE_STATE_LEVEL;

typedef UINT8 VEHICLE_STATE_LEVEL;  /* save us some bytes... :-) */


// parameter for SurvScrollVehicleState()
#define VST_SCROLL_DOWN     FALSE
#define VST_SCROLL_UP       TRUE


// public prototypes
ERRCODE SurvInit(void);
ERRCODE SurvCheckAllValues(void);

void SurvCheckAllDigitalWarnings(void);
void SurvCheckAllAnalogWarnings(void);
void SurvSetVehicleState(VEHICLE_STATE_PARM parameter, VEHICLE_STATE_LEVEL level);
void SurvScrollVehicleState(BOOL fScrollDir);
void SurvUpdateAnalogData(void);
void SurvUpdateEngRunTime(void);
void SurvCheckRPMFlash(void);
void SurvResetVehicleStates(void);
UINT8 SurvGetNOKStates(void);
VEHICLE_STATE_LEVEL SurvGetVehicleState(VEHICLE_STATE_PARM parameter);


#endif // _SURVEILL_H
