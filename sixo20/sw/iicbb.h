/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        01-10-2001 by Arnold Neugebauer
 *  Project:        SIxO20
 *  Module:         IIC bit banging routines
 *  Purpose:        simple IIC routines which do *not* use UART2
 *
 *  mcu:            Mitsubishi M30624FGLFP @ 10MHz
 *  notes:          No multi-master, no collision detection, no wait states (no
 *                  IIC bus clock extending allowed).
 *                  Make sure, that the routines are called with the correct
 *                  paremeter format (use ASSERTs for example). Parameters are
 *                  not checked in the low level routines.
 *                  The NC30 seems to have a problem with structure and union
 *                  types in #pragma parameter declarations. If you experience
 *                  problems in calling iic_rcv or iic_snd try to
 *                  use standard types.
 *  history:        first build 28.02.01
 *                  first release 03.02.01
 *                  introduced different buffers for address and data 04.03.01
 *                  SCL/SDA can now be any general purpose i/o port 06.03.01
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

#ifndef _IICBB_H   
#define _IICBB_H     

//functions available for C
//return value is 1 = OK, acknowledge received
//                0 = failed, no acknowledge received
//see header of iicbb.a30 for details


extern unsigned char iic_rcv( unsigned char *adr_buffer,
                              unsigned char number_of_adr,
                              unsigned char *dat_buffer,
                              unsigned char number_of_dat );

extern unsigned char iic_snd( unsigned char *adr_buffer,
                              unsigned char number_of_adr,
                              unsigned char *dat_buffer,
                              unsigned char number_of_dat );


#pragma PARAMETER iic_rcv( A1, R1H, A0, R0H )
#pragma PARAMETER iic_snd( A1, R1H, A0, R0H )

#endif /* _IICBB_H   */
