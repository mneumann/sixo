/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-10-25 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         message queue
 *  Purpose:        message defintions
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
 *      Arnold Neugebauer                   Ralf Krizsan
 *      Stöckener Str. 115                  Knickstr. 10
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
 * Revision 2.1  2009/06/21 21:21:32  tuberkel
 * Changes done by AN:
 * New MSG_COMPASS_REFRESH Message
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
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
    MSG_MAINDEV_VEHIC_STATE_SHOW,       /* Main-Device: Show Vehicle (Error) State for defined time */
    MSG_MAINDEV_VEHIC_STATE_HIDE,       /* Main-Device: Hide Vehicle (Error) State */

    /* special time/date messages ---------------------------------- */
    MSG_TIMEDATE_SECOND_GONE,           /* to snychronize sconds on display */

    /* compass messages -------------------------------------------- */
    MSG_COMPASS_REFRESH,                /* data from compass has been updated */
    
    /* last message check ------------------------------------------- */
    MSG_LAST_MESSAGE                    /* last - unused! - message */

} MESSAGE_ID;






#endif // _STDMSGS_H
