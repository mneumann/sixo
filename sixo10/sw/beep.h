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
 *  Module:         Beep
 *  Purpose:        Beep services (timing, auto update)
 *  Comments:       -
 *
 ************************************************************************ */
#ifndef _BEEP_H
#define _BEEP_H



/* Beep timing */
typedef struct
{
    UINT8   wOnTicks;    /* Beep ON duration in 1/10 sec */
    UINT8   wOffTicks;   /* Beep OFF duration in 1/10 sec */
} BEEPTIMINGTYPE;


/* beeper defines */
#define BEEP_OFF FALSE        /* beeper is off */
#define BEEP_ON  TRUE         /* beeper is on */


/* macros for easier Beep messages and timings in ms / sec / ticks */
#define BEEP_MSG_MS(msg, status, ontime, offtime)    MSG_BUILD_UINT8(msg, MSG_BEEP_SET, status, MS2TICKS(ontime), MS2TICKS(offtime))
#define BEEP_MSG_SEC(msg, status, ontime, offtime)   MSG_BUILD_UINT8(msg, MSG_BEEP_SET, status, SEC2TICKS(ontime), SEC2TICKS(offtime))
#define BEEP_MSG_TICKS(msg, status, ontime, offtime) MSG_BUILD_UINT8(msg, MSG_BEEP_SET, status, ontime, offtime)


/* prototypes */
ERRCODE BeepInit(void);
ERRCODE BeepMsgEntry(MESSAGE msg);
ERRCODE BeepSetNewState(MESSAGE msg);

void BeepOk(void);
void BeepEsc(void);
void BeepClick(void);


/* test functions */
void TestBeepSendMessage(void);

#endif /* _Beep_H */



