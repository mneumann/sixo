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
 *  Module:         message queue
 *  Purpose:        message defintions
 *  Comments:       -
 *
 ************************************************************************ */
#ifndef _STDMSGS_H
#define _STDMSGS_H


/* define MESSAGE_ID as UINT8 */
typedef enum
{
    /* system ----------------------------------------------------- */
    MSG_NULL_MSG,                       /* invalid  message */
    MSG_TIMER,                          /* timer message */

    /* keyboard ----------------------------------------------------- */
    MSG_KEY_OK,                         /* [OK]     key detected */
    MSG_KEY_UP,                         /* [UP]     key detected */
    MSG_KEY_DOWN,                       /* [DOWN]   key detected */
    MSG_KEYS_PRESSED,                   /* (multiple) keys pressed (param: 8 bit key mask) */

    /* display ----------------------------------------------------- */
    MSG_DPL_FLASH_ON,                   /* display cursor flashes ON */
    MSG_DPL_FLASH_OFF,                  /* display cursor flashes OFF */

    /* leds ----------------------------------------------------- */
    MSG_LED_SET,                        /* set new led activation (pulse width & freq.) status */
    MSG_LED_ON,                         /* simply switches led ON */
    MSG_LED_OFF,                        /* simply switches led OFF */

    /* beeper ----------------------------------------------------- */
    MSG_BEEP_SET,                       /* set new beeper activation (pulse width & freq.) status */
    MSG_BEEP_ON,                        /* simply switches beeper ON */
    MSG_BEEP_OFF,                       /* simply switches beeper OFF */

    /* devices ----------------------------------------------------- */
    MSG_GET_FOCUS,                      /* device request to get focus from currently focused device */
    MSG_SET_FOCUS,                      /* give focus to another device */
    MSG_SCREEN_REFRESH,                 /* system advice to update screen data of focused device */

    /* APPEND MORE STUFF HERE */


    /* last message check ------------------------------------------- */
    MSG_LAST_MESSAGE                    /* last - unused! - message */

} MESSAGE_ID;






#endif // _STDMSGS_H
