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
 *  Module:         -
 *  Purpose:        standard defines for all modules
 *  Comments:       -
 *  Changes:        added DUMMY_OFF and DUMMY_ON, AN 16.2.2003
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
 ************************************************************************ */
#ifndef _STANDARD_H
#define _STANDARD_H


/* DATA TYPES */
typedef signed char     INT8;       /* define INT8 as signed integers */
typedef unsigned char   UINT8;      /* define UINT8 as unsigned integers */

typedef short           INT16;      /* define INT16 as signed integers */
typedef unsigned short  UINT16;     /* define UINT16 as unsigned short  */

typedef unsigned long   UINT32;     /* define UINT32 as unsigned long  */
typedef long            INT32;      /* define INT32 as long  */

typedef unsigned char   CHAR;       /* define CHAR as unsigned char */
typedef unsigned char far *STRING;  /* define STRING as pointer to CHAR */


/* DATA TYPES VALUE RANGES */
#define INT8_MAX    127
#define INT8_MIN    -128
#define INT16_MAX	32767
#define INT16_MIN	-32768
#define INT32_MAX   2147483647L
#define INT32_MIN   -2147483648L
#define INT32_MAX_STR  "2147483647"
#define UINT8_MAX   0xFF
#define UINT16_MAX  0xFFFF
#define UINT32_MAX  0xFFFFFFFFL


/* define NULL */
#ifndef NULL
#define NULL (void *)0
#endif

/* typedefs fuer TRUE und FALSE */
typedef enum
{
    FALSE,
    TRUE
} BOOL;


/* other boolean defines */
#define LEFT	        (unsigned char) 0x0
#define RIGHT	        (unsigned char) 0x1

#define OFF             (unsigned char) 0x0
#define ON              (unsigned char) 0x1

#define DUMMY_OFF       (unsigned char) 0x0
#define DUMMY_ON        (unsigned char) 0x1


/* byte access makros */
#define LOWBYTE(word)   ((UINT8)(((UINT16)word)&0x00ff))
#define HIGHBYTE(word)  ((UINT8)((((UINT16)word)&0xff00)>>8))

/* Minimum/Maximum */
#define MIN(x,y) (x<=y?x:y)
#define MAX(x,y) (x>y?x:y)

/* return number of elements in an array */
#define ARRAY_SIZE(a_) (sizeof(a_) / sizeof(a_[0]))


/* standard type for error handling, return code of most functions */
typedef enum
{
    ERR_OK,
    ERR_NOT_SUPPORTED,
    ERR_OUT_OF_RANGE,
    ERR_PARAM_ERR,
    ERR_NO_SUCH_ID,
    ERR_WRONG_STATE,
    ERR_NO_MEM,
    ERR_TIMEOUT,

    ERR_MSGQUEUE_NO_WRITE,
    ERR_MSGQUEUE_MAX_FULL,
    ERR_MSGQUEUE_NO_MSGS,
    ERR_MSGQUEUE_NO_ID,
    ERR_MSG_NOT_PROCESSED,
    ERR_MSG_PROCESSED,

    ERR_TIMER_NO_FUNC,
    ERR_TIMERMSG_NOT_FOUND,
    ERR_TIMERMSG_FULL,

    ERR_EEPROM_NOWRITE,
    ERR_EEPROM_NOREAD,

    ERR_CLOCK_NOWRITE,
    ERR_CLOCK_NOREAD,
    ERR_CLOCK_EXCESS,   //this is a warning, may be ignored
    ERR_NVRAM_NOWRITE,
    ERR_NVRAM_NOREAD
} ERRCODE;


#endif // _STANDARD_H
