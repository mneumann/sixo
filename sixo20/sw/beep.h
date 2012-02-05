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
 *  Module:         Beep
 *  Purpose:        Beep services (timing, auto update) API export header
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
 * Revision 3.3  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 3.2  2012/01/14 10:26:44  tuberkel
 * LED PWM handling changed:
 * - no longer Msgs/TimerMsgs used (inaccurate optic)
 * - instead use TimerISR to control PWM
 * - Granulartiy is SystemTicks (20 ms)
 * - works well
 * - prevent direct LEDDrv access (if possible)
 *
 *
 ************************************************************************ */


#ifndef _BEEP_H
#define _BEEP_H



/* beeper defines */
#define BEEP_OFF    DIGOUT_PERM_OFF        /* beeper is off */
#define BEEP_ON     DIGOUT_PERM_ON         /* beeper is on */


/* prototypes */
ERRCODE Beep_Init(void);
void    Beep_SignalOk(void);
void    Beep_SignalEsc(void);
void    Beep_SignalClick(void);


/* special test defines (default: off)*/
#ifndef TESTBEEP
#define TESTBEEP   0           // 1 enables a beep for every keypress
#endif

#if(TESTBEEP==1)

#endif // TESTBEEP

#endif /* _Beep_H */



