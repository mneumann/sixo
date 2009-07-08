/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2007-03-29 by Matthias Otto
 *  Project:        SIxO
 *  Module:         devices
 *  Purpose:        Language specific resource strings and formats
 *  Comments:       use this file for language specific stuff only
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
 * Revision 2.3  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.2  2009/06/21 18:04:11  tuberkel
 * New: RESTXT_SET_COMPCAL
 *
 * Revision 2.1  2007/03/30 09:56:28  tuberkel
 * - Added language specific resource_x.h files
 * - controled via #define LANG=0/1/2 for DE/EN/NL
 *
 *
 ************************************************************************ */


#ifndef _RESOURCE_EN_H
#define _RESOURCE_EN_H


/**************************** E N G L I S H   U K  ***************************************/

/* language decriptor */
#define RESTXT_LANG            "EN"

/* main device strings */
#define RESTXT_SPEED_DESC       "km/h"
#define RESTXT_RPM_DESC         "rpm"
#define RESTXT_DIST_DESC        "km"
#define RESTXT_DEGC_DESC        "�C"
#define RESTXT_VOLT_DESC        "V"
#define RESTXT_TRIP1DESC        "T1"
#define RESTXT_TRIP2DESC        "T2"
#define RESTXT_SPEEDMAX_DESC    "max"


/* monitor device resources */
#define RESTXT_MON_TEMP_DEV        "Device  "
#define RESTXT_MON_TEMP_AIR        "Air     "
#define RESTXT_MON_BATT            "Battery "
#define RESTXT_MON_TEMP_WAT        "Water   "
#define RESTXT_MON_TEMP_OIL        "Oil     "
#define RESTXT_MON_VOLTAGE         "Battery "


/* statistic device resources */
#define RESTXT_STAT_BATT            "Battery "
#define RESTXT_STAT_BATT_DESC       "V"
#define RESTXT_STAT_TEMP_AIR        "Air  "
#define RESTXT_STAT_TEMP_OIL        "Oil  "
#define RESTXT_STAT_TEMP_WAT        "H2O  "
#define RESTXT_STAT_TEMP_DESC       "�C"
#define RESTXT_STAT_RPM_MAX         "rpm max "
#define RESTXT_STAT_RPM_DESC        "rpm"
#define RESTXT_STAT_V_MAX           "v max "
#define RESTXT_STAT_V_DESC          "km/h"
#define RESTXT_STAT_H_SERV          "Serv."
#define RESTXT_STAT_H_ALL           "Tot."
#define RESTXT_STAT_H_DESC          "h"


/* vehicle state texts;
 * NOTE: if you add one here, also add the VEHICLE_STATE_ value in device.h,
 * as well as an assignment in MonitorDeviceInit() */
                                // ....!....!....!....!.
#define RESTXT_STATE_ALLRIGHT     "      \xbb All OK \xab     "
#define RESTXT_STATE_WATERTEMP    "Water Temp. Too High!"
#define RESTXT_STATE_OILTEMP      " Oil Temp. Too High! "
#define RESTXT_STATE_ENGINE_COLD  "  Engine Too Cold!   "
#define RESTXT_STATE_VOLTAGE_LOW  "  Voltage Too Low!   "
#define RESTXT_STATE_VOLTAGE_HIGH "  Voltage Too High!  "
#define RESTXT_STATE_ALTERNATOR   "    Not Charging!    "
#define RESTXT_STATE_OILPRESS     "  Low Oil Pressure!  "
#define RESTXT_STATE_OILSWDEF     " Oil Pr. Sw. Defect? "
#define RESTXT_STATE_FUEL8L       " < 8 Ltr. Fuel Left  "
#define RESTXT_STATE_FUEL4L       " < 4 Ltr. Fuel Left  "
#define RESTXT_STATE_ABS          "     ABS inaktiv!    "
#define RESTXT_STATE_GLACED       "        Ice?         "
#define RESTXT_STATE_SERVICEKM    "  Service-Interval   "



/* settings device strings */
#define RESTXT_SET_WHEELSIZEDESC    "Wheel:"
#define RESTXT_SET_WHEELSIZEUNIT    "mm"
#define RESTXT_SET_CCFNOMDESC       "Ign:"
#define RESTXT_SET_CCFNOMUNIT       "/"
#define RESTXT_SET_DPLLIGHTDESC     "Display A:"
#define RESTXT_SET_DPLHELLGKDESC    "B:"
#define RESTXT_SET_DPLCONTRDESC     "C:"

#define RESTXT_SET_RPMFLASH         "RPM Flash:"
#define RESTXT_SET_RPMFLASHUNIT     " rpm"

#define RESTXT_SET_BIKETYPE         "Bike:"
#define RESTXT_SET_COMPCAL          "Cmp:"
#define RESTXT_SET_TRIPCNTFL        "TripCnt:"

#define RESTXT_SET_SERVKM           "Serv. Due:"

#define RESTXT_SET_ERT_SRV          "Serv.:"
#define RESTXT_SET_ERT_ALL          "Tot.:"
#define RESTXT_SET_ERT_UNIT         "h"

#define RESTXT_SET_VEHICKMDESC      "Tot. Veh.:"
#define RESTXT_SET_VEHICKMUNIT      " km"


/* display light strings */
#define RESTXT_DPLLIGHTCHOICE_A " on "
#define RESTXT_DPLLIGHTCHOICE_B " off"
#define RESTXT_DPLLIGHTCHOICE_c "auto"


/* 'day of week' strings */
#define RESTXT_DOWSHORT_MON     "Mo"
#define RESTXT_DOWSHORT_TUE     "Tu"
#define RESTXT_DOWSHORT_WED     "We"
#define RESTXT_DOWSHORT_THU     "Th"
#define RESTXT_DOWSHORT_FRI     "Fr"
#define RESTXT_DOWSHORT_SAT     "Sa"
#define RESTXT_DOWSHORT_SUN     "Su"

#define RESTXT_DOWLONG_MON      "Monday"
#define RESTXT_DOWLONG_TUE      "Tuesday"
#define RESTXT_DOWLONG_WED      "Wednesday"
#define RESTXT_DOWLONG_THU      "Thursday"
#define RESTXT_DOWLONG_FRI      "Friday"
#define RESTXT_DOWLONG_SAT      "Saturday"
#define RESTXT_DOWLONG_SUN      "Sunday"

#define RESTXT_DEC_SEPARATOR	'.'
#define RESTXT_THOU_SEPARATOR	','

/* time/date format separator */
#define RESTXT_DAYSEPERATOR     "/"
#define RESTXT_TIMESEPERATOR    ":"
#define RESTXT_CLKCALIBDESC     "�"

#define RESENUM_HHMM			ENGL_HHMM
#define RESENUM_HHMMSS			ENGL_HHMMSS

#define RESENUM_DDMMYY			ENGL_DDMMYY
#define RESENUM_DDMMYYYY		ENGL_DDMMYYYY
#define RESENUM_WWDDMMYY		ENGL_WWDDMMYY


#endif /* _RESOURCE_EN_H */

