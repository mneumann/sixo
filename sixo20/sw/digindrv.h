/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-09 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         Digital Input
 *  Purpose:        Digital Input api export header
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
 ************************************************************************ */
#ifndef _DIGINDRV_H
#define _DIGINDRV_H






/* keyboard ports */
/* port 2_6 and 2_7 reserved for output purpose */

#define KeyPortD_Up     pd2_0   /* DOWN Button */
#define KeyPortD_Down   pd2_1   /* UP Button */
#define KeyPortD_Ok     pd2_2   /* OK Button */

#define KeyPort_Up      p2_0    /* DOWN Button */
#define KeyPort_Down    p2_1    /* UP Button */
#define KeyPort_Ok      p2_2    /* OK Button */


/* hardware version number ports */
/* ATTENTION: p4 is used to control LCD module too! */
#define HWVERS_PINS     0x7     // 3 bit used to code hw version
#define HWVerPortD      pd4     // port direction registers
#define HWVerD0         pd4_0
#define HWVerD1         pd4_1
#define HWVerD2         pd4_2

#define HWVerPort       p4      // port direction registers
#define HWVer0          p4_0    // port register
#define HWVer1          p4_1
#define HWVer2          p4_2


/* serial port pins (for tests only) */
#define Uart0_TX        p6_3
#define Uart0D_TX       pd6_3
#define Uart0_RX        p6_2

#define Uart1_TX        p6_7
#define Uart1D_TX       pd6_7
#define Uart1_RX        p6_6


/* other digital in port register */
#define DIGIN_ALL_P2    0xf8    /* DigIn ports only */


/* common dedicated digital inputs (unfiltered values) */
#define DigIn_TurnL   p2_3
#define DigIn_TurnR   p2_4
#define DigIn_OilSw   p2_5
#define DigIn_Neutral p2_6
#define DigIn_HBeam   p2_7

/* GPIs - General Purpose Digital Inputs (unfiltered values) */
#define DigIn_GPI_0   p0_4      // also connected to p1_4 and p8_4 eq. INT2
#define DigIn_GPI_1   p0_5      // also connected to p1_5          eq. INT3
#define DigIn_GPI_2   p0_6      // also connected to p1_6          eq. INT4
#define DigIn_GPI_3   p0_7      // also connected to p1_7          eq. INT5

#define DigIn_GPID_0  pd0_4     // equivalent port direction registers set to 'input'
#define DigIn_GPID_1  pd0_5
#define DigIn_GPID_2  pd0_6
#define DigIn_GPID_3  pd0_7


/* GPI0..3 / INT2..5 interrupt time measurements (PulseWidth & Frequency) */
typedef struct
{
    UINT32  dwLHCounter;          // counts Low->High edges only!
    UINT16  wLastHLTrans;       // system time stamp of last High->Low edge (in ms)
    UINT16  wLastLHTrans;       // system time stamp of last Low->High edge (in ms)
    UINT16  wHighWidth;         // High-PulseWidth (in ms)
    UINT16  wLowWidth;          // Low-PulseWidth (in ms)
} DIGINTMEAS;

typedef enum                    // enumerate GPI0..3 Interrup Measurement
{   eGPI0_Int2,
    eGPI1_Int3,
    eGPI2_Int4,
    eGPI3_Int5,
    eGPI_MAX,                   // invalid
} DIGINTMEAS_GPI;


/* digital filter structure */
typedef struct
{
    INT16   swFiltValue;    // represents filter value 0..255 (0=LOW, 255=HIGH)
    UINT8   bIncrValue;     // increment step if input HIGH
    UINT8   bDecrValue;     // deccrement step if input LOW
    BOOL    fState;         // result of filter process
} DIGFILTTYPE;

/* enum to access array elements of digital filter structure via index */
typedef enum
{
    eDF_TURNL,      // filter element turn-left
    eDF_TURNR,      // filter element turn-right
    eDF_OILSW,      // filter element oil pressure switch
    eDF_NEUTR,      // filter element neutral-gear
    eDF_HBEAM,      // filter element high-beam
    eDF_GPI_0,      // filter element general purpose 0
    eDF_GPI_1,      // filter element general purpose 1
    eDF_GPI_2,      // filter element general purpose 2
    eDF_GPI_3,      // filter element general purpose 3
    eDF_LAST        // last - invalid element!
} DIGFILTELEMENT;


/* common access macros to access filtered digital inputs */
#define DF_TURNL    DigInFilter[eDF_TURNL].fState
#define DF_TURNR    DigInFilter[eDF_TURNR].fState
#define DF_OILSW    DigInFilter[eDF_OILSW].fState
#define DF_NEUTR    DigInFilter[eDF_NEUTR].fState
#define DF_HBEAM    DigInFilter[eDF_HBEAM].fState
#define DF_GPI_0    DigInFilter[eDF_GPI_0].fState
#define DF_GPI_1    DigInFilter[eDF_GPI_1].fState
#define DF_GPI_2    DigInFilter[eDF_GPI_2].fState
#define DF_GPI_3    DigInFilter[eDF_GPI_3].fState


/* Honda AfriaTwin dedicated macros to access filtered digital inputs */
#define DF_Fuel_8l_AT           DigInFilter[eDF_GPI_0].fState     // low active switch
#define DF_Fuel_4l_AT           DigInFilter[eDF_GPI_1].fState     // low active switch


/* BMW F650 dedicated macros to access filtered digital inputs */
#define DF_Temp_Warn_F650       DigInFilter[eDF_GPI_0].fState     // high active switch
#define DF_Fuel_4l_F650         DigInFilter[eDF_GPI_1].fState     // low active switch
#define DF_ABS_Warn_F650        DigInFilter[eDF_GPI_2].fState     // low active switch



/* MuZ Baghira decicated macros to access filtered digital inputs */
#define DF_Temp_Warn_BAGHIRA    DigInFilter[eDF_GPI_0].fState     // high active switch




/* ----------------------------------------------------------
   Digital Input Filter

   Note:    - digital inputs are mapped to UINT8 variabel
            - any HIGH value incr. filter value with SLOW/FAST_INR
            - any LOW  value decr. filter value with SLOW/FAST_DECR
            - filter value is interpreted as
                - HIGH if value == 255 (max)
                - LOW  if value == 0   (min)
            - Digital values will be checked/updated every 20 ms (LOOP):

            - Filtertime to detect HIGH value:  [ FT_high = MAX / INCR * LOOP ]

                                             => [ INCR    = MAX * LOOP / FT_high ]

            - Filtertime to detect LOW value:   [ FT_low  = MAX / DECR * LOOP ]

                                             => [ DECR    = MAX * LOOP / FT_low ]

            - Examples:
                - incr/decr = 100  ->  255/100 * 20ms =   40 ms  ->  0,04 sec filter time
                - incr/decr =  50  ->  255/ 50 * 20ms =  100 ms  ->  0,10 sec filter time
                - incr/decr =   1  ->  255/  1 * 20ms = 5100 ms  ->  5,10 sec filter time

    ---------------------------------------------------------- */

#define DIGFILT_MIN           1         // in ms, time until LOW/HIGH detected!
#define DIGFILT_MAX        5100         // in ms, time until LOW/HIGH detected!

#define DIGFILT_DEF     DIGFILT_MIN     // indicated immediate behaviour (no filtering)

#define DIGFILT_FUEL_HIGH      2000     // in ms, time until HIGH detected!
#define DIGFILT_FUEL_LOW       4000     // in ms, time until LOW detected!



/* time constants */
#define MINDURWAIT     (UINT8)15        /* minimal wait time in ms before 'pressed' detected */
#define MINDUR         (UINT8)1         /* minimum 1 ms duration */
#define KEYSHORT       (UINT16)800      /* delay to begin key repetition rate in ms */
#define KEYREPRATE     (UINT16)100      /* message repition rate in ms, when pressed long time */
#define KEYSECURE      (UINT16)2000     /* time for user before strongly reset values in ms */
#define KEYSAVE        (UINT16)1200     /* time for user close & save an edited value in ms */



/* key IDs (just to read key name in debugger ssource code) */
typedef enum
{
    KEY_OK,         /* [OK] Button */
    KEY_UP,         /* [UP] Button */
    KEY_DOWN,       /* [DOWN] Button */
    KEY_LAST        /* THIS IS AN INVALID KEY! */
} KEYNUMBER;

/* key flag definitions (bit masked) if multiple keys pressed */
#define KEYFL_OK      ((UINT8)(1 << KEY_OK))        /* [OK] Button */
#define KEYFL_UP      ((UINT8)(1 << KEY_UP))        /* [UP] Button */
#define KEYFL_DOWN    ((UINT8)(1 << KEY_DOWN))      /* [DOWN] Button */


/* key transition (activation) states */
typedef enum
{
    KEYTRANS_OFF,       /* key is unpressed for a longer period */
    KEYTRANS_PRESSED,   /* key has just been pressed! */
    KEYTRANS_ON,        /* key has been pressed for a longer period (KEYSHORT) */
    KEYTRANS_RELEASED,  /* key has just been released! */
    KEYTRANS_LAST       /* THIS IS AN INVALID TRANSITION STATE! */
} KEYTRANSIT;

/* to protocol pressed duration */
/* theses values can count up to 65 s press time */
typedef struct
{
    UINT16      wStartTime_ms;      /* time stamp of first 'pressed' detection in ms */
    UINT16      wLastSentTime_ms;   /* time stamp to handle intervals between key messages */
    UINT16      wDur_ms;       /* duration of 'pressed time' in ms */
    KEYTRANSIT  KeyTransit;         /* to handle state transitions */
} KEYTIME;


/* macros for easier acces to message user data */
/* easier interface to generate MSG_KEY_x messgaes */
#define MSG_BUILD_KEY(msg, KeyMsgId, fpKeyTime) MSG_BUILD_UINT8(msg, KeyMsgId, LOWBYTE(fpKeyTime->wDur_ms), HIGHBYTE(fpKeyTime->wDur_ms), fpKeyTime->KeyTransit);
/* get key pressed duration out of msg 'MSG_KEY_x': */
#define MSG_KEY_DURATION(msg)                   (((UINT16)MSG_CHAR1(msg)) + ((UINT16)(MSG_CHAR2(msg)) << 8))
/* get key transition state out of msg 'MSG_KEY_x': */
#define MSG_KEY_TRANSITION(msg)                 (MSG_CHAR3(msg))
/* get all pressed keys out of msg 'MSG_KEYS_PRESSED': */
#define MSG_KEY_STATES(msg)                     (MSG_CHAR3(msg))



/* function prototypes */

ERRCODE DigInDrv_Init(void);

UINT8   DigInDrv_GetKeyStates(void);
ERRCODE DigInDrv_CheckKeyAction(void);
ERRCODE DigInDrv_SendKeyMessage(const KEYNUMBER Key, const KEYTIME far * fpKeyData);

UINT8   DigInDrv_GetHWVersion(void);

void    DigInDrv_CheckAllPorts(void);

void    DigInDrv_FilterInit(void);
void    DigInDrv_Filter(void);
UINT8   DigInDrv_FilterConvertTime(UINT16 wFilterTime);

#endif /* _DIGINDRV_H */





 