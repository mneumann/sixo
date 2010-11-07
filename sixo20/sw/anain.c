/***********************************************************************
 *
 *  $RCSfile: anain.c
 *
 *  $Revision$
 *
 *  $Date$
 *
 *  Author: Markus Mueller, mm@rsj.de
 *
 *  --------------------------------------------------------------------
 *  Created:        03-30-2004 by Markus Mueller
 *  Project:        SIxO 2.0
 *  Module:         anain.c
 *  Purpose:        high level stuff for A/D measurement
 *  Comments:       -
 *  Changes:        revision, changes in architecture - RK 10-28-2004
 *                  tons of changes, correct filter calculations - AN 11-04-2004
 *                  separate scale factors, correted volt calcs, - AN 11-06-2004
 *                  added AnaInGetAltwVoltage
 *                  new NTC compensation table - AN 11-07-2004
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
 * Revision 3.0  2010/11/07 14:00:50  tuberkel
 * V30 Preparations - no changes
 *
 * Revision 2.6  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.5  2009/07/08 21:41:17  tuberkel
 * All compiler defines reviewed:
 * - set to unique usage: set define to 0 or 1
 * - default values set, if not used
 * - see 'Project Editor' for details
 *
 * Revision 2.4  2007/03/30 10:12:00  tuberkel
 * - Matthias Otto stuff:  language specific display content
 *
 * Revision 2.3  2007/03/26 23:14:38  tuberkel
 * just comments
 *
 * Revision 2.2  2006/10/19 18:52:26  tuberkel
 * BugFix: ANAIN_TEMP_OFFSET -> ANAIN_TEMP_SENSORDETECT
 * - now uses values <-20° to detect missing temp sensor
 *
 * Revision 2.1  2006/07/20 22:56:34  tuberkel
 * Added different filters LDR sensor:
 * - fast detection of darkness
 * - slowly detection of brightness
 * This helps quickly switching on backlight e.g. under bridges
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 * Revision 1.15  2006/02/18 14:19:19  Ralf
 * - check of sensor availablilty imrpoved in AnaInFormatTemperature()
 *
 ************************************************************************ */


#include <stdio.h>
#include <string.h>
#include "standard.h"
#include "timer.h"
#include "sfr62p.h"
#include "debug.h"
#include "measdrv.h"
#include "digoutdr.h"
#include "msgqueue.h"
#include "anaindrv.h"
#include "anain.h"
#include "surveill.h"
#include "resource.h"


/* Temperature LookUp Table 'aucNTCCompTable[]' ------------------------

    Arnold:     Calculated values based on the table in the datasheet with
                logarithmic approximation of the intermediate values.
                Index 0 is manually set to 180°C, theoretically Index 0
                can't be reached. Index 213..255 have been manually set
                to -40°C, the lowest value in the given range.
                However, you probably will be dead before you see these
                temperatures displayed.

    Content:    LookUp Table for SIxO-internal NTC B57164-K103,
                which is used for display contrast compensation and
                to show SIxO's internal temperature ;-)

    Principle:  1. Maps any AD-sample raw values into °C with Offset -40°C,
                2. The AD-sample is the direct index into this lookup table
                3. Integrates complete temperature measurement with NTC
                   characteristic, HW input design, AD conversion etc up
                   to 'real' physical temperatur value.
                4. Accuracy might be point of discussion. ;-)
                5. Example:
                        -> ADSample           = 0
                        -> aucNTCCompTable[0] = 220
                        -> real Temp          = 220 - 40
                                              = 180°C
                          ( which is really hot ;-) )

  ---------------------------------------------------------------------- */
static const UINT8 aucNTCCompTable[256] = {
   220, 208, 180, 165, 155, 147, 141, 136,
   132, 128, 125, 122, 120, 117, 115, 113,
   111, 109, 108, 106, 105, 103, 102, 101,
   100,  98,  97,  96,  95,  94,  93,  92,
    91,  91,  90,  89,  88,  87,  87,  86,
    85,  84,  84,  83,  82,  82,  81,  81,
    80,  79,  79,  78,  78,  77,  77,  76,
    76,  75,  75,  74,  74,  73,  73,  72,
    72,  71,  71,  70,  70,  69,  69,  69,
    68,  68,  67,  67,  67,  66,  66,  65,
    65,  65,  64,  64,  63,  63,  63,  62,
    62,  62,  61,  61,  61,  60,  60,  59,
    59,  59,  58,  58,  58,  57,  57,  57,
    56,  56,  56,  55,  55,  55,  54,  54,
    54,  54,  53,  53,  53,  52,  52,  52,
    51,  51,  51,  50,  50,  50,  50,  49,
    49,  49,  48,  48,  48,  47,  47,  47,
    47,  46,  46,  46,  45,  45,  45,  44,
    44,  44,  44,  43,  43,  43,  42,  42,
    42,  41,  41,  41,  41,  40,  40,  40,
    39,  39,  39,  38,  38,  38,  38,  37,
    37,  37,  36,  36,  36,  35,  35,  35,
    34,  34,  34,  33,  33,  33,  32,  32,
    32,  31,  31,  31,  30,  30,  30,  29,
    29,  29,  28,  28,  28,  27,  27,  26,
    26,  26,  25,  25,  25,  24,  24,  23,
    23,  22,  22,  22,  21,  21,  20,  20,
    19,  19,  18,  18,  17,  17,  16,  16,
    15,  14,  14,  13,  13,  12,  11,  11,
    10,   9,   8,   8,   7,   6,   5,   4,
     3,   2,   1,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0
};

//checksum: simple sum of all values of aucNTCCompTable[]
#define NTC_COMP_TABLE_CHECKSUM 13757


/* Temperature LookUp Table 'vdo_temp_table[]' ------------------------

    Arnold:     Compensation table for VDO sensors with characteristic curve 2.
                These magic values have been compiled by measurements with a
                reference system and a little hand editing.

    Content:    LookUp Table for any standard VDO Temperature Sensor with
                characteristic curve 2. It might be used for oil, water or
                ambient air temperature.

    Principle:  1. Maps any AD-sample raw values into °C with -40°C Offset!
                2. The AD-sample is the direct index into this lookup table
                3. Integrates complete temperature measurement with NTC
                   characteristic, HW input design, AD conversion etc up
                   to 'real' physical temperatur value.
                4. Accuracy might be point of discussion. ;-)
                5. Example:
                        -> ADSample           = 15
                        -> aucVDOCompTable[15]= 196
                        -> real Temp          = 196 - 40
                                              = 154 °C
                          ( which is really hot ;-) )

  ---------------------------------------------------------------------- */
static const UINT8 aucVDOCompTable[256] = {
   220, 220, 220, 220, 220, 220, 220, 220,
   220, 220, 220, 216, 211, 206, 199, 196,
   192, 192, 189, 186, 182, 179, 177, 175,
   173, 173, 171, 169, 167, 165, 164, 161,
   160, 159, 157, 156, 155, 153, 152, 151,
   150, 149, 148, 147, 146, 145, 144, 143,
   142, 141, 140, 139, 138, 137, 137, 136,
   135, 134, 133, 133, 132, 131, 130, 130,
   129, 128, 128, 127, 126, 126, 125, 124,
   124, 123, 122, 122, 121, 120, 120, 119,
   118, 118, 117, 117, 116, 116, 115, 114,
   114, 113, 113, 112, 112, 111, 111, 111,
   110, 110, 109, 109, 108, 108, 107, 107,
   106, 106, 105, 105, 104, 104, 103, 103,
   102, 102, 101, 101, 101, 100, 100,  99,
    99,  98,  98,  98,  97,  97,  96,  96,
    95,  95,  94,  94,  94,  93,  93,  92,
    92,  91,  91,  91,  90,  90,  89,  89,
    89,  88,  88,  87,  87,  86,  86,  86,
    85,  85,  84,  84,  83,  83,  83,  82,
    82,  81,  81,  81,  80,  80,  79,  79,
    79,  78,  78,  77,  77,  76,  76,  75,
    75,  75,  74,  74,  73,  73,  72,  72,
    71,  71,  70,  70,  70,  69,  69,  68,
    68,  67,  67,  66,  66,  65,  65,  64,
    63,  63,  62,  62,  61,  61,  60,  59,
    59,  58,  58,  57,  56,  56,  55,  54,
    54,  53,  52,  51,  51,  50,  49,  49,
    48,  48,  47,  47,  46,  45,  44,  43,
    42,  41,  40,  40,  39,  37,  36,  35,
    33,  32,  30,  29,  27,  25,  22,  20,
    19,  18,  14,   9,   3,   0,   0,   0
};
//checksum: simple sum of all values of aucVDOCompTable[]
#define VDO_COMP_TABLE_CHECKSUM 26031


// private data
static UINT16 usADPhysValues[ANAIN_LAST]; //AD conversion physical values
static UINT8  ADChannelInitBits;          //each bit=1 indicates an initialized channel (LSB = ANAIN_EXTU)


//private prototypes
#if(DEBUG==1)
BOOL AnaInCRCCheckTables( void );
#endif


/***********************************************************************
 *  FUNCTION:       AnaInInit
 *  DESCRIPTION:    initialisation of AD converter interface
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:
 *********************************************************************** */
ERRCODE AnaInInit( void )
{
   //initialize low level driver
   AnaInDrvInit();

   //mark all channels as uninitialized
   ADChannelInitBits = 0;
   memset ( usADPhysValues, ANAIN_INVALID_U, sizeof(usADPhysValues));

#if(DEBUG==1)
   //check compensation tables in debug mode only
   if( AnaInCRCCheckTables() == FALSE ){
      ODS( DBG_DRV, DBG_INFO, "AnaInInit() FAILED!" );
      return ERR_OK;
   }
#endif

    ODS( DBG_DRV, DBG_INFO, "AnaInInit() done!" );
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       AnaInCRCCheckTables
 *  DESCRIPTION:    used to assure un-manipulated lookup tables
 *  PARAMETER:      -
 *  RETURN:         TRUE  - everything ok
 *                  FALSE - compensation tables are faulty
 *  COMMENT:
 *********************************************************************** */
#if(DEBUG==1)
BOOL AnaInCRCCheckTables ( void )
{
   UINT16 usIdx;
   UINT16 usChecksum;

   //check VDO table
   usChecksum = 0;
   for( usIdx=0; usIdx < 256; usIdx++ ){
      usChecksum += aucVDOCompTable[usIdx];
   }

   if( usChecksum != VDO_COMP_TABLE_CHECKSUM ){
      ODS( DBG_DRV, DBG_WARNING, "AnaInCRCCheckTables(): aucVDOCompTable[] corrupted!" );
      return FALSE;
   }

   //check internal temp sensor table
   usChecksum = 0;
   for( usIdx=0; usIdx < 256; usIdx++ ){
      usChecksum += aucNTCCompTable[usIdx];
   }

   if( usChecksum != NTC_COMP_TABLE_CHECKSUM ){
      ODS( DBG_DRV, DBG_WARNING, "AnaInCRCCheckTables(): aucNTCCompTable[] corrupted!" );
      return FALSE;
   }

   return TRUE;
}
#endif


/***********************************************************************
 *  FUNCTION:       AnaInRefreshValues
 *  DESCRIPTION:    The function is called by the low level driver when
 *                  a new AD sample has been measured. The AD samples
 *                  are filtered and scaled for further application access.
 *  PARAMETER:      ucADChannel - channel number
 *                  ucADSample  - AD sample value
 *  RETURN:         -
 *  COMMENT:        This callback function will be called by the low level
 *                  driver. The calling direction (lower->upper layer) should
 *                  normally be avoided. In breaking up with this rule here,
 *                  we don't have the overhead of implementing a second
 *                  cyclicly called timer routine to get the application
 *                  usable values regularly updated.
 *  NOTE:           The steady-state error depends on the precision of the
 *                  calculation. So we use a higher precision here (more
 *                  bits of an integer). All values are multiplied with
 *                  ANAIN_xxxx_SCALE before calculation (e.g. 1°C = 32).
 *                  The interface functions are reversing this process. To
 *                  account for rounding errors ANAIN_xxxx_SCALE/2 is added
 *                  before finally dividing (e.g. 16/32 = 0.5).
 *                  The values read out of the compensation tables for
 *                  temperature values have already an offset of 40 added to
 *                  the real temperature in °C: -40...215°C + 40 = 0...255.
 *                  The range with ANAIN_TEMP_SCALE=32 used during filtering is
 *                  32*(-40...215°C + 40) = -1280...6880 + 1280 = 0...8160.
 *                  If you change the filter characteristics be sure not to
 *                  exceed the maximum integer range during calculation
 *                  (UINT16 at the moment).
 *
 *                  When an AD channel delivers the very first value, the
 *                  physical value will NOT be filtered but taken directly.
 *                  This avoids slow converging values for strong filters
 *                  after powerup/reset.
 *********************************************************************** */
void AnaInRefreshValues( UINT8 ucADChannel, UINT8 ucADSample )
{
   BOOL    bADChannelInit;
   UINT16  usNewValue;

   //check AD channel init bit and set it afterwards
   bADChannelInit = ( (ADChannelInitBits & (1<<ucADChannel)) == 0 ) ? FALSE : TRUE;
   ADChannelInitBits |= (1<<ucADChannel);

   //use dedicated filter routines for each kind of AD channel
   switch( ucADChannel ){
      case ANAIN_EXTU:     //general purpose AD input ------------------
         if( bADChannelInit ){
            usADPhysValues[ucADChannel] = (UINT16)ucADSample; //TBD: insert scaler+filter here
         }
         else{
            usADPhysValues[ucADChannel] = (UINT16)ucADSample; //TBD: insert scaler here only
         }
         break;

      case ANAIN_TEMP:     //internal temp sensor ----------------------
            //convert AD raw value to physical value via lookup table
            //resolution with ANAIN_TEMP_SCALE=32: 32=1°C, offset 1280=40°C
            usNewValue = ANAIN_TEMP_SCALE * (UINT16)aucNTCCompTable[ucADSample];
            if( bADChannelInit ){
               //temperature filter: take 7/8 of old value and 1/8 of new sample
               //(+4/8=0.5 for rounding error, equals behaviour for pos. and neg. slopes)
               usADPhysValues[ucADChannel] = (7 * usADPhysValues[ucADChannel] + usNewValue + 4) >> 3;
            }
            else{
               //take new sample without filtering at very first time to initialise filter
               usADPhysValues[ucADChannel] = usNewValue;
            }
            break;

        case ANAIN_LUMI:   //internal luminescence sensor (LDR) --------
            //we use the raw value of the LDR as physical value
            usNewValue = ANAIN_LUMI_SCALE * (UINT16)ucADSample;  //range 0=bright...255=dark
            if( bADChannelInit )
            {
               /* different filters for darker/brighter transition:
                    - fast detection of darkness
                    - slowly detection of brightness
                  this helps quickly switching on backlight e.g. under bridges */
               if ( usNewValue > usADPhysValues[ucADChannel] )
               {
                   // trend: going to be darker!
                   // luminescence filter: take 1/8 of old value and 7/8 of new sample
                   //(+4/8=0.5 for rounding error, equals behaviour for pos. and neg. slopes)
                   usADPhysValues[ucADChannel] = (1*usADPhysValues[ucADChannel] + 7*usNewValue + 4) >> 3;
               }
               else
               {
                   // trend: going to be brighter!
                   // luminescence filter: take 7/8 of old value and 1/8 of new sample
                   // (+4/8=0.5 for rounding error, equals behaviour for pos. and neg. slopes)
                   usADPhysValues[ucADChannel] = (7*usADPhysValues[ucADChannel] + 1*usNewValue + 4) >> 3;
               }

            }
            else{
               //take new sample without filtering at very first time to initialise filter
               usADPhysValues[ucADChannel] = usNewValue;
            }
            break;

        case ANAIN_VOLT:   //internal power supply ---------------------
            //get physical value from AD raw value via linear calculation (experimental)
            //resolution with ANAIN_VOLT_SCALE=4: 4=0,01 V
            //R46=100K, R48=33K, Vref = 4,43V, Vdiodes = 1,56V
            usNewValue = ANAIN_VOLT_SCALE * ( (UINT16)ucADSample * 7 + 156 );
            if( bADChannelInit ){
               //voltage filter: take 7/8 of old value and 1/8 of new sample
               //(+4/8=0.5 for rounding error, equals behaviour for pos. and neg. slopes)
               usADPhysValues[ucADChannel] = (7 * usADPhysValues[ucADChannel] + usNewValue + 4) >> 3;
            }
            else{
               //take new sample without filtering at very first time to initialise filter
               usADPhysValues[ucADChannel] = usNewValue;
            }
            break;

        case ANAIN_ALTW:   //alternator warning light ------------------
            //get physical value from AD raw value via linear calculation
            //resolution with ANAIN_VOLT_SCALE=4: 4=0,01 V
            //R49=100K, R50=33K, Vref = 4,43V
            usNewValue = ANAIN_VOLT_SCALE * (UINT16)ucADSample * 7;
            if( bADChannelInit ){
               //voltage filter: take 7/8 of old value and 1/8 of new sample
               //(+4/8=0.5 for rounding error, equals behaviour for pos. and neg. slopes)
               usADPhysValues[ucADChannel] = (7 * usADPhysValues[ucADChannel] + usNewValue + 4) >> 3;
            }
            else{
               //take new sample without filtering at very first time to initialise filter
               usADPhysValues[ucADChannel] = usNewValue;
            }
            break;

         case ANAIN_TAIR:  //external air temperature ------------------
            //convert AD raw value to physical value via lookup table
            //resolution with ANAIN_TEMP_SCALE=32: 32=1°C, offset 1280=40°C
            usNewValue = ANAIN_TEMP_SCALE * (UINT16)aucVDOCompTable[ucADSample];
         if( bADChannelInit ){
            //temperature filter: take 7/8 of old value and 1/8 of new sample
            //(+4/8=0.5 for rounding error, equals behaviour for pos. and neg. slopes)
            usADPhysValues[ucADChannel] = (7 * usADPhysValues[ucADChannel] + usNewValue + 4) >> 3;
         }
         else{
            //take new sample without filtering at very first time to initialise filter
            usADPhysValues[ucADChannel] = usNewValue;
         }
         break;

      case ANAIN_TWAT:     //external water temperature ----------------
         //convert AD raw value to physical value via lookup table
         //resolution with ANAIN_TEMP_SCALE=32: 32=1°C, offset 1280=40°C
         usNewValue = ANAIN_TEMP_SCALE * (UINT16)aucVDOCompTable[ucADSample];
         if( bADChannelInit ){
            //temperature filter: take 7/8 of old value and 1/8 of new sample
            //(+4/8=0.5 for rounding error, equals behaviour for pos. and neg. slopes)
            usADPhysValues[ucADChannel] = (7 * usADPhysValues[ucADChannel] + usNewValue + 4) >> 3;
         }
         else{
            //take new sample without filtering at very first time to initialise filter
            usADPhysValues[ucADChannel] = usNewValue;
         }
         break;

      case ANAIN_TOIL:     //external oil temperature ------------------
         //convert AD raw value to physical value via lookup table
         //resolution with ANAIN_TEMP_SCALE=32: 32=1°C, offset 1280=40°C
         usNewValue = ANAIN_TEMP_SCALE * (UINT16)aucVDOCompTable[ucADSample];
         if( bADChannelInit ){
            //temperature filter: take 7/8 of old value and 1/8 of new sample
            //(+4/8=0.5 for rounding error, equals behaviour for pos. and neg. slopes)
            usADPhysValues[ucADChannel] = (7 * usADPhysValues[ucADChannel] + usNewValue + 4) >> 3;
         }
         else{
            //take new sample without filtering at very first time to initialise filter
            usADPhysValues[ucADChannel] = usNewValue;
         }
         break;

      default:
         ODS( DBG_DRV, DBG_ERROR, "AnaInRefreshValues() Invalid AD channel!" );
         break;
   }
}


/***********************************************************************
 *  FUNCTION:       AnaInGetTemperature
 *  DESCRIPTION:    returns filtered & scaled internal temperature
 *  PARAMETER:      -
 *  RETURN:         internal temperature in °C in the range -40°C...215°C
 *                  returns ANAIN_INVALID_S if no valid value is available
 *  COMMENT:        -
 *********************************************************************** */
INT16 AnaInGetTemperature( void )
{
    UINT16 usTemp;
    INT16 sTemp;

    usTemp = usADPhysValues[ANAIN_TEMP];
    if( usTemp == ANAIN_INVALID_U )
        return ANAIN_INVALID_S;

    usTemp = ( usTemp + (ANAIN_TEMP_SCALE/2) ) / ANAIN_TEMP_SCALE;

    //range check
    if( usTemp > 255 )
    {
        ODS( DBG_DRV, DBG_ERROR, "AnaInGetTemperature() Invalid temperature!" );
        usTemp = 255;
    }

    // transform into correct range/offset
    sTemp = (INT16)usTemp - ANAIN_TEMP_OFFSET;

    return sTemp;
}


/***********************************************************************
 *  FUNCTION:       AnaInGetLumiVal
 *  DESCRIPTION:    returns filtered & scaled value of internal LDR
 *                  (brightness sensor)
 *  PARAMETER:      -
 *  RETURN:         luminescence value in the range 0=bright...255=dark
 *                  returns ANAIN_INVALID_U if no valid value is available
 *  COMMENT:
 *********************************************************************** */
UINT16 AnaInGetLumiVal( void )
{
    UINT16 usLumi;

    usLumi = usADPhysValues[ANAIN_LUMI];
    if( usLumi == ANAIN_INVALID_U )
        return ANAIN_INVALID_U;

    usLumi = ( usLumi + (ANAIN_LUMI_SCALE/2) ) / ANAIN_LUMI_SCALE;

    //range check
    if( usLumi > 255 )
    {
        ODS( DBG_DRV, DBG_ERROR, "AnaInGetLumiVal() Invalid luminescence!" );
        usLumi = 255;
    }

    return usLumi;
}


/***********************************************************************
 *  FUNCTION:       AnaInGetVoltage
 *  DESCRIPTION:    returns filtered & scaled power supply voltage
 *  PARAMETER:      -
 *  RETURN:         voltage with resolution 0,01 V/bit
 *                  returns ANAIN_INVALID_U if no valid value is available
 *  COMMENT:        -
 *********************************************************************** */
UINT16 AnaInGetVoltage( void )
{
    UINT16 usVolt;

    usVolt = usADPhysValues[ANAIN_VOLT];
    if( usVolt == ANAIN_INVALID_U )
        return ANAIN_INVALID_U;

    usVolt = ( usVolt + (ANAIN_VOLT_SCALE/2) ) / ANAIN_VOLT_SCALE;

    return usVolt;
}


/***********************************************************************
 *  FUNCTION:       AnaInGetAltwVoltage
 *  DESCRIPTION:    returns filtered & scaled alternator voltage
 *  PARAMETER:      -
 *  RETURN:         voltage with resolution 0,01V/bit
 *                  returns ANAIN_INVALID_U if no valid value is available
 *  COMMENT:        -
 *********************************************************************** */
UINT16 AnaInGetAltwVoltage( void )
{
    UINT16 usVolt;

    usVolt = usADPhysValues[ANAIN_ALTW];
    if( usVolt == ANAIN_INVALID_U )
        return ANAIN_INVALID_U;

    usVolt = ( usVolt + (ANAIN_VOLT_SCALE/2) ) / ANAIN_VOLT_SCALE;

    return usVolt;
}


/***********************************************************************
 *  FUNCTION:       AnaInGetAirTemperature
 *  DESCRIPTION:    returns filtered & scaled external air temperature
 *  PARAMETER:      -
 *  RETURN:         temperature         if ok, in °C in the range -40°C...215°C
 *                  ANAIN_INVALID_S     if error or not available
 *  COMMENT:        TBD: use calibration offset given by user
 *********************************************************************** */
INT16 AnaInGetAirTemperature( void )
{
    UINT16 usTemp;
    INT16 sTemp;

    // get current filtered + scaled value
    usTemp = usADPhysValues[ANAIN_TAIR];

    // check: value yet valid?
    if( usTemp == ANAIN_INVALID_U )
        return ANAIN_INVALID_S;

    // division and correct rounding done in higher resolution for more accuracy
    usTemp = ( usTemp + (ANAIN_TEMP_SCALE/2) ) / ANAIN_TEMP_SCALE;

    // range check (should always be correct!)
    if( usTemp > 255 ){
        ODS( DBG_DRV, DBG_ERROR, "AnaInGetAirTemperature() Invalid temperature!" );
        return ANAIN_INVALID_S;
    }

    // transform into correct range/offset
    sTemp = (INT16)usTemp - ANAIN_TEMP_OFFSET;
    return sTemp;
}


/***********************************************************************
 *  FUNCTION:       AnaInGetWatTemperature
 *  DESCRIPTION:    returns filtered & scaled external water temperature
 *  PARAMETER:      -
 *  RETURN:         temperature         if ok, in °C in the range -40°C...215°C
 *                  ANAIN_INVALID_S     if error or not available
 *  COMMENT:        TBD: use calibration offset given by user
 *********************************************************************** */
INT16 AnaInGetWatTemperature( void )
{
    UINT16 usTemp;
    INT16 sTemp;

    // get current filtered + scaled value
    usTemp = usADPhysValues[ANAIN_TWAT];

    // check: value yet valid?
    if( usTemp == ANAIN_INVALID_U )
        return ANAIN_INVALID_S;

    // division and correct rounding done in higher resolution for more accuracy
    usTemp = ( usTemp + (ANAIN_TEMP_SCALE/2) ) / ANAIN_TEMP_SCALE;

    // range check (should always be correct!)
    if( usTemp > 255 ){
        ODS( DBG_DRV, DBG_ERROR, "AnaInGetWatTemperature() Invalid temperature!" );
        return ANAIN_INVALID_S;
    }

    // transform into correct range/offset
    sTemp = (INT16)usTemp - ANAIN_TEMP_OFFSET;
    return (sTemp);
}


/***********************************************************************
 *  FUNCTION:       AnaInGetOilTemperature
 *  DESCRIPTION:    returns filtered & scaled external oil temperature
 *  PARAMETER:      -
 *  RETURN:         temperature         if ok, in °C in the range -40°C...215°C
 *                  ANAIN_INVALID_S     if error or not available
 *  COMMENT:        TBD: use calibration offset given by user
 *********************************************************************** */
INT16 AnaInGetOilTemperature( void )
{
    UINT16 usTemp;
    INT16 sTemp;

    // get current filtered + scaled value
    usTemp = usADPhysValues[ANAIN_TOIL];

    // check: value yet valid?
    if( usTemp == ANAIN_INVALID_U )
        return ANAIN_INVALID_S;

    // division and correct rounding done in higher resolution for more accuracy
    usTemp = ( usTemp + (ANAIN_TEMP_SCALE/2) ) / ANAIN_TEMP_SCALE;

    // range check (should always be correct!)
    if( usTemp > 255 ){
        ODS( DBG_DRV, DBG_ERROR, "AnaInGetOilTemperature() Invalid temperature!" );
        return ANAIN_INVALID_S;
    }

    // transform into correct range/offset
    sTemp = (INT16)usTemp - ANAIN_TEMP_OFFSET;
    return sTemp;
}


/***********************************************************************
 *  FUNCTION:       AnaInFormatVoltage
 *  DESCRIPTION:    create voltage display string in format 'xx,x'
 *  PARAMETER:      usVolts  - given voltage
 *                  szResult - target string buffer
 *                  cbResult - target string buffer[ANAIN_CB_FORMAT]
 *  RETURN:         -
 *  COMMENT:        if the given value is invalid, RESTXT_NOTAVAIL is returned
 *********************************************************************** */
void AnaInFormatVoltage( UINT16 usVolts, STRING szResult, size_t cbResult)
{
   // check: sensor signal available?
   if( usVolts == ANAIN_INVALID_U ){
      strncpy( szResult, RESTXT_NOTAVAIL, cbResult );
      szResult[cbResult-1] = '\0';
      return;
   }
   else{
      sprintf( szResult, "%2d%c%1d", usVolts/100, RESTXT_DEC_SEPARATOR, (usVolts - (usVolts/100) * 100) / 10);
   }
}


/***********************************************************************
 *  FUNCTION:       AnaInFormatTemperature
 *  DESCRIPTION:    create temperature display string in format 'xxx'
 *  PARAMETER:      temp     - temperature value
 *                  cbResult - target string buffer[ANAIN_CB_FORMAT]
 *  RETURN:         -
 *  COMMENT:        if the given value is invalid, RESTXT_NOTAVAIL is returned
 *********************************************************************** */
void AnaInFormatTemperature( INT16 sTemp, STRING szResult, size_t cbResult )
{
   // check: sensor signal available?
   if( sTemp == ANAIN_INVALID_S )
   {
      strncpy( szResult, RESTXT_NOTAVAIL, cbResult );
      szResult[cbResult-1] = '\0';
   }

   // check: sensor signal plausible (sensor connected)?
   else if( sTemp <= ANAIN_TEMP_SENSORDETECT )
   {
      strncpy( szResult, RESTXT_NOTCONN, cbResult );
      szResult[cbResult-1] = '\0';
   }
   // ok, format value to string
   else{
      sprintf( szResult, "%3d", (int)sTemp );
   }
}
