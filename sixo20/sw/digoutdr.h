/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-01 by Arnold Neugebauer
 *  Project:        SIxO
 *  Module:         Digital Output Driver
 *  Purpose:        Digital Output Driver api export header
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
 *  This information is automatically added while 'commit' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 3.6  2012/02/04 22:25:49  tuberkel
 * LEDs renamed
 *
 * Revision 3.5  2012/02/04 21:59:15  tuberkel
 * Toggle Pads renamed
 *
 * Revision 3.4  2012/02/04 21:49:42  tuberkel
 * All BeeperDriver functions mapped ==> DigOutDrv()
 *
 * Revision 3.3  2012/02/04 20:38:05  tuberkel
 * Moved all BeeperDriver / LEDDriver stuff ==> 'digoutdrv'
 *
 * Revision 3.2  2012/02/04 08:39:10  tuberkel
 * BugFix DigOut PWM
 *
 *
 ************************************************************************ */





/* led driver LED enumeration */
typedef enum
{
    LEDDRV_MIN,     // INVALID index, just for loops..
    LEDDRV_NEUTR,   // D4,  yellow     green
    LEDDRV_TURN,    // D3,  green      green
    LEDDRV_INFO,    // D9,  yellow     white
    LEDDRV_BEAM,    // D5,  blue       blue
    LEDDRV_WARN,    // D11, orange     orange
    LEDDRV_ERR,     // D10, red        red
    LEDDRV_MAX      // INVALID index, just for loops..
} LEDDRV_LEDS;


/* function protoypes */
ERRCODE LEDDrvInit      ( BOOL fFlash );
ERRCODE LEDDrvSetLED    ( LEDDRV_LEDS bLEDIndex, BOOL fActivate);
ERRCODE LEDDrvSetBright ( unsigned char bBrightness);
BOOL    LEDDrvGetLED    ( LEDDRV_LEDS eLED );









/* ----------------------------------------------------- */
/* define all Digital Port Pins (& Direction Register) */
/* (redefine of port register of sfr62p.h for readability) */

#define PORT_TESTPAD        p3      // port p3 controls all pads
#define PORT_TESTPAD_D      pd3     // port p3 driection register
#define PORT_TESTPAD_MASK   0x1c    // mask p3 pads pins
#define PIN_TESTPAD9        p3_2    // schematics test pad 9
#define PIN_TESTPAD10       p3_3    // schematics test pad 10
#define PIN_TESTPAD11       p3_4    // schematics test pad 11

#define PIN_BEEP            p8_7    // beeper out port pin
#define PIN_BEEP_D          pd8_7   // beeper out port pin direction register

#define PIN_GPO0            p3_0    // General Purpose Out 0 port pin
#define PIN_GPO0_D          pd3_0   // General Purpose Out 0 port direction register
#define PIN_GPO1            p3_1    // General Purpose Out 1 port pin
#define PIN_GPO1_D          pd3_1   // General Purpose Out 0 port direction register

#define PORT_LED            p9      // port p9 controls all LEDs (!!! p9 is protected by prc2!!!)
#define PORT_LED_D          pd9     // port 9 direction control register
#define PORT_LED_MASK       0xe7    // all p9 led pins (except p9_3 + p9_4 used for display!)
#define PIN_LED_WARN        p9_1    // LED D11 - orange
#define PIN_LED_ERR         p9_0    // LED D10 - red
#define PIN_LED_INFO        p9_5    // LED D9  - white
#define PIN_LED_HBEAM       p9_2    // LED D5  - blue
#define PIN_LED_NEUTR       p9_7    // LED D4  - green
#define PIN_LED_TURN        p9_6    // LED D3  - TURN
#define PIN_LED_BRIGHT      p8_0    // LED current over transistor Q14
#define PIN_LED_BRIGHT_D    pd8_0   // p8 direction control register



/* ----------------------------------------------------- */
/* makros for test pads for ossi checks */
/* port pin will change state for everey call */
#if(TOGGLE_TESTPADS==1)
    #define PIN_TESTPAD9_TOGGLE     (PIN_TESTPAD9  = PIN_TESTPAD9  ? 0 : 1)
    #define PIN_TESTPAD10_TOGGLE    (PIN_TESTPAD10 = PIN_TESTPAD10 ? 0 : 1)
    #define PIN_TESTPAD11_TOGGLE    (PIN_TESTPAD11 = PIN_TESTPAD11 ? 0 : 1)
#else
    #define PIN_TESTPAD9_TOGGLE     // undefined
    #define PIN_TESTPAD10_TOGGLE
    #define PIN_TESTPAD11_TOGGLE
#endif


/* ----------------------------------------------------- */
/* Parameter to directly set pin on/off */
#define DIGOUT_OFF  FALSE   // digital out off
#define DIGOUT_ON   TRUE    // digital out on


/* ----------------------------------------------------- */
/* Enumeration of all Digital Out Driver Pins  */
typedef enum
{
    eDIGOUT_GPO0,   // General Purpose Out 0
    eDIGOUT_GPO1,   // General Purpose Out 1
    eDIGOUT_BEEP    // Beeper
} DIGOUT_PINS;




// prototypes
ERRCODE DigOutDrv_Init(void);
ERRCODE DigOutDrv_SetPin(DIGOUT_PINS ePin, BOOL fActivate);
BOOL    DigOutDrv_GetPin(DIGOUT_PINS ePin );

