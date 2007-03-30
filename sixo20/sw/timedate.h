/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2004-02-29 by Ralf Krizsan
 *  Project:        SIxO20
 *  Module:         Time & Date
 *  Purpose:        High-Level api export header
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
 *  --------------------------------------------------------------------
 *
 *  CVS History
 *
 *  This information is automatically added while 'commiting' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 2.1  2007/03/30 09:59:17  tuberkel
 * Matthias Otto stuff:
 * - added  ENGL_WWDDMMYY       // return date in format 'Fr, 28-02-04'
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.3  2006/02/18 15:17:40  Ralf
 * - dwSystemTime (ms) added
 *
 *
 ************************************************************************ */


#ifndef _TIMEDATE_H
#define _TIMEDATE_H


// common defines
#define TIMEDATEREFRESH_MS      100     // refresh cycle of RTC-TimeDate copy in MilliSeconds


/* clock time structure
    - to be used for clock */
typedef struct
{
   UINT8    bHour;      // 0..23  decimal
   UINT8    bMin;       // 0..59  decimal
   UINT8    bSec;       // 0..59  decimal
   UINT16   wMilliSec;  // 0..999 decimal
} TIME_TYPE;


/* long distance time structure
    - to be used for long distance time counters */
typedef struct
{
   UINT16   wHour;      // 0..65535 decimal
   UINT8    bMin;       // 0..59  decimal
   UINT8    bSec;       // 0..59  decimal
} TIME_TYPE_LD;


/* long lap time structure
    - to be used for long lap time counters */
typedef struct
{
   UINT8    bMin;       // 0..99  decimal
   UINT8    bSec;       // 0..59  decimal
} TIME_TYPE_LL;


/* short lap time structure
    - to be used for long lap time counters */
typedef struct
{
   UINT8    bMin;       // 0..99  decimal
   UINT8    bSec;       // 0..59  decimal
   UINT16   wMilliSec;  // 0..999 decimal
} TIME_TYPE_SL;




// day of week enum (adapted to clkdrv.c)
typedef enum
{
    eMon = 1,           // Monday
    eTue,               // Tuesday
    eWed,               // Wednesday
    eThu,               // Thursday
    eFri,               // Friday
    eSat,               // Saturday
    eSun                // Sunday
} DOW_TYPE;


// date structure
typedef struct
{
   UINT8    bDate;      // 1..31 decimal format, RTC-Sync once per hour or when set
   UINT8    bMonth;     // 1..12 decimal format, RTC-Sync once per day or when set
   UINT8    bYear;      // 0..99 decimal format, RTC-Sync once per month or when set
   DOW_TYPE eDoW;       // eMon..eSun decimal format, day of week, RTC-Sync once per hour
} DATE_TYPE;


// predefined format for string return values
typedef enum
{
    // german formats
    GERM_HHMM,          // return time in format '12:34'
    GERM_HHMMSS,        // return time in format '12:34:56'
    GERM_DDMMYY,        // return date in format '28.02.04'
    GERM_DDMMYYYY,      // return date in format '28.02.2004'
    GERM_WWDDMMYY,      // return date in format 'Fr, 28.02.04'
    // english formats
    ENGL_HHMM,          // return time in format '12:34'
    ENGL_HHMMSS,        // return time in format '12:34:56'
    ENGL_YYMMDD,        // return date in format '04-12-28'
    ENGL_YYYYMMDD,      // return date in format '2004-12-28'
    ENGL_WWYYMMDD,      // return date in format 'Fr, 04-12-28'
    ENGL_WWDDMMYY       // return date in format 'Fr, 28-02-04'

} DATETIME_STRFORMAT;


// system runtime in MilliSec
typedef UINT32 TIME_TYPE_SYS;  // system time since application start in MilliSec (max ca. 49 days)


// public interface functions
ERRCODE TimeDateInit(void);
void    TimeDateUpdate (void);

ERRCODE TimeDate_GetTime( TIME_TYPE * pTime );
ERRCODE TimeDate_GetDate( DATE_TYPE * pDate );
ERRCODE TimeDate_GetString( DATETIME_STRFORMAT, STRING szBuffer );
STRING  TimeDate_GetDoWString( BOOL fShortForm, DOW_TYPE eDayofWeek);
ERRCODE TimeDate_GetCalib( INT8* pcCalib, INT32* plSecPerMonth );
ERRCODE TimeDate_GetCalibDirect( UINT8* pcCalib );

ERRCODE TimeDate_SetTime( TIME_TYPE * pTime );
ERRCODE TimeDate_SetDate( DATE_TYPE * pTime );
ERRCODE TimeDate_SetCalib( INT32 lDuration, INT32 lDeviation );
ERRCODE TimeDate_SetCalibDirect( UINT8* pcCalib );


// private interface functions
void    TimeDateUpdateTime(void);
void    TimeDateUpdateDate(void);
DOW_TYPE TimeDate_GetDayOfWeek(DATE_TYPE * pDate);


#endif // _TIMEDATE_H
