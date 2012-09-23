/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-25 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         Simulator
 *  Purpose:        Functions & stuff to simulate vehicle action
 *                  and data
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
 *  This information is automatically added while 'commiting' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 3.5  2012/09/23 07:41:54  tuberkel
 * Vehicle Simulation Update:
 * - Coolride GPI supported (but not yet ready)
 * - Simulation set to 'fixed values' (SIM_STATIC)
 *
 * Revision 3.4  2012/07/16 20:52:25  tuberkel
 * Vehicle Simulation:
 * - FuelSensor (GPI0) works
 *
 * Revision 3.3  2012/07/15 20:43:56  tuberkel
 * Intermediate update:
 * - constant simulation ok
 *
 * Revision 3.2  2012/07/15 19:25:56  tuberkel
 * VehicleSimulation completely reviewed
 * - prepared to handle multiple IRQs
 * - new acoustic support
 *
 * Revision 3.1  2012/06/12 19:39:43  tuberkel
 * just comments
 *
 * Revision 3.0  2010/11/07 12:29:56  tuberkel
 * V30 Preparations:
 * - Module 'Vehicle Simualation ' ALWAYS supported
 * - may be activated by user settings
 *
 * Revision 2.3  2009/07/15 09:01:26  tuberkel
 * D-Options reviewed
 *
 * Revision 2.2  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.1  2009/07/08 21:41:17  tuberkel
 * All compiler defines reviewed:
 * - set to unique usage: set define to 0 or 1
 * - default values set, if not used
 * - see 'Project Editor' for details
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.5  2006/02/18 15:09:16  Ralf
 * - bug fix in simulation start handling (no more overflows)
 *
 *
 ************************************************************************ */



/* includes */
#include "m16c.h"
#include "standard.h"
#include "timer.h"
#include "measdrv.h"
#include "vehicsim.h"
#include "digoutdr.h"
#include "digindrv.h"



/* external symbols */
extern UINT16  wSystemTime_ms;   // valid values: 0h .. ffffh
extern UINT16  wSystemTime_sec;        // valid values: 0h .. ffffh


/* software interrupt declaration */
/* (this is necessary to cause an interrupt by software) */
#pragma INTCALL 29 WheelSensor_ISR();
#pragma INTCALL 30 RPMSensor_ISR();
#pragma INTCALL 31 GPI0_Int2_ISR();
#pragma INTCALL  4 GPI1_Int3_ISR();
#pragma INTCALL  9 GPI2_Int4_ISR();
#pragma INTCALL  8 GPI3_Int5_ISR();



/* module global symbols */
SIM_CNTRL       SimSeq;                     // main simulation control sequence
SIM_KIND_CNTRL  SimulKind[SIM_KIND_MAX];    // kind simulation control



/* simulation sequence pattern */
SIM_SEQ_STEP SimPattern[] =
{
//        Dur     km/h        RPM      l/1000km    Coolr%
//        sec   StartEnd   StartEnd    StartEnd   StartEnd
//       -----  ---------  ----------- ---------- ---------
/* 0 */ {   5,    0,   0,     0,    0,   0,    0,   0,   0,  },   // all off & halted
/* 1 */ {   5,    0,   0,   600, 1000,  20,   20,   0,   0,  },   // start engine, halted
/* 2 */ {  10,    0,  50,  1200, 3000,  50,   75,  20,  20,  },   // accelerate  to  50 km/h
/* 3 */ {  10,   50,  50,  3000, 3000,  75,   75,  40,  40,  },   // const speed     50 km/h
/* 4 */ {  10,   50, 150,  3000, 4000,  75,  100,  60,  60,  },   // accelerate  to 160 km/h
/* 5 */ {  10,  150, 150,  4000, 4000, 100,  100,  80,  80,  },   // const speed    160 km/h
/* 6 */ {  10,  150,   0,  4000, 1000, 100,   50, 100, 100,  },   // slow down   to   0 km/h
/* 7 */ {   5,    0,   0,   800,  800,  20,   20,   0,   0,  },   // halted, engine run
//       -----  ---------  ----------- ---------- ---------
};




/***********************************************************************
 *  FUNCTION:       Sim_KindSetup
 *  DESCRIPTION:    Basic setup for one simulation kind
 *  PARAMETER:      pKind       ref to kind
 *                  eKind       type of kind
 *                  fActive     TRUE = start simulation, FALSE = stop
 *                  wScaling    scaling from phys. value to IRQ period
 *                  fClick      TRUE = click for each ISR call
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void Sim_KindSetup   (  SIM_KIND_CNTRL *    pKind,
                        SIM_KIND            eKind,
                        BOOL                fActive,
                        UINT16              wScaling,
                        BOOL                fClick   )
{
    pKind->eKind    = eKind;
    pKind->fActive  = fActive;
    pKind->wScaling = wScaling;
    pKind->fClick   = fClick;
}



 /***********************************************************************
 *  FUNCTION:       Sim_FrequenceSetup
 *  DESCRIPTION:    Interface to initialize one ISR simulator kind
 *  PARAMETER:      pKind           kind of simulation to activate
 *                  iFreqStart      start period in ms
 *                  iFreqEnd        end period in ms
 *                  iDuration       transtion time from Start->End in sec
 *  RETURN:         -
 *  COMMENT:        The adequate ISR will cyclicly be called like the
 *                  hardware would do.
  *********************************************************************** */
void Sim_FrequenceSetup (SIM_KIND_CNTRL * pKind, INT16 iFreqStart, INT16 iFreqEnd, INT16 iDuration)
{
    /* setup new values for this kind */
    pKind->wFreqStart = iFreqStart;
    pKind->wFreqEnd   = iFreqEnd;
    pKind->iFreqDiff  = iFreqEnd - iFreqStart;
    pKind->iDuration  = iDuration;

    /* valid parameters to frequently generate an IRQ? */
    if ( pKind->iDuration > 0 )
    {
        /* calculate frequency steps */
        pKind->iFreqStep        = (iFreqEnd - iFreqStart) / iDuration;
        pKind->wFreqCurr        = iFreqStart;
        pKind->wISRPeriodCurr   = pKind->wScaling / pKind->wFreqCurr;

        /* setup IRQ period base values */
        pKind->wLastFreqUpdate  = wSystemTime_ms;
        pKind->wLastISRCall     = wSystemTime_ms;
    }
    else
    {
        /* settings to prevent any IRQ (needed, if kind not used/enabled) */
        pKind->iFreqStep        = 0;
        pKind->wFreqCurr        = 0;
        pKind->wLastFreqUpdate  = wSystemTime_ms;
        pKind->wLastISRCall     = wSystemTime_ms;
        pKind->wISRPeriodCurr   = 0xffff;
    }
}



/***********************************************************************
 *  FUNCTION:       Sim_Click
 *  DESCRIPTION:    Supports a single short beeper click
 *  PARAMETER:      BOOL    TRUE = generate click
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void Sim_Click (BOOL fClick)
{
    if ( fClick == TRUE )
    {
        /* short 'click' as ISR Indicator */
        DigOutDrv_SetNewState( eDIGOUT_BEEP, 1, 0, 0 );
        DigOutDrv_SetNewState( eDIGOUT_BEEP, 0, 1, 0 );
    }
    else
    {   // nothing to do here
    }
}



/***********************************************************************
 *  FUNCTION:       Sim_FrequenceControl
 *  DESCRIPTION:    Controls the simulation of one kind,
 *                  has to be repeatetly called f.e. from main loop
 *  PARAMETER:      SIM_KIND eKind      kind of simulation to activate
 *  RETURN:         -
 *  COMMENT:        This function controls for itself to be active only
 *                  once per second.
 *********************************************************************** */
void Sim_FrequenceControl (SIM_KIND_CNTRL * pKind)
{
    /* anything to do for this sumlation kind? */
    if (pKind->fActive == TRUE)
    {
        UINT16 wThisISRCall = 0;        /* current time stamp in ms */

        /* get current timestamp  */
        TimerGetSys_msec(wThisISRCall);

        /* ----------------------------------------------- */
        /* check: do we need the next ISR call? */
        if ((wThisISRCall - pKind->wLastISRCall) > pKind->wISRPeriodCurr)
        {
            /* call adequate ISR */
            /* NOTE: In NC30, this can not be done via a function ptr, which is
                     is setup at init time. So we have to call it here explicitly. */
            switch (pKind->eKind)
            {   case SIM_WHEEL: WheelSensor_ISR();  break;
                case SIM_RPM:   RPMSensor_ISR();    break;
                case SIM_FUEL:  GPI0_Int2_ISR();    break;
                case SIM_COOLR: GPI1_Int3_ISR();    break;
                default:                            break;
            }

            /* generate a single click (if enabled) */
            Sim_Click (pKind->fClick);

            /* reset recall time for next use */
            pKind->wLastISRCall = wThisISRCall;
        }

        /* ----------------------------------------------- */
        /* check: do we need to change ISR intervall? */
        if ( (wThisISRCall - pKind->wLastFreqUpdate) >= SIM_SEQSTEPINTV )
        {
            /* YES! step forward to next sequence step! */
            /* update simulation period only in SEQUENCE mode */
            if (SimSeq.fSeqMode == SIM_SEQUENCE)
            {
                /* update current frequenz & period */
                pKind->wFreqCurr     += pKind->iFreqStep;
                pKind->wISRPeriodCurr = pKind->wScaling / pKind->wFreqCurr;
            }
            else
            {   /* do NOT change ISR time intervall in STATIC mode */
            }

            /* save this period update */
            pKind->wLastFreqUpdate = wThisISRCall;
        }
    }
}



/***********************************************************************
 *  FUNCTION:       Sim_SequenceControl(void)
 *  DESCRIPTION:    Control simulation sequence steps
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        The Sequence predefined by SimPattern[]
 *********************************************************************** */
void Sim_SequenceControl(void)
{
    UINT16      wThisISRCall = 0;       // current time stamp in sec.

    /* get a fresh time stamp */
    TimerGetSys_sec(wThisISRCall);

    /* do not process more than 1/second */
    if (wThisISRCall == SimSeq.wSeqLastCheck)
        return;

    /* check: time to change to next sequence step? */
    if (SimSeq.wSeqStepDur == 0)
    {
        /* switch to next sequence part */
        SimSeq.bSeqStepNr++;                                            // next part
        if(SimSeq.bSeqStepNr >= (sizeof(SimPattern)/sizeof(SIM_SEQ_STEP)))  // part out of range?
           SimSeq.bSeqStepNr = 0;                                       // restart at first part

        /* set new sequence part for all simualtion kinds */
        {   SIM_KIND eKind;
            for(eKind = SIM_WHEEL; eKind < SIM_KIND_MAX; eKind++)
            {   Sim_FrequenceSetup( &SimulKind[eKind],
                                    SimPattern[SimSeq.bSeqStepNr].StepVal[eKind].iStart,
                                    SimPattern[SimSeq.bSeqStepNr].StepVal[eKind].iEnd,
                                    SimPattern[SimSeq.bSeqStepNr].iDuration  );
            }
        }

        /* reset time */
        SimSeq.wSeqStepDur = SimPattern[SimSeq.bSeqStepNr].iDuration;
    }
    else
    {   /* wait.. & incr. sequence time */
        SimSeq.wSeqStepDur--;
    }

    /* save last call time */
    SimSeq.wSeqLastCheck = wThisISRCall;
}



/***********************************************************************
 *  FUNCTION:       Sim_Init
 *  DESCRIPTION:    complete simulation initialization
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:
 *********************************************************************** */
void Sim_Init(BOOL fSequence)
{
    /* save given simulation mode (sequence/static) */
    SimSeq.fSeqMode = fSequence;

    /* basic simulation kind setup  */
    Sim_KindSetup( &SimulKind[SIM_WHEEL], SIM_WHEEL, TRUE, SIM_SCALE_WHEEL, SIM_CLICK_WHEEL);
    Sim_KindSetup( &SimulKind[SIM_RPM],   SIM_RPM,   TRUE, SIM_SCALE_RPM,   SIM_CLICK_RPM  );
    Sim_KindSetup( &SimulKind[SIM_FUEL],  SIM_FUEL,  TRUE, SIM_SCALE_FUEL,  SIM_CLICK_FUEL );
    Sim_KindSetup( &SimulKind[SIM_COOLR], SIM_COOLR, TRUE, SIM_SCALE_COOLR, SIM_CLICK_COOLR);

    /* no sequence required? setup static usage! */
    if (SimSeq.fSeqMode == SIM_STATIC )
    {
        /* setup a STATIC behaviour, which will not be changed */
        /* use constant simulation (no sequence) */
        Sim_FrequenceSetup( &SimulKind[SIM_WHEEL],  100,  100, 10);   // constant speed 100 km/h
        Sim_FrequenceSetup( &SimulKind[SIM_RPM  ], 2000, 2000, 10);   // constant RPM 2000 km/h
        Sim_FrequenceSetup( &SimulKind[SIM_FUEL ],   50,   50, 10);   // constant Fuel 5,0 l/100km
        Sim_FrequenceSetup( &SimulKind[SIM_COOLR],   50,   50, 10);   // constant PWM 50%
    }

    /* initialize only once */
    SimSeq.fInit = TRUE;
}



/***********************************************************************
 *  FUNCTION:       Sim_Main
 *  DESCRIPTION:    simple & complete interface to handle vehicle simulation
 *  PARAMETER:      fSequence   SIM_SEQUENCE    supports dynamic sequence of simulation
 *                              SIM_STATIC      supports fixed values for speed/rpm
 *  RETURN:         -
 *  COMMENT:        Will repeatly be called from main loop
 *********************************************************************** */
void Sim_Main(BOOL fSequence)
{
    /* first usage? => init! */
    if (SimSeq.fInit == FALSE)
        Sim_Init(fSequence);    /* one-time init */

    /* at runtime check: support sequence mode changes? */
    if (fSequence == SIM_SEQUENCE)
    {   Sim_SequenceControl();          /* control step-sequence */
    }

    /* always: control software interrupts separately  */
    Sim_FrequenceControl( &SimulKind[SIM_WHEEL] );
    Sim_FrequenceControl( &SimulKind[SIM_RPM  ] );
    Sim_FrequenceControl( &SimulKind[SIM_FUEL ] );
    Sim_FrequenceControl( &SimulKind[SIM_COOLR] );

}



