/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-31 by Ralf Schwarzer
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
 * Revision 3.11  2012/05/27 16:01:42  tuberkel
 * All Eeprom/Nvram Variables renamed
 *
 * Revision 3.10  2012/05/15 20:11:35  tuberkel
 * FuelSensor: BasicSettings enabled & ok (not yet displayed)
 *
 * Revision 3.9  2012/02/27 23:01:54  tuberkel
 * - Eeprom layout changed ==> V3.0.5
 * - new: PID_COOLR_CNTRL, PID_COMPS_CNTRL, PID_LANGUAGE
 * - empty: PID_DEVFLAGS3
 * - CompassDrv corrected
 * - COOLRIDECNTRL_TYPE Bugfix (union)
 * - SysPar- API changed
 *
 * Revision 3.8  2012/02/27 20:46:50  tuberkel
 * - all Coolride GPIs/GPOs correctly set by Eeprom value
 *
 * Revision 3.7  2012/02/21 22:01:56  tuberkel
 * Compass Control/Mode/Eeprom reveiwed/fixed
 *
 * Revision 3.6  2012/02/14 21:08:03  tuberkel
 * - #define COMPASS ==> COMPDRV
 * - Compass SystemParam moved from devFlags2 -> 3
 * - Settings 'Compass' ==> 'Extensions'
 * - all Compass-Display modules enabled by default
 *
 * Revision 3.5  2012/02/11 12:21:45  tuberkel
 * dedicated COOLRIDE macros prepared & used
 *
 * Revision 3.4  2012/02/05 11:22:43  tuberkel
 * V3.0.3
 *
 * Revision 3.3  2012/01/23 05:47:23  tuberkel
 * Update V3.0.2- ReleaseCandidate
 *
 * Revision 3.2  2012/01/15 09:55:42  tuberkel
 * Version incr. to V3.0.1 - Release Candidate
 *
 * Revision 3.1  2010/11/07 14:06:16  tuberkel
 * V30 Preparations:
 * - SW Version # ==> V3.0.0
 *
 * Revision 3.0  2010/11/07 09:33:29  tuberkel
 * V30 Preparations:
 * - New System Parameters:
 *   - DayLightSavin, Beeper, VehSimulation, Hardcopy
 *   - WheelImpulses/Revolution
 *   - CompassDisplayMode
 *   - Language
 *   - Fuel Capacity/Consumption
 *   - IntroLogoDelay
 *
 * Revision 2.8  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.7  2009/06/21 21:19:55  tuberkel
 * V2.4.7
 * (former used as V2.5.0 at Ralley Dalmatia)
 *
 * Revision 2.6  2009/04/14 20:59:26  tuberkel
 * Changes done by Arnold:
 * - Vers# inr. 245 => 246
 * - EEPR_MAGICNUM_ADDR removed (dyn. detected)
 *
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
 * - NV_TripCom_Aounter improved to 4 different counters
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
#define DEF_SWID_APL   3  // APL = (0..15) main aplication number (changed with new main application features)
#define DEF_SWID_SWV   0  // SWV = (0..15) sw version number (changed with additional features)
#define DEF_SWID_BLD   6  // BLD = (0..15) build number (changed with bugfixes)


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

    Note: This helps to distinguish between necessary automatic eeprom handling at startup:

          a) EEPR_MAGICNUM_VAL not found!
                - a never used hardware
                - initialize COMPLETE eeprom/nvram
          b) EEPR_MAGICNUM_VAL ok, but Eeprom EE_SWID != DEF_SWID_NUMBER!
                - a software update is detected
                - initialize PARTIAL eeprom/nvram
                - NVRAM_SEC + EEPROM_SEC will NOT be changed!
          c) EEPR_MAGICNUM_VAL + DEF_SWID_NUMBER ok
                - normal start behavior
                - nothing to be changed

    Note:   Changing the 'magic number' results in a completely re-initialized eeprom,
            any application/user settings will be erased!     */
#define EEPR_MAGICNUM_SIZE       2        // size of magic number (last two bytes in eeprom)
#define EEPR_MAGICNUM_VAL   0xfedc        // THE magic number (might be a very 'unusual' number ;-) )



/* ----------------------------------------------------------------------------- */
/* common defines */
#define MAXPARAMSIZE      4     /* size of biggest possible system parameter in bytes */
#define PARAM_CYCLE      20     /* time period in [ms] to save one(!) system parameter */
#define COMPLETE        TRUE    /* for eeprom / vnram init */
#define PARTIAL         FALSE   /* for eeprom / vnram init */




/* ----------------------------------------------------------------------------- */
/* DEVICE STATUS FLAGS 1 - used to reconstruct system after power up  */
typedef union
{
    UINT8           byte;
    struct
    {
        unsigned char   ActDevNr    :4;     /* Active device nibble, saves last used device (valid: DEVID_MAIN..DEVID_SET) */
        unsigned char   MainDevState:4;     /* State of Main Device (scrollable sub area) */
    } flags;
} DEVFLAGS1_TYPE;
#define DEF_DEVFLAGS1 (0x10 | DEVID_MAIN )  /* default: set device# of 'MainDevice' and RPM in main device */


/* ----------------------------------------------------------------------------- */
/* DEVICE STATUS FLAGS 2 - used to reconstruct system after power up  */
typedef union
{
    UINT8           byte;
    struct
    {
        unsigned char   NV_TripCom_ALongDistUp :1;     /* NV_TripCom_AounterLongDist: 1=upside (like roadbook), 0=downside */
        unsigned char   BeeperAvail     :1;     /* Beeper:              1=available, 0=not available */
        unsigned char   DLS_Auto        :1;     /* DaylightSavingAuto:  1=on (automatic on), 0=Off */
        unsigned char   DLS_Active      :1;     /* DaylightSavingActive: 1=CEST active (CentralEurpSummerTime active), 0=Off */
        unsigned char   Metric          :1;     /* Metric:              0=km, 1=miles */
        unsigned char   LedWarnMode     :1;     /* LedWarningMode:      0=Sixo-like, 1=common (Warning lamps ON at ignition like in cars) */
        unsigned char   VehicSimul      :1;     /* vehicle Simulation:  1=on, 0=off */
        unsigned char   Hardcopy        :1;     /* Hardcopy via Uart:   1=available, 0=off (via Highbeam switch) */
    } flags;
} DEVFLAGS2_TYPE;
#define DEF_DEVFLAGS2 (0x03 )  /* default: LongDistUp=on, BeeperAvail=1, */


/* ----------------------------------------------------------------------------- */
/* DEVICE STATUS FLAGS 3 - used to reconstruct system after power up  */
typedef union
{
    UINT8           byte;
    struct
    {
        unsigned char   res8            :8;     /* reserved */
    } flags;
} DEVFLAGS3_TYPE;
#define DEF_DEVFLAGS3 (0x00 )  /* default: all off */


/* ----------------------------------------------------------------------------- */
/* BIKE TYPE - USED TO INDICATE KIND OF SPECIAL BIKE ADAPTION
   NOTE: This list must be identical to ressource text list RESTXT_SET_BIKE_xxx !*/
typedef enum
{
    eBIKE_STANDARD,      // default bike version
    eBIKE_R100GS,        // BMW R100 GS version
    eBIKE_AFRICATWIN,    // Honda AfricaTwin version
    eBIKE_BAGHIRA,       // MuZ Baghia version
    eBIKE_F650,          // BMW F650 version
    eBIKE_INVALID        // invalid bike type
} BIKE_TYPE;
/* NOTE: For bike-LOGO(!) - please use LOGO_TYPE instead of BIKE_TYPE! */


/* ----------------------------------------------------------------------------- */
/* COMPASSDRV SETUP FLAGS */
typedef union
{
    UINT8           byte;
    struct
    {
        unsigned char   CompassAvail:1;     /* Compass available: 1=driver active, 0: driver inactive */
        unsigned char   CompassDisplay:2;   /* Compass Display Mode (0=none/1=heading/2=graph/3=head+graph) */
        unsigned char   reserved:5;         /* reserved */
    } flags;
} COMPASSCNTL_TYPE;
#define DEF_COMPASSCNTFL (0x00 )/* default: complete off*/
#define COMPASS_DPL_OFF         0x00   /* no display (although module available) */
#define COMPASS_DPL_HEAD        0x01   /* show heading only  */
#define COMPASS_DPL_GRAPH       0x02   /* show bargraph only */
#define COMPASS_DPL_HEADGRAPH   0x03   /* show heading & bargraph only */


/* ----------------------------------------------------------------------------- */
/* COOLRIDE - HEATGRIP SETTINGS
    KEY-OUTPUT:
        - SIxO controls Coolride keyInput by shortening Coolride-Key-Input to SIxO GPO0-Gnd
        - Coolride needs at least Key-pressed 100 ms and 380 ms Intervall to next Press
        - we currently reserve GPO_0 as Coolride Control
        - we here adjust 100 ms GPO-ON-Time and 280 ms DURATION
    PWM-INPUT
        - SIxO measures Coolride Heatgrip PWM  via GPIx
        - Coolride uses ~720 ms PWM cycle, so we set timout to 1000 ms
        - Coolride generates PWM in 10% steps (ambient temperatur compensation)
        - but user ca setup PWM in 20% steps only
        - for display, SIXO uses 5x 10% bar */
typedef union
{
    UINT8   byte;   // for access via MACRO / bytewise
    struct
    {
        unsigned char   CoolrAvail  :1;     /* Coolride Available: 1=available, 0=n.a. */
        unsigned char   CoolrGPI    :2;     /* Coolride PWM-Measurement: GPI 0..3 */
        unsigned char   CoolrGPO    :1;     /* Coolride Key-Output: GPO 0..1 */
        unsigned char   Coolr_res4  :4;     /* reserved */
    } flags;
} COOLRIDECNTRL_TYPE;

#define COOLR_KEYOUT_SIGNAL     100, 0, 280 // GPO PWM signal for coolride key input
#define COOLR_PWMIN_LOGIC       TRUE        // GPI PWM logic: high active
#define COOLR_PWMIN_TO          1000        // Timeout in ms to detect missing transitions



/* ----------------------------------------------------------------------------- */
/* FUELSENSOR - INCREMENTAL
    
    PRINCIPLE
        - an external incremental sensor measures fuel volume passed by
        - fuel sensor delivers typical n Impulses/Liter
        - SIxO PWM-Input counts transitions low2high and increments impulses        
        - current impulscount is always saved in NVRAM area (fast changing)
        - impulses devided by Imp/Litre can be used to calculate the consumption
        - at refueling, the fuel distance counter and fuel sensor impulses counter are reseted

    COUNT-INPUT
        - SIxO measures Fuelsensor impulse via GPIx
        - typ. Fuelsenor impulsrate is ~20000 impulses/liter
        - typ. Fuelsenor impulsrate is therefor about ??? impulses/min */

typedef struct
{
    UINT16  FuelSImpulseRate;               /* Number of Impulses per Litre */
    struct
    {
        unsigned char   FuelSAvail  :1;     /* FuelSensor Available: 1=available, 0=n.a. */
        unsigned char   FuelSGPI    :2;     /* FuelSensor Counter-Measurement: GPI 0..3 */
        unsigned char   FuelS_res5  :5;     /* reserved */        
        unsigned char   FuelS_res8  :8;     /* reserved */        
    } flags;
} FUELSCNTRL_TYPE;

#define FUELS_PWMIN_LOGIC       TRUE        // GPI PWM logic: high active
#define FUELS_PWMIN_TO          1000        // Timeout in ms to detect missing transitions






/* ----------------------------------------------------------------------------- */
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
    PID_TRIP_A_KM,                          // NV_TripCnt_A distance km struct (for trip counter)
    PID_TRIP_B_KM,                          // NV_TripCnt_B distance km struct (for trip counter)
    PID_TRIP_C_KM,                          // NV_TripCom_A distance km struct (for common use)
    PID_TRIP_D_KM,                          // NV_TripCom_B distance km struct (for common use)
    PID_SPEED_AVR_M,                        // average speed of time in motion only (no pauses)
    PID_SPEED_AVR_P,                        // average speed of time with pauses
    PID_FUEL_KM,                            // distance since last refuel (km struct)
    PID_FUEL_SENSOR,                        // number of fuel sensor impulses since last refuel

    PID_NVRAM_END,                          // INVALID PID to mark end!

    /* EEPROM area Parameter IDs */
    PID_SWVERSION = PID_EEPROM_START,       // software version number byte
    PID_WHEELSIZE,                          // wheel size
    PID_CCF,                            // rpm cylinder correcture factor
    PID_DEVFLAGS1,                          // device flags 1 (e.g. current device shown)
    PID_DPLFL,                              // bit flags for display status
    PID_DBGFILTFL,                          // bit flags for debug status
    PID_DBGDETDIRFL,                        // bit flags for debug details and direction
    PID_LOGO,                               // number of currently selected logo (enum)
    PID_DEVFLAGS2,                          // device flags 1 (e.g. trip counter flags and show flags)
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
    PID_FUEL_CAP,                           // fuel reservoir
    PID_FUEL_CONS,                          // fuel consumption in l/100 km
    PID_BOOT_DELAY,                         // start screen dealy in 1/10 s
    PID_WHEEL_IMP,                          // number of impulses / wheel rotation
    PID_DEVFLAGS3,                          // device flags 3 (e.g. metric)
    PID_COOLR_CNTRL,                        // Coolride Heatgrip Control
    PID_COMPS_CNTRL,                        // Microcopter Compass Control
    PID_LANGUAGE,                           // SIxO user Language
    PID_FUELS_CNTRL,                        // FuelSensor Control

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
    NVRAM,      // NVRAM , automatically reseted to default by software update!
    EEPROM,     // EEPROM, automatically reseted to default by software update!
    NVRAM_SEC,  // NVRAM , not changed by software update
    EEPROM_SEC  // EEPROM, not changed by software update
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
ERRCODE SysPar_ReadSysParam     ( const PARAM_ID_TYPE eParamID, void far * fpParamBuffer );
ERRCODE SysPar_WriteSysParam    ( const PARAM_ID_TYPE eParamID, void far * fpGivenParam );
ERRCODE ParInitSystemParam  ( void );
ERRCODE SysPar_SetDefaults      ( BOOL fComplete );
ERRCODE SysPar_CheckFirstInit   ( void );
ERRCODE SysPar_CyclicSaveValues ( void );
ERRCODE SysPar_InitSystemPar    ( void );
BOOL    SysPar_CheckMagicNumber ( void );
void    SysPar_DebugOutParameter( const PARAM_ID_TYPE PID );
void    SysPar_SetupSWVersionStr( void );

/* private function prototypes */
//SYSPARINFO_TYPE far * SysPar_GetSysParamInfo(const PARAM_ID_TYPE eSearchedID);



#endif // _SYSPARAM_H

 