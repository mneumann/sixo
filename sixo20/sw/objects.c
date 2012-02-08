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
 * Revision 3.4  2012/02/08 03:55:24  tuberkel
 * KEY_TIMING parameter names reviewed/changed
 *
 * Revision 3.3  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 3.2  2012/01/15 09:56:54  tuberkel
 * ObjEditBool: new behaviour
 * - UP/DOWN always toggls boolean state
 *
 * Revision 3.1  2011/05/29 13:52:36  tuberkel
 * BugFix ObjSelectShow
 * - now shows underline only if editable
 *
 * Revision 3.0  2010/11/07 13:25:12  tuberkel
 * V30 Preparations:
 * - Object Handling reviewed
 * - BugFix in Blinking Cursor in Editmode
 * - BugFix in ODS() Makros
 * - EditBoolObj with new 'Box' appearence
 * - SelectObj with optimized output
 *
 * Revision 2.9  2009/07/22 12:42:31  tuberkel
 * Just comments
 *
 * Revision 2.8  2009/07/22 08:29:41  tuberkel
 * BugFix: in ODS 'EditBool set to [%s]'
 *
 * Revision 2.7  2009/07/19 12:38:37  tuberkel
 * - ObjectInit reviewed
 * - EditText Object works with EXTERNAL(!) work buffer
 *
 * Revision 2.6  2009/07/18 06:27:16  tuberkel
 * - BugFixes: EditBoolObject value handling
 * - NEW: SelectObject
 *
 * Revision 2.5  2009/07/15 08:58:52  tuberkel
 * NEW: Boolean edit object
 *
 * Revision 2.4  2009/07/11 13:26:46  tuberkel
 * Improvement:
 * - TextEdit Object now supports LED/Beep as 'saved'/ESC' indicator like other objects
 * - TextEdit Object now handles descriptor field like other objects (not inverted if focused))
 *
 * Revision 2.3  2009/07/11 13:19:21  tuberkel
 * Improvement:
 * TextEdit Object now shows/edits complete edit _field_
 * instead of given edit _string_
 *
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
#include "digoutdr.h"
#include "beep.h"
#include "led.h"
#include "resource.h"


/* character array for edit mode */
/* NOTE '�' and '�' are the only characters, that the compiler
        does not handle directly, so we've to use the hex-form! */
const UINT8 szLowerChars[]      ="a�bcdefghijklmno\xf6pqrs�tu\xfcvwxyz";
const UINT8 szUpperChars[]      ="A�BCDEFGHIJKLMNO�PQRSTU�VWXYZ";
const UINT8 szNumericChars[]    ="0123456789";
const UINT8 szPunctChars[]      ="_-([{)]}!\x22#%&'*,./:;?@\\";
const UINT8 szSpecialChars[]    ="+<=>|~$^`";

#define MAXCHARLISTSIZE     120     /* max. number of chars if all listed are used together */







/* ====================================================================
   BITMAP OBJECT
   ==================================================================== */



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
ERRCODE ObjBmpInit( BMPOBJECT_INITTYPE far * fpInitData)
{
    fpInitData->fpObject->eType           = OBJT_BMP;
    fpInitData->fpObject->State.byte      = fpInitData->bState;
    fpInitData->fpObject->Org.wXPos       = fpInitData->wOrgPosX;
    fpInitData->fpObject->Org.wYPos       = fpInitData->wOrgPosY;
    fpInitData->fpObject->Data.wWidth     = fpInitData->wWidth;
    fpInitData->fpObject->Data.wHeight    = fpInitData->wHeight;
    fpInitData->fpObject->Data.fpucBitmap = (unsigned char far *)fpInitData->rgBMPRawData;
    fpInitData->fpObject->bMode           = fpInitData->bMode;
    return ERR_OK;
}








/* ====================================================================
   TEXT OBJECT
   ==================================================================== */




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
    CHAR       szBuffer[TXTTEMPBUFF];   /* buffer to split text into WindowLines */
    STRING     szText;                  /* pointer to output string */
    STRING     szRestText;              /* pointer to rest output string */
    DISPLXY    DisplayCaps;             /* display dimensions */

    /* check: do we have to re-paint a static element? */
    if (  (pObject->State.bits.fDynamic   == FALSE )
        &&(pObject->State.bits.fDisplayed == TRUE  ) )
    {
        /* this is a static text element which already has been shown */
        /* ==>nothing to do! */
        return ERR_OK;

        // TBD: NOT YET IMPLEMENTED!
        // If this code is active, any text object has to be reseted
        // whenever the screen/device is de-focused!
        // return ERR_OK;
    }

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

    /* ok, save, that we displayed this object */
    pObject->State.bits.fDisplayed = TRUE;

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
ERRCODE ObjTextInit(    TEXTOBJECT_INITTYPE far * fpInitData )
{
    fpInitData->fpObject->eType           = OBJT_TXT;
    fpInitData->fpObject->Org.wXPos       = fpInitData->wOrgPosX;
    fpInitData->fpObject->Org.wYPos       = fpInitData->wOrgPosY;
    fpInitData->fpObject->eFont           = fpInitData->eFont;
    fpInitData->fpObject->Window.bHeight  = fpInitData->bWindHeight;
    fpInitData->fpObject->Window.bWidth   = fpInitData->bWindWidth;
    fpInitData->fpObject->eAlign          = fpInitData->eAlign;
    fpInitData->fpObject->eFormat         = fpInitData->eFormat;
    fpInitData->fpObject->szText          = fpInitData->szText;
    fpInitData->fpObject->State.byte      = fpInitData->bState;

    return ERR_OK;
}





/* ====================================================================
   EDIT TEXT OBJECT
   ==================================================================== */



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
    CHAR        szOutText[TXTTEMPBUFF];     /* output temp buffer  */
    DISPLXY     DisplayCaps;                /* display dimensions */
    UINT8       bXTextOffset;               /* x offset of edit text in text window oin pixel */
    UINT8       bCharWidth;                 /* character width in pixel */
    UINT8       i;

    /* init stuff */
    bCharWidth = GetFontWidth(pObject->eFont);  /* for offset calculations */
    PixelCoord.wYPos = pObject->Org.wYPos;      /* local y pos */

    /* ------------------------------------- */
    /* going to show the descriptor */
    if ( bUpdateMode && SHOW_DESCR )
    {
        /* descriptor/all not yet been shown? */
        if ( pObject->State.bits.fDisplayed == FALSE )
        {
            /* clear buffer  */
            memset( szOutText, 0x0, TXTTEMPBUFF);

            /* handling output mode */
            bMode = DPLNORM;                    /* always show normal */

            /* get & show descriptor at left side */
            strncpy(szOutText, pObject->szDescr, strlen(pObject->szDescr));
            PixelCoord.wXPos = pObject->Org.wXPos;
            DisplPrintAString( szOutText, &PixelCoord, pObject->eFont, bMode );

            /* save that state */
            pObject->State.bits.fDisplayed = TRUE;
        }
    }

    /* ------------------------------------- */
    /* show edit text */
    if ( bUpdateMode && SHOW_EDIT )
    {
        /* clear buffer to spaces */
        memset( szOutText, 0x20, TXTTEMPBUFF);

        /* insert original or working copy of string */
        if (pObject->State.bits.fEditActive == TRUE)
             memcpy(szOutText, pObject->szWorkText, strlen(pObject->szWorkText));
        else memcpy(szOutText, pObject->szText,     strlen(pObject->szText));

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
    if ( bUpdateMode && SHOW_DESCR )
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
ERRCODE ObjEditTextInit( EDITTEXT_INITTYPE far * fpInitData )
{
    /* check parameters first */
    /* descr + text + border fits into window? */
    if ( (strlen(fpInitData->szDescr) + fpInitData->bLength + 2) > fpInitData->bWindWidth)
        return ERR_PARAM_ERR;

    /* copy object data */
    fpInitData->fpObject->eType          = OBJT_ETXT;
    fpInitData->fpObject->Org.wXPos      = fpInitData->wOrgPosX;         /* X origin in pixel coord. (0 = left) */
    fpInitData->fpObject->Org.wYPos      = fpInitData->wOrgPosY;         /* Y origin in pixel coord. (0 = upper) */
    fpInitData->fpObject->eFont          = fpInitData->eFont;            /* used font */
    fpInitData->fpObject->Window.bWidth  = fpInitData->bWindWidth;       /* text windows width in chars */
    fpInitData->fpObject->szText         = fpInitData->szText;           /* address of string to be edited */
    fpInitData->fpObject->szDescr        = fpInitData->szDescr;          /* address of description string */
    fpInitData->fpObject->szWorkText     = fpInitData->szWorkText;       /* address of buffer for editing */
    fpInitData->fpObject->bLength        = fpInitData->bLength;          /* length of edit field in chars */
    fpInitData->fpObject->bCharList      = fpInitData->bCharList;        /* bitcoded list of chars to be used to edit */
    fpInitData->fpObject->bCursorPos     = 0;                            /* cursor on left position */
    fpInitData->fpObject->State.byte     = fpInitData->bState;           /* all states initilzed */

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

                /* first fill buffer withs spaces, then insert current string */
                memset(fpObject->szWorkText, CHAR_SPACE,       fpObject->bLength);
                memcpy(fpObject->szWorkText, fpObject->szText, strlen(fpObject->szText));

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
            /* ------------------------------------------------------------------------ */
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
                &&(MSG_KEY_DURATION(GivenMsg) > KEYTM_PRESSED_VLONG) )   /* pressed 'long'? */
            {                                                   /* [OK] pressed 'long'! */
                strcpy(fpObject->szText, fpObject->szWorkText); /* save edited text! */
                fpObject->State.bits.fEditActive = FALSE;       /* user ended edit mode! */
                ODS1(DBG_USER, DBG_INFO, "EditText saved [%s]", fpObject->szWorkText);
                LED_SignalOk();                                        /* additional 'saved' indicator */
                Beep_SignalOk();                                       /* additional 'saved' indicator */
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEYS_PRESSED                 )     /* multiple keys */
                &&(MSG_KEY_STATES(GivenMsg) & (KEY_UP | KEY_DOWN) ) )   /* [UP]&[DOWN] pressed the same time? */
            {                                                           /* uses presses ESC! */
                strcpy(fpObject->szWorkText, fpObject->szText); /* reset the work copy to original*/
                fpObject->State.bits.fEditActive = FALSE;       /* user ended edit mode! */
                ODS1(DBG_USER, DBG_INFO, "EditText ESC! String [%s] not changed!", fpObject->szWorkText);
                LED_SignalEsc();                                       /* additional 'ESC' indicator */
                Beep_SignalEsc();                                      /* additional 'ESC' indicator */
            }

            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_UP )                  /* [UP] (short/long pressed) */
                &&(  (MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED)     /* pressed the first time?*/
                   ||(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON     ) ) ) /* or longer pressed?*/
            {
                /* change character under cursor */
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
                /* change character under cursor */
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






/* ====================================================================
   EDIT NUMBER OBJECT
   ==================================================================== */




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
    CHAR        rgTextBuffer[TXTTEMPBUFF];  /* textbuffer for output purpose */
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
        memset( rgTextBuffer, 0x0, TXTTEMPBUFF);                            /* clear buffer to spaces */
        strncpy(szOutText, fpObject->szDescr, strlen(fpObject->szDescr));   /* get descriptor string */
        PixelCoord.wXPos = fpObject->Org.wXPos;                             /* show descriptor at left side */
        DisplPrintAString( szOutText, &PixelCoord, fpObject->eFont, bMode );/* print out text */

        /* --------------------------- */
        /* units stuff: */
        memset( rgTextBuffer, 0x0, TXTTEMPBUFF);                                    /* clear buffer to spaces */
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
        memset( rgTextBuffer, 0x0, TXTTEMPBUFF);

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
ERRCODE ObjEditNumInit( EDITNUMBER_INITTYPE far * fpInitData )
{
    /* check parameters first */
    /* descr + text + border fits into window? */
    if ( (strlen(fpInitData->szDescr) + fpInitData->bLength + strlen(fpInitData->szUnit) ) > fpInitData->bWindWidth)
    {
        ODS(DBG_USER, DBG_WARNING, "ObjEditNumInit() Object does not fit into window!");
        fpInitData->fpObject->State.byte = 0;   // object remains completely disabled
        return ERR_PARAM_ERR;
    }

    /* copy object data */
    fpInitData->fpObject->eType         = OBJT_ENUM;
    fpInitData->fpObject->Org.wXPos     = fpInitData->wOrgPosX;         /* X origin in pixel coord. (0 = left) */
    fpInitData->fpObject->Org.wYPos     = fpInitData->wOrgPosY;         /* Y origin in pixel coord. (0 = upper) */
    fpInitData->fpObject->eFont         = fpInitData->eFont;            /* used font */
    fpInitData->fpObject->Window.bWidth = fpInitData->bWindWidth;       /* text windows width in chars */
    fpInitData->fpObject->fpNumber      = fpInitData->fpNumber;         /* original number to be edited */
    fpInitData->fpObject->fpWorkNumber  = fpInitData->fpWorkNumber;     /* number copy to be edited */
    fpInitData->fpObject->eNumType      = fpInitData->eNumType;         /* type of number */
    fpInitData->fpObject->Limits.lMin   = fpInitData->lMin;             /* min limit */
    fpInitData->fpObject->Limits.lMax   = fpInitData->lMax;             /* max limit */
    fpInitData->fpObject->Limits.lStep  = fpInitData->lStepSize;        /* step size */
    fpInitData->fpObject->eDplType      = fpInitData->eDplType;         /* display type */
    fpInitData->fpObject->eMode         = fpInitData->eMode;            /* edit mode */
    fpInitData->fpObject->bComma        = fpInitData->bComma;           /* comma position */
    fpInitData->fpObject->szDescr       = fpInitData->szDescr;          /* address of description string */
    fpInitData->fpObject->szUnit        = fpInitData->szUnit;           /* address of description string */
    fpInitData->fpObject->bLength       = fpInitData->bLength;          /* length of edit field in chars */
    fpInitData->fpObject->State.byte    = fpInitData->bState;           /* prepared caps & states */

    /* default values */
    fpInitData->fpObject->bCursorPos     = 0;                /* cursor on left position */
    fpInitData->fpObject->Window.bHeight = 1;                /* text windows height in chars */

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
            /* ------------------------------------------------------------------------ */
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
            else if (  (MsgId == MSG_KEY_OK   )                 /* [OK] */
                &&(MSG_KEY_DURATION(GivenMsg) > KEYTM_PRESSED_VLONG) )      /* pressed 'long'? */
            {                                                   /* [OK] pressed 'long'! */
                ObjEditNumCopy(fpObject, FALSE);                /* save copy into original value */
                fpObject->State.bits.fEditActive = FALSE;       /* user ended edit mode! */
                ODS1(DBG_USER, DBG_INFO, "EditNumber saved new value [%s]", ObjEditNum2String(fpObject, rgDebugBuff));
                LED_SignalOk();                                        /* additional 'saved' indicator */
                Beep_SignalOk();                                       /* additional 'saved' indicator */
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEYS_PRESSED                 )  /* multiple keys */
                &&(MSG_KEY_STATES(GivenMsg) & (KEY_UP | KEY_DOWN) ) )/* [UP]&[DOWN] pressed the same time? */
            {                                                        /* uses presses ESC! */
                //strcpy(fpObject->WorkNumber, fpObject->fpNumber);  /* reset the work copy to original*/
                fpObject->State.bits.fEditActive = FALSE;            /* user ended edit mode! */
                ODS1(DBG_USER, DBG_INFO, "EditNumber  [%s] not changed!", ObjEditNum2String(fpObject, rgDebugBuff));
                LED_SignalEsc();                                       /* additional 'ESC' indicator */
                Beep_SignalEsc();                                      /* additional 'ESC' indicator */
            }

            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_UP )                  /* [UP] (short/long pressed) */
                &&(  (MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED)     /* pressed the first time?*/
                   ||(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON     ) ) ) /* or longer pressed?*/
            {
                ObjEditNumToggleNum(fpObject, MsgId);           /* handle selection of one character */

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
    switch (fpObject->eNumType)
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
    switch (fpObject->eNumType)
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
    CHAR  szBuffer[TXTTEMPBUFF];    /* temp textbuffer */

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
    switch (fpObject->eNumType)
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
                        ODS2(DBG_SYS, DBG_INFO, "[-%lu]: %lu", (UINT32)bStep, (UINT32)*fpValue);
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
                        ODS2(DBG_SYS, DBG_INFO, "[+%lu]: %lu ", (UINT32)bStep, (UINT32)*fpValue);
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









/* ====================================================================
   EDIT BOOL OBJECT
   ==================================================================== */



/***********************************************************************
 *  FUNCTION:       ObjEditBoolShow
 *  DESCRIPTION:    updates display appearance of edit bool field
 *  PARAMETER:      fpObject         ptr edit num object
 *                  bUpdateMode      to show single parts only
 *  RETURN:         ERR_OK
 *  COMMENT:        uses the form "descriptor:   [bool]"
 *                  NOTE:   This function only works with FIXED font sizes
 *                          over all characters (non proportional!)
 *
 *                  EXAMPLE:  +------------------------+
 *                            |decriptor:           [x]|
 *                            +------------------------+
 *
 *********************************************************************** */
ERRCODE ObjEditBoolShow( EDITBOOLOBJECT far * fpObject, UINT8 bUpdateMode )
{
    DISPLXY     PixelCoord;                 /* coord of text parts (lines) */
    UINT8       bMode;                      /* selected display modes */
    CHAR        rgTextBuffer[TXTTEMPBUFF];  /* textbuffer for output purpose */
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
        memset( rgTextBuffer, 0x0, TXTTEMPBUFF);                            /* clear buffer to spaces */
        strncpy(szOutText, fpObject->szDescr, strlen(fpObject->szDescr));   /* get descriptor string */
        PixelCoord.wXPos = fpObject->Org.wXPos;                             /* show descriptor at left side */
        DisplPrintAString( szOutText, &PixelCoord, fpObject->eFont, bMode );/* print out text */
    }

    /* ------------------------------------- */
    /* show bool value */
    if (  (bUpdateMode & SHOW_DESCR)
        ||(bUpdateMode & SHOW_EDIT ) )
    {
        /* handling output mode */
        bMode = 0x0;                                   /* default mode */
        if (  (fpObject->State.bits.fSelected   == TRUE )
            &&(fpObject->State.bits.fEditActive == FALSE) )
        { bMode = DPLINVERS; }                         /* show focused */

        /* convert boolean value into a displayable icon */
        if (*fpObject->fpValue == TRUE)
             { strcpy(szOutText, EDITBOOL_TEXT_TRUE);  }
        else { strcpy(szOutText, EDITBOOL_TEXT_FALSE); }

        /* set pixel coordinates: begin of bool icon */
        PixelCoord.wXPos =    fpObject->Org.wXPos
                            + (bCharWidth * ( fpObject->Window.bWidth - EDITBOOL_TEXT_WIDTH ));

        /* show bool value */
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
            /* convert boolean WORK value into a displayable icon */
            if (*fpObject->fpWorkValue == TRUE)
                 { strcpy(szOutText, EDITBOOL_TEXT_TRUE);  }
            else { strcpy(szOutText, EDITBOOL_TEXT_FALSE); }

            /* handling cursor blink phase */
            if (fpObject->State.bits.fCursorOn  == TRUE )           /* cursur blink is in ON phase? */
                    bMode = DPLINVERS;                              /* ON = invers */
            else    bMode = DPLNORM;                                /* OFF = normal mode (repaint to clear inversed old cursor) */

            /* set pixel coordinates: begin of bool icon */
            PixelCoord.wXPos =   fpObject->Org.wXPos
                                + ( bCharWidth * ( fpObject->Window.bWidth - EDITBOOL_TEXT_WIDTH ) );

            /* show cursor */
            DisplPrintAString( szOutText, &PixelCoord, fpObject->eFont, bMode );
        }
    }
    /* else: nothing to do... */
    return ERR_OK;
}




/***********************************************************************
 *  FUNCTION:       ObjEditBoolInit
 *  DESCRIPTION:    start interface to edit bool object
 *  PARAMETER:      complete parameter list
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ObjEditBoolInit(EDITBOOL_INITTYPE far * fpInitData )
{
    EDITBOOLOBJECT fpObject;

    /* descr + text + border fits into window? */
    if ( (strlen(fpInitData->szDescr) + EDITBOOL_TEXT_WIDTH ) > fpInitData->bWindWidth)
    {
        ODS(DBG_USER, DBG_WARNING, "ObjEditBoolInit() Object does not fit into window!");
        fpInitData->fpObject->State.byte = 0;   // object remains completely disabled
        return ERR_PARAM_ERR;
    }

    /* copy const rom data into object data */
    fpInitData->fpObject->eType         = OBJT_EBOOL;
    fpInitData->fpObject->Org.wXPos     = fpInitData->wOrgPosX;     /* X origin in pixel coord. (0 = left) */
    fpInitData->fpObject->Org.wYPos     = fpInitData->wOrgPosY;     /* Y origin in pixel coord. (0 = upper) */
    fpInitData->fpObject->eFont         = fpInitData->eFont;        /* used font */
    fpInitData->fpObject->Window.bWidth = fpInitData->bWindWidth;   /* text windows width in chars */
    fpInitData->fpObject->fpValue       = fpInitData->fpValue;      /* original value to be edited */
    fpInitData->fpObject->fpWorkValue   = fpInitData->fpWorkValue;  /* value copy to be edited */
    fpInitData->fpObject->szDescr       = fpInitData->szDescr;      /* address of description string */
    fpInitData->fpObject->State.byte    = fpInitData->bState;       /* prepared caps & states */

    /* default values */
    fpInitData->fpObject->Window.bHeight = 1;                       /* text windows height in chars */

    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       ObjEditBoolMsgEntry
 *  DESCRIPTION:    Msg analyse function
 *  PARAMETER:      edit bool object
 *  RETURN:         ERR_MSG_NOT_PROCESSED   if focused
 *                  ERR_MSG_PROCESSED       if not focused
 *  COMMENT:        Another DeviceMsgEntryFct. knows how to handle
 *                  our focus. If we've got this msg, this object really
 *                  HAS the focus!
 *********************************************************************** */
ERRCODE ObjEditBoolMsgEntry( EDITBOOLOBJECT far * fpObject, MESSAGE GivenMsg )
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
            /* EDITMODE IS NOT ACTIVE! */

            /* ------------------------------------------------------------------------ */
            /* check: start edit mode? */
            if (  (MsgId == MSG_KEY_OK                             )    /* [OK] starts the edit mode? */
                &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED)    /* pressed the first time?*/
                &&(fpObject->State.bits.fEditable == TRUE          ) )  /* object is editable? */
            {
                /* init edit mode */
                fpObject->State.bits.fCursorOn = FALSE;     /* cursor visible */
                fpObject->State.bits.fEditActive = TRUE;    /* set new edit state! */
                *fpObject->fpWorkValue = *fpObject->fpValue;/* get a copy of value to work with */
                ObjEditBoolShow( fpObject, SHOW_ALL);       /* show new state immediatly */
                MSG_FLASH_ON(NewMsg);                       /* next: cursor ON */
                SetTimerMsg(NewMsg, FLASH_OFF_TIME);        /* delay: cursor ON time */
                ODS1(DBG_USER, DBG_INFO, "EditBool started with [%s]", (*fpObject->fpValue==TRUE)? "x" : "_" );
                RValue = ERR_MSG_PROCESSED;                         /* we used the msg in any way */
            }
            else
            {   /* nothing to be done here
                   up/down - move focus - will be processed in device for itself */
            }

        }
        else
        {
            /* ======================================================================== */
            /* EDITMODE IS ALREADY ACTIVE! */

            /* ------------------------------------------------------------------------ */
            RValue = ERR_MSG_PROCESSED;                     /* we used the msg in any way */

            /* ------------------------------------------------------------------------ */
            if ( MsgId == MSG_DPL_FLASH_OFF )               /* FLASH OFF */
            {
                fpObject->State.bits.fCursorOn = FALSE;         /* hide cursor for a time */
                ObjEditBoolShow( fpObject, SHOW_CURSOR);        /* show new state immediatly */
                MSG_FLASH_ON(NewMsg);                           /* next: cursor ON */
                SetTimerMsg(NewMsg, FLASH_OFF_TIME);            /* delay: cursor OFF time */
            }
            /* ------------------------------------------------------------------------ */
            else if ( MsgId == MSG_DPL_FLASH_ON )           /* FLASH ON */
            {
                fpObject->State.bits.fCursorOn = TRUE;          /* hide cursor for a time */
                ObjEditBoolShow( fpObject, SHOW_CURSOR);         /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */
            }

            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_OK   )                 /* [OK] */
                     &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED) ) /* pressed the first time?*/
            {                                                   /* [OK] pressed 'short'! */
                // no reaction, user has to press OK long to save!
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_OK   )                 /* [OK] */
                &&(MSG_KEY_DURATION(GivenMsg) > KEYTM_PRESSED_VLONG) )      /* pressed 'long'? */
            {                                                   /* [OK] pressed 'long'! */
                *fpObject->fpValue = *fpObject->fpWorkValue;    /* save the working copy into target */
                fpObject->State.bits.fEditActive = FALSE;       /* user ended edit mode! */
                ODS1(DBG_USER, DBG_INFO, "EditBool saved [%s]", (*fpObject->fpValue==TRUE)? "1" : "0" );
                LED_SignalOk();                                        /* additional 'saved' indicator */
                Beep_SignalOk();                                       /* additional 'saved' indicator */
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEYS_PRESSED                )  /* multiple keys */
                &&(MSG_KEY_STATES(GivenMsg) & (KEY_UP | KEY_DOWN)) )/* [UP]&[DOWN] pressed the same time? */
            {                                                        /* uses presses ESC! */
                fpObject->State.bits.fEditActive = FALSE;            /* user ended edit mode! */
                ODS1(DBG_USER, DBG_INFO, "EditBool [%s] not changed!", (*fpObject->fpValue==TRUE)? "x" : "_" );
                LED_SignalEsc();                                       /* additional 'ESC' indicator */
                Beep_SignalEsc();                                      /* additional 'ESC' indicator */
            }

            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_DOWN )                             /* [DOWN] */
                     &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON ) )     /* longer pressed?*/
            {
                // no reaction, no key repetition, just ignore...
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_UP )                           /* [UP]  */
                     &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON ) ) /* longer pressed?*/
            {
                // no reaction, no key repetition, just ignore...
            }

            /* ------------------------------------------------------------------------ */
            else if (  (  (MsgId == MSG_KEY_DOWN )                          /*    [DOWN]  */
                        ||(MsgId == MSG_KEY_UP   ) )                        /* or [UP]    */
                     &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED) ) /* pressed the first time?*/
            {
                /* simply INVERT boolean state! */
                *fpObject->fpWorkValue = (*fpObject->fpWorkValue==FALSE) ? TRUE : FALSE;
                ODS1(DBG_USER, DBG_INFO, "EditBool set to %u!", *fpObject->fpWorkValue );

                /* re-initiate cursor flashing */
                MSG_FLASH_OFF(NewMsg);                          /* remove all pending cursor OFF messages */
                TimerRemoveMsg(NewMsg);
                MSG_FLASH_ON(NewMsg);                           /* remove all pending cursor ON messages */
                TimerRemoveMsg(NewMsg);
                fpObject->State.bits.fCursorOn = TRUE;          /* cursor is visible */
                ObjEditBoolShow( fpObject, SHOW_ALL);           /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */
            }

            /* ------------------------------------------------------------------------ */
            else RValue = ERR_MSG_NOT_PROCESSED;                /* msg not used, although we have the focus */
        }
    }
    return RValue;
}









/* ====================================================================
   SELECT OBJECT
   ==================================================================== */





/***********************************************************************
 *  FUNCTION:       ObjSelectShow
 *  DESCRIPTION:    updates display appearance of select object field
 *  PARAMETER:      fpObject         ptr edit num object
 *                  bUpdateMode      to show single parts only
 *  RETURN:         ERR_OK
 *  COMMENT:        NOTE:   This function only works with FIXED font sizes
 *                          over all characters (non proportional!)
 *
 *                  EXAMPLE:  +------------------------+
 *                            |decriptor:  selecttext_a|
 *                            +------------------------+
 *
 *********************************************************************** */
ERRCODE ObjSelectShow( SELECTOBJECT far * fpObject, UINT8 bUpdateMode )
{
    DISPLXY     PixelCoord;                 /* coord of text parts (lines) */
    UINT8       bMode;                      /* selected display modes */
    CHAR        rgTextBuffer[TXTTEMPBUFF];  /* textbuffer for output purpose */
    CHAR        rgEmptyText[TXTTEMPBUFF];   /* for clear purpose */
    STRING      szOutText;                  /* pointer to output string */
    DISPLXY     DisplayCaps;                /* display dimensions */
    UINT8       bXTextOffset;               /* x offset of edit text in text window oin pixel */
    UINT8       bCharWidth;                 /* character width in pixel */
    UINT8       i;

    /* return, if not enabled to display */
    if ( fpObject->State.bits.fDisplayable == 0 )
        return ERR_OK;

    /* init stuff */
    szOutText  = &rgTextBuffer[0];                  /* set target buffer ptr */
    bCharWidth = GetFontWidth(fpObject->eFont);     /* for offset calculations */
    PixelCoord.wYPos = fpObject->Org.wYPos;         /* static y pos */
    memset(rgEmptyText, CHAR_SPACE, TXTTEMPBUFF);   /* generate an empty string */

    /* ------------------------------------- */
    /* show descriptor & units */
    if (bUpdateMode & SHOW_DESCR)
    {
        /* handling output mode */
        bMode = DPLNORM;                                                    /* always show normal */

        /* --------------------------- */
        /* descriptor stuff: */
        memset( rgTextBuffer, 0x0, TXTTEMPBUFF);                            /* clear buffer to spaces */
        strncpy(szOutText, fpObject->szDescr, strlen(fpObject->szDescr));   /* get descriptor string */
        PixelCoord.wXPos = fpObject->Org.wXPos;                             /* show descriptor at left side */
        DisplPrintAString( szOutText, &PixelCoord, fpObject->eFont, bMode );/* print out text */
    }

    /* ------------------------------------- */
    /* show selected text */
    if (  (bUpdateMode & SHOW_DESCR)
        ||(bUpdateMode & SHOW_EDIT ) )
    {
        // use this case in NON-EDIT mode only! (else: see below 'SHOW_CURSOR')
        if (fpObject->State.bits.fEditActive == FALSE)
        {
            /* handling output mode */
            if     ( fpObject->State.bits.fSelected == TRUE )
                    bMode = DPLINVERS;                          /* show focused */
            else if( fpObject->State.bits.fEditable == TRUE )
                    bMode = DPLUNDERL;                          /* show underline */
            else    bMode = DPLNORM;                            /* show default mode unfocused */

            /* convert ORIGINAL enum value into a selection text (and clip if too long)*/
            strcpy(szOutText, fpObject->pszSlctTxtList[*fpObject->fpValue]);
            szOutText[fpObject->bSelectWidth] = 0x00;

            /* set pixel coordinates: begin of selection text */
            PixelCoord.wXPos =     fpObject->Org.wXPos
                                +  bCharWidth
                                * (fpObject->Window.bWidth - fpObject->bSelectWidth);

            /* show select value */
            DisplPrintAString( szOutText,   &PixelCoord, fpObject->eFont, bMode  );
        }
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
            /* convert WORK enum value into a selection text (and clip if too long)*/
            strcpy(szOutText, fpObject->pszSlctTxtList[*fpObject->fpWorkValue]);
            szOutText[fpObject->bSelectWidth] = 0x00;

            /* handling cursor blink phase */
            if (fpObject->State.bits.fCursorOn  == TRUE )           /* cursur blink is in ON phase? */
                    bMode = DPLINVERS;                              /* ON = invers */
            else    bMode = DPLNORM;                                /* OFF = normal mode (repaint to clear inversed old cursor) */

            /* set pixel coordinates: begin of selection text */
            PixelCoord.wXPos =     fpObject->Org.wXPos
                                +  bCharWidth
                                * (fpObject->Window.bWidth - fpObject->bSelectWidth);

            /* show select value */
            DisplPrintAString( szOutText,   &PixelCoord, fpObject->eFont, bMode  );

            /* clear rest of line */
            rgEmptyText[fpObject->bSelectWidth - strlen(szOutText)] = 0x00;
            PixelCoord.wXPos += bCharWidth * strlen(szOutText);
            DisplPrintAString( rgEmptyText, &PixelCoord, fpObject->eFont, DPLNORM );
        }
    }
    /* else: nothing to do... */
    return ERR_OK;
}




/***********************************************************************
 *  FUNCTION:       ObjSelectInit
 *  DESCRIPTION:    start interface to select object
 *  PARAMETER:      complete parameter list
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ObjSelectInit(  SELECT_INITTYPE far * fpInitData )
{
    /* descr + text + border fits into window? */
    if ( (strlen(fpInitData->szDescr) + fpInitData->bSelectWidth ) > fpInitData->bWindWidth)
    {
        ODS(DBG_USER, DBG_WARNING, "ObjSelectInit() Object does not fit into window!");
        fpInitData->fpObject->State.byte = 0;   // object remains completely disabled
        return ERR_PARAM_ERR;
    }

    /* copy rom init data into object data */
    fpInitData->fpObject->eType          = OBJT_SLCT;
    fpInitData->fpObject->Org.wXPos      = fpInitData->wOrgPosX;        /* X origin in pixel coord. (0 = left) */
    fpInitData->fpObject->Org.wYPos      = fpInitData->wOrgPosY;        /* Y origin in pixel coord. (0 = upper) */
    fpInitData->fpObject->eFont          = fpInitData->eFont;           /* used font */
    fpInitData->fpObject->Window.bWidth  = fpInitData->bWindWidth;      /* text windows width in chars */
    fpInitData->fpObject->fpValue        = fpInitData->fpValue;         /* original value to be edited */
    fpInitData->fpObject->u8Max          = fpInitData->u8Max;           /* max value = list size */
    fpInitData->fpObject->fpWorkValue    = fpInitData->fpWorkValue;     /* value copy to be edited */
    fpInitData->fpObject->szDescr        = fpInitData->szDescr;         /* address of description string */
    fpInitData->fpObject->pszSlctTxtList = (STRING far *)fpInitData->pszSlctTxtList;  /* address of selection text list */
    fpInitData->fpObject->bSelectWidth   = fpInitData->bSelectWidth;    /* field width of select text */
    fpInitData->fpObject->State.byte     = fpInitData->bState;          /* prepared caps & states */

    /* default values */
    fpInitData->fpObject->Window.bHeight = 1;                           /* text windows height in chars */

    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       ObjSelectMsgEntry
 *  DESCRIPTION:    Msg analyse function
 *  PARAMETER:      select object
 *                  GivenMsg
 *  RETURN:         ERR_MSG_NOT_PROCESSED   if focused
 *                  ERR_MSG_PROCESSED       if not focused
 *  COMMENT:        Another DeviceMsgEntryFct. knows how to handle
 *                  our focus. If we've got this msg, this object really
 *                  HAS the focus!
 *********************************************************************** */
ERRCODE ObjSelectMsgEntry( SELECTOBJECT far * fpObject, MESSAGE GivenMsg )
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
                fpObject->State.bits.fCursorOn = FALSE;     /* cursor visible */
                fpObject->State.bits.fEditActive = TRUE;    /* set new edit state! */
                *fpObject->fpWorkValue = *fpObject->fpValue;/* get a copy of value to work with */
                ObjSelectShow( fpObject, SHOW_ALL);         /* show new state immediatly */
                MSG_FLASH_ON(NewMsg);                       /* next: cursor ON */
                SetTimerMsg(NewMsg, FLASH_OFF_TIME);        /* delay: cursor ON time */
                ODS1(DBG_USER, DBG_INFO, "Select started with [%u]", *fpObject->fpValue );
                RValue = ERR_MSG_PROCESSED;                 /* we used the msg in any way */
            }
        }
        else
        {
            /* ======================================================================== */
            /* EDITMODE IS ALREADY ACTIVE! */
            /* ------------------------------------------------------------------------ */
            RValue = ERR_MSG_PROCESSED;                         /* we used the msg in any way */

            /* ------------------------------------------------------------------------ */
            if ( MsgId == MSG_DPL_FLASH_OFF )               /* FLASH OFF */
            {
                fpObject->State.bits.fCursorOn = FALSE;         /* hide cursor for a time */
                ObjSelectShow( fpObject, SHOW_CURSOR);          /* show new state immediatly */
                MSG_FLASH_ON(NewMsg);                           /* next: cursor ON */
                SetTimerMsg(NewMsg, FLASH_OFF_TIME);            /* delay: cursor OFF time */
            }
            /* ------------------------------------------------------------------------ */
            else if ( MsgId == MSG_DPL_FLASH_ON )           /* FLASH ON */
            {
                fpObject->State.bits.fCursorOn = TRUE;          /* hide cursor for a time */
                ObjSelectShow( fpObject, SHOW_CURSOR);          /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */
            }

            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_OK   )                 /* [OK] */
                     &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED) ) /* pressed the first time?*/
            {                                                   /* [OK] pressed 'short'! */
                // no reaction
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_OK   )                 /* [OK] */
                &&(MSG_KEY_DURATION(GivenMsg) > KEYTM_PRESSED_VLONG) )      /* pressed 'long'? */
            {                                                   /* [OK] pressed 'long'! */
                *fpObject->fpValue = *fpObject->fpWorkValue;    /* save the working copy into target */
                fpObject->State.bits.fEditActive = FALSE;       /* user ended edit mode! */
                ODS1(DBG_USER, DBG_INFO, "Select saved [%u]", *fpObject->fpValue );
                LED_SignalOk();                                        /* additional 'saved' indicator */
                Beep_SignalOk();                                       /* additional 'saved' indicator */
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEYS_PRESSED                 )  /* multiple keys */
                &&(MSG_KEY_STATES(GivenMsg) & (KEY_UP | KEY_DOWN) ) )/* [UP]&[DOWN] pressed the same time? */
            {                                                        /* ==> user pressed ESC! */
                fpObject->State.bits.fEditActive = FALSE;            /* user ended edit mode! */
                ODS1(DBG_USER, DBG_INFO, "Select [%u] not changed!", *fpObject->fpValue);
                LED_SignalEsc();                                       /* additional 'ESC' indicator */
                Beep_SignalEsc();                                      /* additional 'ESC' indicator */
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_DOWN )                             /* [DOWN] */
                     &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON ) )     /* longer pressed?*/
            {
                // no reaction, no key repetition, just ignore...
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_DOWN )                             /* [DOWN]  */
                     &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED) ) /* pressed the first time?*/
            {
                /* set new selection value! */
                /* increment, if not already MAX */
                if(*fpObject->fpWorkValue < (fpObject->u8Max - 1))
                {   (*fpObject->fpWorkValue)++;
                    ODS1(DBG_USER, DBG_INFO, "Select changed to [%u]", *fpObject->fpWorkValue );
                }
                else
                {   ODS1(DBG_USER, DBG_INFO, "Select reached upper limit [%u]", *fpObject->fpWorkValue );
                }

                /* re-initiate cursor flashing */
                MSG_FLASH_OFF(NewMsg);                          /* remove all pending cursor OFF messages */
                TimerRemoveMsg(NewMsg);
                MSG_FLASH_ON(NewMsg);                           /* remove all pending cursor ON messages */
                TimerRemoveMsg(NewMsg);
                fpObject->State.bits.fCursorOn = TRUE;          /* cursor is visible */
                ObjSelectShow( fpObject, SHOW_ALL);             /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_UP )                           /* [UP]  */
                     &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_ON ) ) /* longer pressed?*/
            {
                // no reaction, no key repetition, just ignore...
            }
            /* ------------------------------------------------------------------------ */
            else if (  (MsgId == MSG_KEY_UP )                             /* [UP]  */
                     &&(MSG_KEY_TRANSITION(GivenMsg) == KEYTRANS_PRESSED) ) /* pressed the first time?*/
            {
                /* set new selection value! */
                /* decrement, if not already MIN */
                if(*fpObject->fpWorkValue > 0)
                {   (*fpObject->fpWorkValue)--;
                    ODS1(DBG_USER, DBG_INFO, "Select changed to [%u]", *fpObject->fpWorkValue );
                }
                else
                {   ODS1(DBG_USER, DBG_INFO, "Select reached lower limit [%u]", *fpObject->fpWorkValue );
                }

                /* re-initiate cursor flashing */
                MSG_FLASH_OFF(NewMsg);                          /* remove all pending cursor OFF messages */
                TimerRemoveMsg(NewMsg);
                MSG_FLASH_ON(NewMsg);                           /* remove all pending cursor ON messages */
                TimerRemoveMsg(NewMsg);
                fpObject->State.bits.fCursorOn = TRUE;          /* cursor is visible */
                ObjSelectShow( fpObject, SHOW_ALL);            /* show new state immediatly */
                MSG_FLASH_OFF(NewMsg);                          /* next: cursor OFF */
                SetTimerMsg(NewMsg, FLASH_ON_TIME);             /* delay: cursor ON time */
            }

            /* ------------------------------------------------------------------------ */
            else RValue = ERR_MSG_NOT_PROCESSED;                /* msg not used, although we have the focus */
        }
    }
    return RValue;
}








