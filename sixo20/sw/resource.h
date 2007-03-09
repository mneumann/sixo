/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-24 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         devices
 *  Purpose:        list of all dialog resources (strings)
 *  Comments:       use this file for all language specific stuff
 *
 *                  REPLACE THIS FILE TO LANGUAGE SPECIFIC VERSION!!
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
 * Revision 2.2  2007/03/09 08:38:36  niezeithat
 * - Added additional Motorbike Version, Africatwin RD07, no Fule Level Sensors!
 *
 * Revision 2.1  2006/07/20 23:01:31  tuberkel
 * - added F650 version
 * - added ABS warning
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.14  2006/02/18 14:23:17  Ralf
 * - adaption to BIKE_MOTOBAU
 * - settings screen changed
 * - lapcounter and statistics screen new
 *
 ************************************************************************ */



#ifndef _RESOURCE_H
#define _RESOURCE_H


/* intro device bike version strings
   7 characters for up to 15 bikes  "1234567" */
#define RESTXT_BIKESTRING_DEFAULT   "STANDARD"
#define RESTXT_BIKESTRING_R100GS    "R100GS"
#define RESTXT_BIKESTRING_F650      "F650"
#define RESTXT_BIKESTRING_HUSQV     "HUSQV"
#define RESTXT_BIKESTRING_BAGHIRA   "BAGHIRA"
#define RESTXT_BIKESTRING_AT        "AT"
#define RESTXT_BIKESTRING_ATRD07    "AT RD07"
#define RESTXT_BIKESTRING_R1100GS   "1100GS"
#define RESTXT_BIKESTRING_KTM       "KTM"
#define RESTXT_BIKESTRING_HUSQVRS   "HUSQVRS"
#define RESTXT_BIKESTRING_MOTOBAU   "MOTOBAU"


/* analog input formating*/
#define RESTXT_NOTAVAIL     "  -"  // 'not available'
#define RESTXT_NOTCONN      "  -"  // 'not connected'


/* intro special sw version strings
   1 character, might be concated  */
#define RESTXT_SWVER_DEBUG     "D"  // 'D' for DebugOuts on Uart0
#define RESTXT_SWVER_MINIEMU   "E"  // 'E' for Emulator KD30
#define RESTXT_SWVER_VEHICSIM  "S"  // 'S' for Simulation of Vehicle
#define RESTXT_SWVER_HARDCOPY  "H"  // 'H' for Hardcopy on Debug-Uart via 'HighBeam'-switch


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
#define MON_TXT_LEN  22         /* len of strings in monitor screen */
#define RESTXT_MON_TEMP_DEV        "Ger\xe4t   "
#define RESTXT_MON_TEMP_AIR        "Luft    "
#define RESTXT_MON_BATT            "Batt.   "
#define RESTXT_MON_TEMP_WAT        "Wasser  "
#define RESTXT_MON_TEMP_OIL        "\xd6l      "
#define RESTXT_MON_VOLTAGE         "Batt.   "


/* statistic device resources */
#define STAT_TXT_LEN  22      /* len of strings in statistic screen */
#define RESTXT_STAT_BATT            "Batterie:"
#define RESTXT_STAT_BATT_DESC       "V"
#define RESTXT_STAT_TEMP_AIR        "Luft:"
#define RESTXT_STAT_TEMP_OIL        "\xd6l:  "
#define RESTXT_STAT_TEMP_WAT        "H2O: "
#define RESTXT_STAT_TEMP_DESC       "°C"
#define RESTXT_STAT_RPM_MAX         "DRZ max:"
#define RESTXT_STAT_RPM_DESC        "U/Min"
#define RESTXT_STAT_V_MAX           "v max:"
#define RESTXT_STAT_V_DESC          "km/h"
#define RESTXT_STAT_H_SERV          "Serv."
#define RESTXT_STAT_H_ALL           "Ges."
#define RESTXT_STAT_H_DESC          "h"






/* vehicle state texts;
 * NOTE: if you add one here, also add the VEHICLE_STATE_ value in device.h,
 * as well as an assignment in MonitorDeviceInit() */
#define STATE_TEXT_LEN        21  /* maximum characters printable in status line, excl. null terminator */
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

/* time/date format seperator */
#define RESTXT_DAYSEPERATOR     "."
#define RESTXT_TIMESEPERATOR    ":"
#define RESTXT_CLKCALIBDESC     "±"


/* HW-Test device ressources
   Should look like this:

   4x6 font
   +....!....!....!....!....!....!..+
   |V+   16.1V _   EEPROM  _  BAT  _|
   |ALTW  4.3V _   NVRAM   _  RTC  _|
   |RPM   1250 _   UART 01__  HWID _|
   |WHEEL  120 _                    |
   |TWAT  50'C _   TURNLR __  OIL  _|
   |TAIR  50'C _   HBEAM   _  NEUTR_|
   |TOIL  50'C _   GPI 0123 ____    |
   |TDEV  23'C _   KEY 012  ___  UDO|
   +....!....!....!....!....!....!..+

Note: We use a single string per complete screen,
      to reduce overhead for every test item.
      Test items and results will be inserted
      separately as objects.
Note: We use the Text-Object property to automatically
      wrap aroung strings longer than 1 character line :-) */

#define RESTXT_HWTDEV_SCREEN_32x8   "V+   xx.xV _   EEPROM  _  BAT  _"  \
                                    "ALTW xx.xV _   NVRAM   _  RTC  _"  \
                                    "RPM  xxxxx _   UART01 __  HWID _"  \
                                    "KM/H   xxx _   LDR xxx _  WHEEL_"  \
                                    "TWAT  xx'C _   TURNLR __  OIL  _"  \
                                    "TAIR  xx'C _   HBEAM   _  NEUTR_"  \
                                    "TOIL  xx'C _   GPI 0123 ____    "  \
                                    "TDEV  xx'C _   KEY 012  ___  UDO"





/* test resources */
#define RESTXT_TEST1    "Eingabetest"
#define RESTXT_TEST2    "12340"
#define RESTXT_TEST3    "BMW"
                      /*!....!....!....!....!.*/
#define RESTXT_TEST4    "Hey Mann, das ist ein" \
                        "sehr langer Text, der" \
                        "aus dem ROM kommt und" \
                        "deshalb das RAM kaum " \
                        "belastet. Trotzdem   " \
                        "sollte er in das def-" \
                        "inierte Fenster pas- " \
                        "sen, allerding etwas " \
                        "wirkürlich umgebroch-" \
                        "en."
                      /*!....!....!....!....!....!....!..*/
#define RESTXT_TEST5    "Hey Mann, das ist ein sehr lan- " \
                        "ger Text, der aus dem ROM kommt " \
                        "und deshalb das RAM kaum belas- " \
                        "tet. Trotzdem sollte er in das  " \
                        "definierte Fenster passen, aller" \
                        "ding etwas willkürlich umgebroch" \
                        "en."
                      /*!....!....!....!.*/
#define RESTXT_TEST6    "  Hey Mann, das " \
                        "  ist ein sehr  " \
                        "  langer Text!  "


#endif /* _RESOURCE_H */

