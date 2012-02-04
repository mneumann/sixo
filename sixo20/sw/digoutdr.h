/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-03-01 by Arnold Neugebauer
 *  Project:        SIxO
 *  Module:         Digital Output Driver
 *  Purpose:        Digital Output Driver api export header
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
 *  This information is automatically added while 'commit' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 3.2  2012/02/04 08:39:10  tuberkel
 * BugFix DigOut PWM
 *
 *
 ************************************************************************ */


ERRCODE DigOutInit(void);


#if(TOGGLE_PADS==1)
/* makros for port3_2/3/4 for ossi checks */
/* port pin will change state for everey call */
    #define TOGGLE_PAD9         (p3_2 = p3_2 ? 0 : 1)
    #define TOGGLE_PAD10        (p3_3 = p3_3 ? 0 : 1)
    #define TOGGLE_PAD11        (p3_4 = p3_4 ? 0 : 1)
#else
    #define TOGGLE_PAD9
    #define TOGGLE_PAD10
    #define TOGGLE_PAD11
#endif


// define GeneralPurposeOutput and Pads
#define GPO0    p3_0
#define GPO1    p3_1


/* GPO driver enumeration */
typedef enum
{
    eDIGOUT_GPO0,    //
    eDIGOUT_GPO1,    //
} DIGOUT_GPOS;


// solder pads (might be used for debug/extension purpuse)
#define PAD9    p3_2
#define PAD10   p3_3
#define PAD11   p3_4


// prototypes
ERRCODE DigOutSetGPO(DIGOUT_GPOS eGPO, BOOL fActivate);
BOOL    DigOutGetGPO( DIGOUT_GPOS eGPO );

