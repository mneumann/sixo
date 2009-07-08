/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-02-10 by Arnold Neugebauer
 *  Project:        SIxO
 *  Module:         clock driver
 *  Purpose:        clock api export header
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
 ************************************************************************ */

#ifndef _CLKDRV_H
#define _CLKDRV_H


//time of day
typedef struct
{
   UINT8 hours;   //00..23 bcd format
   UINT8 minutes; //00..59 bcd format
   UINT8 seconds; //00..59 bcd format
} CLOCKTIME;

//date
typedef struct
{
   UINT8 date;    //01..31 bcd format
   UINT8 month;   //01..12 bcd format
   UINT8 year;    //00..99 bcd format
   UINT8 day;     //01..07 bcd format, day of week
} CLOCKDATE;

//macros for bcd handling, conversion of 8bit-BCDs
#define LEFT_BCD_DIGIT( byte )   ((byte&(UINT8)0xF0)>>4)
#define RIGHT_BCD_DIGIT( byte )  (byte&(UINT8)0x0F)
#define BCD2VAL( byte )          (((byte&(UINT8)0xF0)>>4)*10+(byte&(UINT8)0x0F))
#define CHECK_BCD( byte )        ((((byte&(UINT8)0xF0)>>4)>9)||((byte&(UINT8)0x0F)>9))
#define VAL2BCD( byte )          ((((UINT8)byte/10)<<4)+((UINT8)byte%10))

//special function bits in the calibration register
#define SPECIAL_OUT  ( (UINT8)0x80 )   //output level of pin FT/OUT if SPECIAL_FT=0
#define SPECIAL_FT   ( (UINT8)0x40 )   //0=disable, 1=enable frequency test with FT/OUT pin
#define SPECIAL_S    ( (UINT8)0x20 )   //sign of calibration value, 0=negative, 1=positive
#define SPECIAL_CALB ( (UINT8)0x1F )   //calibration value

//for iicClockCalibDirect
#define WRITE_CALIB TRUE
#define READ_CALIB  FALSE

//function prototypes
ERRCODE iicSetClockTime( CLOCKTIME *pClktme );
ERRCODE iicGetClockTime( CLOCKTIME *pClktme );
ERRCODE iicSetClockDate( CLOCKDATE *pClkdte );
ERRCODE iicGetClockDate( CLOCKDATE *pClkdte );
ERRCODE iicSetClockCalib( INT32 dwDuration, INT32 dwDeviation );
ERRCODE iicGetClockCalib( INT8* chCalib, INT32* dwSecPerMonth );
ERRCODE iicClockCalibDirect( BOOL bWrite, UINT8* chCalib );

//test functions only present in the debug version
#if(DEBUG==1)
   ERRCODE iicTestClockTime( void );
   ERRCODE iicTestClockDate( void );
   ERRCODE iicTestClockCalib( void );
#endif


#endif //_CLKDRV_H
