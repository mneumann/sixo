/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-24 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         objects & devices
 *  Purpose:        object api export header
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
 *      Arnold Neugebauer                   Ralf Krizsan
 *      St�ckener Str. 115                  Knickstr. 10
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
 *  --------------------------------------------------------------------
 *
 *  CVS History
 *
 *  This information is automatically added while 'commiting' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.6  2006/02/18 15:00:10  Ralf
 * - edit number init type added
 *
 *
 ************************************************************************ */


#ifndef _OBJECTS_H
#define _OBJECTS_H


/* ------------------------------------------- */
/* general defines */
#define TXTBUFFSIZE     64          /* standard 64 character buffer size to handle with strings */
#define TXTHEX          "0x"        /* for hex formated output */

/* flashing cursor */
#define MSG_FLASH_ON(msg)    MSG_BUILD_UINT8(msg,MSG_DPL_FLASH_ON,0xff,0xff,0xff)
#define MSG_FLASH_OFF(msg)   MSG_BUILD_UINT8(msg,MSG_DPL_FLASH_OFF,0xff,0xff,0xff)
#define FLASH_ON_TIME        MS2TICKS(500)    /* cursor blink ON time in ms */
#define FLASH_OFF_TIME       MS2TICKS(300)    /* cursor blink ON time in ms */


/* update mode: special show modes to an edit object */
#define SHOW_CLEAR      0x0         /* clear text window area */
#define SHOW_DESCR      0x1         /* show edit descriptor only */
#define SHOW_EDIT       0x2         /* show edit item only */
#define SHOW_CURSOR     0x4         /* show cursor char only */
#define SHOW_ALL        0x7         /* show complete edit item elements */



/* -------------------------------------------
   OBJECT CAPAPILITIES AND STATE                */

/* standard status flags for objects */

/* valid object states: YES / NO */
#define NO  0x0
#define YES 0x1

/* capabilities and states struct (for bit access) */
typedef struct
{
    char fDisplayable:1;    /* able to be shown in display */
    char fSelectable :1;    /* able to focused, disabled underline too if FALSE */
    char fEditable   :1;    /* able to be edited */
    char fDplInit    :1;    /* currently grafical initialized (static elements shown)*/
    char fDisplayed  :1;    /* currently displayed */
    char fSelected   :1;    /* currently focused */
    char fEditActive :1;    /* edit mode is currently active */
    char fCursorOn   :1;    /* currently shows cursor (used to blink cursor) */
} OBJSTATEBF;

typedef struct {
  UINT16 wOrgPosX;
  UINT16 wOrgPosY;
  UINT16 wWidth;
  UINT16 wHeight;
  unsigned char far * rgBMPRawData;
  UINT8 bMode;
  UINT8 bState;
} BITMAPOBJECT_INIT_DATA;

/* defines for byte access level (same as OBJSTATEBF type) */
#define OC_DISPL    0x01    /* able to be shown in display */
#define OC_SELECT   0x02    /* able to focused */
#define OC_EDIT     0x04    /* able to be edited */
#define OS_INIT     0x08    /* currently grafical initialized (static elements shown)*/
#define OS_DISPL    0x10    /* currently displayed */
#define OS_SELECT   0x20    /* currently focused */
#define OS_EDIT     0x40    /* edit mode is currently active */
#define OS_CURSOR   0x80    /* currently shows cursor (used to blink cursor) */

typedef union
{
    OBJSTATEBF  bits;       /* to acces state bits seperatly */
    UINT8       byte;       /* to access all states in one step */
} OBJSTATE;



/* ------------------------------------------- */
/* COMMON OBJECT TYPES */

typedef struct
{
    UINT8   bWidth;     /* text window width in char units */
    UINT8   bHeight;    /* text window height in char units */
} TXTWINDIM;


/* alignment to output text in text window on screen */
typedef enum
{
    TXT_LEFT,       /* object is left aligned in window */
    TXT_RIGHT,      /* object is right aligned in window */
    TXT_CENTER      /* object is centered in window */
} TXTALIGN;


/* format to output text on screen */
typedef enum
{
    TXT_NORM,       /* show normal characters */
    TXT_UNDERL,     /* show characters with underline */
    TXT_INVERS      /* show characters inversed */
} TXTFORMAT;



/* ================================================ */
/* BITMAP OBJECT - BMP object data

    bitmap object features:

        - multiple display overlay modes (e.g. XOR)
        - focus possible

   ================================================ */
typedef struct
{
    OBJSTATE    State;       /* bitfields to handle display state */
    DISPLXY     Org;         /* bitmaps origin coordinates (0,0 = upper left corner ) */
    BITMAP      Data;        /* bitmap height, width, addr raw data */
    UINT8       bMode;       /* display mode (f.e. DPLNORM, DPLXOR, .. ) */
} BMPOBJECT;

/* BMP init data */
typedef struct
{
    BMPOBJECT far * pObject;
    UINT16 wOrgPosX;
    UINT16 wOrgPosY;
    UINT16 wWidth;
    UINT16 wHeight;
    unsigned char far * rgBMPRawData;
    UINT8 bMode;
    UINT8 bState;
} BMPOBJECT_INITTYPE;


/* text object prototypes */
ERRCODE ObjBmpShow( BMPOBJECT far * pObject);
ERRCODE ObjBmpInit( BMPOBJECT far * pObject,
                    UINT16 wOrgPosX,
                    UINT16 wOrgPosY,
                    UINT16 wWidth,
                    UINT16 wHeight,
                    unsigned char far * rgBMPRawData,
                    UINT8 bMode,
                    UINT8 bState);





/* ====================================================== */
/* TEXT OBJECT - text object data in a text window

    text object features:

        - external string pointer for edit purpose
        - free font use
        - 2 dimensional window (width & height)
        - window dimensions in chars (instead of pixel)
        - free text alignement (left/right/center)
        - text is broken into next line, if right window border reached

    text objects examples (border is 'text window'):

        +-----------+  +----+  +-+
        |text object|  |text|  |t|
        +-----------+  |obje|  |e|
                       |ct  |  |x|
                       +----+  |t|
                               +-+

   ======================================================= */
typedef struct
{
    OBJSTATE    State;          /* bitfields to handle display state */
    DISPLXY     Org;            /* origin in pixel coord. (0,0 = upper left) */
    TXTWINDIM   Window;         /* text windows dimension (in char units) */
    DPLFONT     eFont;          /* used font */
    STRING      szText;         /* address of string to be displayed */
    TXTALIGN    eAlign;         /* alignment of text in text window (left, right,..) */
    TXTFORMAT   eFormat;        /* format of text in text window (normal, invers,..) */
} TEXTOBJECT;

typedef struct {
  UINT16      wOrgPosX;
  UINT16      wOrgPosY;
  DPLFONT     eFont;
  UINT8       bWindHeight;
  UINT8       bWindWidth;
  TXTALIGN    eAlign;
  TXTFORMAT   eFormat;
  STRING      szText;
  UINT8       bState;
} TEXTOBJECT_INIT_DATA;


/* TextObject init data */
typedef struct
{
    TEXTOBJECT far * pObject;
    UINT16      wOrgPosX;
    UINT16      wOrgPosY;
    DPLFONT     eFont;
    UINT8       bWindHeight;
    UINT8       bWindWidth;
    TXTALIGN    eAlign;
    TXTFORMAT   eFormat;
    STRING      szText;
    UINT8       bState;
} TEXTOBJECT_INITTYPE;



/* text object prototypes */
ERRCODE ObjTextShow( TEXTOBJECT far * pObject);
ERRCODE ObjTextInit(    TEXTOBJECT far * pObject,
                        UINT16      wOrgPosX,
                        UINT16      wOrgPosY,
                        DPLFONT     eFont,
                        UINT8       bWindHeight,
                        UINT8       bWindWidth,
                        TXTALIGN    eAlign,
                        TXTFORMAT   eFormat,
                        STRING      szText,
                        UINT8       bState   );





/* ================================================ */
/* EDIT TEXT OBJECT - edit text object data

    edit text object features:

        - ptr to external descriptor text and edit string
        - local buffer for edit work
        - preselected list of chars to be used for edit
        - flickering cursor if edit mode active
        - seperate window width and edit field length
        - descriptor text is left aligned, edit text right aligned

    using edit text objects:

        - start edit mode by focusing and pressing ok key
        - change character under cursor with up/down key
        - move cursor with ok-key
        - save text by pressing ok-key 1 second
        - escape from edit mode by pressing up&down key together

    edit text object example (border is 'text window'):

        +------------------------------+
        |Decriptor:      Edittext______|
        +------------------------------+

   ================================================ */

typedef struct
{
    OBJSTATE    State;          /* bitfields to handle display state */
    DISPLXY     Org;            /* origin in pixel coord. (0,0 = upper left) */
    TXTWINDIM   Window;         /* text windows dimension (in chars units) */
    DPLFONT     eFont;          /* used font */
    STRING      szText;         /* address of string to be edited */
    STRING      szDescr;        /* address of description string (left aligned) */
    UINT8       bLength;        /* length of edit field in chars */
    UINT8       bCharList;      /* bitcoded list of chars to be used to edit */
    UINT8       bCursorPos;     /* position of current cursor in edit text (0..bLength) */
    CHAR        szWorkText[TXTBUFFSIZE];    /* text buffer for edit work */
                                            /* original text remains unchanged until [OK] */
} EDITTEXTOBJECT;

/* edit text choice (bitcoded) */
#define CHARL_LOWER     0x01    /* lower case characters */
#define CHARL_UPPER     0x02    /* upper case characters */
#define CHARL_NUMERIC   0x04    /* numeric characters */
#define CHARL_PUNCT     0x08    /* punctuations characters */
#define CHARL_SPECIAL   0x10    /* special characters */


/* edit text object prototypes */
ERRCODE ObjEditTextMsgEntry( EDITTEXTOBJECT far * pObject, MESSAGE msg );
ERRCODE ObjEditTextShow( EDITTEXTOBJECT far * pObject, UINT8 bUpdateMode );
ERRCODE ObjEditTextToggleChar(EDITTEXTOBJECT far * pObject, MESSAGE_ID MsgID);
ERRCODE ObjEditTextGetCharList(STRING szCharList, UINT8  bCharLists);
ERRCODE ObjEditTextInit(    EDITTEXTOBJECT far * pObject,
                            UINT16  wOrgPosX,
                            UINT16  wOrgPosY,
                            DPLFONT eFont,
                            UINT8   bWindWidth,
                            STRING  szText,
                            STRING  szDescr,
                            UINT8   bLength,
                            UINT8   bCharList,
                            UINT8   bState );






/* ================================================ */
/* EDIT NUMBER OBJECT - edit number object data

    edit number object features:

        - ptr to external descriptor, edit number and unit string
        - local buffer for edit work
        - preselected min / max / stepsize values
        - preselected number type (signed/unsigned, 8/16/32 bit size)
        - flickering cursor if edit mode active
        - descriptor is left aligned, edit number with units right aligned
        - seperate window width and edit field length
        - two modes to edit: complete value change or single column change

    using edit number object (stepsize == 0, single column change):

        - start edit mode by focusing and pressing ok key
        - change number under cursor with up/down key
        - move cursor with ok-key
        - save number by pressing ok-key 1 second
        - escape from edit mode by pressing up&down key together
        - edit mode closes only with valid number or ESC

    using edit number object (stepsize != 0, complete value change):

        - start edit mode by focusing and pressing ok key
        - change value in stepsize with up/down key
        - save number by pressing ok-key 1 second
        - escape from edit mode by pressing up&down key together
        - edit mode closes only with valid number or ESC

    edit int object examples (border is 'text window'):

        +------------------------+
        |decriptor:    _2340 unit|
        +------------------------+

   ================================================ */


/* ------------------------------------------- */
/* EDIT NUMBER TYPES

   used for char, byte, int, word, double word, long types */

/* enum to define kind of edit number type */
typedef enum
{
    eUCHAR,     /* unsigned  8 bit value */
    eUINT,      /* unsigned 16 bit value */
    eULONG,     /* unsigned 32 bit value */
    eCHAR,      /*   signed  8 bit value */
    eINT,       /*   signed 16 bit value */
    eLONG       /*   signed 32 bit value */
} NUMTYPE;

/* edit mode */
typedef enum
{
    eColumn,    /* edit in columns at cursor position */
    eStep       /* edit in steps of 'stepsize' at complete number */
} NUMMODETYPE;


/* enum to define display type of number */
typedef enum
{
    eBin,       /* display in binary format ( f.e. '0010.0110' as a byte) */
    eDez,       /* display in dez. format (f.e. '123.4' as word) */
    eHex        /* display in Hex-format (f.e. '0xff12.ac4f' as double word) */
} NUMDPLTYPE;


/* edit number limits (signed!, to be used for all types) */
typedef struct
{
    INT32  lMin;        /* lower limit (including) */
    INT32  lMax;        /* upper limit (including) */
    INT32  lStep;       /* inkr/decr. stepsize */
} EDTNUMLIMITS;


/* object structure */
typedef struct
{
    OBJSTATE        State;              /* bitfields to handle display state */
    DISPLXY         Org;                /* origin in pixel coord. (0,0 = upper left) */
    TXTWINDIM       Window;             /* text windows dimension (in chars units) */
    DPLFONT         eFont;              /* used font */
    NUMTYPE         eType;              /* bit size and signed type of edit number */
    NUMDPLTYPE      eDplType;           /* used display format (bin/dez/hex) */
    NUMMODETYPE     eMode;              /* used edit mode (eColumn/eStep) */
    UINT8           bComma;             /* display '.' as a seperator (f.e.: 0='1234'; 1='123.4'; 2='12.34' */
    EDTNUMLIMITS    Limits;             /* struct to contain value limits and incr. steps */
    void far *      fpNumber;           /* address of original number */
    STRING          szDescr;            /* address of description string (left aligned) */
    STRING          szUnit;             /* address of units string (right aligned) */
    UINT8           bLength;            /* length of edit field in chars (incl. '.', but without '0x') */
    UINT8           bCursorPos;         /* position of current cursor in edit string (valid: 0..bLength) */
    void far *      fpWorkNumber;       /* address of buffer for edit work
                                           (original number remains unchanged until [OK]) */
} EDITNUMBEROBJECT;


/* init object structure */
typedef struct
{
    EDITNUMBEROBJECT far *  fpObject;
    UINT16                  wOrgPosX;
    UINT16                  wOrgPosY;
    DPLFONT                 eFont;
    UINT8                   bWindWidth;
    void far *              fpNumber;
    void far *              fpWorkNumber;
    NUMTYPE                 eType;
    INT32                   lMin;
    INT32                   lMax;
    INT32                   lStepSize;
    NUMDPLTYPE              eDplType;
    NUMMODETYPE             eMode;
    UINT8                   bComma;
    STRING                  szDescr;
    STRING                  szUnit;
    UINT8                   bLength;
    UINT8                   bState;
} EDITNUMBER_INITTYPE;



/* edit text object prototypes */
ERRCODE ObjEditNumMsgEntry( EDITNUMBEROBJECT far * fpObject, MESSAGE msg );
ERRCODE ObjEditNumShow( EDITNUMBEROBJECT far * fpObject, UINT8 bUpdateMode );
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
                        UINT8        bState );
ERRCODE ObjEditNumCopy(EDITNUMBEROBJECT far * fpObject, BOOL fCopy);
STRING  ObjEditNum2String( EDITNUMBEROBJECT far * fpObject, STRING szTargetBuffer );
ERRCODE ObjEditNumToggleNum(EDITNUMBEROBJECT far * fpObject, MESSAGE_ID MsgID);
UINT32 dwYExpX(UINT8 bBase, UINT8 bExp);
UINT8 bCharToByte(CHAR cChar);




#endif /* _OBJECTS_H */

