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
 * Revision 3.19  2012/05/28 12:47:31  tuberkel
 * Corrections for renamed Eeprom/Nvram Variables
 *
 * Revision 3.18  2012/05/27 17:52:40  tuberkel
 * Corrections for renamed Eeprom/Nvram Variables
 *
 * Revision 3.17  2012/05/27 16:01:42  tuberkel
 * All Eeprom/Nvram Variables renamed
 *
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
/* monitor device resources */
/* =============================================== */
#define RESTXT_MON_TEMP_DEV        "Ger\xe4t   "            // Device
#define RESTXT_MON_TEMP_AIR        "Luft    "               // Air
#define RESTXT_MON_BATT            "Batt.   "               // Battery
#define RESTXT_MON_TEMP_WAT        "Wasser  "               // Water
#define RESTXT_MON_TEMP_OIL        "\xd6l      "            // Oil


/* =============================================== */
/* statistic device resources */
/* =============================================== */
#define RESTXT_STAT_BATT            "Batterie "                 // Battery
#define RESTXT_STAT_TEMP_AIR        "Luft "                     // Air
#define RESTXT_STAT_TEMP_OIL        "\xd6l   "                  // Oil
#define RESTXT_STAT_TEMP_WAT        "H2O  "                     // Water
#define RESTXT_STAT_RPMMAX_DESC     "DRZ max "                  // RPM max
#define RESTXT_STAT_RPMMAX_UNIT     "U/Min"                     // Engine Revolutions 
#define RESTXT_STAT_VMAX_DESC       "v max "                    // Speed max
#define RESTXT_STAT_H_SERV          "Serv."                     // Service
#define RESTXT_STAT_H_ALL           "Ges."                      // Overall


/* =============================================== */
/* vehicle/device parameter state texts;
 * NOTE: See 'SURVP_ID' and 'szSurvParamDesc[]' for usage!
 * NOTE: Only 17 of 21 chars available (e.g.: "1/1    <8 l. Sprit!  " */
 /* =============================================== */
                                //  +....!....!....!..+..+..+
#define RESTXT_STATE_ALLRIGHT       "   \xbb Alles ok \xab  "   // 00 - special case: 21 char len
                                                                
#define RESTXT_STATE_BATT_HIGH      "  Spg. zu hoch!  "         // Voltage too high
#define RESTXT_STATE_OILPRESS       "    \xd6ldruck!     "      // Oilpressure
#define RESTXT_STATE_ABS            "   ABS inaktiv!  "         // ABS inactive
#define RESTXT_STATE_OILTEMP        "   \xd6l zu heiß!   "      // Oil too hot
#define RESTXT_STATE_WATTEMPSW      " K\xfchlw. zu heiß! "      // Water too hot (detected by switch)
#define RESTXT_STATE_WATERTEMP      " K\xfchlw. zu heiß! "      // Water too hot (detected by sensor)
#define RESTXT_STATE_FUEL8L         "  < 8 l. Sprit!  "         // less than 8l fuel
#define RESTXT_STATE_FUEL4L         "  < 4 l. Sprit!  "         // less than 4l fuel
#define RESTXT_STATE_ENGINE_COLD    "   Motor kalt!   "         // Engine cold
#define RESTXT_STATE_BATT_LOW       "Spg. zu niedrig! "         // Voltage too low
#define RESTXT_STATE_ALTERNATOR     "Ladespg. niedrig!"         // Alternator Voltage too low
#define RESTXT_STATE_OILSWDEF       "\xd6ldrucksch. def.?"      // Oilpressure switch defect?
#define RESTXT_STATE_GLACED         "    Glatteis?    "         // Glaced?
#define RESTXT_STATE_SERVICEKM      "   km-Service!   "         // km-Service-Intervall
#define RESTXT_STATE_SERVICEHOUR    "    h-Service!   "         // h-Service-Intervall
#define RESTXT_STATE_DLS_SUMMER     "Sommerzeit aktiv!"         // Daylight Saving active
#define RESTXT_STATE_DLS_WINTER     "Winterzeit aktiv!"         // Daylight Saving NOT active
#define RESTXT_STATE_RTC_BATT       "RTC-Batt. defekt?"         // SIxO internal RealTimeClock defect?
#define RESTXT_STATE_COOLRIDE       "Heizgriffe aktiv!"         // Heatgrips active
#define RESTXT_STATE_SIMULATION     "Simulation aktiv!"         // SIxO vehicle Simulation active
#define RESTXT_STATE_HARDCOPY       " Hardcopy aktiv! "         // SIxO Screendump active 




/* =============================================== */
/* settings device strings */
/* =============================================== */

// ---------------------------------------------------
// headline strings                  ....!....!....!....!.
#define RESTXT_SET_HL_VEHICLE       ">> FAHRZEUG         \x8c"          // VEHICLE      (Arrow down)
#define RESTXT_SET_HL_DEVICE        ">> GER\xc4T           \x8c\x8b"    // DEVICE       (Arrow down/up)
#define RESTXT_SET_HL_DISPLAY       ">> LCD/LED         \x8c\x8b"       // LCD/LED      (Arrow down/up)
#define RESTXT_SET_HL_EXTENS1       ">> ERWEITERUNGEN    \x8b"          // EXTENSIONS   (Arrow up)
#define RESTXT_SET_HL_IOPORTS       ">> ANSCHL\xdcSSE        "          // I/O-PORTS    (Arrow up)
#define RESTXT_SET_HL_WARNINGS      ">> WARNUNGEN         "             // WARNINGS     

// ---------------------------------------------------
// Setup Vehicle Screen              ....!....!....!....!.
#define RESTXT_SET_BIKE_DESC        "Typ:"              // Bike Type
#define RESTXT_SET_LOGO_DESC        "Logo:"             // Logo
                                                        
#define RESTXT_SET_CCFNOM_DESC      "ZKF:"              // Cylinder Correctur Factor
#define RESTXT_SET_CCFNOM_UNIT      "/"                 // divider
                                                        
#define RESTXT_SET_WHEELSIZE_DESC   "Rad:"              // Wheelsize
#define RESTXT_SET_WHEELIMP_DESC    "I/U:"              // Impulses per Revolution
                                                        
#define RESTXT_SET_VEHICKM_DESC     "Fzg:"              // Overall Vehicle distance 
#define RESTXT_SET_SERVKM_DESC      "Srv:"              // Next service distance 
#define RESTXT_SET_TANKCAP_DESC     "Benzin:"           // Fuel

// ---------------------------------------------------
// Setup Device Screen               ....!....!....!....!.
#define RESTXT_SET_RTC_TIME         "Zeit:"             // time
#define RESTXT_SET_RTC_DATE         "Datum:"            // date
#define RESTXT_SET_RTC_CALIB        "K:±"               // Clock Correctur factor
#define RESTXT_SET_RTC_DLS          "Smz:"              // Daylight Saving
                                                        
#define RESTXT_SET_TRIP_DESC        "TripC-kurz:"       // TricpCounter (Short-Distance)
#define RESTXT_SET_METRIC_DESC      "Metrik:"           // Metrics
#define RESTXT_SET_LEDWM_DESC       "WarnModus:"        // LED Warning Mode
#define RESTXT_SET_VEHICSIM         "Sim:"              // Vehicle Simulation 
#define RESTXT_SET_HARDCOPY         "Bmp:"              // Hardcopy (Bitmap)
#define RESTXT_SET_LANG_DESC        "Sprache:"          // Language
#define RESTXT_SET_DBGDESC          "DbgOut:"           // DebugOuts Filter
#define RESTXT_SET_SW_VERS          "SW:"               // Software Version
#define RESTXT_SET_BEEP_ON          "Sum:"              // Beeper 
#define RESTXT_SET_RESETEEPROM      "Def:"              // Reset Eeprom to Defaults

// ---------------------------------------------------
// Setup LCD/LED Screen              ....!....!....!....!.
#define RESTXT_SET_LCD              "LCD:"              // Display settings
#define RESTXT_SET_LED              "LED:"              // LED Settings
#define RESTXT_SET_LCD_BL_DESC      "Lichtschwelle:"    // BacklightOn Threashold
#define RESTXT_SET_LCD_BR_DESC      "Helligkeit:"       // Backlight Luminance
#define RESTXT_SET_LCD_CNT_DESC     "Kontrast:"         // Display Contrast
#define RESTXT_SET_LED_DIM_DESC     "Helligkeit:"       // LED Luminance
#define RESTXT_SET_RPMFL_DESC       "Schaltbl.:"        // RPM Flashlight
#define RESTXT_SET_RPMFL_UNIT       "U/Min"             // Engine Revolutions per Minute


// ---------------------------------------------------
// Extensions 1 Screen              ....!....!....!....!.
#define RESTXT_SET_COMPASS          "Kompass"           // Compass Module                                                       
#define RESTXT_SET_COMPD_DESC       "A:"                // Compass Display Mode (3 chars, right aligned!)
#define RESTXT_SET_COMPD_NA         " nv"               // do not show any compass value
#define RESTXT_SET_COMPD_HD         "K  "               // show Course only  in TripCounter
#define RESTXT_SET_COMPD_GR         "  G"               // show Graph  only  in TripCounter
#define RESTXT_SET_COMPD_HDGR       "K+G"               // show Course+Graph in TripCounter                                                       
#define RESTXT_SET_COMPC_DESC       "K:"                // Compass Calibration State (3 chars, right aligned!)
#define RESTXT_SET_COMPC_NA         " nv"               // Calibration not possible 
#define RESTXT_SET_COMPC_WORK       "akt"               // Compass not in calibration mode
#define RESTXT_SET_COMPC_HHOLD      "H-H"               // Module horizontal hold
#define RESTXT_SET_COMPC_HTURN      "H-D"               // Module horizontal turn
#define RESTXT_SET_COMPC_HSAVE      "H-S"               // Module horizontal save
#define RESTXT_SET_COMPC_VHOLD      "V-H"               // Module vertical   hold
#define RESTXT_SET_COMPC_VTURN      "V-D"               // Module vertical   turn
#define RESTXT_SET_COMPC_VSAVE      "V-S"               // Module vertical   save                                                        
#define RESTXT_SET_FUELSENSOR       "Tanks."            // Fuelsensor 
#define RESTXT_SET_FUELS_IN         "I:"                // Fuelsensor Input (GPI 0..3)
#define RESTXT_SET_FUELS_IMP        "I/l"               // Fuelsensor Impulserate (Impulses/Liter)                                                        
#define RESTXT_SET_CHAINOILER       "Kette"             // Chain Oiler verfügbar
#define RESTXT_SET_GEARINFO         "Ganganzeige"       // Gearbox Info
#define RESTXT_SET_GEARIM_DESC      "M:"                // Gearbox Display Mode
#define RESTXT_SET_GEARIM_NA        "  nv"              // Gearbox not available
#define RESTXT_SET_GEARIM_AUTO      "auto"              // Automatic Mode (self calibrating)
#define RESTXT_SET_GEARIM_GPI       " GPI"              // GPI Input Mode (needs 3 GPIs)
#define RESTXT_SET_GPS              "GPS-Maus"          // GPS Mouse



/* =============================================== */
/* time/date strings */
/* =============================================== */

/* 'day of week' strings */
#define RESTXT_DOWSHORT_MON     "Mo"                    // days of week (short)
#define RESTXT_DOWSHORT_TUE     "Di"
#define RESTXT_DOWSHORT_WED     "Mi"
#define RESTXT_DOWSHORT_THU     "Do"
#define RESTXT_DOWSHORT_FRI     "Fr"
#define RESTXT_DOWSHORT_SAT     "Sa"
#define RESTXT_DOWSHORT_SUN     "So"

#define RESTXT_DOWLONG_MON      "Montag"                // days of week (long)
#define RESTXT_DOWLONG_TUE      "Dienstag"
#define RESTXT_DOWLONG_WED      "Mittwoch"
#define RESTXT_DOWLONG_THU      "Donnerstag"
#define RESTXT_DOWLONG_FRI      "Freitag"
#define RESTXT_DOWLONG_SAT      "Samstag"
#define RESTXT_DOWLONG_SUN      "Sonntag"

#define RESTXT_DEC_SEPARATOR 	','                     // Decimal separator (e.g. '3.1 Liter')
#define RESTXT_1K_SEPARATOR	    '.'                     // Thousends Separator (e.g. '4.500 km')

/* time/date format separator */
#define RESTXT_DAYSEPERATOR     "."                     // Day/Month/Year Separator 
#define RESTXT_TIMESEPERATOR    ":"                     // Hour/Minunte/Second Separator 

#define RESENUM_HHMM	GERM_HHMM                       // Time display Mode for Hour+Minute
#define RESENUM_HHMMSS	GERM_HHMMSS                     // Time display Mode for Hour+Minute+Second

#define RESENUM_DDMMYY	    GERM_DDMMYY                 // date display mode for Day+Month+Year(short)
#define RESENUM_DDMMYYYY	GERM_DDMMYYYY               // date display mode for Day+Month+Year(long)
#define RESENUM_WWDDMMYY	GERM_WWDDMMYY               // date display mode for DayOfWeek+Day+Month+Year


#endif /* _RESOURCE_DE_H */

