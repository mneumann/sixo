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
 *  Created:        01-10-2002 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         beeper
 *  Purpose:        low level beep functions
 *  Comments:       -
 *
 ************************************************************************ */
#ifndef _BEEPDRV_H
#define _BEEPDRV_H

/* general purpose defines */


/* redefine of port register of sfr62.h for readability */
#define fBeep_D         pd0_7   /* beeper out pin direction register */
#define fBeep           p0_7    /* beeper out pin */

/* function protoypes */
ERRCODE BeepDrvInit(void);
ERRCODE BeepDrvSetBeeper(BOOL fActive);

#endif /* _BEEPDRV_H */

