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
 *  Module:         Measurement Driver
 *  Purpose:        Low level routines for all Measurements,
 *                  calculations, filters
 *  Comments:
 *                  Note, that the counters are down-counting, so that the
 *                  requested time period is (?_MAX - CounterValue)!
 *
 ************************************************************************ */


/* inlcudes */
#include "standard.h"
#include "sfr62.h"
#include "digoutdr.h"
#include "timer.h"
#include "debug.h"
#include "measdrv.h"
#include "measure.h"



/* external symbols */
extern  UINT16      wMilliSecCounter;   /* valid values: 0h .. ffffh */
extern  UINT16      wWheelSize;         /* wheel size in mm, to be read from eeprom */

extern  DIST_TYPE   VehicDist;          /* vehicle distance */
extern  DIST_TYPE   TripA;              /* TripCounter A */
extern  DIST_TYPE   TripB;              /* TripCounter B */
extern  DIST_TYPE   FuelDist;           /* fuel distance */


/* module protected internal symbols */
static UINT16 wWheelPeriod4;            /* Wheel period in 200 탎ec/Digit (1/4 per new value) */
static UINT16 wWheelPeriodLast;         /* Wheel period in 200 탎ec/Digit, newest value */
static UINT16 wRPMPeriod8;              /* RPM period in 10 탎ec/Digit (1/8 per new value)*/
static UINT16 wRPMPeriod32;             /* RPM period in 10 탎ec/Digit (1/32 per new value)*/


/* module internal functions */
void MeasDrvClearWheelSamples(void);
void MeasDrvClearRPMSamples(void);




/***********************************************************************
 *  FUNCTION:       MeasDrvInit
 *  DESCRIPTION:    Main-Init to call all measurement init stuff for
 *                  timers and isr's
 *  PARAMETER:      -
 *  RETURN:         error
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MeasDrvInit(void)
{
    ERRCODE RValue = ERR_OK;

    /* port init */
    WheelPort   = 0;
    RPMPort     = 0;
    WheelPortD  = PIN_IN;
    RPMPortD    = PIN_IN;

    /* timer & intx inits */
    RValue = MeasDrvInitWheel();       /* init & start velocity measurement */
    RValue = MeasDrvInitRPM();         /* init & start engine rpm measurement */

    ODS(DBG_SYS,DBG_INFO,"MeasDrvInit() done!");
    return RValue;
}

/***********************************************************************
 *  FUNCTION:       MeasDrvInitWheel
 *  DESCRIPTION:    Init & Start all timers for wheel speed measurement
 *  PARAMETER:      -
 *  RETURN:         errorcode
 *  COMMENT:        Note1:  We use two timers, to get a sufficient accuracy
 *                          in calculating wheel speed:
 *                              TB2: Pre-Timer, 5 kHz, Source for TA2
 *                              TA2: UpCounter, 200usec/Digit
 *                  Note2:  TA2 contains the wheel impuls contacts time period
 *                          for 1 wheel round -> base for km/h
 *                  Note3:  There is no need to use the TB2 isr, as we use
 *                          it as a hw source for timer TA2.
 *                  Note4:  Don't set TA2 start flag here!
 *                          -> start TA2 if first impuls detected!
 *********************************************************************** */
ERRCODE MeasDrvInitWheel(void)
{
    ERRCODE RValue = ERR_OK;

    INT_GLOB_DISABLE            /* disable ALL interrupts while changing settings */

    /* Init TB2: 200 usec Prescaler */
    tb2s    = 0;                /* clear TB2 start flag */
    tb2mr   = 0x00;             /* TB2 Mode register: 'Timer mode', count source 'f1' = 10 MHz */
    tb2     = 2000;             /* set TB2 cont value & reload register to 200 usec/Interrupt */
    tb2ic   = 0x00;             /* TB2 isr prio level: 0 = disabled */
    tb2s    = 1;                /* set TB2 start flag -> RUN! */

    /* Init TA2: wheel sensor period counter (200 usec/Digit) */
    fWheelCnt   = FALSE;        /* clear TA2 start flag */
    ta2mr       = 0x01;         /* TA2 Mode register: 'CounterMode', 'No Output', 'Use U/D Flag', 'Reload' */
    ta2ud       = 0;            /* TA2 Up/down Flag: 0 = count down! */
    ta2tgl      = 1;            /* TA2 trigger select: TB2 Overflow selected!*/
    ta2tgh      = 0;            /* (together with ta2tgl) */
    wWheelCnt   = 0;            /* leave counter value empty! (restarted later)*/
    ta2ic       = 0x03;         /* TA2 isr prio level: 3 = lower than msCounter*/

    /* Init INT0 wheel sensor isr */
    int0ic     = 0x05;          /* WHEEL int0 interrupt control register settings:
                                    - level set to 5
                                    - polarity flag to 'falling edge'
                                    - request cause select set to 'one edge' (ifsr2=0 default) */

    MeasDrvClearWheelSamples(); /* reset  complete sample buffer */

    INT_GLOB_ENABLE             /* enable all ISRs */
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       MeasDrvInitRPM
 *  DESCRIPTION:    Init & Start all timers for engine rpm measurement
 *  PARAMETER:      -
 *  RETURN:         errorcode
 *  COMMENT:        Note1:  We use two timers, to get a sufficient accuracy
 *                          in calculating engine speed:
 *                              TA4: Pre-Timer, ? kHz, Source for TA3
 *                              TA3: UpCounter, ? sec/Digit
 *                  Note2:  TA3 contains the wheel impuls time period
 *                          for an ignition cord -> base for RPM
 *                  Note3   There is no need to use the TA4 isr, as we use
 *                          it as a hw source for timer TA3.
 *                  Note4:  Don't set TA3 start flag here!
 *                          -> start TA3 if first impuls detected!
 *********************************************************************** */
ERRCODE MeasDrvInitRPM(void)
{
    ERRCODE RValue = ERR_OK;

    INT_GLOB_DISABLE            /* disable ALL interrupts while changing settings */

    /* Init TA4: 10 usec Prescaler for TA3 */
    ta4s    = 0;                /* clear TA4 start flag */
    ta4mr   = 0x00;             /* TA4 Mode register: 'Timer mode', 'f1', 'No Gate', 'No Output' */
    ta4ud   = 0;                /* TA4 Up/down Flag: 0 = count down! */
    ta4     = 100;              /* set TA4 cont value & reload register to 10 usec/Interrupt */
    ta4ic   = 0x00;             /* TA4 isr prio level: 0 = disabled!*/
    ta4s    = 1;                /* set TA4 start flag -> RUN! */

    /* Init TA3: engine rpm sensor period counter (10 usec/Digit) */
    fRPMCnt = FALSE;            /* clear TA3 start flag */
    ta3mr   = 0x01;             /* TA3 Mode register: 'CounterMode', 'No Output', 'Use U/D Flag', 'Reload' */
    ta3ud   = 0;                /* TA3 Up/down Flag: 0 = count down! */
    ta3tgl  = 1;                /* TA3 trigger select: TA4 Overflow selected!*/
    ta3tgh  = 1;                /* (together with ta3tgl) */
    wRPMCnt = 0;                /* leave counter value empty! (restarted later)*/
    ta3ic   = 0x03;             /* TA3 isr prio level: 3 = lower than msCounter*/

    /* Init INT1 engine rpm sensor isr */
    int1ic     = 0x06;          /* RPM int1 interrupt control register settings:
                                    - level set to 6
                                    - polarity flag to 'falling edge'
                                    - request cause select register flag to 'one edge' (ifsr2=0 default) */

    MeasDrvClearRPMSamples();      /* reset  complete sample buffer */

    INT_GLOB_ENABLE             /* enable all ISRs */
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       PreScaleWheel_ISR
 *  DESCRIPTION:    ISR for TB2 200 usec timer
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        ONLY FOR DEBUG PURPOSE BREAKS!
 *                  This ISR is normaly disabled by register tb2ic !
 *********************************************************************** */
#pragma INTERRUPT   PreScaleWheel_ISR
void PreScaleWheel_ISR(void)
{
    TOGGLE_PAD29;               /* toggle port pin (debug only) */
}


/***********************************************************************
 *  FUNCTION:       PreScaleRPM_ISR
 *  DESCRIPTION:    ISR for TA4 10 usec timer
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        ONLY FOR DEBUG PURPOSE BREAKS!
 *                  This ISR is normaly disabled by register ta4ic !
 *********************************************************************** */
#pragma INTERRUPT   PreScaleRPM_ISR
void PreScaleRPM_ISR(void)
{
    TOGGLE_PAD30;               /* toggle port pin (debug only) */
}



/***********************************************************************
 *  FUNCTION:       WheelOverflow_ISR
 *  DESCRIPTION:    ISR for TA2 wheel period counter (200 usec/digit)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        When this interrupt occures, the wheel sensor counter
 *                  has reached timeout after WHEEL_MAXVALUE dekrements!
 *                  (Means, that wheel stands still or is too slow)
 *********************************************************************** */
#pragma INTERRUPT   WheelOverflow_ISR
void WheelOverflow_ISR(void)
{
    TOGGLE_PAD29;               /* toggle port pin (debug only) */
    fWheelCnt   = FALSE;        /* stop counter after timeout */
    wWheelCnt   = 0;            /* set and remain at max value */
    MeasDrvClearWheelSamples(); /* reset complete sample buffer */
}



/***********************************************************************
 *  FUNCTION:       RPMOverflow_ISR
 *  DESCRIPTION:    ISR for TA3 engine rpm period counter (10 usec/digit)
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        When this interrupt occures, the rpm sensor counter
 *                  has reached timeout after RPM_MAXVALUE dekrements!
 *                  (Means, that engine stands still or is too slow)
 *********************************************************************** */
#pragma INTERRUPT   RPMOverflow_ISR
void RPMOverflow_ISR(void)
{
    TOGGLE_PAD30;               /* toggle port pin (debug only) */
    fRPMCnt = FALSE;            /* stop counter after timeout */
    wRPMCnt = 0;                /* set and remain at min value */
    MeasDrvClearRPMSamples();   /* reset complete sample buffer */
}




/***********************************************************************
 *  FUNCTION:       WheelSensor_ISR
 *  DESCRIPTION:    Interrupt service to handle wheel sensor impuls
 *                  given at m16c's INT0 port pin
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        At this time we make a copy of the current
 *                  timer value and re-init him to go!
 *                  Note1:  The wheel impuls is already filtered by hardware
 *                  Note2:  Note that we save the elapsed time since last
 *                          impuls in wWheelPeriod[] and NOT the current timer
 *                          value! (the timer downcounts from MAX to 0!)
 *                  Note3:  Period is filtered by:
 *
 *                      Period4 = Period(old) * 3/4 + Period(new) * 1/4
 *
 *********************************************************************** */
#pragma INTERRUPT WheelSensor_ISR
void WheelSensor_ISR(void)
{
    static UINT16   wWheelDist = 0;             /* to sum wheelsize in mm */
    UINT16 wWheelPeriod;                        /* to ge current value */

    TOGGLE_PAD31;                               /* toggle port pin (debug only) */

    /* wheel speed stuff ------------------- */
    fWheelCnt   = FALSE;                        /* stop counter now! */
    wWheelPeriod = WHEEL_MAXVALUE - wWheelCnt;  /* save elapsed time */
    wWheelCnt   = WHEEL_MAXVALUE;               /* reload timer */
    fWheelCnt   = TRUE;                         /* restart timer now! */

    /* save most actual wheel period */
    wWheelPeriodLast = wWheelPeriod;            /* save last used index */

    /* calculated filtered wheel period */
    wWheelPeriod4 = (wWheelPeriod4 >> 2) * 3 + (wWheelPeriod >> 2);

    /* distance calculations stuff --------- */
    wWheelDist += wWheelSize;                   /* add to wheel distance in mm */
    if (wWheelDist > MM_OVFL)                   /* check millimeter overflow */
    {
        /* 10 meter overflow detected! */
        wWheelDist -= MM_OVFL;                  /* take rest(!) for next calculations */

        /* increment all distance counters */
        VehicDist.dkm++;                            /* vehicle distance */
        TripA.dkm++;                                /* TripA distance */
        TripB.dkm++;                                /* TripB distance */
        FuelDist.dkm++;                             /* FuelDist distance calculations */
    }
}



/***********************************************************************
 *  FUNCTION:       RPMSensor_ISR
 *  DESCRIPTION:    Interrupt service to handle engines rounds per
 *                  minute impuls given at m16c's INT1 port pin
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        At this time we make a copy of the current
 *                  timer value and re-init him to go!
 *                  Note1:  The rpm impuls is already filtered by hardware
 *                  Note2:  Note that wRPMPeriod saves the time since last
 *                          impuls (not the current timer value, the timer
 *                          decremenst from MAX to 0!)
 *                  NOTE3:  The functions supports two filtererd values:
 *
 *                      RPM8  = RPM8(old)  *  7/8  + RPM8(new)  * 1/8
 *
 *                      RPM32 = RPM32(old) * 31/32 + RPM32(new) * 1/32
 *
 *********************************************************************** */
#pragma INTERRUPT RPMSensor_ISR
void RPMSensor_ISR(void)
{
    UINT16 wRPMPeriod;                              /* just to save the current value and restart timer */

    TOGGLE_PAD32;                                   /* toggle port pin (debug only) */

    /* get current RPM timer value */
    fRPMCnt     = FALSE;                            /* stop counter now! */
    wRPMPeriod  = RPM_MAXVALUE - wRPMCnt;           /* save elapsed time */
    wRPMCnt     = RPM_MAXVALUE;                     /* reload timer */
    fRPMCnt     = TRUE;                             /* restart timer now! */

    /* calculate the filtered RPM values */
    wRPMPeriod8  = (wRPMPeriod8 >> 3)  * 7  + (wRPMPeriod >> 3);
    wRPMPeriod32 = (wRPMPeriod32 >> 5) * 31 + (wRPMPeriod >> 5);
}



/***********************************************************************
 *  FUNCTION:       MeasDrvClearWheelSamples
 *  DESCRIPTION:    resets sample buffer
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        sets samples to max period = slowest wheel speed
 *********************************************************************** */
void MeasDrvClearWheelSamples(void)
{
    wWheelPeriod4    = UINT16_MAX;
    wWheelPeriodLast = UINT16_MAX;
}


/***********************************************************************
 *  FUNCTION:       MeasDrvClearRPMSamples
 *  DESCRIPTION:    resets sample buffer
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        sets samples to max period = slowest wheel speed
 *********************************************************************** */
void MeasDrvClearRPMSamples(void)
{
    wRPMPeriod8  = UINT16_MAX;
    wRPMPeriod32 = UINT16_MAX;
}



/***********************************************************************
 *  FUNCTION:       MeasDrvGetWheelPeriod
 *  DESCRIPTION:    Interface to get an averaged wheel period time
 *  PARAMETER:      BOOL fCurrent  TRUE    returns current value
 *                                 FALSE   returns averaged value
 *  RETURN:         UINT16 wheel period time in 200 탎ec/digit
 *  COMMENT:        -
 *********************************************************************** */
UINT16 MeasDrvGetWheelPeriod(BOOL fCurrent)
{
    UINT16  RValue;

    /* vehicle is very slow? */
    if (fCurrent == TRUE)
            RValue = wWheelPeriodLast;      /* YES: return last sample un-averaged */
    else    RValue = wWheelPeriod4;         /* NO:  return averaged value */

    /* just a security check to prevent 'division by zero' */
    if (RValue == 0)
        RValue = UINT16_MAX;

    return RValue;
}


/***********************************************************************
 *  FUNCTION:       MeasDrvGetRPMPeriod
 *  DESCRIPTION:    Interface to get an averaged rpm period time
 *  PARAMETER:      BOOL fFast  TRUE    returns the faster changing value
 *                              FALSE   returns the the slower changing value
 *  RETURN:         UINT16 averaged rpm period time in 10 탎ec/digit
 *  COMMENT:        -
 *********************************************************************** */
UINT16 MeasDrvGetRPMPeriod(BOOL fFast)
{
    UINT32  dwScratch = 0;
    UINT16  RValue;
    UINT8   bSample;

    /* which filter depth is requested? */
    if (fFast == TRUE)
            RValue = wRPMPeriod8;           /* use less deep filtered value */
    else    RValue = wRPMPeriod32;          /* use stronger low pass filter */

    /* just a security check to prevent 'division by zero' */
    if (RValue == 0)
        RValue = UINT16_MAX;

    return RValue;
}


