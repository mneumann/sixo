/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-01 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         Measurement Driver
 *  Purpose:        Low level routines for all Measurements,
 *                  calculations, filters
 *  Comments:
 *                  Note, that the counters are down-counting, so that the
 *                  requested time period is (?_MAX - CounterValue)!
 *
 *  Changes:        Adapted timer reload values to new system frequency
 *                  of SIxO 2.0 AN 28.02.2004
 *                  Select clock f1 for timers, Tachovoreilung AN 14.06.2005
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
 * Revision 3.8  2012/07/15 18:29:03  tuberkel
 * SystemTimer Vars renamed
 *
 * Revision 3.7  2012/06/03 17:45:18  tuberkel
 * Updated API-Function-Name according to Modul-Name
 *
 * Revision 3.6  2012/05/27 17:52:40  tuberkel
 * Corrections for renamed Eeprom/Nvram Variables
 *
 * Revision 3.5  2012/05/27 16:01:40  tuberkel
 * All Eeprom/Nvram Variables renamed
 *
 * Revision 3.4  2012/02/04 22:25:49  tuberkel
 * LEDs renamed
 *
 * Revision 3.3  2012/02/04 21:59:15  tuberkel
 * Toggle Pads renamed
 *
 * Revision 3.2  2012/02/01 21:23:13  tuberkel
 * comments
 *
 * Revision 3.1  2011/05/29 12:43:19  tuberkel
 * BugFix gwWheelImpulse
 * - Typ korrgiert
 * - jetzt auch im Eeprom gesichert
 *
 * Revision 3.0  2010/11/07 14:00:50  tuberkel
 * V30 Preparations - no changes
 *
 * Revision 2.2  2010/11/07 11:56:57  tuberkel
 * V30 Preparations:
 * - New: gwWheelImpulse
 * - Devider/Prescaler for WheelSensor Interrupt
 * - if used, has to be supported by HW-changes too
 *
 * Revision 2.1  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 * Revision 1.10  2006/03/18 09:02:22  Ralf
 * - TripCounter improved to 4 different counters
 *
 *
 ************************************************************************ */



/* inlcudes */
#include "sfr62p.h"
#include "m16c.h"
#include "standard.h"
#include "digoutdr.h"
#include "timer.h"
#include "debug.h"
#include "measdrv.h"
#include "measure.h"



/* external symbols */
extern  UINT16      wSystemTime_ms;   /* valid values: 0h .. ffffh */
extern  UINT16      EE_WheelSize;       /* wheel size in mm, to be read from eeprom */
extern  UINT8       EE_WheelImpPRev;   /* wheel impulses per revolution */

extern  DIST_TYPE   NV_VehicDist;       /* vehicle distance */
extern  DIST_TYPE   NV_TripCnt_A;       /* TripCounter A */
extern  DIST_TYPE   NV_TripCnt_B;       /* TripCounter B */
extern  DIST_TYPE   NV_TripCom_A;       /* TripCommon A */
extern  DIST_TYPE   NV_TripCom_B;       /* TripCommon B */
extern  DIST_TYPE   NV_FuelDist;        /* fuel distance */


/* dedicated measurement values for low/high pass filter depending on current speed */
static UINT32 dwWheelPeriod4;           /* Wheel period in 25 µsec/Digit (1/4 per new value, scaled) */
static UINT16 wWheelPeriodLast;         /* Wheel period in 200 µsec/Digit, newest value */
static UINT16 wRPMPeriod8;              /* RPM period in 10 µsec/Digit (1/8 per new value)*/
static UINT16 wRPMPeriod32;             /* RPM period in 10 µsec/Digit (1/32 per new value)*/


/* module internal functions */
void MeasDrv_ClearWheelSamples(void);
void MeasDrv_ClearRPMSamples(void);




/***********************************************************************
 *  FUNCTION:       MeasDrv_Init
 *  DESCRIPTION:    Main-Init to call all measurement init stuff for
 *                  timers and isr's
 *  PARAMETER:      -
 *  RETURN:         error
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MeasDrv_Init(void)
{
    ERRCODE RValue = ERR_OK;

    /* port init */
    WheelPort   = 0;
    RPMPort     = 0;
    WheelPortD  = 0;
    RPMPortD    = 0;

    /* select clock f1 for timerA and B */
    prcr  = 0x01;
    pclk0 = 1;
    prcr  = 0x00;

    /* timer & intx inits */
    RValue = MeasDrv_InitWheel();       /* init & start velocity measurement */
    RValue = MeasDrv_InitRPM();         /* init & start engine rpm measurement */

    ODS(DBG_SYS,DBG_INFO,"MeasDrv_Init() done!");
    return RValue;
}

/***********************************************************************
 *  FUNCTION:       MeasDrv_InitWheel
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
ERRCODE MeasDrv_InitWheel(void)
{
    ERRCODE RValue = ERR_OK;

    INT_GLOB_DISABLE;            /* disable ALL interrupts while changing settings */

    /* Init TB2: 200 usec Prescaler */
    tb2s    = 0;                /* clear TB2 start flag */
    tb2mr   = 0x00;             /* TB2 Mode register: 'Timer mode', count source 'f1'*/
    tb2     = TB2_RELOAD;       /* set TB2 cont value & reload register to 200 usec/Interrupt */
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

    MeasDrv_ClearWheelSamples(); /* reset  complete sample buffer */

    INT_GLOB_ENABLE;             /* enable all ISRs */
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       MeasDrv_InitRPM
 *  DESCRIPTION:    Init & Start all timers for engine rpm measurement
 *  PARAMETER:      -
 *  RETURN:         errorcode
 *  COMMENT:        Note1:  We use two timers, to get a sufficient accuracy
 *                          in calculating engine speed:
 *                              TA4: Pre-Timer, 100 kHz, Source for TA3
 *                              TA3: UpCounter, ? sec/Digit
 *                  Note2:  TA3 contains the wheel impuls time period
 *                          for an ignition cord -> base for RPM
 *                  Note3   There is no need to use the TA4 isr, as we use
 *                          it as a hw source for timer TA3.
 *                  Note4:  Don't set TA3 start flag here!
 *                          -> start TA3 if first impuls detected!
 *********************************************************************** */
ERRCODE MeasDrv_InitRPM(void)
{
    ERRCODE RValue = ERR_OK;

    INT_GLOB_DISABLE;            /* disable ALL interrupts while changing settings */

    /* Init TA4: 10 usec Prescaler for TA3 */
    ta4s    = 0;                /* clear TA4 start flag */
    ta4mr   = 0x00;             /* TA4 Mode register: 'Timer mode', 'f1', 'No Gate', 'No Output' */
    ta4ud   = 0;                /* TA4 Up/down Flag: 0 = count down! */
    ta4     = TA4_RELOAD;       /* set TA4 cont value & reload register to 10 usec/Interrupt */
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

    MeasDrv_ClearRPMSamples();   /* reset  complete sample buffer */

    INT_GLOB_ENABLE;             /* enable all ISRs */
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
    PIN_TESTPAD9_TOGGLE;               /* toggle port pin (debug only) */
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
    PIN_TESTPAD10_TOGGLE;               /* toggle port pin (debug only) */
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
    PIN_TESTPAD9_TOGGLE;               /* toggle port pin (debug only) */
    fWheelCnt   = FALSE;        /* stop counter after timeout */
    wWheelCnt   = 0;            /* set and remain at max value */
    MeasDrv_ClearWheelSamples(); /* reset complete sample buffer */
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
    PIN_TESTPAD10_TOGGLE;               /* toggle port pin (debug only) */
    fRPMCnt = FALSE;            /* stop counter after timeout */
    wRPMCnt = 0;                /* set and remain at min value */
    MeasDrv_ClearRPMSamples();   /* reset complete sample buffer */
}




/***********************************************************************
 *  FUNCTION:       WheelSensor_ISR
 *  DESCRIPTION:    Interrupt service to handle wheel sensor impuls
 *                  given at m16c's INT0 port pin
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        At this time we make a copy of the current
 *                  timer value and re-init him to go!
 *                  Note1:  The wheel impuls is already debounced by hardware
 *                  Note2:  Note that we save the elapsed time since last
 *                          impuls in dwWheelPeriod and NOT the current timer
 *                          value! (the timer downcounts from MAX to 0!)
 *                  Note3:  Period is filtered by:
 *
 *                           Period4 = Period(old) * 3/4 + Period(new) * 1/4
 *
 *                  Note4:  To account for calculation errors we scale up
 *                          by 8 the values used in the filter calculation.
 *                          For a deeper understanding read the comments in
 *                          anaindrv.c.
 *                  Note5:  To pass the Europeen approvals we have to imple-
 *                          ment a 'Tachovoreilung'. The speedo shows a
 *                          little bit more than the actual speed.
 *                          We are pretty close to the allowed lower value.
 *                          To make sure we don't pass the lower value due
 *                          to +-1 digit errors that may occur we just add
 *                          1 km/h in maindev.c.
 *                          The rest of the 'Tachovoreilung' programmed
 *                          here corrects the assumed increase of wheel
 *                          circumference at higher speeds. Due to the
 *                          nature of the problem we just have to add a
 *                          constant offset to the measured period (see
 *                          pruefbericht_geschwindigkeitsmessung for details).
 *                          At very high speeds where a +-1 digit error
 *                          (+-200 us) leads up to +-2..3 km/h change we need
 *                          a little bit stronger correction to account for
 *                          that error.
 *********************************************************************** */
#pragma INTERRUPT WheelSensor_ISR
void WheelSensor_ISR(void)
{
    static UINT16 wWheelDist    = 0;    /* to sum wheelsize in mm */
    static UINT8  bImpPrescaler = 0;    /* to sum impulses before detectin a COMPLETE wheel revolution */
    UINT32 dwWheelPeriod;               /* to ge current value */

    PIN_TESTPAD11_TOGGLE;                 /* toggle port pin (debug only) */

    /* --------------------------------------------------------------------------------- */
    /* wheel revolution prescaler - EE_WheelImpPRev
       NOTE: Some wheel sensors provide more than 1 impulses/revolution.
             This can be supported by software be setting up the 'gwWheelImpulse'
             up 99 Imp/rev.
             BUT: This has to be set up in hardware too, because of the
             WHEEL input low pass filter (see forum for details). */
    if ( ++bImpPrescaler < EE_WheelImpPRev )
    {   return;         // do nothing, just wait for next ISR trigger to continue sum of impulses
    }
    bImpPrescaler = 0;  // reset prescaler to restart again...

    /* --------------------------------------------------------------------------------- */
    /* wheel speed stuff */
    fWheelCnt     = FALSE;                        /* stop counter now! */
    dwWheelPeriod = WHEEL_MAXVALUE - wWheelCnt;   /* save elapsed time */

    /* --------------------------------------------------------------------------------- */
    /* Tachovoreilung */
    /* Do not touch these values. These tiny magic bits have been thoroughfully  */
    /* evaluated during a night shift. Some beers may have improved the results. */
    /* (Note that there is also a +1 km/h in maindev.c)                          */
    if( dwWheelPeriod > 200 )
         dwWheelPeriod -= 6; /* for speeds of about < 190 km/H @ 2100mm or < 160 km/h @ 1800mm */
    else dwWheelPeriod -= 7;

    /* --------------------------------------------------------------------------------- */
    wWheelCnt   = WHEEL_MAXVALUE;               /* reload timer */
    fWheelCnt   = TRUE;                         /* restart timer now! */

    /* save most actual wheel period */
    wWheelPeriodLast = (UINT16)dwWheelPeriod;   /* save last used index */

    /* calculated filtered wheel period (scaled)*/
    dwWheelPeriod4 = ( 3 * dwWheelPeriod4 + (dwWheelPeriod<<3) + 2 ) >> 2;

    /* --------------------------------------------------------------------------------- */
    /* distance calculations stuff --------- */
    wWheelDist += EE_WheelSize;                   /* add to wheel distance in mm */
    if (wWheelDist > MM_OVFL)                   /* check millimeter overflow */
    {
        /* 10 meter overflow detected! */
        wWheelDist -= MM_OVFL;                  /* take rest(!) for next calculations */

        /* check and increment all distance counters */
        if (NV_VehicDist.dkm < DIST_MAX_VEHIC)
            NV_VehicDist.dkm++;                    /* overall vehicle distance */
        if (NV_TripCnt_A.dkm < DIST_MAX_TRIP)
            NV_TripCnt_A.dkm++;                        /* NV_TripCnt_A distance */
        if (NV_TripCnt_B.dkm < DIST_MAX_TRIP)
            NV_TripCnt_B.dkm++;                        /* NV_TripCnt_B distance */
        if (NV_TripCom_A.dkm < DIST_MAX_TRIP)
            NV_TripCom_A.dkm++;                        /* NV_TripCom_A distance */
        if (NV_TripCom_B.dkm < DIST_MAX_TRIP)
            NV_TripCom_B.dkm++;                        /* NV_TripCom_B distance */
        if (NV_FuelDist.dkm < DIST_MAX_FUEL)
            NV_FuelDist.dkm++;                     /* NV_FuelDist distance calculations */
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

    //PIN_TESTPAD11_TOGGLE;                                 /* toggle port pin (debug only, same as WheelSensor_ISR()!!! ) */

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
 *  FUNCTION:       MeasDrv_ClearWheelSamples
 *  DESCRIPTION:    resets sample buffer
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        sets samples to max period = slowest wheel speed
 *********************************************************************** */
void MeasDrv_ClearWheelSamples(void)
{
    dwWheelPeriod4   = UINT16_MAX * 8L; /* scaled */
    wWheelPeriodLast = UINT16_MAX;
}


/***********************************************************************
 *  FUNCTION:       MeasDrv_ClearRPMSamples
 *  DESCRIPTION:    resets sample buffer
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        sets samples to max period = slowest wheel speed
 *********************************************************************** */
void MeasDrv_ClearRPMSamples(void)
{
    wRPMPeriod8  = UINT16_MAX;
    wRPMPeriod32 = UINT16_MAX;
}



/***********************************************************************
 *  FUNCTION:       MeasDrv_GetWheelPeriod
 *  DESCRIPTION:    Interface to get an averaged wheel period time
 *  PARAMETER:      BOOL fCurrent  TRUE    returns current value
 *                                 FALSE   returns averaged value
 *  RETURN:         UINT16 wheel period time in 200 µsec/digit
 *  COMMENT:        -
 *********************************************************************** */
UINT16 MeasDrv_GetWheelPeriod(BOOL fCurrent)
{
    UINT16  RValue;

    /* vehicle is very slow? */
    if (fCurrent == TRUE)
    {
        RValue = wWheelPeriodLast;                      /* YES: return last sample un-averaged */
        INT_GLOB_DISABLE;                               /* disable ALL interrupts while changing value */
        dwWheelPeriod4 = ((UINT32)wWheelPeriodLast)<<3; /* make this value base for average in ISR too (scaled) */
        INT_GLOB_ENABLE;                                /* disable ALL interrupts while changing value */
    }
    else
    {
        INT_GLOB_DISABLE;                          /* disable ALL interrupts while changing value */
        RValue = (UINT16)((dwWheelPeriod4 + 4)>>3);/* NO:  return averaged value (scaled) */
        INT_GLOB_ENABLE;                           /* disable ALL interrupts while changing value */
    }
    /* just a security check to prevent 'division by zero' */
    if (RValue == 0)
        RValue = UINT16_MAX;

    return RValue;
}


/***********************************************************************
 *  FUNCTION:       MeasDrv_GetRPMPeriod
 *  DESCRIPTION:    Interface to get an averaged rpm period time
 *  PARAMETER:      BOOL fFast  TRUE    returns the faster changing value
 *                              FALSE   returns the the slower changing value
 *  RETURN:         UINT16 averaged rpm period time in 10 µsec/digit
 *  COMMENT:        -
 *********************************************************************** */
UINT16 MeasDrv_GetRPMPeriod(BOOL fFast)
{
    UINT32  dwScratch = 0;
    UINT16  RValue;
    UINT8   bSample;

    /* which filter depth is requested? */
    if (fFast == TRUE)
    {
        RValue = wRPMPeriod8;           /* use less deep filtered value */
        INT_GLOB_DISABLE;               /* disable ALL interrupts while changing value */
        wRPMPeriod32 = wRPMPeriod8;     /* make this value base for average in ISR too */
        INT_GLOB_ENABLE;                /* enable interrupts  */
    }
    else
    {
        RValue = wRPMPeriod32;          /* use stronger low pass filter */
    }

    /* just a security check to prevent 'division by zero' */
    if (RValue == 0)
        RValue = UINT16_MAX;

    return RValue;
}


