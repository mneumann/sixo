/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-02-17 Arnold Neugebauer
 *  Project:        SIxO
 *  Module:         eeprom driver
 *  Purpose:        eeprom api export header
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
 ************************************************************************ */

#ifndef _EEPRDRV_H
#define _EEPRDRV_H


// function prototypes
ERRCODE iicInit( void );
UINT16  iicEepromSize( void );
ERRCODE iicEepromWrite(UINT16 EepromTrgtAddr, UINT16 nBytes, UINT8 * pSrcAddr);
ERRCODE iicEepromRead(UINT16 EepromSrcAddr, UINT16 nBytes, UINT8 * pTrgtAddr);

//test functions only present in the debug version
#if(DEBUG==1)
   ERRCODE iicTestEeprom( void );
   ERRCODE iicTestEepromEmu( void );
   BOOL iic_snd_test( unsigned char *adr_buffer,
                      unsigned char number_of_adr,
                      unsigned char *dat_buffer,
                      unsigned char number_of_dat );
   BOOL iic_rcv_test( unsigned char *adr_buffer,
                      unsigned char number_of_adr,
                      unsigned char *dat_buffer,
                      unsigned char number_of_dat );
#endif


#endif //_EEPRDRV_H
