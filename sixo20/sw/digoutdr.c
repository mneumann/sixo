/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-01 by Arnold Neugebauer
 *  Project:        SIxO
 *  Module:         Digital Output Driver
 *  Purpose:        Low level support of all digital outputs
 *                  (but not GPOs and Buzzer!)
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
 *  This information is automatically added while 'commit' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 3.3  2012/02/04 20:38:05  tuberkel
 * Moved all BeeperDriver / LEDDriver stuff ==> 'digoutdrv'
 *
 * Revision 3.2  2012/02/04 08:33:13  tuberkel
 * BugFix DigOut PWM
 *
 *
 ************************************************************************ */


 #include "standard.h"
 #include "sfr62p.h"
 #include "debug.h"
 #include "digoutdr.h"




/***********************************************************************
 *  FUNCTION:       DigOut_Init
 *  DESCRIPTION:    Initializes all General Digital Output Pins
 *                  and some solder pads
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        Prevents floating input pins p3_0 / p3_1 of
 *                  destroying transistors, should be called as early as
 *                  possible
 *********************************************************************** */
ERRCODE DigOutInit(void)
{
    /* Beeper port pin */
    fBeep   = 0;    /* switch off output */
    fBeep_D = 1;    /* enabled output */

  /* switch off all leds */
    p9 &= ~LEDS_ALL;    // LED pins only !

    /* switch all led port direction register to 'output' */
    prc2 = 1;           // unprotect pd9 by prc2
    pd9  = LEDS_ALL;    // bits 3 and 4 reserved for display!
                        // (prc2 automatic falls back to protection)

    /* switch on LED brightness control */
    pd8_0 = 1;          // port direction to OUTPUT
    p8_0  = 1;          // enable LED common current control transistor

    /* set general purpose outputs GPO0/1 to prevent damage */
    p3_0 = 0;
    p3_1 = 0;
    pd3_0 = 1;
    pd3_1 = 1;
    // use macros GPO0/1 for further use
    ODS(DBG_DRV,DBG_INFO,"DigOutInit() done!");


    /* 'test flash' to display init success? */
    #if(TEST_LED_PORTS==1)
    if (fFlash == TRUE)
    {
        p9 = LEDS_ALL;      // all LEDs on
        Delay_ms(500);      // wait
        p9 &= ~LEDS_ALL;    // all LEDs off
    }
    #endif

    /* TEST: Port p3_2/3/4 for ossi checks */
    #if(TEST_TOGGLEPADS_PORTS==1)
    p3   = p3 & (~0x1c);   /* clear output of p3_2/3/4 */
    pd3  = p3 |   0x1c;    /* set p3_2/3/4 to output */
    // use macros TOGGLE_PAD9/10/11 for further tests
    #endif

    return ERR_OK;
}




/***********************************************************************
 *  FUNCTION:       BeepDrvSetBeeper()
 *  DESCRIPTION:    switches beeper on/off
 *  PARAMETER:      BOOL    fActive     TRUE = on
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE BeepDrvSetBeeper(BOOL fActive)
{
    fBeep = (fActive == TRUE) ? 1 : 0;
    if (fActive == TRUE)
         fBeep   = 1;    /* switch ON output */
    else fBeep   = 0;    /* switch OFF output */
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       BeepDrvGetBeeper()
 *  DESCRIPTION:    returns current beeper pin state
 *  PARAMETER:      -
 *  RETURN:         BOOL    fActive     TRUE = on
 *  COMMENT:        -
 *********************************************************************** */
BOOL BeepDrvGetBeeper(void)
{
    return ( fBeep );
}



/***********************************************************************
 *  FUNCTION:       LEDDrvSetLED()
 *  DESCRIPTION:    control over one single LED
 *  PARAMETER:      eLED            LED indicator
 *                  fActivate       TRUE = LED on
 *  RETURN:         ERR_OK          ok
 *                  ERR_PARAM_ERR   parameter error
 *  COMMENT:        LED State could have been set _directly_ with
 *                  given parameter. But we here distinguish
 *                  the true/false states to support separate debugging
 *                  breakpoints for both states.
 *********************************************************************** */
ERRCODE LEDDrvSetLED(LEDDRV_LEDS eLED, BOOL fActivate)
{
    switch (eLED)
    {
        case LEDDRV_WARN:
            if (fActivate)
                 fLEDDrv_Warn  = 1;
            else fLEDDrv_Warn  = 0;
            break;
        case LEDDRV_ERR:
            if (fActivate)
                 fLEDDrv_Err = 1;
            else fLEDDrv_Err = 0;
            break;
        case LEDDRV_INFO:
            if (fActivate)
                 fLEDDrv_Info = 1;
            else fLEDDrv_Info = 0;
            break;
        case LEDDRV_BEAM:
            if (fActivate)
                 fLEDDrv_Beam = 1;
            else fLEDDrv_Beam = 0;
            break;
        case LEDDRV_NEUTR:
            if (fActivate)
                 fLEDDrv_Neutr = 1;
            else fLEDDrv_Neutr = 0;
            break;
        case LEDDRV_TURN:
            if (fActivate)
                 fLEDDrv_Turn  = 1;
            else fLEDDrv_Turn  = 0;
            break;
        default:
            return ERR_PARAM_ERR;
     }
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       LEDDrvGetLED()
 *  DESCRIPTION:    Returns current state of single LED
 *  PARAMETER:      eLED            LED indicator
 *  RETURN:         fActivate       TRUE  = LED on
 *                                  FALSE = LED off
 *  COMMENT:        -
 *********************************************************************** */
BOOL LEDDrvGetLED( LEDDRV_LEDS eLED )
{
    BOOL fActivated;
    switch (eLED)
    {
        case LEDDRV_WARN:   fActivated = fLEDDrv_Warn;  break;
        case LEDDRV_ERR:    fActivated = fLEDDrv_Err  ; break;
        case LEDDRV_INFO:   fActivated = fLEDDrv_Info ; break;
        case LEDDRV_BEAM:   fActivated = fLEDDrv_Beam ; break;
        case LEDDRV_NEUTR:  fActivated = fLEDDrv_Neutr; break;
        case LEDDRV_TURN:   fActivated = fLEDDrv_Turn ; break;
        default: return fActivated = FALSE;
     }
    return fActivated;
}



/***********************************************************************
 *  FUNCTION:       LEDDrvSetBright
 *  DESCRIPTION:    Switches common LED current control on/off
 *  PARAMETER:      bBrightness     0..255  brightness (0=off)
 *  RETURN:         ERR_OK
 *  COMMENT:        Simply enables/disables LEDs:
 *                      0:  all LEDs off
 *                      >0: all LEDs on
 *
 *  T.B.D.: Common LED Brightness Control p8_0 will have to be done with
 *          analogue signal instead of digital switch yet.
 *
 *********************************************************************** */
ERRCODE LEDDrvSetBright(unsigned char bBrightness)
{
    fLEDDrv_Cntrl = (bBrightness == 0) ? 0 : 1;   // set control
    return(ERR_OK);
}



 /***********************************************************************
 *  FUNCTION:       DigOutSetGPO()
 *  DESCRIPTION:    control over one single GPO
 *  PARAMETER:      eGPO            GPO indicator
 *                  fActivate       TRUE = GPO on
 *  RETURN:         ERR_OK          ok
 *                  ERR_PARAM_ERR   parameter error
 *  COMMENT:        GPO State could have been set _directly_ with
 *                  given parameter. But we here distinguish
 *                  the true/false states to support separate debugging
 *                  breakpoints for both states.
 *********************************************************************** */
ERRCODE DigOutSetGPO(DIGOUT_GPOS eGPO, BOOL fActivate)
{
    switch (eGPO)
    {
        case eDIGOUT_GPO0:
            if (fActivate)
                 GPO0  = 1;
            else GPO0  = 0;
            break;
        case eDIGOUT_GPO1:
            if (fActivate)
                 GPO1 = 1;
            else GPO1 = 0;
            break;
        default:
            return ERR_PARAM_ERR;
     }
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       DigOutGetGPO()
 *  DESCRIPTION:    Returns current state of single GPO
 *  PARAMETER:      eGPO            GPO indicator
 *  RETURN:         fActivate       TRUE  = GPO on
 *                                  FALSE = GPO off
 *  COMMENT:        -
 *********************************************************************** */
BOOL DigOutGetGPO( DIGOUT_GPOS eGPO )
{
    BOOL fActivated;
    switch (eGPO)
    {
        case eDIGOUT_GPO0:   fActivated = GPO0; break;
        case eDIGOUT_GPO1:   fActivated = GPO1; break;
        default: return fActivated = FALSE;
     }
    return fActivated;
}


 