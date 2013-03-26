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
 *  Purpose:        simulate vehicle action api export header
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
 * Revision 3.6  2013/03/26 21:10:29  tuberkel
 * Simulation-Handling simplified
 *
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
 * Revision 3.1  2012/05/24 20:00:14  tuberkel
 * BMP renamed
 *
 * Revision 3.0  2010/11/07 12:28:57  tuberkel
 * V30 Preparations - no changes
 *
 * Revision 2.3  2010/11/07 11:58:46  tuberkel
 * V30 Preparations:
 * - Module 'Vehicle Simulation' ALWAYS supported
 * - may be activated by user settings
 *
 * Revision 2.2  2009/07/15 09:01:48  tuberkel
 * D-Options reviewed
 *
 * Revision 2.1  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.4  2006/02/18 15:10:22  Ralf
 * - only some additional defines
 *
 *
 ************************************************************************ */


#ifndef _SIMULATOR_H
#define _SIMULATOR_H




/* simulation mode */
#define SIM_SEQUENCE    TRUE    // TRUE enables simulation sequence, FALSE switches to static mode


/* enable simulation kind */
#define SIM_WHEEL_ON    TRUE    // TRUE enables this kind, FALSE disables
#define SIM_RPM_ON      TRUE
#define SIM_FUEL_ON     TRUE
#define SIM_COOLR_ON    FALSE


/* acoustic feedback - enable a 'click' for each simulated impuls */
#define SIM_CLICK_WHEEL     0
#define SIM_CLICK_RPM       0
#define SIM_CLICK_FUEL      0
#define SIM_CLICK_COOLR     0


/* start delay and step interval */
#define SIM_STARTDELAY     2    // start delay in sec
#define SIM_SEQSTEPINTV 1000    // intervall time in ms for one sequence step


/* scaling for 'SimPattern' values */
#define SIM_SCALE_WHEEL  7200   // to convert km/h -> WheelPeriod-IRQ (at wheelsize 2000 mm)
#define SIM_SCALE_RPM   26000   // to convert RPM  -> RPM-Period-IRQ (at CCF 1/1)
#define SIM_SCALE_FUEL   2500   // to convert l/1000 km -> Fuel-Period-IRQ (at 8500 I/L)
#define SIM_SCALE_COOLR   100   // to convert % heat -> Coolride-Period-IRQ


/* Kind of simulation */
typedef enum
{   SIM_WHEEL,      /* index of wheelspeed simulation */
    SIM_RPM,        /* index of engine RPM simulation */
    SIM_FUEL,       /* index of Fuelsensor simulation */
    SIM_COOLR,      /* index of Coolride simulation */
    SIM_KIND_MAX    /* invalid max index  */
} SIM_KIND;


/* ISR routine function prototype */
typedef void (*SIM_ISR_FCT)(void);


/* simulation sequence control data*/
typedef struct
{
    BOOL        fInit;              /* 1 = initialized */
    BOOL        fSeqMode;           /* 1 = sequence mode (else: static mode) */
    UINT8       bSeqStepNr;         /* current sequence step nr */
    UINT16      wSeqStepDur;        /* current sequence step time in sec. (counting down) */
    UINT16      wSeqLastCheck;      /* last call time stamp in sec. */
} SIM_CNTRL;


/* simulation kind control data*/
typedef struct
{
    SIM_KIND    eKind;              /* indicator for Kind type */
    BOOL        fActive;            /* 1 = simulation active  */
    BOOL        fClick;             /* 1 = beeper 'click' for each ISR-call */
    UINT16      wScaling;           /* scaling factor to convert Freq => Period */
    INT16       iDuration;          /* step duration in msec */

    UINT16      wLastFreqUpdate;    /* timestamp of last IRQ-Period-Update (in ms) */
    UINT16      wLastISRCall;       /* timestamp of last IRQ-Call (in ms) */
    UINT16      wISRPeriodCurr;     /* current period for IRQ-Call (in ms) */

    UINT16      wFreqStart;         /* IRQ-Call-Frequency at step begin (in Hz) */
    UINT16      wFreqEnd;           /* IRQ-Call-Frequency at step end (in Hz)*/
    INT16       iFreqDiff;          /* IRQ-Call-Frequency range from start->end (in Hz) */

    UINT16      wFreqCurr;          /* IRQ-Call-Frequency currently used (in Hz)*/
    INT16       iFreqStep;          /* signed(!) IRQ-Call-Frequency stepsize to sweep from start to end (in Hz) */
} SIM_KIND_CNTRL;



/* kind simulation setp start/end value  */
typedef struct
{
    INT16   iStart;    /* start value */
    INT16   iEnd;      /* end value */
} SIM_STEPVAL;


/* simulation control array entry */
typedef struct
{
    INT8         iDuration;              /* step duration in sec. */
    SIM_STEPVAL  StepVal[SIM_KIND_MAX];  /* pair of start/end value for each kind */
} SIM_SEQ_STEP;



/* public prototypes */
void Sim_Main               (void);
void Sim_Init               (BOOL fSequence);
void Sim_KindSetup          (SIM_KIND_CNTRL * pKind, SIM_KIND eKind, BOOL fActive, UINT16 wScaling, BOOL fClick);
void Sim_FrequenceSetup     (SIM_KIND_CNTRL * pKind, INT16 iFreqStart, INT16 iFreqEnd, INT16 iDuration);
void Sim_FrequenceControl   (SIM_KIND_CNTRL * pKind);
void Sim_Click              (BOOL fClick);

 #endif // _SIMULATOR_H


 