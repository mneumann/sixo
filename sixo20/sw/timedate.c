/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2004-02-29 by Ralf Schwarzer
 *  Project:        SIxO20
 *  Module:         Time & Date
 *  Purpose:        High-Level service to get & set RTC time+date
 *  Comments:       Purpose of this module:
 *                      - more simple interface to clock functions
 *                      - reduce IIC-Bus system load
 *                      - global access to automatically updated
 *                        variables without care of IIC-Bus behaviour
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
 * Revision 3.0  2010/11/07 09:53:13  tuberkel
 * V30 Preparations:
 * - New: DayLightSaving 'CEST' supported
 *
 * Revision 2.3  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.2  2009/06/24 21:13:08  tuberkel
 * just comments
 *
 * Revision 2.1  2007/03/30 10:00:22  tuberkel
 * Matthias Otto stuff:
 * - language specific display content
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.5  2006/02/18 15:07:34  Ralf
 * - dwSystemTime (ms) added
 *
 *
 ************************************************************************ */


/* includes */
#include <string.h>
#include <stdio.h>
#include "standard.h"
#include "timer.h"
#include "timedate.h"
#include "clkdrv.h"
#include "debug.h"
#include "m16c.h"
#include "resource.h"
#include "stdmsgs.h"
#include "msgqueue.h"
#include "sysparam.h"
#include "surveill.h"


/* non public variables for internal use */
static BOOL fInitialized = FALSE;       // prevent early timer-isr call problems

static TIME_TYPE    RTCTime;            // time READ access, always up2date
static DATE_TYPE    RTCDate;            // date READ access, always up2date

/* external symbols */
extern UINT16           wMilliSecCounter;       // high resolution short distance timer, ms,  max  65 sec
extern UINT16           wSecCounter;            // low  resolution long  distance timer, sec, max. 18 h
extern UINT16           dwSystemTime;           // high resolution long  distance timer, ms,  max. 49 days
extern DEVFLAGS2_TYPE   gDeviceFlags2;          // daylight saving settings
extern BOOL             fCESTChanged;           // DaylaightSaving 'CET/CEST changed flag'




/***********************************************************************
 *  FUNCTION:       TimeDateInit
 *  DESCRIPTION:    time date modules init stuff
 *  PARAMETER:      -
 *  RETURN:         ERR_OK      always
 *  COMMENT:        The hardware may deliver wrong values. This is likely
 *                  to occur after the very first power up or when the
 *                  Li-Battery is replaced from the RTC.
 *                  If the hardware delivers a wrong time or date part
 *                  it is set to fixed valid value.
 *********************************************************************** */
ERRCODE TimeDateInit(void)
{
    CLOCKTIME ClkDrvTime;
    CLOCKDATE ClkDrvDate;
    UINT8     bClkCalib;

    // variable init
    memset( &RTCTime, 0, sizeof(TIME_TYPE));
    memset( &RTCDate, 0, sizeof(DATE_TYPE));

    // get current time
    if (iicGetClockTime( &ClkDrvTime ) == ERR_OK )      // get fresh time stamp
    {
        RTCTime.bSec = BCD2VAL(ClkDrvTime.seconds);     // convert BCD -> dec
        RTCTime.bMin = BCD2VAL(ClkDrvTime.minutes);     // convert BCD -> dec
        RTCTime.bHour= BCD2VAL(ClkDrvTime.hours);       // convert BCD -> dec

        // replace wrong values
        if( RTCTime.bSec  > 59 ) RTCTime.bSec  = 0;
        if( RTCTime.bMin  > 59 ) RTCTime.bMin  = 0;
        if( RTCTime.bHour > 23 ) RTCTime.bHour = 0;
    }

    // get current date
    if ( iicGetClockDate( &ClkDrvDate ) == ERR_OK )
    {
        UINT8 bDay;

        RTCDate.bDate   = BCD2VAL(ClkDrvDate.date);             // convert BCD -> dec
        RTCDate.bMonth  = BCD2VAL(ClkDrvDate.month);            // convert BCD -> dec
        RTCDate.bYear   = BCD2VAL(ClkDrvDate.year);             // convert BCD -> dec
        bDay            = BCD2VAL(ClkDrvDate.day);              // convert BCD -> dec

        // replace wrong values
        if( (RTCDate.bDate  - 1) > 30 ) RTCDate.bDate  = 1;
        if( (RTCDate.bMonth - 1) > 11 ) RTCDate.bMonth = 1;
        if(  RTCDate.bYear > 99 ) RTCDate.bYear = 0;
        if( (bDay - 1) > 6 ){
            RTCDate.eDoW = eSat; //the 1.1.2000 was a saturday
        }
        else{
            RTCDate.eDoW = (DOW_TYPE)bDay;
        }
    }

    // read calibration value
    iicClockCalibDirect( FALSE, &bClkCalib );

    // debug stuff:
    ODS(DBG_SYS,DBG_INFO,"Reading RTC settings:");
    ODS3(DBG_SYS,DBG_INFO,"- Time:  %02u:%02u:%02u", RTCTime.bHour, RTCTime.bMin, RTCTime.bSec);
    ODS4(DBG_SYS,DBG_INFO,"- Date:  %s, %02u.%02u.%02u",
        TimeDate_GetDoWString(TRUE, RTCDate.eDoW), RTCDate.bDate, RTCDate.bMonth, RTCDate.bYear);
    ODS1(DBG_SYS,DBG_INFO,"- Calib: %u", bClkCalib);
    ODS(DBG_SYS,DBG_INFO,"\n");    // preceding empty line

    fInitialized = TRUE;    // from now on allow timer-isr refreshs of local RTC values
}


/***********************************************************************
 *  FUNCTION:       TimeDateUpdate
 *  DESCRIPTION:    Automatic sync of all time & date variables
 *                  with RTC values, updates RTC time 1/second,
 *                  updates RTC date 1/minute
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        - Used to reduce RTC-IIC bus system load.
 *                  - Can be called as often as possible, but update
 *                    cycle is based on MilliSecCounter.
 *                  - This function is called frequently by Timer-ISR.
 *********************************************************************** */
void TimeDateUpdate (void)
{
    // check: already started, but not yet initialized?
    // (this prevents problems with early timer-isr calls)
    if ( fInitialized == FALSE )
        return;

    // just check RTC time and support global variable RTCTime
    TimeDateUpdateTime();

    // check: any minute gone? (eq. Sec==0)
    if ( RTCTime.bSec == 0 )
    {
        // just check RTC date and support global variable RTCDate
        TimeDateUpdateDate();
    }
}


/***********************************************************************
 *  FUNCTION:       TimeDateUpdateTime
 *  DESCRIPTION:    Just get RTC Time update immedeatly
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        The hardware may deliver wrong values. This is likely
 *                  to occur after the very first power up or when the
 *                  Li-Battery is replaced from the RTC.
 *                  If the hardware delivers a wrong time or date part
 *                  it is set to fixed valid value.
 *********************************************************************** */
void TimeDateUpdateTime (void)
{
            UINT16 wThisCall_ms         = 0;
    static  UINT16 wLastCall_ms         = 0;
    static  UINT16 wLastCompleteSec_ms  = 0;
    static  UINT8  bLastSecond          = 0;
    static  UINT8  bLastHour            = 0;
    static  UINT8  bLastDay             = 0;
    static  UINT8  bLastMonth           = 0;
            MESSAGE Msg;                            /* for building messages */

    // check: do we have to update seconds (e.g. 5 times/second) ?
    TimerGetSys_msec(wThisCall_ms);
    if ( (wThisCall_ms - wLastCall_ms) > TIMEDATEREFRESH_MS )
    {
        // REFRESH RTC TIME NOW:
        CLOCKTIME ClkDrvTime;

        wLastCall_ms = wThisCall_ms;                        // save this call
        if (iicGetClockTime( &ClkDrvTime ) == ERR_OK )      // get fresh time stamp
        {
            RTCTime.bSec = BCD2VAL(ClkDrvTime.seconds);     // convert BCD -> dec
            RTCTime.bMin = BCD2VAL(ClkDrvTime.minutes);     // convert BCD -> dec
            RTCTime.bHour= BCD2VAL(ClkDrvTime.hours);       // convert BCD -> dec
            //ODS3(DBG_SYS,DBG_INFO,"TimeDateUpdateTime(): Current Time %02u:%02u:%02u",
            //        RTCTime.bHour, RTCTime.bMin, RTCTime.bSec);

            // replace wrong values
            if( RTCTime.bSec  > 59 ) RTCTime.bSec  = 0;
            if( RTCTime.bMin  > 59 ) RTCTime.bMin  = 0;
            if( RTCTime.bHour > 23 ) RTCTime.bHour = 0;
        }
        else
        {   ODS(DBG_SYS,DBG_ERROR,"TimeDateUpdateTime(): Error reading clock time!");
        }
    }

    // support devices which show seconds with an extra update msg
    if ( bLastSecond != RTCTime.bSec )
    {
        // save current second
        bLastSecond = RTCTime.bSec;

        // trigger screen refresh for time objects for any device...
        MSG_BUILD_UINT8(Msg, MSG_TIMEDATE_SECOND_GONE, 0xff, 0xff, 0xff);
        MsgQPostMsg( Msg, MSGQ_PRIO_LOW);
    }

    // support daylight saving too - only if MON/DAY/HOUR changed (at PowerUp & runtime)
    if (  ( bLastMonth != RTCDate.bMonth )
        ||( bLastDay   != RTCDate.bDate  )
        ||( bLastHour  != RTCTime.bHour  ) )
    {
        TimeDate_CheckDaylightSaving(); // check & execute DLS changes
        bLastMonth = RTCDate.bMonth;     // save new state for next use
        bLastDay   = RTCDate.bDate;
        bLastHour  = RTCTime.bHour;
    }
}


/***********************************************************************
 *  FUNCTION:       TimeDateUpdateDate
 *  DESCRIPTION:    Just get RTC Date update immedeatly
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        The hardware may deliver wrong values. This is likely
 *                  to occur after the very first power up or when the
 *                  Li-Battery is replaced from the RTC.
 *                  If the hardware delivers a wrong time or date part
 *                  it is set to fixed valid value.
 *********************************************************************** */
void TimeDateUpdateDate (void)
{
    // REFRESH RTC DATE NOW:
    CLOCKDATE ClkDrvDate;
    if ( iicGetClockDate( &ClkDrvDate ) == ERR_OK )
    {
        UINT8 bDay;

        RTCDate.bDate   = BCD2VAL(ClkDrvDate.date);             // convert BCD -> dec
        RTCDate.bMonth  = BCD2VAL(ClkDrvDate.month);            // convert BCD -> dec
        RTCDate.bYear   = BCD2VAL(ClkDrvDate.year);             // convert BCD -> dec
        bDay            = BCD2VAL(ClkDrvDate.day);              // convert BCD -> dec
        //ODS4(DBG_SYS,DBG_INFO,"TimeDateUpdate(): Current Date %u, %02u.%02u.%02u",
        //        RTCDate.eDoW, RTCDate.bDate, RTCDate.bMonth, RTCDate.bYear);

        // replace wrong values
        if( (RTCDate.bDate  - 1) > 30 ) RTCDate.bDate  = 1;
        if( (RTCDate.bMonth - 1) > 11 ) RTCDate.bMonth = 1;
        if(  RTCDate.bYear > 99 ) RTCDate.bYear = 0;
        if( (bDay - 1) > 6 ){
            RTCDate.eDoW = eSat; //the 1.1.2000 was a saturday
        }
        else{
            RTCDate.eDoW = (DOW_TYPE)bDay;
        }
    }
}




/***********************************************************************
 *  FUNCTION:       TimeDate_GetTime
 *  DESCRIPTION:    return current time
 *  PARAMETER:      pTime       addr of struct to return time
 *  RETURN:         ERR_OK      always
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE TimeDate_GetTime( TIME_TYPE * pTime )
{
    INT_GLOB_DISABLE;
    memcpy( pTime, &RTCTime, sizeof(TIME_TYPE));  // get a copy
    INT_GLOB_ENABLE;
    return (ERR_OK);
}


/***********************************************************************
 *  FUNCTION:       TimeDate_GetDate
 *  DESCRIPTION:    returns current date
 *  PARAMETER:      pDate       addr of struct to return date
 *  RETURN:         ERR_OK      always
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE TimeDate_GetDate( DATE_TYPE * pDate )
{
    INT_GLOB_DISABLE;
    memcpy( pDate, &RTCDate, sizeof(DATE_TYPE));  // get a copy
    INT_GLOB_ENABLE;
    return (ERR_OK);
}


/***********************************************************************
 *  FUNCTION:       TimeDate_GetString
 *  DESCRIPTION:    returns formated string of requested values
 *  PARAMETER:      eFormat         requested data format
 *                  szBuffer        buffer to return string
 *  RETURN:         ERR_OK          always
 *  COMMENT:        TAKE CARE OF BUFFER SIZE!
 *********************************************************************** */
ERRCODE TimeDate_GetString( DATETIME_STRFORMAT eFormat, STRING szBuffer )
{
    TIME_TYPE   Time;
    DATE_TYPE   Date;

    TimeDate_GetTime( &Time );
    TimeDate_GetDate( &Date );

    switch (eFormat)
    {
        // german formats
        // Note: the leading SPACE in HHMM format compensates the preceeding 'a'/'p' of the englisch version!
        case GERM_HHMM:          // return time in format '  5:34' (see note above!)
            sprintf(szBuffer," %2u:%02u", Time.bHour, Time.bMin);
            break;
        // Note: the leading SPACE in HHMM format compensates the preceeding 'a'/'p' of the englisch version!
        case GERM_HHMMSS:        // return time in format '  5:34:56' (see note above!)
            sprintf(szBuffer," %2u:%02u:%02u", Time.bHour, Time.bMin, Time.bSec);
            break;
        case GERM_DDMMYY:        // return date in format ' 1.02.04'
            sprintf(szBuffer,"%2u.%02u.%02u", Date.bDate, Date.bMonth, Date.bYear);
            break;
        case GERM_DDMMYYYY:      // return date in format ' 1.02.2004'
            sprintf(szBuffer,"%2u.%02u.20%02u", Date.bDate, Date.bMonth, Date.bYear);
            break;
        case GERM_WWDDMMYY:      // return date in format 'Fr  1.02.04'
            sprintf(szBuffer,"%s %2u.%02u.%02u", TimeDate_GetDoWString(TRUE, Date.eDoW), Date.bDate, Date.bMonth, Date.bYear);
            break;

        // english formats
        case ENGL_HHMM:          // return time in format ' 2:34p'
         	sprintf(szBuffer,"%2u:%02u%c", Time.bHour<=12?Time.bHour:Time.bHour-12, Time.bMin, Time.bHour<12?'a':'p');
			break;
        case ENGL_HHMMSS:        // return time in format ' 2:34:56p'
            sprintf(szBuffer,"%2u:%02u:%02u%c", Time.bHour<=12?Time.bHour:Time.bHour-12, Time.bMin, Time.bSec, Time.bHour<12?'a':'p');
            break;
        case ENGL_YYMMDD:        // return date in format '04-12-28'
        case ENGL_YYYYMMDD:      // return date in format '2004-12-28'
        case ENGL_WWYYMMDD:      // return date in format 'Fr 04-12-28'
		case ENGL_WWDDMMYY:		 // return date in format 'Fr  2/02/04'
		     sprintf(szBuffer,"%s %2u/%02u/%02u", TimeDate_GetDoWString(TRUE, Date.eDoW), Date.bDate, Date.bMonth, Date.bYear);
            break;
        default: strcpy(szBuffer,"?");
    }
    return (ERR_OK);
}

/***********************************************************************
 *  FUNCTION:       TimeDate_GetDoWString
 *  DESCRIPTION:    returns string to adequate 'day of week' string
 *  PARAMETER:      fShortForm      TRUE uses 2-char form like 'Mo'
 *                                  FALSE uses full form like 'Monday'
 *                  eDayofWeek      requested day of week
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
STRING TimeDate_GetDoWString( BOOL fShortForm, DOW_TYPE eDayofWeek)
{
    if (fShortForm == TRUE)
    {
        switch (eDayofWeek)
        {
            case eMon: return( RESTXT_DOWSHORT_MON ); break;
            case eTue: return( RESTXT_DOWSHORT_TUE ); break;
            case eWed: return( RESTXT_DOWSHORT_WED ); break;
            case eThu: return( RESTXT_DOWSHORT_THU ); break;
            case eFri: return( RESTXT_DOWSHORT_FRI ); break;
            case eSat: return( RESTXT_DOWSHORT_SAT ); break;
            case eSun: return( RESTXT_DOWSHORT_SUN ); break;
            default: return("?"); break;
        }
    }
    else
    {
        switch (eDayofWeek)
        {
            case eMon: return( RESTXT_DOWLONG_MON ); break;
            case eTue: return( RESTXT_DOWLONG_TUE ); break;
            case eWed: return( RESTXT_DOWLONG_WED ); break;
            case eThu: return( RESTXT_DOWLONG_THU ); break;
            case eFri: return( RESTXT_DOWLONG_FRI ); break;
            case eSat: return( RESTXT_DOWLONG_SAT ); break;
            case eSun: return( RESTXT_DOWLONG_SUN ); break;
            default: return("?"); break;
        }
    }
    return("?");
}


/***********************************************************************
 *  FUNCTION:       TimeDate_GetCalibDirect
 *  DESCRIPTION:    just a wrapper for iicClockCalibDirect()
 *  PARAMETER:      pcCalib         pointer to calib value that will be
 *                                  returned
 *  RETURN:         ERR_CLOCK_NOREAD   read access failed, no acknowledge
 *                  ERR_OK             success
 *  COMMENT:        see 'clkdrv.c' for handling
 *********************************************************************** */
ERRCODE TimeDate_GetCalibDirect( UINT8* pcCalib )
{
    // read value
    return iicClockCalibDirect( FALSE, pcCalib );
}


/***********************************************************************
 *  FUNCTION:       TimeDate_SetCalibDirect
 *  DESCRIPTION:    just a wrapper for iicClockCalibDirect()
 *  PARAMETER:      pcCalib         pointer to calib value that will be set
 *  RETURN:         ERR_CLOCK_NOWRITE  write access failed, no acknowledge
 *                  ERR_OK             success
 *  COMMENT:        see 'clkdrv.c' for handling
 *********************************************************************** */
ERRCODE TimeDate_SetCalibDirect( UINT8* pcCalib )
{
    // write value
    ODS1(DBG_SYS,DBG_INFO,"TimeDate_SetCalibDirect(): Calibration value changed to %u", *pcCalib);
    return iicClockCalibDirect( TRUE, pcCalib );
}




/***********************************************************************
 *  FUNCTION:       TimeDate_GetCalib
 *  DESCRIPTION:    just a wrapper for iicGetClockCalib()
 *  PARAMETER:      chCalib         pointer to calib value that will be
 *                                  returned
 *                  dwSecPerMonth   pointer to equivalent seconds per month
 *  RETURN:         ERR_CLOCK_NOREAD   read access failed, no acknowledge
 *                  ERR_OK             success
 *  COMMENT:        see 'clkdrv.c' for handling
 *********************************************************************** */
ERRCODE TimeDate_GetCalib( INT8* pcCalib, INT32* plSecPerMonth )
{
    return iicGetClockCalib( pcCalib, plSecPerMonth );
}


/***********************************************************************
 *  FUNCTION:       TimeDate_SetTime
 *  DESCRIPTION:    sets given time into RTC
 *  PARAMETER:      pTime       addr of struct with given time
 *  RETURN:         ERR_PARAM_ERR        invalid parameter
 *                  ERR_CLOCK_NOWRITE    write access failed, no acknowledge
 *                  ERR_OK               success
 *  COMMENT:        File local 'RTCTime' will be updated too, but only
 *                  with value read back and corrected by RTC!
 *********************************************************************** */
ERRCODE TimeDate_SetTime( TIME_TYPE * pTime )
{
    CLOCKTIME   DrvClock;
    ERRCODE     RValue = ERR_CLOCK_NOWRITE;

    // convert data into drivers BCD format
    DrvClock.hours   = VAL2BCD(pTime->bHour);       // 00..23 bcd format
    DrvClock.minutes = VAL2BCD(pTime->bMin);        // 00..59 bcd format
    DrvClock.seconds = VAL2BCD(pTime->bSec);        // 00..59 bcd format

    // save it into RTC via IIC-Bus
    INT_GLOB_DISABLE;                               // disable timer refresh
    RValue = iicSetClockTime( &DrvClock );
    INT_GLOB_ENABLE;                                // re-enable timer refresh
    if (RValue != ERR_OK)
    {
        ODS(DBG_SYS,DBG_WARNING,"TimeDate_SetTime(): Error setting clock time!");
    }
    else
    {
        RTCTime = *pTime;                               // change global date field too
        ODS3(DBG_SYS,DBG_INFO,"TimeDate_SetTime(): Time changed to %02u:%02u:%02u",
                RTCTime.bHour, RTCTime.bMin, RTCTime.bSec);
    }
    return (ERR_OK);
}


/***********************************************************************
 *  FUNCTION:       TimeDate_SetDate
 *  DESCRIPTION:    sets given date into RTC
 *  PARAMETER:      pDate       addr of struct with given date
 *  RETURN:         ERR_PARAM_ERR        invalid parameter
 *                  ERR_CLOCK_NOWRITE    write access failed, no acknowledge
 *                  ERR_OK               success
 *  COMMENT:        File local 'RTCDate' will be updated too, but only
 *                  with value read back and corrected by RTC!
 *********************************************************************** */
ERRCODE TimeDate_SetDate( DATE_TYPE * pDate )
{
    CLOCKDATE   DrvDate;
    ERRCODE     RValue = ERR_CLOCK_NOWRITE;

    // automatic day of week calculation
    TimeDate_GetDayOfWeek(pDate);

    // convert data into drivers BCD format
    DrvDate.date  = VAL2BCD(pDate->bDate);          // 01..31 bcd format
    DrvDate.month = VAL2BCD(pDate->bMonth);         // 01..12 bcd format
    DrvDate.year  = VAL2BCD(pDate->bYear);          // 00..99 bcd format
    DrvDate.day   = VAL2BCD((UINT8)(pDate->eDoW));  // 01..07 bcd format, day of week

    // save it into RTC via IIC-Bus
    INT_GLOB_DISABLE;                               // disable timer refresh
    RValue = iicSetClockDate( &DrvDate );
    INT_GLOB_ENABLE;                                // re-enable timer refresh
    if (RValue != ERR_OK)
    {
        ODS(DBG_SYS,DBG_WARNING,"TimeDate_SetDate(): Error setting clock date!");
    }
    else
    {
        TimeDateUpdateDate();                       // get (corrected) date now
        *pDate = RTCDate;                           // change global date field too
        ODS4(DBG_SYS,DBG_INFO,"TimeDate_SetDate(): Changed Date to %s, %02u.%02u.%02u",
                TimeDate_GetDoWString(TRUE, RTCDate.eDoW), RTCDate.bDate, RTCDate.bMonth, RTCDate.bYear);

    }
    return (ERR_OK);
}



/***********************************************************************
 *  FUNCTION:       TimeDate_SetCalib
 *  DESCRIPTION:    Just a wrapper for iicSetClockCalib()
 *  PARAMETER:      lDuration       calibration measurement duration
 *                                  in seconds
 *                  lDeviation      deviation in seconds
 *  RETURN:         ERR_PARAM_ERR        invalid parameter
 *                  ERR_CLOCK_EXCESS     warning, calibration value is
 *                                       set according to excessively big
 *                                       values, there might be something
 *                                       wrong with the parameters
 *                  ERR_CLOCK_NOWRITE    write access failed, no acknowledge
 *                  ERR_OK               success
 *  COMMENT:        see 'clkdrv.c' for handling
 *********************************************************************** */
ERRCODE TimeDate_SetCalib( INT32 lDuration, INT32 lDeviation )
{
    return iicSetClockCalib( lDuration, lDeviation );
}




/***********************************************************************
 *  FUNCTION:       TimeDate_UpdateCEST
 *  DESCRIPTION:    Just updates the global CEST flag to be used,
 *                  whenever DaylightSaving Automatic is on.
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Should be called, whenever one of the Daylight Saving
 *                  parameters (month, day, hour) have manually been changed!
 *                  Note: This functions expects RTCDate/RTCTime right
 *                  before being updated!
 *********************************************************************** */
void TimeDate_UpdateCEST( void )
{
    // if DaylightSave automatic enabled, update CEST state here too!
    if( gDeviceFlags2.flags.DaylightSaveAuto == TRUE )
    {   gDeviceFlags2.flags.CESTActive = TimeDate_GetCEST();// update CEST state too
        ODS1(DBG_SYS,DBG_INFO,"CEST: %s", (gDeviceFlags2.flags.CESTActive == TRUE)?"ON":"off");
    }
}



/***********************************************************************
 *  FUNCTION:       TimeDate_GetCEST
 *  DESCRIPTION:    Tries to determine, wether current date/time resides
 *                  inside CentralEuropeanSummerTime (CEST)
 *                  between 28.03. 02:00 until 31.10. 3:00.
 *  PARAMETER:      -
 *  RETURN:         TRUE        if current date/time is inside CEST
 *                  FALSE       if not CEST (=CET)
 *  COMMENT:        We distinguish betwwen these 3 time areas:
 *                      a) before beginning of CEST -> return FALSE
 *                      b) inside CEST              -> return TRUE
 *                      c) behind of end of CEST    -> return FALSE
 *
 *                  Note: This functions expects RTCDate/RTCTime right
 *                  before being updated!
 *********************************************************************** */
BOOL TimeDate_GetCEST( void )
{
    BOOL RValue = FALSE;

    // check a): before beginning of CEST?
    if         ( RTCDate.bMonth <  TIME_DLS_SUM_MONTH )
    {   RValue = FALSE;
    }
    else if (  ( RTCDate.bMonth == TIME_DLS_SUM_MONTH )
             &&( RTCDate.bDate  <  TIME_DLS_SUM_DAY   ) )
    {   RValue = FALSE;
    }
    else if (  ( RTCDate.bMonth == TIME_DLS_SUM_MONTH )
             &&( RTCDate.bDate  == TIME_DLS_SUM_DAY   )
             &&( RTCTime.bHour  <  TIME_DLS_SUM_HOUR  ) )
    {   RValue = FALSE;
    }
    else if (  ( RTCDate.bMonth == TIME_DLS_SUM_MONTH )
             &&( RTCDate.bDate  == TIME_DLS_SUM_DAY   )
             &&( RTCTime.bHour  >= TIME_DLS_SUM_HOUR  ) )
    {   RValue = TRUE;
    }

    // check c): after end of CEST?
    else if    ( RTCDate.bMonth >  TIME_DLS_WIN_MONTH )
    {   RValue = FALSE;
    }
    else if (  ( RTCDate.bMonth == TIME_DLS_WIN_MONTH )
             &&( RTCDate.bDate  >  TIME_DLS_WIN_DAY   ) )
    {   RValue = FALSE;
    }
    else if (  ( RTCDate.bMonth == TIME_DLS_WIN_MONTH )
             &&( RTCDate.bDate  == TIME_DLS_WIN_DAY   )
             &&( RTCTime.bHour  >= TIME_DLS_WIN_HOUR  ) )
    {   RValue = FALSE;
    }
    // check b): remaining state must be case b: It's CEST!
    else
    {   RValue = TRUE;
    }
    return (RValue);
}



/***********************************************************************
 *  FUNCTION:       TimeDate_CheckDaylightSaving
 *  DESCRIPTION:    Supports automatic setting of daylight saving for
 *                  Europe (if enabled). Changes RTC Time, if necessary.
 *  PARAMETER:      -
 *  RETURN:         ERR_OK               success
 *  COMMENT:        Should only be called once/hour if RTC-hour changed!
 *
 *                  Note: The surveillance module will generates an additonal Info-Msg
 *                  ONCE at screen to tell user about changes, which will be
 *                  cleared at next power up.
 *
 *                  Note: This functions expects RTCDate/RTCTime right
 *                  before being updated!
 *********************************************************************** */
ERRCODE TimeDate_CheckDaylightSaving( void )
{
    // check: Daylight Saving Automatic enabled by user?
    if (gDeviceFlags2.flags.DaylightSaveAuto == TRUE)
    {
        // check: Change CET => CEST detected?
        if(  gDeviceFlags2.flags.CESTActive == FALSE )
        {
            if ( TimeDate_GetCEST() == TRUE  )
            {   ODS(DBG_SYS,DBG_INFO,"Summertime CEST detected -> set Hour++!");
                fCESTChanged = TRUE;                        // set for surveillance module
                RTCTime.bHour++;                            // incr. 1 hour
                TimeDate_SetTime( &RTCTime );               // setup new time
                gDeviceFlags2.flags.CESTActive = TRUE;      // save new DaylightSaving State
            }
            else
            {   // CEST active and valid -> nothing tbd.
            }
        }
        // check: Change CEST => CET detected?
        else
        {   if ( TimeDate_GetCEST() == FALSE )
            {   ODS(DBG_SYS,DBG_INFO,"Wintertime CET detected -> set Hour--!");
                fCESTChanged = TRUE;                        // set for surveillance module
                RTCTime.bHour--;                            // decr. 1 hour
                TimeDate_SetTime( &RTCTime );               // setup new time
                gDeviceFlags2.flags.CESTActive = FALSE;     // save new DaylightSaving State
            }
            else
            {   // CET active and valid -> nothing tbd.
            }
        }
    }
    else
    {   // DLS automatic not enabled, just let RTC as is...
    }

    return (ERR_OK);
}



/***********************************************************************
 *  FUNCTION:       TimeDate_GetDayOfWeek
 *  DESCRIPTION:    This is a very delicate algorithm to calculate
 *                  the 'day of Week' (Mon,Tue, etc) out of any given
 *                  date. So we don't have the user to handle this
 *                  RTC data.
 *  PARAMETER:      date            to be transferred into day of week
 *  RETURN:         eDayOfWeek      calculated result as enum
 *  COMMENT:        algorithm taken from
 *                  NOTE: does only support date 01.03.1900 .. 28.02.2100
 *                  ;-)
 *
 *  Der Grundalgorithmus ist also ganz einfach:
 *  1.  Zunächst verminderst Du das Jahr um 1, wenn das Datum in einem Januar
 *      oder Februar liegt.
 *  2.  Danach muß das Jahr zwischen 1900 und 2099 liegen, ansonsten kommst
 *      Du nicht direkt zum Ziel.
 *  3.  Ist das der Fall, ziehst Du 1900 vom Jahr ab, vergißt also im
 *      wesentlichen nur die ersten zwei Stellen.
 *  4.  Dann teilst Du das Jahr durch 4, rundest das Ergebnis ab, und addierst
 *      nochmal das Jahr, den Tag und die zum Monat gehörende Zahl hinzu.
 *  5.  Das Ergebnis teilst Du durch 7, so daß ein Teilungsrest von 0 bis 6
 *      übrig bleibt, der dem Wochentag entspricht, 0 etwa korrespondiert zu Sonntag.
 *  6.  Für 1. März 2000 - 28. Februar 2100: Korrektur +6
 *      http://www.math.uni-bonn.de/people/gw/wochentag.html
 *
 *      Das Ergebnis der Formel muss umgerechnet werden auf den DOW_TYPE:
 *      Sonntag: 	0  	-> eSun, 7
 *      Montag: 	1  	-> eMon, 1
 *      Dienstag: 	2  	-> eTue, 2
 *      Mittwoch: 	3   -> eWed, 3
 *      Donnerstag:	4 	-> eThu, 4
 *      Freitag: 	5 	-> eFri, 5
 *      Samstag: 	6   -> eSat, 6
 *
 *********************************************************************** */
DOW_TYPE TimeDate_GetDayOfWeek(DATE_TYPE * pDate)
{
    /* day of week calculations - month-index lookup table
       this is a special lookup table for month-index to be used
       in 'day of week' calcualtion, order: jan..dez */
    UINT8       DoW_Table[] = {1, 4, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
    DOW_TYPE    RValue;
    UINT8       bYear;

    // step 1
    if ( pDate->bMonth < 3)
         bYear = pDate->bYear - 1;  // don't change original value, use year
    else bYear = pDate->bYear;
    RValue  = bYear / 4 ;
    RValue += bYear + pDate->bDate + 6;  // * <- siehe 6.
    RValue += DoW_Table[(pDate->bMonth-1)];
    RValue  = RValue % 7;

    // convert to DOW_TYPE:
    if (RValue == 0)
        RValue = 7;     // So

    // copy result into struct too
    pDate->eDoW = RValue;

    // prepare for debug out
    ODS4(DBG_SYS,DBG_INFO,"TimeDate_GetDayOfWeek(): Day of Week for %02u.%02u.%02u is %s",
        pDate->bDate, pDate->bMonth, pDate->bYear, TimeDate_GetDoWString( FALSE, RValue));
}



