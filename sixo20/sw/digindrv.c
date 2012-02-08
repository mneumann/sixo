/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-09 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         Digital Input
 *  Purpose:        Handles all General Purpose Inputs
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
 *      Arnold Neugebauer                   Ralf Schwarzer
 *      St�ckener Str. 115                  Hudeplan 37 A
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
 * Revision 3.8  2012/02/08 04:52:34  tuberkel
 * Real HeatGrip measurement activated (but not yet bugfree)
 *
 * Revision 3.7  2012/02/08 03:55:24  tuberkel
 * KEY_TIMING parameter names reviewed/changed
 *
 * Revision 3.6  2012/02/08 03:41:30  tuberkel
 * renamed #define MINIEMU ==> KD30_USED
 *
 * Revision 3.5  2012/02/07 17:32:31  tuberkel
 * HeatGrip PWM test animated
 *
 * Revision 3.4  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 3.3  2012/02/04 20:38:05  tuberkel
 * Moved all BeeperDriver / LEDDriver stuff ==> 'digoutdrv'
 *
 * Revision 3.2  2012/02/01 22:33:49  tuberkel
 * PWM-Measurement of GPI0..3 running & ok!
 *
 * Revision 3.1  2012/02/01 21:24:00  tuberkel
 * INT2..5 enabled & ok for GPI0..3
 *
 * Revision 3.0  2010/11/07 09:50:23  tuberkel
 * V30 Preparations - no changes
 *
 * Revision 2.4  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.3  2009/07/08 21:41:17  tuberkel
 * All compiler defines reviewed:
 * - set to unique usage: set define to 0 or 1
 * - default values set, if not used
 * - see 'Project Editor' for details
 *
 * Revision 2.2  2009/06/24 21:00:28  tuberkel
 * Just comments and removed spaces
 *
 * Revision 2.1  2006/10/01 22:17:10  tuberkel
 * NEW:
 * - DigitalFilter functions
 * Fixed:
 * DigInDrv_Init()
 * - removed isr2 code (obsolete)
 * - removed ALTW digital access
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 *
 ************************************************************************ */


#include "sfr62p.h"
#include "m16c.h"
#include "standard.h"
#include "timer.h"
#include "debug.h"
#include "measdrv.h"
#include "digoutdr.h"
#include "msgqueue.h"
#include "led.h"
#include "device.h"
#include "anaindrv.h"
#include "sysparam.h"
#include "digindrv.h"


/* external vars */
extern UINT16       wMilliSecCounter;   /* valid values: 0h .. ffffh */
extern BIKE_TYPE    gBikeType;          /* bike type */



/* module global vars */
KEYTIME   rgKeyControl[KEY_LAST];    /* protocol of duration & start time */
KEYTIME   MultiKeyControl;           /* protocol for multiple pressed keys */


/* digital filter table for all inputs */
DIGFILTTYPE DigInFilter[eDF_LAST];


/* GPI0..3 / INT2..5 interrupt time measurements (PulseWidth & Frequency) */
DIGINTMEAS DigIntMeas[eGPI_MAX];



/* KD30 support: These values can set via KD30-GUI too! */
#if(KD30_USED==1)
unsigned char KD30_KEY_OK = 0;
unsigned char KD30_KEY_UP = 0;
unsigned char KD30_KEY_DOWN = 0;
unsigned char KD30_KEY_UPDOWN = 0;
#endif




/***********************************************************************
 *  FUNCTION:       DigInDrv_Init
 *  DESCRIPTION:    HW initialisation of all general purpose input pins
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        All port pin are already in 'input mode' after
 *                  reset, but we can do this at any time...
 *********************************************************************** */
ERRCODE DigInDrv_Init(void)
{
    UINT8 i;

    INT_GLOB_DISABLE;            /* disable ALL interrupts while changing settings */

    /* assure Input Pins being working in INPUT direction */
    KeyPortD_Ok     = 0;    /* all KEY pins set to 'input' */
    KeyPortD_Down   = 0;
    KeyPortD_Up     = 0;

    DigIn_GPID_0    = 0;    /* all GPI pins set to 'input' */
    DigIn_GPID_1    = 0;
    DigIn_GPID_2    = 0;
    DigIn_GPID_3    = 0;

    /* other digital in init stuff (TURN/OIL/NEUTR/HBEAM) */
    pd2 &= ~DIGIN_ALL_P2;   // set all digin pins to INPUT

    /* HW version detection (use internal pullups if ext. pulldown not mounted) */
    HWVerPortD &= ~HWVERS_PINS;     // assure 3 HW-Version pins to input only
    pu10 = 1;                       // activate M16C pull-ups for p4_0 .. p4_3

    /* setup additional GPI0..3 connected INT2..5 interrupts, common settings:
        - all GPIO Int level set to 2
        - polarity flag to '0' (essential for 'both edges' setup in ifsr!)
        - INT4/INT5 have to enabled additionally in ifsr.bit6+7 !!!
        - request cause select set to 'both edges' to prepare for pulse width measurement */
    int2ic  = 0x02;      // GPI00
    int3ic  = 0x02;      // GPI01
    int4ic  = 0x02;      // GPI02
    int5ic  = 0x02;      // GPI03
    ifsr    = 0xfc;      // setup for 'both edges' for INT2/INT3/INT4/INT5, enable INT4/INT5

    /* init key pressed time protocol */
    for (i=0; i<(KEY_LAST); i++)
    {
        rgKeyControl[i].wStartTime_ms     = 0;
        rgKeyControl[i].wLastSentTime_ms  = 0;
        rgKeyControl[i].wDur_ms      = 0;
    }
    MultiKeyControl.wStartTime_ms     = 0;
    MultiKeyControl.wLastSentTime_ms  = 0;
    MultiKeyControl.wDur_ms      = 0;

    /* initialize digital input filter table */
    DigInDrv_FilterInit();

    INT_GLOB_ENABLE;             /* enable all ISRs */

    ODS(DBG_DRV,DBG_INFO,"DigInDrv_Init() done!");
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       DigInDrv_GetKeyStates
 *  DESCRIPTION:    return actual keyboard status in a bit field
 *  PARAMETER:      -
 *  RETURN:         UINT8 bit masked key states (FALSE if 'pressed')
 *  COMMENT:        Key pressed pulls down signal to LOW.
 *********************************************************************** */
UINT8 DigInDrv_GetKeyStates(void)
{
    UINT8 RValue = 0x0;

    /* check keyboard port pins */
    if (KeyPort_Ok == 0)
        RValue |= KEYFL_OK;
    if (KeyPort_Up == 0)
        RValue |= KEYFL_UP;
    if (KeyPort_Down == 0)
        RValue |= KEYFL_DOWN;

#if(KD30_USED==1)
    /* check value given by KD30 */
    if (KD30_KEY_OK)
        RValue |= KEYFL_OK;
    if ((KD30_KEY_UP) || (KD30_KEY_UPDOWN))
        RValue |= KEYFL_UP;
    if ((KD30_KEY_DOWN) || (KD30_KEY_UPDOWN))
        RValue |= KEYFL_DOWN;
    /* make a 'longer press' by decr. until 0 reached */
    if (KD30_KEY_OK)
        KD30_KEY_OK--;
    if (KD30_KEY_UP)
        KD30_KEY_UP--;
    if (KD30_KEY_DOWN)
        KD30_KEY_DOWN--;
    if (KD30_KEY_UPDOWN)
        KD30_KEY_UPDOWN--;
#endif



    return RValue;
}


/***********************************************************************
 *  FUNCTION:       DigInDrv_CheckKeyAction
 *  DESCRIPTION:    supports protocol of transition and duration
 *                  of keys and sends messages
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        1) We do NOT handle times > 65 sec !!!
 *                  If it is pressed longer than that, it will be
 *                  like pressed the first time! Any problem??? ;-)
 *                  2) Any key - if pressed or not - has to be handled,
 *                  in order to support recognition of 'key released'
 *                  3) If more than 1 key has been pressed, we sent a
 *                  special additional 'multiple key' message
 *********************************************************************** */
ERRCODE DigInDrv_CheckKeyAction(void)
{
    ERRCODE     RValue = ERR_OK;
    static UINT8       bKeyState;                      /* current bit masked key state */
    KEYNUMBER   Key;                            /* loop over all keys */
    UINT16      wActTime_ms;                    /* to calculate the duration */
    UINT16      wCommonDuration_ms = 0;         /* maximum of all keys pressed the same time */
    UINT8       bKeyCounter;                    /* to handle more than 1 key */
    MESSAGE     KeyMsg;                         /* to send appropriate message */
    UINT16      wWaitTime;                      /* to handle repition rate & delay */

    /* ---------------------------------------------------- */
    /* init stuff */
    bKeyState = DigInDrv_GetKeyStates();         /* get current state of all keys (bitmasked) */
    TimerGetSys_msec( wActTime_ms );            /* to handle time stamps */
    bKeyCounter = 0;
    for (Key = KEY_OK; Key < KEY_LAST; Key++)
        if (bKeyState & (1<<Key))
            bKeyCounter++;                      /* get number of detected pressed buttons */

    /* ---------------------------------------------------- */
    /*  One OR none buttons have been pressed?
        This part has to be executetd too, if NO key have been pressed,
        in order to reset key to OFF state!!! */
    if (bKeyCounter < 2)
    {
        /* one or none key pressed, find out which.. */
        for (Key = KEY_OK; Key < KEY_LAST; Key++)
        {
            /* check key bit: key pressed? */
            if (bKeyState & (1<<Key))
            {
                /* PRESSED! first press? */
                if (rgKeyControl[Key].KeyTransit == KEYTRANS_OFF)   /* state until now: OFF! */
                {
                    /* PRESSED FIRST TIME! */
                    if (rgKeyControl[Key].wDur_ms <= KEYTM_PRESSED )
                    {
                        /* set start time and minimal duration */
                        /* BUT do net send msg now! */
                        if (rgKeyControl[Key].wDur_ms == 0)
                        {
                            rgKeyControl[Key].wStartTime_ms = wActTime_ms;
                            rgKeyControl[Key].wDur_ms  = KEYTM_MIN;
                        }
                        else rgKeyControl[Key].wDur_ms = wActTime_ms - rgKeyControl[Key].wStartTime_ms;
                    }
                    else
                    {
                        /* check for minimal time period (avoid jitter) */
                        /* and THEN send message */
                        rgKeyControl[Key].KeyTransit        = KEYTRANS_PRESSED;
                        rgKeyControl[Key].wLastSentTime_ms  = wActTime_ms;
                        rgKeyControl[Key].wDur_ms = wActTime_ms - rgKeyControl[Key].wStartTime_ms;
                        DigInDrv_SendKeyMessage(Key, &rgKeyControl[Key]);
                        //ODS1(DBG_SYS,DBG_INFO,"New KeyPress: Key#%u",Key);
                    }
                }
                /* PRESSED LONGER TIME! check duration time */
                else
                {
                    /* check if pressed detected 2nd time : */
                    if ( rgKeyControl[Key].KeyTransit == KEYTRANS_PRESSED)
                         wWaitTime = KEYTM_PRESSED_SHORT;      /* this is the second key event */
                    else wWaitTime = KEYTM_MSGREPRATE;    /* there have been more key events */

                    /* handle 'first delay' & 'repetition rate' */
                    if ( (wActTime_ms - rgKeyControl[Key].wLastSentTime_ms) > wWaitTime)
                    {
                        rgKeyControl[Key].wDur_ms = wActTime_ms - rgKeyControl[Key].wStartTime_ms;
                        rgKeyControl[Key].KeyTransit = KEYTRANS_ON;
                        rgKeyControl[Key].wLastSentTime_ms = wActTime_ms;
                        DigInDrv_SendKeyMessage(Key, &rgKeyControl[Key]);
                        //ODS1(DBG_SYS,DBG_INFO,"KeyPress repetition: Key#%u",Key);
                    }
                }
                /* save common duration of pressed keys only */
                wCommonDuration_ms = MAX(wCommonDuration_ms, rgKeyControl[Key].wDur_ms);
            }
            /* KEY NOT PRESSED! reset duration */
            else
            {
                /* key has just at the moment been released? */
                if (rgKeyControl[Key].wDur_ms > 0)
                {
                    /* RELEASED AT THIS MOMENT! */
                    rgKeyControl[Key].KeyTransit = KEYTRANS_RELEASED;
                    DigInDrv_SendKeyMessage(Key, &rgKeyControl[Key]);
                    rgKeyControl[Key].wDur_ms = 0;     // reset AFTER sending duration !
                    //ODS1(DBG_SYS,DBG_INFO,"KeyReleased: Key#%u",Key);
                }
                else
                {
                    /* NO ACTION FOR A LONGER PERIOD */
                    rgKeyControl[Key].wDur_ms = 0;
                    rgKeyControl[Key].KeyTransit = KEYTRANS_OFF;
                }
            }
        } /* of 'check all buttons' */
    }

    /* ---------------------------------------------------- */
    /* generate an extra message, if MULTIPLE keys pressed */
    if (bKeyCounter > 1)
    {
        /* MULTIPLE KEYS PRESSED! first press? */
        if (MultiKeyControl.wDur_ms == 0)
        {
            /* PRESSED FIRST TIME! set start time and minimal duration */
            MultiKeyControl.wStartTime_ms     = wActTime_ms;
            MultiKeyControl.KeyTransit        = KEYTRANS_PRESSED;
            MultiKeyControl.wDur_ms      = KEYTM_MIN;
            MultiKeyControl.wLastSentTime_ms  = wActTime_ms;
            MSG_BUILD_UINT8(    KeyMsg,
                                MSG_KEYS_PRESSED,
                                LOWBYTE(wCommonDuration_ms),
                                HIGHBYTE(wCommonDuration_ms),
                                bKeyState);
            RValue = MsgQPostMsg(KeyMsg, MSGQ_PRIO_HIGH);
            //ODS1(DBG_SYS,DBG_INFO,"New Multi-KeyPress: Keys 0x%x",bKeyState);
        }
        /* MULTIPLE KEYS PRESSED LONGER TIME! check duration time */
        else
        {
            /* check if pressed detected 2nd time : */
            if ( MultiKeyControl.KeyTransit == KEYTRANS_PRESSED)
                 wWaitTime = KEYTM_PRESSED_SHORT;      /* this is the second key event */
            else wWaitTime = KEYTM_MSGREPRATE;    /* there have been more key events */
            /* handle 'first delay' & 'repitition rate' */
            if ( (wActTime_ms - MultiKeyControl.wLastSentTime_ms) > wWaitTime)
            {
                MultiKeyControl.wDur_ms = wActTime_ms - MultiKeyControl.wStartTime_ms;
                MultiKeyControl.KeyTransit = KEYTRANS_ON;
                MultiKeyControl.wLastSentTime_ms = wActTime_ms;
                MSG_BUILD_UINT8(    KeyMsg,
                                    MSG_KEYS_PRESSED,
                                    LOWBYTE(wCommonDuration_ms),
                                    HIGHBYTE(wCommonDuration_ms),
                                    bKeyState);
                RValue = MsgQPostMsg(KeyMsg, MSGQ_PRIO_HIGH);
                //ODS1(DBG_SYS,DBG_INFO,"Multi-KeyPress repetition: Keys 0x%x",bKeyState);
            }
        }
    }
    /* MULTIPLE KEYS NOT PRESSED! reset duration */
    else
    {
        /* key has just at the moment been released? */
        if (MultiKeyControl.wDur_ms > 0)
        {
            /* RELEASED AT THIS MOMENT! */
            MultiKeyControl.KeyTransit = KEYTRANS_RELEASED;
            MSG_BUILD_UINT8(    KeyMsg,
                                MSG_KEYS_PRESSED,
                                LOWBYTE(wCommonDuration_ms),
                                HIGHBYTE(wCommonDuration_ms),
                                bKeyState);
            RValue = MsgQPostMsg(KeyMsg, MSGQ_PRIO_HIGH);
            MultiKeyControl.wDur_ms = 0;   // reset AFTER sending duration !
            //ODS1(DBG_SYS,DBG_INFO,"Multi-KeyReleased: Keys 0x%x",bKeyState);
        }
        else
        {
            /* NO ACTION FOR A LONGER PERIOD */
            MultiKeyControl.wDur_ms = 0;
            MultiKeyControl.KeyTransit = KEYTRANS_OFF;
        }
    }
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       DigInDrv_SendKeyMessage
 *  DESCRIPTION:    Sends a message for one key - according
 *                  to actual and past keyboard state;
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE DigInDrv_SendKeyMessage(const KEYNUMBER Key, const KEYTIME far * fpKeyData)
{
    ERRCODE     RValue = ERR_OK;
    MESSAGE     KeyMsg;

    /* prepare to send a message */
    switch (Key)
    {
        case KEY_OK:
            MSG_BUILD_KEY(KeyMsg, MSG_KEY_OK, fpKeyData);
            break;
        case KEY_UP:
            MSG_BUILD_KEY(KeyMsg, MSG_KEY_UP, fpKeyData);
            break;
        case KEY_DOWN:
            MSG_BUILD_KEY(KeyMsg, MSG_KEY_DOWN, fpKeyData);
            break;
        default: RValue = ERR_PARAM_ERR;
    }

    /* send message */
    if (RValue == ERR_OK)
        RValue = MsgQPostMsg(KeyMsg, MSGQ_PRIO_HIGH);
    return RValue;
}


/***********************************************************************
 *  FUNCTION:       DigInDrv_CheckAllPorts
 *  DESCRIPTION:    check all other digital in ports
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Bike specific handling is handled in surveillance
 *                  modules SurvProcessAll()
 *********************************************************************** */
void DigInDrv_CheckAllPorts(void)
{
    /* Update all digital filter values ========================== */
    DigInDrv_Filter();

    /* TURN LED ------------------------------------------- */
    if (  (DF_TURNL == 1)                // high active
        ||(DF_TURNR == 1) )              // high active
         DigOutDrv_SetPin( eDIGOUT_LED_TURN, TRUE);
    else DigOutDrv_SetPin( eDIGOUT_LED_TURN, FALSE);

    /* NEUTRAL LED ------------------------------------------- */
    if (DF_NEUTR == 0)                 // low active
         DigOutDrv_SetPin( eDIGOUT_LED_NEUTR, TRUE);
    else DigOutDrv_SetPin( eDIGOUT_LED_NEUTR, FALSE);

    /* HIGHBEAM LED ------------------------------------------- */
    if (DF_HBEAM == 1)                   // high active
         DigOutDrv_SetPin( eDIGOUT_LED_HBEAM, TRUE);
    else DigOutDrv_SetPin( eDIGOUT_LED_HBEAM, FALSE);

}


/***********************************************************************
 *  FUNCTION:       DigInDrv_GetHWVersion
 *  DESCRIPTION:    returns hardware version number, given by
 *                  external pulldown resistors at p4_0/1/2
 *  PARAMETER:      -
 *  RETURN:         UINT8       hw version, valid 0..7
 *  COMMENT:        M16C internal pullup resistors are used to detect
 *                  not existing external pull-downs. So DigInDrv_Init()
 *                  must have been called first before using this function!
 *********************************************************************** */
UINT8 DigInDrv_GetHWVersion(void)
{
    return (HWVerPort & HWVERS_PINS);   // get port status, mask hw version pins only
}



/***********************************************************************
 *  FUNCTION:       DigInDrv_Filter
 *  DESCRIPTION:    updates digital filtered values for all digital
 *                  input ports
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void DigInDrv_Filter(void)
{
    DIGFILTELEMENT elem;
    UINT8          input;

    // loop over all digital inpus
    for (elem = eDF_TURNL; elem<eDF_GPI_3; elem++)
    {
        // get current input value
        switch (elem)
        {   case eDF_TURNL: input = DigIn_TurnL  ; break;
            case eDF_TURNR: input = DigIn_TurnR  ; break;
            case eDF_OILSW: input = DigIn_OilSw  ; break;
            case eDF_NEUTR: input = DigIn_Neutral; break;
            case eDF_HBEAM: input = DigIn_HBeam  ; break;
            case eDF_GPI_0: input = DigIn_GPI_0  ; break;
            case eDF_GPI_1: input = DigIn_GPI_1  ; break;
            case eDF_GPI_2: input = DigIn_GPI_2  ; break;
            case eDF_GPI_3: input = DigIn_GPI_3  ; break;
            default: input = 0; break;
        }

        // check current input value: count up/down?
        if ( input == 1 )             // current input: HIGH?
             DigInFilter[elem].swFiltValue += (INT16) DigInFilter[elem].bIncrValue; // count UP!
        else DigInFilter[elem].swFiltValue -= (INT16) DigInFilter[elem].bDecrValue; // count DOWN!

        // limit value & detect threshold (0/255)
        if ( DigInFilter[elem].swFiltValue > UINT8_MAX)
        {
            DigInFilter[elem].swFiltValue = UINT8_MAX;
            DigInFilter[elem].fState = 1;
        }
        else if ( DigInFilter[elem].swFiltValue < 0)
        {
            DigInFilter[elem].swFiltValue = 0;
            DigInFilter[elem].fState = 0;
        }
    }
}




/***********************************************************************
 *  FUNCTION:       DigInDrv_FilterInit
 *  DESCRIPTION:    bike specific digital filter initialization
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void    DigInDrv_FilterInit(void)
{
    DIGFILTELEMENT elem;

    // reset all filter entries to common default filter settings
    for (elem = eDF_TURNL; elem < eDF_LAST; elem++)
    {
        DigInFilter[elem].bIncrValue    = DigInDrv_FilterConvertTime( DIGFILTM_DEF );
        DigInFilter[elem].bDecrValue    = DigInDrv_FilterConvertTime( DIGFILTM_DEF );
        DigInFilter[elem].swFiltValue   = 0;
        DigInFilter[elem].fState        = FALSE;
    }

    // add bike specific filter settings
    switch(gBikeType)
    {
        case eBIKE_F650:
        {
            // DF_Temp_Warn_F650 filter values - no special filtering
            // DF_ABS_Warn_F650 - no special filtering

            // DF_Fuel_4l_F650 filter values
            DigInFilter[eDF_GPI_1].bIncrValue = DigInDrv_FilterConvertTime( DIGFILTM_FUEL_HIGH );
            DigInFilter[eDF_GPI_1].bDecrValue = DigInDrv_FilterConvertTime( DIGFILTM_FUEL_LOW  );

            ODS(DBG_DRV,DBG_INFO,"DigInDrv_FilterInit() set to eBIKE_F650!");
        } break;

        case eBIKE_AFRICATWIN:
        {
            // DF_Fuel_8l_AT filter values
            DigInFilter[eDF_GPI_0].bIncrValue = DigInDrv_FilterConvertTime( DIGFILTM_FUEL_HIGH );
            DigInFilter[eDF_GPI_0].bDecrValue = DigInDrv_FilterConvertTime( DIGFILTM_FUEL_LOW  );

            // DF_Fuel_4l_AT filter values
            DigInFilter[eDF_GPI_1].bIncrValue = DigInDrv_FilterConvertTime( DIGFILTM_FUEL_HIGH );
            DigInFilter[eDF_GPI_1].bDecrValue = DigInDrv_FilterConvertTime( DIGFILTM_FUEL_LOW  );

            ODS(DBG_DRV,DBG_INFO,"DigInDrv_FilterInit() set to eBIKE_AFRICATWIN!");
        } break;

        default:
            ODS(DBG_DRV,DBG_INFO,"DigInDrv_FilterInit() set to default settings!");
            break;
    }
}


/***********************************************************************
 *  FUNCTION:       DigInDrv_FilterConvertTime
 *  DESCRIPTION:    converts filter time into incr/decr value for filter
 *  PARAMETER:      wFilterTime     in ms
 *  RETURN:         incr/decr       value to be used in filter
 *  COMMENT:        helper function
 *********************************************************************** */
UINT8   DigInDrv_FilterConvertTime(UINT16 wFilterTime)
{
    UINT16 rvalue;

    // check limits
    wFilterTime = MIN(wFilterTime, DIGFILTM_MAX);
    wFilterTime = MAX(wFilterTime, DIGFILTM_MIN);

    // calculate incr/der value
    rvalue = UINT8_MAX * MS_PER_TICK / wFilterTime;

    // limit result to UINT8
    if ( rvalue > UINT8_MAX )
        rvalue = UINT8_MAX;    // indicates 'no filter', direct switch
    if ( rvalue == 0 )
        rvalue = 1;            // indicates slowest possible filter
    return ((UINT8) rvalue);
}



/***********************************************************************
 *  FUNCTION:       DigInDrv_GetGPIMeas
 *  DESCRIPTION:    return complete GPI_x measurement status in a structure
 *  PARAMETER:      eGpi            GPI port to be returned
 *  RETURN:         reference       to DIGINTMEAS structure
 *  COMMENT:        Updates calculated values on each call
 *********************************************************************** */
DIGINTMEAS far * DigInDrv_GetGPIMeas(DIGINTMEAS_GPI eGpi)
{
    /* check parameter */
    if ( eGpi >= eGPI_MAX )
    {   ODS(DBG_DRV,DBG_INFO,"DigInDrv_FilterInit() set to default settings!");
        return 0L;
    }

    /* update calculated values */
    DigIntMeas[eGpi].wPulseCycle = DigIntMeas[eGpi].wHighWidth + DigIntMeas[eGpi].wLowWidth;
    DigIntMeas[eGpi].wPulseFreq  = ( 1000 / DigIntMeas[eGpi].wPulseCycle) / 1000;
    DigIntMeas[eGpi].ucPWM       = ((1000 * DigIntMeas[eGpi].wPulseCycle) / DigIntMeas[eGpi].wHighWidth) / 1000 * 100;
    switch(eGpi)
    {   case eGPI0_Int2: DigIntMeas[eGpi].fCurrState = DigIn_GPI_0; break;
        case eGPI1_Int3: DigIntMeas[eGpi].fCurrState = DigIn_GPI_1; break;
        case eGPI2_Int4: DigIntMeas[eGpi].fCurrState = DigIn_GPI_2; break;
        case eGPI3_Int5: DigIntMeas[eGpi].fCurrState = DigIn_GPI_3; break;
        default: break;
    }

    /* correct calculated values, if missing interrupts for long time */
    if ( ( wMilliSecCounter - DigIntMeas[eGpi].wLastHLTrans ) > DIGINTMEAS_TIMEOUT )
    {
        /* TOO old events: check 0% / 100% */
        if ( DigIntMeas[eGpi].fCurrState == DIGIN_HIGH )
             DigIntMeas[eGpi].ucPWM = 100;
        else DigIntMeas[eGpi].ucPWM = 0;
    }

    /* return results */
    return ( &DigIntMeas[eGpi]);
}


/***********************************************************************
 * FUNCTION:       GPI0_Int2_ISR
 * DESCRIPTION:    HW Interrupt generated by/at GPI0,
 *                 stimulated at every high/low transition
 * PARAMETER:      -
 * RETURN:         -
 * COMMENT:        Also added in 'sect30.inc'
 *                 General purpose function to measure pulse width
 *                 and/or count impulses at this pin.
 *                 Application software is responsible to further
 *                 process this data.
 *                 Note: SHOULD RETURN AS FAST AS POSSIBLE!
 *                 Note: Does NOT re-enable higher interrupts,
 *                 because we here read the high prior system timer!
 *********************************************************************** */
#pragma INTERRUPT GPI0_Int2_ISR        // declare as M16C isr
void GPI0_Int2_ISR(void)
{
    /* select kind of edge (Low<->High) & update our measurement */
    if ( DigIn_GPI_0 == 1 )
    {   /* H -> L edge: calculate HIGH pulse width & save new time stamp */
        DigIntMeas[eGPI0_Int2].wHighWidth   = wMilliSecCounter - DigIntMeas[eGPI0_Int2].wLastLHTrans;
        DigIntMeas[eGPI0_Int2].wLastHLTrans = wMilliSecCounter;
    }
    else
    {   /* L -> H edge: calculate HIGH pulse width & save new time stamp */
        DigIntMeas[eGPI0_Int2].wLowWidth    = wMilliSecCounter - DigIntMeas[eGPI0_Int2].wLastHLTrans;
        DigIntMeas[eGPI0_Int2].wLastLHTrans = wMilliSecCounter;

        /* addtionally count this transition */
        DigIntMeas[eGPI0_Int2].dwLHCounter++;
    }
}


/***********************************************************************
 * FUNCTION:       GPI1_Int3_ISR
 * DESCRIPTION:    HW Interrupt generated by/at GPI1,
 *                 stimulated at every high/low transition
 * PARAMETER:      -
 * RETURN:         -
 * COMMENT:        Also added in 'sect30.inc'
 *                 General purpose function to measure pulse width
 *                 and/or count impulses at this pin.
 *                 Application software is responsible to further
 *                 process this data.
 *                 Note: SHOULD RETURN AS FAST AS POSSIBLE!
 *                 Note: Does NOT re-enable higher interrupts,
 *                 because we here read the high prior system timer!
 *********************************************************************** */
#pragma INTERRUPT GPI1_Int3_ISR        // declare as M16C isr
void GPI1_Int3_ISR(void)
{
    /* select kind of edge (Low<->High) & update our measurement */
    if ( DigIn_GPI_1 == 1 )
    {   /* H -> L edge: calculate HIGH pulse width & save new time stamp */
        DigIntMeas[eGPI1_Int3].wHighWidth   = wMilliSecCounter - DigIntMeas[eGPI1_Int3].wLastLHTrans;
        DigIntMeas[eGPI1_Int3].wLastHLTrans = wMilliSecCounter;
    }
    else
    {   /* L -> H edge: calculate HIGH pulse width & save new time stamp */
        DigIntMeas[eGPI1_Int3].wLowWidth    = wMilliSecCounter - DigIntMeas[eGPI1_Int3].wLastHLTrans;
        DigIntMeas[eGPI1_Int3].wLastLHTrans = wMilliSecCounter;

        /* addtionally count this transition */
        DigIntMeas[eGPI1_Int3].dwLHCounter++;
    }
}


/***********************************************************************
 * FUNCTION:       GPI2_Int4_ISR
 * DESCRIPTION:    HW Interrupt generated by/at GPI2,
 *                 stimulated at every high/low transition
 * PARAMETER:      -
 * RETURN:         -
 * COMMENT:        Also added in 'sect30.inc'
 *                 General purpose function to measure pulse width
 *                 and/or count impulses at this pin.
 *                 Application software is responsible to further
 *                 process this data.
 *                 Note: SHOULD RETURN AS FAST AS POSSIBLE!
 *                 Note: Does NOT re-enable higher interrupts,
 *                 because we here read the high prior system timer!
 *********************************************************************** */
#pragma INTERRUPT GPI2_Int4_ISR        // declare as M16C isr
void GPI2_Int4_ISR(void)
{
    /* select kind of edge (Low<->High) & update our measurement */
    if ( DigIn_GPI_2 == 1 )
    {   /* H -> L edge: calculate HIGH pulse width & save new time stamp */
        DigIntMeas[eGPI2_Int4].wHighWidth   = wMilliSecCounter - DigIntMeas[eGPI2_Int4].wLastLHTrans;
        DigIntMeas[eGPI2_Int4].wLastHLTrans = wMilliSecCounter;
    }
    else
    {   /* L -> H edge: calculate HIGH pulse width & save new time stamp */
        DigIntMeas[eGPI2_Int4].wLowWidth    = wMilliSecCounter - DigIntMeas[eGPI2_Int4].wLastHLTrans;
        DigIntMeas[eGPI2_Int4].wLastLHTrans = wMilliSecCounter;

        /* addtionally count this transition */
        DigIntMeas[eGPI2_Int4].dwLHCounter++;
    }
}



/***********************************************************************
 * FUNCTION:       GPI3_Int5_ISR
 * DESCRIPTION:    HW Interrupt generated by/at GPI3,
 *                 stimulated at every high/low transition
 * PARAMETER:      -
 * RETURN:         -
 * COMMENT:        Also added in 'sect30.inc'
 *                 General purpose function to measure pulse width
 *                 and/or count impulses at this pin.
 *                 Application software is responsible to further
 *                 process this data.
 *                 Note: SHOULD RETURN AS FAST AS POSSIBLE!
 *                 Note: Does NOT re-enable higher interrupts,
 *                 because we here read the high prior system timer!
 *********************************************************************** */
#pragma INTERRUPT GPI3_Int5_ISR        // declare as M16C isr
void GPI3_Int5_ISR(void)
{
    /* select kind of edge (Low<->High) & update our measurement */
    if ( DigIn_GPI_3 == 1 )
    {   /* H -> L edge: calculate HIGH pulse width & save new time stamp */
        DigIntMeas[eGPI3_Int5].wHighWidth   = wMilliSecCounter - DigIntMeas[eGPI3_Int5].wLastLHTrans;
        DigIntMeas[eGPI3_Int5].wLastHLTrans = wMilliSecCounter;
    }
    else
    {   /* L -> H edge: calculate HIGH pulse width & save new time stamp */
        DigIntMeas[eGPI3_Int5].wLowWidth    = wMilliSecCounter - DigIntMeas[eGPI3_Int5].wLastHLTrans;
        DigIntMeas[eGPI3_Int5].wLastLHTrans = wMilliSecCounter;

        /* addtionally count this transition */
        DigIntMeas[eGPI3_Int5].dwLHCounter++;
    }
}





