/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-10-22 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         message queue
 *  Purpose:        message queue handling and interface
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
 * Revision 3.5  2012/05/27 16:01:42  tuberkel
 * All Eeprom/Nvram Variables renamed
 *
 * Revision 3.4  2012/02/06 20:54:14  tuberkel
 * Just renamed all 'Devices' function prefixes for better readability
 *
 * Revision 3.3  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 3.2  2012/01/14 10:26:44  tuberkel
 * LED PWM handling changed:
 * - no longer Msgs/TimerMsgs used (inaccurate optic)
 * - instead use TimerISR to control PWM
 * - Granulartiy is SystemTicks (20 ms)
 * - works well
 * - prevent direct LEDDrv access (if possible)
 *
 * Revision 3.1  2012/01/14 08:28:42  tuberkel
 * Message-IDs shortened / reviewed
 *
 * Revision 3.0  2010/11/07 14:00:50  tuberkel
 * V30 Preparations - no changes
 *
 * Revision 2.4  2009/07/15 09:00:52  tuberkel
 * NEW: #define TESTSCREEN for GUI Tests
 *
 * Revision 2.3  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.2  2009/07/08 21:41:17  tuberkel
 * All compiler defines reviewed:
 * - set to unique usage: set define to 0 or 1
 * - default values set, if not used
 * - see 'Project Editor' for details
 *
 * Revision 2.1  2009/06/21 17:56:44  tuberkel
 * Changes done by AN:
 * New MSG_COMPASS_RFRSH Message
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.6  2006/02/18 14:58:39  Ralf
 * - BIKE_MOTOBAU: lapcounter and statistic devices added
 *
 *
 ************************************************************************ */


#include <string.h>
#include "m16c.h"
#include "standard.h"
#include "stdmsgs.h"
#include "msgqueue.h"
#include "stdmsgs.h"
#include "debug.h"
#include "device.h"



/* local symbols */
MESSAGE grgMessageQueue[LENGTH_MSGQUEUE];   // MessageQueue with the Index 0..(LENGTH_MSGQUEUE-1)
UINT8   gbMsgQueueRead;                     // Index of next entry to read
UINT8   gbMsgQueueWrite;                    // Index of next location to write.
UINT8   gbMsgQueueMax;                      // global definition max length of queue
UINT8   gbMsgQueueCurrent;                  // current elements in queue

// temporary modul specific message entry functions
MSGENTRYFCT TestMessageEntry;


// optional debug outs (default: off)
#ifndef DBGMSGQ
#define DBGMSGQ 0
#endif



/***********************************************************************
 *  FUNCTION:       MsgQInit
 *  DESCRIPTION:    Initializes messagequeue at startup
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MsgQInit(void)
{
   gbMsgQueueWrite  = 0;
   gbMsgQueueRead   = 0;
   gbMsgQueueCurrent= 0;
   gbMsgQueueMax    = 0;
   ODS(DBG_SYS,DBG_INFO,"MsgQInit() done!");
   return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       MsgQPostMsg
 *  DESCRIPTION:    Writes message into message queue
 *  PARAMETER:      MESSAGE     message to post
 *                              priority of message (MSGQ_PRIO_HIGH / MSGQ_PRIO_LOW)
 *  RETURN:         - ERR_OK if successful
 *                  - ERR_MSGQUEUE_NO_WRITE if not written because of low priority
 *                    or message id is MSG_NULL_MSG
 *                  - ERR_MSGQUEUE_MAX_FULL if queue full
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MsgQPostMsg(MESSAGE msg, INT8 chPriority)
{
   if ( MSG_ID(msg) == MSG_NULL_MSG )
   {
      // nothing to post
      ODS(DBG_SYS,DBG_WARNING,"NULL msg!");
      return ERR_MSGQUEUE_NO_WRITE;
   }

   if ( gbMsgQueueCurrent == LENGTH_MSGQUEUE )
   {
      // no space to post
      ODS(DBG_SYS,DBG_FATAL,"Q full!");
      return ERR_MSGQUEUE_MAX_FULL;
   }

   // check if message queue nearly full
   if ( gbMsgQueueCurrent > FULL_MSGQUEUE )
   {
      if ( chPriority != MSGQ_PRIO_HIGH )
      {
         // don't write low-priority-msgs
         ODS(DBG_SYS,DBG_WARNING,"Q nearly full!");
         return ERR_MSGQUEUE_NO_WRITE;
      }
   }  //  gbMsgQueueCurrent > FULL_MSGQUEUE

   // write message, must not be interrupted
    INT_GLOB_DISABLE;; // lock, in case an interrupts wants to post right now
   grgMessageQueue[gbMsgQueueWrite] = msg;
   gbMsgQueueWrite++;
   if ( gbMsgQueueWrite == LENGTH_MSGQUEUE )
   {
      gbMsgQueueWrite=0;
   }
   gbMsgQueueCurrent++;  // increase number of entries
   INT_GLOB_ENABLE;;

   // diagnostic: save max number of msg in queue
   if ( gbMsgQueueCurrent > gbMsgQueueMax)
        gbMsgQueueMax = gbMsgQueueCurrent;

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       MsgQGetMsg
 *  DESCRIPTION:    Reads a message from the message queue.
 *                  Internal function called by MsgQPumpMsg()
 *  PARAMETER:      pointer to message
 *  RETURN:         - ERR_OK                if message found
 *                  - ERR_MSGQUEUE_NO_MSGS  if queue is empty
 *                  - ERR_MSGQUEUE_NO_ID    if no valid ID found
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MsgQGetMsg(MESSAGE *pmsg)
{
   if ( gbMsgQueueCurrent == 0 )
   {
      // if Queue is empty
      return ERR_MSGQUEUE_NO_MSGS;
   }

   MSG_ID(*pmsg) = MSG_NULL_MSG;

    INT_GLOB_DISABLE;;
   *pmsg = grgMessageQueue[gbMsgQueueRead];
   gbMsgQueueCurrent--;  // decrease current queue-size
   INT_GLOB_ENABLE;;

   gbMsgQueueRead++;
   if ( gbMsgQueueRead == LENGTH_MSGQUEUE )
   {
      gbMsgQueueRead=0;
   }

   if ( MSG_ID(*pmsg) == MSG_NULL_MSG )
   {
      ODS(DBG_SYS,DBG_WARNING,"No valid MsgID found in Q!");
      return ERR_MSGQUEUE_NO_ID; // tritt nur nach MsgQPumpMsg auf
   }
   else
   {
      return ERR_OK;
   }
}



/***********************************************************************
 *  FUNCTION:       MsgQDeleteItem
 *  DESCRIPTION:    Deletes a message from the grgMessageQueue.
 *                  Internal function, used by MsgQRemoveMsg(MESSAGE_ID bID)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MsgQDeleteItem(UINT8 bItem)
{
   if ( gbMsgQueueRead > bItem )
   {
      memmove(&grgMessageQueue[1], &grgMessageQueue[0], bItem * sizeof(MESSAGE));

      grgMessageQueue[0] = grgMessageQueue[LENGTH_MSGQUEUE - 1];

      memmove(&grgMessageQueue[gbMsgQueueRead + 1],
                    &grgMessageQueue[gbMsgQueueRead],
                    (LENGTH_MSGQUEUE-gbMsgQueueRead-1) * sizeof(MESSAGE));
   }
   else
   {
      memmove(&grgMessageQueue[gbMsgQueueRead+1],
                    &grgMessageQueue[gbMsgQueueRead],
                    (bItem - gbMsgQueueRead) * sizeof(MESSAGE));
   }

   gbMsgQueueRead++;
   if ( gbMsgQueueRead == LENGTH_MSGQUEUE )
   {
      gbMsgQueueRead=0;
   }

    INT_GLOB_DISABLE;;  // must not be interrupted
   gbMsgQueueCurrent--;
   INT_GLOB_ENABLE;;
}


/***********************************************************************************
 * Removes all messages with given message id from message queue
 *
 * returns:
 *         ERR_MSGQUEUE_NO_MSGS if queue is empty
 *        ERR_OK if at least one entry removed
 *        ERR_MSGQUEUE_NO_ID if ID not found
 ***********************************************************************************/
/***********************************************************************
 *  FUNCTION:       MsgQRemoveMsg
 *  DESCRIPTION:    Removes all messages with given message id
 *                  from message queue
 *  PARAMETER:      MESSAGE_ID bID
 *  RETURN:         ERR_MSGQUEUE_NO_MSGS    if queue is empty
 *                  ERR_OK                  if at least one entry removed
 *                  ERR_MSGQUEUE_NO_ID      if ID not found
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MsgQRemoveMsg(MESSAGE_ID bID)
{
   UINT8   bInternalSysQueueRead;   // read index
   ERRCODE Result = ERR_MSGQUEUE_NO_ID;

   if ( gbMsgQueueCurrent == 0 )
   {
      // queue is empty
      Result = ERR_MSGQUEUE_NO_MSGS;
   }
   else
   {
      // remove all messages with specified bID
      bInternalSysQueueRead = gbMsgQueueRead;
      do
      {
         if ( MSG_ID(grgMessageQueue[bInternalSysQueueRead]) == bID )
         {
            MsgQDeleteItem(bInternalSysQueueRead);
            Result = ERR_OK;
         }

         bInternalSysQueueRead++;
         if ( bInternalSysQueueRead == LENGTH_MSGQUEUE )
         {
            bInternalSysQueueRead=0;
         }
      }
      while (bInternalSysQueueRead != gbMsgQueueWrite) ;
   }

   return Result;
}


/***********************************************************************
 * FUNCTION:       MsgQPumpMsg
 * DESCRIPTION:    - main dispatcher for messages,
 *                 - calls modul specific msg entry functions
 *                 - if no msg: handle idle stuff (t.b.d.)
 *                 - is beeing called in main loop
 * PARAMETER:      MESSAGE_ID bID to distinguish between
 *                      a) normal dispatch mode and
 *                      b) search for special message ids
 * RETURN:         error code:
 *                 - ERR_OK                if msg found
 *                 - ERR_MSGQUEUE_NO_MSGS  if no msg found or Q empty
 * COMMENT:        Register here to support your own message entry function!
 *                 - ToBeDoneLater: dynamically register MsgEntryFunction...
 *********************************************************************** */
ERRCODE MsgQPumpMsg(MESSAGE_ID bID)
{
   UINT8    bFlag,bInternalSysQueueRead;
   MESSAGE  msg;
   ERRCODE  err = ERR_OK;

   // evtl. ToDo: Watchdog triggern

   // Default main loop call:
   if ( bID == MSG_NULL_MSG )  // handle next msg in queue
   {
       // --------------------------------------------------
       // MSG_NULL_MSG means 'nothing SPECIAL to read' from Q
       if (MsgQGetMsg(&msg) == ERR_OK)  // any message in Q?
       {
          err = ERR_MSG_NOT_PROCESSED;  // per default

          #if(DBGMSGQ==1)
          if (msg.BBBI.id != MSG_SCREEN_RFRSH)
            ODS1(DBG_SYS,DBG_INFO,"'%s' received!", MsgQGiveMsgAsString(msg.BBBI.id));
          #endif

          // -------------------------------------------------
          // REGISTER YOUR FUNCTION HERE:
          //    use function prototype 'MSGENTRYFCT' from msgqeue.h
          //
          if (err == ERR_MSG_NOT_PROCESSED)
            err = NV_TripCom_ADev_MsgEntry(msg);         /* trip counter device */

          if (err == ERR_MSG_NOT_PROCESSED)
            err = MainDev_MsgEntry(msg);      /* main device */

          if (err == ERR_MSG_NOT_PROCESSED)
            err = MonDev_MsgEntry(msg);   /* monitor device device */

          if (err == ERR_MSG_NOT_PROCESSED)
            err = LCDev_MsgEntry(msg);    /* lap counter device - special MOTOBAU version */

          if (err == ERR_MSG_NOT_PROCESSED)
            err = IntroDev_MsgEntry(msg);     /* device 'intro screen' */

          if (err == ERR_MSG_NOT_PROCESSED)     /* settings device */
            err = SetDev_MsgEntry(msg);

          #if(TESTSCREEN==1)
          if (err == ERR_MSG_NOT_PROCESSED)
            err = TestScreen_MsgEntry(msg);      /* device 'test screen' */
          #endif //(TESTSCREEN==1)

          if (err == ERR_MSG_NOT_PROCESSED)
            err = HWTDev_MsgEntry(msg);    /* HW test device */

          // -------------------------------------------------

          /* nobody processed this message ? */
          #if(DBGMSGQ==1)
          if (err == ERR_MSG_NOT_PROCESSED)
              ODS1(DBG_SYS,DBG_WARNING,"'%s' not processed!", MsgQGiveMsgAsString(msg.BBBI.id));
          #endif

          /* check msg Q entries */
          #if(DBGMSGQ==1)
          ODS1(DBG_SYS,DBG_INFO,"MsgQ: still %u",gbMsgQueueCurrent);
          #endif

          return err;
       }
       else
       {
           //AppIdle();
           // ToDo: Application idle, nothing to do
          return ERR_MSGQUEUE_NO_MSGS;
       }
   }
   else
   {
      // --------------------------------------------------
      // Search for special message id
      // (handle next msg whith specific ID)
      if ( gbMsgQueueCurrent == 0 )
      {
         //if queue is empty then idle mode
         return ERR_MSGQUEUE_NO_MSGS;
      }

      bFlag=0;
      bInternalSysQueueRead = gbMsgQueueRead;

      do
      {
         //scanning the queue for msg with bID
         if ( MSG_ID(grgMessageQueue[bInternalSysQueueRead]) == bID )
         {

            // ToDo: Message an Applikatoin reichen
            //(void)AppMsgEntry(grgMessageQueue[bInternalSysQueueRead]);
            MsgQDeleteItem(bInternalSysQueueRead);
            bFlag=1;
            break;
         }

         bInternalSysQueueRead++;
         if ( bInternalSysQueueRead == LENGTH_MSGQUEUE )
         {
           bInternalSysQueueRead=0;
         }
      }
      while ( bInternalSysQueueRead != gbMsgQueueWrite );

      if (bFlag==1)
      {
         return ERR_OK;
      }
      else
      {
         return ERR_MSGQUEUE_NO_MSGS;
      }
   }
}


/***********************************************************************************
 * Returns the number of max used entries in Countdown-MessageQueue. The value
 * is equal to difference between read- and writepointer in the grgMessageQueue.
 ***********************************************************************************/
UINT8 MsgQGetMaxEntries (void)
{
   return gbMsgQueueMax;
}


/***********************************************************************
 *        FUNCTION:       TestMessageEntry
 *        DESCRIPTION:    Example of modul specific message entry function
 *        PARAMETER:      MESSAGE msg to be handled
 *        RETURN:         error code:
 *                  - ERR_OK                if msg handled
 *        COMMENT:        just as an example and as dummy entry in MsgQPumpMsg()
 *********************************************************************** */
ERRCODE TestMessageEntry(MESSAGE msg)
{
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       MsgQGiveMsgAsString
 *  DESCRIPTION:    returns the message id in a plain text string
 *  PARAMETER:      MESSAGE_ID bID
 *  RETURN:         STRING  pointer to the identifier string
 *  COMMENT:        for degug purpose
 *********************************************************************** */
STRING MsgQGiveMsgAsString(MESSAGE_ID bID)
{
    STRING szBuffer;                    // return value

    switch(bID)
    {
        case MSG_NULL_MSG:      szBuffer = "MSG_NULL_MSG";          break;
        case MSG_TIMER:         szBuffer = "MSG_TIMER";             break;
        case MSG_KEY_OK:        szBuffer = "MSG_KEY_OK";            break;
        case MSG_KEY_UP:        szBuffer = "MSG_KEY_UP";            break;
        case MSG_KEY_DOWN:      szBuffer = "MSG_KEY_DOWN";          break;
        case MSG_KEYS_PRESSED:  szBuffer = "MSG_KEYS_PRESSED";      break;
        case MSG_DPL_FLASH_ON:  szBuffer = "MSG_DPL_FLASH_ON";      break;
        case MSG_DPL_FLASH_OFF: szBuffer = "MSG_DPL_FLASH_OFF";     break;
        case MSG_LED_SET:       szBuffer = "MSG_LED_SET";           break;
        case MSG_LED_ON:        szBuffer = "MSG_LED_ON";            break;
        case MSG_LED_OFF:       szBuffer = "MSG_LED_OFF";           break;
        case MSG_BEEP_SET:      szBuffer = "MSG_BEEP_SET";          break;
        case MSG_BEEP_ON:       szBuffer = "MSG_DPL_FLASH_OFF";     break;
        case MSG_BEEP_OFF:      szBuffer = "MSG_DPL_FLASH_OFF";     break;
        case MSG_GET_FOCUS:     szBuffer = "MSG_GET_FOCUS";         break;
        case MSG_SET_FOCUS:     szBuffer = "MSG_SET_FOCUS";         break;
        case MSG_SCREEN_RFRSH:  szBuffer = "MSG_SCREEN_RFRSH";      break;
        case MSG_COMPASS_RFRSH: szBuffer = "MSG_COMPASS_RFRSH";     break;
        case MSG_SECOND_GONE:   szBuffer = "MSG_SECOND_GONE";       break;
        case MSG_VEHSTATE_SHOW: szBuffer = "MSG_VEHSTATE_SHOW";     break;
        case MSG_VEHSTATE_HIDE: szBuffer = "MSG_VEHSTATE_HIDE";     break;
        default:                szBuffer = "UNKNOWN MESSAGE!";      break;
    }
    return szBuffer;
}
