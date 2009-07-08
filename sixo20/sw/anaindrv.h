/***********************************************************************
 *
 *  $RCSfile$
 *
 *  $Revision$
 *
 *  $Date$
 *
 *  Author: Markus Mueller, mm@rsj.de
 *          Arnold Neugebauer
 *
 *  --------------------------------------------------------------------
 *  Created:        10-22-2003 by Markus Mueller
 *  Project:        SIxO 2.0
 *  Module:         Illumination Measurement
 *  Purpose:        measure lumination with A/D port AN3 (P103)
 *  Comments:       -
 *  Changes:        "Alles neu macht der Mai" - a german proverb, AN 10-23-2004
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
 *      Stoeckener Str. 115                 Knickstr. 10
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
#ifndef _ANAINDRV_H
#define _ANAINDRV_H


#include "standard.h"


//power state of external temperature sensors
#define ANAIN_EN_OFF  0x07
#define ANAIN_EN_TAIR 0x06 //one of the three bits low
#define ANAIN_EN_TWAT 0x05
#define ANAIN_EN_TOIL 0x03


//channel assignment
//equals hardware register layout, thus hard coded values (no enum)
#define ANAIN_EXTU 0 //internal general purpose input connected to header on pcb
#define ANAIN_TEMP 1 //internal temperature sensor on the pcb
#define ANAIN_LUMI 2 //internal luminance sensor near LEDs
#define ANAIN_VOLT 3 //supply voltage (connected internally)
#define ANAIN_ALTW 4 //voltage at alternator side of regulator (external connection)
#define ANAIN_TAIR 5 //external air temperature sensor
#define ANAIN_TWAT 6 //external water temperature sensor
#define ANAIN_TOIL 7 //external oil temperature sensor
#define ANAIN_LAST 8 //no sensor, this is for range check and array size


//prototypes
ERRCODE AnaInDrvInit( void );
void AnaInDrvTriggerADConverter( void );

#if(DEBUG==1)
UINT8 AnaInDrvTestGenerateADValues( UINT8 ucADValue );
#endif //DEBUG


#endif //_ANAINDRV_H
