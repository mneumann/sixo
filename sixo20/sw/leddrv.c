/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-10-31 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         led
 *  Purpose:        low level led functions
 *  Comments:
 *                  Note that all led ouput pins are currently
 *                  mapped to m16c 'protected port' port#9.
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
 *  This information is automatically added while 'commiting' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 2.2  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.1  2009/07/08 21:41:17  tuberkel
 * All compiler defines reviewed:
 * - set to unique usage: set define to 0 or 1
 * - default values set, if not used
 * - see 'Project Editor' for details
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 *
 ************************************************************************ */


#include "sfr62p.h"
#include "standard.h"
#include "leddrv.h"
#include "timer.h"
#include "debug.h"



/***********************************************************************
 *  FUNCTION:       LEDDrvInit()
 *  DESCRIPTION:    initializes led hardware
 *  PARAMETER:      fFlash      TRUE shows all LEDs flashing once for success
 *  RETURN:         ERR_OK
 *  COMMENT:        1) port pins p9_3/4 reserved for display!
 *                  2) port direction register pd9 is protected by prc2!
 *
 *  T.B.D.: Common LED Brightness Control p8_0 will have to be done with
 *          analogue signal instead of digital switch yet.
 *
 *********************************************************************** */
ERRCODE LEDDrvInit(BOOL fFlash)
{
    /* switch off all leds */
    p9 &= ~LEDS_ALL;    // LED pins only !

    /* switch all led port direction register to 'output' */
    prc2 = 1;           // unprotect pd9 by prc2
    pd9  = LEDS_ALL;    // bits 3 and 4 reserved for display!
                        // (prc2 automatic falls back to protection)

    /* switch on LED brightness control */
    pd8_0 = 1;          // port direction to OUTPUT
    p8_0  = 1;          // enable LED common current control transistor

    /* 'test flash' to display init success? */
    if (fFlash == TRUE)
    {
        p9 = LEDS_ALL;      // all LEDs on
        Delay_ms(500);      // wait
        p9 &= ~LEDS_ALL;    // all LEDs off
    }

    ODS(DBG_DRV,DBG_INFO,"LEDDrvInit() done!");
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       LEDDrvSetLED()
 *  DESCRIPTION:    control over one single LED
 *  PARAMETER:      eLED            LED indicator
 *                  fActivate       TRUE = LED on
 *  RETURN:         ERR_OK          ok
 *                  ERR_PARAM_ERR   parameter error
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE LEDDrvSetLED(LEDDRV_LEDS eLED, BOOL fActivate)
{
    switch (eLED)
    {
        case LEDDRV_WARN:   fLEDDrv_Warn  = fActivate; break;
        case LEDDRV_ERR:    fLEDDrv_Err   = fActivate; break;
        case LEDDRV_INFO:   fLEDDrv_Info  = fActivate; break;
        case LEDDRV_BEAM:   fLEDDrv_Beam  = fActivate; break;
        case LEDDRV_NEUTR:  fLEDDrv_Neutr = fActivate; break;
        case LEDDRV_TURN:   fLEDDrv_Turn  = fActivate; break;
        default: return ERR_PARAM_ERR;
     }
    return ERR_OK;
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
 *  FUNCTION:       LEDTest
 *  DESCRIPTION:
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
#if(TESTLED==1)
void LEDDrvTest(unsigned int loopdelay)
{
    LEDDRV_LEDS eLED;

    for (eLED = 0; eLED < LEDDRV_MAX; eLED++)
    {
        LEDDrvSetLED(eLED, TRUE);
        Delay_ms(loopdelay);
    }
    for (eLED = 0; eLED < LEDDRV_MAX; eLED++)
    {
        LEDDrvSetLED(eLED, FALSE);
        Delay_ms(loopdelay);
    }
}
#endif // TESTLED


 