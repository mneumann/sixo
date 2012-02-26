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
 *      Arnold Neugebauer                   Ralf Schwarzer
 *      Stöckener Str. 115                  Hudeplan 37 A
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
 * Revision 3.16  2012/02/26 10:26:57  tuberkel
 * Update
 *
 * Revision 3.15  2012/02/26 10:11:52  tuberkel
 * - All GearInfo Settings available
 *
 * Revision 3.14  2012/02/25 20:43:01  tuberkel
 * - All ChainOiler Settings available
 *
 * Revision 3.12  2012/02/25 17:25:05  tuberkel
 * All Coolride Settings available
 *
 * Revision 3.11  2012/02/25 16:48:30  tuberkel
 * 5 new Objects in ExtensionsScreen
 *
 * Revision 3.10  2012/02/21 22:01:56  tuberkel
 * Compass Control/Mode/Eeprom reveiwed/fixed
 *
 * Revision 3.9  2012/02/19 21:04:51  tuberkel
 * - Extensions-Screen reviewed
 * - Compass-Settings-Strings reduced
 *
 * Revision 3.8  2012/02/19 11:23:13  tuberkel
 * BugFix BOOL Obj - R-mode-Left aligned
 *
 * Revision 3.7  2012/02/14 21:08:03  tuberkel
 * - #define COMPASS ==> COMPDRV
 * - Compass SystemParam moved from devFlags2 -> 3
 * - Settings 'Compass' ==> 'Extensions'
 * - all Compass-Display modules enabled by default
 *
 * Revision 3.6  2012/02/11 00:06:42  tuberkel
 * - HeatGrip Icon shown in upper area
 *
 * Revision 3.5  2012/02/10 23:45:22  tuberkel
 * - Survelannce HeatGrip <Info> - if active
 * - Surveillance-API reviewed
 *
 * Revision 3.4  2012/02/06 20:54:14  tuberkel
 * Just renamed all 'Devices' function prefixes for better readability
 *
 * Revision 3.3  2012/01/21 19:52:49  tuberkel
 * Additional Logos for 'TeneristI and 'Coolride'
 *
 * Revision 3.2  2012/01/14 08:28:42  tuberkel
 * Message-IDs shortened / reviewed
 *
 * Revision 3.1  2011/05/29 20:58:28  tuberkel
 * Settings Header corrected
 *
 * Revision 3.0  2010/11/07 09:07:02  tuberkel
 * V30 Preparations - no changes
 *
 * Revision 2.4  2010/11/07 09:05:59  tuberkel
 * V30 Preparations:
 * - diverse new textblocks for changed Settings/Appearance
 *
 * Revision 2.3  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.2  2009/06/21 17:58:09  tuberkel
 * New: RESTXT_SET_COMPCAL
 *
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

/* =============================================== */
/* main device strings */
/* =============================================== */
#define RESTXT_SPEED_DESC       "km/h"
#define RESTXT_SPEEDA_DESC      "km"
#define RESTXT_SPEEDB_DESC      "h"
#define RESTXT_RPM_DESC         "/min"
#define RESTXT_DIST_DESC        "km"
#define RESTXT_DEGC_DESC        "°C"
#define RESTXT_VOLT_DESC        "V"
#define RESTXT_TRIP1DESC        "T1"
#define RESTXT_TRIP2DESC        "T2"
#define RESTXT_SPEEDMAX_DESC    "max"
#define RESTXT_SPEEDMAX_UNIT    "km/h"


/* =============================================== */
/* monitor device resources */
/* =============================================== */
#define RESTXT_MON_TEMP_DEV        "Ger\xe4t   "
#define RESTXT_MON_TEMP_AIR        "Luft    "
#define RESTXT_MON_BATT            "Batt.   "
#define RESTXT_MON_TEMP_WAT        "Wasser  "
#define RESTXT_MON_TEMP_OIL        "\xd6l      "
#define RESTXT_MON_VOLTAGE         "Batt.   "

/* =============================================== */
/* statistic device resources */
/* =============================================== */
#define RESTXT_STAT_BATT            "Batterie "
#define RESTXT_STAT_BATT_DESC       "V"
#define RESTXT_STAT_TEMP_AIR        "Luft "
#define RESTXT_STAT_TEMP_OIL        "\xd6l   "
#define RESTXT_STAT_TEMP_WAT        "H2O  "
#define RESTXT_STAT_TEMP_DESC       "°C"
#define RESTXT_STAT_RPMMAX_DESC     "DRZ max "
#define RESTXT_STAT_RPMMAX_UNIT     "U/Min"
#define RESTXT_STAT_VMAX_DESC       "v max "
#define RESTXT_STAT_VMAX_UNIT       "km/h"
#define RESTXT_STAT_H_SERV          "Serv."
#define RESTXT_STAT_H_ALL           "Ges."
#define RESTXT_STAT_H_DESC          "h"


/* =============================================== */
/* vehicle/device parameter state texts;
 * NOTE: if you add one here, also add the eSURVID_ value in device.h,
 * as well as an assignment in MonDev_Init()
 *  Note: (max 4+17 chars),
 *  E.g.: "1/1    <8 l. Sprit!  */
 /* =============================================== */
                                //+....!....!....!..+..+..+
#define RESTXT_STATE_ALLRIGHT     "   \xbb Alles ok \xab  "      // 00 - special case: 21 char len

#define RESTXT_STATE_SURV_BATT_HIGH "  Spg. zu hoch!  "       //
#define RESTXT_STATE_OILPRESS     "    \xd6ldruck!     "    //
#define RESTXT_STATE_ABS          "   ABS inaktiv!  "       //
#define RESTXT_STATE_OILTEMP      "   \xd6l zu heiß!   "    //
#define RESTXT_STATE_WATTEMPSW    " K\xfchlw. zu heiß! "    // Switch
#define RESTXT_STATE_WATERTEMP    " K\xfchlw. zu heiß! "    // Sensor
#define RESTXT_STATE_FUEL8L       "  < 8 l. Sprit!  "       //
#define RESTXT_STATE_FUEL4L       "  < 4 l. Sprit!  "       //
#define RESTXT_STATE_ENGINE_COLD  "   Motor kalt!   "       //
#define RESTXT_STATE_SURV_BATT_LOW  "Spg. zu niedrig! "       //
#define RESTXT_STATE_ALTERNATOR   "Ladespg. niedrig!"       //
#define RESTXT_STATE_OILSWDEF     "\xd6ldrucksch. def.?"    //
#define RESTXT_STATE_GLACED       "    Glatteis?    "       //
#define RESTXT_STATE_SERVICEKM    "   km-Service!   "       //
#define RESTXT_STATE_SERVICEHOUR  "    h-Service!   "       //
#define RESTXT_STATE_DLS_SUMMER   "Sommerzeit aktiv!"       //
#define RESTXT_STATE_DLS_WINTER   "Winterzeit aktiv!"       //
#define RESTXT_STATE_RTC_BATT     "RTC-Batt. defekt?"       //
#define RESTXT_STATE_COOLRIDE     "Heizgriffe aktiv!"       //
#define RESTXT_STATE_SIMULATION   "Simulation aktiv!"       //
#define RESTXT_STATE_HARDCOPY     " Hardcopy aktiv! "       //




/* =============================================== */
/* settings device strings */
/* =============================================== */

// ---------------------------------------------------
// headline strings                  ....!....!....!....!.
#define RESTXT_SET_HL_VEHICLE       ">> FAHRZEUG         \x8c"          // Arrow down
#define RESTXT_SET_HL_DEVICE        ">> GER\xc4T           \x8c\x8b"    // Arrow down/up
#define RESTXT_SET_HL_DISPLAY       ">> LCD/LED         \x8c\x8b"       // Arrow down/up
#define RESTXT_SET_HL_EXTENS1       ">> ERWEITERUNGEN    \x8b"          // Arrow up
#define RESTXT_SET_HL_IOPORTS       ">> ANSCHL\xdcSSE        "          // Arrow up
#define RESTXT_SET_HL_WARNINGS      ">> WARNUNGEN         "

// ---------------------------------------------------
// Setup Vehicle Screen              ....!....!....!....!.
#define RESTXT_SET_BIKE_DESC        "Typ:"
#define RESTXT_SET_LOGO_DESC        "Logo:"

#define RESTXT_SET_CCFNOM_DESC      "ZKF:"
#define RESTXT_SET_CCFNOM_UNIT      "/"

#define RESTXT_SET_WHEELSIZE_DESC   "Rad:"
#define RESTXT_SET_WHEELSIZE_UNIT   "mm"
#define RESTXT_SET_WHEELIMP_DESC    "I/U:"

#define RESTXT_SET_VEHICKM_DESC     "Fzg:"
#define RESTXT_SET_VEHICKM_UNIT     "km"
#define RESTXT_SET_SERVKM_DESC      "Srv:"
#define RESTXT_SET_SERVKM_UNIT      "km"
#define RESTXT_SET_ERT_UNIT         "h"
#define RESTXT_SET_TANKCAP_DESC     "Benzin:"
#define RESTXT_SET_TANKCAP_UNIT     "l"
#define RESTXT_SET_FUELCONS_DESC    ""
#define RESTXT_SET_FUELCONS_UNIT    "l/h"

// ---------------------------------------------------
// Setup Device Screen               ....!....!....!....!.
#define RESTXT_SET_RTC_TIME         "Zeit:"
#define RESTXT_SET_RTC_DATE         "Datum:"
#define RESTXT_SET_RTC_CALIB        "K:±"
#define RESTXT_SET_RTC_DLS          "Smz:"

#define RESTXT_SET_TRIP_DESC        "TripC-kurz:"
#define RESTXT_SET_METRIC_DESC      "Metrik:"
#define RESTXT_SET_LEDWM_DESC       "WarnModus:"
#define RESTXT_SET_VEHICSIM         "Sim:"
#define RESTXT_SET_LOGODELAY_UNIT   "s"
#define RESTXT_SET_HARDCOPY         "Bmp:"
#define RESTXT_SET_LANG_DESC        "Sprache:"
#define RESTXT_DBGOUTDESCR          "DbgOut:"
#define RESTXT_SET_SW_VERS          "SW:"
#define RESTXT_SET_BEEP_ON          "Sum:"
#define RESTXT_SET_RESETEEPROM      "Def:"


// ---------------------------------------------------
// Appearance/output Screen          ....!....!....!....!.
#define RESTXT_SET_LCD              "LCD:"
#define RESTXT_SET_LED              "LED:"
#define RESTXT_SET_LCD_BL_DESC      "Lichtschwelle:"
#define RESTXT_SET_LCD_BR_DESC      "Helligkeit:"
#define RESTXT_SET_LCD_CNT_DESC     "Kontrast:"
#define RESTXT_SET_LED_DIM_DESC     "Helligkeit:"
#define RESTXT_SET_RPMFL_DESC       "Schaltbl.:"
#define RESTXT_SET_RPMFL_UNIT       "U/Min"


/* =============================================== */
// Extensions 1 Screen              ....!....!....!....!.
/* =============================================== */
// Compass objects
#define RESTXT_SET_COMPASS          "Kompass"

#define RESTXT_SET_COMPD_DESC       "A:"    // Kompass Anzeige Mode (3 chars, right aligned!)
#define RESTXT_SET_COMPD_NA         " nv"   // nicht verfuegbar
#define RESTXT_SET_COMPD_HD         "K  "   // nur Kurs im TripCounter
#define RESTXT_SET_COMPD_GR         "  G"   // nur Graph im TripCounter
#define RESTXT_SET_COMPD_HDGR       "K+G"   // Kurs + Graph im TripCounter

#define RESTXT_SET_COMPC_DESC       "K:"    // Kompass Kalibriering (3 chars, right aligned!)
#define RESTXT_SET_COMPC_NA         " nv"   // Modul nicht verfügbar
#define RESTXT_SET_COMPC_WORK       "akt"   // Modul aktiv (nicht kalibrierbar)
#define RESTXT_SET_COMPC_HHOLD      "H-H"   // Modul horizontal halten
#define RESTXT_SET_COMPC_HTURN      "H-D"   // Modul horizontal drehen
#define RESTXT_SET_COMPC_HSAVE      "H-S"   // Modul horizontal sichern
#define RESTXT_SET_COMPC_VHOLD      "V-H"   // Modul vertikal   halten
#define RESTXT_SET_COMPC_VTURN      "V-D"   // Modul vertikal   drehen
#define RESTXT_SET_COMPC_VSAVE      "V-S"   // Modul vertikal   sichern

#define RESTXT_SET_COOLRIDE         "Coolride"  // Coolride Modul verfügbar
#define RESTXT_SET_COOLR_IN         "I:"        // Mess-Eingang:   GPI 0..3
#define RESTXT_SET_COOLR_OUT        "O:"        // Tasten-Ausgang: GPO 0..1

#define RESTXT_SET_FUELSENSOR       "Tanks."    // Tanksensor Modul verfügbar
#define RESTXT_SET_FUELS_IN         "I:"        // GebMess-Eingang:   GPI 0..3
#define RESTXT_SET_FUELS_IMP        "I/l"       // Sensordaten: Impulse/Liter

#define RESTXT_SET_CHAINOILER       "Kette"     // Kettenöler Module verfügbar
#define RESTXT_SET_CHAINO_DUR       "s"         // Aktivität für n sec.
#define RESTXT_SET_CHAINO_KM        "km"        // Aktivität alle n km
#define RESTXT_SET_CHAINO_OUT       "O:"        // Kettenölerausgang: GPO 0..1

#define RESTXT_SET_GEARINFO         "Ganganzeige"
#define RESTXT_SET_GEARIM_DESC      "M:"
#define RESTXT_SET_GEARIM_NA        "  nv"
#define RESTXT_SET_GEARIM_AUTO      "auto"
#define RESTXT_SET_GEARIM_GPI       " GPI"

#define RESTXT_SET_GPS              "GPS-Maus"





/* =============================================== */
/* time/date strings */
/* =============================================== */

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

#define RESENUM_HHMM	GERM_HHMM
#define RESENUM_HHMMSS	GERM_HHMMSS

#define RESENUM_DDMMYY	    GERM_DDMMYY
#define RESENUM_DDMMYYYY	GERM_DDMMYYYY
#define RESENUM_WWDDMMYY	GERM_WWDDMMYY


#endif /* _RESOURCE_DE_H */

