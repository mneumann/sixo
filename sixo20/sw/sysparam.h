/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-31 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         System Parameter
 *  Purpose:        interface to read / save all parameters in
 *                  EEPROM or NVRAM
 *  Comments:       All stuff to reproduce SIxOs status after power up
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
 *      Arnold Neugebauer                   Ralf Krizsan
 *      Stöckener Str. 115                  Knickstr. 10
 *      30419 Hannover                      30890 Barsinghausen
 *      arnold.neugebauer@web.de            ralf.krizsan@web.de
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
 * Revision 2.5  2007/03/30 10:00:57  tuberkel
 * incr. to V2.4.5
 *
 * Revision 2.4  2007/03/26 23:13:33  tuberkel
 * changed MOTOBAU version handling
 * - eBikeType -> #define
 *
 * Revision 2.3  2007/03/09 08:38:36  niezeithat
 * - Added additional Motorbike Version, Africatwin RD07, no Fule Level Sensors!
 *
 * Revision 2.2  2006/10/01 22:18:00  tuberkel
 * just comment
 *
 * Revision 2.1  2006/07/20 23:06:03  tuberkel
 * - added F650 version
 * - incr. to V2.4.4
 *
 * Revision 2.0  2006/06/26 23:25:51  tuberkel
 * no message
 *
 * Revision 1.15  2006/03/18 08:59:22  Ralf
 * - TripCounter improved to 4 different counters
 *
 *
 ************************************************************************ */

#ifndef _SYSPARAM_H
#define _SYSPARAM_H



/* ================================================== */
/* SOFTWARE VERSION HANDLING */

/* software version number:
    - gives the opportunity to re-initialze eeprom/nvram after new software download
    - divided into two nibbles with main & less importent version part
    - f.e. version '4.6.3.a' = APL.SWV.BLD.SPC
    - maximum: '15.15.15.f'
    - defintion: */

/* software version number e.q. '2.1.1' */
#define DEF_SWID_APL   2  // APL = (0..15) mean application number (changed with new mean application features)
#define DEF_SWID_SWV   4  // SWV = (0..15) sw version number (changed with additional features)
#define DEF_SWID_BLD   5  // BLD = (0..15) build number (changed with bugfixes)


/* hardware specific version number */
// SPC = (0..15 eq. 'A'..'F') special version number (changed for special enabled/disabled features)
#define DEF_SWID_SPC   0    // 'A' reserved for base version


/* collect all SW+HW Ids to one WORD value */
#define DEF_SWID_NUMBER ((DEF_SWID_APL<<12) | (DEF_SWID_SWV<<8) | (DEF_SWID_BLD<<4) | (DEF_SWID_SPC))


/* SW version type */
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


/* ================================================== */
/* 'MAGIC NUMBER' HANDLING

    Note: This helps to distinguish between
                a) a never used hardware -> initialize complete eeprom/nvram
                b) a software update     -> initialize partial eeprom/nvram
          In case of b), vehicle unique all parameters (like e.g. wheelsize etc.)
          will not be re-initilized.

    Note:   Changing the 'magic number' results in a completely re-initialized eeprom,
            any application/user settings will be erased!
          */
#define EEPR_MAGICNUM_ADDR    2046        // address of magic number (last two bytes in eeprom)
#define EEPR_MAGICNUM_SIZE       2        // size of magic number (last two bytes in eeprom)
#define EEPR_MAGICNUM_VAL   0xfedc        // THE magic number (might be a very 'unusual' number ;-) )




/* common defines */
#define MAXPARAMSIZE      4     /* size of biggest possible system parameter in bytes */
#define PARAM_CYCLE     100     /* time period in [ms] to save one(!) system parameter */
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



/* tripcounter flags
    - used for permanent settings of tripcounter */
typedef union
{
    UINT8           byte;
    struct
    {
        unsigned char   LongDistUp:1;       /* LongDistance: 1=upside (like roadbook), 0=downside */
        unsigned char   reserved:7;         /* reserved */
    } flags;
} TRPCNTFL_TYPE;
#define DEF_TRPCNTFL (0x01 )  /* default: Long Distance up like in Roadbook */



/* bike type - used to indicate kind of special bike adaption */
typedef enum
{
    eBIKE_STANDARD,      // default bike version
    eBIKE_R100GS,        // BMW R100 GS version
    eBIKE_R1100GS,       // BMW R1100 GS version
    eBIKE_AFRICATWIN,    // Honda AfricaTwin version
    eBIKE_AFRICATWINRD07,// Honda AfricaTwin RD07 version
    eBIKE_BAGHIRA,       // MuZ Baghia version
    eBIKE_HUSQV,         // Husqvarna version
    eBIKE_HUSQVRS,       // Husqvarna RS version
    eBIKE_KTM,           // KTM version
    eBIKE_F650,          // BMW F650 version
    eBIKE_INVALID        // invalid bike type
} BIKE_TYPE;





/* parameter ID type for simple interface to requested eeprom / nvram value:
    - enum type, does not refer to value size or address!
    - up to 255 IDs possible
    - ID 0.. 127 is NVRAM reserved
    - ID 128..255 is EEPROM reserved

    NOTE:   Standard rule to define the parameters location:
            Always use NVRAM for any parameter that could be
            changed more than once per day !

    NOTE:   SIxO 2.0 Hardware supports up to
                -   56 bytes NVRAM  (see 'NVRAMSIZE')
                - 2048 bytes EEPROM (see 'wEepromSize') */

/* distinguish between NVRAM & EEPROM access */
#define PID_NVRAM_START     0
#define PID_EEPROM_START    127

typedef enum
{
    /* NVRAM area Parameter IDs */
    PID_VEHIC_KM = PID_NVRAM_START,         // vehicle overall distance km struct
    PID_HOURS_ALL,                          // engine running overall
    PID_HOURS_SERV,                         // engine running since last service
    PID_TRIP_A_KM,                          // TripA distance km struct (for trip counter)
    PID_TRIP_B_KM,                          // TripB distance km struct (for trip counter)
    PID_TRIP_C_KM,                          // TripC distance km struct (for common use)
    PID_TRIP_D_KM,                          // TripD distance km struct (for common use)
    PID_SPEED_AVR_M,                        // average speed of time in motion only (no pauses)
    PID_SPEED_AVR_P,                        // average speed of time with pauses
    PID_FUEL_KM,                            // distance since last refuel km struct

    PID_NVRAM_END,                          // INVALID PID to mark end!

    /* EEPROM area Parameter IDs */
    PID_SWVERSION = PID_EEPROM_START,       // software version number byte
    PID_WHEELSIZE,                          // wheel size
    PID_RPM_CCF,                            // rpm cylinder correcture factor
    PID_SYSFL,                              // system status (e.g. current device shown)
    PID_DPLFL,                              // bit flags for display status
    PID_DBGFILTFL,                          // bit flags for debug status
    PID_DBGDETDIRFL,                        // bit flags for debug details and direction
    PID_LOGO,                               // number of currently selected logo (enum)
    PID_TRIPCNTFL,                          // trip counter flags
    PID_SPEED_MAX,                          // max vehicle speed
    PID_RPM_MAX,                            // max engine speed
    PID_RPM_FLASH,                          // rpm to activate flash (rpm warning)
    PID_VOLT_MIN,                           // battery voltage minimum
    PID_VOLT_MAX,                           // battery voltage maximum
    PID_TAIR_MIN,                           // air temp minimum
    PID_TAIR_MAX,                           // air temp maximum
    PID_TOIL_MIN,                           // oil temp minimum
    PID_TOIL_MAX,                           // oil temp maximum
    PID_TWAT_MIN,                           // water temp minimum
    PID_TWAT_MAX,                           // water temp maximum
    PID_BIKETYPE,                           // definition of bike version (enum)
    PID_SERV_KM,                            // km, at which next service is required

    PID_LAPCSTAT,                           // lap counter status
    PID_LAPC_0,                             // lap timer #
    PID_LAPC_1,                             // lap timer #
    PID_LAPC_2,                             // lap timer #
    PID_LAPC_3,                             // lap timer #
    PID_LAPC_4,                             // lap timer #
    PID_LAPC_5,                             // lap timer #
    PID_LAPC_6,                             // lap timer #
    PID_LAPC_7,                             // lap timer #
    PID_LAPC_8,                             // lap timer #
    PID_LAPC_9,                             // lap timer #
    PID_LAPC_10,                            // lap timer #
    PID_LAPC_11,                            // lap timer #
    PID_LAPC_12,                            // lap timer #
    PID_LAPC_13,                            // lap timer #
    PID_LAPC_14,                            // lap timer #
    PID_LAPC_15,                            // lap timer #

    PID_EEPROM_END                          // INVALID PID to mark end!

} PARAM_ID_TYPE;



/* memory type defines for easier debugging */
typedef enum
{
    NVRAM,      // NVRAM  memory area, will be cleared with any software update
    EEPROM,     // EEPROM memory area, will be cleared with any software update
    NVRAM_SEC,  // NVRAM  memory area, will NOT be changed with any software update
    EEPROM_SEC  // EEPROM memory area, will NOT be changed with any software update
} MEM_TYPE;



/* non public element structure of system parameter control array */
typedef struct
{
    PARAM_ID_TYPE   eParamID;       // enum of parameter ID
    MEM_TYPE        eMem;           // location memory of system parameter: NVRAM / EEPROM
    UINT16          wStartAddr;     // start address in memory location type
    UINT8           bSize;          // number of byte allocated by system parameter
    void far *      fpData;         // adress of variable containing used data
    void far *      fpCompare;      // adress of variable used to detect changes
    void far *      fpDefault;      // adress of constant containing default value
} SYSPARINFO_TYPE;




/* public function prototypes */
ERRCODE ParReadSysParam(const PARAM_ID_TYPE eParamID, void far * fpParamBuffer);
ERRCODE ParWriteSysParam(const PARAM_ID_TYPE eParamID, void far * fpGivenParam);
ERRCODE ParInitSystemParam(void);
ERRCODE ParSetDefaults(BOOL fComplete);
ERRCODE ParCheckFirstInit(void);
ERRCODE ParCyclicSaveValues(void);
ERRCODE ParInitSystemPar(void);
BOOL    ParCheckMagicNumber(void);
void    ParDebugOutParameter( const PARAM_ID_TYPE PID );


/* private function prototypes */
//SYSPARINFO_TYPE far * ParGetSysParamInfo(const PARAM_ID_TYPE eSearchedID);



#endif // _SYSPARAM_H

 