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
 *  Created:        06-08-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         lcd driver
 *  Purpose:        low level lcd display driver functions
 *                  for HD61202 with 128x56 pixel b/w grafic display
 *  Comments:
 *                  MOST OF THESE FUNCTIONS AFFECT ON
 *                  ONLY 1 DISPLAY HALF (left/right)
 *
 *                  Use LCDDrvSetSide(BOOL bSide) to switch active
 *                  controller from LEFT to RIGHT (vs)!
 *
 *  Changes:        added control of dummy read in LCDDrvReadData and
 *                  LCDDrvGetPixel, AN 16.2.2003
 ************************************************************************ */


/* includes */
#include "sfr62.h"
#include "standard.h"
#include "leddrv.h"
#include "displdrv.h"
#include "timer.h"




/*****************************************************************
    EL backlight look up table according to:
    v = 0.2 ln(256-n) + 2  <=>  n = 256 - exp(5v-10) */
static unsigned char BlightValues[]=
{
   0, 36, 66, 93,116,135,152,166,   /* 0 eq. maximum brightness */
 179,190,199,207,214,220,225,229,
 233,236,239,241,243,245,246,247,
 248,249,250,251,252,253,254,255    /* 255 eq. minimum brightness */
};

/*****************************************************************
   reduced backlight lookuptable with not inverted index */
static unsigned char rgBlightSteps[]=
{
   255,247,229,166,0
};
#define BLL_MAX     (UINT8)(sizeof(rgBlightSteps)-1)
#define BLL_MIN     0


/* global flag to disable and ignore all lcd outputs
   if lcd error detected */
BOOL gfLCD_Ok = TRUE;          /* default: ok! */


/***********************************************************************
 * FUNCTION:    LCDDrvSetSide
 * DESCRIPTION: Gives control to left or right display controller
 * PARAMETER:   LEFT / RIGHT
 * RETURN:      ok
 * COMMENT:     changed to correct implementation for SIxO pinout
 *********************************************************************** */
ERRCODE LCDDrvSetSide(const BOOL bSide)
{
    if ( !gfLCD_Ok ) return ERR_OK;     /* LCD may be disconnected */
    if( bSide == LEFT )
    {
        ucLCDCtrl &= (~PIN_RL);
    }
    else
    {
        ucLCDCtrl |= PIN_RL;
    }
    return (ERR_OK);
}


/***********************************************************************
 * FUNCTION:    LCDDrvReset
 * DESCRIPTION: Hardware Reset for LCD controller
 * PARAMETER:   -
 * RETURN:      Ok
 * COMMENT:     TBD: Toggle at the display reset line
 *********************************************************************** */
ERRCODE LCDDrvReset(void)
{
    /* not yet implemented */
    if ( !gfLCD_Ok ) return ERR_OK;     /* LCD may be disconnected */

    return (ERR_OK);
}


/***********************************************************************
 *  FUNCTION:       LCDDrvSetContrast
 *  DESCRIPTION:    Sets the lcd module(!) contrast on/off
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LCDDrvSetContrast(const BOOL fContrastOn)
{
    if ( !gfLCD_Ok ) return ERR_OK;     /* LCD may be disconnected */

    ucLCDCtrl = (fContrastOn == TRUE) ? (ucLCDCtrl | PIN_CENBL) : (ucLCDCtrl & (~PIN_CENBL));
    return ERR_OK;
}

/***********************************************************************
 *  FUNCTION:       LCDDrvSetContrastLevel
 *  DESCRIPTION:    Sets the lcd module contrast level
 *
 *  PARAMETER:      level       from 0=light .. 255=dark
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LCDDrvSetContrastLevel(const UINT8 bContrast)
{
    if ( !gfLCD_Ok ) return ERR_OK;     /* LCD may be disconnected */

    ContrLvl = bContrast;
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       LCDDrvSetBacklight
 *  DESCRIPTION:    Sets the lcd module(!) backlight on/off
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LCDDrvSetBacklight(const BOOL fLightOn)
{
    if ( !gfLCD_Ok ) return ERR_OK;     /* LCD may be disconnected */

    ucLCDCtrl = (fLightOn == TRUE) ? (ucLCDCtrl & (~PIN_BENBL)) : (ucLCDCtrl | PIN_BENBL);
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       LCDDrvSetBacklightLevel
 *  DESCRIPTION:    Sets the lcd module backlight brightness
 *                  from 0=dark .. 5=bright
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LCDDrvSetBacklightLevel(const UINT8 bBrightness)
{
    if ( !gfLCD_Ok ) return ERR_OK;     /* LCD may be disconnected */

    /* check parameters */
    if (bBrightness > BLL_MAX)
        return ERR_PARAM_ERR;

    /* map value to Arnolds LookUpTable */
    BlightLvl = rgBlightSteps[bBrightness];

    return ERR_OK;
}



/***********************************************************************
 * FUNCTION:    LCDDrvReadInstr
 * DESCRIPTION: reads back the lcd controllers status byte
 * PARAMETER:   UCHAR * pucValue    R-Value status byte
 * RETURN:      ERR_OK
 * COMMENT:     -
 *********************************************************************** */
ERRCODE LCDDrvReadInstr( unsigned char * pucValue )
{
    if ( !gfLCD_Ok ) return ERR_OK;         /* LCD may be disconnected */

    ucADBusD    =  PORT_IN;                 /* set m16c ports to INPUT */
    ucLCDCtrl   &= CLR_GDRE;                /* clear D/I, R/W, E, and Gate245, !!! t(as)min = 140 ns between Set(R/W) <-> Set(E) */
    ucLCDCtrl   |= PIN_RW;                  /* Set R/W to READ (while waiting) */

    ucLCDCtrl   |= PIN_ENBL;                /* [200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB) */
    *pucValue  = ucADBus;                   /* [200 ns] save current state before disable E */
    ucLCDCtrl   &= (~PIN_ENBL);             /* [200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high! */

    ucLCDCtrl   &= (~PIN_RW);               /* RW=low=write */
    ucADBusD    = PORT_OUT;                 /* set m16c ports back to OUTPUT */
    return(ERR_OK);
}

/***********************************************************************
 * FUNCTION:    LCDDrvCheckStatus
 * DESCRIPTION: Reads back current lcd controller status
 * PARAMETER:   -
 * RETURN:      lcd status byte with BUSY, RESET and ON/OFF flags
 * COMMENT:     -
 *********************************************************************** */
UINT8 LCDDrvCheckStatus(void)
{
    unsigned char    ucStatus;
    unsigned char   RValue = (unsigned char) 0x0;

    if ( !gfLCD_Ok ) return ERR_OK;         /* LCD may be disconnected */

    /* get current status */
    LCDDrvReadInstr( &ucStatus );

    /* just for debug purpose */
    if (ucStatus & STAT_BUSY)   /* display controller still busy */
        RValue |= STAT_BUSY;
    if (ucStatus & STAT_OFF)    /* display controller is OFF */
        RValue |= STAT_OFF;
    if (ucStatus & STAT_RST)    /* display controller is in reset state */
        RValue |= STAT_RST;
    if (ucStatus & STAT_OK)     /* display controller is in normal mode */
        RValue |= STAT_OK;

    /* give back status */
    return( RValue );
}


/***********************************************************************
 * FUNCTION:    LCDDrvWaitBusy
 * DESCRIPTION: stays in loop until controller BUSY flag is cleared
 * PARAMETER:   -
 * RETURN:      LCD Status
 * COMMENT:     NOTE1: Values in [ ] are µC instruction times
 *              NOTE2: THIS CODE IS ASSEMBLER FINE TUNED!
 *                     Don't change any order of instructions!
 *********************************************************************** */
UINT8 LCDDrvWaitBusy(void)
{
    UINT8 bLoops     = 0;
    UINT8 bLCDStatus = STAT_BUSY;

    if ( !gfLCD_Ok ) return 0;              /* LCD may be disconnected */

    ucADBusD    =  PORT_IN;                 /* set m16c ports to INPUT */
    ucLCDCtrl   &= CLR_GDRE;                /* clear D/I, R/W, E, and Gate245, !!! t(as)min = 140 ns between Set(R/W) <-> Set(E) */
    ucLCDCtrl   |= PIN_RW;                  /* Set R/W to READ (while waiting) */

    do
    {
        ucLCDCtrl   |= PIN_ENBL;            /* [200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB) */
        bLoops++;                           /* [300 ns] increment our wait counter (while waiting) */
        bLCDStatus  = ucADBus;              /* [200 ns] save current state before disable E */
        ucLCDCtrl   &= (~PIN_ENBL);         /* [200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high! */
    }                                       /* [200 ns] jump back to loop begin  */
    while (bLCDStatus & STAT_BUSY);         /* [300 ns] check BUSY flag */

    ucLCDCtrl   &= (~PIN_RW);               /* RW=low=write */
    ucADBusD    = PORT_OUT;                 /* set m16c ports back to OUTPUT */
    return( bLCDStatus );
}

/***********************************************************************
 * FUNCTION:    LCDDrvWriteInstr
 * DESCRIPTION: Writes an instruction to lcd controller
 * PARAMETER:   unsigned char ucInstr
 * RETURN:      Ok
 * COMMENT:     -
 *********************************************************************** */
ERRCODE LCDDrvWriteInstr( const unsigned char ucInstr )
{
    UINT8 bLCDStatus;

    if ( !gfLCD_Ok ) return ERR_OK;         /* LCD may be disconnected */

    /* init */
    ucADBusD    =  PORT_IN;                 /* set m16c port to INPUT */
    ucLCDCtrl   &= CLR_GDRE;                /* clear D/I, R/W, E, and Gate245, [t(as)min = 140 ns between Set(R/W) <-> Set(E)] */
    ucLCDCtrl   |= PIN_RW;                  /* Set R/W to READ (while waiting) */

    /* wait busy */
    bLCDStatus = STAT_BUSY;                 /* [200 ns] clear state */
    do
    {
        ucLCDCtrl   |= PIN_ENBL;            /* [200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB) */
        bLCDStatus  = ucADBus;              /* [200 ns] save current state before disable E */
        ucLCDCtrl   &= (~PIN_ENBL);         /* [200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high! */
    }                                       /* [200 ns] if jump back to loop begin  */
    while (bLCDStatus & STAT_BUSY);         /* [300 ns] check BUSY flag */

    /* write instruction */
    ucLCDCtrl   &= (~PIN_RW);               /* [200 ns] Clear R/W to 'Write' (while waiting) */
    ucADBusD    =  PORT_OUT;                /* [200 ns] set m16c port to OUTPUT (while waiting) */
    ucADBus      = ucInstr;                 /* [300 ns] Set output instruction byte */
    ucLCDCtrl   |= PIN_ENBL;                /* [200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB) */
    ucLCDCtrl   &= (~PIN_ENBL);             /* [200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high! */
    return(ERR_OK);
}


/***********************************************************************
 * FUNCTION:    LCDDrvWriteData
 * DESCRIPTION: Writes 8 bit data to lcd controller
 * PARAMETER:   unsigned char ucInstr
 * RETURN:      Ok
 * COMMENT:     NOTE:   THIS CODE IS ASSEMBLER FINE TUNED!
 *                      We don't need any nops, if we use the display
 *                      instruction time for µC purpose.
 *                      But: Don't change any order of instructions!
 *********************************************************************** */
ERRCODE LCDDrvWriteData( const unsigned char ucData )
{
    UINT8 bLCDStatus;

    if ( !gfLCD_Ok ) return ERR_OK;         /* LCD may be disconnected */

    /* init */
    ucADBusD    =  PORT_IN;                 /* set m16c port to INPUT */
    ucLCDCtrl   &= CLR_GDRE;                /* clear D/I, R/W, E, and Gate245, [t(as)min = 140 ns between Set(R/W) <-> Set(E)] */
    ucLCDCtrl   |= PIN_RW;                  /* Set R/W to READ (while waiting) */

    /* wait busy */
    bLCDStatus = STAT_BUSY;                 /* [200 ns] clear state */
    do
    {
        ucLCDCtrl   |= PIN_ENBL;            /* [200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB) */
        bLCDStatus  = ucADBus;              /* [200 ns] save current state before disable E */
        ucLCDCtrl   &= (~PIN_ENBL);         /* [200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high! */
    }                                       /* [200 ns] if jump back to loop begin  */
    while (bLCDStatus & STAT_BUSY);         /* [300 ns] check BUSY flag */

    /* write data */
    ucLCDCtrl   &= (~PIN_RW);               /* [200 ns] Clear R/W to 'Write' (while waiting) */
    ucADBusD    =  PORT_OUT;                /* [200 ns] set m16c port to OUTPUT */
    ucLCDCtrl   |= PIN_DI;                  /* [200 ns] Set D/I to 'Data' !!! t(as)min = 140 ns between Set(D/I) <-> Set(E) */
    ucLCDCtrl   |= PIN_ENBL;                /* [200 ns] Set Enable */
    ucADBus      = ucData;                  /* [300 ns] Set output data byte !!! t(dsw)min = 200 ns between Set(DB) <-> Clear(E) */

    // TEST: INVERS MODE?
    //ucADBus      = ~ucData;  /* invers */   /* [300 ns] Set output data byte !!! t(dsw)min = 200 ns between Set(DB) <-> Clear(E) */

    ucLCDCtrl   &= (~PIN_ENBL);             /* [200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high! */

    //Delay_ms(2); //Slow down display for debug if necessary
    return (ERR_OK);
}

/***********************************************************************
 * FUNCTION:    LCDDrvReadData
 * DESCRIPTION: Reads back data from lcd controller display memory
 * PARAMETER:   unsigned char target pointer of ucData to be read
 *              flag if a dummy read cycle must be done (DUMMY_ON/DUMMY_OFF)
 * RETURN:      Ok
 * COMMENT:     NOTE1:  We need a 'dummy read' as specified in hd61202
 *                      data sheet, to get a copy of output BUFFER into
 *                      output REGISTER if the address was set right
 *                      before the read.
 *              NOTE2:  f(max) for 'ENABLE-Pin' is 1/(450+450ns) !
 *              NOTE3:  Values in [ ] are µC instruction times
 *              NOTE4:  THIS CODE IS ASSEMBLER FINE TUNED!
 *                      We don't need any nops, if we use the display
 *                      instruction time for µC purpose.
 *                      But: Don't change any order of instructions!
 * CHANGES:     added control of the dummy read cycle for support of
 *              multiple reads without address set (sequence is address
 *              set, dummy read, read data0, read data1,...), AN 16.2.2003
 ********************************************************************** */
ERRCODE LCDDrvReadData( unsigned char * pucData, const BOOL bMode )
{
    UINT8 bLCDStatus;

    if ( !gfLCD_Ok ) return ERR_OK;         /* LCD may be disconnected */

    /* init */
    ucADBusD    =  PORT_IN;                 /* set m16c port to INPUT */
    ucLCDCtrl   &= CLR_GDRE;                /* clear D/I, R/W, E, and Gate245, [t(as)min = 140 ns between Set(R/W) <-> Set(E)] */
    ucLCDCtrl   |= PIN_RW;                  /* Set R/W to READ (while waiting) */

    /* wait busy */
    ucLCDCtrl   &= (~PIN_DI);               /* [200 ns] Set D/I to 'Instruction' */
    bLCDStatus = STAT_BUSY;                 /* [200 ns] clear state */
    do
    {
        ucLCDCtrl   |= PIN_ENBL;            /* [200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB) */
        bLCDStatus  = ucADBus;              /* [200 ns] save current state before disable E */
        ucLCDCtrl   &= (~PIN_ENBL);         /* [200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high! */
    }                                       /* [200 ns] if jump back to loop begin  */
    while (bLCDStatus & STAT_BUSY);         /* [300 ns] check BUSY flag */

    if( bMode == DUMMY_ON ){
       /* first (dummy) read */
       ucLCDCtrl   |= PIN_DI;                  /* [200 ns] Set D/I to 'Data' */
       ucLCDCtrl   |= PIN_ENBL;                /* [200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB) */
       //NOP;                                  /* WE DON'T READ ANYTHING; SO WE DON'T NEED A NOP TO READ CORRECT DATA! */
       ucLCDCtrl   &= (~PIN_ENBL);             /* [200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high! */
   
       /* wait busy */
       ucLCDCtrl   &= (~PIN_DI);               /* [200 ns] Set D/I to 'Instruction' */
       bLCDStatus = STAT_BUSY;                 /* [200 ns] clear state */
       do
       {
           ucLCDCtrl   |= PIN_ENBL;            /* [200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB) */
           bLCDStatus  = ucADBus;              /* [200 ns] save current state before disable E */
           ucLCDCtrl   &= (~PIN_ENBL);         /* [200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high! */
       }                                       /* [200 ns] jump back to loop begin  */
       while (bLCDStatus & STAT_BUSY);         /* [300 ns] check BUSY flag */
    }

    /* second (real) data read */
    ucLCDCtrl   |= PIN_DI;                  /* [200 ns] Set D/I to 'Data' */
    ucLCDCtrl   |= PIN_ENBL;                /* [200 ns] Set Enable !!! t(ddr)min = 320 ns between Set(E) <-> Valid(DB) */
    *pucData     = ucADBus;                 /* [600! ns] Save data byte */
    ucLCDCtrl   &= (~PIN_ENBL);             /* [200 ns] Clear Enable !!! P(weh)min = 450 ns to reset E to high! */

    /* de-init */
    ucLCDCtrl   &= (~PIN_RW);               /* 5. RW=low=write  */
    ucADBusD    = PORT_OUT;                 /* set m16c ports back to OUTPUT */
    return(ERR_OK);
}


/***********************************************************************
 *  FUNCTION:       LCDDrvCheckController
 *  DESCRIPTION:    LCD controller (write & read back) test to check, wether
 *                  a) LCD is connected and ok or
 *                  b) LCD is disconnected or defect.
 *  PARAMETER:      -
 *  RETURN:         ERR_OK              if LCD ok
 *                  ERR_NOT_SUPPORTED   if LCD not ok (e.g. not connected)
 *  COMMENT:        > To be used for debug purpose, to run application
 *                  without LCD.
 *                  > Checks only one controller side!
 *  CHANGES:        extra Parameter in call to LCDDrvReadData added, AN 16.2.2003
 *********************************************************************** */
ERRCODE LCDDrvCheckController(void)
{
    UINT8 bTestPattern = TESTPATTERN;   /* test pattern */

    /* check status first */
    LCDDrvCheckStatus();

    /* write & read back test */
    LCDDrvSetStartLine(0);              /* set Y display start line to 0 */
    LCDDrvSetColumn(0);                 /* set x column address to 0 */
    LCDDrvSetPage(0);                   /* set page (8 bit y address) to 0 */
    LCDDrvWriteData( bTestPattern );    /* write test pattern */
    bTestPattern = 0;                   /* just to assure read back result */
    LCDDrvWaitBusy();                   /* delay */
    LCDDrvSetColumn(0);                 /* set x column back to address 0 */
    LCDDrvReadData( &bTestPattern, DUMMY_ON ); /* read back test pattern */

    /* check results */
    if (bTestPattern == TESTPATTERN)
        return ERR_OK;                  /* LCD ok */
    else return ERR_NOT_SUPPORTED;      /* LCD disconnected or defect */
}



/***********************************************************************
 * FUNCTION:    LCDDrvInitLCD
 * DESCRIPTION: Initializes uC relevant ports and lcd status
 * PARAMETER:   -
 * RETURN:      ERR_OK              if lcd ok
 *              ERR_NOT_SUPPORTED   if lcd disconnected or defect
 * COMMENT:     > Does not CLEAR the display!
 *              > if error detected:
 *                  - disables lcd output (ignored)
 *                  - disables lcd backlight
 *                  - disables lcd contrast
 *********************************************************************** */
ERRCODE LCDDrvInitLCD(void)
{
    /* gate of 74245, backlight, contrast and LCD remain disabled */
    ucLCDCtrl       = PIN_GATE | PIN_BENBL;
    ucLCDCtrlD      = PORT_OUT;             /* all pins to output */

    /* initial settings ucADBus bus */
    ucADBus         = (unsigned char) 0x00; /* no data */
    ucADBusD        = PORT_OUT;             /* all pins to output */

    /* initial settings D-A conversion for backlight and contrast */
    fM16C_P9DWE = 1;            /* unprotect pd9, will be reset after each write to pd9_x */
    BlightLvlD  = 0;            /* must be input for D-A conversion */
    fM16C_P9DWE = 1;            /* unprotect pd9 again */
    ContrLvlD   = 0;            /* must be input for D-A conversion */
    dacon       = 0x03;         /* enable D-A conversion */

    //ToDo: initialise user settings for backlight and contrast (valid
    //      data from NVRAM?)
    BlightLvl   = 0xff;             /* initial lowest brightness */
    LCDDrvSetContrastLevel(0x30);   /* initial standard contrast */
    LCDDrvSetContrast(ON);          /* set display contrast visible */

    /* ckeck lcd connected & ok */
    {
        /* check left lcd controller */
        BOOL fErr = FALSE;

        /* tests: both lcd controllers */
        gfLCD_Ok = TRUE;                            /* enable all subsequent tests */
        LCDDrvSetSide(LEFT);                        /* check left lcd controller */
        if (LCDDrvCheckController() != ERR_OK)
            fErr = TRUE;
        LCDDrvSetSide(RIGHT);                       /* right lcd controller active */
        if (LCDDrvCheckController() != ERR_OK)
            fErr = TRUE;

        /* check: lcd ok? */
        if (fErr == TRUE)
        {
            /* disable lcd output / backlight / contrast if error */
            LCDDrvSetBacklight(OFF);                /* set display backlight off */
            LCDDrvSetContrast(OFF);                 /* set display contrast visible */
            gfLCD_Ok = FALSE;                       /* global flag: Error! */
            return ERR_NOT_SUPPORTED;
        }
    }

    // activate left display controller and init
    LCDDrvSetSide(LEFT);                /* left lcd controller active */
    LCDDrvSetStartLine(0);              /* set Y display start line to 0 */
    LCDDrvSetColumn(0);                 /* set x column address to 0 */
    LCDDrvSetPage(0);                   /* set page (8 bit y address) to 0 */
    LCDDrvSetVisible(ON);               /* switch display controller on */

    // activate right display controller and init
    LCDDrvSetSide(RIGHT);               /* right lcd controller active */
    LCDDrvSetStartLine(0);              /* set Y display start line to 0 */
    LCDDrvSetColumn(0);                 /* set x column address to 0 */
    LCDDrvSetPage(0);                   /* set page (8 bit y address) to 0 */
    LCDDrvSetVisible(ON);               /* switch display controller on */

    // lcd module default settings */
    return( ERR_OK );
}







/***********************************************************************
 * FUNCTION:    LCDDrvSetVisible
 * DESCRIPTION: Switches the controller to visible/invisible mode
 * PARAMETER:   ON / OFF
 * RETURN:      ok
 * COMMENT:     -
 *********************************************************************** */
ERRCODE LCDDrvSetVisible( const BOOL bMode)
{
    if ( !gfLCD_Ok ) return ERR_OK;         /* LCD may be disconnected */

    if ( bMode == ON )
        LCDDrvWriteInstr( DISPLON );
    else
        LCDDrvWriteInstr( DISPLOFF );
    return (ERR_OK);
}


/***********************************************************************
 * FUNCTION:    LCDDrvSetPixel
 * DESCRIPTION: Prints out one set of pixel (8 vertical bits) to controller
 * PARAMETER:   unsigned char ucPixel to be displayed
 * RETURN:      ok
 * COMMENT:     -
 *********************************************************************** */
ERRCODE LCDDrvSetPixel( const unsigned char ucPixel)
{
    if ( !gfLCD_Ok ) return ERR_OK;         /* LCD may be disconnected */

    LCDDrvWriteData( ucPixel );
    return (ERR_OK);
}


/***********************************************************************
 * FUNCTION:    LCDDrvGetPixel
 * DESCRIPTION: Reads back set of pixel (8 vertical bits) from controller
 * PARAMETER:   unsigned char *pucPixel to be read from lcd memory
 *              flag if a dummy read cycle must be done (DUMMY_ON/DUMMY_OFF)
 * RETURN:      ok
 * COMMENT:     -
 * CHANGES:     added control of the dummy read cycle for support of
 *              multiple reads without address set (sequence is address
 *              set, dummy read, read data0, read data1,...), AN 16.2.2003
 *********************************************************************** */
ERRCODE LCDDrvGetPixel( unsigned char * pucPixel, const BOOL bMode )
{
    if ( !gfLCD_Ok ) return ERR_OK;         /* LCD may be disconnected */

    LCDDrvReadData( pucPixel, bMode );
    return (ERR_OK);
}


/***********************************************************************
 * FUNCTION:    LCDDrvSetColumn
 * DESCRIPTION: Sets controller current output X-pointer to column n
 * PARAMETER:   unsigned char uiXColumn
 * RETURN:      ok
 * COMMENT:     TBD: check LCDDrv return value!
 *********************************************************************** */
ERRCODE LCDDrvSetColumn (const unsigned char ucColumn )
{
    if ( !gfLCD_Ok ) return ERR_OK;         /* LCD may be disconnected */

    if ( ucColumn < PAGE_COLUMN_MAX )
    {
        LCDDrvWriteInstr( SETXADDR | ucColumn );   /* set x column address */
        return (ERR_OK);
    }
    return (ERR_PARAM_ERR);
}


/***********************************************************************
 * FUNCTION:    LCDDrvSetPage
 * DESCRIPTION: Sets controller current output Y pointer to page n
 * PARAMETER:   unsigned char ucPage
 * RETURN:      ok
 * COMMENT:     TBD: check LCDDrv return value!
 *********************************************************************** */
ERRCODE LCDDrvSetPage( const unsigned char ucPage )
{
    if ( !gfLCD_Ok ) return ERR_OK;         /* LCD may be disconnected */

    if ( ucPage < PAGE_MAX )
    {
        LCDDrvWriteInstr( SETYPAGE | ucPage );     /* set page (8 bit y address) */
        return (ERR_OK);
    }
    return (ERR_PARAM_ERR);
}

/***********************************************************************
 * FUNCTION:    LCDDrvSetStartLine
 * DESCRIPTION: Sets controller current output start line (scroll effects!)
 * PARAMETER:   unsigned char ucStartLine
 * RETURN:      ok
 * COMMENT:     TBD: check LCDDrv return value!
 *
 *              Scrolls across the hidden 8th page!
 *********************************************************************** */
ERRCODE LCDDrvSetStartLine( const unsigned char ucStartLine )
{
    if ( !gfLCD_Ok ) return ERR_OK;         /* LCD may be disconnected */

    if ( ucStartLine < Y_MAX_CONTR )
    {
        LCDDrvWriteInstr( SETSTARTL | ucStartLine );     /* set startline */
        return (ERR_OK);
    }
    return (ERR_PARAM_ERR);
}



/***********************************************************************
 * FUNCTION:    LCDDrvFillPattern
 * DESCRIPTION: Fills controller memory with pattern
 * PARAMETER:   const unsigned char ucPattern
 * RETURN:      ok
 * COMMENT:     Uses implicit X-Column increment of lcd controller
 *              (quasi burst mode )
 *********************************************************************** */
ERRCODE LCDDrvFillPattern(const unsigned char ucPattern)
{
    unsigned char ucPage = 0;
    unsigned char ucColumn = 0;

    if ( !gfLCD_Ok ) return ERR_OK;         /* LCD may be disconnected */

    /* fill complete display half */
    for ( ucPage = 0; ucPage < PAGE_MAX; ucPage++ )
    {
        LCDDrvSetColumn(0);            /* set X column to 0 */
        LCDDrvSetPage( ucPage );       /* set y page */

        /* fill complete page */
        for (ucColumn = 0; ucColumn < PAGE_COLUMN_MAX; ucColumn++)
        {
            LCDDrvWriteData ( ucPattern );
        }
    }
    return ERR_OK;
}


    