/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        22-10-2001 by Ralf Krizsan
 *  Project:        SIxO20
 *  Module:         main
 *  Purpose:        main loop
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
 *  This information is automatically added while 'commit' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 2.1  2007/03/26 23:25:13  tuberkel
 * changed MOTOBAU version handling
 * - eBikeType -> #define
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 * Revision 1.21  2006/02/18 14:08:16  Ralf
 * - adaption to BIKER_MOTOBAU
 * - logo delay set
 * - compiler handling of vehicsim / hardcopy improved
 *
 * Revision 1.20  2005/10/03 16:32:19  Ralf
 * - activate initial display backlight switched on
 * - added CVS '$log' keyword to suuport history of changes
 *
 *
 *  --------------------------------------------------------------------
 */

/* include */
#include <stdio.h>
#include "standard.h"
#include "stdmsgs.h"
#include "debug.h"
#include "diagnose.h"
#include "msgqueue.h"
#include "timer.h"
#include "sfr62p.h"
#include "led.h"
#include "leddrv.h"
#include "digindrv.h"
#include "digoutdr.h"
#include "display.h"
#include "displdrv.h"
#include "device.h"
#include "eeprdrv.h"
#include "measdrv.h"
#include "vehicsim.h"
#include "measure.h"
#include "sysparam.h"
#include "beep.h"
#include "timedate.h"
#include "anaindrv.h"
#include "anain.h"
#include "surveill.h"



/* main.c prototype area */
int  main(void);
void VehicleSimulation(void);
void Hardcopy (void);

/* local symbols */
DEVICE_ID eStartDevice;                 // handles startdevice


/* external symbols */
extern SYSFLAGS_TYPE    gSystemFlags;   // system parameters
extern UINT16           wWheelSize;     // to control vehicle simulation


/* external symbols for diagnostic service */
extern  UINT16  wUSPStart;              // user stack pointer start value
extern  UINT16  wUSPMax;                // user stack pointer max value
extern  UINT16  wISPStart;              // interrupt stack pointer start value
extern  UINT16  wISPMax;                // interrupt stack pointer max value

extern  UINT16  wMilliSecCounter;       // valid values: 0h .. ffffh
extern  UINT16  wSecCounter;            // valid values: 0h .. ffffh

extern  DBGFILT_TYPE    gDebugFilter;   /* default off, use DebugSetFilterDetails() to change */
extern  DBGDETDIR_TYPE  gDebugDetails;  /* default uart, use DebugSetFilterDetails() to change */

extern BIKE_TYPE        gBikeType;          /* bike type selcetion */

#define LOGODELAY   2000    // in ms



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
    BOOL    fReBootInProgress;      /* for HW selftest including re-boot */

    /* diagnostic init ------------------------------ */
    asm(" stc sp,$$", wUSPStart);   /* get user stack pointer */
    asm(" stc isp,$$", wISPStart);  /* get interrupt stack pointer */
    wUSPMax = wUSPStart;            /* save USP start value */
    wISPMax = wISPStart;            /* save ISP start value */

    /* prepare Debug stuff -------------------------- */
    /*  set debug-filter, -target and -details as default
        until real settings read from EEPROM */
    {
        DBGFILT_TYPE    Filter  = DEF_DBGFILT;
        DBGDETDIR_TYPE  Details = DEF_DBGDETDIR;
        Error = DebugInit(Filter, Details);
    }

    /* system init stuff ----------------------------- */
    Error = TimerInit();            /* start our both timers (ta0 1kHz, ta1 50 Hz) */
    Error = iicInit();              /* prepare iic ports and eeprom type (needs TimerInit()!)*/
    Error = ParCheckFirstInit();    /* check, wether eeprom/nvram update necessary */
    Error = ParInitSystemPar();     /* initialize all system parameters from eeprom/nvram */
    Error = TimeDateInit();         /* initialize system time & date stuff */
    Error = DisplInit(FALSE);       /* prepare lcd output & clear screen */
    Error = DigOutInit();           /* prepare general purpose output ports */
    Error = LEDInit();              /* prepare led output ports & service */
    Error = BeepInit();             /* prepare beeper output ports & service */
    Error = DigInDrv_Init();        /* prepare keyboard ports and other input ports */
    Error = MsgQInit();             /* reset message queue */
    Error = MeasDrvInit();          /* measurement init stuff (ta2, ta3, ta4, tb2, int1, int0) */
    Error = AnaInInit();            /* A/D converter for all measurements  */
    Error = SurvInit();             /* vehicle surveillance */

    /* check: hw self diagnostic test ---------------- */
    /* (all keys pressed together at startup) */
    if ( DigInDrv_GetKeyStates() == ( KEYFL_OK | KEYFL_UP | KEYFL_DOWN ) )
    {
        // HW SELF TEST MODE ============================================

        TimerRegisterEntryFunction( AnaInDrvTriggerADConverter );   /* generation of AD samples in single sweep mode */
        Error = HWTestDeviceInit();                                 /* HW test device */
        eStartDevice = DEVID_HWTEST;                                /* start HW TEST MODE */
        MSG_BUILD_SETFOCUS(Msg, DEVID_UNKNOWN, eStartDevice);       /* give focus to that device */
        MsgQPostMsg(Msg, MSGQ_PRIO_LOW);                            /* post message */
    }
    else
    {
        // NORMAL USER MODE =============================================

        /* device screen inits --------------------------- */
        Error = IntroScreenInit();                                  /* intro screen device */
        Error = MainDeviceInit();                                   /* main device (speed&rpm) */
        Error = TripCntDevInit();                                   /* trip counter device */
        Error = MonitorDeviceInit();                                /* monitor device */
        #ifdef BIKE_MOTOBAU                                         /* special MOTOBAU behaviour */
        Error = LapCntDeviceInit();                                 /* LapCounter device */
        #endif // BIKE_MOTOBAU
        Error = SetDeviceInit();                                    /* settings device */

        /* Display & LED 'HW pseudo test' ---------------- */
        LCDDrvSetBacklightLevel(TRUE, 63);  // switch on Backlight
        IntroScreenShow(TRUE);              // show 'splash screen'
        p9 = LEDS_ALL;                      // all LEDs on
        Delay_ms(LOGODELAY);                // wait
        IntroScreenShow(FALSE);             // clear 'splash screen'
        p9 &= ~LEDS_ALL;                    // all LEDs off

        /* Register cyclicely called (50Hz) fast functions --------- */
        TimerRegisterEntryFunction( DigInDrv_CheckKeyAction );      /* check keys */
        TimerRegisterEntryFunction( DigInDrv_CheckAllPorts );       /* update of standard LEDs for Turn/HighBeam/Neutral */
        TimerRegisterEntryFunction( ParCyclicSaveValues );          /* check/save eeprom values */
        TimerRegisterEntryFunction( DevCyclicRefresh );             /* generation of MSG_SCREEN_REFRESH */
        TimerRegisterEntryFunction( TimeDateUpdate );               /* RTC check */
        TimerRegisterEntryFunction( AnaInDrvTriggerADConverter );   /* generation of AD samples in single sweep mode */
        TimerRegisterEntryFunction( SurvCheckAllValues );           /* check of all digital/analoge values for warnings/errors */

        // set start screen ------------------------------ */
        if (gSystemFlags.flags.ActDevNr == DEVID_HWTEST)            /* prevent from starting with HW-Test all the time */
            gSystemFlags.flags.ActDevNr = DEVID_MAIN;
        eStartDevice = gSystemFlags.flags.ActDevNr;                 /* start NORMAL USER MODE */
        MSG_BUILD_SETFOCUS(Msg, DEVID_UNKNOWN, eStartDevice);       /* give focus to that device */
        MsgQPostMsg(Msg, MSGQ_PRIO_LOW);                            /* post message */
    }


    /* THE main loop --------------------------------- */
    while (1)
    {
        Error = MsgQPumpMsg(MSG_NULL_MSG);      /* MessagePump: look for messages & execute them */
        VehicleSimulation();                    /* if defined: RPM+WHEEL simulation support */
        Hardcopy();                             /* if defined: Grafic Hardcopy support */
    }
    return 0;
}




/***********************************************************************
 * FUNCTION:    VehicleSimulation
 * DESCRIPTION: Vehicle Simulation active if WheelSize exactly 2000 mm,
 *              simulated WHEEL + RPM input via ISR calls
 * PARAMETER:   -
 * RETURN:      -
 * COMMENT:     To simulate real SIXO action while nobody moves...
 *********************************************************************** */
void VehicleSimulation(void)
{
#ifdef VEHICSIM
    static BOOL fSimEnabled = FALSE;

    // delayed start to prevent ISR overflows
    if ( wSecCounter < SIM_STARTDELAY )
        return;

    // enable/disable simulation
    if ( wWheelSize == SIM_WHEELSIZE  )
    {
        SimVehicSimulation(SIM_SEQUENCE);
        if (fSimEnabled == FALSE)
        {
            fSimEnabled = TRUE;
            ODS(DBG_SYS,DBG_INFO,"==> Vehicle Simulation enabled!");
        }
    }
    else
    {
        if (fSimEnabled == TRUE)
        {
            fSimEnabled = FALSE;
            ODS(DBG_SYS,DBG_INFO,"==> Vehicle Simulation disabled!");
        }
    }
#endif
}





/***********************************************************************
 * FUNCTION:    Hardcopy
 * DESCRIPTION: Send a screen hardcopy in BMP format to printf-uart (debug)
 *              if HIGHBEAM state changes from LOW->HIGH
 * PARAMETER:   -
 * RETURN:      -
 * COMMENT:     FOR DOCUMENATATION PURPOSE ONLY!
 *********************************************************************** */
void Hardcopy (void)
{
#ifdef HARDCOPY

#ifndef DEBUG
    #error "Error: HARDCOPY supported only via uart, please activate DEBUG too!"
#endif // DEBUG

    #include "digindrv.h"
    #include "displdrv.h"
    extern void LCDDrvHardcopy(void);
    static BOOL fState = 0;
    if (  (DigIn_HBeam == 1 )
        &&(fState      == 0 ) )
    {
        LCDDrvHardcopy();
        fState = 1;
    }
    if (  (DigIn_HBeam == 0 )
        &&(fState      == 1 ) )
    {
        fState = 0;
    }
#endif // HARDCOPY
}




