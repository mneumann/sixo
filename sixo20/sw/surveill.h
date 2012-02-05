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
 * Revision 3.2  2012/02/05 14:37:55  tuberkel
 * OIL_TEMP_WARN    110 ==> 120 °C
 * OIL_TEMP_ERR       120   ==> 150 °C
 *
 * Revision 3.1  2012/01/23 04:04:16  tuberkel
 * BugFix Symbol "SystemError"
 *
 * Revision 3.0  2010/11/07 09:31:37  tuberkel
 * V30 Preparations - no changes
 *
 * Revision 2.3  2010/11/07 09:31:16  tuberkel
 * V30 Preparations:
 * - Surveillance completely reviewed
 * - new: sorted list of any Info/warning/error
 * - new: informational states (e.g. SummerTime activated)
 * - new: internal states (e.g. RTC defect)
 * - new: API to retrieve currently active StateList
 * . moved some features from 'MOTO_BAU' version to general version
 *
 * Revision 2.2  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.1  2006/07/20 23:07:13  tuberkel
 * - added ABS input
 * - added TempSwitch instead of analog TempSensor
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
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
#define OIL_TEMP_WARN    120    /* °C */
#define OIL_TEMP_ERR     150    /* °C */

#define OIL_PRESS_RPM    500    /* U/Min */

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


/* Surveilance start delay in sec. (time for e.g. AD-Values to become valid) */
#define SURV_STARTDELAY     ANAIN_STARTDELAY    // mapped to ADC value start deleay



/* all surveilled vehicle/device parameter index/id */
typedef enum
{
   eSURVP_NOENTRY = 0,  // 00 MUST BE THE FIRST - default string, indicates 'no problem available'
   eSURVP_OILTEMP,      // 01 oil temp sensor
   eSURVP_WATTEMP,      // 02 water temp sensor
   eSURVP_VOLTAGE_LOW,  // 03 RPM + device power supply
   eSURVP_VOLTAGE_HIGH, // 04 device power supply
   eSURVP_ALTERNATOR,   // 05 RPM + external alternator input
   eSURVP_OILPRESS,     // 06 RPM + oil switch or oil sensor
   eSURVP_OILSWDEF,     // 07 RPM + oil switch defekt?
   eSURVP_ENGINECOLD,   // 08 RPM + oil/water temp sensor
   eSURVP_FUEL8L,       // 09 digital input
   eSURVP_FUEL4L,       // 10 digital input
   eSURVP_ABS,          // 11 digital input
   eSURVP_WATTEMPSW,    // 12 water temp switch
   eSURVP_GLACED,       // 13 air temp sensor
   eSURVP_SERVICEKM,    // 14 general distance vehicle service intervall
   eSURVP_SERVICEHOUR,  // 15 general runtime  vehicle service intervall
   eSURVP_SIMULATION,   // 16 vehicle simulation is active!
   eSURVP_HARDCOPY,     // 17 screen hardcopy via HBEAM switch and UART available!
   eSURVP_DLS_SUMMER,   // 18 changed daylight saving from winter -> summer
   eSURVP_DLS_WINTER,   // 19 changed daylight saving from summer -> winter
   eSURVP_RTC_BATT,     // 20 Invalid NVRAM parameters -> RTC Battery defect!

   eSURVP_LAST          // --- invalid state ---
} SURV_PARAM_ID_TYPE;


/* surveillance parameter states
   Note: this enumtype is bitcoded, to use acombination of states too! */
typedef enum
{
   eSURVST_OK       = 0,    // MUST BE THE FIRST - default state, indicates 'not info/warning/error'
   eSURVST_INFO     = 1,    // indicates 'just info, no problem!'
   eSURVST_WARNING  = 2,    // indicates 'might be a problem!'
   eSURVST_ERROR    = 4,    // indicates 'we have a serious problem!'
   eSURVST_ALL      = 255   // indicates 'any information/problem!'
} SURV_PARAM_STATE_TYPE;


/* surveillance LED warning mode */
#define SURV_LWM_SIXO    FALSE       // use SIxO propritary warning mode
#define SURV_LWM_STD     TRUE        // use SIxO propritary warning mode


/* surveillance parameter bundle (id/state) */
typedef struct
{
    SURV_PARAM_ID_TYPE      param;  // parameter ID
    SURV_PARAM_STATE_TYPE   state;  // state of this parameter
} SURV_PARAM_TYPE;

#define SURV_PARAM_MAX  9   // max number of supported concurrent infos/warnings/errors
                            // Note: limited to 1 cipher, because of '1/x error text' format

// parameter for SurvScrollVehicleState()
#define VST_SCROLL_DOWN     FALSE
#define VST_SCROLL_UP       TRUE


// public prototypes
ERRCODE SurvInit                    (void);
ERRCODE SurvProcessAll              (void);
void    SurvSetGlobalState          (UINT8 ucIdx);
void    SurvSetLEDState             (void);

void    SurvCheckAllDigitalWarnings (void);
void    SurvCheckAllAnalogWarnings  (void);
void    SurvCheckAllDeviceWarnings  (void);
void    SurvListSetParamState       (SURV_PARAM_ID_TYPE parameter, SURV_PARAM_STATE_TYPE level);
void    SurvScrollVehicleState      (BOOL fScrollDir);
void    SurvUpdateAnalogData        (void);
void    SurvUpdateEngRunTime        (void);
void    SurvCheckRPMFlash           (void);
void    SurvResetAllParameters      (void);

void    SurvListRemoveParam ( UINT8 bListIndex );
void    SurvListUpdateParam ( UINT8 bListIndex, SURV_PARAM_STATE_TYPE state );
void    SurvListAddParam    ( SURV_PARAM_ID_TYPE parameter, SURV_PARAM_STATE_TYPE state );
UINT8   SurvListGetIndex    ( SURV_PARAM_ID_TYPE parameter );
UINT8   SurvListGetCount    ( UINT8 statemask );
void    SurvListShow        ( void );


SURV_PARAM_STATE_TYPE SurvListGetParamState         (SURV_PARAM_ID_TYPE parameter);


#endif // _SURVEILL_H
