/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-01 by Ralf Krizsan
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

#ifndef _LED_H
#define _LED_H


/* LED activation */
#define LED_ON  TRUE
#define LED_OFF FALSE


/* led timing */
typedef struct
{
    UINT8   wOnTicks;    /* led ON duration in 1/10 sec */
    UINT8   wOffTicks;   /* led OFF duration in 1/10 sec */
} LEDTIMINGTYPE;


/* macros for easier led messages and timings in ms / sec / ticks */
#define LED_MSG_MS(msg, led, status, ontime, offtime) MSG_BUILD_UINT8(msg, MSG_LED_SET, (led | (status<<4)), MS2TICKS(ontime), MS2TICKS(offtime))
#define LED_MSG_SEC(msg, led, status, ontime, offtime) MSG_BUILD_UINT8(msg, MSG_LED_SET, (led | (status<<4)), SEC2TICKS(ontime), SEC2TICKS(offtime))
#define LED_MSG_TICKS(msg, led, status, ontime, offtime) MSG_BUILD_UINT8(msg, MSG_LED_SET, (led | (status<<4)), ontime, offtime)


/* prototypes */
ERRCODE LEDInit(void);
ERRCODE LEDMsgEntry(MESSAGE msg);
ERRCODE LEDSetNewState(MESSAGE msg);
void LEDOk(void);
void LEDEsc(void);



#if(TESTLED==1)
    /* test functions */
    void TestLEDSendMessage(void);
    void TestCheckKeyStateMsgs(MESSAGE msg);
#endif // TESTLED


#endif /* _LED_H */



