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
 *
 ************************************************************************ */
#ifndef _DISPLAY_H
#define _DISPLAY_H


/* display modes (8 bit coded)*/
                            /* THESE LOW NIBBLE MODES CAN BE MIXED ! */
#define DPLNORM    0x00     /* normal output: easily set new pixel */
#define DPLINVERS  0x01     /* invers output: set inverted new pixel */
#define DPLUNDERL  0x02     /* underline output: set new pixel AND set lowest pixel to underline */
#define DPLMODE8   0x04     /* tbd. */
                            /* THESE HIGH NIBBLE MODES CAN NOT BE MIXED ! */
#define DPLXOR     0x08     /* read back display data and XOR  with new data */
#define DPLOR      0x10     /* read back display data and OR   with new data */
#define DPLAND     0x20     /* read back display data and AND  with new data */
#define DPLNAND    0x40     /* read back display data and NAND with new data */
#define DPLNOR     0x80     /* read back display data and NOR  with new data */



/* display fonts */
typedef enum
{
    DPLFONT_NOFONT,     /* invalid font */
    DPLFONT_4X6,        /* only uppercase char without special chars */
    DPLFONT_6X8,        /* full featured font */
    DPLFONT_14X16,      /* numbers only */
    DPLFONT_8X16,       /* full featured font */
    DPLFONT_24X32,      /* numbers only */
    DPLFONT_30X56,      /* not yet implemented - numbers only */
} DPLFONT;



/* general defines */
#define UNDERLINE   0x80    /* pattern to set lowest bitmap pixel part */
#define THICKMAX    8       /* max thickness of a drawed line */



/* Bitmap resource struct */
typedef struct
{
    UINT16 wWidth;                      /* width in pixels */
    UINT16 wHeight;                     /* height in pixels */
    unsigned char far * fpucBitmap;     /* address of bitmap raw data */
} BITMAP;

/* struct to contain x/y coordinates */
/* (0,0 is upper left corner) */
typedef struct
{
    UINT16 wXPos;     /* horizontal grafic position in pixel */
    UINT16 wYPos;     /* vertical grafic position in pixel */
} DISPLXY;


/* display status flags
    - used to reconstruct display setting after power up */
typedef union
{
    UINT16 word;
    struct
    {
        unsigned char   ContrLev:8;     /* LOWBYTE:  lcd contrast level (valid 0..255) */
        unsigned char   BacklLev:3;     /* HIGHBYTE: lcd backlight level (valid 0..5) */
        unsigned char   fBackl:1;       /* HIGHBYTE: backlight switch ON/OFF flag */
        unsigned char   fDplInvers:1;   /* HIGHBYTE: lcd invers mode switch ON/OFF flag */
        unsigned char   ResB:3;         /* HIGHBYTE: currently unused nibble */
    } flags;
} DPLFLAGS_TYPE;
#define DEF_DPLFLAGS (0x0530)  /* default: Backl-Level:Max Backlight:OFF, Invers:OFF, Contrast 0x30 */



/* exported function prototypes */
ERRCODE DisplInit(void);

void DisplClearScreen(const unsigned char ucPattern);

DISPLXY DisplGetDimension(const DPLFONT eFont);

ERRCODE DisplPrintABitmap(  const BITMAP far * pBitmap,
                            const DISPLXY far * pCoord,
                            const unsigned char ucMode );

ERRCODE DisplPrintACharacter(  const BITMAP far * pChar,
                            const DISPLXY far * pCoord,
                            const unsigned char ucMode);

ERRCODE DisplPrintAString(     const unsigned char far * szString,
                            const DISPLXY far * pCoord,
                            const DPLFONT Font,
                            const unsigned char ucMode );

ERRCODE DisplDrawHorLine(   const DISPLXY far * pCoord,
                            const UINT16 wLength,
                            const UINT8 bPattern);

#endif /* _DISPLAY_H */
