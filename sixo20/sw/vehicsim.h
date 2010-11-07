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



/* defines */
#define SIM_STARTDELAY  2       // start delay in sec
#define SIM_SEQUENCE    TRUE    // to control simulation: enables handling of sequence
#define SIM_STATIC      FALSE   // to control simulation: enables handling of static behavoiur

/* software interrupt declaration */
/* (this is necessary to cause an interrupt by software) */
#pragma INTCALL 29 WheelSensor_ISR();
#pragma INTCALL 30 RPMSensor_ISR();


/* weel simulator types */
typedef enum        /* simulator state */
{
    WSIM_CONST,         /* simulator uses constant speed */
    WSIM_ACC            /* simulator calculates accelerated speed */
} PSIM_STATE;


typedef struct      /* simualtor control data*/
{
    BOOL        fActive;            /* switch on/off */
    PSIM_STATE  eMode;              /* constant / accelerated mode */
    UINT8       bTransition_sec;    /* time to come from start period to end period */
    INT16       iPeriodStepSize_ms; /* signed stepsize to inkrement wheel period from start to end */
    UINT16      wStartPeriod_ms;    /* wheel period at simulation begin */
    UINT16      wEndPeriod_ms;      /* wheel period at simulation end */
    UINT16      wCurrentPeriod_ms;  /* current simulation wheel period */

} PSIM_TYPE;



/* macros to convert km/h and U/Min into ISR period in ms */
/* beide Werte weichen um Faktor 1/1.3 vom eigentlich richtigen Wert ab, warum? */
#define KMH2PERIOD(kmh) ((UINT16)(9400/kmh))        // empirisch, eigentlich 7200 bei 2000mm Radumfang,
#define RPM2PERIOD(rpm) ((UINT16)(7800/(rpm/10)))   // empirisch, eigentlich 60000


/* simulation array type */
typedef struct
{
    UINT16  wSpeedStart;    // start in km/h
    UINT16  wSpeedEnd;      // end in km/h
    UINT16  wRPMStart;      // start in RPM
    UINT16  wRPMEnd;        // end in RPM
    UINT8   bTransition;    // transition time from start to end value in sec.
} SIMULCNTR;



/* public prototypes */
void SimWheelSpeedActive(BOOL fActive);
void SimWheelSpeedSet( UINT16 wPeriod1, UINT16 wPeriod2, UINT8 bTransTime);
void SimWheelSpeedControl(void);
void SimWheelSpeedSequence(void);


void SimRPMSpeedActive(BOOL fActive);
void SimRPMSpeedSet( UINT16 wPeriod1, UINT16 wPeriod2, UINT8 bTransTime);
void SimRPMSpeedControl(void);
void SimRPMSpeedSequence(void);

void SimVehicSimulation(BOOL fSequence);


 #endif // _SIMULATOR_H


 