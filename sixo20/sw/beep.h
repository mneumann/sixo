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

#ifndef _BEEP_H
#define _BEEP_H



/* Beep timing */
typedef struct
{
    UINT8   wOnTicks;    /* Beep ON duration in 1/10 sec */
    UINT8   wOffTicks;   /* Beep OFF duration in 1/10 sec */
} BEEPTIMINGTYPE;


/* beeper defines */
#define BEEP_OFF FALSE        /* beeper is off */
#define BEEP_ON  TRUE         /* beeper is on */


/* macros for easier Beep messages and timings in ms / sec / ticks */
#define BEEP_MSG_MS(msg, status, ontime, offtime)    MSG_BUILD_UINT8(msg, MSG_BEEP_SET, status, MS2TICKS(ontime), MS2TICKS(offtime))
#define BEEP_MSG_SEC(msg, status, ontime, offtime)   MSG_BUILD_UINT8(msg, MSG_BEEP_SET, status, SEC2TICKS(ontime), SEC2TICKS(offtime))
#define BEEP_MSG_TICKS(msg, status, ontime, offtime) MSG_BUILD_UINT8(msg, MSG_BEEP_SET, status, ontime, offtime)


/* prototypes */
ERRCODE BeepInit(void);
ERRCODE BeepMsgEntry(MESSAGE msg);
ERRCODE BeepSetNewState(MESSAGE msg);

void BeepOk(void);
void BeepEsc(void);
void BeepClick(void);


#ifdef BEEPDEBUG
void TestBeepSendMessage(void); 
#endif // BEEPDEBUG

#endif /* _Beep_H */



