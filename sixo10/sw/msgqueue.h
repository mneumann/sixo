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
#ifndef _MSGQUEUE_H
#define _MSGQUEUE_H

#include "stdmsgs.h"


/* define union MESSAGE */
typedef union _TAG_MESSAGE
{
    struct
    {
        INT8 byte1;                /* @ field Transfer of signed byte1. */
        INT8 byte2;                /* @ field Transfer of signed byte2. */
        INT8 byte3;                /* @ field Transfer of signed byte3. */
        MESSAGE_ID id;             /* @ field  This is the identification of the message. */
    } BBBI;
    struct
    {
        UINT8 uChar1;        /* @ field Transfer of unsigned byte1. */
        UINT8 uChar2;        /* @ field Transfer of unsigned byte2. */
        UINT8 uChar3;        /* @ field Transfer of unsigned byte3. */
        MESSAGE_ID id;       /* @ field  This is the identification of the message. */
    } UUUI;
} MESSAGE;


// standard function type for modul specific 
// message entry functions 
typedef ERRCODE (MSGENTRYFCT) (MESSAGE);

// define maximum number of entries in message queue
#define LENGTH_MSGQUEUE                50

// define when only messsages with high priority will be posted
#define FULL_MSGQUEUE                (LENGTH_MSGQUEUE*4/5)      /* treshold: 80 % */

// defines for message priorities
// message will be posted if any space in queue left
#define MSGQ_PRIO_HIGH          1
// message has low priority, posted only if less than FULL_MSGQUEUE entries in queue
#define MSGQ_PRIO_LOW           0


// uC Implementation: use 4 bytes, abuse pointer highbyte for ID (writing pointer
// destroys ID !!! So write pointer first then ID)
// Unfortunately, the uC compiler doesn't like anonymous structures.
// This makes the access macros difficult to read.


#define MSG_BUILD_INT8(msg,idty,int81,int82,int83)     msg.BBBI.id=idty; msg.BBBI.byte1=int81;   msg.BBBI.byte2=int82;   msg.BBBI.byte3=int83

#define MSG_BUILD_UINT8(msg,idty,uint81,uint82,uint83) msg.UUUI.id=idty; msg.UUUI.uChar1=uint81; msg.UUUI.uChar2=uint82; msg.UUUI.uChar3=uint83

#define MSG_BUILD_ID(msg,idty) msg.UUUI.id=idty

#define MSG_ID(msg)                ((msg).UUUI.id)

#define MSG_BYTE1(msg)        ((msg).BBBI.byte1)
#define MSG_BYTE2(msg)        ((msg).BBBI.byte2)
#define MSG_BYTE3(msg)        ((msg).BBBI.byte3)

#define MSG_CHAR1(msg)        ((msg).UUUI.uChar1)
#define MSG_CHAR2(msg)        ((msg).UUUI.uChar2)
#define MSG_CHAR3(msg)        ((msg).UUUI.uChar3)

#define MSG_INAREA(msg,begin,end)    ((MSG_ID(msg)>=begin)&&(MSG_ID(msg)<=end))


/* declaration of the functions for the MessageQueue */
ERRCODE MsgQPostMsg(MESSAGE msg,INT8 priority);
ERRCODE MsgQPumpMsg(MESSAGE_ID bID);
ERRCODE MsgQRemoveMsg(MESSAGE_ID bID);
ERRCODE MsgQGetMsg(MESSAGE *pmsg);
ERRCODE MsgQInit(void);
void    MsgQDeleteItem(UINT8 bItem);
UINT8   MsgQGetMaxEntries(void);                        
STRING MsgQGiveMsgAsString(MESSAGE_ID bID);

/* end of header file definitions */
#endif     /* _MSGQUEUE_H */
