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
 *  Purpose:        Low level routines for all measurements,
 *                  calculations, filters
 *
 *  Comments:       -
 *
 ************************************************************************ */
#ifndef _MEASDRV_H
#define _MEASDRV_H



/* defines */
#define WHEEL_MAXVALUE           10000      /* max counter (=reload) value = equals 2 seconds */
#define RPM_MAXVALUE             50000      /* max counter (=reload) value = equals 0,5 seconds */


/* sfr62.h redefines */
#define WheelPortD      pd8_2               /* INT0 WHEEL impuls contact direction bit */
#define WheelPort       p8_2                /* INT0 WHEEL impuls contact bit */
#define RPMPortD        pd8_3               /* INT1 RPM impuls contact direction bit */
#define RPMPort         p8_3                /* INT1 RPM impuls contact bit */

#define fWheelCnt       ta2s                /* timer TA2 start/stop flag */
#define wWheelCnt       ta2                 /* timer TA2 register value */
#define fRPMCnt         ta3s                /* timer TA3 start/stop flag */
#define wRPMCnt         ta3                 /* timer TA3 register value */




/* public function prototypes */
ERRCODE MeasDrvInit(void);
ERRCODE MeasDrvInitWheel(void);
ERRCODE MeasDrvInitRPM(void);
UINT16 MeasDrvGetWheelPeriod(BOOL fCurrent);
UINT16 MeasDrvGetRPMPeriod(BOOL fFast);
void WheelSensor_ISR(void);
void RPMSensor_ISR(void);

#endif /* _MEASDRV_H */


