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
 *  Created:        25-03-2002 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         Simulator
 *  Purpose:        Functions & stuff to simulate vehicle action
 *                  and data
 *  Comments:       -
 *
 ************************************************************************ */

/* includes */
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
 *  PARAMETER:      UINT8 bPeriod1      start wheel period in ms
 *                  UINT8 bPeriod2      end wheel period in ms
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
void SimWheelSpeedSet( UINT16 wPeriod1, UINT16 wPeriod2, UINT8 bTransTime)
{
    /* save input data just to know what we do */
    WheelSim.wStartPeriod_ms    = wPeriod1;
    WheelSim.wEndPeriod_ms      = wPeriod2;
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
    static UINT16 wLastPeriodUpdate_s  = 0;     /* last simulation call */
    static UINT16 wLastISRCall_ms = 0;     /* last ISR call */
    UINT16 wThisSimCall_ms = 0;            /* current time stamp in ms */
    UINT16 wThisSimCall_s  = 0;            /* current time stamp in sec*/

    /* anything to do? */
    if (WheelSim.fActive == TRUE)
    {
        /* basic stuff */
        TimerGetSys_msec(wThisSimCall_ms);
        TimerGetSys_sec(wThisSimCall_s);

        /* do we need next wheel impuls? */
        if ((wThisSimCall_ms - wLastISRCall_ms) > WheelSim.wCurrentPeriod_ms)
        {
            WheelSensor_ISR();                  /* calls the ISR of the wheel sensor */
            wLastISRCall_ms = wThisSimCall_ms;  /* reset recall time */
        }

        /* check the 1 sec bounding to update wheel period */
        if (wThisSimCall_s == wLastPeriodUpdate_s)
        {
            /* 1 sec still not elapsed, try again later ...*/
            NOP;
        }
        else
        {
            /* min. 1 sec elapse since last call */
            wLastPeriodUpdate_s = wThisSimCall_s;

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
        }
    }
}



/***********************************************************************
 *  FUNCTION:       SimWheelSpeedSequence(void)
 *  DESCRIPTION:    Handling of 65 second simulation sequence
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Sequence wraps around every MilliSecCounter overflow
 *                  after 65 sec and starts again
 *********************************************************************** */
void SimWheelSpeedSequence(void)
{
    static PSIM_SEQ bSeqNr = SEQ0;
    UINT16 wThisSimCall_ms = 0;            /* current time stamp in ms */
    TimerGetSys_msec(wThisSimCall_ms);

    /* control different simualtion sequences */
    switch (bSeqNr)
    {
        case SEQ0:
        {
            if (wThisSimCall_ms < 100)
            {
                SimWheelSpeedActive(FALSE);
                SimWheelSpeedSet( 2500, 2500, 0);
                bSeqNr++;
            }
        } break;
        case SEQ1:
        {
            if (wThisSimCall_ms > 1000)
            {
                SimWheelSpeedActive(TRUE);
                SimWheelSpeedSet( 2000, 68, 5);
                bSeqNr++;
            }
        } break;
        case SEQ2:
        {
            if (wThisSimCall_ms > 10000)
            {
                SimWheelSpeedSet( 68, 34, 8);
                bSeqNr++;
            }
        } break;

        case SEQ3:
        {
            if (wThisSimCall_ms > 20000)
            {
                SimWheelSpeedSet( 34, 100, 20);
                bSeqNr++;
            }
        } break;
        case SEQ4:
        {
            if (wThisSimCall_ms > 45000)
            {
                SimWheelSpeedSet( 100, 1000, 10);
                bSeqNr++;
            }
        } break;
        case SEQ5:
        {
            if (wThisSimCall_ms > 55000)
            {
                SimWheelSpeedSet( 1000, 1000, 0);
                bSeqNr++;
            }
        } break;
        case SEQ6:
        {
            if (wThisSimCall_ms > 65000)
            {
                SimWheelSpeedSet( 2500, 2500, 0);
                bSeqNr = SEQ0;
            }
        } break;
        default: break;
    }
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
void SimRPMSpeedSet( UINT16 wPeriod1, UINT16 wPeriod2, UINT8 bTransTime)
{
    /* save input data just to know what we do */
    RPMSim.wStartPeriod_ms    = wPeriod1;
    RPMSim.wEndPeriod_ms      = wPeriod2;
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
    static UINT16 wLastPeriodUpdate_s  = 0;     /* last simulation call */
    static UINT16 wLastISRCall_ms = 0;     /* last ISR call */
    UINT16 wThisSimCall_ms = 0;            /* current time stamp in ms */
    UINT16 wThisSimCall_s  = 0;            /* current time stamp in sec*/

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
    static PSIM_SEQ bSeqNr = SEQ0;
    UINT16 wThisSimCall_ms = 0;            /* current time stamp in ms */
    TimerGetSys_msec(wThisSimCall_ms);

    /* control different simualtion sequences */
    switch (bSeqNr)
    {
        case SEQ0:
        {
            if (wThisSimCall_ms < 100)
            {
                SimRPMSpeedActive(TRUE);
                SimRPMSpeedSet( 10000, 10000, 0);
                bSeqNr++;
            }
        } break;
        case SEQ1:
        {
            if (wThisSimCall_ms > 1000)
            {
                SimRPMSpeedActive(TRUE);
                SimRPMSpeedSet( 2000, 30, 5);
                bSeqNr++;
            }
        } break;
        case SEQ2:
        {
            if (wThisSimCall_ms > 10000)
            {
                SimRPMSpeedSet( 30, 15, 8);
                bSeqNr++;
            }
        } break;

        case SEQ3:
        {
            if (wThisSimCall_ms > 20000)
            {
                SimRPMSpeedSet( 15, 50, 20);
                bSeqNr++;
            }
        } break;
        case SEQ4:
        {
            if (wThisSimCall_ms > 45000)
            {
                SimRPMSpeedSet( 50, 500, 10);
                bSeqNr++;
            }
        } break;
        case SEQ5:
        {
            if (wThisSimCall_ms > 55000)
            {
                SimRPMSpeedSet( 500, 5000, 0);
                bSeqNr++;
            }
        } break;
        case SEQ6:
        {
            if (wThisSimCall_ms > 65000)
            {
                SimRPMSpeedSet( 5000, 5000, 0);
                bSeqNr = SEQ0;
            }
        } break;
        default: break;
    }
}



/***********************************************************************
 *  FUNCTION:       SimVehicSimulation
 *  DESCRIPTION:    simple & complete interface to handle vehicle simulation
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Will repeatly be called from main loop
 *********************************************************************** */
void SimVehicSimulation(void)
{
    static BOOL fInitialized = FALSE;

    if (fInitialized == FALSE)
    {
        /* simulation main switch */
        SimWheelSpeedActive(TRUE);
        SimRPMSpeedActive(TRUE);

        /* use constant speed */
        SimWheelSpeedSet( 1232, 1232, 0);       // initiate a constant wheel speed
        SimRPMSpeedSet( 40, 40, 0);             // initiate a constant engine speed

        fInitialized = TRUE;
    }

    /* use a 65 sec sequence */
    //SimWheelSpeedSequence();              // handle a long time wheel simulation behaviour
    //SimRPMSpeedSequence();                // handle a long time rpm simulation behaviour

    /* handle software interrupts */
    SimWheelSpeedControl();                 // repeatly generate a wheel ISR call
    SimRPMSpeedControl();                   // repeatly generate a rpm ISR call

}




