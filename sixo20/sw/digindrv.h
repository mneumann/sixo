/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-09 by Ralf Krizsan
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
#define HWVERS_PINS     0x3     // 3 bit used to code hw version
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

#define DigIn_TurnL   p2_3
#define DigIn_TurnR   p2_4
#define DigIn_OilSw   p2_5
#define DigIn_Neutral p2_6
#define DigIn_HBeam   p2_7

#define DigIn_GPI_0   p1_4
#define DigIn_GPI_1   p1_5
#define DigIn_GPI_2   p1_6
#define DigIn_GPI_3   p1_7

/* Alternater Warning will later be done via analogue measurement */
#define DigInD_ALTW   pd10_4
#define DigIn_ALTW    p10_4


/* special motor cycle defines */

// BIKE_AFRICATWIN
#define DigIn_Fuel_8l       DigIn_GPI_0     // low active switches for
#define DigIn_Fuel_4l       DigIn_GPI_1     // low active switches

// BIKE_BAGHIRA
#define DigIn_Temp_Warn     DigIn_GPI_0     // high active switch

// BIKE_R100GS
#define DigIn_Altern_Warn   DigIn_GPI_0     // high active switch



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
    UINT16      wDuration_ms;       /* duration of 'pressed time' in ms */
    KEYTRANSIT  KeyTransit;         /* to handle state transitions */
} KEYTIME;


/* macros for easier acces to message user data */
/* easier interface to generate MSG_KEY_x messgaes */
#define MSG_BUILD_KEY(msg, KeyMsgId, fpKeyTime) MSG_BUILD_UINT8(msg, KeyMsgId, LOWBYTE(fpKeyTime->wDuration_ms), HIGHBYTE(fpKeyTime->wDuration_ms), fpKeyTime->KeyTransit);
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
UINT8 DigInDrv_GetHWVersion(void);
void DigInDrv_CheckAllPorts(void);


#endif /* _DIGINDRV_H */





 