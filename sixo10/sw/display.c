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
 *  Created:        16-02-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         lcd high level driver
 *  Purpose:        display service functions
 *  Comments:       -
 *  Changes:        extra Parameter in call to LCDDrvGetPixel added, AN 16.2.2003
 ************************************************************************ */


#include <string.h>
#include "standard.h"
#include "displdrv.h"
#include "display.h"
#include "fonts.h"
#include "textout.h"
#include "debug.h"

/* font arrays */
extern unsigned char far rgFont4x6[];
extern unsigned char far rgFont6x8[];
extern unsigned char far rgFont8x16[];
extern unsigned char far rgNumber14x16[];
extern unsigned char far rgNumber24x32[];

/* global status */
DPLFLAGS_TYPE   gDisplayFlags = DEF_DPLFLAGS;


/***********************************************************************
 *  FUNCTION:       DisplInit
 *  DESCRIPTION:    initialize ports & clear screen
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        uses global display flags initialized by
 *                  ParInitSystemPar()
 *********************************************************************** */
ERRCODE DisplInit(void)
{
    ERRCODE RValue = ERR_OK;
    RValue = LCDDrvInitLCD();
    if (RValue == ERR_OK)
    {
        RValue = LCDDrvSetContrastLevel(gDisplayFlags.flags.ContrLev);  /* set contrast level */
        RValue = LCDDrvSetBacklightLevel(gDisplayFlags.flags.BacklLev); /* set EL backlight brigthness */
        RValue = LCDDrvSetBacklight(gDisplayFlags.flags.fBackl);        /* activate EL backlight */
        DisplClearScreen(0x0);
        ODS(DBG_DRV,DBG_INFO,"DisplInit() done!");
    }
    else
    {
        ODS(DBG_DRV,DBG_ERROR,"DisplInit() ERROR! LCD may be disconnected or damaged!");
    }
    return RValue;
}



/***********************************************************************
 * FUNCTION:    DisplClearScreen
 * DESCRIPTION: Clears the the screen with given pattern
 * PARAMETER:   unsigned char ucPattern as 8 bit vertical data (page height)
 * RETURN:      -
 * COMMENT:     -
 *********************************************************************** */
void DisplClearScreen(const unsigned char ucPattern)
{
    LCDDrvSetSide(LEFT);
    LCDDrvFillPattern(ucPattern);
    LCDDrvSetSide(RIGHT);
    LCDDrvFillPattern(ucPattern);
}


/***********************************************************************
 *  FUNCTION:       DisplGetDimension
 *  DESCRIPTION:    returns the display capabilities in dependency
 *                  of given units (pixel/chars)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
DISPLXY DisplGetDimension(const DPLFONT eFont)
{
    DISPLXY Dim;

    switch (eFont)
    {
        case DPLFONT_NOFONT:
            Dim.wXPos = X_MAX_DISPL;
            Dim.wYPos = Y_MAX_DISPL;
            break;
        case DPLFONT_4X6:
            Dim.wXPos = X_MAX_DISPL / 4;
            Dim.wYPos = Y_MAX_DISPL / 6;
            break;
        case DPLFONT_6X8:
            Dim.wXPos = X_MAX_DISPL / 6;
            Dim.wYPos = Y_MAX_DISPL / 8;
            break;
        case DPLFONT_8X16:
            Dim.wXPos = X_MAX_DISPL / 8;
            Dim.wYPos = Y_MAX_DISPL / 16;
            break;
        case DPLFONT_14X16:
            Dim.wXPos = X_MAX_DISPL / 14;
            Dim.wYPos = Y_MAX_DISPL / 16;
            break;
        case DPLFONT_24X32:
            Dim.wXPos = X_MAX_DISPL / 24;
            Dim.wYPos = Y_MAX_DISPL / 32;
            break;
        case DPLFONT_30X56:
            Dim.wXPos = X_MAX_DISPL / 30;
            Dim.wYPos = Y_MAX_DISPL / 56;
            break;
        default: Dim.wXPos = 0; Dim.wYPos = 0;
    }
    return Dim;
}


/***********************************************************************
 * FUNCTION:    DisplPrintABitmap
 * DESCRIPTION: general purpose function to bring every bitmap to display
 * PARAMETER:   BITMAP far * pBitmap    pointer to bitmap info
 *              DISPLXY far * pCoord    pointer to y/x coordinates
 *              unsigned char ucMode    display mode (bit coded, f.e. invers)
 * RETURN:      ERRCODE
 * COMMENT:     BMPs must be at least 8 Bits high!
  *********************************************************************** */
ERRCODE DisplPrintABitmap(  const BITMAP far * pBitmap,
                            const DISPLXY far * pCoord,
                            const unsigned char ucMode )
{
    unsigned char far * fpucData = pBitmap->fpucBitmap; /* pointer TO bmp source data (1 pixel column) */
    unsigned char   ucBMPPixel;                         /* pointer IN bmp source data (1 pixel column) */
    BOOL            bSide;              /* to handle left/right controller sides */
    unsigned char   ucPage;             /* current page (display specific) */
    unsigned char   ucPageOld;          /* past page (display specific) */
    unsigned char   ucPageStart;        /* first upper page (display specific) */
    unsigned char   ucPageEnd;          /* last lower page (display specific) */
    unsigned int    uiXCol;             /* current column (display specific) */
    unsigned int    uiXColStart;        /* first left column (display specific) */
    unsigned int    uiXColEnd;          /* last right column (display specific) */
    unsigned int    uiXColCtrl;         /* column (controller side specific) */
    unsigned char   ucOffsetPart1;      /* =0..7, for vertical bitshift operation, upper part */
    unsigned char   ucOffsetPart2;      /* =0..7, for vertical bitshift operation, lower part */
    unsigned char   ucPixelNew;         /* new output data (1 pixel column) */
    unsigned char   ucPixelOld;         /* old display data (1 pixel column) */
    unsigned char   ucMaskUnEffect;     /* mask to prevent affecting old display data at bmp offsets (1 pixel column) */
                                        /* '1' means to protect that bit */

    /* check parameters first */
    if (  (pBitmap == NULL                )     /* no bmp? */
        ||(pBitmap->fpucBitmap == NULL    )     /* no bmp address? */
        ||(pCoord->wYPos >= Y_MAX_DISPL  )     /* start y pos outside display? */
        ||(pCoord->wXPos >= X_MAX_DISPL  )     /* start y pos outside display? */
        ||(pBitmap->wHeight < PAGE_HEIGHT) )   /* bmp height to small?*/
        return ERR_PARAM_ERR;

    /* init page start & end (0..6) */
    ucPageStart = pCoord->wYPos / PAGE_HEIGHT;
    ucPageEnd   = (pCoord->wYPos + pBitmap->wHeight - 1) / PAGE_HEIGHT;

    /* offset for vertical bit shift operations */
    ucOffsetPart1 = (pCoord->wYPos + PAGE_HEIGHT) % PAGE_HEIGHT;   /* valid: 0..7 */
    ucOffsetPart2 = PAGE_HEIGHT - ucOffsetPart1;                    /* valid: 7..0 */

    /* init xColumn (start, end, act.) */
    uiXColStart = pCoord->wXPos;
    uiXColEnd   = uiXColStart + pBitmap->wWidth - 1;
    if (uiXColStart < PAGE_COLUMN_MAX)                      /* on which display half do we start? */
    {
        bSide     = LEFT;                                           /* we start on LEFT side */
        uiXColCtrl = uiXColStart;                                   /* set X column pixel on side */
    }
    else
    {
        bSide     = RIGHT;                                          /* we start on RIGHT side */
        uiXColCtrl = uiXColStart - PAGE_COLUMN_MAX;                 /* set X column pixel on side */
    }
    /* complete bitmap init of display controller */
    LCDDrvSetSide(bSide);                                  /* activate L/R display controller */
    LCDDrvSetPage(ucPageStart);                            /* set Y page (0..6) */
    ucPageOld = ucPageStart;                                /* save page */
    LCDDrvSetColumn(uiXColCtrl);                           /* set X column for read/write */

    /* --------------------------- */
	/* span complete bitmap height */
	for ( ucPage = ucPageStart; ucPage <= ucPageEnd; ucPage++ )
	{
		/* span complete bitmap width */
        /* NOTE:    The display controller handles an 'column auto increment', so that
                    we only need to 'set column' after controller/page switching, but
                    NOT after a simple read/write! */
		for (uiXCol = uiXColStart; uiXCol <= uiXColEnd; uiXCol++)
		{
            /* ----------------------------------------- */
            /* set current controller side, if necessary */
        	if (uiXCol < PAGE_COLUMN_MAX)   /* on which display half are we? */
        	{
                /* we are on LEFT side */
                uiXColCtrl = uiXCol;            /* X column pixel on side */
                if (bSide != LEFT)              /* We need LEFT side: is he alrady on LEFT side? */
                {
                    LCDDrvSetSide(LEFT);               /* activate L/R display controller */
    	            bSide     = LEFT;                   /* we are on left side */
                    LCDDrvSetPage(ucPage);             /* set Y page (0..6) */
                    ucPageOld = ucPage;                 /* save page */
                    LCDDrvSetColumn(uiXColCtrl);       /* set X column for read/write */
                }
        	}
        	else
        	{
                /* we are on RIGHT side */
                uiXColCtrl = uiXCol - PAGE_COLUMN_MAX;  /* X column pixel on L/R side */
                if (bSide != RIGHT)             /* We need RIGHT side: is he alrady on RIGHT side? */
                {
                    LCDDrvSetSide(RIGHT);              /* activate L/R display controller */
    	            bSide     = RIGHT;                      /* we are on right side */
                    LCDDrvSetPage(ucPage);             /* set Y page (0..6) */
                    ucPageOld = ucPage;                 /* save page */
                    LCDDrvSetColumn(uiXColCtrl);       /* set X column for read/write */
                }
            }

            /* reset column, if only page switch without side switch was done */
            if (ucPage != ucPageOld)
            {
                LCDDrvSetPage(ucPage);                         /* set page on current controller */
                ucPageOld = ucPage;                             /* save page */
                LCDDrvSetColumn(uiXColCtrl);                   /* set X column for read/write */
           	}

            /* =======================================================*/
            /* Is current bitmap column complete out of display area? */
            /* =======================================================*/
            if (  (uiXColCtrl < PAGE_COLUMN_MAX)        /* at right border left/right controller area? */
                &&(ucPage     < PAGE_MAX        ) )     /* or at lower display border? */
            {
                /* --------------------------------------- */
                /* read old display data only if necessary */
                ucPixelOld = 0x0;                       /* default: don't effect new data with old data */
                if (  (ucMode & (DPLXOR | DPLOR | DPLAND | DPLNAND | DPLNOR) )
                    ||(ucOffsetPart1 > 0                                     ) )
                {
                    /* get old display data */
                    LCDDrvGetPixel(&ucPixelOld, DUMMY_ON);/* get old lcd data (lcd addr. is incremented!) */
                    LCDDrvSetColumn(uiXColCtrl);          /* Re-Set X column for later write access */
                }

                /* ------------------------------------------------------------- */
                /* prepare mask to protect old data if any bmp offset operations */
                ucMaskUnEffect = 0x0;                   /* default: don't protect old display data */
                if (ucOffsetPart1 > 0)
                {
                    if (ucPage == ucPageStart)
                    {
                        /* clear upper part bits of mask to protect old display data */
                        ucMaskUnEffect = (((UINT8)0xff) << ucOffsetPart2) >> ucOffsetPart2;
                    }
                    if (ucPage == ucPageEnd)
                    {
                        /* clear lower part bits of mask to protect old display data */
                        ucMaskUnEffect = (((UINT8)0xff) >> ucOffsetPart1) << ucOffsetPart1;
                    }
                }

                /* --------------------------------*/
                /* get and prepare new bitmap data */
                ucPixelNew = 0x0;                       /* reset pixel data */
                if (ucOffsetPart1 > 0)                  /* vertical bitshift if necessary */
                {
                    if (ucPage > ucPageStart)           /* lower byte part if necessary (not in first page!) */
                    {
                        ucBMPPixel = *(fpucData - pBitmap->wWidth);    /* read bitmap data of page above */
                		if (ucMode & DPLINVERS)
                            ucBMPPixel = ~ucBMPPixel;                   /* first: invers bitmap */
                        if (  (ucMode & DPLUNDERL)
                            &&(ucPage == ucPageEnd) )                   /* only if lowest page */
                            ucBMPPixel |= UNDERLINE;                    /* second: set underline */
                        ucPixelNew = ucBMPPixel >> ucOffsetPart2;       /* shift to get relevant bits of actual page */

                    }
                    if (ucPage < ucPageEnd)             /* upper byte part if necessary (not in last page!) */
                    {
                        ucBMPPixel = *fpucData;                         /* read bitmap data */
                		if (ucMode & DPLINVERS)
                            ucBMPPixel = ~ucBMPPixel;                   /* first: invers bitmap */
                        //if (ucMode & DPLUNDERL)                       /* NO UNDERLINE IN UPPER PARTS! */
                        //    ucBMPPixel |= UNDERLINE;                  /* second: set underline */
                        ucPixelNew |= ucBMPPixel << ucOffsetPart1;      /* shift to get relevant bits of page above */
                    }
                }
                else
                {
                    ucPixelNew = *fpucData;             /* no page offsets: read normal bitmap data */
            		if (ucMode & DPLINVERS)
                        ucPixelNew = ~ucPixelNew;       /* first invers bitmap */
                    if (  (ucMode & DPLUNDERL)
                        &&(ucPage == ucPageEnd) )       /* only if lowest page */
                        ucPixelNew |= UNDERLINE;        /* second: set underline */
                }

                /* ------------------------------------- */
                /* protect old display data if necessary */
                if ( ucMaskUnEffect > 0 )
                {
                    ucPixelNew |= ucPixelOld & ucMaskUnEffect;      /* copy only protected old data into new data */
                    ucPixelOld = ucPixelOld & (~ucMaskUnEffect);    /* keep rest of old data to manipulate with ucMode */
                }

                /* ---------------------------- */
                /* combine new & old pixel data */
                if (  (ucMode & (DPLXOR | DPLOR | DPLAND | DPLNAND | DPLNOR ) )
                    ||(ucOffsetPart1 > 0                                     ) )
                {
                    /* only use bit mask 0xf8 for this case instr. */
                    switch (ucMode & ~(DPLINVERS | DPLUNDERL | DPLMODE8))
        		{
                    case DPLXOR:    ucPixelNew ^= ucPixelOld; break;
                    case DPLOR:     ucPixelNew |= ucPixelOld; break;
                    case DPLAND:    ucPixelNew &= ucPixelOld; break;
                    case DPLNAND:   ucPixelNew &= ~ucPixelOld; break;
                    case DPLNOR:    ucPixelNew |= ~ucPixelOld; break;
                    default: break;
        		}
                }

                /* -------------------------- */
        		/* copy raw data into display */
                LCDDrvSetPixel ( ucPixelNew );         /* write new pixel data byte */
                                                        /* NOTE that display controller auto-increments
                                                           the column after write acccess! */
            }
            fpucData++;                                /* next bitmap raw data (pixel column) */
            /* =============================================================================== */
        }
    }
    return ERR_OK;
}


/***********************************************************************
 * FUNCTION:    DisplPrintACharacter
 * DESCRIPTION: special function to print out a 6x8 character in a page line
 * PARAMETER:   BITMAP far * pChar      pointer to the character bitmap
 *              DISPLXY far * pCoord    pointer to display x,y coordinates
 *              unsigned char ucMode    display mode (bit coded, f.e. invers)
 * RETURN:
 * COMMENT:     !!! ONLY TO BE USED WITH STANDARD 6x8 FONT IN PAGE MODE !!!
 *
 *********************************************************************** */
ERRCODE DisplPrintACharacter( const BITMAP far * pChar,
                            const DISPLXY far * pCoord,
                            const unsigned char ucMode )
{
	unsigned char far * fpucData    = pChar->fpucBitmap;    /* pointer to raw data */
	unsigned char       ucPage      = 0;                    /* Y page 0..6 */
	unsigned int        uiXCol      = 0;                    /* X column 0..128 */
	unsigned char       ucXColDpl   = 0;                    /* X column 0..63 of controller */

    /* check parameters first */
    if (  (pChar == NULL)
        ||(pChar->fpucBitmap == NULL)
        ||(pCoord->wYPos >= DisplGetDimension(DPLFONT_NOFONT).wYPos)
        ||(pCoord->wXPos >= DisplGetDimension(DPLFONT_NOFONT).wXPos) )
        return ERR_PARAM_ERR;

    /* get the right page (0..6) */
    ucPage = pCoord->wYPos / PAGE_HEIGHT;

    /* initialize left/right controller */
    if (pCoord->wXPos < PAGE_COLUMN_MAX)
	{
	    LCDDrvSetSide(LEFT);
		LCDDrvSetPage(ucPage);
        ucXColDpl = (unsigned char) pCoord->wXPos;
        LCDDrvSetColumn(ucXColDpl);
	}
	else
	{
	    LCDDrvSetSide(RIGHT);
		LCDDrvSetPage(ucPage);
        ucXColDpl = ((unsigned char) pCoord->wXPos) - PAGE_COLUMN_MAX;
        LCDDrvSetColumn(ucXColDpl);
	};

	/* span complete character bitmap width */
	for (uiXCol = pCoord->wXPos; uiXCol < (pCoord->wXPos + pChar->wWidth); uiXCol++)
	{
		/* change to right display controller only if necessary */
    	if (uiXCol == PAGE_COLUMN_MAX)
    	{
    	    LCDDrvSetSide(RIGHT);
    		LCDDrvSetPage(ucPage);
	        LCDDrvSetColumn(0);
    	}
		/* copy raw data into display */
		if (ucMode == DPLNORM)
		    LCDDrvSetPixel ( *fpucData++ );    /* normal display */
		if (ucMode & DPLINVERS)
		    LCDDrvSetPixel ( ~(*fpucData++) ); /* invers character */
	}
	return ERR_OK;
}

/***********************************************************************
 * FUNCTION:    DisplPrintAString
 * DESCRIPTION: general purpose function to bring a string to display,
 *              by selecting the right characters from a font and give
 *              them as a bitmap to display
 * PARAMETER:   unsigned char far * szString    pointer to a formated string
 *              DISPLXY far * pCoord            pointer to x,y coordinates
 *              DPLFONT Font                    font define
 *              unsigned char ucMode            display mode (bit coded, f.e. invers)
 * RETURN:      error code
 * COMMENT:     > only complete pages height can be written!
 *              > Use the TXT_PrintText() instead, if you wish to use
 *                a standard 6x8 pixel character line output
 *********************************************************************** */
ERRCODE DisplPrintAString(     unsigned char far *         szString,
                            const DISPLXY far *         pCoord,
                            const DPLFONT               Font,
                            const unsigned char         ucMode )
{
    BITMAP Bitmap;                  /* character bitmap */
    DISPLXY Coord;                  /* coordinates for each character */
    unsigned char far * pucChar;    /* one char of szString */
    unsigned char far * RValue;     /* return value */

    /* check parameters first */
    if (  (szString == NULL)
        ||(strlen(szString) == 0)
        ||(pCoord->wYPos >= DisplGetDimension(DPLFONT_NOFONT).wYPos)
        ||(pCoord->wXPos >= DisplGetDimension(DPLFONT_NOFONT).wXPos) )
        return ERR_PARAM_ERR;

    /* init stuff */
    Coord.wXPos = pCoord->wXPos;
    Coord.wYPos = pCoord->wYPos;
    pucChar = szString;

    /* loop over string length */
    while (*pucChar != 0)
    {
        /* select the font */
        RValue = NULL;
        switch (Font)
        {
            case DPLFONT_4X6:        /* full featured font */
            case DPLFONT_6X8:        /* full featured font */
            case DPLFONT_8X16:       /* full featured font */
                {
                    RValue = GetCharInfo(*pucChar, Font, &Bitmap);
                }
                break;
            case DPLFONT_14X16:      /* numbers only */
            case DPLFONT_24X32:      /* numbers only */
            case DPLFONT_30X56:      /* numbers only */
                {
                    RValue = GetNumberCharInfo(*pucChar, Font, &Bitmap);
                };
                break;
            default:
                return ERR_PARAM_ERR;
        } /* of switch */

        /* character in font array? */
        if (RValue != NULL)
        {
            /* display this one character */
            DisplPrintABitmap( &Bitmap, &Coord, ucMode );
            /* coordinates for next character */
            Coord.wXPos += Bitmap.wWidth;
        }
        /* next character */
        pucChar++;

    }   /* of while */
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       DisplDrawHorLine
 *  DESCRIPTION:    prints a horizontal line with a defined thickness
 *                  to the screen
 *  PARAMETER:      DISPLXY far *   pCoord      x,y start coordinates
 *                  UINT16          wLength     length of line in pixels
 *                  UINT8           bPattern    line style pattern
 *  RETURN:         error code
 *  COMMENT:        Function uses a 8 vertical pixel with a bitcoded bPattern
 *                  to be drawn 'wLength'-times at a line:
 *                      - to Draw a single pixel line use 0x01
 *                      - to Draw a 8 pixel line use 0xff
 *                  Function uses XOR mode to draw line
 *
 *      t.b.d.:     This function could be faster, if we use a temporary
 *                  memory to be filled with 'bPattern' and then give it
 *                  as one(!) BMP to draw, instead of n BMPs.
 *
 *********************************************************************** */
ERRCODE DisplDrawHorLine(const DISPLXY far * pCoord, const UINT16 wLength, const UINT8 bPattern)
{
    ERRCODE RValue = ERR_OK;
    BITMAP  BMPLine;
    DISPLXY Coord;
    UINT16  wColumn;

    /* check parameters first */
    if (  (pCoord->wYPos >= DisplGetDimension(DPLFONT_NOFONT).wYPos)
        ||(pCoord->wXPos >= DisplGetDimension(DPLFONT_NOFONT).wXPos)
        ||(bPattern == 0 )
        ||(wLength  == 0 ) )
        return ERR_PARAM_ERR;

    /* prepare bitmap */
    Coord = *pCoord;        /* get a copy to work with */
    BMPLine.wWidth  = 1;    /* single column pattern */
    BMPLine.wHeight = 8;    /* 8 bit vertical pattern */
    BMPLine.fpucBitmap = (UINT8 far *) &bPattern;

    /* loop to draw line length */
    for (wColumn = 0; wColumn < wLength; wColumn++)
    {
        RValue = DisplPrintABitmap( &BMPLine, &Coord, DPLXOR );
        if (RValue != ERR_OK)
            break;
        Coord.wXPos++;
    }
    return RValue;
}

