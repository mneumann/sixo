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
 *                  Note1:  System parameter in this context are all
 *                          device parameters saved in eeprom or nvram
 *                          to reproduce device status after power up
 *                          cycle and to save vehicle specific data.
 *
 *                  Note2:  There is no central init function to read back
 *                          ALL eeprom/nvram paramters after power up.
 *                          Each module has to do this for it's own!
 *
 ************************************************************************ */

/* includes */
#include <string.h>
#include "standard.h"
#include "nvramdrv.h"
#include "eeprdrv.h"
#include "sysparam.h"
#include "measure.h"
#include "debug.h"
#include "timer.h"
#include "display.h"
#include "device.h"



/* external symbols */
extern UINT16           wMilliSecCounter;       /* valid values: 0h .. ffffh */
extern far STRING       szDevName[];            /* debug out stuff */
extern  CCF_TYPE        CCF;                    /* RPM cylinder correcture factor */
extern DIST_TYPE        VehicDist;
extern DIST_TYPE        TripA;
extern DIST_TYPE        TripB;
extern DIST_TYPE        FuelDist;
extern SPEED_TYPE       SpeedMax;
extern SPEED_TYPE       SpeedAvrM;
extern SPEED_TYPE       SpeedAvrP;
extern UINT16           wWheelSize;             /* original wheel size from eeprom in mm */
extern DPLFLAGS_TYPE    gDisplayFlags;          /* original display flags */
extern DBGFILT_TYPE     gDebugFilter;           /* debug filter */
extern DBGDETDIR_TYPE   gDebugDetails;          /* debug details */


/* global symbols */
SYSFLAGS_TYPE    gSystemFlags = 0x0;     /* system parameters */
SWVERS_TYPE      gSWID        = 0x0000;  /* software id */



/* system parameter control structure
    (complete NVRAM/EEPROM memory mapping) */
const SYSPARINFO_TYPE  rgSysParControl[] =
{
    /*  NVRAM:
        Param ID            MemType     Start    Size of type */
    {   PID_VEHIC_KM,       NVRAM,      0x00,    sizeof(DIST_TYPE)  },      // 4 bytes, verhicle overall distance
    {   PID_TRIPA_KM,       NVRAM,      0x04,    sizeof(DIST_TYPE)  },      // 4 bytes, TripCounter A distance
    {   PID_TRIPB_KM,       NVRAM,      0x08,    sizeof(DIST_TYPE)  },      // 4 bytes, TripCounter B distance
    {   PID_SPEED_MAX,      NVRAM,      0x0c,    sizeof(SPEED_TYPE) },      // 2 bytes, maximum speed
    {   PID_SPEED_AVR_M,    NVRAM,      0x0e,    sizeof(SPEED_TYPE) },      // 2 bytes, average speed EX-cluding pause times
    {   PID_SPEED_AVR_P,    NVRAM,      0x10,    sizeof(SPEED_TYPE) },      // 2 bytes, average speed IN-cluding pause times
    {   PID_FUEL_KM,        NVRAM,      0x12,    sizeof(DIST_TYPE)  },      // 4 bytes, distance since last refuel

    /*  EEPROM:
        Param ID            MemType     Start    Size of type */
    {   PID_SWVERSION,      EEPROM,     0x00,    sizeof(UINT16)         },  // 2 bytes, 4 software version nibbles
    {   PID_WHEELSIZE,      EEPROM,     0x02,    sizeof(UINT16)         },  // 2 bytes, wheel circumcistence in mm
    {   PID_RPM_CCF,        EEPROM,     0x04,    sizeof(CCF_TYPE)       },  // 1 byte,  two cylinder correcture factor nibbles
    {   PID_SYSFL,          EEPROM,     0x05,    sizeof(SYSFLAGS_TYPE)  },  // 1 bytes, bitfield for system status
    {   PID_DPLFL,          EEPROM,     0x06,    sizeof(DPLFLAGS_TYPE)  },  // 2 byte,  bitfield for display control
    {   PID_DBGFILTFL,      EEPROM,     0x08,    sizeof(DBGFILT_TYPE)   },  // 1 byte,  bitfield for debug filters
    {   PID_DBGDETDIRFL,    EEPROM,     0x09,    sizeof(DBGDETDIR_TYPE) },  // 1 byte,  bitfield for dbug detaills & direction
};


/* number of system parameters in control structure array */
#define NROFSYSPAR  (sizeof(rgSysParControl)/sizeof(SYSPARINFO_TYPE))



/***********************************************************************
 *  FUNCTION:       ParGetSysParamInfo
 *  DESCRIPTION:    returns a pointer to the requested system parameter
 *                  info structure (not the sys parameter itself!)
 *  PARAMETER:      PARAM_ID_TYPE eSearchedID   searched parameter ID
 *  RETURN:         address of requested system parameter info structure
 *                  NULL if not found!
 *  COMMENT:        to be used before reading / writing a system parameter
 *********************************************************************** */
SYSPARINFO_TYPE far * ParGetSysParamInfo(const PARAM_ID_TYPE eSearchedID)
{
    UINT8 bIndex;
    for (bIndex = 0; bIndex < NROFSYSPAR; bIndex++)
    {
        if (rgSysParControl[bIndex].eParamID == eSearchedID)
        {
            return &rgSysParControl[bIndex];
        }
    }
    ODS1(DBG_SYS,DBG_ERROR,"Unknown SystemParameter PID: %u", eSearchedID);
    return NULL;
}



/***********************************************************************
 *  FUNCTION:       ParReadSysParam
 *  DESCRIPTION:    Reads the requested system variable from EEPROM or NVRAM
 *                  and fills the given buffer
 *  PARAMETER:      PARAM_ID_TYPE   eParamID   enum type of requested system parameter
 *                  void far *      pointer to prepared return buffer
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ParReadSysParam(const PARAM_ID_TYPE eParamID, void far * fpParamBuffer)
{
    UINT8                   LocalBuffer[MAXPARAMSIZE];
    SYSPARINFO_TYPE far *   fpParamInfo;
    ERRCODE                 RValue;

    /* parameter check: */
    fpParamInfo = ParGetSysParamInfo(eParamID);     /* parameter ID exists? */
    if (  (fpParamInfo == NULL  )
        ||(fpParamBuffer == NULL) )
    {
        return ERR_PARAM_ERR;
    }

    /* memory type choice */
    if (fpParamInfo->eMem == NVRAM)
    {
        /* read from NVRAM */
        RValue = iicNvramRead( fpParamInfo->wStartAddr, fpParamInfo->bSize, LocalBuffer );
    }
    else
    {
        /* read from EEPROM */
        RValue = iicEepromRead( fpParamInfo->wStartAddr, fpParamInfo->bSize, LocalBuffer );
    }

    /* make a local copy to prevent near/far ptr compiler errors */
    if (RValue != ERR_OK)
        ODS1(DBG_SYS,DBG_ERROR,"Can not read system parameter PID %u!", eParamID);
    else
        memcpy(fpParamBuffer, LocalBuffer, fpParamInfo->bSize);
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       ParWriteSysParam
 *  DESCRIPTION:    Writes the given system variable into EEPROM or NVRAM
 *  PARAMETER:      PARAM_ID_TYPE   eParamID   enum type of given system parameter
 *                  void far *      pointer to given parameter
 *  RETURN:         ERRCODE         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ParWriteSysParam(const PARAM_ID_TYPE eParamID, void far * fpGivenParam)
{
    UINT8                 LocalBuffer[MAXPARAMSIZE];
    SYSPARINFO_TYPE far * fpParamInfo;
    ERRCODE               RValue;

    /* parameter check: */
    fpParamInfo = ParGetSysParamInfo(eParamID);     /* parameter ID exists? */
    if (  (fpParamInfo == NULL )
        ||(fpGivenParam == NULL) )
    {
        return ERR_PARAM_ERR;
    }

    /* make a local copy to prevent near/far ptr compiler errors */
    memcpy(LocalBuffer, fpGivenParam, fpParamInfo->bSize);

    /* memory type choice */
    if (fpParamInfo->eMem == NVRAM)
    {
        /* write into NVRAM */
        RValue = iicNvramWrite( fpParamInfo->wStartAddr, fpParamInfo->bSize, LocalBuffer );
    }
    else
    {
        /* write into EEPROM */
        RValue = iicEepromWrite( fpParamInfo->wStartAddr, fpParamInfo->bSize, LocalBuffer );
    }

    /* check for errors */
    if (RValue != ERR_OK)
        ODS1(DBG_SYS,DBG_ERROR,"Can not save system parameter PID %u!", eParamID);
    return RValue;
}









/***********************************************************************
 *  FUNCTION:       ParSetDefaults
 *  DESCRIPTION:    Re-Write all nvram/eeprom parameters with default
 *                  data.
 *  PARAMETER:      BOOL fComplete  TRUE overwrites WHEELSIZE
                                    & CCF & VehicDist  too!
 *  RETURN:         error code
 *  COMMENT:        - To be used after first power up or while
 *                    reseting to 'factory defaults'
 *                  - Manual reset eeprom/nvram by setting
 *                    '0xffff' into sw id!
 *
 *********************************************************************** */
ERRCODE ParSetDefaults(BOOL fComplete)
{
    ERRCODE         RValue      = ERR_OK;

    /* EEPROM values */
    SWVERS_TYPE     SWID        = DEF_SWID_NUMBER;
    CCF_TYPE        CCF         = DEF_CCF_NOM | DEF_CCF_DENOM;
    SYSFLAGS_TYPE   SysFlags    = DEF_SYSFLAGS;
    DPLFLAGS_TYPE   DplFlags    = DEF_DPLFLAGS;
    DBGFILT_TYPE    DbgFilt     = DEF_DBGFILT;
    DBGDETDIR_TYPE  DbgDetDir   = DEF_DBGDETDIR;

    /* NVRAM values */
    DIST_TYPE       VehicDist   = 0x0L;
    DIST_TYPE       TripA       = 0x0L;
    DIST_TYPE       TripB       = 0x0L;
    SPEED_TYPE      SpeedMax    = 0x0;
    SPEED_TYPE      SpeedAvrM   = 0x0;
    SPEED_TYPE      SpeedAvrP   = 0x0;
    DIST_TYPE       FuelDist    = 0x0;

    /* normally unchanged vehicle specific stuff */
    if (fComplete == TRUE)
    {
        ODS1(DBG_SYS,DBG_INFO,"- WheelSize %u mm", wWheelSize);
        RValue = ParWriteSysParam( PID_WHEELSIZE, &wWheelSize);
        if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;

        ODS2(DBG_SYS,DBG_INFO,"- CylinderCorrectureFactor: %u/%u", CCF.nibble.nom, CCF.nibble.denom);
        RValue = ParWriteSysParam( PID_RPM_CCF, &CCF);
        if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;

        ODS(DBG_SYS,DBG_INFO,"- Vehicle Distance: 0 km");
        RValue = ParWriteSysParam( PID_VEHIC_KM,  &VehicDist);
        if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;
    }

    /* normal eeprom stuff to be reseted */
    ODS4(DBG_SYS,DBG_INFO,"- Software Version ID: %u.%u.%u.%c",
            SWID.Fields.apl, SWID.Fields.swv, SWID.Fields.bld, SWID.Fields.spc+APLNUMCOFFS);
    RValue = ParWriteSysParam( PID_SWVERSION,  &SWID);
    if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;

    ODS2(DBG_SYS,DBG_INFO,"- System Flags: ActiveDevice:%s MainDevState:%u", szDevName[SysFlags.flags.ActDevNr], SysFlags.flags.MainDevState);
    RValue = ParWriteSysParam( PID_SYSFL, &SysFlags);
    if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;

    ODS4(DBG_SYS,DBG_INFO,"- Display Flags: C:%u BL:%u BLL:%u Inv:%u",
            DplFlags.flags.ContrLev, DplFlags.flags.fBackl, DplFlags.flags.BacklLev, DplFlags.flags.fDplInvers);
    RValue = ParWriteSysParam( PID_DPLFL, &DplFlags);
    if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;

    ODS1(DBG_SYS,DBG_INFO,"- Debug Filter & Modules: 0x%x", DbgFilt);
    RValue = ParWriteSysParam( PID_DBGFILTFL, &DbgFilt);
    if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;

    ODS1(DBG_SYS,DBG_INFO,"- Debug Details & Direction: 0x%x", DbgDetDir);
    RValue = ParWriteSysParam( PID_DBGDETDIRFL, &DbgDetDir);
    if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;

    /* normal nvram stuff to be reseted */
    ODS(DBG_SYS,DBG_INFO,"- TripCounterA: 0 km");
    RValue = ParWriteSysParam( PID_TRIPA_KM, &TripA);
    if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;

    ODS(DBG_SYS,DBG_INFO,"- TripCounterB: 0 km");
    RValue = ParWriteSysParam( PID_TRIPB_KM, &TripB);
    if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;

    ODS(DBG_SYS,DBG_INFO,"- MaxSpeed: 0 km/h");
    RValue = ParWriteSysParam( PID_SPEED_MAX, &SpeedMax);
    if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;

    ODS(DBG_SYS,DBG_INFO,"- AverageSpeed (Motion only): 0,0 km/h");
    RValue = ParWriteSysParam( PID_SPEED_AVR_M, &SpeedAvrM);
    if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;

    ODS(DBG_SYS,DBG_INFO,"- AverageSpeed (incl. Pauses): 0,0 km/h");
    RValue = ParWriteSysParam( PID_SPEED_AVR_P, &SpeedAvrP);
    if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;

    ODS(DBG_SYS,DBG_INFO,"- FuelDistance: 0 km");
    RValue = ParWriteSysParam( PID_FUEL_KM, &FuelDist);
    if (RValue != ERR_OK) goto LABEL_PARSETDEFAULTS;

LABEL_PARSETDEFAULTS:

    if (RValue != ERR_OK)
        ODS(DBG_SYS,DBG_ERROR,"Error while initializing Eeprom/Nvram!");
    return RValue;
}




/***********************************************************************
 *  FUNCTION:       ParCheckFirstInit
 *  DESCRIPTION:    Checks the eeprom software ID.
 *                  If its not up2date, it (completly) renews
 *                  eeprom/nvram memory content to defaults
 *                  download was done.
 *  PARAMETER:      -
 *  RETURN:         error code
 *  COMMENT:        setting 0xFFFF forces complete re-initialize of eeprom
 *********************************************************************** */
ERRCODE ParCheckFirstInit(void)
{
    ERRCODE     RValue  = ERR_OK;
    SWVERS_TYPE SWID    = 0x0;

    /* read eeprom saved software version number */
    RValue = ParReadSysParam(PID_SWVERSION, &SWID);
    /*ODS4(DBG_SYS,DBG_INFO,"EEPROM Software Version: %u.%u.%u.%c",
            SWID.Fields.apl, SWID.Fields.swv, SWID.Fields.bld, SWID.Fields.spc+APLNUMCOFFS); */


    /* First EEPROM use or user forced eeprom init?
       (indicated by 0xff in complete eeprom memory) */
    if (SWID.Number == 0xFFFF)
    {
        /* renew the COMPLETE nvram & eeprom */
        ODS(DBG_SYS,DBG_INFO,"First use of EEPROM/NVRAM: Complete Init!");
        RValue = ParSetDefaults(COMPLETE);
    }

    /* check software ID */
    else if (SWID.Number != DEF_SWID_NUMBER)
    {
        /*renew the nvram & eeprom */
        ODS(DBG_SYS,DBG_INFO,"New SW version: Renewing EEPROM/NVRAM!");
        RValue = ParSetDefaults(PARTIAL);
    }
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       ParCyclicSaveValues
 *  DESCRIPTION:    Cyclicly called routine to asure all necessary
 *                  measurement data to be saved in EEPROM
 *  PARAMETER:      -
 *  RETURN:         error code
 *  COMMENT:        - Does only write into eeprom if any changes made in value
 *                  - Reduces sytem load by only saving one value every 200 ms.
 *********************************************************************** */
ERRCODE ParCyclicSaveValues(void)
{
    static  BOOL            fInit       = FALSE;            /* to check for initialized static vars */
    static  PARAM_ID_TYPE   PID         = PID_EEPROM_START; /* loop variable over multiple call cycles */
    static  UINT16          wLastCall   = 0;                /* time stamp of last call */
            UINT16          wThisCall   = 0;                /* time stamp of this call */
            ERRCODE         RValue      = ERR_OK;           /* return value*/
    static  SWVERS_TYPE     SWIDCopy;                       /* local reference copy to detect change */
    static  UINT16          wWheelSizeCopy;                 /* local reference copy to detect change */
    static  CCF_TYPE        CCFCopy;                        /* local reference copy to detect change */
    static  SYSFLAGS_TYPE   SystemFlagsCopy;                /* local reference copy to detect change */
    static  DPLFLAGS_TYPE   DisplayFlagsCopy;               /* local reference copy to detect change */
    static  DBGFILT_TYPE    DebugFilterCopy;                /* local reference copy to detect change */
    static  DBGDETDIR_TYPE  DebugDetailsCopy;               /* local reference copy to detect change */

    /* first init local copies at first call */
    if (fInit == FALSE)
    {
        SWIDCopy            = gSWID;
        wWheelSizeCopy      = wWheelSize;
        CCFCopy             = CCF;
        SystemFlagsCopy     = gSystemFlags;
        DisplayFlagsCopy    = gDisplayFlags;
        DebugFilterCopy     = gDebugFilter;
        DebugDetailsCopy    = gDebugDetails;
        fInit = TRUE;
    }

    /* check: time to check one system parameter? */
    TimerGetSys_msec(wThisCall);
    if (wThisCall - wLastCall > EEPROM_CYCLE)
    {
        wLastCall = wThisCall;          /* update time stamp */
        switch (PID)                    /* choice of system parameter to be saved */
        {
            case PID_SWVERSION:
                if (SWIDCopy.Number != gSWID.Number)
                {
                    SWIDCopy.Number != gSWID.Number;
                    RValue = ParWriteSysParam( PID_SWVERSION, &gSWID);
                    if (RValue == ERR_OK)
                        ODS4(DBG_SYS,DBG_INFO,"EEPROM: SW-ID changed: %u.%u.%u.%c",
                                gSWID.Fields.apl, gSWID.Fields.swv, gSWID.Fields.bld, gSWID.Fields.spc+APLNUMCOFFS);
                    else goto LABEL_PARCYCSAVE;
                } break;
            case PID_WHEELSIZE:
                if (wWheelSizeCopy != wWheelSize)                               /* any changes made? */
                {
                    wWheelSizeCopy = wWheelSize;                                /* reset local copy */
                    RValue = ParWriteSysParam( PID_WHEELSIZE, &wWheelSize);     /* save new value */
                    if (RValue == ERR_OK)
                        ODS1(DBG_SYS,DBG_INFO,"EEPROM: WheelSize changed: %u mm" ,wWheelSize);
                    else goto LABEL_PARCYCSAVE;
                } break;
            case PID_RPM_CCF:
                if (CCFCopy.byte  != CCF.byte)
                {
                    CCFCopy = CCF;
                    RValue = ParWriteSysParam( PID_RPM_CCF, &CCF);
                    if (RValue == ERR_OK)
                        ODS2(DBG_SYS,DBG_INFO,"EEPROM: CCF changed: %u/%u", CCF.nibble.nom, CCF.nibble.denom);
                    else goto LABEL_PARCYCSAVE;
                 } break;
            case PID_SYSFL:
                if (gSystemFlags.byte != SystemFlagsCopy.byte)
                {
                    SystemFlagsCopy = gSystemFlags;
                    RValue = ParWriteSysParam( PID_SYSFL, &gSystemFlags);
                    if (RValue == ERR_OK)
                        ODS2(DBG_SYS,DBG_INFO,"EEPROM: SystemFlags changed: ActiveDevice:%s MainDevState:%u", szDevName[gSystemFlags.flags.ActDevNr], gSystemFlags.flags.MainDevState);
                    else goto LABEL_PARCYCSAVE;
                } break;
            case PID_DPLFL:
                if (gDisplayFlags.word != DisplayFlagsCopy.word)
                {
                    DisplayFlagsCopy = gDisplayFlags;
                    RValue = ParWriteSysParam( PID_DPLFL, &gDisplayFlags);
                    if (RValue == ERR_OK)
                        ODS4(DBG_SYS,DBG_INFO,"EEPROM: Display Flags changed: C:%u BL:%u BLL:%u Inv:%u",
                                                gDisplayFlags.flags.ContrLev, gDisplayFlags.flags.fBackl, gDisplayFlags.flags.BacklLev, gDisplayFlags.flags.fDplInvers);
                    else goto LABEL_PARCYCSAVE;
                } break;
            case PID_DBGFILTFL:
                if (DebugFilterCopy.byte != gDebugFilter.byte)
                {
                    DebugFilterCopy = gDebugFilter;
                    RValue = ParWriteSysParam( PID_DBGFILTFL, &gDebugFilter);
                    if (RValue == ERR_OK)
                        ODS1(DBG_SYS,DBG_INFO,"EEPROM: DebugFilter changed: 0x%x ", gDebugFilter);
                    else goto LABEL_PARCYCSAVE;
                } break;
            case PID_DBGDETDIRFL:
                if (DebugDetailsCopy.byte != gDebugDetails.byte)
                {
                    DebugDetailsCopy = gDebugDetails;
                    RValue = ParWriteSysParam( PID_DBGDETDIRFL, &gDebugDetails);
                    if (RValue == ERR_OK)
                        ODS1(DBG_SYS,DBG_INFO,"EEPROM: DebugDetails changed: 0x%x", gDebugDetails);
                    else goto LABEL_PARCYCSAVE;
                } break;
            default:
                ODS1(DBG_SYS,DBG_ERROR,"Unknown PID: %u", PID);
                break;
        }

        /* next system parameter for next call */
        PID++;                                          /* next PID */
        if (PID > PID_DBGDETDIRFL)
           PID = PID_EEPROM_START;                      /* start at the begining again */
    }

LABEL_PARCYCSAVE:

    if (RValue != ERR_OK)
        ODS1(DBG_SYS,DBG_ERROR,"Error saving PID:%u", PID);
    return RValue;

}



/***********************************************************************
 *  FUNCTION:       ParInitSystemPar
 *  DESCRIPTION:    Reads all system eeprom/nvram parameters to
 *                  initialize them as global vars
 *  PARAMETER:      -
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE ParInitSystemPar (void)
{
    ERRCODE         RValue;
    PARAM_ID_TYPE   PID = PID_NVRAM_START;

    /* --- eeprom part --------------------------------------------------- */
    ODS(DBG_SYS,DBG_INFO,"Reading system init values from EEPROM:");

    /* get sw version */
    RValue = ParReadSysParam(PID = PID_SWVERSION, &gSWID);
    if (RValue == ERR_OK)
        ODS4(DBG_SYS,DBG_INFO,"- Software Version ID: %u.%u.%u.%c",
                gSWID.Fields.apl, gSWID.Fields.swv, gSWID.Fields.bld, gSWID.Fields.spc+APLNUMCOFFS);
    else goto LABEL_PARSYSINIT;

    /* get vehicle wheel size */
    RValue = ParReadSysParam(PID = PID_WHEELSIZE, &wWheelSize);
    if (RValue == ERR_OK)
        ODS1(DBG_SYS,DBG_INFO,"- WheelSize: %u mm", wWheelSize);
    else goto LABEL_PARSYSINIT;

    /* get vehicle engine cylinder correcture factor */
    RValue = ParReadSysParam(PID = PID_RPM_CCF, &CCF);            /* read from eeprom */
    if (RValue == ERR_OK)
        ODS2(DBG_SYS,DBG_INFO,"- CCF: %u / %u", CCF.nibble.nom, CCF.nibble.denom);
    else goto LABEL_PARSYSINIT;

    /* get system flags */
    RValue = ParReadSysParam(PID = PID_SYSFL, &gSystemFlags);
    if (RValue == ERR_OK)
        ODS2(DBG_SYS,DBG_INFO,"- System Flags: ActiveDevice:%s MainDevState:%u", szDevName[gSystemFlags.flags.ActDevNr], gSystemFlags.flags.MainDevState);
    else goto LABEL_PARSYSINIT;

    /* get display flags */
    RValue = ParReadSysParam( PID = PID_DPLFL, &gDisplayFlags);
    if (RValue == ERR_OK)
        ODS4(DBG_SYS,DBG_INFO,"- Display Flags: C:%u BL:%u BLL:%u Inv:%u",
                gDisplayFlags.flags.ContrLev, gDisplayFlags.flags.fBackl, gDisplayFlags.flags.BacklLev, gDisplayFlags.flags.fDplInvers);
    else goto LABEL_PARSYSINIT;

    /* get debug filter flags */
    RValue = ParReadSysParam( PID = PID_DBGFILTFL, &gDebugFilter);
    if (RValue == ERR_OK)
        ODS1(DBG_SYS,DBG_INFO,"- Debug Filter & Modules: 0x%x", gDebugFilter);
    else goto LABEL_PARSYSINIT;

    /* get debug details flags */
    RValue = ParReadSysParam( PID = PID_DBGDETDIRFL, &gDebugDetails);
    if (RValue == ERR_OK)
        ODS1(DBG_SYS,DBG_INFO,"- Debug Details & Direction: 0x%x", gDebugDetails);
    else goto LABEL_PARSYSINIT;


    /* --- nvram part --------------------------------------------------- */
    ODS(DBG_SYS,DBG_INFO,"Reading measurement init values from NVRAM:");

    /* get vehicle distance */
    RValue = ParReadSysParam( PID = PID_VEHIC_KM, &VehicDist);
    if (RValue == ERR_OK)
        ODS2(DBG_SYS,DBG_INFO,"- VehicDist: %lu,%.2lu km", VehicDist.dkm/100L, VehicDist.dkm-(VehicDist.dkm/100L)*100L);
    else goto LABEL_PARSYSINIT;

    /* get TripCounterA distance */
    RValue = ParReadSysParam( PID = PID_TRIPA_KM, &TripA);
    if (RValue == ERR_OK)
         ODS2(DBG_SYS,DBG_INFO,"- TripA: %lu,%.2lu km", TripA.dkm/100L, TripA.dkm-(TripA.dkm/100L)*100L);
    else goto LABEL_PARSYSINIT;

    /* get TripCounterB distance */
    RValue = ParReadSysParam( PID = PID_TRIPB_KM, &TripB);
    if (RValue == ERR_OK)
         ODS2(DBG_SYS,DBG_INFO,"- TripB: %lu,%.2lu km", TripB.dkm/100L, TripB.dkm-(TripB.dkm/100L)*100L);
    else goto LABEL_PARSYSINIT;

    /* get Fuel Distance */
    RValue = ParReadSysParam( PID = PID_FUEL_KM, &FuelDist);
    if (RValue == ERR_OK)
         ODS2(DBG_SYS,DBG_INFO,"- FuelDist: %lu,%.2lu km", FuelDist.dkm/100L, FuelDist.dkm-(FuelDist.dkm/100L)*100L);
    else goto LABEL_PARSYSINIT;

    /* get Maximum Speed */
    RValue = ParReadSysParam( PID = PID_SPEED_MAX, &SpeedMax);
    if (RValue == ERR_OK)
         ODS2(DBG_SYS,DBG_INFO,"- SpeedMax: %u,%.2u km/h", SpeedMax/100, SpeedMax-(SpeedMax/100)*100);
    else goto LABEL_PARSYSINIT;

    /* get Average Speed (in motion only) */
    RValue = ParReadSysParam( PID = PID_SPEED_AVR_M, &SpeedAvrM);
    if (RValue == ERR_OK)
         ODS2(DBG_SYS,DBG_INFO,"- SpeedAvrM: %u,%.2u km/h", SpeedAvrM/100, SpeedAvrM-(SpeedAvrM/100)*100);
    else goto LABEL_PARSYSINIT;

    /* get Average Speed (incl pauses) */
    RValue = ParReadSysParam( PID = PID_SPEED_AVR_M, &SpeedAvrP);
    if (RValue == ERR_OK)
         ODS2(DBG_SYS,DBG_INFO,"- SpeedAvrP: %u,%.2u km/h", SpeedAvrP/100, SpeedAvrP-(SpeedAvrP/100)*100);
    else goto LABEL_PARSYSINIT;

LABEL_PARSYSINIT:

    if (RValue != ERR_OK)
        ODS1(DBG_SYS,DBG_ERROR,"Error reading PID:%u", PID);
    return RValue;
}

 