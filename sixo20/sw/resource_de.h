/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2007-03-29 by Ralf Schwarzer
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
 *      Stöckener Str. 115                  Knickstr. 10
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
 * Revision 2.1  2007/03/30 09:56:28  tuberkel
 * - Added language specific resource_x.h files
 * - controled via #define LANG=0/1/2 for DE/EN/NL
 *
 *
 ************************************************************************ */


#ifndef _RESOURCE_DE_H
#define _RESOURCE_DE_H


/****************************  G E R M A N  *******************************************/

/* language decriptor */
#define RESTXT_LANG            "DE"

/* main device strings */
#define RESTXT_SPEED_DESC       "km/h"
#define RESTXT_RPM_DESC         "U/Min"
#define RESTXT_DIST_DESC        "km"
#define RESTXT_DEGC_DESC        "°C"
#define RESTXT_VOLT_DESC        "V"
#define RESTXT_TRIP1DESC        "T1"
#define RESTXT_TRIP2DESC        "T2"
#define RESTXT_SPEEDMAX_DESC    "max"


/* monitor device resources */
#define RESTXT_MON_TEMP_DEV        "Ger\xe4t   "
#define RESTXT_MON_TEMP_AIR        "Luft    "
#define RESTXT_MON_BATT            "Batt.   "
#define RESTXT_MON_TEMP_WAT        "Wasser  "
#define RESTXT_MON_TEMP_OIL        "\xd6l      "
#define RESTXT_MON_VOLTAGE         "Batt.   "


/* statistic device resources */
#define RESTXT_STAT_BATT            "Batterie "
#define RESTXT_STAT_BATT_DESC       "V"
#define RESTXT_STAT_TEMP_AIR        "Luft "
#define RESTXT_STAT_TEMP_OIL        "\xd6l   "
#define RESTXT_STAT_TEMP_WAT        "H2O  "
#define RESTXT_STAT_TEMP_DESC       "°C"
#define RESTXT_STAT_RPM_MAX         "DRZ max "
#define RESTXT_STAT_RPM_DESC        "U/Min"
#define RESTXT_STAT_V_MAX           "v max "
#define RESTXT_STAT_V_DESC          "km/h"
#define RESTXT_STAT_H_SERV          "Serv."
#define RESTXT_STAT_H_ALL           "Ges."
#define RESTXT_STAT_H_DESC          "h"


/* vehicle state texts;
 * NOTE: if you add one here, also add the VEHICLE_STATE_ value in device.h,
 * as well as an assignment in MonitorDeviceInit() */
                                // ....!....!....!....!.
#define RESTXT_STATE_ALLRIGHT     "     \xbb Alles ok \xab    "
#define RESTXT_STATE_WATERTEMP    " K\xfchlwasser zu heiß! "
#define RESTXT_STATE_OILTEMP      "     \xd6l zu heiß!     "
#define RESTXT_STATE_ENGINE_COLD  " Motor noch zu kalt! "
#define RESTXT_STATE_VOLTAGE_LOW  " Spannung zu niedrig!"
#define RESTXT_STATE_VOLTAGE_HIGH "  Spannung zu hoch!  "
#define RESTXT_STATE_ALTERNATOR   " Ladespg. zu niedrig!"
#define RESTXT_STATE_OILPRESS     " \xd6ldruck zu niedrig! "
#define RESTXT_STATE_OILSWDEF     " \xd6ldrucksch. defekt? "
#define RESTXT_STATE_FUEL8L       "  Unter 8 l. Sprit!  "
#define RESTXT_STATE_FUEL4L       "  Unter 4 l. Sprit!  "
#define RESTXT_STATE_ABS          "     ABS inaktiv!    "
#define RESTXT_STATE_GLACED       "      Glatteis?      "
#define RESTXT_STATE_SERVICEKM    "  Service-Intervall  "



/* settings device strings */
#define RESTXT_SET_WHEELSIZEDESC    "Rad:"
#define RESTXT_SET_WHEELSIZEUNIT    " mm"
#define RESTXT_SET_CCFNOMDESC       "ZKF:"
#define RESTXT_SET_CCFNOMUNIT       "/"
#define RESTXT_SET_DPLLIGHTDESC     "Display A:"
#define RESTXT_SET_DPLHELLGKDESC    "H:"
#define RESTXT_SET_DPLCONTRDESC     "K:"

#define RESTXT_SET_RPMFLASH         "Schaltbl.:"
#define RESTXT_SET_RPMFLASHUNIT     " U/Min"

#define RESTXT_SET_BIKETYPE         "Mrd-Typ:"
#define RESTXT_SET_TRIPCNTFL        "TripCnt:"

#define RESTXT_SET_SERVKM           "Serv. bei:"

#define RESTXT_SET_ERT_SRV          "Serv.:"
#define RESTXT_SET_ERT_ALL          "Ges.:"
#define RESTXT_SET_ERT_UNIT         "h"

#define RESTXT_SET_VEHICKMDESC      "Fzg. ges.:"
#define RESTXT_SET_VEHICKMUNIT      " km"


/* display light strings */
#define RESTXT_DPLLIGHTCHOICE_A " an "
#define RESTXT_DPLLIGHTCHOICE_B " aus"
#define RESTXT_DPLLIGHTCHOICE_c "auto"


/* 'day of week' strings */
#define RESTXT_DOWSHORT_MON     "Mo"
#define RESTXT_DOWSHORT_TUE     "Di"
#define RESTXT_DOWSHORT_WED     "Mi"
#define RESTXT_DOWSHORT_THU     "Do"
#define RESTXT_DOWSHORT_FRI     "Fr"
#define RESTXT_DOWSHORT_SAT     "Sa"
#define RESTXT_DOWSHORT_SUN     "So"

#define RESTXT_DOWLONG_MON      "Montag"
#define RESTXT_DOWLONG_TUE      "Dienstag"
#define RESTXT_DOWLONG_WED      "Mittwoch"
#define RESTXT_DOWLONG_THU      "Donnerstag"
#define RESTXT_DOWLONG_FRI      "Freitag"
#define RESTXT_DOWLONG_SAT      "Samstag"
#define RESTXT_DOWLONG_SUN      "Sonntag"

#define RESTXT_DEC_SEPARATOR 	','
#define RESTXT_THOU_SEPARATOR	'.'

/* time/date format separator */
#define RESTXT_DAYSEPERATOR     "."
#define RESTXT_TIMESEPERATOR    ":"
#define RESTXT_CLKCALIBDESC     "±"

#define RESENUM_HHMM			GERM_HHMM
#define RESENUM_HHMMSS			GERM_HHMMSS

#define RESENUM_DDMMYY			GERM_DDMMYY
#define RESENUM_DDMMYYYY		GERM_DDMMYYYY
#define RESENUM_WWDDMMYY		GERM_WWDDMMYY


#endif /* _RESOURCE_DE_H */

