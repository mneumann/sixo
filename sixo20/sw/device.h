/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-11-24 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         devices
 *  Purpose:        all devices interfaces export api header
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
 *  --------------------------------------------------------------------
 *
 *  CVS History
 *
 *  This information is automatically added while 'commiting' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 3.0  2010/11/07 13:28:26  tuberkel
 * V30 Preparations:
 * - Device Handling completely reviewed & simplified
 * - Support of 'ObjectLists' inside selected Device
 * - automatic focus of selected object in device
 *
 * Revision 2.3  2009/07/22 12:46:30  tuberkel
 * Device Object Focus handling simplified:
 * - use DevObjFocusSet() / DevObjFocusReset() inside any device
 * - DevObjFocusReset() to set focus to default state
 * - use DevObjFocusSet() to move focus based on UP/DOWN key
 *
 * Revision 2.2  2009/07/15 09:03:27  tuberkel
 * NEW: #define TESTSCREEN for GUI Tests
 *
 * Revision 2.1  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 * Revision 1.8  2006/02/18 14:37:33  Ralf
 * - BIKE_MOTOBAU: lapcounter and statistic devices added
 *
 *
 ************************************************************************ */


#ifndef _DEVICES_H
#define _DEVICES_H



// ----------------------------------------------------------------
/* general defines */
#define MAX_REFRESH_MS    400     /* max delay between two screen refreshes in ms */



// ----------------------------------------------------------------
/* IDs to handle messages between devices */
typedef enum
{
    DEVID_UNKNOWN,          // to unknown recipient device

    // these devices are currently supported
    DEVID_ALL,              // to all devices
    DEVID_MAIN,             // Main device
    DEVID_INTRO,            // IntroScreen device
    DEVID_TRIPCOUNT,        // TripCounter device
    DEVID_SET,              // Settings device
    DEVID_STATISTIC,        // Statistic device
    DEVID_LAPCNT,           // LapCounter device
    DEVID_MONITOR,          // Monitor device
    DEVID_HWTEST,           // HW test Device
    DEVID_TESTSCREEN,       // Testscreen device

    // these device IDs are prepared for future use
    DEVID_MENU,             // Menu device
    DEVID_DIAGN,            // Diagnose device
    DEVID_DEBUG,            // Debug device
    DEVID_WARNING,          // Warning device

    DEVID_LAST              // invalid ID
} DEVICE_ID;




// ----------------------------------------------------------------
// object list control structure (to handle a screen)
typedef struct
{
    UINT8           ObjCount;       // overall list size in number of objects
    UINT8           LastSelObj;     // last selectable object in list
    UINT8           FirstSelObj;    // first selectable object in list
    void far *      ObjList;        // address of object list
} DEVOBJLIST;




// ----------------------------------------------------------------
/* device object data itself */
typedef struct
{
    DEVICE_ID   eDevID;         /* unique device ID */
    STRING      szDevName;      /* debug purpose: name of device */
    BOOL        fFocused;       /* flag to show device is 'on screen' */
    BOOL        fScreenInit;    /* flag to show that device screen is initialized */
    UINT16      wDevState;      /* device state machine */
    UINT8       bFocusObj;      /* index of currently focused object inside device */
    DEVOBJLIST  Objects;        /* list of all objects inside the device */
} DEVDATA;




// ----------------------------------------------------------------
/* special LapCounter device state */
typedef struct
{
    char    fActive:1;      // 1 indicates, LapCounter is running on focused Counter
    char    fReserved:3;    // for future use
    char    cCurrentLap:4;  // currently focused (active) lap, valid 0..15

} LCSTATE_TYPE;
#define LAPS_MAX    16  // we have 16 laps max



// ----------------------------------------------------------------
// some macros to ease object alignement in characters
// result is number of pixels.

// ROW pixel alignement (row 1..6)
// an additional empty pixel line is
// used to improve readability
#define R1     (0 * (DPLFONT_6X8_HEIGHT+1))
#define R2     (1 * (DPLFONT_6X8_HEIGHT+1))
#define R3     (2 * (DPLFONT_6X8_HEIGHT+1))
#define R4     (3 * (DPLFONT_6X8_HEIGHT+1))
#define R5     (4 * (DPLFONT_6X8_HEIGHT+1))
#define R6     (5 * (DPLFONT_6X8_HEIGHT+1))
#define R7     (6 * (DPLFONT_6X8_HEIGHT+1))

// COLUMN pixel alignement (columns 1..21)
#define C01    ( 0  * DPLFONT_6X8_WIDTH)
#define C02    ( 1  * DPLFONT_6X8_WIDTH)
#define C03    ( 2  * DPLFONT_6X8_WIDTH)
#define C04    ( 3  * DPLFONT_6X8_WIDTH)
#define C05    ( 4  * DPLFONT_6X8_WIDTH)
#define C06    ( 5  * DPLFONT_6X8_WIDTH)
#define C07    ( 6  * DPLFONT_6X8_WIDTH)
#define C08    ( 7  * DPLFONT_6X8_WIDTH)
#define C09    ( 8  * DPLFONT_6X8_WIDTH)
#define C10    ( 9  * DPLFONT_6X8_WIDTH)
#define C11    (10  * DPLFONT_6X8_WIDTH)
#define C12    (11  * DPLFONT_6X8_WIDTH)
#define C13    (12  * DPLFONT_6X8_WIDTH)
#define C14    (13  * DPLFONT_6X8_WIDTH)
#define C15    (14  * DPLFONT_6X8_WIDTH)
#define C16    (15  * DPLFONT_6X8_WIDTH)
#define C17    (16  * DPLFONT_6X8_WIDTH)
#define C18    (17  * DPLFONT_6X8_WIDTH)
#define C19    (18  * DPLFONT_6X8_WIDTH)
#define C20    (19  * DPLFONT_6X8_WIDTH)
#define C21    (20  * DPLFONT_6X8_WIDTH)




// ----------------------------------------------------------------
/* message function macros */
#define MSG_BUILD_SETFOCUS(msg,SendID,ReceivID)     msg.BBBI.id=MSG_SET_FOCUS;      msg.BBBI.byte1=SendID; msg.BBBI.byte2=ReceivID; msg.BBBI.byte3=0xff
#define MSG_BUILD_GETFOCUS(msg,SendID,ReceivID)     msg.BBBI.id=MSG_GET_FOCUS;      msg.BBBI.byte1=SendID; msg.BBBI.byte2=ReceivID; msg.BBBI.byte3=0xff
#define MSG_BUILD_REFRESH(msg,SendID,ReceivID)      msg.BBBI.id=MSG_SCREEN_REFRESH; msg.BBBI.byte1=SendID; msg.BBBI.byte2=ReceivID; msg.BBBI.byte3=0xff
#define MSG_SENDER_ID(msg)                          MSG_BYTE1(msg)
#define MSG_RECEIVER_ID(msg)                        MSG_BYTE2(msg)



// ----------------------------------------------------------------
/* undedicated device functions */
ERRCODE DevCyclicRefresh        ( void );
ERRCODE DevObjFocusSet          ( DEVDATA far * fpDevData, void far * far * GivenList, UINT8 ListSize, MESSAGE GivenMsg);
ERRCODE DevObjMsg               ( DEVDATA far * fpDevData, void far * far * GivenList, UINT8 ListSize, MESSAGE GivenMsg );
ERRCODE DevObjShow              ( DEVDATA far * fpDevData, void far * far * GivenList, UINT8 ListSize, UINT8 UpdateMode );
ERRCODE DevObjFocusReset        ( DEVDATA far * fpDevData, void far * far * GivenList, UINT8 ListSize );
ERRCODE DevObjInit              ( DEVDATA far * fpDevData, void far *       GivenList, UINT8 ListSize, UINT8 eType );
ERRCODE DevObjSetState          ( DEVDATA far * fpDevData, void far * far * GivenList, UINT8 ListSize, UINT8 State );
ERRCODE DevObjClearState        ( DEVDATA far * fpDevData, void far * far * GivenList, UINT8 ListSize, UINT8 State );
UINT8   DevObjGetFirstSelectable( DEVDATA far * fpDevData, void far * far * GivenList, UINT8 ListSize );
UINT8   DevObjGetLastSelectable ( DEVDATA far * fpDevData, void far * far * GivenList, UINT8 ListSize );



// ----------------------------------------------------------------
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

/* monitor device function protypes */
ERRCODE MonitorDeviceInit(void);
void    MonitorDeviceShow(BOOL fShow);
ERRCODE MonitorDeviceMsgEntry(MESSAGE msg);

/* statistic device function protypes */
ERRCODE StatisticDeviceInit(void);
void    StatisticDeviceShow(BOOL fShow);
ERRCODE StatisticDeviceMsgEntry(MESSAGE msg);
void    StatisticDeviceUpdateStrings(void);

/* lapcounter device function protypes */
ERRCODE LapCntDeviceInit(void);
void    LapCntDeviceShow(BOOL fShow);
ERRCODE LapCntDeviceMsgEntry(MESSAGE msg);
void    LapCntUpdateTime(void);

/* HW self test device function protypes */
ERRCODE HWTestDeviceInit(void);
void    HWTestDeviceShow(BOOL fShow);
ERRCODE HWTestDeviceMsgEntry(MESSAGE msg);

/* test device (for development purpose only) */
#ifndef TESTSCREEN
#define TESTSCREEN 0
#endif

#if(TESTSCREEN==1)
ERRCODE TestScreenInit(void);
void    TestScreenShow(BOOL fShow);
ERRCODE TestScreenMsgEntry(MESSAGE msg);
void    TestScreenStartMsg(UINT16);
#endif// TESTSCREEN


#endif /* _DEVICES_H */


