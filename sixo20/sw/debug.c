/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-10-25 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         debug
 *  Purpose:        debug out to uart0/1
 *  Comments:       -
 *
 *  --------------------------------------------------------------------
 *
 *  Compiler:       Renesas NC30WA V.5.00 Release 2
 *  Options:        if KD30_USED is defined, debug out is given to
 *                  uart0, else debug out uses uart1
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
 * Revision 3.2  2012/02/08 03:41:30  tuberkel
 * renamed #define MINIEMU ==> KD30_USED
 *
 * Revision 3.1  2010/11/07 09:18:26  tuberkel
 * Revision History added
 *

 ************************************************************************ */


#include <sfr62p.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "xtoa.h"
#include "standard.h"
#include "debug.h"
#include "timer.h"
#include "display.h"
#include "fonts.h"
#include "displdrv.h"



#define LEN_SEND_BUFFER 80                  /* buffer to handle debug string */

DBGFILT_TYPE    gDebugFilter = DEF_DBGFILT;     /* default off, use DebugSetFilterDetails() to change */
DBGDETDIR_TYPE  gDebugDetails = DEF_DBGDETDIR;  /* default uart, use DebugSetFilterDetails() to change */

//unsigned char szDispDbgBuff[DBG_DISPLAYCOLUMN][DBG_DISPLAYLINES];
                                        /* 7 lines á 32 character lcd debug screen */

extern   UINT16 wMilliSecCounter;     /* valid values: 0h .. ffffh */
extern   UINT16 wSecCounter;          /* valid values: 0h .. ffffh */


/***********************************************************************
 *        FUNCTION:       DebugSetFilterDetails()
 *        DESCRIPTION:    Sets debug filter bit mask
 *        PARAMETER:      UINT8 bFilter     new debug level & device mask
 *                        UINT8 bDetails    set debug out details
 *        RETURN:         ERR_OK if succesfull
 *                        ERR_OUT_OF_RANGE if illegal level
 *        COMMENT:        -
 *********************************************************************** */
ERRCODE DebugSetFilterDetails(DBGFILT_TYPE bFilter, DBGDETDIR_TYPE bDetails)
{
    ERRCODE wErrorCode = ERR_OK;            /* return value */
#if(DEBUG==1)
    CHAR    szFilter[100] = "";
    STRING  szLevel;
    STRING  szDevice;
    STRING  szDetails;
    CHAR    cMask;
    CHAR    i;
    CHAR    cCount;

    //ODS2(DBG_SYS, DBG_INFO, "DebugSetFilterDetails(): Filter: 0x%x Details: 0x%x", bFilter, bDetails);
    /* store global copy */
    gDebugFilter = bFilter;
    gDebugDetails = bDetails;

    /* ----------------------------------------------------------- */
    /* print out debug DEVICE SETTINGS in plain text */
    cMask = 0x10;                   /* begin with lowest of 4 device bits */
    cCount = 0;                     /* number of items */
    szFilter[0] = 0x0;              /* clear ouput buffer */

    /* check for validity */
    if ((0xf0 & gDebugFilter.byte) == 0x0)
    {
        szDevice = "NO DEVICES";
        strcat( (STRING) szFilter, (STRING) szDevice);
    }
    else
    {
        /* check all device bits */
        for ( i = 0; i < 4; i++ )
        {
            switch (cMask & gDebugFilter.byte)
            {
                case DBG_USER:
                    szDevice = "DBG_USER";
                    break;
                case DBG_DRV:
                    szDevice = "DBG_DRV";
                    break;
                case DBG_MEAS:
                    szDevice = "DBG_MEAS";
                    break;
                case DBG_SYS:
                    szDevice = "DBG_SYS";
                    break;
                default:
                    szDevice = "";
            } /* of switch cMask.. */
            if ( *szDevice != 0 )                       /* count items */
                cCount++;
            if ( (cCount > 1) && (*szDevice != 0) )     /* insert intermediate part */
                strcat( (STRING) szFilter, " | ");
            strcat( (STRING) szFilter, (STRING) szDevice);
            cMask = cMask << 1;                         /* lshift bit through mask */
        } /* of i loop */
    }

    /* print out line */
    wErrorCode = DebugOut("#\r\n");
    wErrorCode = DebugOut("# Debug Devices: ");
    wErrorCode = DebugOut( szFilter );
    wErrorCode = DebugOut("\r\n");


    /* ----------------------------------------------------------- */
    /* print out debug DEBUG LEVEL in plain text */
    cMask = 0x01;                   /* begin with lowest of 4 level bit */
    cCount = 0;                     /* number of items */
    szFilter[0] = 0x0;              /* clear ouput buffer */

    /* check for validity */
    if ((0x0f & gDebugFilter.byte) == 0x0)
    {
        szLevel = "NO LEVEL";
        strcat( (STRING) szFilter, (STRING) szLevel);
    }
    else
    {
        for ( i = 0; i < 4; i++ )
        {
            switch (cMask & gDebugFilter.byte)
            {
                case DBG_FATAL:
                    szLevel = "DBG_FATAL";
                    break;
                case DBG_ERROR:
                    szLevel = "DBG_ERROR";
                    break;
                case DBG_WARNING:
                    szLevel = "DBG_WARNING";
                    break;
                case DBG_INFO:
                    szLevel = "DBG_INFO";
                    break;
                default:
                    szLevel = "";
            } /* of switch cMask.. */
            if ( *szLevel != 0 )                       /* count items */
                cCount++;
            if ( (cCount > 1) && (*szLevel != 0) )     /* insert intermediate part */
                strcat( (STRING) szFilter, " | ");
            strcat( (STRING) szFilter, (STRING) szLevel);
            cMask = cMask << 1;                         /* lshift bit through mask */
        } /* of i loop */
    }

    /* print out line */
    szLevel = szFilter;            /* READY: reset to target output buffer */
    wErrorCode = DebugOut("# Debug Filter:  ");
    wErrorCode = DebugOut( szLevel );
    wErrorCode = DebugOut("\r\n");

    /* ----------------------------------------------------------- */
    /* print out debug DEBUG DEATILS in plain text */
    cMask = 0x01;                   /* begin with lowest of 4 detail bits */
    cCount = 0;                     /* number of items */
    szFilter[0] = 0x0;              /* clear ouput buffer */

    /* check for validity */
    if ((0x0f & gDebugDetails.byte) == 0x0)
    {
        szDetails = "NO DETAILS";
        strcat( (STRING) szFilter, (STRING) szDetails);
    }
    else
    {
        for ( i = 0; i < 4; i++ )
        {
            switch (cMask & gDebugDetails.byte)
            {
                case DBG_LVL:
                    szDetails = "DBG_LVL";
                    break;
                case DBG_DEV:
                    szDetails = "DBG_DEV";
                    break;
                case DBG_TIM:
                    szDetails = "DBG_TIM";
                    break;
                case DBG_SRC:
                    szDetails = "DBG_SRC";
                    break;
                default:
                    szDetails = "";
            } /* of switch cMask.. */
            if ( *szDetails != 0 )                      /* count items */
                cCount++;
            if ( (cCount > 1) && (*szDetails != 0) )     /* insert intermediate part */
                strcat( (STRING) szFilter, " | ");
            strcat( (STRING) szFilter, (STRING) szDetails);
            cMask = cMask << 1;                         /* lshift bit through mask */
        } /* of i loop */
    }

    /* print out line */
    szDetails = szFilter;            /* READY: reset to target output buffer */
    wErrorCode = DebugOut("# Debug Details: ");
    wErrorCode = DebugOut( szDetails );
    wErrorCode = DebugOut("\r\n");
#endif
    return wErrorCode;
}


/***********************************************************************
 *  FUNCTION:       DebugInitHW(void)
 *  DESCRIPTION:    Initializes m16 ports direction register
 *                  for uarts use
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE DebugInitHW(void)
{
#if(DEBUG==1)
    /* port 6 usage as uart */
#endif
    return ERR_OK;
}


/***********************************************************************
 *        FUNCTION:       DebugInit(void)
 *        DESCRIPTION:    Initializes Debugger
 *        PARAMETER:      UINT8 bFilter     new debug level & device mask
 *                        UINT8 bDetails    new debug details
 *        RETURN:         ERR_OK if succesfull or return code of UART
 *        COMMENT:        -
 *********************************************************************** */
ERRCODE DebugInit(DBGFILT_TYPE bFilter, DBGDETDIR_TYPE bDetails)
{
   ERRCODE wErrorCode = ERR_OK;            /* return value */

#if(DEBUG==1)
   wErrorCode = DebugInitHW();
   wErrorCode = DebugOut("\r\n\r\n# ----------------------------------------\r\n");
   wErrorCode = DebugOut("# WELCOME TO SIxO DEBUGGING ! \r\n");
   DebugSetFilterDetails(bFilter,bDetails);
   #if(KD30_USED==1)
      wErrorCode = DebugOut("# Debug Port:    UART0 \r\n");
   #else
      wErrorCode = DebugOut("# Debug Port:    UART1 \r\n");
   #endif /* KD30_USED */
   wErrorCode = DebugOut("# now starting..\r\n\r\n");
#endif /* DEBUG */
   return wErrorCode;
}



 /***********************************************************************
 *        FUNCTION:       DebugExit(void)
 *        DESCRIPTION:    Detach Debugger
 *        PARAMETER:      None
 *        RETURN:         ERR_OK
 *        COMMENT:        -
 *********************************************************************** */
ERRCODE DebugExit(void)
{
   return ERR_OK;
}



 /***********************************************************************
 *        FUNCTION:       DebugTimerEntry(void)
 *        DESCRIPTION:    Timer entry function of debugger
 *        PARAMETER:      None
 *        RETURN:         None
 *        COMMENT:        Used if periodical debug needed
 *********************************************************************** */
void DebugTimerEntry(void)
{
    /* Debug might be called periodically */
}




 /***********************************************************************
 *        FUNCTION:       DebugPrint()
 *        DESCRIPTION:    Output function of debugger,
 *                        called by ODS() macros only!
 *        PARAMETER:      UINT8 chLevel      See debug.h for possible values
 *                        UINT16 wID        ID of debug output
 *                        STRING szText     Output string
 *                        UINT16 sSrcLine   Source code line of macro
 *                        STRING szSrcFile  Source file
 *
 *                        ...               variable parameter list!
 *
 *        RETURN:         ERR_OK if succesfull
 *                        ERR_OUT_OF_RANGE: Wrong debug level
 *        COMMENT:        Used if periodical debug needed

                          ATTENTION:  This is for field test only,
                                      consumes runtime and memory!

 *********************************************************************** */
ERRCODE DebugPrint( UINT8 chLevel, STRING szText, UINT16 wSrcLine, STRING szSrcFile, ...)
{
    va_list marker;                         /* optional argument list */
    ERRCODE ErrorCode = ERR_OK;
    static  BOOL fReentranceLock = FALSE;
    CHAR    DbgTextBuffer[LEN_SEND_BUFFER]; /* place for debug message to be generated */
    STRING  szDbgText;                      /* debug string */
    UINT16  wLen;
    UINT16  uiCurrentTime_ms;               /* current system time in ms */
    UINT16  uiCurrentTime_sec;              /* current system time in sec */

    /* check wether function already in use */
    if ( fReentranceLock )
    {
        return ERR_OK;                      /* discard if reentring */
    }
    fReentranceLock = TRUE;                 /* no more calls to myself allowed */

    /* filter debug level & device */
    if (  ((gDebugFilter.byte & (chLevel & 0x0f)) == 0)      /* check level */
        ||((gDebugFilter.byte & (chLevel & 0xf0)) == 0) )    /* check device */
    {
        ErrorCode =  ERR_OK;                /* ignore, no output */
        goto DebugPrintExit;
    }

    /* build the "introduction" with
       device, level, filename+line, timestamp */

    /* build the level */
    if (gDebugDetails.byte & DBG_LVL)
    {
        szDbgText = DbgTextBuffer;              /* init */
        switch (chLevel & 0x0f)
        {
            case DBG_FATAL:
                szDbgText = "### FATAL:";
                break;
            case DBG_ERROR:
                szDbgText = "** ERROR:";
                break;
            case DBG_WARNING:
                szDbgText = "+ WARN:";
                break;
            case DBG_INFO:
                szDbgText = "INFO:";
                break;
            default:
                ErrorCode =  ERR_OUT_OF_RANGE;     /* more than on bit set */
                goto DebugPrintExit;
        } /* of switch */
        if (DebugOut(szDbgText) != ERR_OK)
            goto DebugPrintExit;
    } /* if DBG_LVL */

    /* build the device */
    if (gDebugDetails.byte & DBG_DEV)
    {
        szDbgText = DbgTextBuffer;
        switch (chLevel & 0xf0)
        {
            case DBG_USER:
                szDbgText = "USR:";
                break;
            case DBG_DRV:
                szDbgText = "DRV:";
                break;
            case DBG_MEAS:
                szDbgText = "MSR:";
                break;
            case DBG_SYS:
                szDbgText = "SYS:";
                break;
            default:
                ErrorCode =  ERR_OUT_OF_RANGE;     /* more than on bit set */
                goto DebugPrintExit;
        } /* of switch */
        if (DebugOut(szDbgText) != ERR_OK)
            goto DebugPrintExit;
    } /* if DBG_DEV */

    /* build the time info */
    if (gDebugDetails.byte & DBG_TIM)
    {
        szDbgText = DbgTextBuffer;
        /* get time stamp, ms without seconds */
        TimerGetSys_msec(uiCurrentTime_ms);
        TimerGetSys_sec(uiCurrentTime_sec);
        sprintf(    szDbgText, "[%d.%03ds]",
                    uiCurrentTime_sec,
                    uiCurrentTime_ms - (uiCurrentTime_ms/1000)*1000 );
        if (DebugOut(szDbgText) != ERR_OK)
            goto DebugPrintExit;
    }

    /* build the source info */
    if (gDebugDetails.byte & DBG_SRC)
    {
        szDbgText = DbgTextBuffer;
        /* skip the path info, if any */
        if ( strrchr(szSrcFile, '\\') != NULL )
            szSrcFile = strrchr(szSrcFile, '\\') + 1;
        sprintf(szDbgText,"(%s/%d)", szSrcFile, wSrcLine);
        if (DebugOut(szDbgText) != ERR_OK)
            goto DebugPrintExit;
    }

    /* compose message string with optional arguments */
    szDbgText = DbgTextBuffer;
    va_start(marker, szSrcFile);
    vsprintf(szDbgText, szText, marker);
    wLen = strlen(szDbgText);
    if (wLen > 0 && szDbgText[wLen-1] != '\n')
    {
        szDbgText[wLen]   = '\r'; /* add <CR><LF> if missing */
        szDbgText[wLen+1] = '\n';
        szDbgText[wLen+2] = '\0';
    }
    else if(wLen > 1 && szDbgText[wLen-1] == '\n' && szDbgText[wLen-2] != '\r')
    {
        szDbgText[wLen-1] = '\r'; /* replace \n with <CR><LF> */
        szDbgText[wLen]   = '\n';
        szDbgText[wLen+1] = '\0';
    }

    /* send user part of debug string */
    ErrorCode = DebugOut(szDbgText); // output message string
    if (ErrorCode != ERR_OK)
    {
        goto DebugPrintExit;
    }

DebugPrintExit:
    fReentranceLock = FALSE; // calls to this function allowed again
    return ErrorCode;
}



/***********************************************************************
 *  FUNCTION:       DebugOut
 *  DESCRIPTION:    calls standard printf() to put out debug string
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE DebugOut(STRING szDbgText)
{
    ERRCODE ErrorCode = ERR_OK;
    static UINT8        bDbgLine = 0;       // initial value
    STRING szEmptyLine = "                                ";

    /* printf() to PC debug out via uart */
    if ( (gDebugDetails.byte & DBG_UART) )
    {
        if (printf(szDbgText) == EOF)
            ErrorCode = 1;  /* error */
    }

    /* printf to SIxOs display */
    if ( (gDebugDetails.byte & DBG_DSPL) )
    {
        /* calculate ouput position */
        DPLFONT DbgFont = DPLFONT_4X6;
        DISPLXY pCoord;
        pCoord.wXPos = 0;                                               // string left aligned
        pCoord.wYPos = GetFontHeight(DbgFont) * bDbgLine;               // use adequat line

        /* print out string */
        DisplPrintAString( szEmptyLine, &pCoord, DbgFont, DPLNORM );    // clear line
        DisplPrintAString( szDbgText, &pCoord, DbgFont, DPLNORM );      // show dbgtext
        if (++bDbgLine >= DBG_DISPLAYLINES)
            bDbgLine = 0;
    }
    return ErrorCode;
}


/***********************************************************************
 *  FUNCTION:
 *  DESCRIPTION:    test debug out to uart1
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE DebugTest(void)
{
    static unsigned int iloop = 1000;

    iloop--;
    if ((iloop % 1000) == 0)
    {
        ODS(DBG_SYS, DBG_INFO,"-------------------------");
        ODS(DBG_SYS,DBG_FATAL,"System Fataler Fehler!");
        ODS1(DBG_MEAS,DBG_ERROR,"Messgerät Fehler %d", 1 );
        ODS2(DBG_USER,DBG_WARNING,"Benutzer Warnung %d %x", 1, 2);
        ODS3(DBG_DRV,DBG_INFO,"Treiber Info %d %x %s", 1, 2, "Dasnehartenuss...");
        ODS(DBG_SYS, DBG_INFO,"-------------------------");
    }
}