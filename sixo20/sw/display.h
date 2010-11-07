/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        16-02-2001 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         lcd high level driver
 *  Purpose:        display service functions api export header
 *  Comments:       -
 *  Changes:        Changes for new display/hardware of SIxO 2.0 (see
 *                  comments preceeded by **AN), AN 27.12.2003
 *                  Changed the default display level to 0xC0, AN 12.02.2004
 *                  DPLFLAGS_TYPE changed to support backlight level with
 *                  bigger range, reduced contrast range. New default
 *                  values. AN 13.03.2004
 *                  DPLFLAGS_TYPE now sizeof dword, MM 13.03.2004
 *                  added fBacklAuto flag to DPLFLAGS_TYPE, MM 13.03.2004
 *                  added fContrastAuto flag to DPLFLAGS_TYPE, MM 13.03.2004
 *                  set default display settings to 0x3f23, MM 13.3.2004
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
 * Revision 3.0  2010/11/07 13:20:14  tuberkel
 * V30 Preparations - no changes
 *
 * Revision 2.5  2010/11/07 13:19:11  tuberkel
 * Test re-added - because of bug in CVS
 *
 *
 *
 ************************************************************************ */

#ifndef _DISPLAY_H
#define _DISPLAY_H


//general defines
#define UNDERLINE 0x80 //pattern to set lowest bitmap pixel part
#define THICKMAX  8    //max thickness of a drawed line


//display modes (8 bit coded)
                       //THESE LOW NIBBLE MODES CAN BE MIXED !
#define DPLNORM   0x00 //normal output: easily set new pixel
#define DPLINVERS 0x01 //invers output: set inverted new pixel
#define DPLUNDERL 0x02 //underline output: set new pixel AND set lowest pixel to underline
#define DPLMODE8  0x04 //tbd.
                       //THESE HIGH NIBBLE MODES CAN NOT BE MIXED !
#define DPLXOR    0x08 //read back display data and XOR  with new data
#define DPLOR     0x10 //read back display data and OR   with new data
#define DPLAND    0x20 //read back display data and AND  with new data
#define DPLNAND   0x40 //read back display data and NAND with new data
#define DPLNOR    0x80 //read back display data and NOR  with new data


//display fonts
typedef enum{
   DPLFONT_NOFONT,     //invalid font
   DPLFONT_4X6,        //only uppercase char without special chars
   DPLFONT_6X8,        //full featured font
   DPLFONT_14X16,      //numbers only
   DPLFONT_8X16,       //full featured font
   DPLFONT_24X32,      //numbers only
   DPLFONT_30X56       //not yet implemented - numbers only
}DPLFONT;


//Bitmap resource struct
typedef struct{
   UINT16 wWidth;      //width in pixels
   UINT16 wHeight;     //height in pixels
   unsigned char far *fpucBitmap; //address of bitmap raw data
}BITMAP;


//struct to contain x/y coordinates (0,0 is upper left corner)
typedef struct{
   UINT16 wXPos;       //horizontal grafic position in pixel
   UINT16 wYPos;       //vertical grafic position in pixel
}DISPLXY;


//display status flags (used to reconstruct display setting after power up)
//(variables with more than 1 bit must start on a boundary)
typedef union{
    UINT32 dword;
    struct{
        unsigned char ContrLev:6;       //lcd contrast level (valid 0..63)
        unsigned char res_a:1;          //currently unused bits
        unsigned char fDplInvers:1;     //lcd invers mode switch ON/OFF flag

        unsigned char BacklLev:6;       //lcd backlight level (valid 0..63)
        unsigned char res_b:2;          //currently unused bits

        unsigned char BacklOnLevel:3;   // lumination level for auto backlight turn on (0...7, 0=always off, 7=always on)
        unsigned char res_c:5;          // currently unused bits

        unsigned char res_d;            // currently unused bits
    }flags;
}DPLFLAGS_TYPE;

// default values to be used to initialize eeprom
#define DEF_DPL_CONTRAST        0x20L   // 6 bits contrast level, set to middle value
#define DEF_DPL_INVERS          0x0L    // 1 bit invers mode, set off
#define DEF_DPL_BACKLLEVEL      0x3fL   // 6 bits backlight level, set to maximum
#define DEF_DPL_BACKLONLEVEL    0x7L    // 3 bits backlight on-switch-level, set to 'always on'

#define DEF_DPLFLAGS ((UINT32)( DEF_DPL_CONTRAST | (DEF_DPL_INVERS<<7) | (DEF_DPL_BACKLLEVEL<<8) | (DEF_DPL_BACKLONLEVEL<<16)))
//#define DEF_DPLFLAGS (0xfc806000UL) //default: invers off, backl. off, backl.-level 63, contrast 32, BacklOnLevel 3


//exported function prototypes
ERRCODE DisplInit( BOOL fFlash );
void    DisplClearScreen( unsigned char ucPattern );
DISPLXY DisplGetDimension( DPLFONT eFont );
ERRCODE DisplPrintABitmap( const BITMAP far * pBitmap,
                           const DISPLXY far * pCoord,
                           unsigned char ucMode );
ERRCODE DisplPrintACharacter( const BITMAP far * pChar,
                              const DISPLXY far * pCoord,
                              unsigned char ucMode);
ERRCODE DisplPrintAString( const unsigned char far * szString,
                           const DISPLXY far * pCoord,
                           DPLFONT Font,
                           unsigned char ucMode );
ERRCODE DisplDrawHorLine( const DISPLXY far * pCoord,
                          unsigned short wLength,
                          unsigned char  bPattern,
                          unsigned char  ucMode );
ERRCODE DisplCheckLCDModule( void );

BOOL    DisplBacklightCheckOn (UINT8 BacklOnLevel);
void    DisplSetParms(void);


#endif //_DISPLAY_H
