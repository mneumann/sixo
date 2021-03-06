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
 *  Created:        25-10-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         display
 *  Purpose:        bitmap raw data resources
 *  Comments:
 *                  Currently a 128x56 (WxH) pixel display is supported.
 *                  This results in 7 pages a 8bit height. (=56 pixel H)
 *
 *                  All bitmaps orientation is horizontal,
 *                  divided into vertical 8 bit stripes
 *                  to support lcds hor. pages
 *
 *                  No segmentation into left/right
 *                  display half is done here.
 *
 *      SPECIAL LOGO DEFINES:   - LOGO_NKDEV        SIxO-Logo with N&K Development
 *                              - LOGO_BMW          SIxO-Logo with BMW Logo
 *                              - LOGO_HUSQVARNA    SIxO-Logo with Husqvarna Logo
 *                              default             SIxO-Logo
 *
 ************************************************************************ */

#include "bitmaps.h"



#ifdef LOGO_NKDEV

/* SIxO Logo:   |      SIxO       | */
/*              | N&K Development | */
/* Dimension: 128x56 (wxh)          */
const unsigned char rgSIxOLogo128x56[]={
0x00,0x00,0x00,0x00,0xc0,0xf0,0xf8,0xfc,	/* Upper 8bit Logo stripe */
0xfc,0xfe,0xfe,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xfe,0xfe,0xfe,0xfc,0xf8,0xf0,
0xe0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
0xfe,0xfe,0x00,0x00,0x00,0x00,0x02,0x06,
0x0e,0x3e,0x7e,0xfe,0xfe,0xfe,0xfe,0xfe,
0xfe,0xfc,0xf8,0xe0,0xc0,0x80,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xe0,
0xf0,0xfc,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
0x7e,0x3e,0x0e,0x06,0x02,0x00,0x00,0x00,
0x00,0x80,0xc0,0xe0,0xf0,0xf0,0xf8,0xfc,
0xfc,0xfe,0xfe,0xfe,0xfe,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xfe,
0xfe,0xfc,0xfc,0xf8,0xf8,0xf0,0xe0,0xc0,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x7f,0xff,0xff,0xff,0xff,	/* 2nd 8bit Logo stripe */
0xff,0xff,0xff,0xff,0xc3,0x81,0x81,0x81,
0x01,0x07,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0x0f,0x0f,0x0c,0x00,0x00,0x00,0x00,0x00,
0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x03,0x07,0x0f,0x3f,
0x7f,0xff,0xff,0xff,0xff,0xff,0xfe,0xfc,
0xf8,0xe0,0xc0,0x00,0x00,0x03,0x07,0x1f,
0x3f,0xff,0x7f,0x3f,0x1f,0x07,0x03,0x00,
0x00,0x00,0x00,0x00,0x00,0xc0,0xf8,0xfc,
0xff,0xff,0xff,0xff,0xff,0xff,0x7f,0x3f,
0x0f,0x0f,0x07,0x03,0x03,0x03,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x03,0x03,0x07,
0x07,0x0f,0x1f,0x7f,0xff,0xff,0xff,0xff,
0xff,0xff,0xfe,0xf8,0xe0,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x03,0x07,0x0f,0x1f,	/* 3rd 8bit Logo stripe */
0x3f,0x3f,0x7f,0x7f,0x3f,0x0f,0x03,0x00,
0x00,0x80,0xe0,0xf8,0xfe,0xfe,0xfc,0xf8,
0xf8,0xf0,0xe0,0x80,0x00,0x00,0x00,0x00,
0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x03,0x07,0x0f,0x3f,0x7f,0xff,
0xff,0xff,0xff,0xff,0xfe,0xfc,0xf0,0xe0,
0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x86,0x87,0x87,0x87,
0x87,0x87,0x87,0x87,0x87,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x87,0x87,0x87,
0x87,0x87,0x87,0x87,0x87,0x87,0x00,0x00,
0x00,0x00,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,    /* 4th 8bit Logo stripe */
0xe0,0xe0,0xe0,0x80,0x00,0x00,0x00,0x00,
0x00,0x01,0x03,0x87,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xfe,0x00,0x00,0x00,
0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x80,0xe0,0xf0,0xf8,
0xff,0xfe,0xfc,0xf8,0xe0,0xc0,0x80,0x01,
0x03,0x07,0x1f,0x3f,0x7f,0xff,0xff,0xff,
0xff,0xff,0xfe,0xfc,0xf0,0xe0,0xc0,0x00,
0x00,0x00,0x00,0x00,0x01,0x0f,0x7f,0xff,
0xff,0xff,0xff,0xff,0xff,0xfe,0xf8,0xf0,
0xc0,0xc0,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
0x80,0xc0,0xe0,0xf8,0xfc,0xff,0xff,0xff,
0xff,0xff,0xff,0x7f,0x1f,0x03,0x00,0x00,
0x00,0x00,0x00,0x07,0x1f,0x3f,0x7f,0x7f,    /* 5th 8bit Logo stripe */
0xff,0xff,0xff,0xff,0xff,0xfe,0xfe,0xfe,
0xfe,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,
0x7f,0x3f,0x1f,0x07,0x01,0x00,0x00,0x00,
0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0x00,0x00,0x00,0x00,0x80,0xe0,
0xf0,0xf8,0xfe,0xff,0xff,0xff,0xff,0xff,
0xff,0x3f,0x1f,0x0f,0x03,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,
0x1f,0x3f,0x7f,0xff,0xff,0xff,0xff,0xff,
0xfe,0xf8,0xf0,0xe0,0x80,0x00,0x00,0x00,
0x03,0x07,0x0f,0x1f,0x3f,0x3f,0x7f,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xfe,
0xfe,0xfe,0xfe,0xfe,0xfe,0xff,0xff,0xff,
0xff,0xff,0xff,0x7f,0x7f,0x3f,0x1f,0x0f,
0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xf8,0x08,    /* 6th 8bit Logo stripe */
0x18,0x79,0xe1,0x81,0x03,0x03,0x03,0xfb,
0x03,0x03,0x03,0x01,0x01,0x01,0x00,0xf8,
0x08,0x08,0x08,0xc8,0x00,0x00,0x00,0x00,
0x00,0x01,0xf9,0x01,0x01,0x01,0x01,0xf9,
0x01,0x01,0x00,0x00,0x00,0x01,0x01,0xf9,
0x09,0x09,0x09,0x09,0x09,0x09,0x09,0xf9,
0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,
0x80,0x80,0x00,0x00,0x00,0x80,0x00,0x80,
0x80,0x80,0x80,0x81,0x81,0x01,0xf9,0x01,
0x01,0x81,0x81,0x81,0x81,0x81,0x80,0x00,
0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,
0x00,0x81,0x81,0x81,0x81,0x83,0x83,0x83,
0x83,0x83,0x03,0x83,0x83,0x83,0x81,0x81,
0x81,0x00,0x80,0x80,0x80,0x80,0x80,0x00,
0x80,0xc0,0x80,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,    /* 7th 8bit Logo stripe */
0x3e,0x00,0x01,0x07,0x1e,0x38,0x20,0x3f,
0x00,0x00,0x00,0x00,0x00,0x3e,0x3f,0x3f,
0x21,0x21,0x21,0x3f,0x01,0x01,0x00,0x00,
0x00,0x00,0x3f,0x3f,0x3f,0x01,0x01,0x01,
0x01,0x3f,0x00,0x00,0x00,0x00,0x00,0x3f,
0x3f,0x3e,0x20,0x20,0x20,0x20,0x20,0x3f,
0x00,0x3f,0x3f,0x24,0x24,0x24,0x37,0x00,
0x07,0x3f,0x20,0x20,0x38,0x07,0x00,0x3f,
0x3f,0x24,0x24,0x24,0x37,0x00,0x3f,0x3c,
0x00,0x3f,0x20,0x20,0x20,0x20,0x3f,0x00,
0x00,0x00,0xff,0xe0,0x20,0x20,0x3f,0x00,
0x00,0x3f,0x3c,0x00,0x00,0x3f,0x00,0x00,
0x00,0x3f,0x00,0x3f,0x3f,0x24,0x24,0x24,
0x37,0x00,0x3f,0x3c,0x00,0x00,0x3f,0x00,
0x00,0x3f,0x21,0x21,0x00,0x00,0x00,0x00
}; /* of SIXO-Logo */






#elif defined LOGO_BMW

/* SIxO Logo:   |      SIxO       | */
/*              |BMW-Logo         | */
/* Dimension: 128x56 (wxh)          */
const unsigned char rgSIxOLogo128x56[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
0xC0,0x60,0x60,0x70,0xF8,0xF8,0xFC,0xFC,0xFC,0xFE,
0xFE,0xFE,0xFF,0xFF,0x01,0x01,0xE3,0x0F,0x0F,0xE3,
0x01,0x01,0xFF,0xFF,0xFE,0xFE,0xFE,0xFC,0xFC,0xFC,
0x38,0x18,0x90,0xE0,0xE0,0xC0,0x80,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xE0,0xF8,0xFC,0xFE,0xFE,0x1E,0x1E,0x1E,0x7E,
0x7E,0x7C,0x78,0x60,0x00,0x00,0x00,0xFE,0xFE,0xFE,
0xFE,0xFE,0x00,0x00,0x02,0x06,0x0E,0x3E,0x7E,0xFE,
0xFC,0xF0,0xE0,0xC0,0x80,0x00,0x20,0x78,0xFC,0xFE,
0x3E,0x1E,0x0E,0x06,0x82,0xE0,0xF0,0xF8,0xFC,0xFC,
0xFC,0x7E,0x3E,0x3E,0x1E,0x1E,0x1E,0x3E,0x3E,0x3E,
0x7C,0xFC,0xF8,0xF0,0xE0,0x80,0x00,0x00,
0x00,0x00,0x00,0xC0,0xE0,0xF8,0xFC,0xFE,0xF3,0xE1,
0xC4,0x8E,0x06,0x30,0x78,0x3B,0x83,0xC7,0x7F,0xBF,
0x7F,0x9F,0x5F,0xBF,0x4C,0xAC,0x4F,0xBF,0x0F,0x0F,
0x0C,0x0C,0x1F,0x1F,0x1F,0x3F,0x3F,0x77,0xE3,0xE0,
0xE4,0xEF,0x07,0x03,0x73,0x3F,0x1F,0x9F,0xC6,0xE4,
0xF8,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x01,0x07,0x0F,0x0F,0x1F,0x0E,0x02,0x20,0x30,
0xFC,0xFC,0xF8,0xF0,0xC0,0x00,0x00,0xFF,0xFF,0xFF,
0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE1,
0xC3,0x07,0x1F,0x3F,0x7F,0xFE,0xFC,0xF0,0xE1,0xC0,
0x00,0x00,0x00,0x00,0xE7,0xE7,0xE7,0xE7,0xE7,0x81,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xC1,0xE7,0xE7,0xE7,0xE7,0x00,0x00,
0xC0,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x9E,0x56,0xAB,0x55,0xAA,0x55,0xAA,
0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x03,0x07,0x1E,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xF8,0xC0,0x00,0x00,0x00,0x00,
0x00,0x07,0x1F,0x1F,0x3F,0x3E,0x7C,0x7C,0x7C,0x7C,
0x3F,0x3F,0x1F,0x0F,0x03,0x00,0x00,0x3F,0x3F,0x3F,
0x3F,0x3F,0x00,0x00,0x30,0x38,0x3C,0x3E,0x3F,0x3F,
0x0F,0x07,0x02,0x00,0x00,0x01,0x03,0x07,0x1F,0x3F,
0x3F,0x3C,0x3C,0x30,0x20,0x03,0x07,0x0F,0x1F,0x1F,
0x3F,0x3E,0x3C,0x7C,0x7C,0x7C,0x7C,0x7C,0x3E,0x3E,
0x3F,0x1F,0x0F,0x07,0x03,0x00,0x00,0x00,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x04,0x02,0x05,0x02,0x05,0x02,0x05,0x02,
0x05,0x02,0x05,0x02,0x05,0x02,0x05,0x02,0xA8,0x50,
0xA8,0x50,0xA8,0x50,0xA8,0x50,0xA8,0x50,0xA8,0x50,
0xA8,0x50,0xA8,0x10,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x03,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xF8,0xE0,0xC0,0x80,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xAA,0x55,
0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0x2A,0xD5,
0xCA,0xE5,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x1F,0x03,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0x07,0x1F,0x3F,0x7F,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,
0xFC,0xF8,0xF8,0xF8,0xF0,0xF0,0xF0,0xF0,0xFA,0xF5,
0xF2,0xF5,0xFA,0xFD,0xFA,0xFD,0xFC,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x3F,
0x1F,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x03,0x07,0x07,0x0F,0x1F,0x1F,0x3F,0x3F,0x3F,0x7F,
0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,0x3F,0x3F,0x3F,
0x1F,0x1F,0x0F,0x07,0x07,0x03,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};



#elif defined LOGO_HUSQVARNA

/* SIxO Logo:   |      SIxO       | */
/*              |Husqvarna-Logo   | */
/* Dimension: 128x56 (wxh)          */
const unsigned char rgSIxOLogo128x56[]={
0x70,0xF0,0xF8,0xF8,0xF8,0xF8,0xF8,0xFC,0xFC,0xFC,
0xFC,0xFE,0xFE,0x80,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x80,0x80,0xC1,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xC1,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0x80,0xFE,0xFE,0xFC,0xFC,0xFC,0xF8,
0xF8,0xF8,0xF8,0xF8,0xF8,0xF0,0xF0,0x70,0x00,0x00,
0x00,0x70,0xF8,0xFE,0xFE,0x9E,0x8E,0x8E,0x3E,0x3E,
0x3C,0x38,0x20,0x00,0x00,0xFE,0xFE,0xFE,0xFE,0x00,
0x00,0x00,0x06,0x0E,0x3E,0x7E,0xFE,0xF8,0xF0,0xE0,
0x00,0x20,0x78,0xFE,0x3E,0x1E,0x0E,0x02,0xC0,0xF0,
0xF8,0xFC,0xFC,0x7E,0x3E,0x1E,0x0E,0x0E,0x0E,0x1E,
0x1E,0x3E,0x7E,0xFC,0xF8,0xF0,0xC0,0x00,
0x00,0x01,0x0F,0x9F,0xDF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7E,0x7E,0x3E,
0x3E,0x3E,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
0x3F,0x3F,0x3F,0x3F,0x3F,0x3E,0x3E,0x3E,0x7E,0x7F,
0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xDF,0x9F,0x0F,0x07,0x01,0x00,0x00,0x00,
0x00,0xE0,0xE0,0xE3,0xE7,0x87,0x03,0x08,0x8E,0xFF,
0xFE,0xFE,0xFC,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,
0x00,0x00,0x00,0x80,0xE0,0xF8,0xF0,0xE3,0x07,0x0F,
0x3F,0x7F,0xFC,0xF8,0xE0,0xC0,0x80,0x00,0x39,0xF9,
0xF9,0xF9,0xF0,0xE0,0x80,0x80,0x00,0x00,0x00,0x80,
0x80,0xC0,0xE0,0xF9,0xF9,0xF9,0x39,0x00,
0xF8,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0x1F,0x07,0x01,0x01,0x00,0x00,0xFE,0xFE,0xFE,
0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0x00,
0x00,0x00,0x01,0x03,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xF8,0xF0,0xC0,0x00,
0x00,0x00,0x03,0x03,0x07,0x0F,0x0F,0x0F,0x0F,0x07,
0x07,0x03,0x03,0x00,0x00,0x07,0x07,0x07,0x07,0x00,
0x00,0x04,0x07,0x07,0x07,0x07,0x03,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0x07,0x07,0x07,0x06,0x04,0x00,
0x01,0x01,0x03,0x07,0x07,0x0F,0x0F,0x0F,0x0F,0x0F,
0x07,0x07,0x03,0x03,0x00,0x00,0x00,0x00,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x03,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0xF0,0xF0,0xF0,
0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,
0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0x03,0x03,0x03,
0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x07,0x1F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFE,0xF8,0xE0,0xC0,0xC0,0x80,0x3F,0x3F,0x3F,
0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x80,
0x80,0xC0,0xE0,0xF0,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,0x07,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x0F,0x1F,
0x1F,0x1F,0x3F,0x3F,0x3F,0x7F,0x7F,0x7F,0x7F,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFE,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,
0x7F,0x7F,0x3F,0x3F,0x3F,0x1F,0x1F,0x0F,0x0F,0x0F,
0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


#else

/* SIxO Logo:   |      SIxO       | */
/*              |                 | */
/* Dimension: 128x48 (wxh)          */
const unsigned char rgSIxOLogo128x56[]={
0x00,0x00,0x00,0x00,0xC0,0xF0,0xF8,0xFC,0xFC,0xFE,
0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFE,
0xFE,0xFC,0xF8,0xF0,0xE0,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,
0xFE,0xFE,0x00,0x00,0x00,0x00,0x02,0x06,0x0E,0x3E,
0x7E,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFC,0xF8,0xE0,
0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xC0,0xE0,0xF0,0xFC,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,
0x7E,0x3E,0x0E,0x06,0x02,0x00,0x00,0x00,0x00,0x80,
0xC0,0xE0,0xF0,0xF0,0xF8,0xFC,0xFC,0xFE,0xFE,0xFE,
0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFE,0xFE,0xFE,0xFC,0xFC,0xF8,0xF8,0xF0,0xE0,0xC0,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xC3,0x81,0x81,0x81,0x01,0x07,0x0F,0x0F,
0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0C,0x00,0x00,0x00,
0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x03,0x07,0x0F,0x3F,0x7F,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFE,0xFC,0xF8,0xE0,0xC0,0x00,0x00,0x03,
0x07,0x1F,0x3F,0xFF,0x7F,0x3F,0x1F,0x07,0x03,0x00,
0x00,0x00,0x00,0x00,0x00,0xC0,0xF8,0xFC,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x7F,0x3F,0x0F,0x0F,0x07,0x03,
0x03,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x03,
0x03,0x07,0x07,0x0F,0x1F,0x7F,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFE,0xF8,0xE0,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x03,0x07,0x0F,0x1F,0x3F,0x3F,
0x7F,0x7F,0x3F,0x0F,0x03,0x00,0x00,0x80,0xE0,0xF8,
0xFE,0xFE,0xFC,0xF8,0xF8,0xF0,0xE0,0x80,0x00,0x00,
0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,
0x0F,0x3F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,
0xF0,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x86,0x87,0x87,0x87,0x87,0x87,
0x87,0x87,0x87,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x87,0x87,0x87,
0x87,0x87,0x87,0x87,0x87,0x87,0x00,0x00,
0x00,0x00,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
0xE0,0x80,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x87,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,
0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0xE0,0xF0,0xF8,0xFF,0xFE,0xFC,0xF8,
0xE0,0xC0,0x80,0x01,0x03,0x07,0x1F,0x3F,0x7F,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,0xF0,0xE0,0xC0,0x00,
0x00,0x00,0x00,0x00,0x01,0x0F,0x7F,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFE,0xF8,0xF0,0xC0,0xC0,0x80,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x80,0x80,0xC0,0xE0,0xF8,0xFC,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x7F,0x1F,0x03,0x00,0x00,
0x00,0x00,0x00,0x07,0x1F,0x3F,0x7F,0x7F,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFE,0xFE,0xFE,0xFE,0xFE,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x7F,0x3F,0x1F,0x07,0x01,0x00,
0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x00,0x00,0x00,0x00,0x80,0xE0,0xF0,0xF8,
0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x1F,0x0F,
0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x03,0x07,0x1F,0x3F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFE,0xF8,0xF0,0xE0,0x80,0x00,0x00,0x00,0x03,0x07,
0x0F,0x1F,0x3F,0x3F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x3F,0x1F,0x0F,
0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,
0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,
0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


#endif // LOGO_x




/* Enduro-Symbol */
/* Dimension: 29x16 (wxh)          */
const unsigned char rgEnduroSymbol29x16[]={
0x00,0x88,0xCC,0xC4,0x66,0x66,0xC6,0xEC,0xBE,0x1F,
0x37,0x7B,0xFE,0xFE,0xFC,0xF8,0xF0,0xF0,0xF0,0x38,
0x98,0xCC,0xCC,0x66,0x66,0xC6,0xC6,0x82,0x00,
0x1F,0x3B,0x60,0x60,0xCC,0xCE,0x63,0x60,0x3B,0x1F,
0x00,0x00,0x03,0x07,0x07,0x07,0x07,0x07,0x06,0x1F,
0x3F,0x66,0x64,0xCC,0xCC,0x60,0x60,0x3B,0x1F
};

/* Fuel-Symbol */
/* Dimension: 16x16 (wxh)          */
const unsigned char rgFuelSymbo16x16[]={
0x00,0xFF,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xFF,
0x40,0x80,0x01,0x0E,0xFC,0x18,
0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x80,0x3F,0x40,0x40,0x47,0x38
};



