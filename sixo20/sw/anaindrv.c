/***********************************************************************
 *
 *  $RCSfile$
 *
 *  $Revision$
 *
 *  $Date$
 *
 *  Author: Markus Mueller, mm@rsj.de
 *          Arnold Neugebauer
 *
 *  --------------------------------------------------------------------
 *  Created:        10-22-2003 by Markus Mueller
 *  Project:        SIxO 2.0
 *  Module:         AD converter
 *  Purpose:        low level code for AD conversion
 *  Comments:       Theres no way to automatically detect if a sensor is
 *                  present or not (without compromising the dynamic range).
 *                  Thus, if a sensors value is available for the upper
 *                  layers must be declared at compile time or indicated
 *                  by the user at run time.
 *  Changes:        "Alles neu macht der Mai" - a german proverb, AN 10-23-2004
 *                  Added counter for initial startup, AN 10-29-2004
 *                  Removed counter, flag is sufficient, AN 10-31-2004
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
 *      Stoeckener Str. 115                 Knickstr. 10
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


#include "anaindrv.h"
#include "debug.h"
#include "standard.h"
#include "sfr62p.h"


//block readout of (invalid) AD-value the very first time
static BOOL gbFirstValue;

//global state of AnaInDrvTriggerADConverter
static UINT8 gucChannel;
static BOOL  gbPrepare;

//external symbols
//note: not given via header file to avoid cyclic includes
extern void AnaInRefreshValues( UINT8 ucADChannel, UINT8 ucADSample );


/***********************************************************************
 *  FUNCTION:       AnaInDrvInit
 *  DESCRIPTION:    initialisation of AD converter
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:
 *********************************************************************** */
ERRCODE AnaInDrvInit( void )
{
   //set all power switches of ext. temperature sensors to off and output
   p0  = (p0  & 0xF8) | ANAIN_EN_OFF;
   pd0 = (pd0 & 0xF8) | 0x07;

   //set AD control registers
   adcon0 = 0x00;    //one shot mode, software trigger, frequency fAD/4
   adcon1 = 0x20;    //8bit resolution, frequency fAD/4, Vref connected, ANEX not used
   adcon2 = 0x01;    //sample and hold, select AN0-7, frequency fAD/4 */

   pd10 = 0;         //set direction bits for all 8 AD ports to input

   //set initial state for AnaInDrvTriggerADConverter()
   gbFirstValue = TRUE;       //block readout of very first (invalid) value
   gucChannel   = ANAIN_TOIL; //will be incremented to first correct measurement on channel 0 (EXT_U)
   gbPrepare    = TRUE;

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       AnaInDrvTriggerADConverter
 *  DESCRIPTION:    timer callback to update current AD samples
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        This is a state machine. States are memorized in
 *                  the global variables gucChannel and gbPrepare.
 *                  The function is called every 20ms. A measurement is
 *                  prepared, then executed. After every preparation and
 *                  every execution a time slice of 20ms passes. So there
 *                  is enough time for the voltages at the input to settle
 *                  and for the AD conversion to finish (we also don't have
 *                  to test the adst busy flag).
 *                  A channel is processed every 320ms. This is quite fast
 *                  for the signals to measure but to make it slower would
 *                  only cost extra overhead in processing time.
 *                  The results are delivered to the upper layer by a function
 *                  call.
 *                  It is essential to enable the power to the external
 *                  temperature sensors one at a time and only for the time
 *                  needed. This keeps the overall current consumption
 *                  within limits.
 *********************************************************************** */
void AnaInDrvTriggerADConverter( void )
{
   UINT8 ucADValue;
   
   //--------------------prepare the measurement--------------------
   if( gbPrepare ){
      gbPrepare = FALSE;

      //Right after initialisation the very first value is wrong (we did'nt
      //trigger the AD converter yet). So we don't deliver it to the upper layer.
      if( !gbFirstValue ){
         //get result of last measurement from hardware register
         ucADValue = *((UINT8*)(0x03c0 + gucChannel * 2));

         //deliver value to the upper layer for filtering
         AnaInRefreshValues( gucChannel, ucADValue );
      }
      else{
         gbFirstValue = FALSE;
      }

      //advance to next AD channel
      gucChannel++;
      if( gucChannel == ANAIN_LAST ) gucChannel = ANAIN_EXTU;
      adcon0 = (adcon0 & 0xF8) | gucChannel;

      //turn on/off power for external sensors
      switch( gucChannel ){
         case ANAIN_TAIR:
            p0 = (p0 & 0xF8) | ANAIN_EN_TAIR;
            break;
         case ANAIN_TWAT:
            p0 = (p0 & 0xF8) | ANAIN_EN_TWAT;
            break;
         case ANAIN_TOIL:
            p0 = (p0 & 0xF8) | ANAIN_EN_TOIL;
            break;
         default:
            //all off
            p0 = (p0 & 0xF8) | ANAIN_EN_OFF;
            break;
      }
   }
   //--------------------execute the measurement--------------------
   else{
      gbPrepare = TRUE;
      adst = 1; //trigger AD conversion
   }
}


//======================================================================
//=
//= the following is test code, only compiled in the debug version
//=
//======================================================================

#ifdef DEBUG

/***********************************************************************
 *  FUNCTION:       AnaInDrvTestGenerateADValues
 *  DESCRIPTION:    Generate AD values for test
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        Generating AD values by software instead of measurent
 *                  is faster and reproducible.
 *                  Insert ucADValue = AnaInDrvTestGenerateADValues( ucADValue );
 *                  directly before call to AnaInRefreshValues. The function
 *                  is then called every 40ms. Each AD channel is processed
 *                  every 320ms.
 *********************************************************************** */
UINT8 AnaInDrvTestGenerateADValues( UINT8 ucADValue )
{
   //Generates 100°C down to 20°C. Since the compensation table (checksum
   //26031) is relatively linear in this area the temp values are falling
   //nearly linear.
   {
      static UINT8 ucGenValue = 50;
   
      if( gucChannel == 5 ){ //only process TAIR, bypass all other channels
         ucADValue = ucGenValue;
         if( ucGenValue < 206 ) ucGenValue++;
      }
   }

   //From 30°C down to 20°C veeeery slowly.
   /*
   {
      static UINT8 ucGenValue = 187;
      static UINT8 ucCnt = 0;

      if( gucChannel == 5 ){ //only process TAIR, bypass all other channels
         ucADValue = ucGenValue;

         ucCnt++;
         if( ucCnt > 250 ){
            ucCnt = 0;
            if( ucGenValue < 206 ) ucGenValue++;
         }
      }
   }
   */

   return ucADValue;
}

#endif //DEBUG
