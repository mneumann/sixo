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
 *  Purpose:        clock api functions
 *  Comments:       Physically the clock and the non volatile ram is
 *                  located in one chip accessed over the iic bus.
 *                  The iicbb low level routines are used. iicInit of
 *                  the module eeprdrv must be called before calling any
 *                  of the functions of this module.
 *
 *                  Equation used for calibration:
 *                  c             calibration value 0..31
 *                  n = 32768     oscillator cycles per second
 *                  cy= 3840      seconds per calibration cycle (64 minutes)
 *                  k = {256|512} one calibration step {subtracts|adds} k
 *                                oscillator cycles within cy
 *                  tdur          duration of the measurement 
 *                  tdev          deviation between clock and reference after
 *                                tdur (positive: clock runs too fast -> sub
 *                                osc cycles; negative: clock runs too slow
 *                                -> add osc cycles)
 *
 *                       n cy  tdev 
 *                  c = ------------
 *                         k tdur
 *
 ************************************************************************ */


#include "standard.h"
#include "clkdrv.h"
#include "iicbb.h"


//register addresses of the hardware
//for the current hardware implementation IIC_CLK_DEV_ADDR and
//IIC_NVRAM_DEV_ADR are the same because the clock and the nvram
//are located physically in the same device
#define IIC_CLK_DEV_ADDR         0x68  //right aligned, highest bit doesn't care
#define IIC_CLK_SECS_SUB_ADDR    0
#define IIC_CLK_MINS_SUB_ADDR    1
#define IIC_CLK_HOURS_SUB_ADDR   2
#define IIC_CLK_DAY_SUB_ADDR     3
#define IIC_CLK_DATE_SUB_ADDR    4
#define IIC_CLK_MONTH_SUB_ADDR   5
#define IIC_CLK_YEAR_SUB_ADDR    6
#define IIC_CLK_CALIB_SUB_ADDR   7

//special function bits in the hardware registers
#define SPECIAL_ST   ( (UINT8)0x80 )   //0=run, 1=stop oscillator
#define SPECIAL_CEB  ( (UINT8)0x80 )   //0=disable, 1=enable century toggle
#define SPECIAL_CB   ( (UINT8)0x40 )   //century toggle bit

//for mathematical calculations in iic{S|G}etClockCalib
//REV_FACTOR / REV_{ADD|SUB}_FACTOR = 30.5*24*60*{512|256}/64/32768
#define ADD_FACTOR      245760   // 32768/512 * 3840
#define SUB_FACTOR      491520   // 32768/256 * 3840
#define REV_FACTOR      ((INT32)2745)
#define REV_ADD_FACTOR  256
#define REV_SUB_FACTOR  512
#define MAX_CALIB       31   // max. calibration factor due to register size
#define MAX_DEVIATION   4369 // INT32_MAX/SUB_FACTOR seconds, max. deviation value without
                             // overflow in the calculation of the calibration factor


/***********************************************************************
 *  FUNCTION:       iicSetClockTime
 *  DESCRIPTION:    sets the clock to the given time of day over iic bus
 *  PARAMETER:      structure CLOCKTIME
 *                  hours     00..23 bcd format
 *                  minutes   00..59 bcd format
 *                  seconds   00..59 bcd format
 *  RETURN:         ERR_PARAM_ERR        invalid parameter
 *                  ERR_CLOCK_NOWRITE    write access failed, no acknowledge
 *                  ERR_OK               success
 *  COMMENT:        century toggle will be enabled but iicGetClockTime
 *                  does not return the century toggle bit
 *********************************************************************** */
ERRCODE iicSetClockTime( CLOCKTIME *pClktme )
{
   UINT8 iic_adr_buffer[2];
   UINT8 iic_dat_buffer[3];

   //check parameters
   if(   CHECK_BCD( pClktme->hours )
      || ( BCD2VAL( pClktme->hours ) > 23 )
      || ( LEFT_BCD_DIGIT( pClktme->minutes ) > 5 )
      || ( RIGHT_BCD_DIGIT( pClktme->minutes ) > 9 )
      || ( LEFT_BCD_DIGIT( pClktme->seconds ) > 5 )
      || ( RIGHT_BCD_DIGIT( pClktme->seconds ) > 9 )
   ) return ERR_PARAM_ERR;

   //prepare buffers for transmission over iic bus
   iic_adr_buffer[0] = IIC_CLK_DEV_ADDR;
   iic_adr_buffer[1] = IIC_CLK_SECS_SUB_ADDR;
 
   iic_dat_buffer[0] = pClktme->seconds;
   iic_dat_buffer[1] = pClktme->minutes;
   iic_dat_buffer[2] = pClktme->hours;

   //set special function bits
   iic_dat_buffer[0] &= ~SPECIAL_ST;   //run oscillator
   iic_dat_buffer[2] |= SPECIAL_CEB;   //enable century toggle

   //transmit time over iic bus
   if( !iic_snd( iic_adr_buffer,
                 2,              // number of addresses: dev addr and sub addr
                 iic_dat_buffer,
                 3 )             // number of bytes to transmit
   ) return ERR_CLOCK_NOWRITE;

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       iicGetClockTime
 *  DESCRIPTION:    reads the current time of day of the iic bus clock
 *  PARAMETER:      structure CLOCKTIME
 *                  hours     00..23 bcd format
 *                  minutes   00..59 bcd format
 *                  seconds   00..59 bcd format
 *  RETURN:         ERR_CLOCK_NOREAD   read access failed, no acknowledge
 *                  ERR_OK             success
 *  COMMENT:        the hardware makes provisions to assure, that a clock
 *                  update does not occur during read
 *                  the century toggle bit is removed from the output
 *********************************************************************** */
ERRCODE iicGetClockTime( CLOCKTIME *pClktme )
{
   UINT8 iic_adr_buffer[2];
   UINT8 iic_dat_buffer[3];

   //prepare buffers for transmission over iic bus
   iic_adr_buffer[0] = IIC_CLK_DEV_ADDR;
   iic_adr_buffer[1] = IIC_CLK_SECS_SUB_ADDR;
 
   //go and get time over iic bus
   if( !iic_rcv( iic_adr_buffer,
                 2,              // number of addresses: dev addr and sub addr
                 iic_dat_buffer,
                 3 )             // number of bytes to transmit
   ) return ERR_CLOCK_NOREAD;

   //remove special function bits
   iic_dat_buffer[0] &= ~SPECIAL_ST;
   iic_dat_buffer[2] &= ~(SPECIAL_CEB + SPECIAL_CB);

   pClktme->seconds = iic_dat_buffer[0];
   pClktme->minutes = iic_dat_buffer[1];
   pClktme->hours   = iic_dat_buffer[2];

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       iicSetClockDate
 *  DESCRIPTION:    sets the clock to the given date over iic bus
 *  PARAMETER:      structure CLOCKDATE
 *                  date   01..31 bcd format
 *                  month  01..12 bcd format
 *                  year   00..99 bcd format
 *                  day    01..07 bcd format
 *  RETURN:         ERR_PARAM_ERR        invalid parameter
 *                  ERR_CLOCK_NOWRITE    write access failed, no acknowledge
 *                  ERR_OK               success
 *  COMMENT:
 *********************************************************************** */
ERRCODE iicSetClockDate( CLOCKDATE *pClkdte )
{
   UINT8 iic_adr_buffer[2];
   UINT8 iic_dat_buffer[4];

   //check parameters
   if(   CHECK_BCD( pClkdte->date )
      || ( (UINT8)( BCD2VAL( pClkdte->date ) - 1) > 30 )
      || CHECK_BCD( pClkdte->month )
      || ( (UINT8)( BCD2VAL( pClkdte->month ) - 1) > 11 )
      || ( CHECK_BCD( pClkdte->year ) )
      || ( LEFT_BCD_DIGIT( pClkdte->day ) != 0 )
      || ( (UINT8)(RIGHT_BCD_DIGIT( pClkdte->day ) - 1) > 6 )
   ) return ERR_PARAM_ERR;

   //transmit date over iic bus
   iic_adr_buffer[0] = IIC_CLK_DEV_ADDR;
   iic_adr_buffer[1] = IIC_CLK_DAY_SUB_ADDR;
 
   iic_dat_buffer[0] = pClkdte->day;
   iic_dat_buffer[1] = pClkdte->date;
   iic_dat_buffer[2] = pClkdte->month;
   iic_dat_buffer[3] = pClkdte->year;

   if( !iic_snd( iic_adr_buffer,
                 2,              // number of addresses: dev addr and sub addr
                 iic_dat_buffer,
                 4 )             // number of bytes to transmit
   ) return ERR_CLOCK_NOWRITE;

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       iicGetClockDate
 *  DESCRIPTION:    reads the current date of the iic bus clock
 *  PARAMETER:      structure CLOCKDATE
 *                  date   01..31 bcd format
 *                  month  01..12 bcd format
 *                  year   00..99 bcd format
 *                  day    01..07 bcd format
 *  RETURN:         ERR_CLOCK_NOREAD   read access failed, no acknowledge
 *                  ERR_OK             success
 *  COMMENT:        the hardware makes provisions to assure, that a clock
 *                  update does not occur during read
 *********************************************************************** */
ERRCODE iicGetClockDate( CLOCKDATE *pClkdte )
{
   UINT8 iic_adr_buffer[2];
   UINT8 iic_dat_buffer[4];

   //go and get date over iic bus
   iic_adr_buffer[0] = IIC_CLK_DEV_ADDR;
   iic_adr_buffer[1] = IIC_CLK_DAY_SUB_ADDR;
 
   if( !iic_rcv( iic_adr_buffer,
                 2,              // number of addresses: dev addr and sub addr
                 iic_dat_buffer,
                 4 )             // number of bytes to transmit
   ) return ERR_CLOCK_NOREAD;

   pClkdte->day   = iic_dat_buffer[0];
   pClkdte->date  = iic_dat_buffer[1];
   pClkdte->month = iic_dat_buffer[2];
   pClkdte->year  = iic_dat_buffer[3];

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       iicSetClockCalib
 *  DESCRIPTION:    sets the calibration value of the iic bus clock
 *  PARAMETER:      dwDuration   duration of the measurement in seconds
 *                  dwDeviation  signed deviation to the reference clock
 *                               at the end of the measurement in seconds
 *  RETURN:         ERR_PARAM_ERR        invalid parameter
 *                  ERR_CLOCK_EXCESS     warning, calibration value is
 *                                       set according to excessively big
 *                                       values, there might be something
 *                                       wrong with the parameters
 *                  ERR_CLOCK_NOWRITE    write access failed, no acknowledge
 *                  ERR_OK               success
 *  COMMENT:        Let the clock run, say, for a month. Then dwDuration
 *                  is 30 days = 2592000 seconds. The deviation to the
 *                  reference clock after this month is for example
 *                  dwDeviation = -90 seconds (the clock runs to slow).
 *                  Then the calibration value is set to +9 by this function.
 *
 *                  If excessive parameters are given the processing is not
 *                  stoped and the calibration value is set accordingly. To
 *                  provide a possibility to inform the user that something
 *                  might be wrong ERR_CLOCK_EXCESS is returned.
 *********************************************************************** */
ERRCODE iicSetClockCalib( INT32 dwDuration, INT32 dwDeviation )
{
   ERRCODE ERR = ERR_OK;
   UINT8  iic_adr_buffer[2];
   UINT8  iic_dat_buffer[1];
   INT32  dwFactor;
   INT32  dwCalib;
   UINT32 dwRemain2;

   //extract sign (scaling of the hardware is
   //different in the positive and negative range)
   if( dwDeviation >= 0 ){
      //clock runs too fast -> subtract from osc cycles
      dwFactor = SUB_FACTOR;
      iic_dat_buffer[0] = 0x00;
   }
   else{
      //clock runs too slow -> add to osc cycles
      dwFactor = ADD_FACTOR;
      iic_dat_buffer[0] = SPECIAL_S;
      //remove sign from dwDeviation (absolute value)
      //(can't convert INT32_MIN, accept little error of one second if
      //this excessive value is given,should never happen anyway) 
      dwDeviation = ~dwDeviation;
      if( dwDeviation < INT32_MAX ) dwDeviation += 1;
   }
      
   //check parameters
   if(   ( dwDuration <= 0 )
      || ( dwDeviation > dwDuration )
   ) return ERR_PARAM_ERR;

   //if the values are excessively big scale them down to prevent overflow
   //(this should never happen but makes me feel better,
   //MAX_DEVIATION equals at least about 13 month of measurement duration,
   //values are already in the positive range here, so >> is ok)
   while( dwDeviation > MAX_DEVIATION ){
      dwDeviation >>= 1;
      dwDuration >>= 1;
      ERR = ERR_CLOCK_EXCESS; //warning, perhaps wrong parameters
   }

   //at this point:  dwDuration  = 1..INT32_MAX
   //                dwDeviation = 0..MAX_DEVIATION
   //                dwDeviation <= dwDuration

   //calculate rounded calibration value
   //truncation of division is compensated (this is usualy implemented by
   //adding half of the divisor before dividing; the implementation here
   //allows for higher input values at the given 32 bit accuracy)
   //note that dwRemain2 is UINT while dwCalib and dwDuration are INT to
   //hold the result of the operation without overflow
   //dwCalib++ is always possible without overflow with the values chosen
   //for the parameter check above
   dwCalib = dwFactor * dwDeviation;   //first multiplication, then..
   dwRemain2 = ( dwCalib % dwDuration ) << 1;
   dwCalib /= dwDuration;              //   ..division for higher accuracy
   if( dwRemain2 >= dwDuration ) dwCalib++;

   if( dwCalib > 31 ){
      dwCalib = 31;
      ERR = ERR_CLOCK_EXCESS; //warning, perhaps wrong parameters
   }

   //set calibration value
   iic_adr_buffer[0] = IIC_CLK_DEV_ADDR;
   iic_adr_buffer[1] = IIC_CLK_CALIB_SUB_ADDR;

   iic_dat_buffer[0] |= (UINT8)dwCalib;   //keep sign bit already set

   //set special function bits, disable frequency test, output low
   iic_dat_buffer[0] &= ~(SPECIAL_OUT + SPECIAL_FT);
  
   if( !iic_snd( iic_adr_buffer,
                 2,              // number of addresses: dev addr and sub addr
                 iic_dat_buffer,
                 1 )             // number of bytes to transmit
   ) return ERR_CLOCK_NOWRITE;
   //a write error is returned with priority and possibly hides an ERR_CLOCK_EXCESS

   return ERR;
}


/***********************************************************************
 *  FUNCTION:       iicGetClockCalib
 *  DESCRIPTION:    reads the calibration value of the iic bus clock
 *  PARAMETER:      chCalib         pointer to calib value that will be
 *                                  returned
 *                  dwSecPerMonth   pointer to equivalent seconds per month
 *  RETURN:         ERR_CLOCK_NOREAD   read access failed, no acknowledge
 *                  ERR_OK             success
 *  COMMENT:        This function reads and returns the calibration value
 *                  of the clock. Additionally this calibration value is
 *                  converted to an equivalent user readable time per
 *                  month (one month is assumed to have 30.5 days).
 *                  Due to rounding errors this is not the direct inverse
 *                  of iicSetClockCalib.
 *********************************************************************** */
ERRCODE iicGetClockCalib( INT8* chCalib, INT32* dwSecPerMonth )
{
   UINT8  iic_adr_buffer[2];

   //read calibration value
   iic_adr_buffer[0] = IIC_CLK_DEV_ADDR;
   iic_adr_buffer[1] = IIC_CLK_CALIB_SUB_ADDR;

   if( !iic_rcv( iic_adr_buffer,
                 2,               // number of addresses: dev addr and sub addr
                 (UINT8*)chCalib,
                 1 )              // number of bytes to transmit
   ) return ERR_CLOCK_NOREAD;

   //test sign bit and convert calib value accordingly
   if( (*chCalib & SPECIAL_S) == 0x00 ){
      //negative, negate chCalib
      *chCalib &= SPECIAL_CALB;
      *chCalib = ~(*chCalib);
      *chCalib += 1;
   }
   else{
      //positive
      *chCalib &= SPECIAL_CALB;
   }

   //calculate equivalent seconds per month (scaling of the hardware is
   //different in the positive and negative range)
   *dwSecPerMonth = *chCalib * REV_FACTOR;
   if( *chCalib >= 0 ){
      *dwSecPerMonth /= REV_ADD_FACTOR;
   }
   else{
      *dwSecPerMonth /= REV_SUB_FACTOR;
   }

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       iicClockCalibDirect
 *  DESCRIPTION:    reads and writes calibration byte of the iic bus clock
 *                  no conversion of the input or output byte is performed
 *  PARAMETER:      bWrite    WRITE_CALIB=write, READ_CALIB=read
 *                  chCalib   pointer to calib value
 *  RETURN:         ERR_CLOCK_NOWRITE  write access failed, no acknowledge
 *                  ERR_CLOCK_NOREAD   read access failed, no acknowledge
 *                  ERR_OK             success
 *  COMMENT:        To adjust the clock usually iicSetClockCalib and
 *                  iicGetClockCalib are used. This function provides
 *                  a rather simple interface to the calibration byte.
 *                  This may be used for the special functions (frequency
 *                  test, general purpose io) or testing.
 *********************************************************************** */
ERRCODE iicClockCalibDirect( BOOL bWrite, UINT8* chCalib )
{
   UINT8  iic_adr_buffer[2];

   //preset addresses
   iic_adr_buffer[0] = IIC_CLK_DEV_ADDR;
   iic_adr_buffer[1] = IIC_CLK_CALIB_SUB_ADDR;

   //transmit byte
   if( bWrite == WRITE_CALIB ){
      if( !iic_snd( iic_adr_buffer,
                    2,              // number of addresses: dev addr and sub addr
                    (UINT8*)chCalib,
                    1 )             // number of bytes to transmit
      ) return ERR_CLOCK_NOWRITE;
   }
   else{
      if( !iic_rcv( iic_adr_buffer,
                    2,               // number of addresses: dev addr and sub addr
                    (UINT8*)chCalib,
                    1 )              // number of bytes to transmit
      ) return ERR_CLOCK_NOREAD;
   }

   return ERR_OK;
}


//======================================================================
//=
//= the following is test code, only compiled in the debug version
//=
//======================================================================

#ifdef DEBUG
/***********************************************************************
 *  FUNCTION:       iicTestClockTime
 *  DESCRIPTION:    selfchecking test routines for iic{S|G}etClockTime
 *  PARAMETER:      none
 *  RETURN:         ERR_OK             success
 *                  ERR_PARAM_ERR      invalid parameter
 *                  ERR_CLOCK_NOWRITE  write to clock failed, no acknowledge
 *                  ERR_CLOCK_NOREAD   read from clock failed, no acknowledge
 *  COMMENT:
 *********************************************************************** */
ERRCODE iicTestClockTime( void )
{
   UINT8 iic_adr_buffer[2];
   UINT8 iic_dat_buffer[3];
   ERRCODE     ERR;
   CLOCKTIME   clktme;
   UINT8       chBcd;

   //set time and read back time
   //It is assumed, that we are fast enough. Even in the
   //debugger not more than 2 seconds should go by.
   clktme.hours = 0x14;
   clktme.minutes = 0x30;
   clktme.seconds = 0x00;
   ERR = iicSetClockTime( &clktme );
   if( ERR != ERR_OK ) return ERR;
   ERR = iicGetClockTime( &clktme );
   if( ERR != ERR_OK ) return ERR;
   if(  ( clktme.hours   != 0x14 )
      ||( clktme.minutes != 0x30 )
      ||( clktme.seconds >  0x01 )
   ) return ERR_CLOCK_NOREAD;

   //go and get time directly to test special function bits
   //assumes that ST and CB should be 0 and CEB should be 1
   iic_adr_buffer[0] = IIC_CLK_DEV_ADDR;
   iic_adr_buffer[1] = IIC_CLK_SECS_SUB_ADDR;
   if( !iic_rcv( iic_adr_buffer,
                 2,              // number of addresses: dev addr and sub addr
                 iic_dat_buffer,
                 3 )             // number of bytes to transmit
   ) return ERR_CLOCK_NOREAD;
   if(   ( (iic_dat_buffer[0] & 0x80) != 0 )
      || ( (iic_dat_buffer[2] & 0xc0) != 0x80 )
   ) return ERR_CLOCK_NOREAD;

   //-------------------------------------------------------------------

   //test parameters
   //function should return ok, BCD 00..23 all
   for( chBcd = 0x00; chBcd <= 0x23; chBcd++ ){
      if( RIGHT_BCD_DIGIT( chBcd ) == 0x0a ) chBcd += 6;
      clktme.hours = chBcd;
      clktme.minutes = chBcd;
      clktme.seconds = chBcd;
      ERR = iicSetClockTime( &clktme );
      if( ERR != ERR_OK ) return ERR;
   }

   //test parameters
   //function should return ok, BCD 24..59 min sec
   clktme.hours = 0x14;
   for( chBcd = 0x24; chBcd <= 0x59; chBcd++ ){
      if( RIGHT_BCD_DIGIT( chBcd ) == 0x0a ) chBcd += 6;
      clktme.minutes = chBcd;
      clktme.seconds = chBcd;
      ERR = iicSetClockTime( &clktme );
      if( ERR != ERR_OK ) return ERR;
   }

   //test parameters
   //function should return err, nonBCD 00..23 hours
   clktme.minutes = 0x30;
   clktme.seconds = 0x00;
   for( chBcd = 0x0a; chBcd <= 0x1f; chBcd++ ){
      if( RIGHT_BCD_DIGIT( chBcd ) == 0x00 ) chBcd += 0x0a;
      clktme.hours = chBcd;
      ERR = iicSetClockTime( &clktme );
      if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   }

   //test parameters
   //function should return err, out of range 24.. hours
   clktme.minutes = 0x30;
   clktme.seconds = 0x00;
   for( chBcd = 0x24; chBcd != 0x00; chBcd++ ){
      clktme.hours = chBcd;
      ERR = iicSetClockTime( &clktme );
      if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   }

   //test parameters
   //function should return err, nonBCD 00..59 min
   clktme.hours = 0x14;
   clktme.seconds = 0x00;
   for( chBcd = 0x0a; chBcd <= 0x4f; chBcd++ ){
      if( RIGHT_BCD_DIGIT( chBcd ) == 0x00 ) chBcd += 0x0a;
      clktme.minutes = chBcd;
      ERR = iicSetClockTime( &clktme );
      if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   }

   //test parameters
   //function should return err, out of range 60.. min
   clktme.hours = 0x14;
   clktme.seconds = 0x00;
   for( chBcd = 0x60; chBcd != 0x00; chBcd++ ){
      clktme.minutes = chBcd;
      ERR = iicSetClockTime( &clktme );
      if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   }

   //test parameters
   //function should return err, nonBCD 00..59 sec
   clktme.hours = 0x14;
   clktme.minutes = 0x30;
   for( chBcd = 0x0a; chBcd <= 0x4f; chBcd++ ){
      if( RIGHT_BCD_DIGIT( chBcd ) == 0x00 ) chBcd += 0x0a;
      clktme.seconds = chBcd;
      ERR = iicSetClockTime( &clktme );
      if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   }

   //test parameters
   //function should return err, out of range 60.. sec
   clktme.hours = 0x14;
   clktme.minutes = 0x30;
   for( chBcd = 0x60; chBcd != 0x00; chBcd++ ){
      clktme.seconds = chBcd;
      ERR = iicSetClockTime( &clktme );
      if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   }

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       iicTestClockDate
 *  DESCRIPTION:    selfchecking test routines for iic{S|G}etClockDate
 *  PARAMETER:      none
 *  RETURN:         ERR_OK             success
 *                  ERR_PARAM_ERR      invalid parameter
 *                  ERR_CLOCK_NOWRITE  write to clock failed, no acknowledge
 *                  ERR_CLOCK_NOREAD   read from clock failed, no acknowledge
 *  COMMENT:
 *********************************************************************** */
ERRCODE iicTestClockDate( void )
{
   ERRCODE     ERR;
   CLOCKTIME   clktme;
   CLOCKDATE   clkdte;
   UINT8       chBcd;

   //set clock to prevent possible date toggle
   //between writing and reading the date
   clktme.hours = 0x14;
   clktme.minutes = 0x30;
   clktme.seconds = 0x00;
   ERR = iicSetClockTime( &clktme );
   if( ERR != ERR_OK ) return ERR;

   //set date and read it back
   clkdte.date  = 0x14;
   clkdte.month = 0x11;
   clkdte.year  = 0x00;
   clkdte.day   = 0x04;
   ERR = iicSetClockDate( &clkdte );
   if( ERR != ERR_OK ) return ERR;
   ERR = iicGetClockDate( &clkdte );
   if( ERR != ERR_OK ) return ERR;
   if(  ( clkdte.date  != 0x14 )
      ||( clkdte.month != 0x11 )
      ||( clkdte.year  != 0x00 )
      ||( clkdte.day   != 0x04 )
   ) return ERR_CLOCK_NOREAD;

   //-------------------------------------------------------------------

   //test parameters
   //function should return ok, BCD 01..07 all
   //(year 00 was tested already above within write/read)
   for( chBcd = 0x01; chBcd <= 0x07; chBcd++ ){
      clkdte.date  = chBcd;
      clkdte.month = chBcd;
      clkdte.year  = chBcd;
      clkdte.day   = chBcd;
      ERR = iicSetClockDate( &clkdte );
      if( ERR != ERR_OK ) return ERR;
   }

   //test parameters
   //function should return ok, BCD 08..12 date month year
   clkdte.day = 0x04;
   for( chBcd = 0x08; chBcd <= 0x12; chBcd++ ){
      if( RIGHT_BCD_DIGIT( chBcd ) == 0x0a ) chBcd += 6;
      clkdte.date  = chBcd;
      clkdte.month = chBcd;
      clkdte.year  = chBcd;
      ERR = iicSetClockDate( &clkdte );
      if( ERR != ERR_OK ) return ERR;
   }

   //test parameters
   //function should return ok, BCD 13..31 date year
   clkdte.month = 0x11;
   clkdte.day   = 0x04;
   for( chBcd = 0x13; chBcd <= 0x31; chBcd++ ){
      if( RIGHT_BCD_DIGIT( chBcd ) == 0x0a ) chBcd += 6;
      clkdte.date  = chBcd;
      clkdte.year  = chBcd;
      ERR = iicSetClockDate( &clkdte );
      if( ERR != ERR_OK ) return ERR;
   }

   //test parameters
   //function should return ok, BCD 32..99 year
   clkdte.date  = 0x14;
   clkdte.month = 0x11;
   clkdte.day   = 0x04;
   for( chBcd = 0x32; chBcd <= 0x99; chBcd++ ){
      if( RIGHT_BCD_DIGIT( chBcd ) == 0x0a ) chBcd += 6;
      clkdte.year  = chBcd;
      ERR = iicSetClockDate( &clkdte );
      if( ERR != ERR_OK ) return ERR;
   }

   //test parameter
   //function should return err, nonBCD 01..31 date
   clkdte.month = 0x11;
   clkdte.year  = 0x00;
   clkdte.day   = 0x04;
   for( chBcd = 0x0a; chBcd <= 0x2f; chBcd++ ){
      if( RIGHT_BCD_DIGIT( chBcd ) == 0x00 ) chBcd += 0x0a;
      clkdte.date = chBcd;
      ERR = iicSetClockDate( &clkdte );
      if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   }

   //test parameters
   //function should return err, out of range 32..00 date
   clkdte.month = 0x11;
   clkdte.year  = 0x00;
   clkdte.day   = 0x04;
   for( chBcd = 0x32; chBcd != 0x01; chBcd++ ){
      clkdte.date = chBcd;
      ERR = iicSetClockDate( &clkdte );
      if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   }

   //test parameters
   //function should return err, nonBCD 01..12 month
   clkdte.date  = 0x14;
   clkdte.year  = 0x00;
   clkdte.day   = 0x04;
   for( chBcd = 0x0a; chBcd <= 0x0f; chBcd++ ){
      clkdte.month = chBcd;
      ERR = iicSetClockDate( &clkdte );
      if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   }

   //test parameters
   //function should return err, out of range 13..00 month
   clkdte.date  = 0x14;
   clkdte.year  = 0x00;
   clkdte.day   = 0x04;
   for( chBcd = 0x13; chBcd != 0x01; chBcd++ ){
      clkdte.month = chBcd;
      ERR = iicSetClockDate( &clkdte );
      if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   }

   //test parameters
   //function should return err, nonBCD 00..99 year
   clkdte.date  = 0x14;
   clkdte.month = 0x11;
   clkdte.day   = 0x04;
   for( chBcd = 0x0a; chBcd <= 0x8f; chBcd++ ){
      if( RIGHT_BCD_DIGIT( chBcd ) == 0x00 ) chBcd += 0x0a;
      clkdte.year  = chBcd;
      ERR = iicSetClockDate( &clkdte );
      if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   }

   //test parameters
   //function should return err, out of range 99.. year
   clkdte.date  = 0x14;
   clkdte.month = 0x11;
   clkdte.day   = 0x04;
   for( chBcd = 0x99; chBcd == 0x00; chBcd++ ){
      clkdte.year = chBcd;
      ERR = iicSetClockDate( &clkdte );
      if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   }

   //test parameters
   //function should return err, out of range 08..00 day
   clkdte.date  = 0x14;
   clkdte.month = 0x11;
   clkdte.year  = 0x00;
   for( chBcd = 0x08; chBcd != 0x01; chBcd++ ){
      clkdte.day = chBcd;
      ERR = iicSetClockDate( &clkdte );
      if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   }

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       iicTestClockCalib
 *  DESCRIPTION:    selfchecking test routines for iic{S|G}etClockCalib
 *                  and iicClockCalibDirect
 *  PARAMETER:      none
 *  RETURN:         ERR_OK             success
 *                  ERR_PARAM_ERR      invalid parameter
 *                  ERR_CLOCK_NOWRITE  write to clock failed, no acknowledge
 *                  ERR_CLOCK_NOREAD   read from clock failed, no acknowledge
 *                  ERR_CLOCK_EXCESS   warning, excessively big values
 *  COMMENT:
 *********************************************************************** */
ERRCODE iicTestClockCalib( void )
{
   ERRCODE ERR;
   INT32 dwDuration;
   INT32 dwDeviation;
   INT32 dwSecPerMonth;
   UINT8 chCalib;
   INT8  uchCalib;

   //set calib value via iicSetClockCalib
   dwDuration = 2592000; //30 days
   dwDeviation = -90;    //-90 secs => calib +8.53
   ERR = iicSetClockCalib( dwDuration, dwDeviation );
   if( ERR != ERR_OK ) return ERR;

   //get back value via iicClockCalibDirect
   //assumes, that FT=0 and OUT=0
   ERR = iicClockCalibDirect( READ_CALIB, &chCalib );
   if( ERR != ERR_OK ) return ERR;
   if( chCalib != 0x29 ) return ERR_CLOCK_NOREAD;

   //get back value via iicGetClockCalib
   ERR = iicGetClockCalib( &uchCalib, &dwSecPerMonth );
   if( ERR != ERR_OK ) return ERR;
   if(  ( uchCalib != +9 )
      ||( dwSecPerMonth != 96 ) //96.5 truncated
   ) return ERR_CLOCK_NOREAD;
  
   //-------------------------------------------------------------------

   //set calib value via iicSetClockCalib
   dwDuration = 1728000; //20 days
   dwDeviation = +45;    //+45 secs => +12.8
   ERR = iicSetClockCalib( dwDuration, dwDeviation );
   if( ERR != ERR_OK ) return ERR;

   //get back value via iicClockCalibDirect
   //assumes, that FT=0 and OUT=0
   ERR = iicClockCalibDirect( READ_CALIB, &chCalib );
   if( ERR != ERR_OK ) return ERR;
   if( chCalib != 0x0d ) return ERR_CLOCK_NOREAD;

   //get back value via iicGetClockCalib
   ERR = iicGetClockCalib( &uchCalib, &dwSecPerMonth );
   if( ERR != ERR_OK ) return ERR;
   if(  ( uchCalib != -13 )
      ||( dwSecPerMonth != -69 ) //69.7 truncated
   ) return ERR_CLOCK_NOREAD;

   //-------------------------------------------------------------------

   //set calib value to -0 (subtract zero cycles)
   dwDuration = 3000000;
   dwDeviation = +1;
   ERR = iicSetClockCalib( dwDuration, dwDeviation );
   if( ERR != ERR_OK ) return ERR;

   //get back value via iicClockCalibDirect
   //assumes, that FT=0 and OUT=0
   ERR = iicClockCalibDirect( READ_CALIB, &chCalib );
   if( ERR != ERR_OK ) return ERR;
   if( chCalib != 0x00 ) return ERR_CLOCK_NOREAD;

   //get back value via iicGetClockCalib
   ERR = iicGetClockCalib( &uchCalib, &dwSecPerMonth );
   if( ERR != ERR_OK ) return ERR;
   if(  ( uchCalib != 0 )
      ||( dwSecPerMonth != 0 )
   ) return ERR_CLOCK_NOREAD;

   //-------------------------------------------------------------------

   //set value via iicClockCalibDirect
   chCalib = 0xaa;
   ERR = iicClockCalibDirect( WRITE_CALIB, &chCalib );
   if( ERR != ERR_OK ) return ERR;

   //get back value via iicClockCalibDirect
   //assumes, that FT=0 and OUT=0
   ERR = iicClockCalibDirect( READ_CALIB, &chCalib );
   if( ERR != ERR_OK ) return ERR;
   if( chCalib != 0xaa ) return ERR_CLOCK_NOREAD;

   //-------------------------------------------------------------------

   //test parameter check, function should return error
   dwDuration = 0;      //=0
   dwDeviation = +45;
   ERR = iicSetClockCalib( dwDuration, dwDeviation );
   if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   //test parameter check, function should return error
   dwDuration = -37;    //negative
   dwDeviation = -45;
   ERR = iicSetClockCalib( dwDuration, dwDeviation );
   if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   //test parameter check, function should return error
   dwDuration = 1870000;
   dwDeviation = 1870001;  //>duration
   ERR = iicSetClockCalib( dwDuration, dwDeviation );
   if( ERR != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   //-------------------------------------------------------------------

   //test excess, function should return error but set calib
   dwDuration = 37033490; //~428 days, 
   dwDeviation = -4370;   //~73 min >MAX_DEVIATION
   ERR = iicSetClockCalib( dwDuration, dwDeviation );
   if( ERR != ERR_CLOCK_EXCESS ) return ERR_PARAM_ERR;

   //get back value via iicClockCalibDirect
   //assumes, that FT=0 and OUT=0
   ERR = iicClockCalibDirect( READ_CALIB, &chCalib );
   if( ERR != ERR_OK ) return ERR;
   if( chCalib != 0x3d ) return ERR_CLOCK_NOREAD; //28.9

   //-------------------------------------------------------------------

   //test excess, function should return error but set calib to max
   dwDuration = 2000000;
   dwDeviation = -260;  //too big, calib value 31.9
   ERR = iicSetClockCalib( dwDuration, dwDeviation );
   if( ERR != ERR_CLOCK_EXCESS ) return ERR_PARAM_ERR;

   //get back value via iicClockCalibDirect
   //assumes, that FT=0 and OUT=0
   ERR = iicClockCalibDirect( READ_CALIB, &chCalib );
   if( ERR != ERR_OK ) return ERR;
   if( chCalib != 0x3f ) return ERR_CLOCK_NOREAD;

   return ERR_OK;
}

#endif //DEBUG
