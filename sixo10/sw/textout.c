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
 *  Created:        21-07-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         text out
 *  Purpose:        function to print standard text (6x8 pixel)
 *                  to display line/column in page mode
 *  Comments:       -
 *
 ************************************************************************ */

#include <string.h>
#include "standard.h"
#include "display.h"
#include "fonts.h"
#include "textout.h"
#include "displdrv.h"
#include "display.h"

extern unsigned char far rgFont6x8[];

/***********************************************************************
 * FUNCTION:    Txt_PrintChar
 * DESCRIPTION: print out one character to display
 * PARAMETER:   ucXPos  x position in text array (0,0 is upper left)
 *              ucYPos  y position in text array (0,0 is upper left)
 *              ucChar  ascii code of character to display
 *              ucStyle binary code style to display character (invers etc.)
 * RETURN:      One of TXTOUTRET
 * COMMENT:     the character to display is taken out of binary array
 *              rgFont6x8[] of fonts.c, special character positions in 
 *              array are calculated. 
 *              - character 0..31 are not used
 *              - character 32..127 is ascii compatible
 *              - character 128..255 is taken from windows standard
 *********************************************************************** */
TXTOUTRET Txt_PrintChar( const unsigned char ucXPos, 
                         const unsigned char ucYPos, 
                         const unsigned char ucChar, 
                         const unsigned char ucStyle )
{ 
    unsigned char ucCharIndex = 0;      /* index of character in font array */
    BITMAP        bmpChar;              /* character bitmap dimension and raw data pointer */
    DISPLXY       Pos;                  /* position in pixel coordinates */
    
    /* check parameters first: */
    if (  (ucXPos >= TXT_MAXCOLUMN)                             /* x out of display */
        ||(ucYPos >= TXT_MAXLINES )                             /* y out of display */
        ||(ucChar <  ASCIIFIRST   )                             /* invalid control character */
        ||((ucChar > ASCIILAST) && (ucChar < EXTASCIIFIRST))   /* invalid character */
        ||(ucChar > EXTASCIILAST) )
        return TXT_PARAMERR;
    
    /* calculate character position in font array */
    /* (font array has a gap of undefined chars   */
    /*  between 0..31 and 128..160 )              */
    if (ucChar <= ASCIILAST )
        /* character is in ascii# 32..126 */
        ucCharIndex = ucChar - ASCIIFIRST;                    
    else 
        /* character is in ascii# 161..255 */
        ucCharIndex = ucChar - EXTASCIIFIRST + ASCIILAST - ASCIIFIRST + 1 ;

    /* set raw data pointer to first character pixel column */
    bmpChar.fpucBitmap = &(rgFont6x8[ucCharIndex * TXT_FONT6X8_W]);
    bmpChar.wWidth    = TXT_FONT6X8_W;
    bmpChar.wHeight   = TXT_FONT6X8_H;
    
    /* calculate pixel position of character */
    Pos.wXPos = ucXPos * TXT_FONT6X8_W;
    Pos.wYPos = ucYPos * TXT_FONT6X8_H;
    
    /* give bitmap of character to display */
    DisplPrintACharacter( &bmpChar, &Pos, ucStyle );
    
    return TXT_OK;
}


/***********************************************************************
 * FUNCTION:    Txt_PrintText
 * DESCRIPTION: print out a string to display 
 * PARAMETER:   ucXPos  x position in text array (0,0 is upper left)
 *              ucYPos  y position in text array (0,0 is upper left)
 *              szText  pointer to string to display 
 *              ucStyle binary code style to display character (invers etc.)
 * RETURN:      One of TXTOUTRET
 * COMMENT:     - no way to use /n new line operator! string can only be 
 *                printed into one line
 *********************************************************************** */
TXTOUTRET Txt_PrintText(    const unsigned char ucXPos, 
                            const unsigned char ucYPos, 
                            const unsigned char far * szText, 
                            const unsigned char ucStyle )
{ 
    unsigned char ucChar;                   /* copy of one character of string */ 
    unsigned char ucLength;                 /* string length */
    unsigned char ucXOffset;                /* string char loop */
        
    /* check parameters first */
    if (  (ucXPos  >= TXT_MAXCOLUMN)
        ||(ucYPos  >= TXT_MAXLINES )
        ||(szText  == NULL         )        /* null pointer? */
        ||(*szText == 0x0          ) )      /* empty string? */
        return TXT_PARAMERR;

    /* check string length */
    ucLength = (unsigned char) strlen(szText);
    if (ucLength > DisplGetDimension(DPLFONT_NOFONT).wXPos)
        return TXT_PARAMERR;
    
    /* scan complete string */        
    for (ucXOffset = 0; ucXOffset < ucLength; ucXOffset++)
    {
        /* get and print one character */
        ucChar = *(szText+ucXOffset);
        Txt_PrintChar( ucXPos + ucXOffset, ucYPos, ucChar, ucStyle );
    }
    return TXT_OK;
}
    
/***********************************************************************
 * FUNCTION:    Txt_
 * DESCRIPTION: -
 * PARAMETER:   -
 * RETURN:      One of TXTOUTRET
 * COMMENT:     -
 *********************************************************************** */
TXTOUTRET Txt_ClearLine(const unsigned char ucYPos, const unsigned char ucStyle )
{ 
    return TXT_OK;
}


/***********************************************************************
 * FUNCTION:    Txt_
 * DESCRIPTION: -
 * PARAMETER:   -
 * RETURN:      One of TXTOUTRET
 * COMMENT:     -
 *********************************************************************** */
TXTOUTRET Txt_ClearDisplay(const unsigned char ucStyle )
{ 
    return TXT_OK;
}
