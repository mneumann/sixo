/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2004-02-29 by Ralf Schwarzer
 *  Project:        SIxO20
 *  Module:         M16C Microcontroller
 *  Purpose:        uC specific stuff
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

#ifndef _M16C_H 
#define _M16C_H
 
 
 
 /* disable/enable m16c interrupts
   note1: no interrupts will be missed!
   note2: up to 4 nops are essential when writing into interrupt control register */

#define INT_GLOB_ENABLE         asm("fset i");asm("nop");asm("nop");asm("nop");asm("nop")
#define INT_GLOB_DISABLE        asm("fclr i");asm("nop");asm("nop");asm("nop");asm("nop")

#define NOP            asm(" nop ")

 
#endif // _M16C_H

