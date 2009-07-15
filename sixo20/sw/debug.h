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
 *  Purpose:        debug out export api header
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
 * Revision 2.2  2009/07/15 09:02:50  tuberkel
 * D-Options reviewed
 *
 * Revision 2.1  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 * Revision 1.5  2006/02/18 15:01:33  Ralf
 * - added history
 *
 *
 ************************************************************************ */


#ifndef _DEBUG_H
#define _DEBUG_H




/* ------------------------------------------------------------------
   DEBUG FILTER DEFINES:

   controls debug out filter:
        - ERROR LEVEL (low nibble) and
        - MODULE (high nibble)

   to be used together with module bit mask high nibble in ODS() macro) */

/* debug filter defines */
#define DBG_FATAL     0x01  /* enable debug out of fatal-errors     (f.e. system crash) */
#define DBG_ERROR     0x02  /* enable debug out of errors           (f.e. unexpected functional errors) */
#define DBG_WARNING   0x04  /* enable debug out of warnings         (f.e. for potential runtime problems) */
#define DBG_INFO      0x08  /* enable debug out of information      (f.e. status info */

#define DBG_DRV       0x10  /* low level driver functions (f.e. eeprom, display, led..) */
#define DBG_SYS       0x20  /* system device (f.e. messages, tasks, resources etc..) */
#define DBG_USER      0x40  /* any kind of user action/reaction (f.e. keyboard, menue, etc. */
#define DBG_MEAS      0x80  /* any kind of measurement unit */

/* combinations of above */
#define DBG_OFF       0x00  /* switch off debug out strings */
#define DBG_ALL       0xff  /* switch on aLL debug strings */
#define DBG_STD       0xf7  /* switch on all but info debug strings */


/* ------------------------------------------------------------------
    DEBUG DETAILS DEFINES:

    control debug out printf
        - DETAILS (low nibble) and
        - DIRECTION (high nibble)
    to be used to control debug out system load */
/* debug details defines */
#define DBG_NODETAIL  0x00  /* disables use of additional details */
#define DBG_LVL       0x01  /* enables use of additional level info */
#define DBG_DEV       0x02  /* enables use of additional device info */
#define DBG_TIM       0x04  /* enables use of additional time info */
#define DBG_SRC       0x08  /* enables use of additional source info */
#define DBG_UART      0x10  /* enables debug outs to m16c uart */
#define DBG_DSPL      0x20  /* enables debug outs to display) */
#define DBG_RAM       0x40  /* not yet implemented (enables debug logging into ram) */
#define DBG_SEC       0x80  /* not yet implemented (enables logging of second ticks) */

#define DEF_DBGDET DBG_NODETAIL
#define DEF_DBGDIR DBG_UART


/* ------------------------------------------------------------------ */
/* standard usage */
#if(DEBUG==1)
#define DEF_DBGFILT   DBG_ALL                       // debug filter settings
#define DEF_DBGDETDIR (DEF_DBGDET | DEF_DBGDIR)     // debug details & directions setting
#else
#define DEF_DBGFILT   DBG_OFF                       // debug filter settings
#define DEF_DBGDETDIR (DEF_DBGDET | DEF_DBGDIR)     // debug details & directions setting
#endif


/* byte/bitfield for easier access */
typedef union
{
    UINT8 byte;
    struct
    {
        unsigned char fFatal:1;   /* enable debug out of fatal-errors     (f.e. system crash) */
        unsigned char fError:1;   /* enable debug out of errors           (f.e. unexpected functional errors) */
        unsigned char fWarning:1; /* enable debug out of warnings         (f.e. for potential runtime problems) */
        unsigned char fInfo:1;    /* enable debug out of information      (f.e. status info */
        unsigned char fDrv:1;     /* low level driver functions (f.e. eeprom, display, led..) */
        unsigned char fSys:1;     /* system device (f.e. messages, tasks, resources etc..) */
        unsigned char fUser:1;    /* any kind of user action/reaction (f.e. keyboard, menue, etc. */
        unsigned char fMeas:1;    /* any kind of measurement unit */
    } flags;
} DBGFILT_TYPE;



/* byte/bitfield for easier access */
typedef union
{
    UINT8 byte;
    struct
    {
        unsigned char fLevel:1;   /* enables use of additional level info */
        unsigned char fDevice:1;  /* enables use of additional device info */
        unsigned char fTime:1;    /* enables use of additional time info */
        unsigned char fSource:1;  /* enables use of additional source info */
        unsigned char fUart:1;    /* enables debug outs to m16c uart */
        unsigned char fDisplay:1; /* enables debug outs to display) */
        unsigned char fRam:1;     /* not yet implemented (enables debug logging into ram) */
        unsigned char fTicks:1;   /* not yet implemented (enables logging of second ticks) */
    } flags;
} DBGDETDIR_TYPE;





/* display debug buffer */
#define DBG_DISPLAYLINES   7   /* 7 lines to display debug out on lcd! */
#define DBG_DISPLAYCOLUMN  33  /* 32 character at a line to display debug out on lcd! */


/* function prototypes */
ERRCODE DebugInitHW(void);
ERRCODE DebugInit(DBGFILT_TYPE bFilter, DBGDETDIR_TYPE bDetails);
ERRCODE DebugExit(void);
void    DebugTimerEntry(void);
ERRCODE DebugSetFilterDetails(DBGFILT_TYPE bFilter, DBGDETDIR_TYPE bDetails);
ERRCODE DebugOut(STRING txt);
ERRCODE DebugTest(void);
ERRCODE DebugPrint(UINT8 chLevel, STRING szText, UINT16 wSrcLine, STRING szSrcFile, ...);


/* ODS macros to be used in source modules */
#if(DEBUG==1)
#define ODS(dev,lvl,txt)                (void)DebugPrint(dev|lvl, txt, (UINT16)__LINE__, __FILE__)
#define ODS1(dev,lvl,txt,a)             (void)DebugPrint(dev|lvl, txt, (UINT16)__LINE__, __FILE__, a)
#define ODS2(dev,lvl,txt,a,b)           (void)DebugPrint(dev|lvl, txt, (UINT16)__LINE__, __FILE__, a,b)
#define ODS3(dev,lvl,txt,a,b,c)         (void)DebugPrint(dev|lvl, txt, (UINT16)__LINE__, __FILE__, a,b,c)
#define ODS4(dev,lvl,txt,a,b,c,d)       (void)DebugPrint(dev|lvl, txt, (UINT16)__LINE__, __FILE__, a,b,c,d)
#define ODS5(dev,lvl,txt,a,b,c,d,e)     (void)DebugPrint(dev|lvl, txt, (UINT16)__LINE__, __FILE__, a,b,c,d,e)
#define ODS6(dev,lvl,txt,a,b,c,d,e,f)   (void)DebugPrint(dev|lvl, txt, (UINT16)__LINE__, __FILE__, a,b,c,d,e,f)
#define ODS7(dev,lvl,txt,a,b,c,d,e,f,g) (void)DebugPrint(dev|lvl, txt, (UINT16)__LINE__, __FILE__, a,b,c,d,e,f,g)

#else /* uC release */
/* tricky preprocessing with ##: reference another macro with first parameter in name */
#define ODS(dev,lvl,txt)
#define ODS1(dev,lvl,txt,a)
#define ODS2(dev,lvl,txt,a,b)
#define ODS3(dev,lvl,txt,a,b,c)
#define ODS4(dev,lvl,txt,a,b,c,d)
#define ODS5(dev,lvl,txt,a,b,c,d,e)
#define ODS6(dev,lvl,txt,a,b,c,d,e,f)
#define ODS7(dev,lvl,txt,a,b,c,d,e,f,g)
#endif


/* @func | ASSERT	|  halt with debug message if result is zero .
 * @parm Pointer |	n	|     Pointer: if NULL ->  DEBUG_FATAL .
 */
#if(DEBUG==1)
#define ASSERT(n) /*lint -save -e506 -e792*/ \
        if (!(n)) { ODS(DBG_FATAL, 0, "Assertion: program halted"); InitFatalError((ERRCODE)(n),"Assert");}\
                           /*lint -restore*/
#else
#define ASSERT(n) /* release version: no code */
#endif


/* end of header file definitions */
#endif /* _DEBUG_H */
