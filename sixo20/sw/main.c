/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        22-10-2001 by Ralf Schwarzer
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
 *      Arnold Neugebauer                   Ralf Schwarzer
 *      Stöckener Str. 115                  Hudeplan 37 A
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
 *  This information is automatically added while 'commit' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 3.13  2012/05/27 16:01:39  tuberkel
 * All Eeprom/Nvram Variables renamed
 *
 * Revision 3.12  2012/02/27 23:15:19  tuberkel
 * - SysPar- API changed
 *
 * Revision 3.11  2012/02/14 21:08:03  tuberkel
 * - #define COMPASS ==> COMPDRV
 * - Compass SystemParam moved from devFlags2 -> 3
 * - Settings 'Compass' ==> 'Extensions'
 * - all Compass-Display modules enabled by default
 *
 * Revision 3.10  2012/02/10 23:45:22  tuberkel
 * - Survelannce HeatGrip <Info> - if active
 * - Surveillance-API reviewed
 *
 * Revision 3.9  2012/02/08 23:05:47  tuberkel
 * - GPI PWM calculations improved
 * - renamed DigIn GPI Functions
 * - renamed DigIn Key Functions
 *
 * Revision 3.8  2012/02/06 20:54:14  tuberkel
 * Just renamed all 'Devices' function prefixes for better readability
 *
 * Revision 3.7  2012/02/05 11:17:08  tuberkel
 * DigOuts completely reviewed:
 * - central PWM-Out handled via DigOutDriver for ALL DigOuts!
 * - simplified LED/Beeper/GPO HL-Driver
 * - unique API & Parameter Handling for LED/Beeper/GPO
 *
 * Revision 3.6  2012/02/04 22:25:49  tuberkel
 * LEDs renamed
 *
 * Revision 3.5  2012/02/04 20:38:05  tuberkel
 * Moved all BeeperDriver / LEDDriver stuff ==> 'digoutdrv'
 *
 * Revision 3.4  2012/02/04 08:35:12  tuberkel
 * New: PWM-Testcode (TEST_DIGOUT_PWM==1)
 *
 * Revision 3.3  2012/02/02 21:55:30  tuberkel
 * New: GPOs - PWM controlled like LEDs
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
 * Revision 3.0  2010/11/07 09:41:32  tuberkel
 * V30 Preparations:
 * - Intro-Screen & LED check duration adjustable by User
 * - VehicleSimulation adjustable by User
 * - Hardcopy adjustable by User
 *
 * Revision 2.8  2009/07/15 08:55:53  tuberkel
 * NEW: #define TESTSCREEN for GUI Tests
 *
 * Revision 2.7  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.6  2009/07/08 21:41:17  tuberkel
 * All compiler defines reviewed:
 * - set to unique usage: set define to 0 or 1
 * - default values set, if not used
 * - see 'Project Editor' for details
 *
 * Revision 2.5  2009/06/24 21:10:10  tuberkel
 * Improvement: Moved DigInDrv_Init() before LCDDrv_Init()
 * - assures correct HW-ID detection (internal pullups)
 * - HW-IFD used to setup correct display contrast
 *
 * Revision 2.4  2009/06/21 17:54:47  tuberkel
 * Changes done by AN:
 * Compass module added
 *
 * Revision 2.3  2007/03/30 10:08:35  tuberkel
 * Added error check for wrong system init values
 *
 * Revision 2.2  2007/03/27 00:06:55  tuberkel
 * BugFix: added missing LCDev_UpdTime()
 *
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

/* basic includes */
#include "standard.h"
#include "stdmsgs.h"
#include "debug.h"
#include "diagnose.h"
#include "msgqueue.h"
#include "timer.h"
#include "sfr62p.h"

/* low level driver stuff */
#include "digoutdr.h"
#include "digindrv.h"

/* high level driver stuff */
#include "led.h"
#include "gpo.h"
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
#include "compassdrv.h"




/* main.c prototype area */
int  main(void);
void VehicleSimulation(void);
void Hardcopy (void);

/* local symbols */
DEVICE_ID eStartDevice;                 // handles startdevice


/* external symbols */
extern DEVFLAGS1_TYPE   EE_DevFlags_1;  // device parameters
extern DEVFLAGS2_TYPE   EE_DevFlags_2;  // device parameters
extern UINT16           EE_WheelSize;     // to control vehicle simulation
extern UINT8            EE_LogoDelay;    // Eeprom value;



/* external symbols for diagnostic service */
extern  UINT16  wUSPStart;              // user stack pointer start value
extern  UINT16  wUSPMax;                // user stack pointer max value
extern  UINT16  wISPStart;              // interrupt stack pointer start value
extern  UINT16  wISPMax;                // interrupt stack pointer max value

extern  UINT16  wMilliSecCounter;       // valid values: 0h .. ffffh
extern  UINT16  wSecCounter;            // valid values: 0h .. ffffh

extern  DBGFILT_TYPE    EE_DbgFilter;   // default off, use DebugSetFilterDetails() to change
extern  DBGDETDIR_TYPE  EE_DbgDetails;  // default uart, use DebugSetFilterDetails() to change

extern  BIKE_TYPE       EE_BikeType;      // bike type selcetion




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
    ODS(DBG_SYS,DBG_INFO,"\n\rInitialize Drivers:");
    Error = TimerInit();            /* start our both timers (ta0 1kHz, ta1 50 Hz) */
    Error = iicInit();              /* prepare iic ports and eeprom type (needs TimerInit()!)*/
    Error = SysPar_CheckFirstInit();    /* check, wether eeprom/nvram update necessary */
    Error = SysPar_InitSystemPar();     /* initialize all system parameters from eeprom/nvram */
    Error = TimeDateInit();         /* initialize system time & date stuff */
    Error = DigOutDrv_Init();       /* all digital output ports & PWM out */
    Error = DigInDrv_Init();        /* prepare keyboard ports and other input ports */
    Error = DisplInit(FALSE);       /* prepare lcd output & clear screen */
    Error = GPO_Init();              /* prepare general purpose output ports */
    Error = LED_Init();              /* prepare led output ports & service */
    Error = Beep_Init();             /* prepare beeper output ports & service */
    Error = MsgQInit();             /* reset message queue */
    Error = MeasDrvInit();          /* measurement init stuff (ta2, ta3, ta4, tb2, int1, int0) */
    Error = AnaInInit();            /* A/D converter for all measurements  */
    Error = Surv_Init();             /* vehicle surveillance */
#if (COMPASSDRV==1)
    Error = CompDrv_Init();          /* inits UART0 and its receive interrupt for compass use */
#endif // COMPASSDRV

    /* check: hw self diagnostic test ---------------- */
    /* (all keys pressed together at startup) */
    if ( DigInDrv_Key_GetStates() == ( KEYFL_OK | KEYFL_UP | KEYFL_DOWN ) )
    {
        // HW SELF TEST MODE ============================================
        TimerRegisterEntryFunction( AnaInDrvTriggerADConverter );   /* generation of AD samples in single sweep mode */
        Error = HWTDev_Init();                                 /* HW test device */
        eStartDevice = DEVID_HWTEST;                                /* start HW TEST MODE */
        MSG_BUILD_SETFOCUS(Msg, DEVID_UNKNOWN, eStartDevice);       /* give focus to that device */
        MsgQPostMsg(Msg, MSGQ_PRIO_LOW);                            /* post message */
    }
    else
    {
        // NORMAL USER MODE =============================================
        /* device screen inits --------------------------- */
        ODS(DBG_SYS,DBG_INFO,"\n\rInitialize Devices:");
        Error = IntroDev_Init();                                  /* intro screen device */
        Error = MainDev_Init();                                   /* main device (speed&rpm) */
        Error = NV_TripCom_ADev_Init();                                   /* trip counter device */
        Error = MonDev_Init();                                /* monitor device */
        #if(BIKE_MOTOBAU==1)                                        /* special MOTOBAU behaviour */
        Error = LCDev_Init();                                 /* LapCounter device */
        #endif // BIKE_MOTOBAU
        Error = SetDev_Init();                                    /* settings device */
        #if(TESTSCREEN==1)
        Error = TestScreen_Init();                                   /* testscreen device */
        #endif

        /* Display & LED 'HW pseudo test' ---------------- */
        LCDDrvSetBacklightLevel(TRUE, 63);  // switch on Backlight
        if(EE_LogoDelay > 0)                 // only if enabled by user:
        {   IntroDev_Show(TRUE);          //    show 'splash screen'
            PORT_LED = PORT_LED_MASK;       //    all LEDs on
            Delay_ms(EE_LogoDelay*100);      //    wait (given in 1/10 sec, set as ms)
            IntroDev_Show(FALSE);         //    clear 'splash screen'
            PORT_LED &= ~PORT_LED_MASK;     //    all LEDs off
        }

        /* Register cyclicely called (50Hz) fast functions --------- */
        TimerRegisterEntryFunction( DigInDrv_Key_CheckKeys );      /* check keys */
        TimerRegisterEntryFunction( DigInDrv_CheckAllPorts );       /* update of standard LEDs for Turn/HighBeam/Neutral */
        TimerRegisterEntryFunction( SysPar_CyclicSaveValues );          /* check/save eeprom values */
        TimerRegisterEntryFunction( DevCyclicRefresh );             /* generation of MSG_SCREEN_RFRSH */
        TimerRegisterEntryFunction( TimeDateUpdate );               /* RTC check */
        TimerRegisterEntryFunction( AnaInDrvTriggerADConverter );   /* generation of AD samples in single sweep mode */
        TimerRegisterEntryFunction( DigOutDrv_Service );            /* support PWM control for all digital outs */
        TimerRegisterEntryFunction( DigInDrv_GPI_UpdateMeas );       /* support PWM control for all digital input */
        TimerRegisterEntryFunction( Surv_ProcessAll );               /* process complete surveillance for infos/warnings/errors */
        #if(BIKE_MOTOBAU==1)                                        /* special MOTOBAU behaviour */
        TimerRegisterEntryFunction( LCDev_UpdTime );             /* enable background lapcounter feature */
        #endif // BIKE_MOTOBAU

        /* TEST LED/GPO PWM Output (kept alive via timer interrupt */
        #if (TEST_DIGOUT_PWM==1)
        {
            UINT16 stay, on, off, duration;
            stay        = 1;
            on          = 100;
            off         = 100;
            duration    = 10000;
            LED_SetNewState( eLED_INFO, on, off, duration );
            GPO_SetNewState( eGPO_1,   on, off, duration );
            while(stay);
        }
        #endif // (TEST_DIGOUT_PWM==1)

        // set start screen ------------------------------ */

        /* check for basic eeprom content error */
        if (  (EE_DevFlags_1.flags.ActDevNr <  DEVID_MAIN)           /* eeprom value in valid area? */
            ||(EE_DevFlags_1.flags.ActDevNr >= DEVID_LAST) )
        {   ODS1( DBG_SYS, DBG_ERROR, "Invalid EE_DevFlags_1.flags.ActDevNr %u corrected!", EE_DevFlags_1.flags.ActDevNr );
            EE_DevFlags_1.flags.ActDevNr = DEVID_MAIN;               /* set main device as default */
        }

        /* prevent from starting with HW-Test all the time */
        if (EE_DevFlags_1.flags.ActDevNr == DEVID_HWTEST)            /* eeprom saved hw test state? */
            EE_DevFlags_1.flags.ActDevNr = DEVID_MAIN;               /* set main device as default */

        /* for device/objects test only: bring GUI testscreen to top */
        #if(TESTSCREEN==1)
        EE_DevFlags_1.flags.ActDevNr = DEVID_TESTSCREEN;             /* enable our gui testscreen */
        #endif

        /* start NORMAL USER MODE */
        eStartDevice = EE_DevFlags_1.flags.ActDevNr;                 /* select start device */
        MSG_BUILD_SETFOCUS(Msg, DEVID_UNKNOWN, eStartDevice);       /* give focus to that device */
        MsgQPostMsg(Msg, MSGQ_PRIO_LOW);                            /* post message */

    }

    /* THE main loop --------------------------------- */
    while (1)
    {
        /* MessagePump: look for messages & execute them */
        Error = MsgQPumpMsg(MSG_NULL_MSG);

        /* if hw self diagnostic test active */
        if (eStartDevice == DEVID_HWTEST)
            HWTDev_Show(TRUE);

        /* if enabled: RPM+WHEEL simulation support */
        if (EE_DevFlags_2.flags.VehicSimul == TRUE)
            VehicleSimulation();

        /* if enabled: Grafic Hardcopy support */
        if (EE_DevFlags_2.flags.Hardcopy == TRUE)
            Hardcopy();

        /* if defined: Compass support */
        #if (COMPASSDRV==1)
        CompDrv_Task();
        #endif
    }
    return 0;
}




/***********************************************************************
 * FUNCTION:    VehicleSimulation
 * DESCRIPTION: Processes a Vehicle Simulation sequence,
 *              simulates WHEEL + RPM input via ISR calls
 * PARAMETER:   -
 * RETURN:      -
 * COMMENT:     To simulate real SIXO action while nobody moves...
 *********************************************************************** */
void VehicleSimulation(void)
{
    // delayed start to prevent ISR overflows
    if ( wSecCounter < SIM_STARTDELAY )
        return;

    // process simulation sequence
    SimVehicSimulation(SIM_SEQUENCE);
}




/***********************************************************************
 * FUNCTION:    Hardcopy
 * DESCRIPTION: Send a screen hardcopy in BMP format to printf-uart (debug)
 *              if HIGHBEAM state changes from LOW->HIGH only!
 * PARAMETER:   -
 * RETURN:      -
 * COMMENT:     FOR DOCUMENATATION PURPOSE ONLY!
 *********************************************************************** */
void Hardcopy (void)
{
    #include "digindrv.h"
    #include "displdrv.h"
    extern void LCDDrvHardcopy(void);
    static BOOL fOldState = 0;

    // detect state change: HBEAM right now activated?
    if (  (DigIn_HBeam == 1 )
        &&(fOldState   == 0 ) )
    {
        LCDDrvHardcopy();   // send hardcopy!
        fOldState = 1;
    }

    // detect state change: HBEAM right now released?
    if (  (DigIn_HBeam == 0 )
        &&(fOldState      == 1 ) )
    {
        fOldState = 0;
    }
}






