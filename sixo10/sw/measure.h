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
 *  Created:        26-03-2002 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         Measurement Module
 *  Purpose:        All high level measurement stuff for all sensors
 *  Comments:       -
 *
 ************************************************************************ */
#ifndef _MEASURE_H
#define _MEASURE_H


/* common defines */


/* default values to initialize eeprom/nvram */
#define DEF_WHEELSIZE    2100   /* wheel circumdistance in Millimeters */
#define DEF_CCF_NOM      0x01   /* low nibble: cylinder correktur factor nominator */
#define DEF_CCF_DENOM    0x10   /* high nibble cylinder correktur factor denominator */




/* measurement limits */
#define MM_OVFL           9999  /* 10 m overflow limit in mm */
#define SLOW_ENGSPEED    1500L  /* in RPM, limit to switch to higher filter depth */
#define SLOW_WHEELSPEED    15L  /* in km/h, limit under which unaveraged values are used */


/* full distance: */
typedef union
{
    UINT32  dkm;    /* dekameter,        10 m/digit, maximum:    42.946.972,95 km */
    UINT32  hm;     /* hektometer,      100 m/digit, maximum:    42.946.972,9  km */
    UINT32  km;     /* kilometer,      1000 m/digit, maximum:    42.946.972    km */
    UINT16  km_o;   /* kilometer only, 1000 m/digit, range:   0    .. 65536    km */
    UINT16  dkm_o;  /* dekameter only,   10 m/digit, range:   0,00 ..     0,99 km */
    UINT16  hkm_o;  /* hektometer only, 100 m/digit, range:   0,0  ..     0,9  km */
} DIST_TYPE;


/* speed strcuture:
    - f.e. used for maximum speed
    - resolution:    0,01 km/h
    - maximum:     655,35 km/h */
typedef UINT16 SPEED_TYPE;


/* cylinder correcture factor (CCF):
    - used to adapt ignition impulses to number of cylinders sharing one impulse
    - devided into two parts: nominator & denominator
    - see 'MeasGetEngineSpeed()' in measure.c for details */
typedef union
{
    UINT8 byte;
    struct
    {
        unsigned char   nom:4;      /* nominator:   # of ignitions per round, range 1..15 */
        unsigned char   denom:4;    /* denominator: # of cylinders sharing this ignition, range 1..15  */
    } nibble;
} CCF_TYPE;





/* enum type simply to define return value units of measurement function */
/* (used to get more after comma information) */
typedef enum
{
    MR_KM_PER_H,        /* result in 1 kilo-meter/hour per digit */
    MR_HM_PER_H,        /* result in 1 hecto-meter/hour per digit (= 10*km/h) */
    MR_M_PER_S,         /* result in 1 meter/second per digit */
    MR_DM_PER_S,        /* result in 1 deci-meter/second per digit (=10*m/s)*/
    MR_CM_PER_S,        /* result in 1 centi-meter/second per digit (=100*m/s)*/
    MR_RPS,             /* result in 1 RoundPerSecond per digit (= RPM/60) */
    MR_RPM,             /* result in 1 RoundPerMinute per digit */
    MR_RPM_R10,         /* result in 1 RoundPerMinute per digit rounded to ten's */
    MR_KM,              /* result in 1 kilometer per digit */
    MR_HM,              /* result in 1 hectometer (=0,1 km) per digit */
    MR_DKM,             /* result in 1 dekameter (=0,01 km) per digit */
    MR_KM_ONLY,         /* result in 1 kilometer per digit LIMITED to 65.536 km */
    MR_HM_ONLY,         /* result in 1 hectometer (=0,1 km) per digit WITHOUT leading km's */
    MR_DKM_ONLY         /* result in 1 dekameter (=0,01 km) per digit WITHOUT leading km's */
} MEASUNITS_TYPE;





/* non public prototypes */
ERRCODE GetFormatedDist( DIST_TYPE far * fpDistSrc, DIST_TYPE far * fpDistTrgt, MEASUNITS_TYPE RUnits);



/* get/set distance measuremant data*/
#define SET  TRUE
#define GET  FALSE


/* public prototypes */
ERRCODE MeasCyclicSaveValues(void);

UINT16  MeasGetWheelSpeed(MEASUNITS_TYPE eUnit);
UINT16  MeasGetEngineSpeed(MEASUNITS_TYPE eUnit);

DIST_TYPE MeasGetVehicDist( MEASUNITS_TYPE eUnits );
UINT16    MeasGetFuelDist( MEASUNITS_TYPE eUnits );
UINT16    MeasGetTripA( MEASUNITS_TYPE eUnits );
UINT16    MeasGetTripB( MEASUNITS_TYPE eUnits );

ERRCODE MeasGetRawFuelDist(DIST_TYPE far * fpDist);
ERRCODE MeasGetRawVehicDist(DIST_TYPE far * fpDist);
ERRCODE MeasGetRawTripA(DIST_TYPE far * fpDist);
ERRCODE MeasGetRawTripB(DIST_TYPE far * fpDist);

ERRCODE MeasSetFuelDist(DIST_TYPE far * fpDist);
ERRCODE MeasSetVehicDist(DIST_TYPE far * fpDist);
ERRCODE MeasSetTripA(DIST_TYPE far * fpDist);
ERRCODE MeasSetTripB(DIST_TYPE far * fpDist);




#endif // _MEASURE_H

