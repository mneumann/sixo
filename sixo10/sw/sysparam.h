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
 *  Created:        31-03-2002 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         System Parameter
 *  Purpose:        interface to read / save all parameters in
 *                  EEPROM or NVRAM
 *  Comments:       All stuff to reproduce SIxOs status after power up
 *
 ************************************************************************ */
#ifndef _SYSPARAM_H
#define _SYSPARAM_H


/* software version number */
#define DEF_SWID_APL   1  // APL = (0..15) mean application number (changed with new mean application features)
#define DEF_SWID_SWV   2  // SWV = (0..15) sw version number (changed with additional features)
#define DEF_SWID_BLD   1  // BLD = (0..15) build number (changed with bugfixes)


/* hw version number */
// SPC = (0..15 eq. 'A'..'F') special version number (changed for special enabled/disabled features)
#ifdef HW_OILGEN
    #define DEF_SWID_SPC   1    // 'B' reserved for BMW version with additional input lines
#else
    #define DEF_SWID_SPC   0    // 'A' reserved for base version
#endif

/* ================================================== */
/* SOFTWARE VERSION HANDLING */

/* software version number:
    - gives the opportunity to re-initialze eeprom/nvram after new software download
    - divided into two nibbles with main & less importent version part
    - f.e. version '4.6.3.a' = APL.SWV.BLD.SPC
    - maximum: '15.15.15.f'
    - defintion: */

#define DEF_SWID_NUMBER ((DEF_SWID_APL<<12) | (DEF_SWID_SWV<<8) | (DEF_SWID_BLD<<4) | (DEF_SWID_SPC))

typedef union TAG_SWVERS_TYPE
{
    UINT16 Number;                 /* sw version as one 16 bit value only */
    struct
    {
        unsigned char    spc:4;    // 0x...X SPC = special version number, ascii char (changed for special enabled/disabled features)
        unsigned char    bld:4;    // 0x..X. BLD = build number (changed with bugfixes)
        unsigned char    swv:4;    // 0x.X.. SWV = sw version number (changed with additional features)
        unsigned char    apl:4;    // 0xX... APL = mean application number (changed with new mean application features)
    } Fields;
} SWVERS_TYPE;

#define APLNUMCOFFS  65         // application number character offset, to display 0x0 as 'a', 0x1 as 'b' ...




/* common defines */
#define MAXPARAMSIZE      4     /* size of biggest possible system parameter in bytes */
#define NVRAM_CYCLE     200     /* time period in [ms] to save one(!) nvram system parameter */
#define EEPROM_CYCLE    200     /* time period in [ms] to save one(!) eeprom system parameter */
#define COMPLETE        TRUE    /* for eeprom / vnram init */
#define PARTIAL         FALSE   /* for eeprom / vnram init */




/* device status flags
    - used to reconstruct system after power up
    - handles all device specific static stuff */
typedef union
{
    UINT8           byte;
    struct
    {
        unsigned char   ActDevNr:4;         /* Active device nibble, saves last used device (valid: DEVID_MAIN..DEVID_SET) */
        unsigned char   MainDevState:4;     /* State of Main Device (scrollable sub area) */
    } flags;
} SYSFLAGS_TYPE;
#define DEF_SYSFLAGS (0x10 | DEVID_MAIN )  /* default: set device# of 'MainDevice' and RPM in main device */





/* distinguish between NVRAM & EEPROM access */
#define PID_NVRAM_START     0
#define PID_EEPROM_START    127


/* parameter ID type for simple interface to requested eeprom / nvram value:

    - enum type, does not refer to value size or address!
    - up to 255 IDs possible
    - ID 0.. 127 is NVRAM reserved
    - ID 128..255 is EEPROM reserved

    NOTE:   Standard rule to define the parameters location:
            Always use NVRAM for any parameter that could be
            changed more than once per day !*/
typedef enum
{
    /* NVRAM area */
    PID_VEHIC_KM = PID_NVRAM_START,         // vehicle overall distance km struct
    PID_TRIPA_KM,                           // TripA distance km struct
    PID_TRIPB_KM,                           // TripA distance km struct
    PID_FUEL_KM,                            // distance since last refuel km struct

    PID_SPEED_MAX,                          // max speed
    PID_SPEED_AVR_M,                        // average speed of time in motion only (no pauses)
    PID_SPEED_AVR_P,                        // average speed of time with pauses

    PID_NVRAM_END,                          // INVALID PID to mark end!

    /* EEPROM area */
    PID_SWVERSION = PID_EEPROM_START,       // software version number byte
    PID_WHEELSIZE,                          // wheel size
    PID_RPM_CCF,                            // rpm cylinder correcture factor
    PID_SYSFL,                              // system status (e.g. current device shown)
    PID_DPLFL,                              // bit flags for display status
    PID_DBGFILTFL,                          // bit flags for debug status
    PID_DBGDETDIRFL,                        // bit flags for debug details and direction

    PID_EEPROM_END                          // INVALID PID to mark end!
} PARAM_ID_TYPE;



/* memory type defines for easier debugging */
typedef enum
{
    NVRAM,
    EEPROM
} MEM_TYPE;



/* non public element structure of system parameter control array */
typedef struct
{
    PARAM_ID_TYPE   eParamID;       // enum of parameter ID
    MEM_TYPE        eMem;           // location memory of system parameter: NVRAM / EEPROM
    UINT16          wStartAddr;     // start address in memory location type
    UINT8           bSize;          // number of byte allocated by system parameter
} SYSPARINFO_TYPE;




/* public function prototypes */
ERRCODE ParReadSysParam(const PARAM_ID_TYPE eParamID, void far * fpParamBuffer);
ERRCODE ParWriteSysParam(const PARAM_ID_TYPE eParamID, void far * fpGivenParam);
ERRCODE ParInitSystemParam(void);
ERRCODE ParSetDefaults(BOOL fComplete);
ERRCODE ParCheckFirstInit(void);
ERRCODE ParCyclicSaveValues(void);
ERRCODE ParInitSystemPar(void);


/* private function prototypes */
//SYSPARINFO_TYPE far * ParGetSysParamInfo(const PARAM_ID_TYPE eSearchedID);



#endif // _SYSPARAM_H

 