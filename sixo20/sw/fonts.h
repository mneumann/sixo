/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        08-07-2001 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         fonts
 *  Purpose:        fonts definitions api export header
 *  Comments:       font orientation:
 *                  every value is a pixel column of the character
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
 *  This software is copyright � 2001-2004 by N&K Development, Germany
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
 *      St�ckener Str. 115                  Hudeplan 37 A
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
 * Revision 3.1  2012/05/24 19:30:03  tuberkel
 * Moved BMP-Logos to 'logos.c'
 *
 * Revision 3.0  2010/11/07 12:49:48  tuberkel
 * V30 Preparations:
 * - new free defined characters (#127..#160)
 * - additional Fonst- Size #defines
 * - CVS History added
 *
 *
 ************************************************************************ */


#ifndef _FONTS_H
#define _FONTS_H


/* valid number chars */
#define CHAR_SPACE      32
#define CHAR_PLUS       43
#define CHAR_COMMA      44
#define CHAR_MINUS      45
#define CHAR_POINT      46
#define CHAR_ZERO       48
#define CHAR_NINE       57
#define CHAR_DBLPOINT   58
#define CHAR_CROSS      88
#define CHAR_SQRBRCK_L  91      /* [ */
#define CHAR_SQRBRCK_R  93      /* ] */


/* ascii character limits */
/* Note: Some fonts do not support free/extended ascii char range */
#define ASCII_STD_FIRST      32     // first STANDARD     ascii char
#define ASCII_STD_LAST      126     // last  STANDARD     ascii char
#define ASCII_FREE_FIRST    127     // first FREE DEFINED ascii char
#define ASCII_FREE_LAST     160     // last  FREE DEFINED ascii char
#define ASCII_EXT_FIRST     161     // first EXTENDED     ascii char
#define ASCII_EXT_LAST      255     // last  EXTENDED     ascii char


/* free defined ascii chars (area #127..#160)
   in DPLFONT_6X8 font only!*/
#define CHAR_RES_127            127
#define CHAR_RES_128            128
#define CHAR_RES_129            129
#define CHAR_RES_130            130
#define CHAR_HOOK_L             131     /* OK  key icon (left side)  'HookLeft'*/
#define CHAR_HOOK_R             132     /* OK  key icon (left side)  'HookRight'*/
#define CHAR_RES_133            133
#define CHAR_RES_134            134
#define CHAR_WIDE_ARROW_UP_L    135     /* UP key icon (left side)   'WideArrowUpLeft'*/
#define CHAR_WIDE_ARROW_UP_R    136     /* UP key icon (right side)  'WideArrowUpRight' */
#define CHAR_WIDE_ARROW_DN_L    137     /* DOWN key icon (left side) 'WideArrowDownLeft' */
#define CHAR_WIDE_ARROW_DN_R    138     /* DOWN key icon (right side)'WideArrowDownRight' */

#define CHAR_FAT_ARROW_UP       139     /* scroll UP icon 'FatArrowUP' */
#define CHAR_FAT_ARROW_DOWN     140     /* scroll DOWN icon 'FatArrowDown' */
#define CHAR_FAT_ARROW_LEFT     141     /* scroll LEFT icon 'FatArrowLeft' */
#define CHAR_FAT_ARROW_RIGHT    142     /* scroll RIGHT icon 'FatArrowRight' */

#define CHAR_CELSIUSDEGREE      143     /* '�C' as one char */

#define CHAR_SLCTARROW_LEFT_L   144     /* '<=' select Arror LEFT icon (left half) 'SelectArrowLeft_L' */
#define CHAR_SLCTARROW_LEFT_R   145     /* '<=' select Arror LEFT icon (right half) 'SelectArrowLeft_R' */
#define CHAR_SLCTARROW_RIGHT_L  146     /* '=>'select Arror RIGHT icon (left half) 'SelectArrowRight_L' */
#define CHAR_SLCTARROW_RIGHT_R  147     /* '=>'select Arror RIGHT icon (right half) 'SelectArrowRight_R' */
#define CHAR_SLCTARROW_UP_L     148     /* '^'select Arror UP icon (left half) 'SelectArrowUp_L' */
#define CHAR_SLCTARROW_UP_R     149     /* '^'select Arror UP icon (right half) 'SelectArrowUp_R' */

#define CHAR_GREEK_OMEGA        152     /* greek 'Omega' as special char for resistance */
#define CHAR_HOOK_OK            153     /* 'Hook' as one char to indicate 'ok' or 'selected' */
#define CHAR_CENT               154     /* 'ct' 'Cent' as one char for european currency */
#define CHAR_EURO               155     /* '�' 'Eur' as one char for european currency */

#define CHAR_ATTENTION          156     /* fat '!' to indicate attention */
#define CHAR_ERROR              157     /* fat 'E' to indicate error */
#define CHAR_RECTANGLE          158     /* small rectangle for 'OK'-key symbol */
#define CAHR_SUMSYMBOL          159     /* greek 'Sum' symbol */

/* character bitmap header */
typedef struct
{
    unsigned char ucAscii;
    unsigned char ucWidth;
    unsigned char ucHeight;
} CHARINFO;


/* some font specific helper macros */
#define DPLFONT_4X6_WIDTH       4
#define DPLFONT_4X6_HEIGHT      8   // !!! 8 instead of 6 to increase reability
#define DPLFONT_6X8_WIDTH       6
#define DPLFONT_6X8_HEIGHT      8
#define DPLFONT_8X16_WIDTH      8
#define DPLFONT_8X16_HEIGHT    16
#define DPLFONT_14X16_WIDTH    14
#define DPLFONT_14X16_HEIGHT   16
#define DPLFONT_24X32_WIDTH    24
#define DPLFONT_24X32_HEIGHT   32



/* exported functions */
unsigned char far * GetCharInfo(  const unsigned char ucChar,
                                  const DPLFONT Font,
                                  BITMAP far * pChar);

unsigned char far * GetNumberCharInfo(  const unsigned char ucChar,
                                        const DPLFONT Font,
                                        BITMAP far * pChar);

UINT8 GetFontHeight(const DPLFONT Font);
UINT8 GetFontWidth(const DPLFONT Font);


#endif /* _FONTS_H */
