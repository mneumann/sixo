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
 *  Created:        01-11-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         LED
 *  Purpose:        led services (timing, auto update)
 *  Comments:       -
 *
 ************************************************************************ */
#ifndef _LED_H
#define _LED_H


/* enumerate LEDs */
typedef enum
{
    LED1,       /* led D3 */
    LED2,       /* led D4 */
    LED3        /* led D5 */
} LED_ENUMTYPE;



/* re-define led acivate status driver type */
#define LED_MODETYPE LEDDrvSTATETYPE


/* led timing */
typedef struct
{
    UINT8   wOnTicks;    /* led ON duration in 1/10 sec */
    UINT8   wOffTicks;   /* led OFF duration in 1/10 sec */
} LEDTIMINGTYPE;


/* macros for easier led messages and timings in ms / sec / ticks */
#define LED_MSG_MS(msg, led, status, ontime, offtime) MSG_BUILD_UINT8(msg, MSG_LED_SET, (led | (status<<4)), MS2TICKS(ontime), MS2TICKS(offtime))
#define LED_MSG_SEC(msg, led, status, ontime, offtime) MSG_BUILD_UINT8(msg, MSG_LED_SET, (led | (status<<4)), SEC2TICKS(ontime), SEC2TICKS(offtime))
#define LED_MSG_TICKS(msg, led, status, ontime, offtime) MSG_BUILD_UINT8(msg, MSG_LED_SET, (led | (status<<4)), ontime, offtime)


/* automatic color led define */
#ifdef HW_OILGEN
    #define HW_LEDRYW
#endif

/* led defines for Ralfs BMW version (red-yellow-white) */
#ifdef HW_LEDRYW
    /* redefines from LED_ENUMTYPE */
    #define LED_OIL     LED3        /* red warning led for oil pressure and ESC key*/
    #define LED_GEN     LED2        /* yellow warning led for engine generator */
    #define LED_FLASH   LED1        /* white engine rounds gearbox flash and OK key*/

    /* redefines from LEDDrvSTATETYPE */
    #define LED_ON      LED_GREEN   /* taken from LEDDrvSTATETYPE */

    /* to easily control leds via messages
       e.g.: LED_MSG( NewMsg, LED_OIL, LED_ON); */
    #define LED_MSG(msg,led,status) MSG_BUILD_UINT8(msg, MSG_LED_SET, (led | (status<<4)), 1, 0)
#endif // HW_LEDRYW




/* prototypes */
ERRCODE LEDInit(void);
ERRCODE LEDMsgEntry(MESSAGE msg);
ERRCODE LEDSetNewState(MESSAGE msg);
void LEDOk(void);
void LEDEsc(void);


/* test functions */
void TestLEDSendMessage(void);
void TestCheckKeyStateMsgs(MESSAGE msg);

#endif /* _LED_H */



