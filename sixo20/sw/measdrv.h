/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-01 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         Measurement Driver
 *  Purpose:        Low level routines for all Measurements API
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
#ifndef _MEASDRV_H
#define _MEASDRV_H



/* defines */
#define WHEEL_MAXVALUE           10000      /* max counter (=reload) value = equals 2 seconds */
#define RPM_MAXVALUE             50000      /* max counter (=reload) value = equals 0,5 seconds */

#define TB2_HZ                    5000L     /* Wheel-Prescaler frequency in Hz = 1/(200 µs) */
#define TB2_RELOAD          FSYS/TB2_HZ     /* TB2-Reload value = 14745600L/5000L = 2949,12 */

#define TA4_HZ                  100000L     /* RPM-Prescaler frequency in Hz = 1/(10 µs) */
#define TA4_RELOAD          FSYS/TA4_HZ     /* TA4-Reload value = 14745600L/100000L =  147,456 */



/* sfr62p.h redefines */
#define WheelPortD      pd8_2               /* INT0 WHEEL impuls contact direction bit */
#define WheelPort       p8_2                /* INT0 WHEEL impuls contact bit */
#define RPMPortD        pd8_3               /* INT1 RPM impuls contact direction bit */
#define RPMPort         p8_3                /* INT1 RPM impuls contact bit */

#define fWheelCnt       ta2s                /* timer TA2 start/stop flag */
#define wWheelCnt       ta2                 /* timer TA2 register value */
#define fRPMCnt         ta3s                /* timer TA3 start/stop flag */
#define wRPMCnt         ta3                 /* timer TA3 register value */




/* public function prototypes */
ERRCODE MeasDrvInit(void);
ERRCODE MeasDrvInitWheel(void);
ERRCODE MeasDrvInitRPM(void);
UINT16 MeasDrvGetWheelPeriod(BOOL fCurrent);
UINT16 MeasDrvGetRPMPeriod(BOOL fFast);
void WheelSensor_ISR(void);
void RPMSensor_ISR(void);

#endif /* _MEASDRV_H */


