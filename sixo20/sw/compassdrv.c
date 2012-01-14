/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2009-04-01 Arnold Neugebauer
 *  Project:        SIxO
 *  Module:         compass driver
 *  Purpose:        compass api functions
 *  Comments:       This is quite a hack, I know. Instead of implementing
 *                  a proper UART driver I use a simple blocking routine
 *                  to send chars. Asymetrically, for receiving, an ISR is
 *                  implemented that is doing the special packet synchroni-
 *                  sation (not reusable by other modules). Init of the
 *                  UART for debug and compass should be the same but it is
 *                  implemented twice. Hard coded values are used, even for
 *                  things dependent on clock frequency. Error signals from
 *                  the hardware are disregarded. Enabling different parts
 *                  of the reception may not be in proper order. The imple-
 *                  mentation is heavyly dependent on the compass software
 *                  version. Header/defines from all over the place are used.
 *                  Some ceavats are left out, necessary to get around the
 *                  flaws of the M16C hardware. Not all hardware registers
 *                  are initialised, setup depends on register reset values.
 *                  No parameter checks. No test functions. Packets, their
 *                  stuffing, encoding, checksums and states are hard coded.
 *                  Assumes M16C and compass are both little endian. A lot
 *                  of global variables...
 *                  You see: a lot of nonsens, a lot of bullshit. But at
 *                  least it works (sometimes). This is how successful
 *                  products are implemented nowadays. I love my job :-|
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
 *  Disclaimer of warranty
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


#include "standard.h"
#include "compassdrv.h"


/* main switch for this module */
#if (COMPASS==1)


#if (DEBUG==1) && (MINIEMU==1)
    #error "ERROR: Use only two of DEBUG, MINIEMU and COMPASS!"
#endif


#include <string.h>
#include "sfr62p.h"
#include "timer.h"
#include "m16c.h"



//buffer size must be a multiple of 3, the largest 'D' packet is 93 bytes big
#define COMPASS_RXD_BUFFER_LEN (3*31)

//All CompassTxByte() calls may be replaced by this macro. Tradeoff between
//execution time (CPU performance nedded) and code size. (Compiler is not very
//clever, code size could be further reduced by implementing this in assembler.)
//#define COMPASS_TX_BYTE( ucChar ) while( !(u0c1 & 0x02) ); u0tb = ucChar


typedef struct
{
   UINT8  ucDigital[2];
   UINT16 usAnalog[32];
} tCompassOutput;

#ifdef COMPASS_DEBUG
typedef struct
{
   UINT8  ucRxIsrState;
   UINT8  ucCompassState;
   UINT8  ucCompassError;
   UINT16 usCurrentTime;
} tCompassDebug;
#endif


extern  UINT16  wMilliSecCounter; //system timer of the module timer.c

UINT8 gucRxIsrState = 0;
UINT8 gaucRxdBuffer[COMPASS_RXD_BUFFER_LEN];
UINT8 gucRxdBufferPtr = 0;

UINT8  gucCompassState  = 0;
UINT8  gucCompassError  = 0;
UINT16 gusDelayStart    = 0;
UINT16 gusDelayTime     = 0;
UINT16 gusSequenceStart = 0;

tCompassVersionInfo gtCompassVersionInfo;
tCompassHeadingInfo gtCompassHeadingInfo;

MESSAGE gtCompassMessage = {0, 0, 0, MSG_COMPASS_RFRSH};


void CompassResetDriverState( void );
void CompassTxByte( UINT8 ucChar );
void CompassCmdSetInterval0( void );
void CompassCmdSetInterval500( void );
void CompassCmdGetVersionInfo( void );
BOOL CompassCheckSumValid( void );
void CompassDecodePacket( void );


/***********************************************************************
 *  FUNCTION:       CompassRxIsr
 *  DESCRIPTION:    UART0 receive ISR
 *  PARAMETER:      none
 *  RETURN:         none
 *  COMMENT:        Runs at very high priority. Keep as short as possible.
 *********************************************************************** */
#pragma INTERRUPT CompassRxIsr
void CompassRxIsr( void )
{
   UINT8  ucChar = u0rb;

   INT_GLOB_ENABLE; //allow interrupt nesting after byte from UART is read

   switch( gucRxIsrState ){
      case 0:
         //blocked, not started or packed received
         return;
      case 1:
         //wait for sync
         if( ucChar == '#' ){
            gucRxdBufferPtr = 0;
            gucRxIsrState++;
         }
         return;
      case 2:
         //collect chars into buffer
         //overflow simply stops reception, leads to checksum error later on
         if( (gucRxdBufferPtr >= COMPASS_RXD_BUFFER_LEN) || (ucChar == '\r') ) gucRxIsrState = 0;
         gaucRxdBuffer[gucRxdBufferPtr++] = ucChar;
         return;
      default:
         //should never happen
         gucRxIsrState = 0;
         return;
   }
}


/***********************************************************************
 *  FUNCTION:       CompassResetDriverState
 *  DESCRIPTION:    resets the internal states of ISR and sequence control
 *  PARAMETER:      none
 *  RETURN:         none
 *  COMMENT:        Called during initialisation, after resetting the
 *                  compass with CommandReset() or on error.
 *********************************************************************** */
void CompassResetDriverState( void )
{
   gucRxIsrState   = 0;
   gucCompassState = 0;

   memset( &gtCompassVersionInfo, 0, sizeof(tCompassVersionInfo) );
   gtCompassVersionInfo.bValid = FALSE;

   memset( &gtCompassHeadingInfo, 0, sizeof(tCompassHeadingInfo) );
   gtCompassHeadingInfo.bValid = FALSE;

   MsgQPostMsg( gtCompassMessage, MSGQ_PRIO_LOW );

   //2 seconds delay after compass power on or CommandReset() needed
   TimerGetSys_msec( gusDelayStart );
   gusDelayTime = 2000;
}


/***********************************************************************
 *  FUNCTION:       CompassTxByte
 *  DESCRIPTION:    transmits the given char via UART0
 *  PARAMETER:      char to transmit
 *  RETURN:         none
 *  COMMENT:        Transmission has already been enabled in CompassInit().
 *                  So u0c1 |= 0x01, present in sput(), has been omitted.
 *********************************************************************** */
void CompassTxByte( UINT8 ucChar )
{
    while ( !(u0c1 & 0x02) );
    u0tb = ucChar;
}


/***********************************************************************
 *  FUNCTION:       CompassCmdSetInterval0
 *  DESCRIPTION:    stops automatic sending of heading and other infor-
 *                  mation
 *  PARAMETER:      none
 *  RETURN:         none
 *  COMMENT:        Used to simplify communication. Usually called before
 *                  a request for other data is sent to the compass.
 *********************************************************************** */
void CompassCmdSetInterval0( void )
{
   CompassTxByte( 0x23 ); //# sync
   CompassTxByte( 0x63 ); //c address 2
   CompassTxByte( 0x64 ); //d command
   CompassTxByte( 0x3d ); //= 1 byte data of value 0, stuffed to 3 bytes with two additional 0's,
   CompassTxByte( 0x3d ); //=        encoded to 4 bytes with offset 0x3d
   CompassTxByte( 0x3d ); //=
   CompassTxByte( 0x3d ); //=
   CompassTxByte( 0x44 ); //D checksum first byte
   CompassTxByte( 0x5b ); //[ checksum second byte
   CompassTxByte( 0x0d ); //termination
}


/***********************************************************************
 *  FUNCTION:       CompassCmdSetInterval500
 *  DESCRIPTION:    sends interval to compass, heading and other infor-
 *                  mation will be sent automatically every 500 ms
 *  PARAMETER:      none
 *  RETURN:         none
 *  COMMENT:
 *********************************************************************** */
void CompassCmdSetInterval500( none )
{
   CompassTxByte( 0x23 ); //# sync
   CompassTxByte( 0x63 ); //c address 2
   CompassTxByte( 0x64 ); //d command
   CompassTxByte( 0x49 ); //I 1 byte data of value 50, stuffed to 3 bytes with two additional 0's,
   CompassTxByte( 0x5d ); //]        encoded to 4 bytes with offset 0x3d
   CompassTxByte( 0x3d ); //=
   CompassTxByte( 0x3d ); //=
   CompassTxByte( 0x45 ); //E checksum first byte
   CompassTxByte( 0x47 ); //G checksum second byte
   CompassTxByte( 0x0d ); //termination
}


/***********************************************************************
 *  FUNCTION:       CompassCmdGetVersionInfo
 *  DESCRIPTION:    sends requests for version information to the compass
 *  PARAMETER:      none
 *  RETURN:         none
 *  COMMENT:
 *********************************************************************** */
void CompassCmdGetVersionInfo( void )
{
   CompassTxByte( 0x23 ); //# sync
   CompassTxByte( 0x63 ); //c address 2
   CompassTxByte( 0x76 ); //v command
   CompassTxByte( 0x40 ); //@ checksum first byte
   CompassTxByte( 0x79 ); //y checksum second byte
   CompassTxByte( 0x0d ); //termination
}


/***********************************************************************
 *  FUNCTION:       CompassCheckSumValid
 *  DESCRIPTION:    computes checksum and compares with checksum in packet
 *  PARAMETER:      none
 *  RETURN:         TRUE if checksum is valid
 *  COMMENT:
 *********************************************************************** */
BOOL CompassCheckSumValid( void )
{
   UINT8  ucIdx      = 0;
   UINT8  ucIdxEnd   = gucRxdBufferPtr - 3; //gucRxdBufferPtr points to byte behind termination
   UINT16 usCheckSum = '#';
   UINT8  ucTmp      = 0;

   for( ucIdx = 0; ucIdx < ucIdxEnd; ucIdx++ ){
      usCheckSum += gaucRxdBuffer[ucIdx];
   }

   usCheckSum %= 4096;

   ucTmp = '=' + usCheckSum / 64;
   if( gaucRxdBuffer[ucIdx] != ucTmp ) return FALSE;

   ucIdx++;
   ucTmp = '=' + usCheckSum % 64;
   if( gaucRxdBuffer[ucIdx] != ucTmp ) return FALSE;

   return TRUE;
}


/***********************************************************************
 *  FUNCTION:       CompassDecodePacket
 *  DESCRIPTION:    revertes the encoding applied by the compass
 *  PARAMETER:      none
 *  RETURN:         none
 *  COMMENT:
 *********************************************************************** */
void CompassDecodePacket( void )
{
   UINT8  ucA,ucB,ucC,ucD;
   UINT8  ucX,ucY,ucZ;
   UINT8  ucIdxIn  = 2;  //point to first data byte
   UINT8  ucIdxOut = 2;
   UINT8  ucIdxInEnd = gucRxdBufferPtr - 3; //gucRxdBufferPtr points to byte behind termination

   while( ucIdxIn < ucIdxInEnd )
   {
      ucA = gaucRxdBuffer[ucIdxIn++] - '='; //remove offset 0x3d
      ucB = gaucRxdBuffer[ucIdxIn++] - '=';
      ucC = gaucRxdBuffer[ucIdxIn++] - '=';
      ucD = gaucRxdBuffer[ucIdxIn++] - '=';

      ucX = (ucA << 2) | (ucB >> 4);        //invert encoding
      ucY = ((ucB & 0x0f) << 4) | (ucC >> 2);
      ucZ = ((ucC & 0x03) << 6) | ucD;

      gaucRxdBuffer[ucIdxOut++] = ucX;
      gaucRxdBuffer[ucIdxOut++] = ucY;
      gaucRxdBuffer[ucIdxOut++] = ucZ;
   }
}



/***********************************************************************
 *  FUNCTION:       CompassInit
 *  DESCRIPTION:    inits UART0 and its receive interrupt
 *  PARAMETER:      none
 *  RETURN:         ERRCODE   ERR_OK
 *  COMMENT:
 *********************************************************************** */
ERRCODE CompassInit( void )
{
   //reset driver
   CompassResetDriverState();

   //57600 Baud 8N1, enable reception
   u0mr  = 0x85; //reversed polarity since compass TxD/RxD has no inverters
   u0brg = 0x0F;
   u0c0  = 0x10;
   u0c1  = 0x05;

   INT_GLOB_DISABLE;
   //1 = lowest, 7 = highest priority
   //UART reception at 57600 Baud needs a very high priority, otherwise
   //bytes are lost. (Timers are currently set to prio 2..5, debugger runs
   //on 7). At least interrupt nesting is allowed in the ISR.
   //A lower priority may be chosen if the application bears to be sometimes
   //not updated. The protocol and the driver is robust enough to withstand
   //lost bytes/packets from time to time. Also consider to disable interrupt
   //nesting in the ISR.
   s0ric = 7;
   INT_GLOB_ENABLE;

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       CompassGetVersionInfo
 *  DESCRIPTION:    provides access to the version information received
 *                  from the compass
 *  PARAMETER:      none
 *  RETURN:         pointer to the version information
 *  COMMENT:        Always check if valid (->bValid).
 *                  An MSG_COMPASS_RFRSH is sent if the version info
 *                  changes.
 *********************************************************************** */
tCompassVersionInfo *CompassGetVersionInfo( void )
{
   return &gtCompassVersionInfo;
}


/***********************************************************************
 *  FUNCTION:       CompassGetHeadingInfo
 *  DESCRIPTION:    provides access to heading and calibration state
 *                  received from the compass
 *  PARAMETER:      none
 *  RETURN:         pointer to the heading information
 *  COMMENT:        Always check if valid (->bValid).
 *                  An MSG_COMPASS_RFRSH is sent if the heading or
 *                  calibration state changes.
 *********************************************************************** */
tCompassHeadingInfo *CompassGetHeadingInfo( void )
{
   return &gtCompassHeadingInfo;
}


/***********************************************************************
 *  FUNCTION:       CompassGetError
 *  DESCRIPTION:    returns the compass error count
 *  PARAMETER:      none
 *  RETURN:         error count
 *  COMMENT:        The error count is simply increased on every error.
 *                  Intended to be implemented as turning wheel "-\|/".
 *                  An MSG_COMPASS_RFRSH is sent if the error count
 *                  changes.
 *********************************************************************** */
UINT8 CompassGetError( void )
{
   return gucCompassError;
}


/***********************************************************************
 *  FUNCTION:       CompassCmdReset
 *  DESCRIPTION:    sends a reset packet to the compass and resets the
 *                  driver
 *  PARAMETER:      none
 *  RETURN:         none
 *  COMMENT:        Not necessary at startup. Intended to break an
 *                  already started calibration sequence by the user.
 *********************************************************************** */
void CompassCmdReset( void )
{
   CompassResetDriverState();

   CompassTxByte( 0x23 ); //# sync
   CompassTxByte( 0x63 ); //c address 2
   CompassTxByte( 0x52 ); //R command
   CompassTxByte( 0x40 ); //@ checksum first byte
   CompassTxByte( 0x55 ); //U checksum second byte
   CompassTxByte( 0x0d ); //termination
}


/***********************************************************************
 *  FUNCTION:       CompassCmdIncCalState
 *  DESCRIPTION:    increments the calibration state by 1, the sequence
 *                  is 0,1,2,3,4,5,0,1...
 *  PARAMETER:      none
 *  RETURN:         none
 *  COMMENT:        The sequence may only be interrupted by powering off
 *                  the compass or calling CompassCmdReset().
 *********************************************************************** */
void CompassCmdIncCalState( void )
{
   UINT8 ucCnt = 0;

   CompassTxByte( 0x23 );    //# sync
   CompassTxByte( 0x63 );    //c address 2
   CompassTxByte( 0x62 );    //b command
   CompassTxByte( 0x3d );    //= 11 bytes data, stuffed to 12 bytes with an additional 0,
   CompassTxByte( 0x3d );    //=        encoded to 16 bytes with offset 0x3d.
   CompassTxByte( 0x3d );    //= All but RemoteButtons = 2 and Frame = 0 is used. Frame is
   CompassTxByte( 0x3f );    //? set to 0, so no confirmation is sent back.

   do{
      CompassTxByte( 0x3d ); //=
      ucCnt++;
   }while( ucCnt < 12 );

   CompassTxByte( 0x4f );    //O checksum first byte
   CompassTxByte( 0x77 );    //W checksum second byte
   CompassTxByte( 0x0d );    //termination
}


/***********************************************************************
 *  FUNCTION:       Compass
 *  DESCRIPTION:    compass sequence control, cyclic called
 *                  by the main loop
 *  PARAMETER:      none
 *  RETURN:         none
 *  COMMENT:        Take care when using variables from another asynchro-
 *                  nuous process. gucRxIsrState is used. Since it is only
 *                  a byte access is atomar, thus ok. The RxBuffer and its
 *                  buffer pointer is used only when the ISR is blocked.
 *                  The timeouts are way more than the compass needs to
 *                  react. But in case communication is not stable we are
 *                  not retrying too fast in a loop. Thus the high timeouts
 *                  make sure to consume only low processing power and they
 *                  make sure not to flood the message system.
 *                  If the compass is not connected (does not send anything)
 *                  it is only tested once a second for reaction.
 *                  The time the compass needs for one main loop cycle is
 *                  varying a lot. Without acceleration measurement it has
 *                  been about 10 ms, with it about 13 ms. During calibration
 *                  9 ms have been measured.
 *********************************************************************** */
void Compass( void )
{
   #ifdef COMPASS_DEBUG
      #define DLEN 100
      static tCompassDebug atD[DLEN];
      static UINT8 ucDx = 0;
   #endif

   UINT16  usCurrentTime  = 0;
   BOOL    bCheckSumValid = FALSE;
   tCompassOutput *ptCompassOutput = (tCompassOutput *)(&(gaucRxdBuffer[2]));

   switch( gucCompassState ){
      case 0:
         //delay after compass power on, CommandReset() or version info error
         #ifdef COMPASS_DEBUG
            if(ucDx < DLEN){atD[ucDx].ucRxIsrState = gucRxIsrState; atD[ucDx].ucCompassState = gucCompassState; atD[ucDx].ucCompassError = gucCompassError; TimerGetSys_msec(atD[ucDx].usCurrentTime); ucDx++;}
         #endif
         TimerGetSys_msec( usCurrentTime );
         if( (usCurrentTime - gusDelayStart) >= gusDelayTime ) gucCompassState++;
         return;
      case 1:
         //stop automatic sending of packets
         #ifdef COMPASS_DEBUG
            if(ucDx < DLEN){atD[ucDx].ucRxIsrState = gucRxIsrState; atD[ucDx].ucCompassState = gucCompassState; atD[ucDx].ucCompassError = gucCompassError; TimerGetSys_msec(atD[ucDx].usCurrentTime); ucDx++;}
         #endif
         //We give away processing time after each state. Currently we loose control for about 25..90 ms each time. The time
         //depends on what's going on in the main loop. So it is not deterministic and it is far more than one cycle of the
         //compass. The ISR has to be retriggered for each packet. Thus we can only handle the reception of ONE PACKET AT A TIME.
         //The compass may send us two packets in one of its cycles. One packet answering our request and one of the automatically
         //packets sent out. So before requesting information the sending of automatic packets is turned off.
         CompassTxByte( 0x0d );    //reset receive ISR of compass, leads to wrong checksum, ISR is ready for exception immediately (especially this 0x0d is needed after reset of SIxO, PORT/UART init process puts compass ISR in blocking state)
         CompassCmdSetInterval0();
         TimerGetSys_msec( gusDelayStart );
         gusDelayTime = 32;        //32 ms delay needed to allow compass to finish a possibly ongoing transmission
         gucCompassState++;
         return;
      case 2:
         //delay after CompassCmdSetInterval0()
         TimerGetSys_msec( usCurrentTime );
         if( (usCurrentTime - gusDelayStart) >= gusDelayTime ){
            #ifdef COMPASS_DEBUG
               if(ucDx < DLEN){atD[ucDx].ucRxIsrState = gucRxIsrState; atD[ucDx].ucCompassState = gucCompassState; atD[ucDx].ucCompassError = gucCompassError; TimerGetSys_msec(atD[ucDx].usCurrentTime); ucDx++;}
            #endif
            gucCompassState++;
         }
         return;
      case 3:
         //request version info
         #ifdef COMPASS_DEBUG
            if(ucDx < DLEN){atD[ucDx].ucRxIsrState = gucRxIsrState; atD[ucDx].ucCompassState = gucCompassState; atD[ucDx].ucCompassError = gucCompassError; TimerGetSys_msec(atD[ucDx].usCurrentTime); ucDx++;}
         #endif
         gucRxIsrState = 1;        //start packet reception
         CompassCmdGetVersionInfo();
         TimerGetSys_msec( gusDelayStart );
         gusDelayTime = 200;       //200 ms timeout
         gucCompassState++;
         return;
      case 4:
         //receive version info
         if( gucRxIsrState == 0 ){ //packet received
            #ifdef COMPASS_DEBUG
               if(ucDx < DLEN){atD[ucDx].ucRxIsrState = gucRxIsrState; atD[ucDx].ucCompassState = gucCompassState; atD[ucDx].ucCompassError = gucCompassError; TimerGetSys_msec(atD[ucDx].usCurrentTime); ucDx++;}
            #endif
            if( (gaucRxdBuffer[1] == 'V') && CompassCheckSumValid() ){
               //version info received
               CompassDecodePacket();
               memcpy( &gtCompassVersionInfo, &(gaucRxdBuffer[2]), 5 );
               gtCompassVersionInfo.bValid = TRUE;
               MsgQPostMsg( gtCompassMessage, MSGQ_PRIO_LOW );
               gucCompassState++;
            }
            else{
               //bad packet, discard it, retry (if compass keeps sending us bad packets - very likely if compass has got
               //the wrong software version - the request for version info is made at a moderately low rate every second)
               gucCompassError++;
               MsgQPostMsg( gtCompassMessage, MSGQ_PRIO_LOW );
               TimerGetSys_msec( gusDelayStart );
               gusDelayTime    = 1000;
               gucCompassState = 0;
            }
         }
         else{
            TimerGetSys_msec( usCurrentTime );
            if( (usCurrentTime - gusDelayStart) >= gusDelayTime ){
               //retry on timeout after 800 ms (very likely if compass is not present, then the
               //request for version info is made at a moderately low rate every 800 + 200 ms)
                gucCompassError++;
                MsgQPostMsg( gtCompassMessage, MSGQ_PRIO_LOW );
                gusDelayStart   = usCurrentTime;
                gusDelayTime    = 800;
                gucCompassState = 0;
            }
         }
         return;
      case 5:
         //start automatic sending of packets
         #ifdef COMPASS_DEBUG
            if(ucDx < DLEN){atD[ucDx].ucRxIsrState = gucRxIsrState; atD[ucDx].ucCompassState = gucCompassState; atD[ucDx].ucCompassError = gucCompassError; TimerGetSys_msec(atD[ucDx].usCurrentTime); ucDx++;}
         #endif
         gucRxIsrState = 1;        //start packet reception
         CompassCmdSetInterval500();
         TimerGetSys_msec( gusDelayStart );
         gusSequenceStart = gusDelayStart;
         gusDelayTime = 200;       //200 ms timeout
         gucCompassState++;
         return;
      case 6:
         //receive heading and calibration state
         if( gucRxIsrState == 0 ){ //packet received
            #ifdef COMPASS_DEBUG
               if(ucDx < DLEN){atD[ucDx].ucRxIsrState = gucRxIsrState; atD[ucDx].ucCompassState = gucCompassState; atD[ucDx].ucCompassError = gucCompassError; TimerGetSys_msec(atD[ucDx].usCurrentTime); ucDx++;}
            #endif
            if( (gaucRxdBuffer[1] == 'D') && CompassCheckSumValid() ){
               //heading and calibration state received
               CompassDecodePacket();
               gtCompassHeadingInfo.ucCalState = ptCompassOutput->usAnalog[14];
               gtCompassHeadingInfo.usHeading  = ptCompassOutput->usAnalog[15] + 45; //sensors are mounted +45 degrees rotated on the PCB
               if( gtCompassHeadingInfo.usHeading > 359 ) gtCompassHeadingInfo.usHeading -= 360;
               gtCompassHeadingInfo.bValid = TRUE;
               MsgQPostMsg( gtCompassMessage, MSGQ_PRIO_LOW );
               gucRxIsrState = 1;  //start packet reception
               TimerGetSys_msec( gusDelayStart );
               gusDelayTime = 600; //500+100 ms timeout
            }
            else{
               //bad packet, discard it, wait for next (sent automatically), reeinit timeout
               gucCompassError++;
               #ifdef COMPASS_INVALIDATE_ON_ERROR
                  gtCompassHeadingInfo.bValid = FALSE;
               #endif
               MsgQPostMsg( gtCompassMessage, MSGQ_PRIO_LOW );
               gucRxIsrState = 1;  //start packet reception
               TimerGetSys_msec( gusDelayStart );
               gusDelayTime = 600; //500+100 ms timeout
            }
         }
         else{
            //reinit sequence on timeout (error) or compass' internal blocking (normal)
            TimerGetSys_msec( usCurrentTime );
            if( (usCurrentTime - gusDelayStart) >= gusDelayTime ){
               //If we do not send something once in a while the compass' internal state blocks
               //sending packets to us after 255 of its main loop cycles. If the timeout
               //came due to this, it's not an error. (For 13 ms cycle time with acceleration
               //measurement the compass blocks after 3315 ms. For 9 ms cycle time with
               //acceleration measurement the compass blocks already after 2295 ms. We take the
               //lowest value with some margin to avoid signaling false errors.)
               if( (usCurrentTime - gusSequenceStart) < 2200 ){
                  gucCompassError++;
                  #ifdef COMPASS_INVALIDATE_ON_ERROR
                     gtCompassHeadingInfo.bValid = FALSE;
                  #endif
                  MsgQPostMsg( gtCompassMessage, MSGQ_PRIO_LOW );
               }
               gucCompassState--;  //restart sequence
            }
         }
         return;
      default:
         CompassResetDriverState(); //should never happen
   }
}



#endif //COMPASS
