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
 *  Created:        01-03-2002 by Arnold Neugebauer
 *  Project:        SIxO
 *  Module:         Digital Output Driver
 *  Purpose:        Low level support of all digital outputs 
 *                  (but not LEDs!)
 *  Comments:       -
 *
 ************************************************************************ */
 
 #include "standard.h"
 #include "sfr62.h"
 #include "debug.h"
 #include "digoutdr.h"


/***********************************************************************
 *  FUNCTION:       DigOut_Init
 *  DESCRIPTION:    Initializes all General Digital Output Pins
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        Prevents floating input pins p2_6 / p2_7 of
 *                  destroying transistors, should be called as early as
 *                  possible
 *********************************************************************** */
ERRCODE DigOutInit(void)
{

#ifdef DEBUG    
    /* TEST: Port 10 for ossi checks */
    p10 = 0;        /* clear output */
    pd10 = 0xfc;    /* But: p10_0 & _1 are not connected to SIxO pads! */
#endif

    /* set general purpose outputs GPO0/1 to prevent damage */
    p2_6 = 0;
    p2_7 = 0;
    pd2_6 = 1;
    pd2_7 = 1;
    ODS(DBG_DRV,DBG_INFO,"DigOutInit() done!");
    return ERR_OK;
}
 
 