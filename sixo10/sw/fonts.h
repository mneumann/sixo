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
 *  Created:        08-07-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         fonts
 *  Purpose:        fonts definitions
 *  Comments:       font orientation: 
 *                  every value is a pixel column of the character
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
#define CHAR_SQRBRCK_L  91      /* [ */
#define CHAR_SQRBRCK_R  93      /* ] */

/* defines for special characters */
#define ASCIIFIRST   32     /* first possible ascii character to display */
#define ASCIILAST   126     /* last possible ascii character to display */

#define EXTASCIIFIRST  161     /* first possible extended ascii character to display */
#define EXTASCIILAST   255     /* last possible extended ascii character to display */


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
