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
 *  Created:        22-03-2002 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         main devices
 *  Purpose:        handles the main screen (speed & rpm) device
 *  Comments:       -
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
#include "measure.h"
#include "sysparam.h"
#include "beep.h"
#include "led.h"



/* defines for device data */
#define SPEED_STRSIZE   6               /* to contain max. string '333,3' km/h */
#define RPM_STRSIZE     6               /* to contain max. string '13500' U/Min */


/* main device states
   defines, which object to be shown under
   line of tachometer */
typedef enum
{
    eMainFirst,         /* invalid state*/
    eMainRPM,           /* show engine speed RPM */
    eMainFuelKm,        /* show fuel distance */
    eMainVehKm,         /* show vehicle overall distance */
    // eMainClock
    eMainLast,          /* invalid state*/
} MAINDEVSTATE;



/* external symbols */
extern UINT16            wMilliSecCounter;          /* valid values: 0h .. ffffh */
extern STRING far        szDevName[];               /* device names */
extern SYSFLAGS_TYPE     gSystemFlags;              /* system parameters */
extern unsigned char far rgEnduroSymbol29x16[];     /* enduro symbol  */
extern unsigned char far rgFuelSymbo16x16[];        /* fuel symbol */


/* device static objects */
static DEVDATA      MainDevice;                 /* this device */

static TEXTOBJECT   SpeedTxtObj;                /* speed text object */
static TEXTOBJECT   SpeedDescTxtObj;            /* speed descriptor 'km/h' text object */
static CHAR         szSpeed[6] = "0,0";         /* buffer for current speed, max. string '333,3' */

static TEXTOBJECT   RPMTxtObj;                  /* rpm text object */
static TEXTOBJECT   RPMDescTxtObj;              /* rpm descriptor 'U/Min' text object */
static CHAR         szRPM[6] = "0";             /* buffer current eng speed, max. string '13500' */

static TEXTOBJECT   FuelDistTxtObj;             /* fuel distance text object */
static CHAR         szFuelDist[10] = "0,0";     /* buffer to contain fuel distance, max. string '9999999,9' */
static BMPOBJECT    FuelSymbolBmpObj;           /* symbol to indicate fuel distance display mode */

//static EDITNUMBEROBJECT VehDistEditObj;         /* editable vehicle distance */
//static DIST_TYPE    dwVehicDist;                /* local copy for updates */
//static DIST_TYPE    dwVehicDistWork;            /* local copy for edit work */
static TEXTOBJECT   VehDistTxtObj;              /* vehicle distance text object */
static CHAR         szVehDist[10] = "0,0";      /* buffer to contain fuel distance, max. string '9999999,9' */
static BMPOBJECT    VehicSymbolBmpObj;          /* symbol to indicate vehicle distance display mode */

static TEXTOBJECT   DistDescTxtObj;             /* vehicle & fuel distance decriptor for 'km' */

/* internal prototypes */
ERRCODE MainDeviceStateMachine(MESSAGE Msg);
ERRCODE MainDeviceVehicDistMsg(MESSAGE Msg);
ERRCODE MainDeviceFuelDistMsg(MESSAGE Msg);


/***********************************************************************
 *  FUNCTION:       MainDeviceInit
 *  DESCRIPTION:    all initial stuff for all objects
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MainDeviceInit(void)
{
    /* device main data */
    MainDevice.eDevID       = DEVID_MAIN;
    MainDevice.szDevName    = szDevName[DEVID_MAIN];
    MainDevice.fFocused     = FALSE;
    MainDevice.fScreenInit  = FALSE;
    MainDevice.wDevState    = gSystemFlags.flags.MainDevState;

    /* speed --------------------------- */
    ObjTextInit(    &SpeedTxtObj,                   /* speed value object */
                    8,  0, DPLFONT_24X32,
                    1, 3, TXT_RIGHT, TXT_NORM,
                    szSpeed,
                    OC_DISPL );
    ObjTextInit(    &SpeedDescTxtObj,               /* speed descriptor 'km/h' */
                    90, 8, DPLFONT_8X16,
                    1, 4, TXT_LEFT, TXT_NORM,
                    RESTXT_SPEED_DESC,
                    OC_DISPL );

    /* rpm --------------------------- */
    ObjTextInit(    &RPMTxtObj,                     /* rpm value object */
                    8, 39, DPLFONT_14X16,
                    1, 5, TXT_RIGHT, TXT_NORM,
                    szRPM,
                    OC_DISPL );
    ObjTextInit(    &RPMDescTxtObj,                 /* rpm descriptor 'U/Min' */
                    86, 39, DPLFONT_8X16,
                    1, 5, TXT_LEFT, TXT_NORM,
                    RESTXT_RPM_DESC,
                    OC_DISPL );

    /* fuel distance ----------------- */
    ObjTextInit(    &FuelDistTxtObj,                /* fuel distance value */
                    24, 39, DPLFONT_14X16,
                    1, 6, TXT_RIGHT, TXT_NORM,
                    szFuelDist,
                    OC_DISPL );
    ObjBmpInit(     &FuelSymbolBmpObj,
                    4, 38, 16, 16,
                    rgFuelSymbo16x16,
                    DPLNORM, FALSE);

    /* vehicle distance ----------------- */
    ObjTextInit(    &VehDistTxtObj,                 // vehicle distance value
                    46, 39, DPLFONT_8X16,
                    1, 6, TXT_RIGHT, TXT_NORM,
                    szVehDist,
                    OC_DISPL );

    /*ObjEditNumInit( &VehDistEditObj,
                     44, 39, DPLFONT_8X16, 7,       // wOrgPosX, wOrgPosY, eFont, bWindWidth
                     &dwVehicDist,                  // original value
                     &dwVehicDistWork,              // buffer for edit
                     eULONG,                        // number type
                     0L, 999999L, 1L,               // lMin, lMax, lStepSize
                     eDez, eColumn, 3,              // eDplType, eMode, bComma-Pos
                     "", "", 7,                     // szDescr, szUnit, bLength,
                     OC_DISPL | OC_EDIT | OS_DISPL);// object state */

    ObjBmpInit(     &VehicSymbolBmpObj,
                    0, 38, 29, 16,
                    rgEnduroSymbol29x16,
                    DPLNORM, FALSE);

    /* distance descriptor -------------- */
    ObjTextInit(    &DistDescTxtObj,                /* distance descriptor 'km' (for fuel & vehicle) */
                    106, 39, DPLFONT_8X16,
                    1, 2, TXT_LEFT, TXT_NORM,
                    RESTXT_DIST_DESC,
                    OC_DISPL );

    /* return */
    return ERR_OK;
}

/***********************************************************************
 *  FUNCTION:       MainDeviceShow
 *  DESCRIPTION:    bring updated main device to display
 *                  by calling Show-Fct. of all relevant objects
 *  PARAMETER:      BOOL    TRUE = show objects, FALSE = clear screen
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void MainDeviceShow(BOOL fShow)
{
    ERRCODE error = ERR_OK;
    MESSAGE NewMsg;             /* for screen fresh message */

    /* 'show' or 'clear' screen? */
    if (fShow == TRUE)
    {
        /* update of measurement values */
        sprintf( szSpeed,    "%3u", MeasGetWheelSpeed(MR_KM_PER_H));
        sprintf( szRPM,      "%5u", MeasGetEngineSpeed(MR_RPM_R10));
        sprintf( szFuelDist, "%4u,%1u", MeasGetFuelDist(MR_KM_ONLY), MeasGetFuelDist(MR_HM_ONLY));
        sprintf( szVehDist,  "%06lu", MeasGetVehicDist(MR_KM));
        //dwVehicDist = MeasGetVehicDist(MR_KM);

        /* do we have to repaint all? */
        if (MainDevice.fScreenInit == FALSE)
        {
            /* yes, repaint complete screen */
            DisplClearScreen(0x0);

            /* horizontal line between wheel speed & rpm */
            {
                DISPLXY Coord = {0,34};                 /* to be removed to an 'LineObject' !!! */
                DisplDrawHorLine(&Coord, 128, 0x03);
            }

            /* show always vehicle speed in upper display part */
            ObjTextShow( &SpeedTxtObj );
            ObjTextShow( &SpeedDescTxtObj );

            /* which data to be shown below speed? */
            switch (MainDevice.wDevState)
            {
                case eMainRPM:
                    ObjTextShow( &RPMTxtObj );
                    ObjTextShow( &RPMDescTxtObj );
                break;
                case eMainFuelKm:
                    ObjTextShow( &FuelDistTxtObj );
                    ObjTextShow( &DistDescTxtObj );
                    ObjBmpShow( &FuelSymbolBmpObj );
                break;
                case eMainVehKm:
                    ObjTextShow( &VehDistTxtObj );
                    //ObjEditNumShow( &VehDistEditObj, SHOW_ALL );
                    ObjTextShow( &DistDescTxtObj );
                    ObjBmpShow( &VehicSymbolBmpObj );
                break;
                // case eMainClock:
                // break;
                default:
                    ODS1( DBG_SYS, DBG_ERROR,
                          "MainDeviceShow(): unknown state: %u", MainDevice.wDevState);
            }
            MainDevice.fScreenInit  = TRUE;
        }
        else
        {
            /* No, repaint only changed stuff */
            ObjTextShow( &SpeedTxtObj );

            /* which data to be shown below speed? */
            switch (MainDevice.wDevState)
            {
                case eMainRPM:
                    ObjTextShow( &RPMTxtObj );
                break;
                case eMainFuelKm:
                    ObjTextShow( &FuelDistTxtObj );
                break;
                case eMainVehKm:
                    ObjTextShow( &VehDistTxtObj );
                    //ObjEditNumShow( &VehDistEditObj, SHOW_EDIT | SHOW_CURSOR );
                break;
                // case eMainClock:
                // break;
                default:
                    ODS1( DBG_SYS, DBG_ERROR,
                          "MainDeviceShow(): unknown state: %u", MainDevice.wDevState);
            }
        }
    }
    else
    {
        DisplClearScreen(0x0);              /* leave a clear screen for next device */
        MainDevice.fScreenInit = FALSE;     /* reset devices screen state */
    }
}


/***********************************************************************
 *  FUNCTION:       MainDeviceMsgEntry
 *  DESCRIPTION:    Receive Message Handler called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MainDeviceMsgEntry(MESSAGE GivenMsg)
{
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID  MsgId;
    MESSAGE     NewMsg;


    /* analyze message: it's for us? */
    MsgId = MSG_ID(GivenMsg);        /* get message id */
    switch (MsgId)
    {
        case MSG_GET_FOCUS:
        {
            /*  Some device want's to get the focus:
                If we've currently got the focus, we'll
                answer to SET his focus! */
            if ( MainDevice.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_MAIN]);
                MSG_BUILD_SETFOCUS(NewMsg,DEVID_MAIN,MSG_SENDER_ID(GivenMsg));   /* build answer message */
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                     /* send answer message */
                MainDevice.fFocused = FALSE;                                     /* clear our focus */
                MainDeviceShow(FALSE);                                           /* clear our screen */
                RValue = ERR_MSG_PROCESSED;
            }
        } break;
        case MSG_SET_FOCUS:
        {
            /* messsage for us? */
            if (MSG_RECEIVER_ID(GivenMsg) == DEVID_MAIN)
            {
                /*  Someone wants us to take the focus?
                    We assume, that nobody else has the focus
                    and we've got the the screen now for us! */
                if (  (MainDevice.fFocused       == TRUE             )
                    &&(MSG_RECEIVER_ID(GivenMsg) == DEVID_MAIN) )
                    ODS2(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but it already has focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_MAIN]);
                else /*
                    ODS2(   DBG_SYS, DBG_INFO,
                            "FOCUS: %s -> %s!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_MAIN]) */;
                MainDevice.fFocused = TRUE;                             /* set our focus */
                MainDeviceShow(TRUE);                                   /* show our screen */
                gSystemFlags.flags.ActDevNr = DEVID_MAIN;               /* save device# for restore */
                RValue = ERR_MSG_PROCESSED;
             }
             else
             {
                if ( MainDevice.fFocused == TRUE )
                {
                    /*  Some other device should be given the focus,
                        BUT WE'VE GOT THE FOCUS!
                        It should first ask US to give the focus! :-( */
                    ODS3(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but %s had focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[MSG_RECEIVER_ID(GivenMsg)],
                            szDevName[DEVID_MAIN]);
                    MainDevice.fFocused = FALSE;                        /* loose our focus */
                    ODS1(   DBG_SYS, DBG_WARNING,
                            "%s now loosing focus :-( ",
                            szDevName[DEVID_MAIN]);
                    RValue = ERR_MSG_NOT_PROCESSED;
                }
             }
        } break;
    }

    /* this part is only for FOCUSED time
       AND msg not already processed */
    if (  (MainDevice.fFocused == TRUE )
        &&(RValue == ERR_MSG_NOT_PROCESSED ) )
    {
        switch (MsgId)
        {
            case MSG_KEY_UP:
            case MSG_KEY_DOWN:
            case MSG_KEY_OK:
            case MSG_DPL_FLASH_ON:
            case MSG_DPL_FLASH_OFF:
            case MSG_KEYS_PRESSED:

                //if (RValue == ERR_MSG_NOT_PROCESSED)
                //    RValue = MainDeviceVehicDistMsg(GivenMsg);    /* try vehicle distance */

                if (RValue == ERR_MSG_NOT_PROCESSED)
                    MainDeviceFuelDistMsg(GivenMsg);                /* try vehicle distance */

                if (RValue == ERR_MSG_NOT_PROCESSED)
                    RValue = MainDeviceStateMachine(GivenMsg);      /* try state machine */

                if (  (RValue == ERR_MSG_NOT_PROCESSED                    ) /* check if UP&DOWN are pressed short */
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN))
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYSHORT              ) )
                {
                    /* give focus immediatly to next screen */
                    MainDevice.fFocused = FALSE;                              /* clear our focus */
                    MainDeviceShow(FALSE);                                    /* clear our screen */
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_MAIN, DEVID_TRIPCOUNT);  /* next: TripCounter */
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_SCREEN_REFRESH:
                MainDeviceShow(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;
            default: RValue = ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}





/***********************************************************************
 *  FUNCTION:       MainDeviceVehicDistMsg
 *  DESCRIPTION:    Handle the vehicle distance msg entries
 *  PARAMETER:      MESSAGE
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
#ifdef VEHICEDIT_POSSIBLE
ERRCODE MainDeviceVehicDistMsg(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                            /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    static BOOL fEditActive = FALSE;

    /* vehicle distance in focus? */
    if (MainDevice.wDevState == eMainVehKm)
    {
        /* user presses OK Button > 2 sec: focus & start edit */
        if (  (MsgId == MSG_KEY_OK             )                /* [OK] */
            &&(MSG_KEY_DURATION(Msg) > KEYSAVE ) )              /* pressed 'long'? */
            VehDistEditObj.State.bits.fSelected = TRUE;         /* simulate invisible focus */

        /* give it to object msg entry */
        RValue = ObjEditNumMsgEntry( &VehDistEditObj, Msg );    /* process msg */

        /* check: started edit mode? */
        if (VehDistEditObj.State.bits.fEditActive == TRUE)      /* recognize edit active */
            fEditActive = TRUE;

        /* check: ended edit mode? */
        if (  (VehDistEditObj.State.bits.fEditActive == FALSE)  /* recognize edit active */
            &&(fEditActive == TRUE                           ) )
        {
            dwVehicDist.dkm *= 100L;                            /* rescale to complete range */
            MeasSetVehicDist(&dwVehicDist);                     /* SAVE new value */
            VehDistEditObj.State.bits.fSelected = FALSE;        /* simulate invisible focus */
            fEditActive = FALSE;                                /* reset state */
        }
    }
    else
    {   /* reset state */
        VehDistEditObj.State.bits.fSelected = FALSE;           /* simulate invisible focus */
        fEditActive = FALSE;
    }
    return RValue;
}
#endif // VEHICEDIT_POSSIBLE

/***********************************************************************
 *  FUNCTION:       MainDeviceStateMachine
 *  DESCRIPTION:    Handle the display states of main device
 *                  and re-init the display state if any changes
 *  PARAMETER:      MESSAGE
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MainDeviceStateMachine(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                            /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;

    /* scroll up? */
    if (  (RValue == ERR_MSG_NOT_PROCESSED             )        /* still unpocessed */
        &&(MsgId  == MSG_KEY_UP                        )        /* [UP] */
        &&(MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED ) )      /* not released */
    {
        MainDevice.wDevState--;                                 /* previous state */
        if (MainDevice.wDevState == eMainFirst)                 /* wrap around? */
            MainDevice.wDevState = (eMainLast-1);
        MainDevice.fScreenInit   = FALSE;                       /* next time rebuild complete screen */
        MainDeviceShow(TRUE);
        RValue = ERR_MSG_PROCESSED;
        ODS1( DBG_SYS, DBG_INFO, "MainDevState: %u", MainDevice.wDevState);
    }

    /* scroll down? */
    if (  (RValue == ERR_MSG_NOT_PROCESSED             )        /* still unpocessed */
        &&(MsgId  == MSG_KEY_DOWN                      )        /* [DOWN] */
        &&(MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED ) )      /* not released */
    {
        MainDevice.wDevState++;                                 /* next state */
        if (MainDevice.wDevState == eMainLast)                  /* wrap around? */
            MainDevice.wDevState = (eMainFirst+1);
        MainDevice.fScreenInit   = FALSE;                       /* next time rebuild complete screen */
        MainDeviceShow(TRUE);
        RValue = ERR_MSG_PROCESSED;
        ODS1( DBG_SYS, DBG_INFO, "MainDevState: %u", MainDevice.wDevState);
    }

    /* save device state */
    gSystemFlags.flags.MainDevState = MainDevice.wDevState;

    return RValue;
}



/***********************************************************************
 *  FUNCTION:       MainDeviceFuelDistMsg
 *  DESCRIPTION:    Handle the fuel distance msg entries
 *  PARAMETER:      MESSAGE
 *  RETURN:         ERR_MSG_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE MainDeviceFuelDistMsg(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                            /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;

    /* vehicle distance in focus? */
    if (MainDevice.wDevState == eMainFuelKm)
    {
        /* user presses OK Button > 2 sec: focus & start edit */
        if (  (MsgId == MSG_KEY_OK             )                /* [OK] */
            &&(MSG_KEY_DURATION(Msg) > KEYSAVE ) )              /* pressed 'long'? */
        {
            DIST_TYPE Dist;
            Dist.dkm = 0;               /* reset distance value */
            MeasSetFuelDist( &Dist );   /* set into memory */
            BeepOk();                   /* beep ok */
            LEDOk();                    /* LED ok */
        }
    }
}



