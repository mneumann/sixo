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
 * Revision 1.4  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
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


/* Mapping LED API to DigOutDrv API  */
#define GPO_ON      DIGOUT_PERM_ON
#define GPO_OFF     DIGOUT_PERM_OFF

#define GPO_ON_CRKEY  250, 0, 400     // coolride key pressed timing 250-on-150-off


/* mapping GPO ports to DigOutDriver */
typedef enum
{
    eGPO_0 = eDIGOUT_GPO_0,
    eGPO_1 = eDIGOUT_GPO_1
} GPO_ENUM;




/* prototypes */
ERRCODE GPO_Init           ( void);
ERRCODE GPO_SetNewState    ( GPO_ENUM eGpo, UINT16 wOn_ms, UINT16 wOff_ms, UINT16 wDur_ms );
BOOL    GPO_GetState       ( GPO_ENUM eGpo);
ERRCODE GPO_SignalCoolRide ( void );



#endif /* _GPO_H */



