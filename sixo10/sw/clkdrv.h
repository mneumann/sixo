/***********************************************************************
 *
 *  $RCSfile$
 *        
 *  $Revision$
 *
 *  $Date$
 *
 *  $Author$ 
 *
 *  --------------------------------------------------------------------
 *  Company:        N&K Development, all rights reserved
 *
 *                  Neugebauer & Krizsan Cooperation, Germany
 *                  contact:    arnold.neugebauer@web.de
 *                              ralf.krizsan@web.de
 *  --------------------------------------------------------------------
 *  Created:        10-02-2002 by Arnold Neugebauer
 *  Project:        SIxO
 *  Module:         clock driver
 *  Purpose:        clock api export header
 *  Comments:
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
#ifdef DEBUG
   ERRCODE iicTestClockTime( void );
   ERRCODE iicTestClockDate( void );
   ERRCODE iicTestClockCalib( void );
#endif


#endif //_CLKDRV_H
