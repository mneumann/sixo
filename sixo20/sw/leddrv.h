/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-10-31 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         led
 *  Purpose:        high & low level led functions api export header
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
 ************************************************************************ */


#ifndef _LEDDrvH
#define _LEDDrvH


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
ERRCODE LEDDrvInit(BOOL fFlash);
ERRCODE LEDDrvSetLED(LEDDRV_LEDS bLEDIndex, BOOL fActivate);
ERRCODE LEDDrvSetBright(unsigned char bBrightness);


/* optional LED test functions (default: off) */
#ifndef TESTLED
#define TESTLED 0       // 1 enables a LED test functions
#endif


#if(TESTLED==1)
void LEDDrvTest(unsigned int loopdelay);
#endif // TESTLED

#endif /* _LEDDrvH */

