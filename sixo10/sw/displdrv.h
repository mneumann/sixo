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
 *  Created:        18-08-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         diplay driver
 *  Purpose:        export header
 *  Comments:       -
 *  Changes:        added control of dummy read in LCDDrvReadData, AN 16.2.2003
 ************************************************************************ */
#ifndef _DISPLDRV_H
#define _DISPLDRV_H

/* redefine of port register of sfr62.h for readability */
#define ucADBus          p1      // seiko & lcd common address / data bus on port1
#define ucADBusD         pd1     // direction register of port1
#define ucLCDCtrl        p0      // lcd display control bus register on port4
#define ucLCDCtrlD       pd0     // direction register of port4

/* redefine of sfr62.h for readability (D-A conversion) */
#define BlightLvl   da0   // D-A output for backlight brightness control
#define BlightLvlD  pd9_3 // direction of port 9 bit 3
#define ContrLvl    da1   // D-A output for contrast level control
#define ContrLvlD   pd9_4 // direction of port 9 bit 4

/* hd61202 control bytes for instruction writes */
#define DISPLON    (unsigned char) 0x3f    /* display on */
#define DISPLOFF   (unsigned char) 0x3e    /* display off */
#define SETSTARTL  (unsigned char) 0xc0    /* y start line read pointer 0..63 to OR'd into */
#define SETYPAGE   (unsigned char) 0xb8    /* y page number 0..6 to be OR'd into */
#define SETXADDR   (unsigned char) 0x40    /* x start adress 0..63 to be OR'd into */

/* hd61202 status bits after status read */
#define BUSY_TIMEOUT (unsigned char) 0xff  /* BUSY not cleared in 255 loops! */
#define STAT_BUSY  (unsigned char) 0x80    /* display controller still busy */
#define STAT_OFF   (unsigned char) 0x20    /* display controller is OFF */
#define STAT_RST   (unsigned char) 0x10    /* display controller is in reset state */
#define STAT_OK    (unsigned char) 0x00    /* display controller is in normal mode */

/* pin usage to control hd61202 lcd controller bus & gate 245 */
#define PIN_BENBL  (unsigned char) 0x01    /* LCD backlight enable (low:on) */
#define PIN_CENBL  (unsigned char) 0x02    /* LCD contrast voltage enable (high:on) */
#define PIN_ENBL   (unsigned char) 0x04    /* hd 61202 'Enable' pin */
#define PIN_DI     (unsigned char) 0x08    /* hd 61202 'D/I' pin (high:data, low:instruction */
#define PIN_RW     (unsigned char) 0x10    /* hd 61202 'R/W' pin (high:read, low:write)  */
#define PIN_RL     (unsigned char) 0x20    /* hd 61202 'R/L' pin (high:right, low:left) */
#define PIN_GATE   (unsigned char) 0x40    /* gate 245 enable pin */

#define CLR_GDRE   (unsigned char) 0xa3    /* clear gate/DI/RW/E of lcd control byte */

/* display CONTROLLER specifications */
#define Y_MAX_CONTR     (unsigned char) 56          /* controller memory y-pixel capability */
#define X_MAX_CONTR     (unsigned char) 64          /* controller memory x-pixel capability */
#define PAGE_HEIGHT     (unsigned char) 8           /* controller page height in pixel */
#define PAGE_MAX        (unsigned char)(Y_MAX_CONTR / PAGE_HEIGHT)   /* controller y-page capability */
#define PAGE_COLUMN_MAX (unsigned char)(X_MAX_CONTR)                 /* controller x-pixel capability */

/* display MODULE specifications */
#define N_CONTR_H       (unsigned char) 2           /* number of horizontal display controller */
#define N_CONTR_V       (unsigned char) 1           /* number of horizontal display controller */
#define X_MAX_DISPL     (unsigned char)(X_MAX_CONTR * N_CONTR_H)     /* display x-pixel capability */
#define Y_MAX_DISPL     (unsigned char)(Y_MAX_CONTR * N_CONTR_V)     /* display y-pixel capability */

/* test & dignostics */
#define TESTPATTERN 0xaa                    /* for lcd controller test */


/* prototypes */
ERRCODE LCDDrvCheckController(void);
unsigned char LCDDrvCheckStatus(void);
UINT8 LCDDrvWaitBusy(void);
ERRCODE LCDDrvSetSide(const BOOL bSide);
ERRCODE LCDDrvInitLCD(void);
ERRCODE LCDDrvReset(void);
ERRCODE LCDDrvSetVisible( const BOOL bMode);
ERRCODE LCDDrvSetColumn (const unsigned char ucColumn );
ERRCODE LCDDrvSetPage( const unsigned char ucPage );
ERRCODE LCDDrvSetStartLine( const unsigned char ucStartLine );
ERRCODE LCDDrvFillPattern(const unsigned char ucPattern);
ERRCODE LCDDrvSetPixel( const unsigned char ucPixel);
ERRCODE LCDDrvGetPixel( unsigned char * pucPixel, const BOOL bMode );
ERRCODE LCDDrvSetContrast(const BOOL fContrastOn);
ERRCODE LCDDrvSetContrastLevel(const UINT8 bContrast);
ERRCODE LCDDrvSetBacklight(const BOOL fLightOn);
ERRCODE LCDDrvSetBacklightLevel(const UINT8 bBrightness);

/* non exported module functions: */
/*
ERRCODE LCDDrvReadInstr( unsigned char * pucValue );
ERRCODE LCDDrvWriteInstr( const unsigned char ucValue );
ERRCODE LCDDrvWriteData( const unsigned char ucValue );
ERRCODE LCDDrvReadData( unsigned char * pucValue, const BOOL bMode );
unsigned char LCDDrvWaitBusy(void);
unsigned int LCDDrvTestBusyFlag( const unsigned char ucData );
*/

#endif // _DISPLDRV_H

