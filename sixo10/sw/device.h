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
 *  Created:        24-11-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         devices
 *  Purpose:        all devices interfaces
 *  Comments:
 *                  NOTE: *** Meaning of devices ***
 *
 *                  Devices in this systems context are single and autark
 *                  working modules to solve a special feature, f.e. the
 *                  'TripCounter' or the 'MainMenu'.
 *
 ************************************************************************ */
#ifndef _DEVICES_H
#define _DEVICES_H


/* general defines */
#define MAX_REFRESH_MS    400     /* max delay between two screen refreshes in ms */


/* IDs to handle messages between devices */
typedef enum
{
    DEVID_UNKNOWN,          // to unknown recipient device
    DEVID_ALL,              // to all devices
    DEVID_MAIN,             // to/from Main device
    DEVID_INTRO,            // to/from IntroScreen device
    DEVID_TRIPCOUNT,        // to/from TripCounter device
    DEVID_SET,              // to/from Settings device
    DEVID_MENU,             // to/from Menu device
    DEVID_TEST,             // to/from Test device
    DEVID_DIAGN,            // to/from Diagnose device
    DEVID_DEBUG,            // to/from Debug device
    DEVID_WARNING,          // to/from Warning device
    DEVID_LAST,             // invalid ID
} DEVICE_ID;



/* device object data itself */
typedef struct
{
    DEVICE_ID   eDevID;         /* unique device ID */
    STRING      szDevName;      /* debug purpose: name of device */
    BOOL        fFocused;       /* flag to show device is 'on screen' */
    BOOL        fScreenInit;    /* flag to show that device screen is initialized */
    UINT16      wDevState;      /* device state machine */
} DEVDATA;


/* message function macros */
#define MSG_BUILD_SETFOCUS(msg,SendID,ReceivID)     msg.BBBI.id=MSG_SET_FOCUS;      msg.BBBI.byte1=SendID; msg.BBBI.byte2=ReceivID; msg.BBBI.byte3=0xff
#define MSG_BUILD_GETFOCUS(msg,SendID,ReceivID)     msg.BBBI.id=MSG_GET_FOCUS;      msg.BBBI.byte1=SendID; msg.BBBI.byte2=ReceivID; msg.BBBI.byte3=0xff
#define MSG_BUILD_REFRESH(msg,SendID,ReceivID)      msg.BBBI.id=MSG_SCREEN_REFRESH; msg.BBBI.byte1=SendID; msg.BBBI.byte2=ReceivID; msg.BBBI.byte3=0xff
#define MSG_SENDER_ID(msg)                          MSG_BYTE1(msg)
#define MSG_RECEIVER_ID(msg)                        MSG_BYTE2(msg)


/* undedicated device functions */
ERRCODE DevCyclicRefresh(void);

/* SetDevice function prototypes */
ERRCODE SetDeviceInit(void);
ERRCODE SetDeviceMsgEntry(MESSAGE msg);


/* TripCounter device function prototypes */
ERRCODE TripCntDevInit(void);
void    TripCntDevShow(BOOL fShow);
ERRCODE TripCntMsgEntry(MESSAGE GivenMsg);

/* Main device function prototypes */
ERRCODE MainDeviceInit(void);
void    MainDeviceShow(BOOL fShow);
ERRCODE MainDeviceMsgEntry(MESSAGE msg);

/* Intro screen device function prototypes */
ERRCODE IntroScreenInit(void);
void    IntroScreenShow(BOOL fShow);
ERRCODE IntroScreenMsgEntry(MESSAGE GivenMsg);

ERRCODE TestScreenInit(void);
void    TestScreenShow(BOOL fShow);
ERRCODE TestScreenMsgEntry(MESSAGE msg);
void    Test_TestScreenStartMsg(void);
void    Test_ValueInkrMsg(MESSAGE GivenMsg);
INT16   Test_ValueInkr(MESSAGE KeyMsg);


#endif /* _DEVICES_H */


