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
 *  Purpose:        low level led functions
 *  Comments:       
 *                  Note that all led ouput pins are currently
 *                  mapped to m16c 'protected port' port#9.
 *
 ************************************************************************ */
 
#include "sfr62.h"
#include "standard.h"
#include "leddrv.h"
#include "timer.h"
#include "debug.h"
 
 
 
/***********************************************************************
 *  FUNCTION:       LEDDrvInit()
 *  DESCRIPTION:    initializes led hardware
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        dual color leds are activated by 'inverted' pins:
 *                  one side to output 'LOW', other side to output 'HIGH'
 *********************************************************************** */
ERRCODE LEDDrvInit(void)
{
    /* switch off all leds (and their invers pendants) */
    p9 = 0;

    /* switch all led port direction register to 'output' */
    prc2  = 1;      // unprotect pd9
    pd9   = 0xE7;   // bits 3 and 4 must be input for D-A conversion
    
    ODS(DBG_DRV,DBG_INFO,"LEDDrvInit() done!");
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       LEDDrvSetLED()
 *  DESCRIPTION:    sets led(x) to defined status (off/normal/invers)
 *  PARAMETER:      LEDDrvENUMTYPE    bLEDIndex
 *                  LEDDrvSTATETYPE   bActivate
 *  RETURN:         error code 
 *  COMMENT:        dual color leds are activated by 'inverted' pins:
 *                  one side to 0, other side to '1'
 *********************************************************************** */
ERRCODE LEDDrvSetLED(LEDDrvENUMTYPE bLEDIndex, LEDDrvSTATETYPE bActivate)
{
    switch (bLEDIndex)
    {
        case LED1_D3:
        {
            switch (bActivate)
            {
                case LED_OFF:
                    fLED1_D3N   = 0;        /* led normal output pin to '0' */
                    fLED1_D3I   = 0;        /* led invers output pin to '0' */
                    break;
                case LED_GREEN:
                    fLED1_D3N   = 1;        /* led normal output pin to '1' */
                    fLED1_D3I   = 0;        /* led invers output pin to '0' */
                    break;
                case LED_RED:
                    fLED1_D3N   = 0;        /* led normal output pin to '0' */
                    fLED1_D3I   = 1;        /* led invers output pin to '1' */
                    break;
                default: return ERR_PARAM_ERR;
            }
        }; break;
        case LED2_D4:
        {
            switch (bActivate)
            {
                case LED_OFF:
                    fLED2_D4N   = 0;        /* led normal output pin to '0' */
                    fLED2_D4I   = 0;        /* led invers output pin to '0' */
                    break;
                case LED_GREEN:
                    fLED2_D4N   = 1;        /* led normal output pin to '1' */
                    fLED2_D4I   = 0;        /* led invers output pin to '0' */
                    break;
                case LED_RED:
                    fLED2_D4N   = 0;        /* led normal output pin to '0' */
                    fLED2_D4I   = 1;        /* led invers output pin to '1' */
                    break;
                default: return ERR_PARAM_ERR;
            }
        }; break;
        case LED3_D5:
        {
            switch (bActivate)
            {
                case LED_OFF:
                    fLED3_D5N   = 0;        /* led normal output pin to '0' */
                    fLED3_D5I   = 0;        /* led invers output pin to '0' */
                    break;
                case LED_GREEN:
                    fLED3_D5N   = 1;        /* led normal output pin to '1' */
                    fLED3_D5I   = 0;        /* led invers output pin to '0' */
                    break;
                case LED_RED:
                    fLED3_D5N   = 0;        /* led normal output pin to '0' */
                    fLED3_D5I   = 1;        /* led invers output pin to '1' */
                    break;
                default: return ERR_PARAM_ERR;
            }
        }; break;
        default: return ERR_PARAM_ERR;
    }
    return ERR_OK;
}




/***********************************************************************
 *  FUNCTION:       LEDTest
 *  DESCRIPTION:    
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Arnolds Original Test-Sequenz
 *********************************************************************** */
void LEDTest(unsigned int loopdelay)
{
    unsigned char davalue = 0x00;
    unsigned char i;

    //init pd9 direction and D-A conversion
    p9    = 0x00; //turn all LEDs off
    prc2  = 1;    //unprotect pd9
    pd9   = 0xE7; //bits 3 and 4 must be input for D-A conversion
    prc2  = 0;      /* protect pd9 (normally returns aut. to '0') */
    da1   = 0x00; //D-A value
    dacon = 0x02; //enable D-A

    da1 = 0x30;   //set contrast

    for(i=0; i<10; i++){
       /* let LEDs flash for a moment to indicate prog is running */
       p9 = 0x01;    /* D3 green on */
       Delay_ms(loopdelay);
       p9 = 0x02;    /* D4 green on */
       Delay_ms(loopdelay);
       p9 = 0x04;    /* D5 green on */
       Delay_ms(loopdelay);
       p9 = 0x20;    /* D3 red on   */
       Delay_ms(loopdelay);
       p9 = 0x40;    /* D4 red on   */
       Delay_ms(loopdelay);
       p9 = 0x80;    /* D5 red on   */
       Delay_ms(loopdelay);
    }
    p9 = 0x00; //all LEDs off
}


/***********************************************************************
 *  FUNCTION:       LEDTest2
 *  DESCRIPTION:    
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void LEDTest2(unsigned int loopdelay)
{
    LEDDrvSetLED(LED1_D3, LED_GREEN);
    Delay_ms(loopdelay);    
    LEDDrvSetLED(LED2_D4, LED_GREEN);
    Delay_ms(loopdelay);    
    LEDDrvSetLED(LED3_D5, LED_GREEN);
    Delay_ms(loopdelay);    
    
    LEDDrvSetLED(LED1_D3, LED_RED);
    Delay_ms(loopdelay);    
    LEDDrvSetLED(LED2_D4, LED_RED);
    Delay_ms(loopdelay);    
    LEDDrvSetLED(LED3_D5, LED_RED);
    Delay_ms(loopdelay);    

    LEDDrvSetLED(LED1_D3, LED_OFF);
    Delay_ms(loopdelay);    
    LEDDrvSetLED(LED2_D4, LED_OFF);
    Delay_ms(loopdelay);    
    LEDDrvSetLED(LED3_D5, LED_OFF);
    Delay_ms(loopdelay);    

}

 