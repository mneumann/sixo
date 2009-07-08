/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-24 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         objects & devices
 *  Purpose:        object defintions (text object, menu object, etc.)
 *                  to be used in 'device screens'
 *  Comments:       -
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
 * Revision 2.2  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.1  2007/03/30 10:05:56  tuberkel
 * just comments
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.5  2006/02/18 15:03:39  Ralf
 * - text object can be inverted now too
 * - small bugfixes in edit num objects
 *
 *
 ************************************************************************ */


/* include */
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "xtoa.h"
#include "standard.h"
#include "msgqueue.h"
#include "timer.h"
#include "debug.h"
#include "display.h"
#include "fonts.h"
#include "objects.h"
#include "digindrv.h"
#include "beep.h"
#include "led.h"
#include "resource.h"


/* character array for edit mode */
/* NOTE 'ö' and 'ü' are the only characters, that the compiler
        does not handle directly, so we've to use the hex-form! */
const UINT8 szLowerChars[]="aäbcdefghijklmno\xf6pqrsßtu\xfcvwxyz";
const UINT8 szUpperChars[]="AÄBCDEFGHIJKLMNOÖPQRSTUÜVWXYZ";
const UINT8 szNumericChars[]="0123456789";
const UINT8 szPunctChars[]="_-([{)]}!\x22#%&'*,./:;?@\\";
const UINT8 szSpecialChars[]="+<=>|~$^`";

#define MAXCHARLISTSIZE     120     /* max. number of chars if all listed are used together */






/***********************************************************************
 *  FUNCTION:       ObjBmpShow
 *  DESCRIPTION:    all handling of displayin BMP Object
 *  PARAMETER:      pointer to this
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ObjBmpShow( BMPOBJECT far * pObject)
{
    return DisplPrintABitmap( &pObject->Data, &pObject->Org, pObject->bMode);
}

/***********************************************************************
 *  FUNCTION:       ObjBmpInit
 *  DESCRIPTION:    Initializes BMP object
 *  PARAMETER:      list of all bmp properties
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ObjBmpInit( BMPOBJECT far * pObject,
                    UINT16 wOrgPosX,
                    UINT16 wOrgPosY,
                    UINT16 wWidth,
                    UINT16 wHeight,
                    unsigned char far * rgBMPRawData,
                    UINT8 bMode,
                    UINT8 bState)
{
    pObject->State.byte      = bState;
    pObject->Org.wXPos       = wOrgPosX;
    pObject->Org.wYPos       = wOrgPosY;
    pObject->Data.wWidth     = wWidth;
    pObject->Data.wHeight    = wHeight;
    pObject->Data.fpucBitmap = rgBMPRawData;
    pObject->bMode           = bMode;
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       ObjTextShow
 *  DESCRIPTION:    Handles displaying a complete multi characte line
 *                  formated text object
 *  PARAMETER:      pointer to TextObject to be shown
 *  RETURN:         ERR_OK
 *  COMMENT:        handles text output in multiple lines!
 *********************************************************************** */
ERRCODE ObjTextShow( TEXTOBJECT far * pObject)
{
    DISPLXY    PixelCoord;              /* coord of text parts (lines) */
    UINT8      bMode;                   /* selected display modes */
    UINT8      bMaxLine;                /* lines to be used */
    UINT8      bLine;                   /* for line loop */
    CHAR       szBuffer[TXTBUFFSIZE];   /* buffer to split text into WindowLines */
    STRING     szText;                  /* pointer to output string */
    STRING     szRestText;              /* pointer to rest output string */
    DISPLXY    DisplayCaps;             /* display dimensions */

    /* handling output mode */
    switch (pObject->eFormat)
    {
        case TXT_NORM:      bMode = DPLNORM;    break;
        case TXT_UNDERL:    bMode = DPLUNDERL;  break;
        case TXT_INVERS:    bMode = DPLINVERS;  break;
        default:            bMode = DPLNORM;    break;
    }

    /* additional check: selected? -> invert anyway! */
    if ( pObject->State.bits.fSelected == TRUE )
    {
        if ( pObject->eFormat == DPLINVERS )    // already inverted?
             bMode = DPLNORM;                   // YES -> show normal!
        else bMode = DPLINVERS;                 // NO  -> show invers!
    }

    /* calculating number of lines */
    bMaxLine = MIN( (strlen(pObject->szText)/pObject->Window.bWidth) + 1,
                    pObject->Window.bHeight                               );

    /* loop over all lines */
    for (bLine = 0; bLine < bMaxLine; bLine++)
    {
        /* set pixel coordinates */
        PixelCoord.wXPos = pObject->Org.wXPos;
        PixelCoord.wYPos = pObject->Org.wYPos + (bLine * GetFontHeight(pObject->eFont));

        /* get line part of the string */
        szRestText = pObject->szText + (bLine * pObject->Window.bWidth);    /* line start pointer */
        szText = strncpy(szBuffer, szRestText, pObject->Window.bWidth);     /* get a copy */
        szBuffer[pObject->Window.bWidth] = 0x0;                             /* strncpy() does not append a '/0' ! */

        /* align string */
        if (pObject->eAlign == TXT_CENTER)
            PixelCoord.wXPos =     pObject->Org.wXPos
                                    + ((pObject->Window.bWidth - strlen(szText)) / 2)
                                    * GetFontWidth(pObject->eFont);
        else if (pObject->eAlign == TXT_RIGHT)
            PixelCoord.wXPos =     pObject->Org.wXPos
                                    + (pObject->Window.bWidth - strlen(szText))
                                    * GetFontWidth(pObject->eFont);

        /* show this object */
        DisplPrintAString( szText, &PixelCoord, pObject->eFont, bMode );
    }
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       ObjTextInit
 *  DESCRIPTION:    Handles displaying a complete multiline formated
 *                  text object
 *  PARAMETER:      complete object parameter list
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ObjTextInit(    TEXTOBJECT far * pObject,   // allocated memory for text object
                        UINT16      wOrgPosX,       // origin X pos in pixel units
                        UINT16      wOrgPosY,       // origin Y pos in pixel units
                        DPLFONT     eFont,          // used font enum type
                        UINT8       bWindHeight,    // text window height in char(!) units
                        UINT8       bWindWidth,     // text window width in char(!) units
                        TXTALIGN    eAlign,         // text alignement in text window
                        TXTFORMAT   eFormat,        // special text output formats
                        STRING      szText,         // ptr to string buffer / static resource
                        UINT8       bState       )  // current state when initialized
{
    /* copy text object data */
    pObject->Org.wXPos      = wOrgPosX;
    pObject->Org.wYPos      = wOrgPosY;
    pObject->eFont          = eFont;
    pObject->Window.bHeight = bWindHeight;
    pObject->Window.bWidth  = bWindWidth;
    pObject->eAlign         = eAlign;
    pObject->eFormat        = eFormat;
    pObject->szText         = szText;
    pObject->State.byte     = bState;

    return ERR_OK;
}







/***********************************************************************
 *  FUNCTION:       ObjEditTextShow
 *  DESCRIPTION:    updates display appearance of edit text field
 *  PARAMETER:      edit text object
 *                  bUpdateMode     to show parts only
 *  RETURN:         ERR_OK
 *  COMMENT:        use the form "descriptor:   [edittext]"
 *                  NOTE:   This function only works with FIXED font sizes
 *                          over all characters (non proportional!)
 *********************************************************************** */
ERRCODE ObjEditTextShow( EDITTEXTOBJECT far * pObject, UINT8 bUpdateMode )
{
    DISPLXY     PixelCoord;                 /* coord of text parts (lines) */
    UINT8       bMode;                      /* selected display modes */
    CHAR        szOutText[TXTBUFFSIZE];     /* buffer to split text into WindowLines */
    DISPLXY     DisplayCaps;                /* display dimensions */
    UINT8       bXTextOffset;               /* x offset of edit text in text window oin pixel */
    UINT8       bCharWidth;                 /* character width in pixel */
    UINT8       i;

    /* init stuff */
    bCharWidth = GetFontWidth(pObject->eFont);  /* for offset calculations */
    PixelCoord.wYPos = pObject->Org.wYPos;      /* local y pos */

    /* ------------------------------------- */
    /* show descriptor & frame */
    if (bUpdateMode & SHOW_DESCR)
    {
        /* clear buffer  */
        memset( szOutText, 0x0, TXTBUFFSIZE);

        /* get descriptor at left side */
        strncpy(szOutText, pObject->szDescr, strlen(pObject->szDescr));

        /* set left & right border bracket */
        //szBuffer[pObject->Window.bWidth - pObject->bLength - 2] = CHAR_SQRBRCK_L;  /* left frame '[' */
        //szBuffer[pObject->Window.bWidth - 1]                    = CHAR_SQRBRCK_R;  /* left frame ']' */
        //szBuffer[pObject->Window.bWidth] = 0x0;                                    /* set string end /0 */

        /* handling output mode */
        if (  (pObject->State.bits.fSelected   == TRUE )
            &&(pObject->State.bits.fEditActive == FALSE) )
                bMode = DPLINVERS;                                  /* show focused */
        else    bMode = DPLNORM;                                    /* show unfocused or edit active */

        /* show descriptor and frame  */
        PixelCoord.wXPos = pObject->Org.wXPos;
        DisplPrintAString( szOutText, &PixelCoord, pObject->eFont, bMode );
    }

    /* ------------------------------------- */
    /* show edit text */
    if (  (bUpdateMode & SHOW_DESCR)
        ||(bUpdateMode & SHOW_EDIT ) )
    {
        /* clear buffer to spaces */
        memset( szOutText, 0x0, TXTBUFFSIZE);

        /* use original or work copy */
        if (pObject->State.bits.fEditActive == TRUE)
             strcpy(szOutText, pObject->szWorkText);
        else strcpy(szOutText, pObject->szText);

        /* get a well sized copy of edit text */
        szOutText[pObject->bLength] = 0x0;

        /* handling output mode */
        if (  (pObject->State.bits.fSelected   == TRUE )
            &&(pObject->State.bits.fEditActive == FALSE) )
                bMode = DPLINVERS;                              /* show focused */
        else
        {
            if ( pObject->State.bits.fSelectable == TRUE )
                 bMode = DPLUNDERL;                             /* show unfocused or edit active */
            else bMode = 0x0;                                   /* avoid showing underline */
        }


        /* set pixel coordinates: begin of edit text */
        PixelCoord.wXPos =   pObject->Org.wXPos
                            + bCharWidth
                            * (pObject->Window.bWidth - pObject->bLength);

        /* show edit text only */
        DisplPrintAString( szOutText, &PixelCoord, pObject->eFont, bMode );
    }

    /* ------------------------------------- */
    /* show char under cursor, if necesary */
    if (  (bUpdateMode & SHOW_DESCR  )
        ||(bUpdateMode & SHOW_EDIT   )
        ||(bUpdateMode & SHOW_CURSOR ) )
    {
        if (  (pObject->State.bits.fSelected   == TRUE )           /* we are focused? */
            &&(pObject->State.bits.fEditActive == TRUE ) )         /* edit mode is active? */
        {
            /* get a copy of single character under cursor */
            strncpy(szOutText, &pObject->szWorkText[pObject->bCursorPos], 1);
            szOutText[1] = 0x0;

            /* handling cursor blink phase */
            if (pObject->State.bits.fCursorOn  == TRUE )           /* cursur blink is in ON phase? */
                    bMode = DPLINVERS;                              /* ON = invers */
            else    bMode = DPLNORM;                                /* OFF = normal mode (repaint to clear inversed old cursor) */

            /* set pixel coordinates: begin of cursor char */
            PixelCoord.wXPos =   pObject->Org.wXPos
                                + bCharWidth
                                * (pObject->Window.bWidth - pObject->bLength + pObject->bCursorPos );

            /* show cursor char only */
            DisplPrintAString( szOutText, &PixelCoord, pObject->eFont, bMode );
        }
    }
    /* else: nothing to do... */
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       ObjEditTextInit
 *  DESCRIPTION:    start interface to edit text object
 *  PARAMETER:      complete parameter list
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ObjEditTextInit(    EDITTEXTOBJECT far * pObject,
                            UINT16      wOrgPosX,
                            UINT16      wOrgPosY,
                            DPLFONT     eFont,
                            UINT8       bWindWidth,
                            STRING      szText,
                            STRING      szDescr,
                            UINT8       bLength,
                            UINT8       bCharList,
                            UINT8       bState)
{
    /* check parameters first */
    if ( (strlen(szDescr) + bLength + 2) > bWindWidth)  /* descr + text + border fits into window? */
        return ERR_PARAM_ERR;

    /* copy object data */
    pObject->Org.wXPos      = wOrgPosX;         /* X origin in pixel coord. (0 = left) */
    pObject->Org.wYPos      = wOrgPosY;         /* Y origin in pixel coord. (0 = upper) */
    pObject->eFont          = eFont;            /* used font */
    pObject->Window.bWidth  = bWindWidth;       /* text windows width in chars */
    pObject->szText         = szText;           /* address of string to be edited */
    pObject->szDescr        = szDescr;          /* address of description string */
    pObject->bLength        = bLength;          /* length of edit field in chars */
    pObject->bCharList      = bCharList;        /* bitcoded list of chars to be used to edit */
    pObject->bCursorPos     = 0;                /* cursor on left position */
    pObject->State.byte     = bState;           /* all states initilzed */

    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       ObjEditTextMsgEntry
 *  DESCRIPTION:    Msg analyse function
 *  PARAMETER:      edit text object
 *  RETURN:         ERR_OK
 *  COMMENT:        Another DeviceMsgEntryFct. knows how to handle
 *                  our focus. If we've got this msg, this object really
 *                  HAS the focus!
 *********************************************************************** */
ERRCODE ObjEditTextMsgEntry( EDITTEXTOBJECT far * fpObject, MESSAGE GivenMsg )
{
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID  MsgId;
    MESSAGE     NewMsg;

    MsgId = MSG_ID(GivenMsg);                    /* get message id */

    if (fpObject->State.bits.fSelected == FALSE) /* Do we have the focus? */
        return RValue;                          /* No focus! */
    else
    {                                                   /* We have the focus! */
        if (fpObject->State.bits.fEditActive == FALSE)   /* Is edit mode active / inactive? */
        {
            /* ======================================================================== */
                                                        /* EDITMODE IS INACTIVE! */
            /* ------------------------------------------------------------------------ */
            if (  (MsgId == MSG_KEY_OK)                 /* [OK] starts the edit mode? */
                &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED) ) /* pressed the first time?*/
            {
                fpObject->bCursorPos  = 0;                      /* cursor on left position */
                fpObject->State.bits.fCursorOn   = TRUE;        /* cursor is visible */
                fpObject->State.bits.fEditActive = TRUE;        /* set new state! */
                strcpy(fpObject->szWorkText, fpObject->szText); /* get a work copy */
                ObjEditTextShow( fpObject, SHOW_ALL);           /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */
                ODS1(DBG_USER, DBG_INFO, "EditText started with [%s]", fpObject->szWorkText);
            }
        }
        else
        {
            /* ======================================================================== */
                                                        /* EDITMODE IS ALREADY ACTIVE! */
            RValue = ERR_MSG_PROCESSED;
            /* ------------------------------------------------------------------------ */
            if ( MsgId == MSG_DPL_FLASH_OFF )             /* FLASH OFF */
            {
                fpObject->State.bits.fCursorOn = FALSE;         /* hide cursor for a time */
                ObjEditTextShow( fpObject, SHOW_CURSOR);        /* show new state immediatly */
                MSG_FLASH_ON(NewMsg);                           /* next: cursor ON */
                SetTimerMsg(NewMsg, FLASH_OFF_TIME);            /* delay: cursor OFF time */
            }
            /* ------------------------------------------------------------------------ */
            else if ( MsgId == MSG_DPL_FLASH_ON )          /* FLASH ON */
            {
                fpObject->State.bits.fCursorOn = TRUE;          /* hide cursor for a time */
                ObjEditTextShow( fpObject, SHOW_CURSOR);        /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */
            }

            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_OK   )              /* [OK] */
                &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED) ) /* pressed the first time?*/
            {                                                   /* [OK] pressed 'short'! */
                fpObject->bCursorPos++;                         /* inkr. cursor position */
                if (fpObject->bCursorPos >= fpObject->bLength)  /* reached field end? */
                    fpObject->bCursorPos = 0;                   /* reset to begin */

                /* re-initiate cursor flashing */
                MSG_FLASH_OFF(NewMsg);                          /* remove all pending cursor OFF messages */
                TimerRemoveMsg(NewMsg);
                MSG_FLASH_ON(NewMsg);                           /* remove all pending cursor ON messages */
                TimerRemoveMsg(NewMsg);
                fpObject->State.bits.fCursorOn = TRUE;          /* cursor is visible */
                ObjEditTextShow( fpObject, SHOW_ALL);           /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */

            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_OK   )              /* [OK] */
                &&(MSG_KEY_DURATION(GivenMsg) > KEYSAVE) )   /* pressed 'long'? */
            {                                                   /* [OK] pressed 'long'! */
                strcpy(fpObject->szText, fpObject->szWorkText); /* save edited text! */
                fpObject->State.bits.fEditActive = FALSE;       /* user ended edit mode! */
                ODS1(DBG_USER, DBG_INFO, "EditText saved [%s]", fpObject->szWorkText);
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEYS_PRESSED                 )  /* multiple keys */
                &&(MSG_KEY_STATES(GivenMsg) & (KEY_UP | KEY_DOWN) ) )/* [UP]&[DOWN] pressed the same time? */
            {                                                           /* uses presses ESC! */
                strcpy(fpObject->szWorkText, fpObject->szText);         /* reset the work copy to original*/
                fpObject->State.bits.fEditActive = FALSE;               /* user ended edit mode! */
                ODS1(DBG_USER, DBG_INFO, "EditText ESC! String [%s] not changed!", fpObject->szWorkText);
            }

            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_UP )                  /* [UP] (short/long pressed) */
                &&(  (MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED)     /* pressed the first time?*/
                   ||(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON     ) ) ) /* or longer pressed?*/
            {
                ObjEditTextToggleChar(fpObject, MsgId);          /* handle selection of one character */

                /* re-initiate cursor flashing */
                MSG_FLASH_OFF(NewMsg);                          /* remove all pending cursor OFF messages */
                TimerRemoveMsg(NewMsg);
                MSG_FLASH_ON(NewMsg);                           /* remove all pending cursor ON messages */
                TimerRemoveMsg(NewMsg);
                fpObject->State.bits.fCursorOn = TRUE;          /* cursor is visible */
                ObjEditTextShow( fpObject, SHOW_ALL);           /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_DOWN )                /* [DOWN] (short/long pressed) */
                &&(  (MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED)     /* pressed the first time?*/
                   ||(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON     ) ) ) /* or longer pressed?*/
            {
                ObjEditTextToggleChar(fpObject, MsgId);          /* handle selection of one character */

                /* re-initiate cursor flashing */
                MSG_FLASH_OFF(NewMsg);                          /* remove all pending cursor OFF messages */
                TimerRemoveMsg(NewMsg);
                MSG_FLASH_ON(NewMsg);                           /* remove all pending cursor ON messages */
                TimerRemoveMsg(NewMsg);
                fpObject->State.bits.fCursorOn = TRUE;          /* cursor is visible */
                ObjEditTextShow( fpObject, SHOW_ALL);           /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */
            }
            else RValue = ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       ObjEditTextGetCharList
 *  DESCRIPTION:    returns a string with a character list to edit
 *  PARAMETER:      STRING  szCharList  pointer to result buffer
 *                  UINT8   bCharLists  bit coded charlist choice
 *  RETURN:         error code
 *  COMMENT:        t.b.d.: check buffer size!
 *                  BufferSize should be at least MAXCHARLISTSIZE
 *********************************************************************** */
ERRCODE ObjEditTextGetCharList(STRING szCharList, UINT8  bCharLists)
{
    CHAR szBuffer[MAXCHARLISTSIZE];

    strcat(szCharList, " ");
    if (bCharLists & CHARL_LOWER)
        strcat(szCharList, szLowerChars);
    if (bCharLists & CHARL_UPPER)
        strcat(szCharList, szUpperChars);
    if (bCharLists & CHARL_NUMERIC)
        strcat(szCharList, szNumericChars);
    if (bCharLists & CHARL_PUNCT)
        strcat(szCharList, szPunctChars);
    if (bCharLists & CHARL_PUNCT)
        strcat(szCharList, szSpecialChars);
    return ERR_OK;
}



/***********************************************************************
 *        FUNCTION:       ObjEditTextToggleChar
 *        DESCRIPTION:    toggles one char under cursor in string
 *                        as reaction to user UP/DOWN key action
 *        PARAMETER:      fpObject   ptr to edit item object
 *                        MsgID     message UP/DOWN to handle
 *        RETURN:         ERR_OK
 *        COMMENT:        The user changes the current char under the
 *                        cursor by pressing the UP/DOWN keys. The next
 *                        char to be displayed under the cursor depends
 *                        on the given format field of the edit item
 *                        (f.e. numericals only)
 *                        If user toggles to last/first char in list,
 *                        a WRAP AROUND is done.
*********************************************************************** */
ERRCODE ObjEditTextToggleChar(EDITTEXTOBJECT far * fpObject, MESSAGE_ID MsgID)
{
    ERRCODE                 RValue;                         /* return value */
    CHAR                    szCharList[MAXCHARLISTSIZE]=""; /* max. number of char as sum of all format */
    CHAR far *              fpCursorChar;                   /* current char under cursor in string */
    CHAR far *              fpListChar;                     /* position of char in prepared char list */

    /* first get the format string */
    ObjEditTextGetCharList(szCharList, fpObject->bCharList);

    /* get the current cursor char from string */
    fpCursorChar = (CHAR far *) &fpObject->szWorkText[fpObject->bCursorPos];

    /* search current cursor char in list to get next/previous char */
    fpListChar = strchr(szCharList, (int) *fpCursorChar);
    if (fpListChar == NULL)             /* current char not in list! (f.e. SPACE in a numeric field!) */
        fpListChar = szCharList;        /* default: just use the first char in list */

    /* get next char to replace cursor char */
    switch (MsgID)
    {
        /* DOWN: -> next char is previous char in list */
        case MSG_KEY_DOWN:
            if (fpListChar == &szCharList[0])                   /* current is first char in list? */
                fpListChar = &szCharList[strlen(szCharList)-1]; /* -> use last char (wrap around) */
            else
                fpListChar--;                                   /* -> simply use previous char */
        break;
        /* UP: -> next char is following char in list */
        case MSG_KEY_UP:
            if (fpListChar == &szCharList[strlen(szCharList)-1])/* current is last char in list? */
                fpListChar = &szCharList[0];                    /* -> use first char (wrap around) */
            else
                fpListChar++;                                   /* -> simply use next char */
        break;
        default: break;     /* can not happen because of parameter check */
    }

    /* replace char under cursor */
    *fpCursorChar = *fpListChar;

    return (ERR_OK);
}





/***********************************************************************
 *  FUNCTION:       ObjEditNumShow
 *  DESCRIPTION:    updates display appearance of edit number field
 *  PARAMETER:      fpObject         ptr edit num object
 *                  bUpdateMode      to show single parts only
 *  RETURN:         ERR_OK
 *  COMMENT:        uses the form "descriptor:   [number] unit"
 *                  NOTE:   This function only works with FIXED font sizes
 *                          over all characters (non proportional!)
 *
 *                  EXAMPLE:  +------------------------+
 *                            |decriptor:  0x_2340 unit|
 *                            +------------------------+
 *
 *********************************************************************** */
ERRCODE ObjEditNumShow( EDITNUMBEROBJECT far * fpObject, UINT8 bUpdateMode )
{
    DISPLXY     PixelCoord;                 /* coord of text parts (lines) */
    UINT8       bMode;                      /* selected display modes */
    CHAR        rgTextBuffer[TXTBUFFSIZE];  /* textbuffer for output purpose */
    STRING      szOutText;                  /* pointer to output string */
    DISPLXY     DisplayCaps;                /* display dimensions */
    UINT8       bXTextOffset;               /* x offset of edit text in text window oin pixel */
    UINT8       bCharWidth;                 /* character width in pixel */
    UINT8       i;

    /* return, if not enabled to display */
    if ( fpObject->State.bits.fDisplayable == 0 )
        return ERR_OK;

    /* init stuff */
    szOutText = &rgTextBuffer[0];                   /* set target buffer ptr */
    bCharWidth = GetFontWidth(fpObject->eFont);     /* for offset calculations */
    PixelCoord.wYPos = fpObject->Org.wYPos;         /* static y pos */

    /* ------------------------------------- */
    /* show descriptor & units */
    if (bUpdateMode & SHOW_DESCR)
    {
        /* handling output mode */
        bMode = DPLNORM;                                                    /* always show normal */

        /* --------------------------- */
        /* decriptor stuff: */
        memset( rgTextBuffer, 0x0, TXTBUFFSIZE);                            /* clear buffer to spaces */
        strncpy(szOutText, fpObject->szDescr, strlen(fpObject->szDescr));   /* get descriptor string */
        PixelCoord.wXPos = fpObject->Org.wXPos;                             /* show descriptor at left side */
        DisplPrintAString( szOutText, &PixelCoord, fpObject->eFont, bMode );/* print out text */

        /* --------------------------- */
        /* units stuff: */
        memset( rgTextBuffer, 0x0, TXTBUFFSIZE);                                    /* clear buffer to spaces */
        strncpy(szOutText, fpObject->szUnit, strlen(fpObject->szUnit));             /* get unit string */
        PixelCoord.wXPos =  fpObject->Org.wXPos
                            + bCharWidth *
                            (fpObject->Window.bWidth - strlen(fpObject->szUnit));   /* show unit at left window side */
        DisplPrintAString( szOutText, &PixelCoord, fpObject->eFont, bMode );        /* print out text */

        /* --------------------------- */
        /* hex code stuff */
        /* shows '0x' in front of edit field, if hex display format selected */
        if (fpObject->eDplType == eHex)
        {
            PixelCoord.wXPos =  fpObject->Org.wXPos
                                + bCharWidth *
                                 ( fpObject->Window.bWidth
                                   - strlen(fpObject->szUnit)
                                   - fpObject->bLength
                                   - strlen(TXTHEX)      );                     /* locate directly left of edit field */
            DisplPrintAString( TXTHEX, &PixelCoord, fpObject->eFont, bMode );   /* print out text */
        }
    }

    /* ------------------------------------- */
    /* show edit number */
    if (  (bUpdateMode & SHOW_DESCR)
        ||(bUpdateMode & SHOW_EDIT ) )
    {
        /* handling output mode */
        if (  (fpObject->State.bits.fSelected   == TRUE )
            &&(fpObject->State.bits.fEditActive == FALSE) )
                bMode = DPLINVERS;                              /* show focused */
        else
        {
            if ( fpObject->State.bits.fSelectable == TRUE )
                 bMode = DPLUNDERL;                             /* show unfocused or edit active */
            else bMode = 0x0;                                   /* avoid showing underline */
        }

        /* clear buffer to spaces */
        memset( rgTextBuffer, 0x0, TXTBUFFSIZE);

        /* get a well formated string from number  */
        ObjEditNum2String( fpObject, szOutText );

        /* set pixel coordinates: begin of edit text */
        PixelCoord.wXPos =   fpObject->Org.wXPos
                            + bCharWidth
                            * (  fpObject->Window.bWidth
                               - fpObject->bLength
                               - strlen(fpObject->szUnit));

        /* show edit number */
        DisplPrintAString( szOutText, &PixelCoord, fpObject->eFont, bMode );
    }

    /* ------------------------------------- */
    /* show char under cursor, if necessary */
    if (  (bUpdateMode & SHOW_DESCR  )
        ||(bUpdateMode & SHOW_EDIT   )
        ||(bUpdateMode & SHOW_CURSOR ) )
    {
        if (  (fpObject->State.bits.fSelected   == TRUE )           /* we are focused? */
            &&(fpObject->State.bits.fEditActive == TRUE ) )         /* edit mode is active? */
        {
            /* get a well formated string from number  */
            ObjEditNum2String( fpObject, szOutText );

            /* get single char at cursor pos */
            szOutText = szOutText + fpObject->bCursorPos;
            szOutText[1] = 0x0;

            /* handling cursor blink phase */
            if (fpObject->State.bits.fCursorOn  == TRUE )           /* cursur blink is in ON phase? */
                    bMode = DPLINVERS;                              /* ON = invers */
            else    bMode = DPLNORM;                                /* OFF = normal mode (repaint to clear inversed old cursor) */

            /* set pixel coordinates: begin of cursor char */
            PixelCoord.wXPos =   fpObject->Org.wXPos
                                + bCharWidth
                                * (   fpObject->Window.bWidth
                                    - fpObject->bLength
                                    - strlen(fpObject->szUnit)
                                    + fpObject->bCursorPos );

            /* show descriptor and frame  */
            DisplPrintAString( szOutText, &PixelCoord, fpObject->eFont, bMode );
        }
    }
    /* else: nothing to do... */
    return ERR_OK;
}




/***********************************************************************
 *  FUNCTION:       ObjEditNumInit
 *  DESCRIPTION:    start interface to edit number object
 *  PARAMETER:      complete parameter list
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ObjEditNumInit( EDITNUMBEROBJECT far * fpObject,
                        UINT16       wOrgPosX,
                        UINT16       wOrgPosY,
                        DPLFONT      eFont,
                        UINT8        bWindWidth,
                        void far *   fpNumber,
                        void far *   fpWorkNumber,
                        NUMTYPE      eType,
                        INT32        lMin,
                        INT32        lMax,
                        INT32        lStepSize,
                        NUMDPLTYPE   eDplType,
                        NUMMODETYPE  eMode,
                        UINT8        bComma,
                        STRING       szDescr,
                        STRING       szUnit,
                        UINT8        bLength,
                        UINT8        bState )
{
    /* check parameters first */
    /* descr + text + border fits into window? */
    if ( (strlen(szDescr) + bLength + strlen(szUnit) ) > bWindWidth)
    {
        ODS(DBG_USER, DBG_WARNING, "ObjEditNumInit() Object does not fit into window!");
        fpObject->State.byte = 0;   // object remains completely disabled
        return ERR_PARAM_ERR;
    }


    /* copy object data */
    fpObject->Org.wXPos     = wOrgPosX;         /* X origin in pixel coord. (0 = left) */
    fpObject->Org.wYPos     = wOrgPosY;         /* Y origin in pixel coord. (0 = upper) */
    fpObject->eFont         = eFont;            /* used font */
    fpObject->Window.bWidth = bWindWidth;       /* text windows width in chars */
    fpObject->fpNumber      = fpNumber;         /* original number to be edited */
    fpObject->fpWorkNumber  = fpWorkNumber;     /* number copy to be edited */
    fpObject->eType         = eType;            /* type of number */
    fpObject->Limits.lMin   = lMin;             /* min limit */
    fpObject->Limits.lMax   = lMax;             /* max limit */
    fpObject->Limits.lStep  = lStepSize;        /* step size */
    fpObject->eDplType      = eDplType;         /* display type */
    fpObject->eMode         = eMode;            /* edit mode */
    fpObject->bComma        = bComma;           /* comma position */
    fpObject->szDescr       = szDescr;          /* address of description string */
    fpObject->szUnit        = szUnit;           /* address of description string */
    fpObject->bLength       = bLength;          /* length of edit field in chars */
    fpObject->State.byte    = bState;           /* prepared caps & states */

    /* default values */
    fpObject->bCursorPos     = 0;                /* cursor on left position */
    fpObject->Window.bHeight = 1;                /* text windows height in chars */

    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       ObjEditNumMsgEntry
 *  DESCRIPTION:    Msg analyse function
 *  PARAMETER:      edit number object
 *  RETURN:         ERR_MSG_NOT_PROCESSED   if focused
 *                  ERR_MSG_PROCESSED       if not focused
 *  COMMENT:        Another DeviceMsgEntryFct. knows how to handle
 *                  our focus. If we've got this msg, this object really
 *                  HAS the focus!
 *********************************************************************** */
ERRCODE ObjEditNumMsgEntry( EDITNUMBEROBJECT far * fpObject, MESSAGE GivenMsg )
{
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID  MsgId;
    MESSAGE     NewMsg;
    CHAR        rgDebugBuff[20];

    MsgId = MSG_ID(GivenMsg);                               /* get message id */

    if (fpObject->State.bits.fSelected == FALSE)            /* Do we have the focus? */
        return RValue;                                      /* No focus! */
    else
    {                                                       /* We have the focus! */
        if (fpObject->State.bits.fEditActive == FALSE)      /* Is edit mode active / inactive? */
        {
            /* ======================================================================== */
                                                        /* EDITMODE IS INACTIVE! */
            /* ------------------------------------------------------------------------ */
            if (  (MsgId == MSG_KEY_OK                             )    /* [OK] starts the edit mode? */
                &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED)    /* pressed the first time?*/
                &&(fpObject->State.bits.fEditable == TRUE          ) )  /* object is editable? */
            {
                /* init edit mode */
                fpObject->bCursorPos = 0;                   /* cursor on left position */
                fpObject->State.bits.fCursorOn = FALSE;     /* cursor visible */
                fpObject->State.bits.fEditActive = TRUE;    /* set new edit state! */
                ObjEditNumCopy(fpObject, TRUE);             /* get a copy of number to work with */
                ObjEditNumShow( fpObject, SHOW_ALL);        /* show new state immediatly */
                MSG_FLASH_ON(NewMsg);                       /* next: cursor ON */
                SetTimerMsg(NewMsg, FLASH_OFF_TIME);        /* delay: cursor ON time */
                ODS1(DBG_USER, DBG_INFO, "EditNumber started with [%s]", ObjEditNum2String(fpObject, rgDebugBuff));
                RValue = ERR_MSG_PROCESSED;                         /* we used the msg in any way */
            }
        }
        else
        {
            /* ======================================================================== */
                                                            /* EDITMODE IS ALREADY ACTIVE! */

            RValue = ERR_MSG_PROCESSED;                         /* we used the msg in any way */

            /* ------------------------------------------------------------------------ */
            if ( MsgId == MSG_DPL_FLASH_OFF )               /* FLASH OFF */
            {
                fpObject->State.bits.fCursorOn = FALSE;         /* hide cursor for a time */
                ObjEditNumShow( fpObject, SHOW_CURSOR);         /* show new state immediatly */
                MSG_FLASH_ON(NewMsg);                           /* next: cursor ON */
                SetTimerMsg(NewMsg, FLASH_OFF_TIME);            /* delay: cursor OFF time */
            }
            /* ------------------------------------------------------------------------ */
            else if ( MsgId == MSG_DPL_FLASH_ON )           /* FLASH ON */
            {
                fpObject->State.bits.fCursorOn = TRUE;          /* hide cursor for a time */
                ObjEditNumShow( fpObject, SHOW_CURSOR);         /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */
            }

            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_OK   )              /* [OK] */
                &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED) ) /* pressed the first time?*/
            {                                                   /* [OK] pressed 'short'! */
                /* inkr. cursor position */
                fpObject->bCursorPos++;                         /* move cursor right */

                /* check: (comma is used) AND (cursor is on comma '.')? */
                if (  (fpObject->bComma > 0)
                    &&(fpObject->bCursorPos == (fpObject->bLength - fpObject->bComma - 1)))
                    fpObject->bCursorPos++;                     /* inkr. cursor position once more */

                /* check: reached edit field end? */
                if (fpObject->bCursorPos >= fpObject->bLength)  /* reached field end? */
                    fpObject->bCursorPos = 0;                   /* reset to begin */

                /* re-initiate cursor flashing */
                MSG_FLASH_OFF(NewMsg);                          /* remove all pending cursor OFF messages */
                TimerRemoveMsg(NewMsg);
                MSG_FLASH_ON(NewMsg);                           /* remove all pending cursor ON messages */
                TimerRemoveMsg(NewMsg);
                fpObject->State.bits.fCursorOn = TRUE;          /* cursor is visible */
                ObjEditNumShow( fpObject, SHOW_ALL);            /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_OK   )              /* [OK] */
                &&(MSG_KEY_DURATION(GivenMsg) > KEYSAVE) )   /* pressed 'long'? */
            {                                                   /* [OK] pressed 'long'! */
                ObjEditNumCopy(fpObject, FALSE);                /* save copy into original value */
                fpObject->State.bits.fEditActive = FALSE;       /* user ended edit mode! */
                ODS1(DBG_USER, DBG_INFO, "EditNumber saved new value [%s]", ObjEditNum2String(fpObject, rgDebugBuff));
                LEDOk();
                BeepOk();
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEYS_PRESSED                 )  /* multiple keys */
                &&(MSG_KEY_STATES(GivenMsg) & (KEY_UP | KEY_DOWN) ) )/* [UP]&[DOWN] pressed the same time? */
            {                                                        /* uses presses ESC! */
                //strcpy(fpObject->WorkNumber, fpObject->fpNumber);  /* reset the work copy to original*/
                fpObject->State.bits.fEditActive = FALSE;            /* user ended edit mode! */
                ODS1(DBG_USER, DBG_INFO, "EditNumber  [%s] not changed!", ObjEditNum2String(fpObject, rgDebugBuff));
                LEDEsc();
                BeepEsc();
            }

            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_UP )                  /* [UP] (short/long pressed) */
                &&(  (MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED)     /* pressed the first time?*/
                   ||(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON     ) ) ) /* or longer pressed?*/
            {
                ObjEditNumToggleNum(fpObject, MsgId);          /* handle selection of one character */

                /* re-initiate cursor flashing */
                MSG_FLASH_OFF(NewMsg);                          /* remove all pending cursor OFF messages */
                TimerRemoveMsg(NewMsg);
                MSG_FLASH_ON(NewMsg);                           /* remove all pending cursor ON messages */
                TimerRemoveMsg(NewMsg);
                fpObject->State.bits.fCursorOn = TRUE;          /* cursor is visible */
                ObjEditNumShow( fpObject, SHOW_ALL);            /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_DOWN )                /* [DOWN] (short/long pressed) */
                &&(  (MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED)     /* pressed the first time?*/
                   ||(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON     ) ) ) /* or longer pressed?*/
            {
                ObjEditNumToggleNum(fpObject, MsgId);          /* handle selection of one character */

                /* re-initiate cursor flashing */
                MSG_FLASH_OFF(NewMsg);                          /* remove all pending cursor OFF messages */
                TimerRemoveMsg(NewMsg);
                MSG_FLASH_ON(NewMsg);                           /* remove all pending cursor ON messages */
                TimerRemoveMsg(NewMsg);
                fpObject->State.bits.fCursorOn = TRUE;          /* cursor is visible */
                ObjEditNumShow( fpObject, SHOW_ALL);            /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */
            }
            else RValue = ERR_MSG_NOT_PROCESSED;                /* msg not used, although we have the focus */
        }
    }
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       ObjEditNumCopy
 *  DESCRIPTION:    copy number value(!): original <-> workcopy
 *  PARAMETER:      fpObject        ptr to number object
 *                  fCopy           TRUE  = Original -> WorkCopy
 *                                  FALSE = Original <- WorkCopy
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ObjEditNumCopy(EDITNUMBEROBJECT far * fpObject, BOOL fCopy)
{
    UINT8   bNumberOfBytes = 0;
    switch (fpObject->eType)
    {
        case eUCHAR:
        case eCHAR:
            bNumberOfBytes = 1;
            break;
        case eINT:
        case eUINT:
            bNumberOfBytes = 2;
            break;
        case eULONG:
        case eLONG:
            bNumberOfBytes = 4;
            break;
        default: ODS(DBG_SYS, DBG_ERROR, "Unknown NumberType!");
            return ERR_PARAM_ERR;
    }
    if (fCopy == TRUE)
         memcpy(fpObject->fpWorkNumber, fpObject->fpNumber, bNumberOfBytes);
    else
         memcpy(fpObject->fpNumber, fpObject->fpWorkNumber, bNumberOfBytes);
    return ERR_OK;
}





/***********************************************************************
 *  FUNCTION:       ObjEditNum2String
 *  DESCRIPTION:    converts number into string to display
 *  PARAMETER:      fpObject        ptr to number object to get string from
 *                  szTargetBuffer  buffer to copy string into
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
STRING ObjEditNum2String( EDITNUMBEROBJECT far * fpObject, STRING szTargetBuffer )
{
    int     iRadix;     // default base 10 format
    UINT8   strdiff;    // for strlen calculations
    UINT8   i;          // loop var
    STRING  fpSrc;      // source string ptr
    STRING  fpTrgt;     // target string ptr
    UINT8   bNumLen;    // result of first conversion
    void far * fpNumber;// temp. ptr original / copy number

    /* ------------------------------------- */
    /* choice of number base format */
    switch (fpObject->eDplType)
    {
        case eBin: iRadix = 2; break;
        case eDez: iRadix = 10; break;
        case eHex: iRadix = 16; break;
        default:
            iRadix = 10;
            ODS(DBG_SYS, DBG_ERROR, "ObjEditNum2String(): Unknown Displayformat!");
            goto NUM2STRINGRETURN;
    }

    /* choice of valid number to show (woriginal / workcopy) */
    if (fpObject->State.bits.fEditActive == TRUE)
         fpNumber = fpObject->fpWorkNumber;         /* EDIT MODE ACTIVE: use work copy */
    else fpNumber = fpObject->fpNumber;             /* NO EDIT MODE: use original number */

    /* ------------------------------------- */
    /* convert number into string */
    switch (fpObject->eType)
    {
        case eUCHAR:
            itoa( *(UINT8 far *)fpNumber, szTargetBuffer, iRadix);
            break;
        case eCHAR:
            itoa( *(INT8 far *)fpNumber, szTargetBuffer, iRadix);
            break;
        case eINT:
            itoa( *(INT16 far *)fpNumber, szTargetBuffer, iRadix);
            break;
        case eUINT: /* does not work with 'itoa()'! */
            ultoa( *(UINT16 far *)fpNumber, szTargetBuffer, iRadix);
            break;
        case eULONG:
            ultoa( *(UINT32 far *)fpNumber, szTargetBuffer, iRadix);
            break;
        case eLONG:
            ltoa( *(INT32 far *)fpNumber, szTargetBuffer, iRadix);
            break;
        default:
            ODS(DBG_SYS, DBG_ERROR, "ObjEditNum2String(): Unknown NumberType!");
            goto NUM2STRINGRETURN;
    }
    bNumLen = strlen(szTargetBuffer);
    if (bNumLen > fpObject->bLength)
    {
        ODS(DBG_SYS, DBG_ERROR, "ObjEditNum2String(): Number too long for conversion!");
        goto NUM2STRINGRETURN;
    }

    /* ------------------------------------- */
    /* fill in preceding zeros:
        f.e. (Length = 5): |1234| -> |01234| */
    if (bNumLen < fpObject->bLength)
    {
        /* backwards copying of chars in same string */
        fpSrc = &szTargetBuffer[bNumLen - 1];               /* source ptr in string */
        fpTrgt = &szTargetBuffer[fpObject->bLength - 1];    /* target ptr in string */
        strdiff = fpTrgt - fpSrc;                           /* copy distance in string */
        for (i = 0; i < bNumLen; i++)                       /* loop over part to shift */
            *fpTrgt-- = *fpSrc--;                           /* now copy char over distance */
        /* fill preceding zeros */
        fpTrgt = &szTargetBuffer[0];                        /* reset target ptr */
        for (i = 0; i < strdiff; i++)                       /* loop over part to fill */
            *fpTrgt++ = CHAR_ZERO;                          /* fill in '0' */
    }

    /* ------------------------------------- */
    /* fill in comma as seperator
        f.e. (Comma = 1): |01234| -> |123.4| */
    if (fpObject->bComma > 0)
    {
        if ( strlen(szTargetBuffer) < fpObject->bLength)
        {
            ODS(DBG_SYS, DBG_ERROR, "ObjEditNum2String(): No space to insert '.'!");
            goto NUM2STRINGRETURN;
        }
        else
        {
            for (i = 0; i < (fpObject->bLength - fpObject->bComma - 1); i++)
                szTargetBuffer[i] = szTargetBuffer[i+1];
        }
        szTargetBuffer[fpObject->bLength - fpObject->bComma - 1] = RESTXT_THOU_SEPARATOR;
    }


NUM2STRINGRETURN:

    /* ------------------------------------- */
    /* fill in secure string end */
    szTargetBuffer[fpObject->bLength] = 0x0;
    return szTargetBuffer;
}



/***********************************************************************
 *  FUNCTION:       ObjEditNumToggleNum
 *  DESCRIPTION:    Changes the number value in dependency of
 *                      - pressed key (up/down)
 *                      - current cursor position (0..n string position)
 *                      - used display mode (bin/dez/hex)
 *                      - used data type (8/16/32 bit, signed/unsigned)
 *                      - selected edit mode (column/step)
 *                      - selected limits and stepsize)
 *  PARAMETER:      fpObject        far ptr to this object
 *                  MsgID           message ID to determine inkr/dekr direction
 *  RETURN:         error code
 *  COMMENT:        Funtion handles 6 different data types with 3 display formats:
 *
 *                      data types:     8/16/32 values, signed/unsigned
 *                      display format: binary/decimal/hexadezimal
 *
 *                  Because of used display format, the current cursor
 *                  position (0..n, from left) represents different values.
 *
 *                  Examples:   Pos  number     cursorvalue
 *                              2    0010.1100  0010.0000(bin)
 *                              2    1234       30(dez)
 *                              2  0x5678       70(hex)
 *
 *                  The incr/decr value (step) is calculated like this:
 *
 *                              step = base^exponent    with exp=f(CursorPos)
 *
 *                  Note:   In column-Mode only the number under cursor is
 *                          affected, no carry is executed!
 *
 *********************************************************************** */
ERRCODE ObjEditNumToggleNum(EDITNUMBEROBJECT far * fpObject, MESSAGE_ID MsgID)
{
    UINT8 bBase;                    /* base value for inc/decr value calculations: 2/10/16 (bin/dec/hex) */
    UINT8 bExp;                     /* exponent value for inc/decr value calculations: 0..32 (32 eq. 2^32) */
    UINT8 bCursorVal;               /* value ofchar under cursor for incr/decr-carry detection */
    CHAR  cCursorChar[2] = {0,0};   /* buffer to handle copy of cursor string /0 closed */
    CHAR  szBuffer[TXTBUFFSIZE];    /* textbuffer */

    /* determine base */
    /* (check used display base format) */
    switch (fpObject->eDplType)
    {
        case eBin: bBase =  2; break;
        case eDez: bBase = 10; break;
        case eHex: bBase = 16; break;
        default:   bBase =  0;
        ODS(DBG_SYS, DBG_ERROR, "ObjEditNumToggleNum(): Unknown NumberDisplayType!");
    }

    /* determine exponent */
    /* (cursor position in string) */
    bExp = fpObject->bLength - fpObject->bCursorPos - 1;    /* equals exponent value for used base */
    if (  (fpObject->bComma > 0   )                         /* comma used in display? */
        &&(bExp > fpObject->bComma) )                       /* AND cursor is LEFT of comma? */
        bExp -= 1;                                          /* -> minus 1, if comma used for display */

    /* get value of char under cursor */
    ObjEditNum2String( fpObject, szBuffer );                /* get a well formated string from number */
    cCursorChar[0] = szBuffer[fpObject->bCursorPos];        /* get single char at cursor pos */
    bCursorVal = bCharToByte(cCursorChar[0]);               /* convert ascii char into byte format */

    /* check used data type */
    switch (fpObject->eType)
    {
        /* === 8 bit unsigned value ======================== */
        case eUCHAR:
        {
            UINT8 bStep = 0;                                            /* incr/decr value */
            UINT8 far * fpValue = (UINT8 far *)fpObject->fpWorkNumber;  /* type casted ptr to current value */
            UINT8 bUpLimit  = (UINT8)(fpObject->Limits.lMax);           /* type casted upper limit */
            UINT8 bLowLimit = (UINT8)(fpObject->Limits.lMin);           /* type casted lower limit */

            /* --------------------------------- */
            /* don't accept init values out of range */
            if (*fpValue > bUpLimit)
                *fpValue = bUpLimit;
            if (*fpValue < bLowLimit)
                *fpValue = bLowLimit;

            /* --------------------------------- */
            /* calculate step size (incr/decr value) */
            if (fpObject->eMode == eStep)
            {
                bStep = (UINT8)(fpObject->Limits.lStep);            /* STEP-Mode: from objects step size */
            }
            else if (fpObject->eMode == eColumn)                    /* COLUMN-Mode: from current value & cursor postion */
                bStep = (UINT8) dwYExpX(bBase, bExp);               /* simulate 'y^x' calculation */

            /* --------------------------------- */
            /* incr/decr with limit check: */
            switch (MsgID)
            {
                /* DOWN: decrement */
                case MSG_KEY_DOWN:
                    if (  ((*fpValue - bStep) <= *fpValue )         /* no underflow? */
                        &&((*fpValue - bStep) >= bLowLimit)         /* not under limit? */
                        &&(bCursorVal > 0                 ) )       /* no carry flag? */
                    {
                        *fpValue -= bStep;
                        //ODS6(DBG_SYS, DBG_INFO, "[-%u]: %u (Min:%u Max:%u Base:%u Exp:%u)", bStep, *fpValue, bLowLimit, bUpLimit, bBase, bExp);
                        ODS2(DBG_SYS, DBG_INFO, "[-%u]: %u", bStep, *fpValue);
                    }
                break;
                /* UP: increment */
                case MSG_KEY_UP:
                    if (  ((*fpValue + bStep) >= *fpValue)          /* no overflow? */
                        &&((*fpValue + bStep) <= bUpLimit)          /* not over limit? */
                        &&((bCursorVal + 1) < bBase      ) )        /* no carry flag? */
                    {
                        *fpValue += bStep;
                        //ODS6(DBG_SYS, DBG_INFO, "[+%u]: %u (Min:%u Max:%u Base:%u Exp:%u)", bStep, *fpValue, bLowLimit, bUpLimit, bBase, bExp);
                        ODS2(DBG_SYS, DBG_INFO, "[+%u]: %u ", bStep, *fpValue);
                    }
                break;
                default: break;
            }
        } break;

        /* === 8 bit signed value =========================== */
        case eCHAR:
        {
            INT8 cStep = 0;                                         /* incr/decr value */
            INT8 far * fpValue = (INT8 far *)fpObject->fpWorkNumber;/* type casted ptr to current value */
            INT8 cUpLimit  = (INT8)(fpObject->Limits.lMax);         /* type casted upper limit */
            INT8 cLowLimit = (INT8)(fpObject->Limits.lMin);         /* type casted lower limit */

            /* --------------------------------- */
            /* don't accept init values out of range */
            if (*fpValue > cUpLimit)
                *fpValue = cUpLimit;
            if (*fpValue < cLowLimit)
                *fpValue = cLowLimit;

            /* --------------------------------- */
            /* calculate step size (incr/decr value) */
            if (fpObject->eMode == eStep)
            {
                cStep = (INT8)(fpObject->Limits.lStep);             /* STEP-Mode: from objects step size */
            }
            else if (fpObject->eMode == eColumn)                    /* COLUMN-Mode: from current value & cursor postion */
                cStep = (INT8) dwYExpX(bBase, bExp);                /* simulate 'y^x' calculation */

            /* --------------------------------- */
            /* incr/decr with limit check: */
            switch (MsgID)
            {
                /* DOWN: decrement */
                case MSG_KEY_DOWN:
                    if (  ((*fpValue - cStep) <= *fpValue )         /* no underflow? */
                        &&((*fpValue - cStep) >= cLowLimit)         /* not under limit? */
                        &&(bCursorVal > 0                 ) )      /* no carry flag? */
                    {
                        *fpValue -= cStep;
                        //ODS6(DBG_SYS, DBG_INFO, "[-%d]: %d (Min:%d Max:%d Base:%u Exp:%u)", cStep, *fpValue, cLowLimit, cUpLimit, bBase, bExp);
                        ODS2(DBG_SYS, DBG_INFO, "[-%d]: %d ", cStep, *fpValue);
                    }
                break;
                /* UP: increment */
                case MSG_KEY_UP:
                    if (  ((*fpValue + cStep) >= *fpValue)          /* no overflow? */
                        &&((*fpValue + cStep) <= cUpLimit)          /* not over limit? */
                        &&((bCursorVal + 1) < bBase      ) )        /* no carry flag? */
                    {
                        *fpValue += cStep;
                        //ODS6(DBG_SYS, DBG_INFO, "[+%d]: %d (Min:%d Max:%d Base:%u Exp:%u)", cStep, *fpValue, cLowLimit, cUpLimit, bBase, bExp);
                        ODS2(DBG_SYS, DBG_INFO, "[+%d]: %d", cStep, *fpValue);
                    }
                break;
                default: break;
            }
        } break;

        /* === 16 bit signed value ========================== */
        case eINT:
        {
            INT16 iStep = 0;                                            /* incr/decr value */
            INT16 far * fpValue = (INT16 far *)fpObject->fpWorkNumber;  /* type casted ptr to current value */
            INT16 iUpLimit  = (INT16)(fpObject->Limits.lMax);           /* type casted upper limit */
            INT16 iLowLimit = (INT16)(fpObject->Limits.lMin);           /* type casted lower limit */

            /* --------------------------------- */
            /* don't accept init values out of range */
            if (*fpValue > iUpLimit)
                *fpValue = iUpLimit;
            if (*fpValue < iLowLimit)
                *fpValue = iLowLimit;

            /* --------------------------------- */
            /* calculate step size (incr/decr value) */
            if (fpObject->eMode == eStep)
            {
                iStep = (INT16)(fpObject->Limits.lStep);                /* STEP-Mode: from objects step size */
            }
            else if (fpObject->eMode == eColumn)                        /* COLUMN-Mode: from current value & cursor postion */
                iStep = (INT16) dwYExpX(bBase, bExp);                   /* simulate 'y^x' calculation */

            /* --------------------------------- */
            /* incr/decr with limit check: */
            switch (MsgID)
            {
                /* DOWN: decrement */
                case MSG_KEY_DOWN:
                    if (  ((*fpValue - iStep) <= *fpValue )         /* no underflow? */
                        &&((*fpValue - iStep) >= iLowLimit)         /* not under limit? */
                        &&(bCursorVal > 0                 ) )       /* no carry flag? */
                    {
                        *fpValue -= iStep;
                        //ODS6(DBG_SYS, DBG_INFO, "[-%i]: %i (Min:%i Max:%i Base:%u Exp:%u)", iStep, *fpValue, iLowLimit, iUpLimit, bBase, bExp);
                        ODS2(DBG_SYS, DBG_INFO, "[-%i]: %i", iStep, *fpValue);
                    }
                break;
                /* UP: increment */
                case MSG_KEY_UP:
                    if (  ((*fpValue + iStep) >= *fpValue)          /* no overflow? */
                        &&((*fpValue + iStep) <= iUpLimit)          /* not over limit? */
                        &&((bCursorVal + 1) < bBase      ) )        /* no carry flag? */
                    {
                        *fpValue += iStep;
                        //ODS6(DBG_SYS, DBG_INFO, "[+%i]: %i (Min:%i Max:%i Base:%u Exp:%u)", iStep, *fpValue, iLowLimit, iUpLimit, bBase, bExp);
                        ODS2(DBG_SYS, DBG_INFO, "[+%i]: %i", iStep, *fpValue);
                    }
                break;
                default: break;
            }
        } break;

        /* === 16 bit unsigned value ======================== */
        case eUINT:
        {
            UINT16 wStep = 0;                                           /* incr/decr value */
            UINT16 far * fpValue = (UINT16 far *)fpObject->fpWorkNumber;/* type casted ptr to current value */
            UINT16 wUpLimit  = (UINT16)(fpObject->Limits.lMax);         /* type casted upper limit */
            UINT16 wLowLimit = (UINT16)(fpObject->Limits.lMin);         /* type casted lower limit */

            /* --------------------------------- */
            /* don't accept init values out of range */
            if (*fpValue > wUpLimit)
                *fpValue = wUpLimit;
            if (*fpValue < wLowLimit)
                *fpValue = wLowLimit;

            /* --------------------------------- */
            /* calculate step size (incr/decr value) */
            if (fpObject->eMode == eStep)
            {
                wStep = (UINT16)(fpObject->Limits.lStep);                /* STEP-Mode: from objects step size */
            }
            else if (fpObject->eMode == eColumn)                        /* COLUMN-Mode: from current value & cursor postion */
                wStep = (UINT16) dwYExpX(bBase, bExp);                   /* simulate 'y^x' calculation */

            /* --------------------------------- */
            /* incr/decr with limit check: */
            switch (MsgID)
            {
                /* DOWN: decrement */
                case MSG_KEY_DOWN:
                    if (  ((*fpValue - wStep) <= *fpValue )         /* no underflow? */
                        &&((*fpValue - wStep) >= wLowLimit)         /* not under limit? */
                        &&(bCursorVal > 0                 ) )       /* no carry flag? */
                    {
                        *fpValue -= wStep;
                        //ODS6(DBG_SYS, DBG_INFO, "[-%u]: %u (Min:%u Max:%u Base:%u Exp:%u)", wStep, *fpValue, wLowLimit, wUpLimit, bBase, bExp);
                        ODS2(DBG_SYS, DBG_INFO, "[-%u]: %u ", wStep, *fpValue);
                    }
                break;
                /* UP: increment */
                case MSG_KEY_UP:
                    if (  ((*fpValue + wStep) >= *fpValue)          /* no overflow? */
                        &&((*fpValue + wStep) <= wUpLimit)          /* not over limit? */
                        &&((bCursorVal + 1) < bBase      ) )        /* no carry flag? */
                    {
                        *fpValue += wStep;
                        //ODS6(DBG_SYS, DBG_INFO, "[+%u]: %u (Min:%u Max:%u Base:%u Exp:%u)", wStep, *fpValue, wLowLimit, wUpLimit, bBase, bExp);
                        ODS2(DBG_SYS, DBG_INFO, "[+%u]: %u", wStep, *fpValue);
                    }
                break;
                default: break;
            }
        } break;

        /* ================================================== */
        case eULONG:
        {
            UINT32 dwStep = 0;                                          /* incr/decr value */
            UINT32 far * fpValue = (UINT32 far *)fpObject->fpWorkNumber;/* type casted ptr to current value */
            UINT32 dwUpLimit  = (UINT32)(fpObject->Limits.lMax);        /* type casted upper limit */
            UINT32 dwLowLimit = (UINT32)(fpObject->Limits.lMin);        /* type casted lower limit */

            /* --------------------------------- */
            /* don't accept init values out of range */
            if (*fpValue > dwUpLimit)
                *fpValue = dwUpLimit;
            if (*fpValue < dwLowLimit)
                *fpValue = dwLowLimit;

            /* --------------------------------- */
            /* calculate step size (incr/decr value) */
            if (fpObject->eMode == eStep)
            {
                dwStep = (UINT32)(fpObject->Limits.lStep);              /* STEP-Mode: from objects step size */
            }
            else if (fpObject->eMode == eColumn)                        /* COLUMN-Mode: from current value & cursor postion */
                dwStep = (UINT32) dwYExpX(bBase, bExp);                 /* simulate 'y^x' calculation */

            /* --------------------------------- */
            /* incr/decr with limit check: */
            switch (MsgID)
            {
                /* DOWN: decrement */
                case MSG_KEY_DOWN:
                    if (  ((*fpValue - dwStep) <= *fpValue )        /* no underflow? */
                        &&((*fpValue - dwStep) >= dwLowLimit)       /* not under limit? */
                        &&(bCursorVal > 0                 ) )       /* no carry flag? */
                    {
                        *fpValue -= dwStep;
                        //ODS6(DBG_SYS, DBG_INFO, "[-%lu]: %u (Min:%lu Max:%lu Base:%u Exp:%u)", dwStep, *fpValue, dwLowLimit, dwUpLimit, bBase, bExp);
                        ODS2(DBG_SYS, DBG_INFO, "[-%lu]: %u", dwStep, *fpValue);
                    }
                break;
                /* UP: increment */
                case MSG_KEY_UP:
                    if (  ((*fpValue + dwStep) >= *fpValue )          /* no overflow? */
                        &&((*fpValue + dwStep) <= dwUpLimit)          /* not over limit? */
                        &&((bCursorVal + 1) < bBase        ) )        /* no carry flag? */
                    {
                        *fpValue += dwStep;
                        //ODS6(DBG_SYS, DBG_INFO, "[+%lu]: %lu (Min:%lu Max:%lu Base:%u Exp:%u)", dwStep, *fpValue, dwLowLimit, dwUpLimit, bBase, bExp);
                        ODS2(DBG_SYS, DBG_INFO, "[+%lu]: %lu ", dwStep, *fpValue);
                    }
                break;
                default: break;
            }
        } break;

        /* ================================================== */
        case eLONG:
            /* signed long not yet supported */
            ODS(DBG_SYS, DBG_ERROR, "ObjEditNumToggleNum(): signed long not yet supported");
            break;
        /* ================================================== */
        default:
            ODS(DBG_SYS, DBG_ERROR, "ObjEditNumToggleNum(): Unknown NumberType!");
    }
}






/***********************************************************************
 *  FUNCTION:       dwYExpX
 *  DESCRIPTION:    Math function calculates y ^ x (base ^exponent)
 *  PARAMETER:      UINT8 bBase     base for calculation
 *                  UINT8 bExp      exponent for calculation
 *  RETURN:         UINT32          result of (bBase^bExp)
 *  COMMENT:        Used to avoid linking of math-lib.
 *********************************************************************** */
UINT32 dwYExpX(UINT8 bBase, UINT8 bExp)
{
    UINT8  i;
    UINT32 dwStep = 1;

    if (bExp > 0)
    {
        dwStep = bBase;
        for (i = 1; i < bExp; i++)
            dwStep *= bBase;
    }
    return dwStep;
}


/***********************************************************************
 *  FUNCTION:       bCharToByte
 *  DESCRIPTION:    converts a single char into a unsigned byte value,
 *                  if possible
 *  PARAMETER:      CHAR cHar       ascii char to be converted
 *  RETURN:         UINT8           value of
 *  COMMENT:        -
 *********************************************************************** */
UINT8 bCharToByte(CHAR cChar)
{
    UINT8 RValue = 0;

    if ( isdigit(cChar) )
        RValue = cChar - 48;        /* ascii char '0'..'9' -> number */
    else if ( isxdigit(cChar) )
    {
        if ( isupper(cChar) )
             RValue = cChar - 55;   /* ascii char 'A'..'F'-> number */
        else RValue = cChar - 87;   /* ascii char 'a'..'f'-> number */
    }
    else ODS1(DBG_SYS, DBG_ERROR, "bCharToByte(): Ascii char '%c' is no (hex-)digit! ", cChar );
    return RValue;
}


