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
 *  Created:        01-03-2002 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         Digital Output Driver
 *  Purpose:        Low level support of all digital outputs
 *                  (but not LEDs!)
 *  Comments:       -
 *
 ************************************************************************ */

ERRCODE DigOutInit(void);


#ifdef TOGGLE_PADS
/* makros for port10 ossi checks */
/* port pin will change state each time, when a isr is called */
    #define TOGGLE_PAD27        (p10_2 = p10_2 ? 0 : 1)
    #define TOGGLE_PAD28        (p10_3 = p10_3 ? 0 : 1)
    #define TOGGLE_PAD29        (p10_4 = p10_4 ? 0 : 1)
    #define TOGGLE_PAD30        (p10_5 = p10_5 ? 0 : 1)
    #define TOGGLE_PAD31        (p10_6 = p10_6 ? 0 : 1)
    #define TOGGLE_PAD32        (p10_7 = p10_7 ? 0 : 1)
#else
    #define TOGGLE_PAD27
    #define TOGGLE_PAD28
    #define TOGGLE_PAD29
    #define TOGGLE_PAD30
    #define TOGGLE_PAD31
    #define TOGGLE_PAD32
#endif


