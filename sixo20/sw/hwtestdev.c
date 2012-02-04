/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2005-03-05 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         hw test device
 *  Purpose:        show hardware selftest device screen
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
 * Revision 3.4  2012/02/04 21:49:42  tuberkel
 * All BeeperDriver functions mapped ==> DigOutDrv()
 *
 * Revision 3.3  2012/02/04 20:38:05  tuberkel
 * Moved all BeeperDriver / LEDDriver stuff ==> 'digoutdrv'
 *
 * Revision 3.2  2012/01/23 05:43:03  tuberkel
 * PIN_GPO1 & 2 activated, if no Tester present
 *
 * Revision 3.1  2012/01/14 08:28:42  tuberkel
 * Message-IDs shortened / reviewed
 *
 * Revision 3.0  2010/11/07 13:38:56  tuberkel
 * V30 Preparations:
 * - changed to ne simplified device object handling
 * - Key- and WHEEL Check behaviour improved
 * - RTC testss
 * - BugFix StringBuffer Sizes
 * - Status Display changed:
 *   - szOk '.' --> '+'
 *   - szUnknown '_' --> '?'
 *
 * Revision 2.5  2009/07/19 12:26:01  tuberkel
 * - ObjectInit reviewed
 * - dyn. text handling disabled
 *
 * Revision 2.4  2009/07/08 21:49:03  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.3  2009/07/08 21:41:17  tuberkel
 * All compiler defines reviewed:
 * - set to unique usage: set define to 0 or 1
 * - default values set, if not used
 * - see 'Project Editor' for details
 *
 * Revision 2.2  2009/06/21 17:49:50  tuberkel
 * Changes done by AN:
 * HW-Tests / Uart not enabled if COMPASS defined
 *
 * Revision 2.1  2009/04/14 21:17:55  tuberkel
 * Changes done by Arnold:
 * - HWTestCheckUnstimulatedErrors() uses dyn. Eeprom Magic Number Address
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 * Revision 1.3  2006/02/18 15:11:38  Ralf
 * - removed obsolete reboot code
 *
 *
 ************************************************************************ */



/* include */
#include <stdio.h>
#include <string.h>

#include "standard.h"
#include "debug.h"
#include "msgqueue.h"
#include "timer.h"
#include "display.h"
#include "resource.h"
#include "objects.h"
#include "device.h"
#include "digindrv.h"
#include "sysparam.h"
#include "anaindrv.h"
#include "anain.h"
#include "led.h"
#include "fonts.h"
#include "digoutdr.h"
#include "sfr62p.h"
#include "measure.h"
#include "m16c.h"
#include "beep.h"
#include "displdrv.h"
#include "measdrv.h"
#include "timedate.h"
#include "nvramdrv.h"
#include "eeprdrv.h"
#include "clkdrv.h"
#include "diagnose.h"



/* external symbols */
extern UINT16  wMilliSecCounter;    // valid values: 0h .. ffffh
extern UINT16  wSecCounter;         // valid values: 0h .. ffffh

extern STRING far           szDevName[];                /* device names */
extern DEVFLAGS1_TYPE        gDeviceFlags1;               /* system parameters */


/* local objects */
static DEVDATA      HWTestDev;           /* this device */
static TEXTOBJECT   HWTestScreen;        /* basic screen content ------------------- */
static BOOL         gfEOLTest = FALSE;   /* TRUE if test adapter present, FALSE if runs in vehicle */
static BOOL         fGPO0 = FALSE;       /* for hw stimulation of pin GPO_0 */
static BOOL         fGPO1 = FALSE;       /* for hw stimulation of pin GPO_1 */

/* variable strings to contain current values */
static char szVoltage[6];
static char szKmh[6];
static char szRPM[6];
static char szOilTemp[6];
static char szWatTemp[6];
static char szExtTemp[6];
static char szIntTemp[6];
static char szAltW[6];
static char szLDR[6];
static char szHWID[2];


/* constant strings to indicate test status */
static char         szOk[] =    "+";
static char         szErr[]=    "E";
static char         szUnknown[]="?";


/* text objects for values */
static TEXTOBJECT   VoltageObj;
static TEXTOBJECT   KmhObj;
static TEXTOBJECT   RPMObj;
static TEXTOBJECT   OilTempObj;
static TEXTOBJECT   WatTempObj;
static TEXTOBJECT   ExtTempObj;
static TEXTOBJECT   IntTempObj;
static TEXTOBJECT   AltWObj;
static TEXTOBJECT   LDRObj;


/* text objects for key status */
static TEXTOBJECT   KeyUp;
static TEXTOBJECT   KeyDown;
static TEXTOBJECT   KeyOk;

/* text objects for test status */
static TEXTOBJECT   StatObj_VBAT;
static TEXTOBJECT   StatObj_KMH;
static TEXTOBJECT   StatObj_WHEEL;
static TEXTOBJECT   StatObj_RPM;
static TEXTOBJECT   StatObj_TOIL;
static TEXTOBJECT   StatObj_TWAT;
static TEXTOBJECT   StatObj_TAIR;
static TEXTOBJECT   StatObj_TDEV;
static TEXTOBJECT   StatObj_ALTW;
static TEXTOBJECT   StatObj_HWVER;
static TEXTOBJECT   StatObj_LDR;
static TEXTOBJECT   StatObj_EEPR;
static TEXTOBJECT   StatObj_RTC;
static TEXTOBJECT   StatObj_NVRAM;
static TEXTOBJECT   StatObj_UART0;
static TEXTOBJECT   StatObj_UART1;
static TEXTOBJECT   StatObj_BAT;
static TEXTOBJECT   StatObj_NEUTR;
static TEXTOBJECT   StatObj_GPI0;
static TEXTOBJECT   StatObj_GPI1;
static TEXTOBJECT   StatObj_GPI2;
static TEXTOBJECT   StatObj_GPI3;
static TEXTOBJECT   StatObj_HBEAM;
static TEXTOBJECT   StatObj_OIL;
static TEXTOBJECT   StatObj_TURNL;
static TEXTOBJECT   StatObj_TURNR;
static TEXTOBJECT   StatObj_KEY0;
static TEXTOBJECT   StatObj_KEY1;
static TEXTOBJECT   StatObj_KEY2;




/* Shows this screen ----------------------------

    +....!....!....!....!....!....!..+
    |V+   16.1V _   EEPROM  _  BAT29_|  4x6 font = 32x8 Screen
    |ALTW  4.3V _   NVRAM   _  RTC  _|
    |RPM   1250 _   UART 01__  HWID _|  '_' equals not yet tested
    |KM/H   120 _   LDR 123 _  WHEEL_|  'E' equals error
    |TWAT  50'C _   TURNLR __  OIL  _|  '.' equals ok
    |TAIR  50'C _   HBEAM   _  NEUTR_|
    |TOIL  50'C _   GPI 0123 ____    |
    |TDEV  23'C _   KEY 012  ___  UDO|
    +....!....!....!....!....!....!..+

    V+:     Power Supply in Volts
    ALTW:   Vcc(out) connected with ALTW (In) in Volts
    RPM:    engine rounds per minute
    KM/H:   measured speed in km/h
    TOIL:   oil temp input in °C
    TWAT:   water temp input in °C
    TAIR:   air temp input in °C
    TDEV:   device internal temp input in °C

    HW:     hardware versionnumber
    EEPROM: EEPROM R/W software selftest
    RTC:    RTC R/W software selftest
    NVRAM:  NVRAM R/W software selftest
    UART:   Uart 0+1 RX/TX Loopbacktest
    BATxx:  Lithium battery check, done by reboot in xx seconds

    WHEEL:  wheel contact status
    NEUTR:  neutral input
    GPI:    0..3 general input
    HBEAM:  high beam input
    OIL:    oil switch input
    TURN:   turn left/right input
    KEY:    Up/Down/Ok input
    UDO:    U=UP / D=Down / O=OK






   ----------------------------------------------------------------------- */

/* Cursor-to-Pixel calculations (based on 6x8 font) */
#define C2PH(c)     ((c)*4)
#define C2PV(c)     ((c)*8)


/* display columns to adjust output */
#define HWTSTDEV_COL_1  C2PH(0)     // left origin
#define HWTSTDEV_COL_2  C2PH(9)     // left 'checked' column
#define HWTSTDEV_COL_3  C2PH(11)    // second text column
#define HWTSTDEV_COL_4  C2PH(20)    // right 'checked' column


// hw test defines
#ifndef VARY_DISPLAY
#define VARY_DISPLAY    0       // 1 enables contrast/backlight variation during test
#endif


// HW Test  device resources ----------------------------
static const TEXTOBJECT_INITTYPE TextObjInit[] =
{
    /* basic screen content ------------------- */

    /* Object Name        X        Y       Font         H  W    align     format    src                  state     */
    /* ----------------- -------  -------  ------------ -- -- ---------- --------- --------------------  --------- */

    /* complete static background description incl. descriptors ------  */
    { &HWTestScreen,     C2PH(0), C2PV(0), DPLFONT_4X6, 8, 32, TXT_LEFT,  TXT_NORM, RESTXT_HWTDEV_SCREEN_32x8,  OC_DISPL },

    /* analog values ------  */
    { &VoltageObj,       C2PH(5), C2PV(0), DPLFONT_4X6, 1,  4, TXT_RIGHT, TXT_NORM, szVoltage,  OC_DISPL | OC_DYN },
    { &AltWObj,          C2PH(5), C2PV(1), DPLFONT_4X6, 1,  4, TXT_RIGHT, TXT_NORM, szAltW,     OC_DISPL | OC_DYN },
    { &RPMObj,           C2PH(5), C2PV(2), DPLFONT_4X6, 1,  5, TXT_RIGHT, TXT_NORM, szRPM,      OC_DISPL | OC_DYN },
    { &KmhObj,           C2PH(6), C2PV(3), DPLFONT_4X6, 1,  4, TXT_RIGHT, TXT_NORM, szKmh,      OC_DISPL | OC_DYN },
    { &WatTempObj,       C2PH(5), C2PV(4), DPLFONT_4X6, 1,  3, TXT_RIGHT, TXT_NORM, szWatTemp,  OC_DISPL | OC_DYN },
    { &ExtTempObj,       C2PH(5), C2PV(5), DPLFONT_4X6, 1,  3, TXT_RIGHT, TXT_NORM, szExtTemp,  OC_DISPL | OC_DYN },
    { &OilTempObj,       C2PH(5), C2PV(6), DPLFONT_4X6, 1,  3, TXT_RIGHT, TXT_NORM, szOilTemp,  OC_DISPL | OC_DYN },
    { &IntTempObj,       C2PH(5), C2PV(7), DPLFONT_4X6, 1,  3, TXT_RIGHT, TXT_NORM, szIntTemp,  OC_DISPL | OC_DYN },


    /* test status objects */
    { &StatObj_VBAT,    C2PH(11), C2PV(0), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_ALTW,    C2PH(11), C2PV(1), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_RPM,     C2PH(11), C2PV(2), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_KMH,     C2PH(11), C2PV(3), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_TOIL,    C2PH(11), C2PV(4), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_TWAT,    C2PH(11), C2PV(5), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_TAIR,    C2PH(11), C2PV(6), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_TDEV,    C2PH(11), C2PV(7), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },

    { &StatObj_EEPR,    C2PH(23), C2PV(0), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_NVRAM,   C2PH(23), C2PV(1), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_UART0,   C2PH(22), C2PV(2), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_UART1,   C2PH(23), C2PV(2), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },

    { &LDRObj,          C2PH(19), C2PV(3), DPLFONT_4X6, 1,  3, TXT_RIGHT, TXT_NORM, szLDR,      OC_DISPL | OC_DYN },
    { &StatObj_LDR,     C2PH(23), C2PV(3), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },

    { &StatObj_TURNL,   C2PH(22), C2PV(4), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_TURNR,   C2PH(23), C2PV(4), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_HBEAM,   C2PH(23), C2PV(5), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_GPI0,    C2PH(24), C2PV(6), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_GPI1,    C2PH(25), C2PV(6), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_GPI2,    C2PH(26), C2PV(6), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_GPI3,    C2PH(27), C2PV(6), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_KEY0,    C2PH(24), C2PV(7), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_KEY1,    C2PH(25), C2PV(7), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_KEY2,    C2PH(26), C2PV(7), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },

    { &StatObj_BAT,     C2PH(31), C2PV(0), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_RTC,     C2PH(31), C2PV(1), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_HWVER,   C2PH(31), C2PV(2), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_WHEEL,   C2PH(31), C2PV(3), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_OIL,     C2PH(31), C2PV(4), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },
    { &StatObj_NEUTR,   C2PH(31), C2PV(5), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, szUnknown,  OC_DISPL | OC_DYN },

    { &KeyUp,           C2PH(29), C2PV(7), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, "U",        OC_DISPL | OC_DYN },
    { &KeyDown,         C2PH(30), C2PV(7), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, "D",        OC_DISPL | OC_DYN },
    { &KeyOk,           C2PH(31), C2PV(7), DPLFONT_4X6, 1,  1, TXT_RIGHT, TXT_NORM, "O",        OC_DISPL | OC_DYN },
};
#define TEXTOBJECTLISTSIZE   (sizeof(TextObjInit)/sizeof(TEXTOBJECT_INITTYPE))



/* this devices object focus handling - list of all objects */
/* NOTE:    this device does not need any focus, this
            list is for common object handling only! */
static const void far * ObjectList[] =
{
    (void far *) &HWTestScreen,
    (void far *) &VoltageObj,
    (void far *) &AltWObj,
    (void far *) &RPMObj,
    (void far *) &KmhObj,
    (void far *) &WatTempObj,
    (void far *) &ExtTempObj,
    (void far *) &OilTempObj,
    (void far *) &IntTempObj,
    (void far *) &StatObj_VBAT,
    (void far *) &StatObj_ALTW,
    (void far *) &StatObj_RPM,
    (void far *) &StatObj_KMH,
    (void far *) &StatObj_TOIL,
    (void far *) &StatObj_TWAT,
    (void far *) &StatObj_TAIR,
    (void far *) &StatObj_TDEV,
    (void far *) &StatObj_EEPR,
    (void far *) &StatObj_NVRAM,
    (void far *) &StatObj_UART0,
    (void far *) &StatObj_UART1,
    (void far *) &LDRObj,
    (void far *) &StatObj_LDR,
    (void far *) &StatObj_TURNL,
    (void far *) &StatObj_TURNR,
    (void far *) &StatObj_HBEAM,
    (void far *) &StatObj_GPI0,
    (void far *) &StatObj_GPI1,
    (void far *) &StatObj_GPI2,
    (void far *) &StatObj_GPI3,
    (void far *) &StatObj_KEY0,
    (void far *) &StatObj_KEY1,
    (void far *) &StatObj_KEY2,
    (void far *) &StatObj_BAT,
    (void far *) &StatObj_RTC,
    (void far *) &StatObj_HWVER,
    (void far *) &StatObj_WHEEL,
    (void far *) &StatObj_OIL,
    (void far *) &StatObj_NEUTR,
    (void far *) &KeyUp,
    (void far *) &KeyDown,
    (void far *) &KeyOk,
};
#define OBJECTLIST_SIZE   (sizeof(ObjectList)/sizeof(OBJSTATE)/sizeof(void far *))
#define OBJECTLIST_FIRSTSEL    0    // first = last = no focus handling required
#define OBJECTLIST_LASTSEL     0




/* internal prototypes */
void HWTestWork ( void );
void HWTestInit ( void );
void HWTestCheckErrors( void );
void HWTestCheckStates( void );
BOOL HWTestCheckTesterPresent( void );
void HWTestUpdateMeasurement ( void );
void HWTestCheckStimulatedErrors( void );
void HWTestCheckUnstimulatedErrors( void );




/***********************************************************************
 *  FUNCTION:       HWTestDeviceInit
 *  DESCRIPTION:    all initial stuff for all objects of
 *                  'HW Test  screen device'
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE HWTestDeviceInit(void)
{
    int i;

    /* device main data */
    HWTestDev.eDevID       = DEVID_HWTEST;
    HWTestDev.szDevName    = szDevName[DEVID_HWTEST];
    HWTestDev.fFocused     = FALSE;
    HWTestDev.fScreenInit  = FALSE;

    /* initialize all objects of any type */
    DevObjInit( &HWTestDev, (void far *)TextObjInit,   TEXTOBJECTLISTSIZE,     OBJT_TXT   );

    /* initialize this devices objects list */
    HWTestDev.Objects.ObjList       = ObjectList;
    HWTestDev.Objects.ObjCount      = OBJECTLIST_SIZE;
    HWTestDev.Objects.FirstSelObj   = DevObjGetFirstSelectable(&HWTestDev,ObjectList,OBJECTLIST_SIZE);
    HWTestDev.Objects.LastSelObj    = DevObjGetLastSelectable (&HWTestDev,ObjectList,OBJECTLIST_SIZE);

    /* reset focus handling to start values */
    DevObjFocusReset( &HWTestDev, ObjectList, OBJECTLIST_SIZE );

    /* initialize HW stuff */
    HWTestInit();

    /* return */
    ODS( DBG_SYS, DBG_INFO, "- HWTestDeviceInit() done!");
    return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       HWTestDeviceShow
 *  DESCRIPTION:    bring updated 'HW Test screen device' to display
 *                  by calling Show-Fct. of all objects
 *  PARAMETER:      BOOL    TRUE  = show objects,
 *                          FALSE = clear screen
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void HWTestDeviceShow(BOOL fShow)
{
    ERRCODE error = ERR_OK;
    UINT8   ShowMode;

    /* its on screen? */
    if (fShow == TRUE)
    {
        /* get/update current display values */
        HWTestWork();

        /* do we have to repaint all? */
        if (HWTestDev.fScreenInit == FALSE)
        {
            DisplClearScreen(0x00);
            HWTestDev.fScreenInit = TRUE;
            ShowMode = SHOW_ALL;                   // repaint all stuff
        }
        else
        {   ShowMode = SHOW_EDIT | SHOW_CURSOR;    // repaint only potential changed stuff
        }

        // FOR DISPLAY OBJECTS TEST ONLY: clear & proof display mode of objects!
        //DisplClearScreen(0xaa);

        /* process complete (active) object to show all objects */
        DevObjShow(&HWTestDev,
                    HWTestDev.Objects.ObjList,
                    HWTestDev.Objects.ObjCount,
                    ShowMode );
    }
    else
    {
        // clear screen
        DisplClearScreen(0x0);

        // reset states of all objects of this device
        DevObjClearState(  &HWTestDev,
                            HWTestDev.Objects.ObjList,
                            HWTestDev.Objects.ObjCount,
                            OS_DISPL | OS_EDIT );

        // set overall device state to 'not init'
        HWTestDev.fScreenInit  = FALSE;
    }
}


/***********************************************************************
 *  FUNCTION:       HWTestDeviceMsgEntry
 *  DESCRIPTION:    Receive Message Handler of 'HW Test screen' device
 *                  called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE HWTestDeviceMsgEntry(MESSAGE GivenMsg)
{
    ERRCODE      RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID   MsgId;
    MESSAGE      NewMsg;

    /* analyze message: it's for us? */
    MsgId = MSG_ID(GivenMsg);        /* get message id */
    switch (MsgId)
    {
        case MSG_GET_FOCUS:
        {
            /*  Some device want's to get the focus:
                If we've currently got the focus, we'll
                answer to SET his focus! */
            if ( HWTestDev.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_HWTEST]);
                MSG_BUILD_SETFOCUS(NewMsg, DEVID_HWTEST, MSG_SENDER_ID(GivenMsg));   /* build answer message */
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                            /* send answer message */
                HWTestDev.fFocused = FALSE;                                        /* clear our focus */
                HWTestDeviceShow(FALSE);                                                 /* clear our screen */
                RValue = ERR_MSG_PROCESSED;
            }
        } break;
        case MSG_SET_FOCUS:
        {
            /* messsage for us? */
            if (MSG_RECEIVER_ID(GivenMsg) == DEVID_HWTEST)
            {
                /*  Someone wants us to take the focus?
                    We assume, that nobody else has the focus
                    and we've got the the screen now for us! */
                if (  (HWTestDev.fFocused   == TRUE             )
                    &&(MSG_RECEIVER_ID(GivenMsg) == DEVID_HWTEST) )
                    ODS2(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but it already has focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_HWTEST]);
                else /*
                    ODS2(   DBG_SYS, DBG_INFO,
                            "FOCUS: %s -> %s!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_HWTEST]) */ ;
                HWTestDev.fFocused = TRUE;                         /* set our focus */
                HWTestDeviceShow(TRUE);                            /* show our screen */
                gDeviceFlags1.flags.ActDevNr = DEVID_HWTEST;        /* save device# for restore */
                RValue = ERR_MSG_PROCESSED;
             }
             else
             {
                if ( HWTestDev.fFocused == TRUE )
                {
                    /*  Some other device should be given the focus,
                        BUT WE'VE GOT THE FOCUS!
                        It should first ask US to give the focus! :-( */
                    ODS3(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but %s had focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[MSG_RECEIVER_ID(GivenMsg)],
                            szDevName[DEVID_HWTEST]);
                    HWTestDev.fFocused = FALSE;                        /* loose our focus */
                    ODS1(   DBG_SYS, DBG_WARNING,
                            "%s now loosing focus :-( ",
                            szDevName[DEVID_HWTEST]);
                    RValue = ERR_MSG_NOT_PROCESSED;
                }
             }
        } break;
    }

    /* this part is only for FOCUSED time
       AND msg not already processed */
    if (  (HWTestDev.fFocused == TRUE )
        &&(RValue == ERR_MSG_NOT_PROCESSED ) )
    {
        switch (MsgId)
        {
            case MSG_SCREEN_RFRSH:
                HWTestDeviceShow(TRUE);     // show updated values
                RValue = ERR_MSG_PROCESSED;
                break;

            case MSG_KEY_OK:
            case MSG_KEY_UP:
            case MSG_KEY_DOWN:
                HWTestDeviceShow(TRUE);     // show updated values
                RValue = ERR_MSG_PROCESSED;
                break;

            case MSG_KEYS_PRESSED:
            case MSG_DPL_FLASH_ON:
            case MSG_DPL_FLASH_OFF:
                // no keyboard action in this device
                RValue = ERR_MSG_PROCESSED;
            default: RValue = ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       HWTestUpdateMeasurement
 *  DESCRIPTION:    update all HW measurement items to display
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void HWTestUpdateMeasurement ( void )
{
    AnaInFormatTemperature(AnaInGetTemperature(),     szIntTemp, sizeof(szIntTemp));
    AnaInFormatTemperature(AnaInGetAirTemperature(),  szExtTemp, sizeof(szExtTemp));
    AnaInFormatTemperature(AnaInGetWatTemperature(),  szWatTemp, sizeof(szWatTemp));
    AnaInFormatTemperature(AnaInGetOilTemperature(),  szOilTemp, sizeof(szOilTemp));
    AnaInFormatVoltage    (AnaInGetVoltage(),         szVoltage, sizeof(szVoltage));
    AnaInFormatVoltage    (AnaInGetAltwVoltage(),     szAltW,    sizeof(szVoltage));

    sprintf( szLDR,"%3u", AnaInGetLumiVal());
    sprintf( szKmh,"%3u", MeasGetWheelSpeed(MR_KM_PER_H));
    sprintf( szRPM,"%5u", MeasGetEngineSpeed(MR_RPM_R10));

    sprintf( szHWID, "%u", DigInDrv_GetHWVersion() );
    StatObj_HWVER.szText = szHWID;

}


/***********************************************************************
 *  FUNCTION:       HWTestWork
 *  DESCRIPTION:    update all HW test stuff, will cyclicaly called
 *                  by screen refresh
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void HWTestWork ( void )
{
    // always update measurement values
    HWTestUpdateMeasurement();

    // check errors stimulated by end-of-line test adapter (if connected)
    if ( gfEOLTest == TRUE )
        HWTestCheckStimulatedErrors();

    // check errors which can always be detected
    HWTestCheckUnstimulatedErrors();

    // show input status (if no EOL test adapter)
    if ( gfEOLTest == FALSE )
        HWTestCheckStates();
}



/***********************************************************************
 *  FUNCTION:       HWTestCheckTesterPresent
 *  DESCRIPTION:    check wether HW test adapter is present or not
 *  PARAMETER:      -
 *  RETURN:         TRUE    if test adapter present
 *                  FALSE   if not test adapter present
 *  COMMENT:        Used to distinguish, wether HW test is started at
 *                  real vehicle or with EOL test adapter (end of line).
 *
 *                  We use HBEAM as an example of all testadapter controlled
 *                  digital inputs: The testadapter returns output of PIN_GPO1 as
 *                  inverted signal at HBEAM input!
 *********************************************************************** */
BOOL HWTestCheckTesterPresent( void )
{
    int wait;
    // test: activate PIN_GPO1 and validate HBEAM for a very short time
    PIN_GPO1 = 1;                           // activate PIN_GPO1
    for (wait=0; wait<1000;wait++);
    if (DigIn_HBeam == 0)               // HBEAM should be invers of PIN_GPO1
    {
        PIN_GPO1 = 0;                       // de-activate PIN_GPO1
        for (wait=0; wait<1000;wait++);
        if (DigIn_HBeam == 1)           // HBEAM should be invers of PIN_GPO1 again
             return TRUE;
        else return FALSE;
    }
    PIN_GPO1 = 0;                           // DON'T FORGET to de-activate PIN_GPO1!
    return FALSE;
}





/***********************************************************************
 *  FUNCTION:       HWTestCheckStimulatedErrors
 *  DESCRIPTION:    check any difference between expected test adapter
 *                  results and current results
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Sould only be called if test adapter detected present
 *                  Test result in display behind item name as an
 *                      - 'E' if misfunction detected or
 *                      - '.' if function ok
 *********************************************************************** */
void HWTestCheckStimulatedErrors( void )
{
    // check V+ in 100 mV
    if (  ( AnaInGetVoltage() > 1000 )
        &&( AnaInGetVoltage() < 1400 ) )
         StatObj_VBAT.szText = szOk;
    else StatObj_VBAT.szText = szErr;

    // check AlternatorWarning voltage in 100 mV
    // Note: in test adapter connected to Vcc - V(diode) = ca. 4,2V
    if (  ( AnaInGetAltwVoltage() > 400 )
        &&( AnaInGetAltwVoltage() < 500 ) )
         StatObj_ALTW.szText = szOk;
    else StatObj_ALTW.szText = szErr;

    // check device internal temp in °C
    // Note: should give realistic production ambiant temp.
    if (  ( AnaInGetTemperature() > 5  )
        &&( AnaInGetTemperature() < 50 ) )
         StatObj_TDEV.szText = szOk;
    else StatObj_TDEV.szText = szErr;

    // check air temp in °C
    // Note: in test adapter fixed to 60°C
    if (  ( AnaInGetAirTemperature() > 55 )
        &&( AnaInGetAirTemperature() < 65 ) )
         StatObj_TAIR.szText = szOk;
    else StatObj_TAIR.szText = szErr;

    // check oil temp in °C
    // Note: in test adapter fixed to 60°C
    if (  ( AnaInGetOilTemperature() > 55 )
        &&( AnaInGetOilTemperature() < 65 ) )
         StatObj_TOIL.szText = szOk;
    else StatObj_TOIL.szText = szErr;

    // check water temp in °C
    // Note: in test adapter fixed to 60°C
    if (  ( AnaInGetWatTemperature() > 55 )
        &&( AnaInGetWatTemperature() < 65 ) )
         StatObj_TWAT.szText = szOk;
    else StatObj_TWAT.szText = szErr;

// we have a 'misterous factor 2' between MiniEmu and Not-MiniEmu-Version!
#if(MINIEMU==1)
    // check vehicle speed in km/h
    if (  (MeasGetWheelSpeed(MR_KM_PER_H) > 70 )
        &&(MeasGetWheelSpeed(MR_KM_PER_H) < 85 ) )
         StatObj_KMH.szText = szOk;
    else StatObj_KMH.szText = szErr;

    // check engine speed in km/h
    if (  (MeasGetEngineSpeed(MR_RPM_R10) > 550 )
        &&(MeasGetEngineSpeed(MR_RPM_R10) < 650 ) )
         StatObj_RPM.szText = szOk;
    else StatObj_RPM.szText = szErr;
#else
    // check vehicle speed in km/h
    if (  (MeasGetWheelSpeed(MR_KM_PER_H) > 35 )
        &&(MeasGetWheelSpeed(MR_KM_PER_H) < 40 ) )
         StatObj_KMH.szText = szOk;
    else StatObj_KMH.szText = szErr;

    // check engine speed in km/h
    if (  (MeasGetEngineSpeed(MR_RPM_R10) > 250 )
        &&(MeasGetEngineSpeed(MR_RPM_R10) < 350 ) )
         StatObj_RPM.szText = szOk;
    else StatObj_RPM.szText = szErr;
#endif

    // check TURNL/R digital input and error status
    if (DigIn_TurnL != PIN_GPO1)
         StatObj_TURNL.szText = szOk;
    else StatObj_TURNL.szText = szErr;
    if (DigIn_TurnR != PIN_GPO1)
         StatObj_TURNR.szText = szOk;
    else StatObj_TURNR.szText = szErr;

    // check NEUTR digital input and error status
    if (DigIn_Neutral != PIN_GPO1)
         StatObj_NEUTR.szText = szOk;
    else StatObj_NEUTR.szText = szErr;

    // check HBEAM digital input and error status
    if (DigIn_HBeam != PIN_GPO1)
         StatObj_HBEAM.szText = szOk;
    else StatObj_HBEAM.szText = szErr;

    // check OIL digital input and error status
    if (DigIn_OilSw != PIN_GPO1)
         StatObj_OIL.szText = szOk;
    else StatObj_OIL.szText = szErr;

    // check GPI0/1/2/3 digital input and error status
    if (DigIn_GPI_0 != PIN_GPO1)
         StatObj_GPI0.szText = szOk;
    else StatObj_GPI0.szText = szErr;
    if (DigIn_GPI_1 != PIN_GPO1)
         StatObj_GPI1.szText = szOk;
    else StatObj_GPI1.szText = szErr;
    if (DigIn_GPI_2 != PIN_GPO1)
         StatObj_GPI2.szText = szOk;
    else StatObj_GPI2.szText = szErr;
    if (DigIn_GPI_3 != PIN_GPO1)
         StatObj_GPI3.szText = szOk;
    else StatObj_GPI3.szText = szErr;

#if (MINIEMU==1) || (DEBUG==1) || (COMPASS==1)
    // do not use uart-loopback-test in this case
    // error status remains 'unknown'
#else
    // check uart0 rx pin as result of uart1 tx
    if ( Uart0_TX == Uart0_RX)
         StatObj_UART0.szText = szOk;
    else StatObj_UART0.szText = szErr;

    // check uart1 rx pin as result of uart0 tx
    if ( Uart1_TX == Uart1_RX)
         StatObj_UART1.szText = szOk;
    else StatObj_UART1.szText = szErr;
#endif

}


/***********************************************************************
 *  FUNCTION:       HWTestCheckUnstimulatedErrors
 *  DESCRIPTION:    check any detectable hw error independently of
 *                  hw test adapter.
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        test result is displayed behind item name as an
 *                      - 'E' if misfunction detected or
 *                      - '.' if function ok
 *********************************************************************** */
void HWTestCheckUnstimulatedErrors( void )
{
    #define CYCLE_CHECK_RTC     1       // check RTC    every 2 seconds
    #define CYCLE_CHECK_EEPROM  5       // check EEPROM every 5 seconds
    static UINT16   wLastRTCCheck = 0;
    static UINT16   wLastEepromCheck = 0;
    static UINT8    bOldSec;
    UINT8           bNewSec;
    UINT8           bNewMinute;
    UINT16          wRTCTimeStamp;
    CLOCKTIME       BCDTime;
    unsigned char   cTestNVRAMMinutes;
    unsigned char   cTestNVRAMSeconds;
    UINT16          wNVRAMTimeStamp;
    UINT16          wTestEEPROMAdr;
    unsigned int    wSavedEEPROMData;
    unsigned int    wTestEEPROMWord;

    // =============================================================================
    // check key UP/DOWN/Ok status (only 1 time)
    if (DigInDrv_GetKeyStates() & KEYFL_UP)
         StatObj_KEY0.szText  = szOk;
    if (DigInDrv_GetKeyStates() & KEYFL_DOWN)
         StatObj_KEY1.szText  = szOk;
    if (DigInDrv_GetKeyStates() & KEYFL_OK)
         StatObj_KEY2.szText  = szOk;

    // check LDR: any value, but valid! (<255)
    if (  ( AnaInGetLumiVal() > 0   )
        &&( AnaInGetLumiVal() < 255 ) )
         StatObj_LDR.szText = szOk;
    else StatObj_LDR.szText = szErr;

    // check WHEEL contact (at least 1 time)
    if ( WheelPort == 1 )
        StatObj_WHEEL.szText = szOk;

    // =============================================================================
    // NVRAM + RTC + Battery Check (every 2nd second)
    if ( (wSecCounter - wLastRTCCheck) > CYCLE_CHECK_RTC)
    {
        // suspend check for CYCLE_CHECK_RTC seconds
        wLastRTCCheck = wSecCounter;

        // check running RTC seconds
        // Note: Min+Sec will be used as NVRAM test pattern and battery check
        iicGetClockTime(&BCDTime);
        bNewSec    = BCD2VAL(BCDTime.seconds);
        bNewMinute = BCD2VAL(BCDTime.minutes);
        if (bOldSec != bNewSec)             // clock runs?
             StatObj_RTC.szText = szOk;
        else StatObj_RTC.szText = szErr;
        bOldSec = bNewSec;
        wRTCTimeStamp = bNewMinute*60 + bNewSec;

        // check lithium battery for NVRAM + RTC:
        // To succeed this test, the ECU must be disconnected from power supply
        //          - at least 5 seconds!
        //          - but not longer than 30 seconds!
        // Note: To distinguish between 'normal test work' (check every n seconds),
        //       timestamp difference should be >10s BUT <30s.
        // Test principle:
        //  - The NVRAM timestamp every 2 seconds updated with RTC timestamp in NVRAM test below
        //  - The NVRAM timestamp is permanentely compared with RTC timestamp
        //  - As long as both timestamps do not differ more than 5 seconds, it is assumed,
        //      that the SIXO was not switched off --> no test update!
        //  - If the the SIXO is switched off longer than 5 seconds and less than 30 seconds,
        //      and the lithium battery does'nt work, the NVRAM will loose its memory
        //      content, the timestamps will differ more than 30 seconds.
        //  - If the the SIXO is switched off more than 5 seconds and less than 30 seconds,
        //      and the lithium battery works well, the NVRAM won't loose its memory
        //      content, the timestamps will differ less than 30 seconds.
        iicNvramRead( NVRAMSIZE-1, 1, &cTestNVRAMMinutes );   // READ
        iicNvramRead( NVRAMSIZE-2, 1, &cTestNVRAMSeconds );   // READ
        wNVRAMTimeStamp = cTestNVRAMMinutes*60 + cTestNVRAMSeconds;
        if ( (wRTCTimeStamp-wNVRAMTimeStamp)>5 )             // test only if rebooted!
        {   if ((wRTCTimeStamp-wNVRAMTimeStamp)<30)
                 StatObj_BAT.szText = szOk;
            else StatObj_BAT.szText = szErr;
        }
        else {};    // no test update yet

        // check NVRAM access:
        // write/read at last NVRAM byte position
        // Note: we use RTC time stamp as testpattern to support
        //       battery check at any time (reboot by user!)
        cTestNVRAMMinutes = bNewMinute;                         // set test pattern
        cTestNVRAMSeconds = bNewSec;
        iicNvramWrite( NVRAMSIZE-1, 1, &cTestNVRAMMinutes );    // WRITE
        iicNvramWrite( NVRAMSIZE-2, 1, &cTestNVRAMSeconds );
        cTestNVRAMMinutes = 0x55;                               // clear test pattern
        cTestNVRAMSeconds = 0x55;
        iicNvramRead( NVRAMSIZE-1, 1, &cTestNVRAMMinutes );     // READ BACK
        iicNvramRead( NVRAMSIZE-2, 1, &cTestNVRAMSeconds );
        wNVRAMTimeStamp = cTestNVRAMMinutes*60 + cTestNVRAMSeconds;
        if ( wRTCTimeStamp == wNVRAMTimeStamp )
             StatObj_NVRAM.szText = szOk;
        else StatObj_NVRAM.szText = szErr;
    }

    // =============================================================================
    // EEPROM check (every 5 seconds to prevent eeprom damage)
    if ( (wSecCounter - wLastEepromCheck) > CYCLE_CHECK_EEPROM )
    {
        // suspend check for CYCLE_CHECK_EEPROM seconds
        wLastEepromCheck = wSecCounter;

        // write/read at last EEPROM word position before MagicNumber, without destroying old data
        wTestEEPROMAdr = iicEepromSize() - EEPR_MAGICNUM_SIZE - sizeof( wTestEEPROMWord );
        iicEepromRead( wTestEEPROMAdr, sizeof( wTestEEPROMWord ), (UINT8*)(&wSavedEEPROMData) );
        wTestEEPROMWord = 0xaaaa;
        iicEepromWrite(wTestEEPROMAdr, sizeof( wTestEEPROMWord ), (UINT8*)(&wTestEEPROMWord) );
        wTestEEPROMWord = 0x5555;
        iicEepromRead( wTestEEPROMAdr, sizeof( wTestEEPROMWord ), (UINT8*)(&wTestEEPROMWord) );
        iicEepromWrite(wTestEEPROMAdr, sizeof( wTestEEPROMWord ), (UINT8*)(&wSavedEEPROMData) );
        if ( wTestEEPROMWord == 0xaaaa)
             StatObj_EEPR.szText = szOk;
        else StatObj_EEPR.szText = szErr;

    }
}




/***********************************************************************
 *  FUNCTION:       HWTestCheckStates
 *  DESCRIPTION:    check current state of all digital inputs, if
 *                  port activated, shows INVERS item.
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        Should only be called if test adapter detected.
 *
 *                  Checked state is display as Test result in display behind item name as an
 *                      - '_' if input/signal is LOW and
 *                      - '' if input/signal is HIGH.
 *********************************************************************** */
void HWTestCheckStates( void )
{
    #define WHEEL_ON_DELAY  50        // just a counter to delay
    static UINT16 wWheelPortOn = 0;     // to delay the display ON time

    // check key UP status
    if (DigInDrv_GetKeyStates() & KEYFL_UP)
    {   StatObj_KEY0.eFormat = TXT_INVERS;
        KeyUp.eFormat        = TXT_INVERS;
    } else
    {   StatObj_KEY0.eFormat = TXT_NORM;
        KeyUp.eFormat        = TXT_NORM;
    }

    // check key DOWN status
    if (DigInDrv_GetKeyStates() & KEYFL_DOWN)
    {   StatObj_KEY1.eFormat = TXT_INVERS;
        KeyDown.eFormat      = TXT_INVERS;
    } else
    {   StatObj_KEY1.eFormat = TXT_NORM;
        KeyDown.eFormat      = TXT_NORM;
    }

    // check key OK status
    if (DigInDrv_GetKeyStates() & KEYFL_OK)
    {   StatObj_KEY2.eFormat = TXT_INVERS;
        KeyOk.eFormat        = TXT_INVERS;
    } else
    {   StatObj_KEY2.eFormat = TXT_NORM;
        KeyOk.eFormat        = TXT_NORM;
    }

    // check WHEEL contact status
    // This contact can NOT really be polled, because it is latched only
    // at edge trigger time (provides a ISR). So, IF we detect the contact
    // being closed, we keep it on a while to display it. :-)
    if ( WheelPort == 1 )
    {   wWheelPortOn = WHEEL_ON_DELAY;
    }
    else
    {   if (wWheelPortOn > 0)      // stop at 0!
            wWheelPortOn--;
    }
    // show ON as long as counter > 0
    if (wWheelPortOn > 0)
         StatObj_WHEEL.eFormat = TXT_INVERS;
    else StatObj_WHEEL.eFormat = TXT_NORM;

    // check TURNL digital input and error status
    if (DigIn_TurnL == 1)
         StatObj_TURNL.eFormat = TXT_INVERS;
    else StatObj_TURNL.eFormat = TXT_NORM;

    // check TURNR digital input and error status
    if (DigIn_TurnR == 1)
         StatObj_TURNR.eFormat = TXT_INVERS;
    else StatObj_TURNR.eFormat = TXT_NORM;

    // check NEUTR digital input and error status
    if (DigIn_Neutral == 1)
         StatObj_NEUTR.eFormat = TXT_INVERS;
    else StatObj_NEUTR.eFormat = TXT_NORM;

    // check HBEAM digital input and error status
    if (DigIn_HBeam == 1)
         StatObj_HBEAM.eFormat = TXT_INVERS;
    else StatObj_HBEAM.eFormat = TXT_NORM;

    // check OIL digital input and error status
    if (DigIn_OilSw == 1)
         StatObj_OIL.eFormat = TXT_INVERS;
    else StatObj_OIL.eFormat = TXT_NORM;

    // check GPI0 digital input and error status
    if (DigIn_GPI_0 == 1)
         StatObj_GPI0.eFormat = TXT_INVERS;
    else StatObj_GPI0.eFormat = TXT_NORM;

    // check GPI1 digital input and error status
    if (DigIn_GPI_1 == 1)
         StatObj_GPI1.eFormat = TXT_INVERS;
    else StatObj_GPI1.eFormat = TXT_NORM;

    // check GPI2 digital input and error status
    if (DigIn_GPI_2 == 1)
         StatObj_GPI2.eFormat = TXT_INVERS;
    else StatObj_GPI2.eFormat = TXT_NORM;

    // check GPI3 digital input and error status
    if (DigIn_GPI_3 == 1)
         StatObj_GPI3.eFormat = TXT_INVERS;
    else StatObj_GPI3.eFormat = TXT_NORM;

};



/***********************************************************************
 *  FUNCTION:       HWTestInit
 *  DESCRIPTION:    initialize all HW test stuff
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void HWTestInit ( void )
{
    /* TIMERB5 as HWTest stimulation Timer 10 Hz */
    INT_GLOB_DISABLE;

    tb5s    = FALSE;                    // stop timer b5 NOW (if running...)
    tb5mr   = 0x80;                     // timer b5 mode register: timer mode / count source: f32
    tb5ic   = 0x02;                     // timer b5 interrupt control register: interrupt level = 2 (eq. 2nd lowest)
    tb5     = (FSYS/32L/10L);           // timer b5 start value & reload register: 10/sec
    tb5s    = TRUE;                     // timer b5 start flag:  1 -> run NOW!

    INT_GLOB_ENABLE;

    // check: HW test started at vehicle or at End of line?
    gfEOLTest = HWTestCheckTesterPresent();

    // enable uart loopback test, if tester present
    // Note: we don't use uarts, we only check digital port reaction
    // Note: can not be used if DEBUG or MiniEmulator uses Uarts
#if (MINIEMU==1) || (DEBUG==1) || (COMPASS==1)
    // do not use uart-loopback-test in this case
#else
    if (gfEOLTest == TRUE)
    {
        u0c1 = 0;       // disable uart0 RX & TX
        u1c1 = 0;       // disable uart1 RX & TX

        Uart0_TX  = 0;
        Uart0D_TX = 1;
        Uart1_TX  = 0;
        Uart1D_TX = 1;
    }
#endif
};




/***********************************************************************
 *        FUNCTION:       HWTestStimuISR
 *        DESCRIPTION:    called by 10 Hz timer tb5
 *        PARAMETER:      -
 *        RETURN:         -
 *        COMMENT:        used to stimulate HW test for WHEEL + RPM
 *********************************************************************** */
#pragma INTERRUPT HWTestStimuISR
void HWTestStimuISR(void)
{
    // toggle internal flag fGPO0 with 10 Hz
    // Note: fGPO0 will be used as source for port pins
    //       only if test adapter present!
    fGPO0 = fGPO0 ? FALSE : TRUE;

    // slowly toggle internal flag fGPO1
    // Note: fGPO1 will be used as source for port pins
    //       only if test adapter present!
    if ( (wSecCounter%2)==0)
         fGPO1 = TRUE;
    else fGPO1 = FALSE;

    // HW stimulation only if HW testadapter found
    if ( gfEOLTest == TRUE )
    {
        // toggle PIN_GPO0 as WHEEL & RPM stimulation with 10 Hz
        PIN_GPO0 = (fGPO0 == TRUE) ? 1 : 0;

        // slowly toggle PIN_GPO1 for all GPIs and digital inputs every second
        PIN_GPO1 = (fGPO1 == TRUE) ? 1 : 0;

        //  toggle uart tx pins to stimualte rx (uart0/1 invers)
        Uart0_TX = (fGPO1 == TRUE) ? 1 : 0;
        Uart1_TX = (fGPO1 == TRUE) ? 1 : 0;

    }

    // single triggered tests every second
    {   static BOOL fGPO1_OldState = FALSE;
        if (fGPO1_OldState != fGPO1)
        {
            // short time toggle beeper
            DigOutDrv_SetPin( eDIGOUT_BEEP, DIGOUT_ON  );
            DigOutDrv_SetPin( eDIGOUT_BEEP, DIGOUT_OFF );

            // save state
            fGPO1_OldState = fGPO1;

            // refresh screen NOW
            {
                MESSAGE Msg;                                        /* for building messages */
                MSG_BUILD_REFRESH(Msg, DEVID_HWTEST, DEVID_HWTEST); /* from me to me */
                MsgQPostMsg(Msg, MSGQ_PRIO_LOW);                    /* post-it..*/
            }
        }
    }

    // show key status only (if tester present)
    if ( gfEOLTest == TRUE )
    if (DigInDrv_GetKeyStates() & KEYFL_UP)
         KeyUp.eFormat = TXT_INVERS;
    else KeyUp.eFormat = TXT_NORM;
    if (DigInDrv_GetKeyStates() & KEYFL_DOWN)
         KeyDown.eFormat = TXT_INVERS;
    else KeyDown.eFormat = TXT_NORM;
    if (DigInDrv_GetKeyStates() & KEYFL_OK)
         KeyOk.eFormat = TXT_INVERS;
    else KeyOk.eFormat = TXT_NORM;

    // activate beeper, if
    // - any key pressed OR
    // - if WHEEL contact closed and used in vehicle (No EOL)
    if (  (DigInDrv_GetKeyStates() & (KEYFL_DOWN | KEYFL_UP | KEYFL_OK))
        ||(  ( WheelPort == TRUE    )
           &&( gfEOLTest == FALSE ) ) )
         DigOutDrv_SetPin( eDIGOUT_BEEP, DIGOUT_ON );
    else DigOutDrv_SetPin( eDIGOUT_BEEP, DIGOUT_OFF );

    // optional disply tests (simultanous Contrast/Backlight)
    // VARY_DISPLAY: this code permanentely dims backlight from off to maximum
    //               and varies the complete contrast spectrum
    #if(VARY_DISPLAY==1)
    {
        // set LCD contrast
        static unsigned char ucTestValue = 30;

        // genarte common test value
        ucTestValue += 1;
        if (ucTestValue > 63)
            ucTestValue = 0;

        // set contrast, only useful > 30
        if (ucTestValue > 30)
             LCDDrvSetContrastLevel( ucTestValue );
        else LCDDrvSetContrastLevel( 30 );

        // set backlight
        LCDDrvSetBacklightLevel( TRUE, ucTestValue );
    }
    #else
        // level fixed here, because dynamic behavour (on/off) is not required
        LCDDrvSetBacklightLevel( TRUE, 63 );
    #endif // VARY_DISPLAY

    // activate LEDs as pairs (controlled by PIN_GPO1)
    if (fGPO1 == 1)
    {   LEDDrvSetLED(LEDDRV_NEUTR, TRUE);
        LEDDrvSetLED(LEDDRV_TURN,  FALSE);
        LEDDrvSetLED(LEDDRV_INFO,  TRUE);
        LEDDrvSetLED(LEDDRV_BEAM,  FALSE);
        LEDDrvSetLED(LEDDRV_WARN,  TRUE);
        LEDDrvSetLED(LEDDRV_ERR,   FALSE);
    }
    else
    {   LEDDrvSetLED(LEDDRV_NEUTR, FALSE);
        LEDDrvSetLED(LEDDRV_TURN,  TRUE);
        LEDDrvSetLED(LEDDRV_INFO,   FALSE);
        LEDDrvSetLED(LEDDRV_BEAM,  TRUE);
        LEDDrvSetLED(LEDDRV_WARN,  FALSE);
        LEDDrvSetLED(LEDDRV_ERR,   TRUE);
    }

    // addionally toggle PIN_GPO0 & PIN_GPO1 - if NO tester present!
    if ( gfEOLTest == FALSE )
    {
        if (fGPO1 == 1)
        {   PIN_GPO0 = 1;
            PIN_GPO1 = 0;
        }
        else
        {   PIN_GPO0 = 0;
            PIN_GPO1 = 1;
        }
    }
}
