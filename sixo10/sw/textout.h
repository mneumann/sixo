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


/* common defines */
#define TXT_FONT6X8_H   8                                   /* font height in pixel*/
#define TXT_FONT6X8_W   6                                   /* font width in pixel */
#define TXT_MAXLINES    Y_MAX_DISPL / TXT_FONT6X8_H         /* =7: max number of lines to display */
#define TXT_MAXCOLUMN   PAGE_COLUMN_MAX * 2 / TXT_FONT6X8_W /* =21: max number of characters per line */


/* standard textout function return values */
typedef enum 
{
    TXT_OK,
    TXT_PARAMERR
} TXTOUTRET;

/* function prototypes */ 
TXTOUTRET Txt_PrintChar(const unsigned char ucXPos, const unsigned char ucYPos, const unsigned char ucChar, const unsigned char ucStyle );
TXTOUTRET Txt_PrintText(const unsigned char ucXPos, const unsigned char ucYPos, const unsigned char far * szText, const unsigned char ucStyle );
TXTOUTRET Txt_ClearLine(const unsigned char ucYPos, const unsigned char ucStyle );
TXTOUTRET Txt_ClearDisplay(const unsigned char ucStyle );