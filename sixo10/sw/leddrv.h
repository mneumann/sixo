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
 *  Created:        31-10-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         led 
 *  Purpose:        high & low level led functions
 *  Comments:       -
 *
 ************************************************************************ */
#ifndef _LEDDrvH
#define _LEDDrvH

/* general purpose defines */
#define NUMBER_LEDS      3       /* we currently use 3 leds (D3, D4, D5)*/

/* redefine of port register of sfr62.h for readability */
#define fLED1_D3N       p9_0    /* D3N = normal = green LED1 mode */
#define fLED1_D3ND      pd9_0   /* D3N = normal = green LED1 port direction register */
#define fLED1_D3I       p9_5    /* D3I = invers = red LED1 mode */
#define fLED1_D3ID      pd9_5   /* D3I = invers = red LED1 port direction register */

#define fLED2_D4N       p9_1    /* D4N = normal = green LED2 mode */
#define fLED2_D4ND      pd9_1   /* D4N = normal = green LED2 port direction register */
#define fLED2_D4I       p9_6    /* D4I = invers = red LED2 mode */
#define fLED2_D4ID      pd9_6   /* D4I = invers = red LED2 port direction register */

#define fLED3_D5N       p9_2    /* D5N = normal = green LED3 mode */
#define fLED3_D5ND      pd9_2   /* D5N = normal = green LED3 port direction register */
#define fLED3_D5I       p9_7    /* D5I = invers = red LED3 mode */
#define fLED3_D5ID      pd9_7   /* D5I = invers = red LED3 port direction register */

#define fM16C_P9DWE     prc2    /* bit2 of m16c protected mode control register (enable p9d write access) */

/* enumerate LEDs */
typedef enum
{
    LED1_D3,       /* led D3 */
    LED2_D4,       /* led D4 */
    LED3_D5        /* led D5 */
} LEDDrvENUMTYPE;   

/* define led acivate status */
typedef enum
{
    LED_OFF,    /* led off */
    LED_GREEN,  /* led normal on = green */
    LED_RED     /* led invers on = red */
} LEDDrvSTATETYPE;


/* function protoypes */
ERRCODE LEDDrvInit(void);
ERRCODE LEDDrvSetLED(LEDDrvENUMTYPE bLEDIndex, LEDDrvSTATETYPE bActivate);
void LEDTest(unsigned int loopdelay);
void LEDTest2(unsigned int loopdelay);

#endif /* _LEDDrvH */