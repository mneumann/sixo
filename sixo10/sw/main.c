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
 *  Created:        22-10-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         main
 *  Purpose:        main loop
 *  Comments:       -
 *
 ************************************************************************ */

/* include */
#include "standard.h"
#include "stdmsgs.h"
#include "debug.h"
#include "diagnose.h"
#include "msgqueue.h"
#include "timer.h"
#include "sfr62.h"
#include "led.h"
#include "leddrv.h"
#include "digindrv.h"
#include "digoutdr.h"
#include "display.h"
#include "device.h"
#include "eeprdrv.h"
#include "measdrv.h"
#include "vehicsim.h"
#include "measure.h"
#include "sysparam.h"
#include "beep.h"



/* main.c prototype area */
int main(void);



/* external symbols */
extern SYSFLAGS_TYPE    gSystemFlags;   /* system parameters */


/* for diagnostic service */
extern  UINT16  wUSPStart;              // user stack pointer start value
extern  UINT16  wUSPMax;                // user stack pointer max value
extern  UINT16  wISPStart;              // interrupt stack pointer start value
extern  UINT16  wISPMax;                // interrupt stack pointer max value

extern  UINT16  wMilliSecCounter;       // valid values: 0h .. ffffh
extern  UINT16  wSecCounter;            // valid values: 0h .. ffffh

extern  DBGFILT_TYPE    gDebugFilter;   /* default off, use DebugSetFilterDetails() to change */
extern  DBGDETDIR_TYPE  gDebugDetails;  /* default uart, use DebugSetFilterDetails() to change */




/***********************************************************************
 *  FUNCTION:       THE main function
 *  DESCRIPTION:    1) system init stuff
 *                  2) handles the message pump
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
int main()
{
    ERRCODE Error;                  /* RValue */
    MESSAGE Msg;                    /* for start message */

    /* diagnostic init */
    asm(" stc sp,$$", wUSPStart);   /* get user stack pointer */
    asm(" stc isp,$$", wISPStart);  /* get interrupt stack pointer */
    wUSPMax = wUSPStart;            /* save USP start value */
    wISPMax = wISPStart;            /* save ISP start value */

    /* prepare Debug stuff*/
    /*    set debug-filter, -target and -details */
    {
        DBGFILT_TYPE    Filter  = DEF_DBGFILT;
        DBGDETDIR_TYPE  Details = DEF_DBGDETDIR;
        Error = DebugInit(Filter, Details);
    }

    /* system init stuff */
    Error = TimerInit();            /* start our both timers (ta0 1kHz, ta1 50 Hz) */
    Error = iicInit();              /* prepare iic ports and eeprom type (needs TimerInit()!)*/
    Error = ParCheckFirstInit();    /* check, wether eeprom/nvram update necessary */
    Error = ParInitSystemPar();     /* initialize all system parameters from eeprom/nvram */
    Error = DebugSetFilterDetails(gDebugFilter, gDebugDetails); /* valid after ParInitSystemPar() */
    Error = DisplInit();            /* prepare lcd output & clear screen */
    Error = DigOutInit();           /* prepare general purpose output ports */
    Error = LEDInit();              /* prepare led output ports & service */
    Error = BeepInit();             /* prepare beeper output ports & service */
    Error = DigInDrv_Init();        /* prepare keyboard ports and other input ports */
    Error = MsgQInit();             /* reset message queue */
    Error = MeasDrvInit();          /* measurement init stuff (ta2, ta3, ta4, tb2, int1, int0) */

    /* Do you want to redirect DebugOuts to display?
       Then be sure to disable all other display output! (disable all devices!)
       This will later be done by a special 'DebugOut-ScreenDevice' */
    //Error = DebugSetFilterDetails(DBG_ALL, DBG_DSPL | DBG_UART);


    /* Register cyclic called fast functions */
    TimerRegisterEntryFunction( DigInDrv_CheckKeyAction );      /* check keys with 50 Hz */
    TimerRegisterEntryFunction( MeasCyclicSaveValues );         /* cyclic check/save nvram values */
    TimerRegisterEntryFunction( ParCyclicSaveValues );          /* cyclic check/save eeprom values */
    TimerRegisterEntryFunction( DevCyclicRefresh );             /* cyclic generation of MSG_SCREEN_REFRESH */
    #ifdef HW_OILGEN
    TimerRegisterEntryFunction( DigInDrv_CheckEnginePorts );    /* cyclic check of oil pressure & engine generator */
    #endif // of HW_OILGEN

    /* device screen inits */
    Error = IntroScreenInit();                                  /* intro screen device */
    Error = MainDeviceInit();                                   /* main device (speed&rpm) */
    Error = TripCntDevInit();                                   /* trip counter device */
    Error = SetDeviceInit();                                    /* settings device */

    /* show the first device screen */
    MSG_BUILD_SETFOCUS(Msg, DEVID_UNKNOWN, gSystemFlags.flags.ActDevNr); /* give focus to last device used */
    MsgQPostMsg(Msg, MSGQ_PRIO_LOW);

    /* THE main loop */
    while (1)
    {
        Error = MsgQPumpMsg(MSG_NULL_MSG);      /* MessagePump: look for messages & execute them */
        //SimVehicSimulation();                 /* comment this out to disable simulation */
    }
    return 0;
}




