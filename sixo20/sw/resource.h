/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-24 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         devices
 *  Purpose:        list of all language _unspecific_ strings and formats
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
 * Revision 3.2  2012/01/14 08:28:42  tuberkel
 * Message-IDs shortened / reviewed
 *
 * Revision 3.1  2011/05/29 20:57:48  tuberkel
 * RESTXT_SWVER_xxx extented
 *
 * Revision 3.0  2010/11/07 09:36:43  tuberkel
 * V30 Preparations:
 * - diverse new Textblocks for changed Settings/Appearance
 * - Logo Selection extended
 * - BikeType Selection new
 *
 * Revision 2.8  2009/07/19 12:34:23  tuberkel
 * Choice text samples changed
 *
 * Revision 2.7  2009/07/18 06:28:19  tuberkel
 * Test-Ressources for new SelectObject
 *
 * Revision 2.6  2009/07/15 08:57:04  tuberkel
 * - new TESTSCREEN texts
 * - language settings renamed
 *
 * Revision 2.5  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.4  2009/06/21 17:57:13  tuberkel
 * Changes done by AN:
 * Compass Version uses add. 'C' indicator
 *
 * Revision 2.3  2007/03/30 09:56:28  tuberkel
 * - Added language specific resource_x.h files
 * - controled via #define LANG=0/1/2 for DE/EN/NL
 *
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



/****************************************  L A N G U A G E S  ****************************************/
#define LANG_GERMAN		    0
#define LANG_ENGLISH_UK	    1
#define LANG_NETHERLANDS	2


// set your language here
#ifndef LANG                        // may be controlled via project editor
#define LANG		LANG_GERMAN		// default: german
#endif

/*******************************************  G E R M A N  *******************************************/
#if LANG == LANG_GERMAN
#include "resource_de.h"


/************************************** E N G L I S H   U K  ***************************************/
#elif LANG == LANG_ENGLISH_UK
#include "resource_en_uk.h"


/************************************** N E T H E R L A N D S ***************************************/
#elif LANG == LANG_NETHERLANDS
#include "resource_nl.h"


/****************************************************************************************************/
#else
#error Undefined Language!
#endif

/******************************************** E N D   L A N G U A G E S  ******************************/








/************************************* Language unspecific stuff ********************************/


/* buffer sizes */
#define MON_TXT_LEN         22      /* len of strings in monitor screen, incl. null terminator */
#define STAT_TXT_LEN        22      /* len of strings in statistic screen, incl. null terminator */
#define VEHSTATE_TXT_LEN    22      /* len of vehicle status string, incl. null terminator */

#define RESTXT_EMPTY_TXT          ""            /* max width of below strings in chars */

/* device bike version strings */
/* NOTE: This list muts be identical to 'BIKE_TYPE' !!! */
#define RESTXT_SET_BIKE_WIDTH      8            /* max width of below strings in chars */
#define RESTXT_SET_BIKE_CNT        5            /* number of bikes types */
                               /* +----.---+ */
#define RESTXT_SET_BIKE_DEFAULT   "Standard"
#define RESTXT_SET_BIKE_R100GS    "  R100GS"
#define RESTXT_SET_BIKE_F650      "    F650"
#define RESTXT_SET_BIKE_BAGHIRA   " Baghira"
#define RESTXT_SET_BIKE_AT        "      AT"


/* device bike version strings */
/* NOTE: This list muts be identical to 'LOGO_TYPE' !!! */
#define RESTXT_SET_LOGO_WIDTH       7           /* max width of below strings in chars */
#define RESTXT_SET_LOGO_CNT         12          /* number of logos */
                                 /* +----.--+ */
#define RESTXT_SET_LOGO_NKDEV       "    N&K"
#define RESTXT_SET_LOGO_BMW         "    BMW"
#define RESTXT_SET_LOGO_HUSQV       "   Husq"
#define RESTXT_SET_LOGO_BAGHIRA     "    MuZ"
#define RESTXT_SET_LOGO_SIXO        "   SIxO"
#define RESTXT_SET_LOGO_AFRICATWIN  "     AT"
#define RESTXT_SET_LOGO_BMW_1100GS  "R1100GS"
#define RESTXT_SET_LOGO_KTM         "    KTM"
#define RESTXT_SET_LOGO_HUSQVRS     " HusqRS"
#define RESTXT_SET_LOGO_HPN         "    HPN"
#define RESTXT_SET_LOGO_MOTOBAU     "Motobau"
#define RESTXT_SET_LOGO_VFORCE      " VForce"


/* tripcounter display mode flag */
#define RESTXT_SET_TRIP_WIDTH       1       /* max width of below strings in chars */
#define RESTXT_SET_TRIP_CNT         2       /* number of values: top/bottom */
#define RESTXT_SET_TRIP_VSB    "\x8c"       /* Arrow Top */
#define RESTXT_SET_TRIP_VST    "\x8b"       /* Arrow Bottom */

/* Metric mode flag */
#define RESTXT_SET_METRIC_WIDTH     5       /* max width of below strings in chars */
#define RESTXT_SET_METRIC_CNT       2       /* number of values: km/Miles */
#define RESTXT_SET_METRIC_KM        "   km"
#define RESTXT_SET_METRIC_MILES     "Miles"

/* LedWarning mode flag */
#define RESTXT_SET_LEDWM_WIDTH     4       /* max width of below strings in chars */
#define RESTXT_SET_LEDWM_CNT       2       /* number of values: km/Miles */
#define RESTXT_SET_LEDWM_SIXO      "SIxO"
#define RESTXT_SET_LEDWM_STD       " Std"

/* language settings */
#define RESTXT_SET_LANG_WIDTH       2
#define RESTXT_SET_LANG_CNT         3
                                   /*....!....!...*/    // 2 chars available
#define RESTXT_SET_LANG_DE          "DE"
#define RESTXT_SET_LANG_EN          "EN"
#define RESTXT_SET_LANG_NL          "NL"


/* compass settings */
#define RESTXT_SET_COMPD_WIDTH      13
#define RESTXT_SET_COMPD_CNT        4

#define RESTXT_SET_COMPC_WIDTH      21
#define RESTXT_SET_COMPC_CNT        8


/* analog input formating*/
#define RESTXT_NOTAVAIL     "  -"  // 'not available'
#define RESTXT_NOTCONN      "  -"  // 'not connected'


/* some default surveillance state levels */
#define RESTXT_STATE_LVL_OK         "OK"
#define RESTXT_STATE_LVL_INFO       "INFO"
#define RESTXT_STATE_LVL_WARN       "WARNING"
#define RESTXT_STATE_LVL_ERR        "ERROR"


/* intro special sw version strings
   1 character, might be concated  */
#define RESTXT_SWVER_DEBUG     " DBG"  // 'D' for DebugOuts on Uart0
#define RESTXT_SWVER_MINIEMU   " EMU " // 'E' for Emulator KD30
#define RESTXT_SWVER_COMPASS   " CMP"  // 'C' for Compass



/* HW-Test device ressources
   Should look like this:

   4x6 font = 32x8 chars on display
   +....!....!....!....!....!....!..+
   |V+   16.1V _   EEPROM  _  BAT  _|
   |ALTW  4.3V _   NVRAM   _  RTC  _|
   |RPM   1250 _   UART01 __  HWID _|
   |WHEEL  120 _   LDR ___ _  WHEEL_|
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

#define RESTXT_HWTDEV_SCREEN_32x8   "V+    x.xV _   EEPROM  _  BAT  _"  \
                                    "ALTW  x.xV _   NVRAM   _  RTC  _"  \
                                    "RPM   xxxx _   UART01 __  HWID _"  \
                                    "KM/H   xxx _   LDR xxx _  WHEEL_"  \
                                    "TWAT  xx'C _   TURNLR __  OIL  _"  \
                                    "TAIR  xx'C _   HBEAM   _  NEUTR_"  \
                                    "TOIL  xx'C _   GPI 0123 ____    "  \
                                    "TDEV  xx'C _   KEY 012  ___  UDO"





/* test resources */
                            /*!....!....!....!....!.*/

#define RESTXT_TEST_GUITEST   "     > GUI-Test <    "

#define RESTXT_TEST_ET_DSC    "EditText:"
#define RESTXT_TEST_EN_DSC    "EditN32:"
#define RESTXT_TEST_EN_UNIT   " units"
#define RESTXT_TEST_EB_DSC    "EditBool:"

#define RESTXT_TEST_SLCT_DSC    "Select:"
#define RESTXT_TEST_SLCT_MAX    5                           /* choice of 0..4 */
#define RESTXT_TEST_SLCT_WIDTH  12                          /* max field width */
                              /*!....!....!....!....!.*/
#define RESTXT_TEST_SLCT_A      "       Typ_A"
#define RESTXT_TEST_SLCT_B      "  was andres"
#define RESTXT_TEST_SLCT_C      "    Choice C"
#define RESTXT_TEST_SLCT_D      "         ???"
#define RESTXT_TEST_SLCT_E      "   unbekannt"



#define RESTXT_TEST3          "BMW"
                            /*!....!....!....!....!.*/
#define RESTXT_TEST4          "Hey Mann, das ist ein" \
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
#define RESTXT_TEST5          "Hey Mann, das ist ein sehr lan- " \
                              "ger Text, der aus dem ROM kommt " \
                              "und deshalb das RAM kaum belas- " \
                              "tet. Trotzdem sollte er in das  " \
                              "definierte Fenster passen, aller" \
                              "ding etwas willkürlich umgebroch" \
                              "en."
                            /*!....!....!....!.*/
#define RESTXT_TEST6          "  Hey Mann, das " \
                              "  ist ein sehr  " \
                              "  langer Text!  "


#endif /* _RESOURCE_H */

