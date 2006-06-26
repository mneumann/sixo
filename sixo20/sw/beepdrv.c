/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-10-01 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         beeper
 *  Purpose:        low level beep functions
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
 *  --------------------------------------------------------------------
 *
 *  CVS History
 *
 *  This information is automatically added while 'commiting' the
 *  changes to CVC ('Log message'):
 *
 * $Log$
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 *
 ************************************************************************ */



#include "sfr62p.h"
#include "standard.h"
#include "beepdrv.h"
#include "timer.h"
#include "debug.h"



/***********************************************************************
 *  FUNCTION:       BeepDrvInit()
 *  DESCRIPTION:    initializes Beeper hardware
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE BeepDrvInit(void)
{
    fBeep   = 0;    /* switch off output */
    fBeep_D = 1;    /* enabled output */

    ODS(DBG_DRV,DBG_INFO,"BeepDrvInit() done!");
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       BeepDrvSetBeeper()
 *  DESCRIPTION:    switches beeper on/off
 *  PARAMETER:      BOOL    fActive     TRUE = on
 *  RETURN:         error code
 *  COMMENT:        -
 *********************************************************************** */
ERRCODE BeepDrvSetBeeper(BOOL fActive)
{
    fBeep = (fActive == TRUE) ? 1 : 0;

    if (fActive == TRUE)
         fBeep   = 1;    /* switch ON output */
    else fBeep   = 0;    /* switch OFF output */

    return ERR_OK;
}



 