/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-02-16 by Arnold Neugebauer
 *  Project:        SIxO
 *  Module:         non volatile ram driver
 *  Purpose:        non volatile ram api export header
 *  Comments:
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
 *      Stöckener Str. 115                  Knickstr. 10
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
#ifndef _NVRAMDRV_H
#define _NVRAMDRV_H


//for the current hardware implementation IIC_NVRAM_DEV_ADDR and
//IIC_CLK_DEV_ADR are the same because the clock and the nvram are
//located physically in the same device
#define IIC_NVRAM_DEV_ADDR 0x68  //right aligned, highest bit doesn't care

//offset between first register of the device (subaddress 0) and first
//nvram location
#define IIC_NVRAM_OFFSET   8

#define NVRAMSIZE 56



//function prototypes
ERRCODE iicInit( void );
ERRCODE iicNvramWrite( UINT8 NvramTrgtAddr, UINT8 nBytes, UINT8* pSrcAddr );
ERRCODE iicNvramRead( UINT8 NvramSrcAddr, UINT8 nBytes, UINT8* pTrgtAddr );

//test functions only present in the debug version
#ifdef DEBUG
   ERRCODE iicTestNvram( void );
#endif


#endif //_NVRAMDRV_H
