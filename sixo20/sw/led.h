/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-01 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         LED
 *  Purpose:        led services api export header
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
 ************************************************************************ */

#ifndef _LED_H
#define _LED_H


/* LED enumeration */
typedef enum
{
    LED_MIN,     // INVALID index, just for loops..
    LED_NEUTR,   // D4,  yellow     green
    LED_TURN,    // D3,  green      green
    LED_INFO,    // D9,  yellow     white
    LED_BEAM,    // D5,  blue       blue
    LED_WARN,    // D11, orange     orange
    LED_ERR,     // D10, red        red
    LED_MAX      // INVALID index, just for loops..
} LED_ENUM;



/* LED activation */
#define LED_ON  TRUE
#define LED_OFF FALSE

/* helper for LEDSetNewState() */
#define LED_PERM_ON     1,0,0       // OnTime:1 ms, OffTime:0 ms, Duration:0 => permanent off
#define LED_PERM_OFF    0,1,0       // OnTime:1 ms, OffTime:0 ms, Duration:0 => permanent off

/* LED PWM timing */
typedef struct
{
    UINT16  wOnTicks;           /* Beep ON duration in ticks (reload value) */
    UINT16  wOffTicks;          /* Beep OFF duration in ticks (reload value) */
    UINT16  wDurationTicks;     /* Beep duration in ticks */
    UINT16  wOnCurrTicks;       /* Beep On PWM timer - counting done */
    UINT16  wOffCurrTicks;      /* Beep Off PWM timer - counting done */
} LEDTIMINGTYPE;


/* prototypes */
ERRCODE LEDInit(void);
ERRCODE LEDService(void);
ERRCODE LEDSetNewState(LED_ENUM eLed, UINT16 wOn_ms, UINT16 wOff_ms, UINT16 wDuration_ms );
BOOL    LEDGetState(LED_ENUM eLed);

void    LEDOk(void);
void    LEDEsc(void);

STRING   LEDGetName( LED_ENUM eLED );


/* test functions */
#if(TESTLED==1)

#endif // TESTLED


#endif /* _LED_H */



