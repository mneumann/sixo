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
 *  Module:         settings device
 *  Purpose:        handles the settings screen device
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
#include "displdrv.h"

/* defines for device data */


/* device states */
typedef enum
{
    eSetFirst,          /* INVALID STATE */
    eSetWheelsize,      /* wheel size */
    eSetCCFNom,         /* CCF nominator */
    eSetCCFDenom,       /* CCF denominator */
    eSetDisplLight,     /* display backlight */
    eSetDisplContr,     /* display contrast */
    eSetVehicDist,      /* vehicle distance */
    eSetLast,           /* INVALID STATE */
} SETDEVSTATE;


/* device object data */
static DEVDATA          SetDevice;                                  /* settingss device object itself */
extern STRING far       szDevName[];                                /* device names */
extern SYSFLAGS_TYPE    gSystemFlags;                               /* system parameters */

/*  Darstellungs-Demo:  +----.----.----.----.-+
    (max. 21 Zeichen)   |Radumfang:    2200 mm|
                        |Zyl.-Fakt.:    1/1   |
                        |Licht:           0   |
                        |Kontrast:      255   |
                        |Gesamt:    112.500 km|
                        +----.----.----.----.-+
*/

/* wheelsize object 'Radumfang:   2200 mm' */
extern UINT16           wWheelSize;                                 /* original wheel size from eeprom in mm */
static UINT16           wWheelSizeCopy;                             /* copy value to edit */
static EDITNUMBEROBJECT EditWheelSizeObj;                           /* complete number object */
static CHAR             szWheelSizeDescr[] = RESTXT_WHEELSIZEDESC;  /* descriptor string */
static CHAR             szWheelSizeUnits[] = RESTXT_WHEELSIZEUNIT;  /* units string */

/* cylinder correcture factor: 'Zyl.-Fakt.: 1/1' */
extern CCF_TYPE         CCF;                                        /* original CCF value (with Nom/Denom nibbles) from eeprom */
static UINT8            CCFNom;                                     /* copy of CCF.Nom as reference */
static UINT8            CCFNomCopy;                                 /* copy of CCF.Nom to work with */
static EDITNUMBEROBJECT EditCCFNomObj;                              /* complete nominator number object */
static CHAR             szCCFNomDescr[] = RESTXT_CCFNOMDESC;        /* descriptor string */
static CHAR             szCCFNomUnits[] = RESTXT_CCFNOMUNIT;        /* units string (here: '/') */
static UINT8            CCFDenom;                                   /* copy of CCF.Denom as reference */
static UINT8            CCFDenomCopy;                               /* copy of CCF.Denom to work with */
static EDITNUMBEROBJECT EditCCFDenomObj;                            /* complete denominator number object */

/* Display Light: 'Licht: 0' */
extern DPLFLAGS_TYPE    gDisplayFlags;                              /* orginal display values */
static BOOL             fBackl;                                     /* copy of gDisplayFlags.flags.fBackl as reference */
static BOOL             fBacklCopy;                                 /* copy of gDisplayFlags.flags.fBackl to work with */
static EDITNUMBEROBJECT EditBacklObj;                               /* complete Backlight object */
static CHAR             szBacklDescr[] = RESTXT_DPLLIGHTDESC;       /* descriptor string */

/* Display Contrast: 'Kontrast: 255' */
static UINT8            bContrLev;                                  /* copy of gDisplayFlags.flags.ContLev as reference */
static UINT8            bContrLevCopy;                              /* copy of gDisplayFlags.flags.ContLev to work with */
static EDITNUMBEROBJECT EditContrLevObj;                            /* complete Contrast level object */
static CHAR             szContrLevDescr[] = RESTXT_DPLCONTRDESC;    /* descriptor string */

/* vehicle distance object 'Gesamt: 112.500 km' */
static DIST_TYPE        VehicDist;                                  /* to get/set original value */
static UINT32           dwVehicDist;                                /* original vehicle distance in km only */
static UINT32           dwVehicDistCopy;                            /* copy value to edit */
static EDITNUMBEROBJECT EditVehicDistObj;                           /* complete number object */
static CHAR             szVehicDistDescr[] = RESTXT_VEHICKMDESC;    /* descriptor string */
static CHAR             szVehicDistUnits[] = RESTXT_VEHICKMUNIT;    /* units string */




/* other external symbols */
extern UINT16           wMilliSecCounter;                           /* valid values: 0h .. ffffh */



/* non public prototypes */
void    SetDeviceShow(BOOL fShow);
ERRCODE SetDeviceTryObjects(MESSAGE GivenMsg);
ERRCODE SetDeviceStateMachine(MESSAGE Msg);
void    SetDeviceCheckChanges(void);
ERRCODE SetDeviceSetFocus(SETDEVSTATE eState);




/***********************************************************************
 *  FUNCTION:       SetDeviceInit
 *  DESCRIPTION:    all initial stuff for all objects
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE SetDeviceInit(void)
{
    /* device main data */
    SetDevice.eDevID       = DEVID_SET;
    SetDevice.szDevName    = szDevName[DEVID_SET];
    SetDevice.fFocused     = FALSE;
    SetDevice.fScreenInit  = FALSE;
    SetDevice.wDevState    = eSetWheelsize;

    /* ---------------------------------- */
    /* get local copies of global unions to handle EditNumObjects */
    CCFNom      = CCF.nibble.nom;
    CCFDenom    = CCF.nibble.denom;
    fBackl      = gDisplayFlags.flags.fBackl;
    bContrLev   = gDisplayFlags.flags.ContrLev;

    /* init wheel size object */
    ObjEditNumInit( &EditWheelSizeObj,      // object to be initilized
                    0, 0,                   // X / Y origin
                    DPLFONT_6X8,            // used font
                    21,                     // overall window width in chars
                    &wWheelSize,            // pointer to original value
                    &wWheelSizeCopy,        // pointer to work copy buffer
                    eUINT,0L, 9999L, 0L,    // type of number, min, max Limits, stepsize
                    eDez, eColumn, 0,       // dez display format, column edit mode, comma pos
                    szWheelSizeDescr,       // descriptor string
                    szWheelSizeUnits,       // units string
                    4,                      // edit field length in chars
                    OC_DISPL | OC_SELECT | OC_EDIT | OS_SELECT ); // init states

    /* init CCF - nominator object */
    ObjEditNumInit( &EditCCFNomObj,         // object to be initilized
                    0, 9,                  // X / Y origin
                    DPLFONT_6X8,            // used font
                    17,                     // overall window width in chars
                    &CCFNom,                // pointer to original value
                    &CCFNomCopy,            // pointer to work copy buffer
                    eUCHAR,1L, 9L, 0L,      // type of number, min, max Limits, stepsize
                    eDez, eColumn, 0,       // dez display format, column edit mode, comma pos
                    szCCFNomDescr,          // descriptor string
                    szCCFNomUnits,          // units string
                    1,                      // edit field length in chars
                    OC_DISPL | OC_SELECT | OC_EDIT ); // init states

    /* init CCF - denominator object */
    ObjEditNumInit( &EditCCFDenomObj,       // object to be initilized
                    102, 9,                 // X / Y origin (x = 16x6 pixel)
                    DPLFONT_6X8,            // used font
                    1,                      // overall window width in chars
                    &CCFDenom,              // pointer to original value
                    &CCFDenomCopy,          // pointer to work copy buffer
                    eUCHAR,1L, 9L, 0L,      // type of number, min, max Limits, stepsize
                    eDez, eColumn, 0,       // dez display format, column edit mode, comma pos
                    "",                     // descriptor string
                    "",                     // units string
                    1,                      // edit field length in chars
                    OC_DISPL | OC_SELECT | OC_EDIT ); // init states

    /* init backlight on/off object */
    ObjEditNumInit( &EditBacklObj,          // object to be initilized
                    0, 18,                  // X / Y origin
                    DPLFONT_6X8,            // used font
                    18,                     // overall window width in chars
                    &fBackl,                // pointer to original value
                    &fBacklCopy,            // pointer to work copy buffer
                    eUCHAR,0L, 1L, 0L,      // type of number, min, max Limits, stepsize
                    eDez, eColumn, 0,       // dez display format, column edit mode, comma pos
                    szBacklDescr,           // descriptor string
                    "",                     // units string
                    1,                      // edit field length in chars
                    OC_DISPL | OC_SELECT | OC_EDIT ); // init states

    /* init contrast level object */
    ObjEditNumInit( &EditContrLevObj,       // object to be initilized
                    0, 27,                  // X / Y origin
                    DPLFONT_6X8,            // used font
                    18,                     // overall window width in chars
                    &bContrLev,             // pointer to original value
                    &bContrLevCopy,         // pointer to work copy buffer
                    eUCHAR,0L, 255L, 0L,    // type of number, min, max Limits, stepsize
                    eDez, eColumn, 0,       // dez display format, column edit mode, comma pos
                    szContrLevDescr,        // descriptor string
                    "",                     // units string
                    3,                      // edit field length in chars
                    OC_DISPL | OC_SELECT | OC_EDIT ); // init states

    /* init vehicle distance object */
    ObjEditNumInit( &EditVehicDistObj,      // object to be initilized
                    0, 36,                  // X / Y origin
                    DPLFONT_6X8,            // used font
                    21,                     // overall window width in chars
                    &dwVehicDist,           // pointer to original value
                    &dwVehicDistCopy,       // pointer to work copy buffer
                    eULONG,0L, 999999L, 0L, // type of number, min, max Limits, stepsize
                    eDez, eColumn, 3,       // dez display format, column edit mode, comma pos
                    szVehicDistDescr,       // descriptor string
                    szVehicDistUnits,       // units string
                    7,                      // edit field length in chars
                    OC_DISPL | OC_SELECT | OC_EDIT ); // init states

    /* return */
    return ERR_OK;
}

/***********************************************************************
 *  FUNCTION:       SetDeviceShow
 *  DESCRIPTION:    update devices appearence (new/update/clear)
 *                  by calling Show-Fct. of all relevant objects
 *  PARAMETER:      BOOL    TRUE = show objects, FALSE = clear screen
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SetDeviceShow(BOOL fShow)
{
    ERRCODE error = ERR_OK;
    MESSAGE NewMsg;                                         /* for screen fresh message */

    /* show mode of complete device */
    if (fShow == TRUE)                                      /* 'show' screen ?*/
    {
        /* refresh dynamic values (only if not in edit mode) */
        if (EditVehicDistObj.State.bits.fEditActive == FALSE)
        {
            VehicDist = MeasGetVehicDist(MR_KM);            /* get fresh value */
            dwVehicDist = VehicDist.km;                     /* get km only */
        }


        if (SetDevice.fScreenInit == FALSE)                 /* do we have to repaint all? */
        {
            /* yes, repaint complete screen */
            ObjEditNumShow( &EditWheelSizeObj, SHOW_ALL );  /* show wheelsize object completly new */
            ObjEditNumShow( &EditCCFNomObj, SHOW_ALL);      /* show CCF decriptor & nominator & '/'*/
            ObjEditNumShow( &EditCCFDenomObj, SHOW_ALL);    /* show CCF denominator */
            ObjEditNumShow( &EditBacklObj, SHOW_ALL);       /* show backlight on/off */
            ObjEditNumShow( &EditVehicDistObj, SHOW_ALL);   /* show vehicle distance */
            ObjEditNumShow( &EditContrLevObj, SHOW_ALL);    /* show contrast level */
            SetDevice.fScreenInit = TRUE;                   /* reset init state */
        }
        else
        {
            /* No, repaint only changed stuff */
            ObjEditNumShow( &EditWheelSizeObj, SHOW_EDIT | SHOW_CURSOR );
            ObjEditNumShow( &EditCCFNomObj, SHOW_EDIT | SHOW_CURSOR );
            ObjEditNumShow( &EditCCFDenomObj, SHOW_EDIT | SHOW_CURSOR );
            ObjEditNumShow( &EditBacklObj, SHOW_EDIT | SHOW_CURSOR);
            ObjEditNumShow( &EditVehicDistObj, SHOW_EDIT | SHOW_CURSOR);
            ObjEditNumShow( &EditContrLevObj, SHOW_EDIT | SHOW_CURSOR);
        }
    }
    else                                                    /* 'clear' screen! */
    {
        DisplClearScreen(0x0);                              /* leave a clear screen for next device */
        SetDevice.fScreenInit = FALSE;                      /* reset devices screen state */
    }
}


/***********************************************************************
 *  FUNCTION:       SetDeviceMsgEntry
 *  DESCRIPTION:    Receive Message Handler called by MsgQPump
 *  PARAMETER:      msg
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_NOT_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE SetDeviceMsgEntry(MESSAGE GivenMsg)
{
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;
    MESSAGE_ID  MsgId;
    MESSAGE     NewMsg;


    /* analyze message: it's for us? */
    MsgId = MSG_ID(GivenMsg);                                                   /* get message id */
    switch (MsgId)
    {
        case MSG_GET_FOCUS:
        {
            /*  Some device want's to get the focus:
                If we've currently got the focus, we'll
                answer to SET his focus! */
            if ( SetDevice.fFocused == TRUE)
            {
                ODS2(   DBG_SYS, DBG_INFO,
                        "%s wants to have focus from %s!",
                        szDevName[MSG_SENDER_ID(GivenMsg)],
                        szDevName[DEVID_SET]);
                MSG_BUILD_SETFOCUS(NewMsg,DEVID_SET,MSG_SENDER_ID(GivenMsg));   /* build answer message */
                RValue = MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);                    /* send answer message */
                SetDevice.fFocused = FALSE;                                     /* clear our focus */
                SetDeviceShow(FALSE);                                           /* clear our screen */
                RValue = ERR_MSG_PROCESSED;
            }
        } break;
        case MSG_SET_FOCUS:
        {
            /* messsage for us? */
            if (MSG_RECEIVER_ID(GivenMsg) == DEVID_SET)
            {
                /*  Someone wants us to take the focus?
                    We assume, that nobody else has the focus
                    and we've got the the screen now for us! */
                if (  (SetDevice.fFocused        == TRUE      )
                    &&(MSG_RECEIVER_ID(GivenMsg) == DEVID_SET) )
                    ODS2(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but it already has focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_SET]);
                else /*
                    ODS2(   DBG_SYS, DBG_INFO,
                            "FOCUS: %s -> %s!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[DEVID_SET]) */ ;
                SetDevice.fFocused = TRUE;                              /* set our focus */
                SetDeviceShow(TRUE);                                    /* show our screen immediatly */
                gSystemFlags.flags.ActDevNr = DEVID_SET;                /* save device# for restore */
                RValue = ERR_MSG_PROCESSED;
             }
             else
             {
                if ( SetDevice.fFocused == TRUE )
                {
                    /*  Some other device should be given the focus,
                        BUT WE'VE GOT THE FOCUS!
                        It should first ask US to give the focus! :-( */
                    ODS3(   DBG_SYS, DBG_WARNING,
                            "FOCUS: %s -> %s, but %s had focus!",
                            szDevName[MSG_SENDER_ID(GivenMsg)],
                            szDevName[MSG_RECEIVER_ID(GivenMsg)],
                            szDevName[DEVID_SET]);
                    SetDevice.fFocused = FALSE;                        /* loose our focus */
                    ODS1(   DBG_SYS, DBG_WARNING,
                            "%s now loosing focus :-( ",
                            szDevName[DEVID_SET]);
                    RValue = ERR_MSG_NOT_PROCESSED;
                }
             }
        } break;
    }

    /* ---------------------------------------------------------------- */
    /* this part is only for FOCUSED time AND msg not already processed */
    if (  (SetDevice.fFocused == TRUE )
        &&(RValue == ERR_MSG_NOT_PROCESSED ) )
    {
        switch (MsgId)
        {
            case MSG_KEYS_PRESSED:
            case MSG_KEY_OK:
            case MSG_KEY_UP:
            case MSG_KEY_DOWN:
            case MSG_DPL_FLASH_ON:
            case MSG_DPL_FLASH_OFF:
                if( RValue == ERR_MSG_NOT_PROCESSED )
                    RValue = SetDeviceTryObjects(GivenMsg);                 /* try all objects */
                if( RValue == ERR_MSG_NOT_PROCESSED )
                    RValue = SetDeviceStateMachine(GivenMsg);               /* try move focus */
                if (  (RValue == ERR_MSG_NOT_PROCESSED                    )
                    &&(MSG_KEY_STATES(GivenMsg) == (KEYFL_UP | KEYFL_DOWN)) /* try to give msg to next device */
                    &&(MSG_KEY_DURATION(GivenMsg) < KEYSHORT              ) )
                {
                    /* give focus immediatly to next screen */
                    SetDevice.fFocused = FALSE;                            /* clear our focus */
                    SetDeviceShow(FALSE);                                  /* clear our screen */
                    MSG_BUILD_SETFOCUS(NewMsg, DEVID_SET, DEVID_INTRO);
                    MsgQPostMsg(NewMsg, MSGQ_PRIO_LOW);
                    RValue = ERR_MSG_PROCESSED;
                }
                break;
            case MSG_SCREEN_REFRESH:
                SetDeviceShow(TRUE);
                RValue = ERR_MSG_PROCESSED;
                break;
            default: return ERR_MSG_NOT_PROCESSED;
        }
    }
    return RValue;
}


/***********************************************************************
 *  FUNCTION:       SetDeviceTryObjects
 *  DESCRIPTION:    message loop over all device objects until one
 *                  objects used the message
 *  PARAMETER:      message given by device
 *  RETURN:         ERR_MSG_NOT_PROCESSED / ERR_MSG_PROCESSED
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE SetDeviceTryObjects(MESSAGE GivenMsg)
{
    ERRCODE RValue = ERR_MSG_NOT_PROCESSED;

    if (RValue == ERR_MSG_NOT_PROCESSED)
        RValue = ObjEditNumMsgEntry(&EditWheelSizeObj, GivenMsg);   /* try wheelsize */
    if (RValue == ERR_MSG_NOT_PROCESSED)
        RValue = ObjEditNumMsgEntry(&EditCCFNomObj, GivenMsg);      /* try CCFNom */
    if (RValue == ERR_MSG_NOT_PROCESSED)
        RValue = ObjEditNumMsgEntry(&EditCCFDenomObj, GivenMsg);    /* try CCFDenom */
    if (RValue == ERR_MSG_NOT_PROCESSED)
        RValue = ObjEditNumMsgEntry(&EditBacklObj, GivenMsg);       /* try backlight switch */
    if (RValue == ERR_MSG_NOT_PROCESSED)
        RValue = ObjEditNumMsgEntry(&EditVehicDistObj, GivenMsg);   /* try vehicle distance */
    if (RValue == ERR_MSG_NOT_PROCESSED)
        RValue = ObjEditNumMsgEntry(&EditContrLevObj, GivenMsg);    /* try display contrast */

    SetDeviceCheckChanges();                                        /* check & execute changes */
    return RValue;
}



/***********************************************************************
 *  FUNCTION:       SetDeviceStateMachine
 *  DESCRIPTION:    focus handles over all selectable objects
 *  PARAMETER:      message id (up/down)
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE SetDeviceStateMachine(MESSAGE Msg)
{
    MESSAGE_ID  MsgId = MSG_ID(Msg);                            /* get message id */
    ERRCODE     RValue = ERR_MSG_NOT_PROCESSED;

    /* scroll up? */
    if (  (RValue == ERR_MSG_NOT_PROCESSED                )     /* still unpocessed */
        &&(MsgId  == MSG_KEY_UP                           )     /* [UP] */
        &&(  (MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED )     /* now pressed */
           ||(MSG_KEY_TRANSITION(Msg) == KEYTRANS_ON      ) ) ) /* or longer pressed? */
    {
        SetDevice.wDevState--;                                  /* previous state */
        if (SetDevice.wDevState == eSetFirst)                   /* wrap around? */
            SetDevice.wDevState = (eSetLast-1);
        SetDeviceSetFocus(SetDevice.wDevState);
        SetDeviceShow(TRUE);
        RValue = ERR_MSG_PROCESSED;
        ODS1( DBG_SYS, DBG_INFO, "SetDevState: %u", SetDevice.wDevState);
    }

    /* scroll down? */
    if (  (RValue == ERR_MSG_NOT_PROCESSED                )     /* still unpocessed */
        &&(MsgId  == MSG_KEY_DOWN                         )     /* [DOWN] */
        &&(  (MSG_KEY_TRANSITION(Msg) == KEYTRANS_PRESSED )     /* now pressed */
           ||(MSG_KEY_TRANSITION(Msg) == KEYTRANS_ON      ) ) ) /* or longer pressed? */
    {
        SetDevice.wDevState++;                                  /* next state */
        if (SetDevice.wDevState  == eSetLast)                   /* wrap around? */
            SetDevice.wDevState = (eSetFirst+1);
        SetDeviceSetFocus(SetDevice.wDevState);
        SetDeviceShow(TRUE);
        RValue = ERR_MSG_PROCESSED;
        ODS1( DBG_SYS, DBG_INFO, "SetDevState: %u", SetDevice.wDevState);
    }
    return RValue;
}


/***********************************************************************
 *  FUNCTION:       SetDeviceStateMachine
 *  DESCRIPTION:    focus handles over all selectable objects
 *  PARAMETER:      message id (up/down)
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE SetDeviceSetFocus(SETDEVSTATE eState)
{
    /* default: clear all focuses */
    EditWheelSizeObj.State.bits.fSelected = FALSE;
    EditCCFNomObj.State.bits.fSelected    = FALSE;
    EditCCFDenomObj.State.bits.fSelected  = FALSE;
    EditBacklObj.State.bits.fSelected     = FALSE;
    EditVehicDistObj.State.bits.fSelected = FALSE;
    EditContrLevObj.State.bits.fSelected  = FALSE;

    /* set focus only to one object */
    switch (eState)
    {
        case eSetWheelsize:     EditWheelSizeObj.State.bits.fSelected = TRUE; break;
        case eSetCCFNom:        EditCCFNomObj.State.bits.fSelected    = TRUE; break;
        case eSetCCFDenom:      EditCCFDenomObj.State.bits.fSelected  = TRUE; break;
        case eSetDisplLight:    EditBacklObj.State.bits.fSelected     = TRUE; break;
        case eSetVehicDist:     EditVehicDistObj.State.bits.fSelected = TRUE; break;
        case eSetDisplContr:    EditContrLevObj.State.bits.fSelected  = TRUE; break;
        default: ODS1( DBG_SYS, DBG_INFO, "Unknown DevState: %u", SetDevice.wDevState);
    }
}


/***********************************************************************
 *  FUNCTION:       SetDeviceCheckChanges
 *  DESCRIPTION:    compares all object data for any change
 *                  if any, the global source will be updated,
 *                  and the changed value will be executed
 *  PARAMETER:      -
 *  RETURN:         -
 *  COMMENT:        -
 *********************************************************************** */
void SetDeviceCheckChanges(void)
{
    static  fEditLast;

    /* CCF ----------------------------------------- */
    if (  (CCF.nibble.nom   != CCFNom  )
        ||(CCF.nibble.denom != CCFDenom) )
    {
        CCF.nibble.nom   = CCFNom;                          /* save global -> auto eepom update! */
        CCF.nibble.denom = CCFDenom;
    }

    /* Backlight ----------------------------------- */
    if (gDisplayFlags.flags.fBackl != fBackl)               /* saved with changes? */
        gDisplayFlags.flags.fBackl = fBackl;                /* -> save global -> auto eepom update! */
    if (EditBacklObj.State.bits.fEditActive == FALSE)       /* edit mode NOT active? */
            LCDDrvSetBacklight(gDisplayFlags.flags.fBackl); /* -> set global backlight value */
    else    LCDDrvSetBacklight(fBacklCopy);                 /* ELSE: -> set object copy backlight value */

    /* Vehicle Distance ---------------------------- */
    if (VehicDist.km != dwVehicDist)
    {
        VehicDist.km = dwVehicDist * 100L;                  /* give back km into dkm structure */
        MeasSetVehicDist( &VehicDist );                     /* save into system varaibale */
    }

    /* Display contrast ---------------------------- */
    if (gDisplayFlags.flags.ContrLev != bContrLev)          /* saved with changes? */
        gDisplayFlags.flags.ContrLev = bContrLev;           /* -> save global -> auto eepom update! */
    if (EditContrLevObj.State.bits.fEditActive == FALSE)            /* edit mode NOT active? */
            LCDDrvSetContrastLevel(gDisplayFlags.flags.ContrLev);   /* -> set global contrast value */
    else    LCDDrvSetContrastLevel(bContrLevCopy);                  /* ELSE: -> set object copy contrast value */
}
