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


ERRCODE DigOutInit(void);


/* redefine of port register of sfr62p.h for readability */

#define LEDDrv_Port     p9      // Attention: M16C port p9 is protected by prc2!!!

#define fLEDDrv_Warn    p9_1   // D11 pin assignement of led
#define fLEDDrv_Err     p9_0   // D10
#define fLEDDrv_Info    p9_5   // D9
#define fLEDDrv_Beam    p9_2   // D5
#define fLEDDrv_Neutr   p9_7   // D4
#define fLEDDrv_Turn    p9_6   // D3

#define fLEDDrv_Cntrl   p8_0    // pin controls common LED current over transistor Q14


/* mask for LED access on p9 (p9 mixed with display pins) */
#define LEDS_ALL        0xe7    // all led pins excepting p9_3 + p9_4



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




#if(TOGGLE_PADS==1)
/* makros for port3_2/3/4 for ossi checks */
/* port pin will change state for everey call */
    #define TOGGLE_PAD9         (p3_2 = p3_2 ? 0 : 1)
    #define TOGGLE_PAD10        (p3_3 = p3_3 ? 0 : 1)
    #define TOGGLE_PAD11        (p3_4 = p3_4 ? 0 : 1)
#else
    #define TOGGLE_PAD9
    #define TOGGLE_PAD10
    #define TOGGLE_PAD11
#endif


/* ----------------------------------------------------- */
/* redefine of port register of sfr62p.h for readability */
#define PIN_BEEP    p8_7    // beeper out port pin
#define PIN_BEEP_D  pd8_7   // beeper out port pin direction register


/* ----------------------------------------------------- */
/* define GeneralPurposeOutput and Pads (& Direction Register) */
#define PIN_GPO0    p3_0    // General Purpose Out 0 port pin
#define PIN_GPO0_D  pd3_0   // General Purpose Out 0 port direction register
#define PIN_GPO1    p3_1    // General Purpose Out 1 port pin
#define PIN_GPO1_D  pd3_1   // General Purpose Out 0 port direction register



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




// solder pads (might be used for debug/extension purpuse)
#define PAD9    p3_2
#define PAD10   p3_3
#define PAD11   p3_4


// prototypes
ERRCODE DigOutDrv_SetPin(DIGOUT_PINS eGPO, BOOL fActivate);
BOOL    DigOutDrv_GetPin(DIGOUT_PINS eGPO );

