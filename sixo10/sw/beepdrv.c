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

#include "sfr62.h"
#include "standard.h"
#include "beepdrv.h"
#include "timer.h"
#include "debug.h"



/***********************************************************************
 *  FUNCTION:       BeepDrvInit()
 *  DESCRIPTION:    initializes Beeper hardware
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE BeepDrvInit(void)
{
    fBeep   = 0;    /* switch off output */
    fBeep_D = 1;    /* enabled output */

    ODS(DBG_DRV,DBG_INFO,"BeepDrvInit() done!");
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       BeepDrvSetBeeper()
 *  DESCRIPTION:    switches beeper on/off
 *  PARAMETER:      BOOL    fActive     TRUE = on
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE BeepDrvSetBeeper(BOOL fActive)
{
    fBeep = (fActive == TRUE) ? 1 : 0;

    if (fActive == TRUE)
         fBeep   = 1;    /* switch ON output */
    else fBeep   = 0;    /* switch OFF output */

    return ERR_OK;
}



 