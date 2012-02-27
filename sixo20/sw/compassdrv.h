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
 *  Purpose:        compass api export header
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
 * Revision 3.3  2012/02/27 23:15:38  tuberkel
 * CompassDrv API changed
 *
 * Revision 3.2  2012/02/14 21:08:03  tuberkel
 * - #define COMPASS ==> COMPDRV
 * - Compass SystemParam moved from devFlags2 -> 3
 * - Settings 'Compass' ==> 'Extensions'
 * - all Compass-Display modules enabled by default
 *
 *
 ************************************************************************ */

#ifndef _COMPASSDRV_H
#define _COMPASSDRV_H


//If defined the heading and calibration state is marked as invalid on
//communication error.
//If not defined the old heading and calibration state remains valid. The
//next succesful communication will update it again. The latter seams to be
//easier for the graphical user interface to implement. It just shows the
//old information until it is updated again.
//#define COMPASS_INVALIDATE_ON_ERROR


typedef struct
{
   UINT8 ucSwMajor;
   UINT8 ucSwMinor;
   UINT8 ucProtoMajor;
   UINT8 ucProtoMinor;
   UINT8 ucSwPatch;
   BOOL  bValid;
} COMPDRV_VERSINFO;

typedef struct
{
   UINT16 usHeading;
   UINT8  ucCalState;
   BOOL   bValid;
} COMPDRV_HEADINFO;


// The upper layers may:
// - Read the version info (note 1).
// - Read the heading and calibration state (note 1).
// - Read the error count (note 2).
// - Issue a CompDrv_Cmd_Reset (resets compass AND driver, note 4).
// - Issue a CompDrv_Cmd_IncCalState (note 4).
//
// All changes of version info, heading, calibration state and error is notified
// to the upper layers with a MSG_COMPASS_RFRSH message.
//
// See also the COMPASS_INVALIDATE_ON_ERROR switch above.
//
// note 1: Not valid until about 2 seconds after reset/init (check .bValid).
// note 2: The error count simply increases on every error. Intended to be implemen-
//         ted as turning wheel "-\|/".
// note 4: If this is called multiple times in a short sequence (or, what is very
//         unlikely, interferes with another command sent to the compass), the compass
//         just does'nt react. So these functions may be called simply as a reaction
//         on the key pressed by the user, disregarding all timing. The user has to
//         observe compass reaction.


// public prototypes
void    CompDrv_RxIsr( void );   //called by interrupt
ERRCODE CompDrv_Init( void );   //called once by main()
void    CompDrv_Task( void );   //called cyclic by main()

COMPDRV_VERSINFO *CompDrv_GetVersionInfo( void );
COMPDRV_HEADINFO *CompDrv_GetHeadingInfo( void ); //not recommended, see above

UINT8 CompDrv_GetError( void );
void  CompDrv_Cmd_Reset( void );
void  CompDrv_Cmd_IncCalState( void );


#endif //_COMPASSDRV_H
