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
 *  --------------------------------------------------------------------
 *
 *  CVS History
 *
 *  This information is automatically added while 'commit' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 3.5  2012/02/11 09:12:40  tuberkel
 * New: LED_SignalAck()
 *
 * Revision 3.4  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 3.3  2012/02/04 08:40:26  tuberkel
 * BugFix LED PWM
 *
 *
 ************************************************************************ */


#ifndef _LED_H
#define _LED_H


/* Mapping LED API to DigOutDrv API  */
#define LED_ON      DIGOUT_PERM_ON
#define LED_OFF     DIGOUT_PERM_OFF


/* mapping GPO ports to DigOutDriver */
typedef enum
{
    eLED_NEUTR  = eDIGOUT_LED_NEUTR,
    eLED_TURN   = eDIGOUT_LED_TURN,
    eLED_INFO   = eDIGOUT_LED_INFO,
    eLED_HBEAM  = eDIGOUT_LED_HBEAM,
    eLED_WARN   = eDIGOUT_LED_WARN,
    eLED_ERROR  = eDIGOUT_LED_ERROR
} LED_ENUM;


/* public prototypes */
ERRCODE LED_Init            ( void );
BOOL    LED_GetState        ( LED_ENUM eLed );
ERRCODE LED_SetNewState     ( LED_ENUM eLed, UINT16 wOn_ms, UINT16 wOff_ms, UINT16 wDur_ms );
ERRCODE LED_SetBrightness   ( unsigned char bBrightness);
void    LED_SignalOk        ( void );
void    LED_SignalAck       ( void );
void    LED_SignalEsc       ( void );


/* test functions */
#if(TESTLED==1)

#endif // TESTLED


#endif /* _LED_H */



