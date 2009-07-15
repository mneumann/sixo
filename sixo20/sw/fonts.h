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
#define ASCIIFIRST      32  // first STANDARD     ascii char
#define ASCIILAST      126  // last  STANDARD     ascii char
#define FREEASCIIFIRST 127  // first FREE DEFINED ascii char
#define FREEASCIILAST  160  // last  FREE DEFINED ascii char
#define EXTASCIIFIRST  161  // first EXTRA        ascii char
#define SMALL_CROSS    164  // prepared '¤'
#define EXTASCIILAST   255  // last  EXTRA        ascii char


/* free defined ascii chars (area #127..#160)
   in DPLFONT_6X8 font only!*/
#define CHAR_BENT_ARROW_UP      127     /* UPDATE key icon (left side) 'BentArrowUp' */
#define CHAR_BENT_ARROW_DN      128     /* UPDATE key icon (right side) 'BentArrowDown */
#define CHAR_CROSS_L            129     /* ESC key icon (left side)  'CrossLeft' */
#define CHAR_CROSS_R            130     /* ESC key icon (right side) 'CrossRight' */
#define CHAR_HOOK_L             131     /* OK  key icon (left side)  'HookLeft'*/
#define CHAR_HOOK_R             132     /* OK  key icon (left side)  'HookRight'*/
#define CHAR_ENVELOPE_L         133     /* envelope icon (left side) 'EnvelopeLeft' */
#define CHAR_ENVELOPE_R         134     /* envelope icon (right side)'EnvelopeRight'*/
#define CHAR_WIDE_ARROW_UP_L    135     /* UP key icon (left side)   'WideArrowUpLeft'*/
#define CHAR_WIDE_ARROW_UP_R    136     /* UP key icon (right side)  'WideArrowUpRight' */
#define CHAR_WIDE_ARROW_DN_L    137     /* DOWN key icon (left side) 'WideArrowDownLeft' */
#define CHAR_WIDE_ARROW_DN_R    138     /* DOWN key icon (right side)'WideArrowDownRight' */

#define CHAR_FAT_ARROW_UP       139     /* scroll UP icon 'FatArrowUP' */
#define CHAR_FAT_ARROW_DOWN     140     /* scroll DOWN icon 'FatArrowDown' */
#define CHAR_FAT_ARROW_LEFT     141     /* scroll LEFT icon 'FatArrowLeft' */
#define CHAR_FAT_ARROW_RIGHT    142     /* scroll RIGHT icon 'FatArrowRight' */

#define CHAR_CELSIUSDEGREE      143     /* '°C' as one char */

#define CHAR_SLCTARROW_LEFT_L   144     /* '<=' select Arror LEFT icon (left half) 'SelectArrowLeft_L' */
#define CHAR_SLCTARROW_LEFT_R   145     /* '<=' select Arror LEFT icon (right half) 'SelectArrowLeft_R' */
#define CHAR_SLCTARROW_RIGHT_L  146     /* '=>'select Arror RIGHT icon (left half) 'SelectArrowRight_L' */
#define CHAR_SLCTARROW_RIGHT_R  147     /* '=>'select Arror RIGHT icon (right half) 'SelectArrowRight_R' */
#define CHAR_SLCTARROW_UP_L     148     /* '^'select Arror UP icon (left half) 'SelectArrowUp_L' */
#define CHAR_SLCTARROW_UP_R     149     /* '^'select Arror UP icon (right half) 'SelectArrowUp_R' */

#define CHAR_GREEK_OMEGA        152     /* greek 'Omega' as special char for resistance */
#define CHAR_HOOK_OK            153     /* 'Hook' as one char to indicate 'ok' or 'selected' */
#define CHAR_CENT               154     /* 'ct' 'Cent' as one char for european currency */
#define CHAR_EURO               155     /* '€' 'Eur' as one char for european currency */

#define CHAR_ATTENTION          156     /* fat '!' to indicate attention */
#define CHAR_ERROR              157     /* fat 'E' to indicate error */
#define CHAR_RECTANGLE          158     /* small rectangle for 'OK'-key symbol */


/* character bitmap header */
typedef struct
{
    unsigned char ucAscii;
    unsigned char ucWidth;
    unsigned char ucHeight;
} CHARINFO;


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
