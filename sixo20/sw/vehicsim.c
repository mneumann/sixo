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



/* external symbols */
extern UINT16  wMilliSecCounter;        // valid values: 0h .. ffffh
extern UINT16  wSecCounter;        // valid values: 0h .. ffffh



/* module global symbols */
static PSIM_TYPE    WheelSim;
static PSIM_TYPE    RPMSim;


/* speed & rpm simulation sequence */
SIMULCNTR SimulCntrl[] =
{
    // km/h    km/h     RPM     RPM
    // Start    End     Start   End     sec
    // -----  -------  ------  ------  -----
    {     0,      0,       0,      0,   10 },
    {     0,      0,       0,   1200,    5 },
    {     0,      0,    1200,   1200,    5 },
    {     0,     50,    1200,   3000,    5 },
    {    50,    160,    3000,   6500,   20 },
    {   160,    160,    6500,   6500,   10 },
    {   160,     50,    6500,   2000,   20 },
    {    50,     50,    2000,   2000,   10 },
    {    50,      0,    2000,   1200,   10 },
    {     0,      0,    1200,   1200,    5 },
    {     0,      0,    1200,      0,    2 }
    // -----  -------  ------  ------  -----
};



 /***********************************************************************
 *  FUNCTION:       SimWheelSpeedActive
 *  DESCRIPTION:    Starts/Stops wheel speed simulation
 *  PARAMETER:      BOOL fActive    TRUE = start simulation, FALSE = stop
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SimWheelSpeedActive(BOOL fActive)
{
    WheelSim.fActive = fActive;
}




 /***********************************************************************
 *  FUNCTION:       SimWheelSpeedSet
 *  DESCRIPTION:    Interface to initialize wheel speed simulator
 *  PARAMETER:      UINT8 wSpeed1       start speed in km/h
 *                  UINT8 wSpeed2       end speed in km/h
 *                  UINT8 bTransTime    transtion time from 1->2 in sec
 *  RETURN:         -
 *  COMMENT:        This function initiates a simulation of vehicle
 *                  acceleration or constant speed. The wheel impuls
 *                  ISR will cyclicly be called like a wheel would do
 *                  at this vehicle motion.
 *
 *                  Examples for use (at wheel size of 2000 mm):
 *
 *                  a) Constant speed of ca. 100 km/h:
 *                      SimSetWheelSpeed( 72, 72, 0)
 *                  b) Acceleration from 0..100 km/h in 5 seconds:
 *                      SimSetWheelSpeed( 2000, 72, 5)
 *                  c) braking down from 100..50 km/h in 10 seconds:
 *                      SimSetWheelSpeed( 72, 144, 10)
 *
 *********************************************************************** */
void SimWheelSpeedSet( UINT16 wSpeed1, UINT16 wSpeed2, UINT8 bTransTime)
{
    /* save input data just to know what we do */
    WheelSim.wStartPeriod_ms    = KMH2PERIOD(wSpeed1);
    WheelSim.wEndPeriod_ms      = KMH2PERIOD(wSpeed2);
    WheelSim.bTransition_sec    = bTransTime;

    /* calculate simulator behavior */
    if (WheelSim.wStartPeriod_ms == WheelSim.wEndPeriod_ms)
    {
        /* use constant speed */
        WheelSim.eMode = WSIM_CONST;
        WheelSim.wCurrentPeriod_ms = WheelSim.wStartPeriod_ms;
    }
    else
    {
        /* use acceleration */
        WheelSim.eMode = WSIM_ACC;
        WheelSim.wCurrentPeriod_ms = WheelSim.wStartPeriod_ms;
        WheelSim.iPeriodStepSize_ms = ((INT16)WheelSim.wEndPeriod_ms - (INT16)WheelSim.wStartPeriod_ms) / (INT16)WheelSim.bTransition_sec;
    }
}






/***********************************************************************
 *  FUNCTION:       SimWheelSpeedControl
 *  DESCRIPTION:    Controls the simulation of wheel speed, has to be
 *                  repeatly called f.e. from main loop
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        This function controls for itself to be active only
 *                  once per second.
 *********************************************************************** */
void SimWheelSpeedControl(void)
{
    static  UINT16 wLastPeriodUpdate  = 0;  /* last simulation call in ms*/
    static  UINT16 wLastISRCall = 0;        /* last ISR call */
            UINT16 wThisSimCall = 0;        /* current time stamp in ms */

    /* anything to do? */
    if (WheelSim.fActive == TRUE)
    {
        /* basic stuff */
        TimerGetSys_msec(wThisSimCall);


        /* do we need next wheel impuls? */
        if ((wThisSimCall - wLastISRCall) > WheelSim.wCurrentPeriod_ms)
        {
            WheelSensor_ISR();                  /* calls the ISR of the wheel sensor */
            wLastISRCall = wThisSimCall;        /* reset recall time */
        }

        /* min. 1000 ms elapse since last call? */
        if ( (wThisSimCall - wLastPeriodUpdate) >= 1000 )
        {
            /* update wheel period only in accelerated sim mode */
            if (WheelSim.eMode == WSIM_ACC)
            {
                WheelSim.wCurrentPeriod_ms += WheelSim.iPeriodStepSize_ms;
                /* handle underflow */
                if (  (WheelSim.wCurrentPeriod_ms > WheelSim.wStartPeriod_ms)
                    &&(WheelSim.iPeriodStepSize_ms < 0                      ) )
                {
                    WheelSim.wCurrentPeriod_ms = WheelSim.wEndPeriod_ms;
                }
            }

            /* check: CurrentPeriod reached EndPeriod? */
            if (  (  (WheelSim.wCurrentPeriod_ms >= WheelSim.wEndPeriod_ms)
                   &&(WheelSim.iPeriodStepSize_ms > 0                     ) )   /* up counting & end */
                ||(  (WheelSim.wCurrentPeriod_ms <= WheelSim.wEndPeriod_ms)
                   &&(WheelSim.iPeriodStepSize_ms < 0                     ) ) )  /* down counting & end */
            {
                /* change to constant speed mode */
                WheelSim.eMode = WSIM_CONST;
            }
            wLastPeriodUpdate = wThisSimCall;
        }
    }
}



/***********************************************************************
 *  FUNCTION:       SimWheelSpeedSequence(void)
 *  DESCRIPTION:    Handling of 65 second simulation sequence
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Sequence controled by SimulCntrl[]
 *********************************************************************** */
void SimWheelSpeedSequence(void)
{
    static  UINT8       bSeqNr = sizeof(SimulCntrl)/sizeof(SIMULCNTR); // current sequence index
    static  UINT16      wSeqTime = 0xffff;      // current sequence time in sec.
    static  UINT16      wLastSimCall = 1;       // last call time stamp in sec.
            UINT16      wThisSimCall = 0;       // current time stamp in sec.

    /* get time stamp */
    TimerGetSys_sec(wThisSimCall);

    /* do not process more than 1/second */
    if (wThisSimCall == wLastSimCall)
        return;

    /* check: time to change to next sequence part? */
    if(wSeqTime > SimulCntrl[bSeqNr].bTransition)
    {
        /* switch to next sequence part */
        bSeqNr++;                                           // next part
        if(bSeqNr >= (sizeof(SimulCntrl)/sizeof(SIMULCNTR)))// part out of range?
            bSeqNr = 0;                                     // restart at first part
        SimWheelSpeedSet(   SimulCntrl[bSeqNr].wSpeedStart, // set new sequence part
                            SimulCntrl[bSeqNr].wSpeedEnd,
                            SimulCntrl[bSeqNr].bTransition);
        wSeqTime = 0;                                       // reset time
    }
    else
    {
        /* wait.. */
        wSeqTime += wThisSimCall - wLastSimCall;            // incr. sequence time
    }
    wLastSimCall = wThisSimCall;                            // save last call time
}



 /***********************************************************************
 *  FUNCTION:       SimRPMSpeedActive
 *  DESCRIPTION:    Starts/Stops wheel speed simulation
 *  PARAMETER:      BOOL fActive    TRUE = start simulation, FALSE = stop
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SimRPMSpeedActive(BOOL fActive)
{
    RPMSim.fActive = fActive;
}




 /***********************************************************************
 *  FUNCTION:       SimRPMSpeedSet
 *  DESCRIPTION:    Interface to initialize engine speed simulator
 *  PARAMETER:      UINT8 bPeriod1      start engine period in ms
 *                  UINT8 bPeriod2      end engine period in ms
 *                  UINT8 bTransTime    transtion time from 1->2 in sec
 *  RETURN:         -
 *  COMMENT:        This function initiates a simulation of engine
 *                  speed acceleration or constant speed. The RPM impuls
 *                  ISR will cyclicly be called like a engine would do.
 *********************************************************************** */
void SimRPMSpeedSet( UINT16 wRPM1, UINT16 wRPM2, UINT8 bTransTime)
{
    /* save input data just to know what we do */
    RPMSim.wStartPeriod_ms    = RPM2PERIOD(wRPM1);
    RPMSim.wEndPeriod_ms      = RPM2PERIOD(wRPM2);
    RPMSim.bTransition_sec    = bTransTime;

    /* calculate simulator behavior */
    if (RPMSim.wStartPeriod_ms == RPMSim.wEndPeriod_ms)
    {
        /* use constant speed */
        RPMSim.eMode = WSIM_CONST;
        RPMSim.wCurrentPeriod_ms = RPMSim.wStartPeriod_ms;
    }
    else
    {
        /* use acceleration */
        RPMSim.eMode = WSIM_ACC;
        RPMSim.wCurrentPeriod_ms = RPMSim.wStartPeriod_ms;
        RPMSim.iPeriodStepSize_ms = ((INT16)RPMSim.wEndPeriod_ms - (INT16)RPMSim.wStartPeriod_ms) / (INT16)RPMSim.bTransition_sec;
    }
}






/***********************************************************************
 *  FUNCTION:       SimRPMSpeedControl
 *  DESCRIPTION:    Controls the simulation of engine speed, has to be
 *                  repeatly called f.e. from main loop
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        This function controls for itself to be active only
 *                  once per second.
 *********************************************************************** */
void SimRPMSpeedControl(void)
{
    static  UINT16 wLastPeriodUpdate_s  = 0;    /* last simulation call */
    static  UINT16 wLastISRCall_ms = 0;         /* last ISR call */
            UINT16 wThisSimCall_ms = 0;         /* current time stamp in ms */
            UINT16 wThisSimCall_s  = 0;         /* current time stamp in sec*/

    /* anything to do? */
    if (RPMSim.fActive == TRUE)
    {
        /* basic stuff */
        TimerGetSys_msec(wThisSimCall_ms);
        TimerGetSys_sec(wThisSimCall_s);

        /* do we need next RPM impuls? */
        if ((wThisSimCall_ms - wLastISRCall_ms) > RPMSim.wCurrentPeriod_ms)
        {
            RPMSensor_ISR();                  /* calls the ISR of the RPM sensor */
            wLastISRCall_ms = wThisSimCall_ms;  /* reset recall time */
        }

        /* check the 1 sec bounding to update RPM period */
        if (wThisSimCall_s == wLastPeriodUpdate_s)
        {
            /* 1 sec still not elapsed, try again later ...*/
            NOP;
        }
        else
        {
            /* min. 1 sec elapse since last call */
            wLastPeriodUpdate_s = wThisSimCall_s;

            /* update RPM period only in accelerated sim mode */
            if (RPMSim.eMode == WSIM_ACC)
            {
                RPMSim.wCurrentPeriod_ms += RPMSim.iPeriodStepSize_ms;
                /* handle underflow */
                if (  (RPMSim.wCurrentPeriod_ms > RPMSim.wStartPeriod_ms)
                    &&(RPMSim.iPeriodStepSize_ms < 0                      ) )
                {
                    RPMSim.wCurrentPeriod_ms = RPMSim.wEndPeriod_ms;
                }
            }

            /* check: CurrentPeriod reached EndPeriod? */
            if (  (  (RPMSim.wCurrentPeriod_ms >= RPMSim.wEndPeriod_ms)
                   &&(RPMSim.iPeriodStepSize_ms > 0                     ) )   /* up counting & end */
                ||(  (RPMSim.wCurrentPeriod_ms <= RPMSim.wEndPeriod_ms)
                   &&(RPMSim.iPeriodStepSize_ms < 0                     ) ) )  /* down counting & end */
            {
                /* change to constant speed mode */
                RPMSim.eMode = WSIM_CONST;
            }
        }
    }
}



/***********************************************************************
 *  FUNCTION:       SimRPMSpeedSequence(void)
 *  DESCRIPTION:    Handling of 65 second simulation sequence
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Sequence wraps around every MilliSecCounter overflow
 *                  after 65 sec and starts again
 *********************************************************************** */
void SimRPMSpeedSequence(void)
{
    static  UINT8       bSeqNr = sizeof(SimulCntrl)/sizeof(SIMULCNTR); // current sequence index
    static  UINT16      wSeqTime = 0xffff;      // current sequence time in sec.
    static  UINT16      wLastSimCall = 1;       // last call time stamp in sec.
            UINT16      wThisSimCall = 0;       // current time stamp in sec.

    /* get time stamp */
    TimerGetSys_sec(wThisSimCall);

    /* do not process more than 1/second */
    if (wThisSimCall == wLastSimCall)
        return;

    /* check: time to change to next sequence part? */
    if(wSeqTime > SimulCntrl[bSeqNr].bTransition)
    {
        /* switch to next sequence part */
        bSeqNr++;                                               // next part
        if(bSeqNr >= (sizeof(SimulCntrl)/sizeof(SIMULCNTR)))    // part out of range?
            bSeqNr = 0;                                         // restart at first part
        SimRPMSpeedSet( SimulCntrl[bSeqNr].wRPMStart,           // set new sequence part
                        SimulCntrl[bSeqNr].wRPMEnd,
                        SimulCntrl[bSeqNr].bTransition);
        wSeqTime = 0;                                           // reset time
    }
    else
    {
        /* wait.. */
        wSeqTime += wThisSimCall - wLastSimCall;            // incr. sequence time
    }
    wLastSimCall = wThisSimCall;                            // save last call time
}



/***********************************************************************
 *  FUNCTION:       SimVehicSimulation
 *  DESCRIPTION:    simple & complete interface to handle vehicle simulation
 *  PARAMETER:      fSequence   SIM_SEQUENCE    supports dynamic sequence of simulation
 *                              SIM_STATIC      supports fixed values for speed/rpm
 *  RETURN:         -
 *  COMMENT:        Will repeatly be called from main loop
 *********************************************************************** */
void SimVehicSimulation(BOOL fSequence)
{
    static BOOL fInitialized = FALSE;

    if (fInitialized == FALSE)
    {
        if (fSequence == SIM_STATIC )
        {
            /* use constant speed */
            SimWheelSpeedSet( 0, 100, 10);        // accelerate speed from 0..100 km/h in 10 sec. and remain at that speed
            SimRPMSpeedSet( 0, 2000, 10);         // accelerate RPM from 0..2000 km/h in 10 sec. and remain at that RPM
        }

        /* simulation main switch */
        SimWheelSpeedActive(TRUE);
        SimRPMSpeedActive(TRUE);

        fInitialized = TRUE;
    }

    if (fSequence == SIM_SEQUENCE)
    {
        /* use a longer simulation sequence */
        SimWheelSpeedSequence();              // handle a long time wheel simulation behaviour
        SimRPMSpeedSequence();                // handle a long time rpm simulation behaviour
    }

    /* handle software interrupts */
    SimWheelSpeedControl();                 // repeatly generate a wheel ISR call
    SimRPMSpeedControl();                   // repeatly generate a rpm ISR call

}



