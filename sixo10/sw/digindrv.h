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
 *  Created:        09-11-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         Digital Input
 *  Purpose:        Handles all General Purpose Inputs
 *  Comments:       -
 *
 *                  only for Ralfs BMW Version to
 *                  support Oil and Generator lamps:
 *
 *                      #ifdef HW_OILGEN
 *
 ************************************************************************ */
#ifndef _DIGINDRV_H
#define _DIGINDRV_H


/* port direction register */
#define DigInIntPortD   pd8_2   /* INT2 OR'd digital inputs port direction bit */

#ifdef HW_OILGEN
    #define OilPress_D      pd2_0   /* oil pressure switch */
    #define EngGen_D        pd2_1   /* engine generator */
#else // HW_OILGEN
    #define Unused_GPI0_D   pd2_0   /* unused input port */
    #define Unused_GPI1_D   pd2_1   /* unused input port */
#endif // HW_OILGEN
#define Unused_GPI2_D   pd2_2   /* unused input port */
#define KeyPortD_Up     pd2_3   /* DOWN Button */
#define KeyPortD_Down   pd2_4   /* UP Button */
#define KeyPortD_Ok     pd2_5   /* OK Button */
// port 2_6 and 2_7 reserved for output purpose */



/* port register */
#define DigInIntPort    p8_2    /* INT2 OR'd digital inputs port bit */

#ifdef HW_OILGEN
    #define OilPress        p2_0   /* oil pressure switch */
    #define EngGen          p2_1   /* engine generator */
#else // HW_OILGEN
    #define Unused_GPI0     p2_0   /* unused input port */
    #define Unused_GPI1     p2_1   /* unused input port */
#endif // HW_OILGEN
#define Unused_GPI2     p2_2   /* unused input port */
#define KeyPort_Up      p2_3    /* DOWN Button */
#define KeyPort_Down    p2_4    /* UP Button */
#define KeyPort_Ok      p2_5    /* OK Button */



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

#ifdef HW_OILGEN
    ERRCODE DigInDrv_CheckEnginePorts( void );
#endif // of HW_OILGEN



#endif /* _DIGINDRV_H */





 