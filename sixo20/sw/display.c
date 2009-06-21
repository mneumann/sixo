/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-02-16 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         lcd high level driver
 *  Purpose:        display service functions
 *  Comments:       -
 *  Changes:        Extra Parameter in call to LCDDrvGetPixel added, AN 16.2.2003
 *                  Changes for new display/hardware of SIxO 2.0 (see
 *                  comments preceeded by **AN), AN 27.12.2003
 *                  LCDDrvSetContrastLevel re-enabled in DisplInit. LCDDrv
 *                  SetContrast removed since There's no way to disable the
 *                  contrast voltage supply anymore.
 *                  LCDDrvSetBacklight re-enabled in DisplInit. LCDDrvSet-
 *                  BacklightLevel removed (behaviour changed with new SIxO
 *                  2.0 hardware) AN 12.02.2004
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


#include <stdlib.h>
#include <string.h>
#include "standard.h"
#include "displdrv.h"
#include "display.h"
#include "fonts.h"
#include "debug.h"
#include "timer.h"
#include "anain.h"


/* font arrays */
extern unsigned char far rgFont4x6[];
extern unsigned char far rgFont6x8[];
extern unsigned char far rgFont8x16[];
extern unsigned char far rgNumber14x16[];
extern unsigned char far rgNumber24x32[];

/* external symbols */
extern UINT16 wMilliSecCounter;        // for cyclic LCD module checks

/* global status */
DPLFLAGS_TYPE   gDisplayFlags = DEF_DPLFLAGS;


/***********************************************************************
 *  FUNCTION:       DisplInit
 *  DESCRIPTION:    initialize ports & clear screen
 *  PARAMETER:      BOOL fFlash     if TRUE, inverses Screen 1/2 second
 *  RETURN:         -
 *  COMMENT:        uses global display flags initialized by
 *                  ParInitSystemPar()
 *********************************************************************** */
ERRCODE DisplInit( BOOL fFlash )
{
    ERRCODE RValue;

    RValue = LCDDrvInitLCD();
    if( RValue == ERR_OK )
    {
      DisplSetParms();
      ODS(DBG_DRV,DBG_INFO,"DisplInit() done!");
    }
    else
    {
      ODS(DBG_DRV,DBG_ERROR,"DisplInit() ERROR! LCD may be disconnected or damaged!");
    }

    /* 'test flash' to display init success? */
    if (fFlash == TRUE)
    {
        DisplClearScreen( 0xff );   // dark display
        Delay_ms(500);              // wait
        DisplClearScreen( 0x00 );   // empty display
    }

    return RValue;
}


/***********************************************************************
 *  FUNCTION:       DisplCheckLCDModule
 *  DESCRIPTION:    Checks, wether LCD module is present and ok!
 *                  If LCD driver returns error, we will try to reset
 *                  the LCD module too!
 *  PARAMETER:      -
 *  RETURN:         ERR_OK              if LCD module works well
 *                  ERR_NOT_SUPPORTED   if any error
 *  COMMENT:        If any error detected, this will result in no display
 *                  output, until the display is detected to work well.
 *********************************************************************** */
ERRCODE DisplCheckLCDModule( void )
{
    ERRCODE RValue;

    if (  ( LCDDrvCheckController ( LEFT )  == ERR_NOT_SUPPORTED )
        ||( LCDDrvCheckController ( RIGHT ) == ERR_NOT_SUPPORTED ) )
    {
        // we have a problem, try to reset
        RValue = LCDDrvInitLCD();
        if ( RValue == ERR_NOT_SUPPORTED )
        {
            ODS(DBG_DRV,DBG_ERROR,"DisplCheckLCDModule() LCD still disconnected or damaged!");
        }
        else
        {
            ODS(DBG_DRV,DBG_ERROR,"DisplCheckLCDModule() LCD now working again!");
        }
    }
    else
    {
        RValue = ERR_OK;
    }
    return RValue;
}




/***********************************************************************
 * FUNCTION:    DisplClearScreen
 * DESCRIPTION: Clears the screen with given pattern
 * PARAMETER:   unsigned char ucPattern as 8 bit vertical data (page height)
 * RETURN:      -
 * COMMENT:     We use this opportunity for a check wether LCD module is
 *              working well and if not, trying to re-initialize
 *********************************************************************** */
void DisplClearScreen( unsigned char ucPattern )
{
    // first check LCD module
    if ( DisplCheckLCDModule() == ERR_OK )
    {
        LCDDrvSetSide( LEFT );
        LCDDrvFillPattern( ucPattern );
        LCDDrvSetSide( RIGHT );
        LCDDrvFillPattern( ucPattern );
    }
}


/***********************************************************************
 *  FUNCTION:       DisplGetDimension
 *  DESCRIPTION:    returns the display capabilities in dependency
 *                  of given units (pixel/chars)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
DISPLXY DisplGetDimension( DPLFONT eFont )
{
   DISPLXY Dim;

   switch( eFont ){
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
       default:
          Dim.wXPos = 0; Dim.wYPos = 0;
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
ERRCODE DisplPrintABitmap(  const BITMAP far *pBitmap,
                            const DISPLXY far *pCoord,
                            unsigned char ucMode )
{
   const unsigned char far *fpucData = pBitmap->fpucBitmap; //pointer to bmp source data (1 pixel column)
   BOOL          bSide;         //to handle left/right controller sides
   unsigned char ucPage;        //current page (display specific)
   unsigned char ucPageOld;     //past page (display specific)
   unsigned char ucPageStart;   //first upper page (display specific)
   unsigned char ucPageEnd;     //last lower page (display specific)
   unsigned int  uiXCol;        //current column (display specific)
   unsigned int  uiXColStart;   //first left column (display specific)
   unsigned int  uiXColEnd;     //last right column (display specific)
   unsigned int  uiXColCtrl;    //column (controller side specific)
   unsigned char ucOffsetPart1; //=0..7, for vertical bitshift operation, upper part
   unsigned char ucOffsetPart2; //=0..7, for vertical bitshift operation, lower part

   //check parameters first
   if(  (pBitmap == NULL)                   //no bmp?
     || (pBitmap->fpucBitmap == NULL)       //no bmp address?
     || (pCoord->wYPos >= Y_MAX_DISPL)      //start y pos outside display?
     || (pCoord->wXPos >= X_MAX_DISPL)      //start y pos outside display?
     || (pBitmap->wHeight < PAGE_HEIGHT) ){ //bmp height to small?
      return ERR_PARAM_ERR;
   }

   //init page start & end (0..6)
   ucPageStart = pCoord->wYPos / PAGE_HEIGHT;
   ucPageEnd   = (pCoord->wYPos + pBitmap->wHeight - 1) / PAGE_HEIGHT;

   //offset for vertical bit shift operations
   ucOffsetPart1 = (pCoord->wYPos + PAGE_HEIGHT) % PAGE_HEIGHT; //valid: 0..7
   ucOffsetPart2 = PAGE_HEIGHT - ucOffsetPart1;                 //valid: 7..0

   //init xColumn (start, end, act.)
   uiXColStart = pCoord->wXPos;
   uiXColEnd   = uiXColStart + pBitmap->wWidth - 1;
   if( uiXColStart < PAGE_COLUMN_MAX ){           //on which display half do we start?
      bSide     = LEFT;                           //we start on LEFT side
      uiXColCtrl = uiXColStart;                   //set X column pixel on side
   }
   else{
      bSide     = RIGHT;                          //we start on RIGHT side
      uiXColCtrl = uiXColStart - PAGE_COLUMN_MAX; //set X column pixel on side
   }

   //complete bitmap init of display controller
   LCDDrvSetSide( bSide );        //activate L/R display controller
   LCDDrvSetPage( ucPageStart );  //set Y page (0..6)
   ucPageOld = ucPageStart;       //save page
   LCDDrvSetColumn( uiXColCtrl ); //set X column for read/write

   //--------------------------
   //span complete bitmap height
   for( ucPage = ucPageStart; ucPage <= ucPageEnd; ucPage++ ){
      //span complete bitmap width
      //the display controller handles an 'column auto increment', so that we only need to set the column
      //after controller/page switching, but not after a simple read/write
      for( uiXCol = uiXColStart; uiXCol <= uiXColEnd; uiXCol++ ){
         //--------------------------
         //set current controller side, if necessary
         if( uiXCol < PAGE_COLUMN_MAX ){           //on which display half are we?
            //we are on the left side
            uiXColCtrl = uiXCol;                   //x column pixel on side
            if( bSide != LEFT ){                   //we need left side: already on left side?
               LCDDrvSetSide( LEFT );              //activate L/R display controller
               bSide = LEFT;                       //we are on the left side
               LCDDrvSetPage( ucPage );            //set y page (0..6)
               ucPageOld = ucPage;                 //save page
               LCDDrvSetColumn( uiXColCtrl );      //set x column for read/write
            }
         }
         else{
            //we are on the right side
            uiXColCtrl = uiXCol - PAGE_COLUMN_MAX; //x column pixel on L/R side
            if( bSide != RIGHT ){                  //we need right side: already on right side?
               LCDDrvSetSide( RIGHT );             //activate L/R display controller
               bSide = RIGHT;                      //we are on the right side
               LCDDrvSetPage( ucPage );            //set y page (0..6)
               ucPageOld = ucPage;                 //save page
               LCDDrvSetColumn( uiXColCtrl );      //set x column for read/write
            }
         }

         //reset column, if only page switch without side switch was done
         if( ucPage != ucPageOld ){
            LCDDrvSetPage( ucPage );               //set page on current controller
            ucPageOld = ucPage;                    //save page
            LCDDrvSetColumn( uiXColCtrl );         //set x column for read/write
         }

         //===============================================================================
         //is current bitmap column complete out of display area?
         //at right border left/right controller area or at lower display border?
         if( (uiXColCtrl < PAGE_COLUMN_MAX) && (ucPage < PAGE_MAX) ){

            unsigned char ucPixelNew;     //new output data (1 pixel column)
            unsigned char ucPixelOld;     //old display data (1 pixel column)
            unsigned char ucMaskUnEffect; //mask to prevent affecting old display data at bmp offsets (1 pixel column), '1' means to protect that bit

            //--------------------------
            //read old display data only if necessary
            ucPixelOld = 0; //default: don't effect new data with old data
            if( (ucMode & (DPLXOR | DPLOR | DPLAND | DPLNAND | DPLNOR)) || (ucOffsetPart1 > 0) ){
               //get old display data
               LCDDrvGetPixel( &ucPixelOld, DUMMY_ON ); //get old lcd data (lcd addr. is incremented)
               LCDDrvSetColumn( uiXColCtrl );           //reset x column for later write access
            }

            //--------------------------
            //prepare mask to protect old data if any bmp offset operations
            ucMaskUnEffect = 0; //default: don't protect old display data
            if( ucOffsetPart1 > 0 ){
               if( ucPage == ucPageStart ){
                  //clear upper part bits of mask to protect old display data
                  ucMaskUnEffect = (((UINT8)0xff) << ucOffsetPart2) >> ucOffsetPart2;
               }

               if( ucPage == ucPageEnd ){
                  //clear lower part bits of mask to protect old display data
                  ucMaskUnEffect = (((UINT8)0xff) >> ucOffsetPart1) << ucOffsetPart1;
               }
            }

            //--------------------------
            //get and prepare new bitmap data
            ucPixelNew = 0;                                          //reset pixel data
            if( ucOffsetPart1 > 0 ){                                 //vertical bitshift if necessary

               unsigned char   ucBMPPixel;                           // pointer in bmp source data (1 pixel column)

               if( ucPage > ucPageStart ){                           //lower byte part if necessary (not in first page)
                  ucBMPPixel = *(fpucData - pBitmap->wWidth);        //read bitmap data of page above
                  if( ucMode & DPLINVERS ) ucBMPPixel = ~ucBMPPixel; //first: invers bitmap
                  if( (ucMode & DPLUNDERL) && (ucPage == ucPageEnd) ) ucBMPPixel |= UNDERLINE; //second: set underline (only if lowest page)
                  ucPixelNew = ucBMPPixel >> ucOffsetPart2;          //shift to get relevant bits of actual page
               }

               if( ucPage < ucPageEnd ){                             //upper byte part if necessary (not in last page)
                  ucBMPPixel = *fpucData;                            //read bitmap data
                  if( ucMode & DPLINVERS ) ucBMPPixel = ~ucBMPPixel; //invers bitmap (no underline in upper parts)
                  ucPixelNew |= ucBMPPixel << ucOffsetPart1;         //shift to get relevant bits of page above
               }
            }
            else{
               ucPixelNew = *fpucData;                               //no page offsets: read normal bitmap data
               if( ucMode & DPLINVERS ) ucPixelNew = ~ucPixelNew;    //first invers bitmap
               if( (ucMode & DPLUNDERL) && (ucPage == ucPageEnd) ) ucPixelNew |= UNDERLINE; //second: set underline (only if lowest page)
            }

            //--------------------------
            //protect old display data if necessary
            if( ucMaskUnEffect > 0 ){
               ucPixelNew |= ucPixelOld & ucMaskUnEffect;   //copy only protected old data into new data
               ucPixelOld = ucPixelOld & (~ucMaskUnEffect); //keep rest of old data to manipulate with ucMode
            }

            //--------------------------
            //combine new & old pixel data
            if( (ucMode & (DPLXOR | DPLOR | DPLAND | DPLNAND | DPLNOR)) || (ucOffsetPart1 > 0) ){
               //only use bit mask 0xf8 for this case instr.
               switch( ucMode & ~(DPLINVERS | DPLUNDERL | DPLMODE8) ){
                  case DPLXOR:  ucPixelNew ^= ucPixelOld;  break;
                  case DPLOR:   ucPixelNew |= ucPixelOld;  break;
                  case DPLAND:  ucPixelNew &= ucPixelOld;  break;
                  case DPLNAND: ucPixelNew &= ~ucPixelOld; break;
                  case DPLNOR:  ucPixelNew |= ~ucPixelOld; break;
                  default: break;
               }
            }

            //--------------------------
            //copy raw data into display, display controller auto-increments the column after write acccess
            LCDDrvSetPixel( ucPixelNew );

         } //if is current bitmap...
         //===============================================================================

         fpucData++; //next bitmap raw data (pixel column)

      } //for uiXCol

   } //for ucPage

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
 *              supports only DPLNORM and DPLINVERS
 *********************************************************************** */
ERRCODE DisplPrintACharacter( const BITMAP far *pChar,
                              const DISPLXY far *pCoord,
                              unsigned char ucMode )
{
   const unsigned char far *fpucData = pChar->fpucBitmap; //pointer to raw data
   unsigned char ucPage;    //Y page 0..6
   unsigned int  uiXCol;    //X column 0..128
   unsigned char ucXColDpl; //X column 0..63 of controller

    //check parameters first
    if(  (pChar == NULL)
       ||(pChar->fpucBitmap == NULL)
       ||(pCoord->wYPos >= DisplGetDimension(DPLFONT_NOFONT).wYPos)
       ||(pCoord->wXPos >= DisplGetDimension(DPLFONT_NOFONT).wXPos) ){
       return ERR_PARAM_ERR;
    }

    //get the right page (0..6)
    ucPage = pCoord->wYPos / PAGE_HEIGHT;

    //initialize left/right controller
    if( pCoord->wXPos < PAGE_COLUMN_MAX ){
       LCDDrvSetSide( LEFT );
       LCDDrvSetPage( ucPage );
       ucXColDpl = (unsigned char)pCoord->wXPos;
       LCDDrvSetColumn( ucXColDpl );
   }
   else{
      LCDDrvSetSide( RIGHT );
      LCDDrvSetPage( ucPage );
      ucXColDpl = ( (unsigned char)pCoord->wXPos ) - PAGE_COLUMN_MAX;
      LCDDrvSetColumn( ucXColDpl );
   }

   //span complete character bitmap width
   for( uiXCol = pCoord->wXPos; uiXCol < (pCoord->wXPos + pChar->wWidth); uiXCol++ ){
      //change to right display controller only if necessary
      if( uiXCol == PAGE_COLUMN_MAX ){
         LCDDrvSetSide( RIGHT );
         LCDDrvSetPage( ucPage );
         LCDDrvSetColumn( 0 );
      }

      //copy raw data into display
      if( ucMode == DPLNORM ) LCDDrvSetPixel ( *fpucData++ );     //normal display
      if( ucMode & DPLINVERS ) LCDDrvSetPixel ( ~(*fpucData++) ); //invers character
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
ERRCODE DisplPrintAString(  const unsigned char far *szString,
                            const DISPLXY far *pCoord,
                            DPLFONT Font,
                            unsigned char ucMode )
{
   BITMAP Bitmap;              //character bitmap
   DISPLXY Coord;              //coordinates for each character
   unsigned char far *pucChar; //one char of szString
   unsigned char far *RValue;  //return value

   //check parameters first
   if(  (szString == NULL)
      ||(strlen( (const char *)szString ) == 0)
      ||(pCoord->wYPos >= DisplGetDimension(DPLFONT_NOFONT).wYPos)
      ||(pCoord->wXPos >= DisplGetDimension(DPLFONT_NOFONT).wXPos) ){
      return ERR_PARAM_ERR;
   }

   //init stuff
   Coord.wXPos = pCoord->wXPos;
   Coord.wYPos = pCoord->wYPos;
   pucChar = (unsigned char far*)szString;

   //loop over string length
   while( *pucChar != 0 ){
      //select the font
      RValue = NULL;
      switch( Font ){
         case DPLFONT_4X6:        //full featured font
         case DPLFONT_6X8:        //full featured font
         case DPLFONT_8X16:       //full featured font
            RValue = GetCharInfo( *pucChar, Font, &Bitmap );
            break;
         case DPLFONT_14X16:      //numbers only
         case DPLFONT_24X32:      //numbers only
         case DPLFONT_30X56:      //numbers only
            RValue = GetNumberCharInfo( *pucChar, Font, &Bitmap );
            break;
         default:
            return ERR_PARAM_ERR;
      } //of switch

      //character in font array?
      if( RValue != NULL ){
         //display this one character
         DisplPrintABitmap( &Bitmap, &Coord, ucMode );
         //coordinates for next character
         Coord.wXPos += Bitmap.wWidth;
      }

      //next character
      pucChar++;

   } //of while

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       DisplDrawHorLine
 *  DESCRIPTION:    prints a horizontal line with a defined thickness
 *                  to the screen
 *  PARAMETER:      DISPLXY far *   pCoord      x,y start coordinates
 *                  unsigned short  wLength     length of line in pixels
 *                  unsigned char   bPattern    line style pattern
 *                  unsigned char   ucMode      display mode (bit coded, f.e. invers)
 *  RETURN:         error code
 *  COMMENT:        Function uses a 8 vertical pixel with a bitcoded bPattern
 *                  to be drawn 'wLength'-times at a line:
 *                      - to Draw a single pixel line use 0x01
 *                      - to Draw a 8 pixel line use 0xff
 *                  To draw a line usually DPLXOR is used to keep
 *                  surrounding area where bPattern is 0.
 *********************************************************************** */
ERRCODE DisplDrawHorLine( const DISPLXY far *pCoord, unsigned short wLength,
                          unsigned char bPattern,
                          unsigned char ucMode )
{
   DISPLXY Coord;
   BITMAP  BMPLine;
   unsigned short wColumn;
   ERRCODE RValue;

   //check parameters first
   Coord = DisplGetDimension( DPLFONT_NOFONT );
   if(  (pCoord->wYPos >= Coord.wYPos)
      ||(pCoord->wXPos >= Coord.wXPos)
      ||(wLength  == 0 )
      ||(wLength  > Coord.wXPos) ){ 
      return ERR_PARAM_ERR;
   }

   //prepare bitmap
   BMPLine.wWidth  = wLength; //single column pattern
   BMPLine.wHeight = 8;       //8 bit vertical pattern
   BMPLine.fpucBitmap = (unsigned char far*)malloc( wLength );
   if( BMPLine.fpucBitmap == NULL ) return ERR_NO_MEM;
   for( wColumn = 0; wColumn < wLength; wColumn++ ){
      BMPLine.fpucBitmap[wColumn] = bPattern;
   }

   RValue = DisplPrintABitmap( &BMPLine, pCoord, ucMode );
   free( BMPLine.fpucBitmap );
   return RValue;
}


/***********************************************************************
 *  FUNCTION:       DisplSetParms
 *  DESCRIPTION:    set contrast and backlight level depending on
 *                  temperature/lumination and user setting
 *  PARAMETER:      none
 *  RETURN:         void
 ***********************************************************************/
void DisplSetParms(void)
{
    BOOL bBackLightOn;

    //set contrast level
    LCDDrvSetContrastLevel( gDisplayFlags.flags.ContrLev );

    //backlight on/off and level
    LCDDrvSetBacklightLevel(    DisplBacklightCheckOn (gDisplayFlags.flags.BacklOnLevel),
                                gDisplayFlags.flags.BacklLev );

}


/***********************************************************************
 *  FUNCTION:       DisplBacklightCheckOn
 *  DESCRIPTION:    returns TRUE if the backlicht should be on, depending
 *                  on user setting and brightness
 *  PARAMETER:      BacklOnLevel (0...7; user setting)
 *  RETURN:         Checks current Backlight on/off state and supports
 *                  hysteresys to avoid flickering:
 *                  - if Backlight is OFF: uses BacklLumiTable[] to switch
                      ON backlight without delay
 *                  - if Backlight is ON:  current AnaInGetLumiVal()
 *                    must be (BacklLumiTable[]-OFFSET) to set backlight
 *                    OFF, if much more ambiant brightness is given
 ***********************************************************************/
BOOL DisplBacklightCheckOn (UINT8 BacklOnLevel)
{
    // BacklLumiTable:  Values equal current AnaInGetLumiVal()
    //                  at current ambiant brightness (255=dark, 130=very bright)
    const UINT8 BacklLumiTable[] = { 255, 245, 235, 220, 190, 130 };
    UINT8   bHystOffsetValue;
    UINT16  wLumiVal;

    // parameter check
    if (BacklOnLevel > ARRAY_SIZE(BacklLumiTable))
    {
        /* value too high -> turn it on */
        return(TRUE);
    }

    // set on/off hysteresys offset value
    // depending on current backlight state:
    if ( LCDDrvGetBacklightState() == FALSE )
         bHystOffsetValue = 0;
    else bHystOffsetValue = 10;

    // check, wether current ambient brightness is darker than given user setting
    // -> check wether backlight should be on/off
    wLumiVal = AnaInGetLumiVal();
    if (  (wLumiVal != ANAIN_INVALID_U                                   )
        &&(wLumiVal > ((BacklLumiTable[BacklOnLevel]-bHystOffsetValue) ) ) )
         return ( TRUE );
    else return ( FALSE );
}
