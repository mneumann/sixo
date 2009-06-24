/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-08-06 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         lcd driver
 *  Purpose:        low level lcd display driver functions
 *                  for HD61202 with 128x64 pixel b/w grafic display
 *  Comments:
 *                  MOST OF THESE FUNCTIONS AFFECT ON
 *                  ONLY 1 DISPLAY HALF (left/right)
 *
 *                  Use LCDDrvSetSide(BOOL bSide) to switch active
 *                  controller from LEFT to RIGHT (vs)!
 *
 *  Changes:        Added control of dummy read in LCDDrvReadData and
 *                  LCDDrvGetPixel, AN 16.2.2003
 *                  Changes for new display/hardware of SIxO 2.0 (backlight
 *                  and contrast functions are disabled at the moment, see
 *                  comments preceeded by **AN). Added functions with
 *                  timeout to get around hangups when the display is not
 *                  working. AN 27.12.2003
 *                  Fixed bug in LCDDrvReset. RST and CS1 remained low.
 *                  Exchanged accesses to ucLCDCtrl1(D). AN 11.02.2004
 *                  Turn LCD power off during reset. AN 12.02.2004
 *                  Function LCDDrvSetContrast deleted. There's no way
 *                  to disable the contrast voltage supply anymore.
 *                  Function LCDDrvSetContrastLevel re-enabled. AN 12.02.2004
 *                  In case of an error during display initialisation the
 *                  display is now completely turned off in LCDDrvInitLCD.
 *                  Removed backlight lookup tables. Current through the LEDs
 *                  is proportional to DA-value in the useful range.
 *                  Re-enabled LCDDrvSetBacklight and LCDDrvSetBacklightLevel.
 *                  Behaviour changed for new SIxO 2.0 hardware! These two
 *                  functions are not independant of each other anymore!
 *                  AN 12.02.2004
 *                  Changed contrast level and backlight level for new range
 *                  0..63. AN 13.02.2004
 *                  Removes LCDDrvSetBacklight and added the functionality to
 *                  LCDDrvSetBacklightLevel. AN 13.02.2004
 * Todo:            Port P4 of the new SIxO 2.0 hardware is not used for
 *                  anything else than E, DI, RW. So theres a way to program
 *                  the LCD driver very performance optimized.
 *                  Use bitset commands to access port pins.
 *                  Remove timing from commtents (not correct anymore).
 *                  Program timeouts with the system timer independent of
 *                  the processor speed (RESET_HL_TIME, RESET_TIMEOUT and
 *                  BUSY_TIMEOUT are'nt correct anymore).
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
 ************************************************************************ */


//includes
#include "sfr62p.h"
#include "standard.h"
#include "display.h"
#include "displdrv.h"
#include "digindrv.h"
#ifdef HARDCOPY
    #include "stdio.h"    // hardcopy via uart
#endif // HARDCOPY


//defines for timeouts and delays
#define POWER_HL_TIME 4767   //~10ms @ 16MHz
#define RESET_HL_TIME 10     //~20us @ 16MHz
#define RESET_TIMEOUT 476625 //~1s @ 16MHz
#define BUSY_TIMEOUT  4767   //~10ms @ 16MHz


//global display status
extern DPLFLAGS_TYPE    gDisplayFlags;      // just to get actual init settings



//global flag to disable and ignore all lcd outputs if lcd error detected
static BOOL gfLCD_Ok = TRUE; //default: ok


//private prototypes
static ERRCODE LCDDrvReset( void );
static ERRCODE LCDDrvReadInstr( unsigned char *pucValue );
static ERRCODE LCDDrvWriteInstr( unsigned char ucValue );
static ERRCODE LCDDrvReadData( unsigned char *pucValue, BOOL bMode );
static ERRCODE LCDDrvWriteInstr_Timeout( unsigned char ucInstr );
static ERRCODE LCDDrvWriteData_Timeout( unsigned char ucData );
static ERRCODE LCDDrvReadData_Timeout( unsigned char *pucData, BOOL bMode );


/***********************************************************************
 * FUNCTION:    LCDDrvSetSide
 * DESCRIPTION: Gives control to left or right display controller
 * PARAMETER:   LEFT / RIGHT
 * RETURN:      ok
 * COMMENT:     new SIxO 2.0 displays have two high active chip selects
 *
 *              NOTE:   We use this relative rarely called driver function
 *                      for a very short function test to detect display
 *                      failure/no failure and to set gfLCD_Ok!
 *
 *********************************************************************** */
ERRCODE LCDDrvSetSide( BOOL bSide )
{
    if( !gfLCD_Ok ) return ERR_OK; //LCD may be disconnected

    if( bSide == LEFT ){
       ucLCDCtrl1 &= ~PIN_CS2;     //CS2 = 0
       ucLCDCtrl1 |= PIN_CS1;      //CS1 = 1
    }
    else{
       ucLCDCtrl1 &= ~PIN_CS1;     //CS1 = 0
       ucLCDCtrl1 |= PIN_CS2;      //CS2 = 1
    }

    return ERR_OK;
}


/***********************************************************************
 * FUNCTION:    LCDDrvReset
 * DESCRIPTION: Hardware Reset of LCD controller. Init LCD control
 *              output and data bus. Select left controller.
 * PARAMETER:   -
 * RETURN:      ERR_OK or ERR_TIMEOUT
 * COMMENT:     Hard coded time loops, bad style.
 *              The display has a reset pin but I doubt that this is a
 *              real hardware reset. The SIxO 2.0 hardware is able to
 *              switch the display supply voltage on/off and with that
 *              generate a power up of the display. This is done here
 *              in case the display behaves wired (for example after
 *              starter operation in the vehicle).
 *              The LCD has a very low power consumption. To turn it off
 *              the control and data lines must also be set to low level.
 *              Otherwise the LCD is halfway on, drawing power through
 *              these signal lines.
 *********************************************************************** */
static ERRCODE LCDDrvReset( void )
{
   unsigned long cnt;
   unsigned char ucValue;

   if( !gfLCD_Ok ) return ERR_OK;                //LCD may be disconnected

   //turn off display
   //(see comment in header, contrast is set back to the saved user level in
   //the calling function LCDDrvInitLCD)
   //After executing this block the voltage at Pin 2 of the LCD connector should
   //be below 0.5V (measured 0.23V).
   ucADBus     = 0x00;                             //initial settings ucADBus bus, no data
   ucADBusD    = PORT_OUT;                         //all pins to output
   ucLCDCtrl0  &= ~( PIN_EN | PIN_RW | PIN_DI );   //set EN, RW, DI low (LCD disabled)
   ucLCDCtrl0D |= PIN_EN | PIN_RW | PIN_DI;        //set LCD pins to output
   ucLCDCtrl1  &= ~( PIN_RST | PIN_CS1 | PIN_CS2 ); //set RST and chip selects low
   ucLCDCtrl1D |= PIN_RST | PIN_CS2 | PIN_CS1;      //set LCD pins to output
   ContrLvl    = 0xff;                              //turn off contrast switch
   cLCDPwr     = 1;                                 //power switch off
   cLCDPwrD    = 1;                                 //set to output
   for( cnt=0; cnt<POWER_HL_TIME; cnt++ );          //~10ms @ 16MHz

   //turn LCD on
   //(about 5V at Pin 2 of the LCD connector, measured 5.01V with 5.03V system supply)
   cLCDPwr  = 0;
   for( cnt=0; cnt<POWER_HL_TIME; cnt++ );          //~10ms @ 16MHz

   //reset LCD and init control bus 0 and 1
   ucLCDCtrl1  |= PIN_RST | PIN_CS1;                //reset high, CS1 high
   ucLCDCtrl1  &= ~PIN_CS2;                         //CS2 low (left controller selected)
   for( cnt=0; cnt<RESET_HL_TIME; cnt++ );          //~20us @ 16MHz
   ucLCDCtrl1  &= ~PIN_RST;                         //reset low
   for( cnt=0; cnt<RESET_HL_TIME; cnt++ );          //~20us @ 16MHz, low time at least 1us for Displaytech 64128A
   ucLCDCtrl1  |= PIN_RST;                          //reset high again

   //wait for completion of reset
   //(to do it right we would have to wait for both controllers to finish)
   cnt = RESET_TIMEOUT;                             //~1s @ 16MHz
   do{
      LCDDrvReadInstr( &ucValue );
      cnt--;
   }while( (ucValue & STAT_RST) && (cnt > 0) );

   if( cnt > 0 ){
      return ERR_OK;
   }
   else{
      return ERR_TIMEOUT;
   }
}


/***********************************************************************
 *  FUNCTION:       LCDDrvSetContrastLevel
 *  DESCRIPTION:    Sets the lcd module contrast level
 *
 *  PARAMETER:      level       from 0=light .. 63=dark
 *  RETURN:         ERR_OK
 *  COMMENT:        Detects Hardware-ID to setup correct contrast
 *                  values for green or white display modules.
 *                  Limits the lower value to avoid a setting
 *                  (at room temperature) whith that nothing
 *                  can be seen on the display anymore:
 *                  Usable ContrLvl values:
 *                      1st charge (green display): 129..255
 *                      2nd charge (white display): 110..173
 *                  Note that 2nd charge uses different resistor
 *                  values for contrast driver!
 *********************************************************************** */
ERRCODE LCDDrvSetContrastLevel( UINT8 bContrast )
{
    UINT8 u8HwId = DigInDrv_GetHWVersion();

    // LCD may be disconnected
    if( !gfLCD_Ok )
        return ERR_OK;

    // check parameter
    if( bContrast > 63 )
        return ERR_PARAM_ERR;

    // Select correct HW settings
    if (u8HwId < 2)
    {
        // settings for 1st production charge
        bContrast = bContrast << 1;     // scale given value x2
        ContrLvl = 129 + bContrast;     // add to specific offset value
    }
    else if (u8HwId == 2)
    {
        // settings for 2nd production charge (different resistors)
        bContrast = bContrast << 0;     // scale given value x1
        ContrLvl = 110 + bContrast;     // add to specific offset value
    }
    else
    {
        // invalid HW id!
    }

    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       LCDDrvSetBacklightLevel
 *  DESCRIPTION:    Sets the lcd module backlight brightness and turns
 *                  the backlight on/off
 *  PARAMETER:      fLightOn        TRUE if backlight is to be switched ON
 *                                  FALSE else
 *                  bBrightness     given back light level (see note)
 *                                  valid: 0..63
 *  RETURN:         ERR_OK
 *  COMMENT:        Current through the LEDs is proportional to the DA-value
 *                  BlightLvl in the useful range 80..255. Below 80 the
 *                  backlight is off.
 *                  BlightLvl<80 off, 80..255 = 0..33mA with R7=82 Ohm
 *********************************************************************** */
ERRCODE LCDDrvSetBacklightLevel( BOOL fLightOn, UINT8 bBrightness )
{
   UINT8 bValue;

   if( !gfLCD_Ok ) return ERR_OK; //LCD may be disconnected

   //check parameter
   if( bBrightness > 63 ) return ERR_PARAM_ERR;

   if( fLightOn == TRUE ){
      bValue =  bBrightness << 1; //limit the lower value since below 80
      bValue += bBrightness;      //the backlight is off anyway
      BlightLvl = 66 + bValue;    //66..255 with input 0..63
   }
   else{
      BlightLvl = 0x00;           //turn backlight off
   }

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       LCDDrvGetBacklightState
 *  DESCRIPTION:    Returns current Backlight state ON/OFF
 *  PARAMETER:      -
 *  RETURN:         TRUE        backlight ON
 *                  FALSE       backlight OFF
 *  COMMENT:        Current through the LEDs is proportional to the DA-value
 *                  BlightLvl in the useful range 80..255. Below 80 the
 *                  backlight is off.
 *                  BlightLvl<80 off, 80..255 = 0..33mA with R7=82 Ohm
 *********************************************************************** */
BOOL LCDDrvGetBacklightState( void )
{
    if ( BlightLvl < 80 )
         return (FALSE);    // OFF per definition
    else return (TRUE);
}



/***********************************************************************
 * FUNCTION:    LCDDrvReadInstr
 * DESCRIPTION: reads back the lcd controllers status byte
 * PARAMETER:   UCHAR * pucValue    R-Value status byte
 * RETURN:      ERR_OK
 * COMMENT:     -
 *********************************************************************** */
static ERRCODE LCDDrvReadInstr( unsigned char *pucValue )
{
   if( !gfLCD_Ok ) return ERR_OK; //LCD may be disconnected

   ucADBusD   =  PORT_IN;         //set port to input
   ucLCDCtrl0 &= CLR_DRE;         //clear D/I, R/W, E !!! t(as)min = 140 ns between Set(R/W) <-> Set(E)
   ucLCDCtrl0 |= PIN_RW;          //Set R/W to READ (while waiting)
   ucLCDCtrl0 |= PIN_EN;          //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
   *pucValue  =  ucADBus;         //[200 ns] save current state before disable E
   ucLCDCtrl0 &= ~PIN_EN;         //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!
   ucLCDCtrl0 &= ~PIN_RW;         //RW=low=write
   ucADBusD   =  PORT_OUT;        //set port back to OUTPUT

   return ERR_OK;
}


/***********************************************************************
 * FUNCTION:    LCDDrvWriteInstr
 * DESCRIPTION: Writes an instruction to lcd controller
 * PARAMETER:   unsigned char ucInstr
 * RETURN:      Ok
 * COMMENT:     -
 *********************************************************************** */
static ERRCODE LCDDrvWriteInstr( unsigned char ucInstr )
{
   UINT8    bLCDStatus;

   if( !gfLCD_Ok ) return ERR_OK;    //LCD may be disconnected

   //init
   ucADBusD   =  PORT_IN;            //set port to INPUT
   ucLCDCtrl0 &= CLR_DRE;            //clear D/I, R/W, E [t(as)min = 140 ns between Set(R/W) <-> Set(E)]
   ucLCDCtrl0 |= PIN_RW;             //Set R/W to READ (while waiting)

   //wait busy
   do{
      ucLCDCtrl0 |= PIN_EN;          //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
      bLCDStatus =  ucADBus;         //[200 ns] save current state before disable E
      ucLCDCtrl0 &= ~PIN_EN;         //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!
   }
   while(bLCDStatus & STAT_BUSY);    //[200 ns] if jump back to loop begin, [300 ns] check BUSY flag

   //write instruction
   ucLCDCtrl0 &= ~PIN_RW;            //[200 ns] Clear R/W to 'Write' (while waiting)
   ucADBusD   =  PORT_OUT;           //[200 ns] set port to OUTPUT (while waiting)
   ucADBus    =  ucInstr;            //[300 ns] Set output instruction byte
   ucLCDCtrl0 |= PIN_EN;             //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
   ucLCDCtrl0 &= ~PIN_EN;            //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!

   return ERR_OK;
}


/***********************************************************************
 * FUNCTION:    LCDDrvWriteInstr_Timeout
 * DESCRIPTION: Writes an instruction to lcd controller
 *              Does not hang if LCD is not present. Use only for LCD
 *              presence check, than use LCDDrvWriteInstr with better
 *              performance.
 * PARAMETER:   unsigned char ucInstr
 * RETURN:      Ok
 * COMMENT:     Hard coded time loops, bad style.-
 *********************************************************************** */
static ERRCODE LCDDrvWriteInstr_Timeout( unsigned char ucInstr )
{
   unsigned long cnt;
   UINT8 bLCDStatus;

   if( !gfLCD_Ok ) return ERR_OK; //LCD may be disconnected

   //init
   ucADBusD   =  PORT_IN;         //set port to INPUT
   ucLCDCtrl0 &= CLR_DRE;         //clear D/I, R/W, E [t(as)min = 140 ns between Set(R/W) <-> Set(E)]
   ucLCDCtrl0 |= PIN_RW;          //Set R/W to READ (while waiting)

   //wait busy
   cnt = BUSY_TIMEOUT;
   do{
      ucLCDCtrl0 |= PIN_EN;       //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
      bLCDStatus = ucADBus;       //[200 ns] save current state before disable E
      ucLCDCtrl0 &= ~PIN_EN;      //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!
      cnt--;
   }while( (bLCDStatus & STAT_BUSY) && (cnt>0) ); //[200 ns] if jump back to loop begin, check BUSY flag and timeout

   //write instruction
   ucLCDCtrl0 &= ~PIN_RW;         //[200 ns] Clear R/W to 'Write' (while waiting)
   ucADBusD   =  PORT_OUT;        //[200 ns] set port to OUTPUT (while waiting)
   ucADBus    =  ucInstr;         //[300 ns] Set output instruction byte
   ucLCDCtrl0 |= PIN_EN;          //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
   ucLCDCtrl0 &= ~PIN_EN;         //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!

   return ERR_OK;
}


/***********************************************************************
 * FUNCTION:    LCDDrvWriteData
 * DESCRIPTION: Writes 8 bit data to lcd controller
 * PARAMETER:   unsigned char ucInstr
 * RETURN:      Ok
 * COMMENT:     NOTE:   THIS CODE IS ASSEMBLER FINE TUNED!
 *                      We don't need any nops, if we use the display
 *                      instruction time for µC purpose.
 *                      But: Don't change any order of instructions!
 *********************************************************************** */
ERRCODE LCDDrvWriteData( unsigned char ucData )
{
   UINT8 bLCDStatus;

   if( !gfLCD_Ok ) return ERR_OK;    //LCD may be disconnected

   //init
   ucADBusD   =  PORT_IN;            //set port to INPUT
   ucLCDCtrl0 &= CLR_DRE;            //clear D/I, R/W, E [t(as)min = 140 ns between Set(R/W) <-> Set(E)]
   ucLCDCtrl0 |= PIN_RW;             //Set R/W to READ (while waiting)

   //wait busy
   do{
      ucLCDCtrl0  |= PIN_EN;         //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
      bLCDStatus  = ucADBus;         //[200 ns] save current state before disable E
      ucLCDCtrl0  &= ~PIN_EN;        //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!
   }
   while(bLCDStatus & STAT_BUSY);    //[200 ns] if jump back to loop begin, [300 ns] check BUSY flag

   //write data
   ucLCDCtrl0 &= ~PIN_RW;            //[200 ns] Clear R/W to 'Write' (while waiting)
   ucADBusD   =  PORT_OUT;           //[200 ns] set port to OUTPUT
   ucLCDCtrl0 |= PIN_DI;             //[200 ns] Set D/I to 'Data' !!! t(as)min = 140 ns between Set(D/I) <-> Set(E)
   ucLCDCtrl0 |= PIN_EN;             //[200 ns] Set Enable
   ucADBus    =  ucData;             //[300 ns] Set output data byte !!! t(dsw)min = 200 ns between Set(DB) <-> Clear(E)

   ucLCDCtrl0 &= ~PIN_EN;            //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!

   return ERR_OK;
}


/***********************************************************************
 * FUNCTION:    LCDDrvWriteData_Timeout
 * DESCRIPTION: Writes 8 bit data to lcd controller
 *              Does not hang if LCD is not present. Use only for LCD
 *              presence check, than use LCDDrvWriteData with better
 *              performance.
 * PARAMETER:   unsigned char ucInstr
 * RETURN:      Ok
 * COMMENT:     NOTE:   THIS CODE IS ASSEMBLER FINE TUNED!
 *                      We don't need any nops, if we use the display
 *                      instruction time for µC purpose.
 *                      But: Don't change any order of instructions!
 *              Hard coded time loops, bad style.
 *********************************************************************** */
static ERRCODE LCDDrvWriteData_Timeout( unsigned char ucData )
{
    unsigned long cnt;
    UINT8 bLCDStatus;

    //init
    ucADBusD    =  PORT_IN;     //set port to INPUT
    ucLCDCtrl0  &= CLR_DRE;     //clear D/I, R/W, E [t(as)min = 140 ns between Set(R/W) <-> Set(E)]
    ucLCDCtrl0  |= PIN_RW;      //Set R/W to READ (while waiting)

    //wait busy
    cnt = BUSY_TIMEOUT;
    do
    {
        ucLCDCtrl0  |= PIN_EN;  //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
        bLCDStatus  = ucADBus;  //[200 ns] save current state before disable E
        ucLCDCtrl0  &= ~PIN_EN; //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!
        cnt--;
    }                           //[200 ns] if jump back to loop begin
    while( (bLCDStatus & STAT_BUSY) && (cnt>0) ); //check BUSY flag and timeout

    //write data
    ucLCDCtrl0  &= ~PIN_RW;     //[200 ns] Clear R/W to 'Write' (while waiting)
    ucADBusD    =  PORT_OUT;    //[200 ns] set port to OUTPUT
    ucLCDCtrl0  |= PIN_DI;      //[200 ns] Set D/I to 'Data' !!! t(as)min = 140 ns between Set(D/I) <-> Set(E)
    ucLCDCtrl0  |= PIN_EN;      //[200 ns] Set Enable
    ucADBus      = ucData;      //[300 ns] Set output data byte !!! t(dsw)min = 200 ns between Set(DB) <-> Clear(E)

    ucLCDCtrl0  &= ~PIN_EN;     //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!

    return ERR_OK;
}


/***********************************************************************
 * FUNCTION:    LCDDrvReadData
 * DESCRIPTION: Reads back data from lcd controller display memory
 * PARAMETER:   unsigned char target pointer of ucData to be read
 *              flag if a dummy read cycle must be done (DUMMY_ON/DUMMY_OFF)
 * RETURN:      Ok
 * COMMENT:     NOTE1:  We need a 'dummy read' as specified in hd61202
 *                      data sheet, to get a copy of output BUFFER into
 *                      output REGISTER if the address was set right
 *                      before the read.
 *              NOTE2:  f(max) for 'ENABLE-Pin' is 1/(450+450ns) !
 *              NOTE3:  Values in [ ] are µC instruction times
 *              NOTE4:  THIS CODE IS ASSEMBLER FINE TUNED!
 *                      We don't need any nops, if we use the display
 *                      instruction time for µC purpose.
 *                      But: Don't change any order of instructions!
 * CHANGES:     added control of the dummy read cycle for support of
 *              multiple reads without address set (sequence is address
 *              set, dummy read, read data0, read data1,...), AN 16.2.2003
 ********************************************************************** */
static ERRCODE LCDDrvReadData( unsigned char * pucData, BOOL bMode )
{
   UINT8 bLCDStatus;

   if( !gfLCD_Ok ) return ERR_OK;    //LCD may be disconnected

   //init
   ucADBusD   =  PORT_IN;            //set port to INPUT
   ucLCDCtrl0 &= CLR_DRE;            //clear D/I, R/W, E [t(as)min = 140 ns between Set(R/W) <-> Set(E)]
   ucLCDCtrl0 |= PIN_RW;             //Set R/W to READ (while waiting)

   //wait busy
   ucLCDCtrl0 &= ~PIN_DI;            //[200 ns] Set D/I to 'Instruction'
   do{
      ucLCDCtrl0 |= PIN_EN;          //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
      bLCDStatus = ucADBus;          //[200 ns] save current state before disable E
      ucLCDCtrl0 &= ~PIN_EN;         //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!
   }while( bLCDStatus & STAT_BUSY ); //[200 ns] if jump back to loop begin, [300 ns] check BUSY flag

   if( bMode == DUMMY_ON ){
      //first (dummy) read
      ucLCDCtrl0 |= PIN_DI;          //[200 ns] Set D/I to 'Data'
      ucLCDCtrl0 |= PIN_EN;          //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
      //NOP;                         //WE DON'T READ ANYTHING; SO WE DON'T NEED A NOP TO READ CORRECT DATA!
      ucLCDCtrl0 &= ~PIN_EN;         //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!

      //wait busy
      ucLCDCtrl0 &= ~PIN_DI;         //[200 ns] Set D/I to 'Instruction'
      do{
         ucLCDCtrl0 |= PIN_EN;       //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
         bLCDStatus =  ucADBus;      //[200 ns] save current state before disable E
         ucLCDCtrl0 &= ~PIN_EN;      //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!
      }while( bLCDStatus & STAT_BUSY ); //[200 ns] jump back to loop begin, [300 ns] check BUSY flag
   }

   //second (real) data read
   ucLCDCtrl0 |= PIN_DI;             //[200 ns] Set D/I to 'Data'
   ucLCDCtrl0 |= PIN_EN;             //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
   *pucData   =  ucADBus;            //[600! ns] Save data byte
   ucLCDCtrl0 &= ~PIN_EN;            //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!

   //de-init
   ucLCDCtrl0 &= ~PIN_RW;            //5. RW=low=write
   ucADBusD   = PORT_OUT;            //set port back to OUTPUT

   return ERR_OK;
}


/***********************************************************************
 * FUNCTION:    LCDDrvReadData_Timeout
 * DESCRIPTION: Reads back data from lcd controller display memory
 *              Does not hang if LCD is not present. Use only for LCD
 *              presence check, than use LCDDrvReadData with better
 *              performance.
 * PARAMETER:   unsigned char target pointer of ucData to be read
 *              flag if a dummy read cycle must be done (DUMMY_ON/DUMMY_OFF)
 * RETURN:      Ok
 * COMMENT:     NOTE1:  We need a 'dummy read' as specified in hd61202
 *                      data sheet, to get a copy of output BUFFER into
 *                      output REGISTER if the address was set right
 *                      before the read.
 *              NOTE2:  f(max) for 'ENABLE-Pin' is 1/(450+450ns) !
 *              NOTE3:  Values in [ ] are µC instruction times
 *              NOTE4:  THIS CODE IS ASSEMBLER FINE TUNED!
 *                      We don't need any nops, if we use the display
 *                      instruction time for µC purpose.
 *                      But: Don't change any order of instructions!
 *              Hard coded time loops, bad style.
 * CHANGES:     added control of the dummy read cycle for support of
 *              multiple reads without address set (sequence is address
 *              set, dummy read, read data0, read data1,...), AN 16.2.2003
 ********************************************************************** */
static ERRCODE LCDDrvReadData_Timeout( unsigned char *pucData, BOOL bMode )
{
   unsigned long cnt;
   UINT8 bLCDStatus;

   if( !gfLCD_Ok ) return ERR_OK; //LCD may be disconnected

   //init
   ucADBusD   =  PORT_IN;         //set port to INPUT
   ucLCDCtrl0 &= CLR_DRE;         //clear D/I, R/W, E [t(as)min = 140 ns between Set(R/W) <-> Set(E)]
   ucLCDCtrl0 |= PIN_RW;          //Set R/W to READ (while waiting)

   //wait busy
   ucLCDCtrl0 &= ~PIN_DI;         //[200 ns] Set D/I to 'Instruction'
   cnt = BUSY_TIMEOUT;
   do{
      ucLCDCtrl0  |= PIN_EN;      //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
      bLCDStatus  = ucADBus;      //[200 ns] save current state before disable E
      ucLCDCtrl0  &= ~PIN_EN;     //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!
      cnt--;
   }while( (bLCDStatus & STAT_BUSY) && (cnt>0) ); //[200 ns] if jump back to loop begin, check BUSY flag and timeout

   if( bMode == DUMMY_ON ){
      //first (dummy) read
      ucLCDCtrl0 |= PIN_DI;       //[200 ns] Set D/I to 'Data'
      ucLCDCtrl0 |= PIN_EN;       //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
      //NOP;                      //WE DON'T READ ANYTHING; SO WE DON'T NEED A NOP TO READ CORRECT DATA!
      ucLCDCtrl0 &= ~PIN_EN;      //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!

      //wait busy
      ucLCDCtrl0 &= ~PIN_DI;      //[200 ns] Set D/I to 'Instruction'
      cnt = BUSY_TIMEOUT;
      do{
         ucLCDCtrl0 |= PIN_EN;    //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
         bLCDStatus =  ucADBus;   //[200 ns] save current state before disable E
         ucLCDCtrl0 &= ~PIN_EN;   //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!
         cnt--;
      }while( (bLCDStatus & STAT_BUSY) && (cnt>0) ); //[200 ns] jump back to loop begin, check BUSY flag and timeout
   }

   //second (real) data read
   ucLCDCtrl0 |= PIN_DI;          //[200 ns] Set D/I to 'Data'
   ucLCDCtrl0 |= PIN_EN;          //[200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB)
   *pucData   =  ucADBus;         //[600! ns] Save data byte
   ucLCDCtrl0 &= ~PIN_EN;         //[200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high!

   //de-init
   ucLCDCtrl0 &= ~PIN_RW;         //5. RW=low=write
   ucADBusD   = PORT_OUT;         //set port back to OUTPUT

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       LCDDrvCheckController
 *  DESCRIPTION:    LCD controller (write & read back) test to check, wether
 *                  a) LCD is connected and ok or
 *                  b) LCD is disconnected or defect.
 *  PARAMETER:      -
 *  RETURN:         ERR_OK              if LCD ok
 *                  ERR_NOT_SUPPORTED   if LCD not ok (e.g. not connected)
 *  COMMENT:        > To be used for debug purpose, to run application
 *                  without LCD.
 *                  > Checks only one controller side!
 *  CHANGES:        extra Parameter in call to LCDDrvReadData added, AN 16.2.2003
 *********************************************************************** */
ERRCODE LCDDrvCheckController( BOOL bSide )
{
    UINT8 bTestPattern;

    //write & read back test, ***use routines with timeout here!!***
    LCDDrvSetSide( bSide );                 //set left or right controller active
    LCDDrvWriteInstr( DISPLON );            //default: set display on
    LCDDrvWriteInstr_Timeout( SETSTARTL );  //set Y display start line to 0
    LCDDrvWriteInstr_Timeout( SETXADDR  );  //set x column address to 0
    LCDDrvWriteInstr_Timeout( SETYPAGE );   //set page (8 bit y address) to 0
    LCDDrvWriteData_Timeout( TESTPATTERN ); //write test pattern
    LCDDrvWriteInstr_Timeout( SETXADDR  );  //set x column back to address 0
    LCDDrvReadData_Timeout( &bTestPattern, DUMMY_ON ); //read back test pattern

    //check results
    if( bTestPattern == TESTPATTERN )
    {
        return ERR_OK;
    }
    else
    {
        LCDDrvModuleTurnOff();              // turn of all M16C output ports
        gfLCD_Ok = FALSE;                   // set global error flag, blocks the LCD routines
        return ERR_NOT_SUPPORTED;
    }
}




/***********************************************************************
 *  FUNCTION:       LCDDrvModuleTurnOff
 *  DESCRIPTION:    turns off all M16C output action according to
 *                  LCD module
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        prevent any damage or LCD drive malfucntion if
 *                  LCD module is defect or removed.
 *                  Use LCDDrvInitLCD() to re-initialize LCD module!
 *********************************************************************** */
void LCDDrvModuleTurnOff( void )
{
    //turn off display
    ucADBus    = 0x00;                              // initial settings ucADBus bus, no data
    ucLCDCtrl0 &= ~( PIN_EN | PIN_RW | PIN_DI );    // set EN, RW, DI low (LCD disabled)
    ucLCDCtrl1 &= ~( PIN_RST | PIN_CS1 | PIN_CS2 ); // set RST and chip selects low
    BlightLvl  = 0;                                 // turn off backlight
    ContrLvl   = 0xff;                              // turn off contrast switch
    cLCDPwr    = 1;                                 // power switch off
}





/***********************************************************************
 * FUNCTION:    LCDDrvInitLCD
 * DESCRIPTION: Initializes uC relevant ports and lcd status
 * PARAMETER:   -
 * RETURN:      ERR_OK              if lcd ok
 *              ERR_NOT_SUPPORTED   if lcd disconnected or defect
 * COMMENT:     If no error detected: activates and clears display but
 *              contrast and backlight is not set and enabled.
 *              If error detected: backlight and contrast off, lcd
 *              routines blocked via global flag gfLCD_OK.
 *********************************************************************** */
ERRCODE LCDDrvInitLCD( void )
{
    ERRCODE RValue;

    // enable access to LCD
    gfLCD_Ok = TRUE;

    // init LCD control output, reset LCD, init data bus
    RValue = LCDDrvReset();

    // check wether module works well
    if (  ( LCDDrvCheckController( LEFT )  == ERR_NOT_SUPPORTED )
        ||( LCDDrvCheckController( RIGHT ) == ERR_NOT_SUPPORTED ) )
        return ERR_NOT_SUPPORTED;

    // initial settings D-A conversion for backlight and contrast
    BlightLvl  = 0;              // turn off backlight
    BlightLvlD = 0;              // must be input for D-A conversion
    ContrLvl   = 0xff;           // turn off contrast switch
    ContrLvlD  = 0;              // must be input for D-A conversion
    dacon      = 0x03;           // enable D-A conversion

    // activate left display controller and init
    LCDDrvSetSide( LEFT );       // left lcd controller active
    LCDDrvSetStartLine( 0 );     // set Y display start line to 0
    LCDDrvSetColumn( 0 );        // set x column address to 0
    LCDDrvSetPage( 0 );          // set page (8 bit y address) to 0
    LCDDrvSetVisible( ON );      // switch display controller on
    LCDDrvFillPattern( 0x00 );   // clear screen

    // activate right display controller and init
    LCDDrvSetSide( RIGHT );      // right lcd controller active
    LCDDrvSetStartLine( 0 );     // set Y display start line to 0
    LCDDrvSetColumn( 0 );        // set x column address to 0
    LCDDrvSetPage( 0 );          // set page (8 bit y address) to 0
    LCDDrvSetVisible( ON );      // switch display controller on
    LCDDrvFillPattern( 0x00 );   // clear screen

    return ERR_OK;
}


/***********************************************************************
 * FUNCTION:    LCDDrvSetVisible
 * DESCRIPTION: Switches the controller to visible/invisible mode
 * PARAMETER:   ON / OFF
 * RETURN:      ok
 * COMMENT:     -
 *********************************************************************** */
ERRCODE LCDDrvSetVisible( BOOL bMode)
{
   if( !gfLCD_Ok ) return ERR_OK; //LCD may be disconnected

   if( bMode == ON ){
      LCDDrvWriteInstr( DISPLON );
   }
   else{
      LCDDrvWriteInstr( DISPLOFF );
   }

   return ERR_OK;
}


/***********************************************************************
 * FUNCTION:    LCDDrvSetPixel
 * DESCRIPTION: Prints out one set of pixel (8 vertical bits) to controller
 * PARAMETER:   unsigned char ucPixel to be displayed
 * RETURN:      ok
 * COMMENT:     -
 *********************************************************************** */
ERRCODE LCDDrvSetPixel( unsigned char ucPixel)
{
   if( !gfLCD_Ok ) return ERR_OK; //LCD may be disconnected

   LCDDrvWriteData( ucPixel );

   return ERR_OK;
}


/***********************************************************************
 * FUNCTION:    LCDDrvGetPixel
 * DESCRIPTION: Reads back set of pixel (8 vertical bits) from controller
 * PARAMETER:   unsigned char *pucPixel to be read from lcd memory
 *              flag if a dummy read cycle must be done (DUMMY_ON/DUMMY_OFF)
 * RETURN:      ok
 * COMMENT:     -
 * CHANGES:     added control of the dummy read cycle for support of
 *              multiple reads without address set (sequence is address
 *              set, dummy read, read data0, read data1,...), AN 16.2.2003
 *********************************************************************** */
ERRCODE LCDDrvGetPixel( unsigned char *pucPixel, BOOL bMode )
{
   if( !gfLCD_Ok ) return ERR_OK; //LCD may be disconnected

   LCDDrvReadData( pucPixel, bMode );

   return ERR_OK;
}


/***********************************************************************
 * FUNCTION:    LCDDrvSetColumn
 * DESCRIPTION: Sets controller current output X-pointer to column n
 * PARAMETER:   unsigned char uiXColumn
 * RETURN:      ok
 * COMMENT:     TBD: check LCDDrv return value!
 *********************************************************************** */
ERRCODE LCDDrvSetColumn( unsigned char ucColumn )
{
   if( !gfLCD_Ok ) return ERR_OK;              //LCD may be disconnected

   if( ucColumn < PAGE_COLUMN_MAX ){
      LCDDrvWriteInstr( SETXADDR | ucColumn ); //set x column address
      return ERR_OK;
   }

   return ERR_PARAM_ERR;
}


/***********************************************************************
 * FUNCTION:    LCDDrvSetPage
 * DESCRIPTION: Sets controller current output Y pointer to page n
 * PARAMETER:   unsigned char ucPage
 * RETURN:      ok
 * COMMENT:     TBD: check LCDDrv return value!
 *********************************************************************** */
ERRCODE LCDDrvSetPage( unsigned char ucPage )
{
   if( !gfLCD_Ok ) return ERR_OK;            //LCD may be disconnected

   if( ucPage < PAGE_MAX ){
      LCDDrvWriteInstr( SETYPAGE | ucPage ); //set page (8 bit y address)
      return ERR_OK;
   }

   return ERR_PARAM_ERR;
}


/***********************************************************************
 * FUNCTION:    LCDDrvSetStartLine
 * DESCRIPTION: Sets controller current output start line (scroll effects!)
 * PARAMETER:   unsigned char ucStartLine
 * RETURN:      ok
 * COMMENT:     TBD: check LCDDrv return value!
 *
 *              Scrolls across the hidden 8th page!
 *********************************************************************** */
ERRCODE LCDDrvSetStartLine( unsigned char ucStartLine )
{
   if( !gfLCD_Ok ) return ERR_OK;                  //LCD may be disconnected

   if( ucStartLine < Y_MAX_CONTR ){
      LCDDrvWriteInstr( SETSTARTL | ucStartLine ); //set startline
      return ERR_OK;
   }

   return ERR_PARAM_ERR;
}


/***********************************************************************
 * FUNCTION:    LCDDrvFillPattern
 * DESCRIPTION: Fills controller memory with pattern
 * PARAMETER:   const unsigned char ucPattern
 * RETURN:      ok
 * COMMENT:     Uses implicit X-Column increment of lcd controller
 *              (quasi burst mode )
 *********************************************************************** */
ERRCODE LCDDrvFillPattern( unsigned char ucPattern )
{
   unsigned char ucPage;
   unsigned char ucColumn;

   if( !gfLCD_Ok ) return ERR_OK; //LCD may be disconnected

   //fill complete display half
   for( ucPage = 0; ucPage < PAGE_MAX; ucPage++ ){
      LCDDrvSetColumn( 0 );       //set X column to 0
      LCDDrvSetPage( ucPage );    //set y page

      //fill complete page
      for( ucColumn = 0; ucColumn < PAGE_COLUMN_MAX; ucColumn++ ){
         LCDDrvWriteData( ucPattern );
      }
   }

   return ERR_OK;
}






#ifdef HARDCOPY

/***********************************************************************
 * FUNCTION:    LCDDrvHardcopySend
 * DESCRIPTION: sort and transmit two pages
 * PARAMETER:   -
 * RETURN:      -
 * COMMENT:     For documentation purpose only
 *              Note: DebugOut (printf) must be enabled via 'initl.c'
 *              settings!
 *********************************************************************** */
void LCDDrvHardcopySend( unsigned char *ucData )
{
   unsigned char ucBitm;
   unsigned char ucLine;
   unsigned char ucOffs;
   unsigned char ucByte;

   ucBitm = 1;
   for( ucLine = 0; ucLine < 8; ucLine ++ ){
      for( ucOffs = 0; ucOffs < 2*PAGE_COLUMN_MAX; ucOffs += 8 ){
            ucByte = ( ( (ucData[  ucOffs]&ucBitm) >> ucLine ) << 7 )
                   + ( ( (ucData[1+ucOffs]&ucBitm) >> ucLine ) << 6 )
                   + ( ( (ucData[2+ucOffs]&ucBitm) >> ucLine ) << 5 )
                   + ( ( (ucData[3+ucOffs]&ucBitm) >> ucLine ) << 4 )
                   + ( ( (ucData[4+ucOffs]&ucBitm) >> ucLine ) << 3 )
                   + ( ( (ucData[5+ucOffs]&ucBitm) >> ucLine ) << 2 )
                   + ( ( (ucData[6+ucOffs]&ucBitm) >> ucLine ) << 1 )
                   +   ( (ucData[7+ucOffs]&ucBitm) >> ucLine );
            _sput( ~ucByte );  // invert -> black on white!
      }
      ucBitm = ucBitm << 1;
   }
}


/***********************************************************************
 * FUNCTION:    LCDDrvHardcopy
 * DESCRIPTION: transmit screen contents over serial interface as BMP
 * PARAMETER:   -
 * RETURN:      -
 *              Note: DebugOut (printf) must be enabled via 'initl.c'
 *              settings!
 *********************************************************************** */
void LCDDrvHardcopy( void )
{
   static unsigned char bmp_header[]={
      0x42, 0x4D, 0xBE, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x28, 0x00,
      0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0xB0, 0x07, 0x00, 0x00, 0xB0, 0x07, 0x00, 0x00, 0x02, 0x00,
      0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00};
   unsigned char  ucPage;
   unsigned char  ucColumn;
   unsigned char  ucData[2*PAGE_COLUMN_MAX];
   unsigned char  ucMirrorByte;

   //printf("\n---------- BMP starts next line ----------\n");
   for( ucPage = 0; ucPage < sizeof( bmp_header ); ucPage++ ){
      _sput( bmp_header[ucPage] );
   }

   for( ucPage = 0; ucPage < PAGE_MAX; ucPage++ ){
      // get complete left controller page
      LCDDrvSetSide( LEFT );
      LCDDrvSetPage( PAGE_MAX-1 - ucPage );     // read pages backward -> vertikal mirrored!
      LCDDrvSetColumn( 0 );
      LCDDrvGetPixel( &ucData[0], DUMMY_ON );
      for( ucColumn = 1; ucColumn < PAGE_COLUMN_MAX; ucColumn++ ){
         LCDDrvGetPixel( &ucData[ucColumn], DUMMY_OFF );
      }
      // append complete right controller page
      LCDDrvSetSide( RIGHT );
      LCDDrvSetPage( PAGE_MAX-1 - ucPage );     // read pages backward -> vertikal mirrored!
      LCDDrvSetColumn( 0 );
      LCDDrvGetPixel( &ucData[PAGE_COLUMN_MAX], DUMMY_ON );
      for( ucColumn = 1; ucColumn < PAGE_COLUMN_MAX; ucColumn++ ){
         LCDDrvGetPixel( &ucData[ucColumn+PAGE_COLUMN_MAX], DUMMY_OFF );
      }

      // mirror MSB<->LSB of data
      for( ucColumn = 0; ucColumn < (2*PAGE_COLUMN_MAX); ucColumn++ ){
         ucMirrorByte =  ((ucData[ucColumn]&0x01) << 7)
                        +((ucData[ucColumn]&0x02) << 5)
                        +((ucData[ucColumn]&0x04) << 3)
                        +((ucData[ucColumn]&0x08) << 1)
                        +((ucData[ucColumn]&0x10) >> 1)
                        +((ucData[ucColumn]&0x20) >> 3)
                        +((ucData[ucColumn]&0x40) >> 5)
                        +((ucData[ucColumn]&0x80) >> 7);
         ucData[ucColumn] = ucMirrorByte;
      }

      LCDDrvHardcopySend( ucData );
   }
   //printf("\n---------- BMP ended line before ----------\n");
   return;
}



#endif // HARDCOPY




