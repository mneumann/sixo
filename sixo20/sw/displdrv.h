/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-08-18 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         diplay driver
 *  Purpose:        api export header
 *  Comments:       The display controller of SIxO 1.0 was a HD61202.
 *                  SIxO 2.0 got a KS0713. These two are compatible to the
 *                  greatest possible extent. Therefore comments mentioning
 *                  the HD61202 haven't been changed.
 *  Changes:        Added control of dummy read in LCDDrvReadData, AN 16.2.2003
 *                  Removed PIN_CENBL/BENBL, not present anymore, AN 12.02.2004
 *                  Changes for new display/hardware of SIxO 2.0 (see
 *                  comments preceeded by **AN), AN 27.12.2003
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

#ifndef _DISPLDRV_H
#define _DISPLDRV_H


// **AN for new SIxO 2.0 hardware
// M16C    LCD
// --------------
// P45  49 RS   4
// P46  48 R/W  5
// P47  47 E    6
// p60  38 CS1  15
// P61  37 CS2  16
// P64  34 RSTB 17
// P50  46 DB0  7
// P51  45 DB1  8
// P52  44 DB2  9
// P53  43 DB3 10
// P54  42 DB4 11
// P55  41 DB5 12
// P56  40 DB6 13
// P57  39 DB7 14


//polarity of data direction registers
#define PORT_IN  0x00                    //port is input port
#define PORT_OUT 0xff                    //port is output port

//redefine of port register of sfr62p.h for readability
#define ucADBus     p5                   //lcd address / data bus on port5
#define ucADBusD    pd5                  //direction register of port5
#define ucLCDCtrl0  p4                   //lcd display control bus on port4
#define ucLCDCtrl0D pd4                  //direction register of port4
#define ucLCDCtrl1  p6                   //lcd display control bus on port6
#define ucLCDCtrl1D pd6                  //direction register of port6
#define cLCDPwr     p0_3                 //power switch transistor of the LCD
#define cLCDPwrD    pd0_3                //direction of bit3 port0

#define BlightLvl   da0                  //D-A output for backlight brightness control
#define BlightLvlD  pd9_3                //direction of port 9 bit 3
#define ContrLvl    da1                  //D-A output for contrast level control
#define ContrLvlD   pd9_4                //direction of port 9 bit 4

//HD61202 control bytes for instruction writes
#define DISPLON     (unsigned char) 0x3f //display on
#define DISPLOFF    (unsigned char) 0x3e //display off
#define SETSTARTL   (unsigned char) 0xc0 //y start line read pointer 0..63 to OR'd into
#define SETYPAGE    (unsigned char) 0xb8 //y page number 0..6 to be OR'd into
#define SETXADDR    (unsigned char) 0x40 //x start adress 0..63 to be OR'd into

//HD61202 status bits after status read
#define STAT_BUSY   (unsigned char) 0x80 //display controller still busy
#define STAT_OFF    (unsigned char) 0x20 //display controller is OFF
#define STAT_RST    (unsigned char) 0x10 //display controller is in reset state
#define STAT_OK     (unsigned char) 0x00 //display controller is in normal mode

//pin usage to control HD61202 lcd controller bus 0
#define PIN_DI      (unsigned char) 0x20 //HD61202 'D/I' pin (high:data, low:instruction
#define PIN_RW      (unsigned char) 0x40 //HD61202 'R/W' pin (high:read, low:write)
#define PIN_EN      (unsigned char) 0x80 //HD61202 'Enable' pin
#define CLR_DRE     (unsigned char) 0x1F //clear DI/RW/E of lcd control byte 0

//pin usage to control HD61202 lcd controller bus 1
#define PIN_CS1     (unsigned char) 0x01 //HD61202 'CS1' pin (left)
#define PIN_CS2     (unsigned char) 0x02 //HD61202 'CS2' pin (right)
#define PIN_RST     (unsigned char) 0x10 //HD61202 'RSTB' pin (reset)

//display CONTROLLER specifications
#define Y_MAX_CONTR (unsigned char) 64   //controller memory y-pixel capability
#define X_MAX_CONTR (unsigned char) 64   //controller memory x-pixel capability
#define PAGE_HEIGHT (unsigned char) 8    //controller page height in pixel
#define PAGE_MAX    (unsigned char)(Y_MAX_CONTR / PAGE_HEIGHT) //controller y-page capability
#define PAGE_COLUMN_MAX (unsigned char)(X_MAX_CONTR) //controller x-pixel capability

//display MODULE specifications
#define N_CONTR_H   (unsigned char) 2    //number of horizontal display controllers
#define N_CONTR_V   (unsigned char) 1    //number of vertical display controllers
#define X_MAX_DISPL (unsigned char)(X_MAX_CONTR * N_CONTR_H) //display x-pixel capability
#define Y_MAX_DISPL (unsigned char)(Y_MAX_CONTR * N_CONTR_V) //display y-pixel capability

//test & dignostics
#define TESTPATTERN 0xaa                 //for lcd controller test


// public prototypes
ERRCODE LCDDrvSetSide( BOOL bSide );
ERRCODE LCDDrvWriteData( unsigned char ucValue );
ERRCODE LCDDrvInitLCD(void);
ERRCODE LCDDrvSetVisible( BOOL bMode);
ERRCODE LCDDrvSetPixel( unsigned char ucPixel);
ERRCODE LCDDrvGetPixel( unsigned char *pucPixel, BOOL bMode );
ERRCODE LCDDrvSetColumn( unsigned char ucColumn );
ERRCODE LCDDrvSetPage( unsigned char ucPage );
ERRCODE LCDDrvSetStartLine( unsigned char ucStartLine );
ERRCODE LCDDrvFillPattern( unsigned char ucPattern );
ERRCODE LCDDrvSetContrastLevel( UINT8 bContrast );
ERRCODE LCDDrvSetBacklightLevel( BOOL fLightOn, UINT8 bBrightness );
BOOL    LCDDrvGetBacklightState( void );
ERRCODE LCDDrvCheckController( BOOL bSide );
void    LCDDrvModuleTurnOff( void );


#endif // _DISPLDRV_H
