/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2012-02-01 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         GPO
 *  Purpose:        GPO services (timing, auto update)
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
 * Revision 1.3  2012/02/04 21:49:42  tuberkel
 * All BeeperDriver functions mapped ==> DigOutDrv()
 *
 * Revision 1.2  2012/02/04 08:39:39  tuberkel
 * BugFix DigOut PWM
 *
 *
 ************************************************************************ */


#ifndef _GPO_H
#define _GPO_H


/* number of available GPOs */
#define GPO_COUNT   2

/* GPO enumeration */
typedef enum
{
    eGPO_0,       // PIN_GPO0
    eGPO_1,       // PIN_GPO1
} GPO_ENUM;



/* GPO activation */
#define GPO_ON  TRUE
#define GPO_OFF FALSE

/* helper for GPOSetNewState() */
#define GPO_PERM_ON     1,0,0       // OnTime:1 ms, OffTime:0 ms, Duration:0 => permanent off
#define GPO_PERM_OFF    0,1,0       // OnTime:1 ms, OffTime:0 ms, Duration:0 => permanent off

/* GPO PWM timing */
typedef struct
{
    UINT16  wOnTicks;           /* Beep ON duration in ticks (reload value) */
    UINT16  wOffTicks;          /* Beep OFF duration in ticks (reload value) */
    UINT16  dwDurTicks;         /* Beep duration in ticks */
    UINT16  wOnCurrTicks;       /* Beep On PWM timer (down counting) */
    UINT16  wOffCurrTicks;      /* Beep Off PWM timer (down counting) */
} GPOTIMINGTYPE;


/* prototypes */
ERRCODE GPOInit(void);
ERRCODE GPOService(void);
ERRCODE GPOSetNewState(GPO_ENUM eGpo, UINT16 wOn_ms, UINT16 wOff_ms, UINT16 wDuration_ms );
BOOL    GPOGetState(GPO_ENUM eGpo);
STRING  GPOGetName( GPO_ENUM eGPO);



#endif /* _GPO_H */



