/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        03-30-2004 by Markus Mueller (mm@rsj.de)
 *  Project:        SIxO 2.0
 *  Module:         Illumination Measurement
 *  Purpose:        measure lumination with A/D port AN3 (P103)
 *  Comments:       -
 *  Changes:        revision, changes in architecture - RK 10-28-2004
 *                  tons of changes, correct filter calculations - AN 11-04-2004
 *                  introduced separate scale factors, - AN 11-06-2004
 *                  added AnaInGetAltwVoltage
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
 *      Stoeckener Str. 115                 Hudeplan 37 A
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
 * Revision 3.1  2012/06/02 06:14:19  tuberkel
 * just file header comments
 *
 * Revision 3.0  2010/11/07 14:00:50  tuberkel
 * V30 Preparations - no changes
 *
 * Revision 2.4  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.3  2007/03/30 10:11:24  tuberkel
 * changed ANAIN_TEMP_SENSORDETECT  -20 -> -10
 *
 * Revision 2.2  2007/03/26 23:17:00  tuberkel
 * just comments
 *
 * Revision 2.1  2006/10/19 18:53:58  tuberkel
 * BugFix: ANAIN_TEMP_OFFSET -> ANAIN_TEMP_SENSORDETECT
 * - now uses values <-20° to detect missing temp sensor
 * BugFix:
 * - ANAIN_STARTDELAY  3 -> 5
 * - prevents flickering fuel warning after 2 sec.
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 * Revision 1.9  2006/02/18 14:32:57  Ralf
 * - ANAIN_STARTDELAY change from MilliSec to seconds
 *
 ************************************************************************ */

#ifndef _ANAIN_H
#define _ANAIN_H


#define ANAIN_TEMP_SCALE  32        // scale factors for higher precision during filtering
#define ANAIN_LUMI_SCALE  32
#define ANAIN_VOLT_SCALE   4

#define ANAIN_TEMP_OFFSET         40     // offset of unsigned temperature values

#define ANAIN_TEMP_SENSORDETECT  (-10)   // in °C, lower values indicate a missing sensor

#define ANAIN_INVALID_S   0x7fff    // marker for invalid signed value (signed, max. positive)
#define ANAIN_INVALID_U   0xffff    // marker for invalid unsigned value (unsigned, max. positive)

#define ANAIN_STARTDELAY  5         // delay in sec to wait, until AD values should be valid


ERRCODE AnaInInit( void );
void    AnaInRefreshValues( UINT8 ucADChannel, UINT8 ucADSample );
INT16   AnaInGetTemperature( void );
UINT16  AnaInGetLumiVal( void );
UINT16  AnaInGetVoltage( void );
UINT16  AnaInGetAltwVoltage( void );
INT16   AnaInGetAirTemperature( void );
INT16   AnaInGetWatTemperature( void );
INT16   AnaInGetOilTemperature( void );
void    AnaInFormatVoltage( UINT16 volts, STRING szResult, size_t cbResult );
void    AnaInFormatTemperature( INT16 temp,  STRING szResult, size_t cbResult );


#endif //_ANAIN_H
