/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-09 by Ralf Krizsan
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
 *      Arnold Neugebauer                   Ralf Krizsan
 *      St�ckener Str. 115                  Knickstr. 10
 *      30419 Hannover                      30890 Barsinghausen
 *      arnold.neugebauer@web.de            ralf.krizsan@web.de
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
 * Revision 1.1  2006/03/18 12:05:18  tuberkel
 * Initial revision
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
#include "leddrv.h"
#include "led.h"
#include "digindrv.h"
#include "device.h"
#include "anaindrv.h"

/* external vars */
extern UINT16  wMilliSecCounter;            /* valid values: 0h .. ffffh */


/* module global vars */
static KEYTIME   rgKeyControl[KEY_LAST];    /* protocol of duration & start time */
static KEYTIME   MultiKeyControl;           /* protocol for multiple pressed keys */


/* KD30 support */
#ifdef MINIEMU
unsigned char kd30_Ok_Pressed = 0;
unsigned char kd30_Up_Pressed = 0;
unsigned char kd30_Down_Pressed = 0;
unsigned char kd30_UpDown_Pressed = 0;
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

    /* set pin direction register */
    KeyPortD_Ok     = 0;   /* OK Button direction bit to INPUT */
    KeyPortD_Down   = 0;   /* UP Button direction bit to INPUT */
    KeyPortD_Up     = 0;   /* DOWN Button direction bit to INPUT */

    /* init key pressed time protocol */
    for (i=0; i<KEY_LAST; i++)
    {
        rgKeyControl[i].wStartTime_ms     = 0;
        rgKeyControl[i].wLastSentTime_ms  = 0;
        rgKeyControl[i].wDuration_ms      = 0;
    }
    MultiKeyControl.wStartTime_ms     = 0;
    MultiKeyControl.wLastSentTime_ms  = 0;
    MultiKeyControl.wDuration_ms      = 0;

    /* we use the a isr to quickly get WHEEL & RPM infos,
       but NOT for keyboard reading */
    /* enable DigitalInputISR */
    INT_GLOB_DISABLE;
    int2ic     = 0x00;  /* int2 interrupt control register settings:
                            - level set to 0 = disabled!
                            - polarity flag to 'falling edge'
                            - request cause select register flag to 'one edge' (ifsr2=0 default) */
    INT_GLOB_ENABLE;

    /* other digital in init stuff */
    pd2 &= ~DIGIN_ALL_P2;   // set all digin pins to INPUT

    /* special handling for alternator warning */
    pd10_4 = 0;             // digital input

    /* HW version detection */
    HWVerPortD &= ~HWVERS_PINS;     // assure 3 HW-Version pins to input only
    pu10 = 1;                       // activate M16C internal pull-ups for p4_0 .. p4_3

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

#ifdef MINIEMU
    /* check value given by KD30 */
    if (kd30_Ok_Pressed)
        RValue |= KEYFL_OK;
    if ((kd30_Up_Pressed) || (kd30_UpDown_Pressed))
        RValue |= KEYFL_UP;
    if ((kd30_Down_Pressed) || (kd30_UpDown_Pressed))
        RValue |= KEYFL_DOWN;
    /* make a 'longer press' by decr. until 0 reached */
    if (kd30_Ok_Pressed)
        kd30_Ok_Pressed--;
    if (kd30_Up_Pressed)
        kd30_Up_Pressed--;
    if (kd30_Down_Pressed)
        kd30_Down_Pressed--;
    if (kd30_UpDown_Pressed)
        kd30_UpDown_Pressed--;
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
                    if (rgKeyControl[Key].wDuration_ms <= MINDURWAIT )
                    {
                        /* set start time and minimal duration */
                        /* BUT do net send msg now! */
                        if (rgKeyControl[Key].wDuration_ms == 0)
                        {
                            rgKeyControl[Key].wStartTime_ms = wActTime_ms;
                            rgKeyControl[Key].wDuration_ms  = MINDUR;
                        }
                        else rgKeyControl[Key].wDuration_ms = wActTime_ms - rgKeyControl[Key].wStartTime_ms;
                    }
                    else
                    {
                        /* check for minimal time period (avoid jitter) */
                        /* and THEN send message */
                        rgKeyControl[Key].KeyTransit        = KEYTRANS_PRESSED;
                        rgKeyControl[Key].wLastSentTime_ms  = wActTime_ms;
                        rgKeyControl[Key].wDuration_ms = wActTime_ms - rgKeyControl[Key].wStartTime_ms;
                        DigInDrv_SendKeyMessage(Key, &rgKeyControl[Key]);
                        //ODS1(DBG_SYS,DBG_INFO,"New KeyPress: Key#%u",Key);
                    }
                }
                /* PRESSED LONGER TIME! check duration time */
                else
                {
                    /* check if pressed detected 2nd time : */
                    if ( rgKeyControl[Key].KeyTransit == KEYTRANS_PRESSED)
                         wWaitTime = KEYSHORT;      /* this is the second key event */
                    else wWaitTime = KEYREPRATE;    /* there have been more key events */

                    /* handle 'first delay' & 'repetition rate' */
                    if ( (wActTime_ms - rgKeyControl[Key].wLastSentTime_ms) > wWaitTime)
                    {
                        rgKeyControl[Key].wDuration_ms = wActTime_ms - rgKeyControl[Key].wStartTime_ms;
                        rgKeyControl[Key].KeyTransit = KEYTRANS_ON;
                        rgKeyControl[Key].wLastSentTime_ms = wActTime_ms;
                        DigInDrv_SendKeyMessage(Key, &rgKeyControl[Key]);
                        //ODS1(DBG_SYS,DBG_INFO,"KeyPress repetition: Key#%u",Key);
                    }
                }
                /* save common duration of pressed keys only */
                wCommonDuration_ms = MAX(wCommonDuration_ms, rgKeyControl[Key].wDuration_ms);
            }
            /* KEY NOT PRESSED! reset duration */
            else
            {
                /* key has just at the moment been released? */
                if (rgKeyControl[Key].wDuration_ms > 0)
                {
                    /* RELEASED AT THIS MOMENT! */
                    rgKeyControl[Key].KeyTransit = KEYTRANS_RELEASED;
                    DigInDrv_SendKeyMessage(Key, &rgKeyControl[Key]);
                    rgKeyControl[Key].wDuration_ms = 0;     // reset AFTER sending duration !
                    //ODS1(DBG_SYS,DBG_INFO,"KeyReleased: Key#%u",Key);
                }
                else
                {
                    /* NO ACTION FOR A LONGER PERIOD */
                    rgKeyControl[Key].wDuration_ms = 0;
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
        if (MultiKeyControl.wDuration_ms == 0)
        {
            /* PRESSED FIRST TIME! set start time and minimal duration */
            MultiKeyControl.wStartTime_ms     = wActTime_ms;
            MultiKeyControl.KeyTransit        = KEYTRANS_PRESSED;
            MultiKeyControl.wDuration_ms      = MINDUR;
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
                 wWaitTime = KEYSHORT;      /* this is the second key event */
            else wWaitTime = KEYREPRATE;    /* there have been more key events */
            /* handle 'first delay' & 'repitition rate' */
            if ( (wActTime_ms - MultiKeyControl.wLastSentTime_ms) > wWaitTime)
            {
                MultiKeyControl.wDuration_ms = wActTime_ms - MultiKeyControl.wStartTime_ms;
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
        if (MultiKeyControl.wDuration_ms > 0)
        {
            /* RELEASED AT THIS MOMENT! */
            MultiKeyControl.KeyTransit = KEYTRANS_RELEASED;
            MSG_BUILD_UINT8(    KeyMsg,
                                MSG_KEYS_PRESSED,
                                LOWBYTE(wCommonDuration_ms),
                                HIGHBYTE(wCommonDuration_ms),
                                bKeyState);
            RValue = MsgQPostMsg(KeyMsg, MSGQ_PRIO_HIGH);
            MultiKeyControl.wDuration_ms = 0;   // reset AFTER sending duration !
            //ODS1(DBG_SYS,DBG_INFO,"Multi-KeyReleased: Keys 0x%x",bKeyState);
        }
        else
        {
            /* NO ACTION FOR A LONGER PERIOD */
            MultiKeyControl.wDuration_ms = 0;
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
 *  COMMENT:        -
 *********************************************************************** */
void DigInDrv_CheckAllPorts(void)
{
    /* DEFAULT HANDLING FOR ALL BIKES ============================= */

    /* TURN LED ------------------------------------------- */
    if (  (DigIn_TurnL == 1)                // high active
        ||(DigIn_TurnR == 1) )              // high active
         LEDDrvSetLED(LEDDRV_TURN, TRUE);
    else LEDDrvSetLED(LEDDRV_TURN, FALSE);

    /* NEUTRAL LED ------------------------------------------- */
    if (DigIn_Neutral == 0)                 // low active
         LEDDrvSetLED(LEDDRV_NEUTR, TRUE);
    else LEDDrvSetLED(LEDDRV_NEUTR, FALSE);

    /* HIGHBEAM LED ------------------------------------------- */
    if (DigIn_HBeam == 1)                   // high active
         LEDDrvSetLED(LEDDRV_BEAM, TRUE);
    else LEDDrvSetLED(LEDDRV_BEAM, FALSE);

}


/***********************************************************************
 *  FUNCTION:       DigInDrv_GetHWVersion
 *  DESCRIPTION:    returns hardware version number, given by
 *                  external pulldown resistors at p4_0/1/2
 *  PARAMETER:      -
 *  RETURN:         UINT8       hw version, valid 0..7
 *  COMMENT:        M16C internal pullup resistors are used to detect
 *                  not existing external pull-downs.
 *********************************************************************** */
UINT8 DigInDrv_GetHWVersion(void)
{
    return (HWVerPort & HWVERS_PINS);   // get port status, mask hw version pins only
}
