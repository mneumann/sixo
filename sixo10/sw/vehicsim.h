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
#ifndef _SIMULATOR_H
#define _SIMULATOR_H
 

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

/* simulator sequence control */
typedef enum
{
    SEQ0,
    SEQ1,
    SEQ2,
    SEQ3,
    SEQ4,
    SEQ5,
    SEQ6,
    SEQ8
} PSIM_SEQ;

    


/* mocro to convert km/h into m/s */
#define KMH2MS(kmh)     ((INIT8)(((((UINT16)kmh)*10)/36)/10))
 
/* public prototypes */
void SimWheelSpeedActive(BOOL fActive);
void SimWheelSpeedSet( UINT16 wPeriod1, UINT16 wPeriod2, UINT8 bTransTime);
void SimWheelSpeedControl(void);
void SimWheelSpeedSequence(void); 
 

void SimRPMSpeedActive(BOOL fActive);
void SimRPMSpeedSet( UINT16 wPeriod1, UINT16 wPeriod2, UINT8 bTransTime);
void SimRPMSpeedControl(void);
void SimRPMSpeedSequence(void); 

void SimVehicSimulation(void);

 
 #endif // _SIMULATOR_H
 
 
 