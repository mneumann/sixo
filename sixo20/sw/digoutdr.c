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
 *  Purpose:        Low level support of all digital outputs
 *                  (but not GPOs and Buzzer!)
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

 #include "standard.h"
 #include "sfr62p.h"
 #include "debug.h"
 #include "digoutdr.h"




/***********************************************************************
 *  FUNCTION:       DigOut_Init
 *  DESCRIPTION:    Initializes all General Digital Output Pins
 *                  and some solder pads
 *  PARAMETER:      -
 *  RETURN:         ERR_OK
 *  COMMENT:        Prevents floating input pins p3_0 / p3_1 of
 *                  destroying transistors, should be called as early as
 *                  possible
 *********************************************************************** */
ERRCODE DigOutInit(void)
{

#if(DEBUG==1)
    /* TEST: Port p3_2/3/4 for ossi checks */
    p3   = p3 & (~0x1c);   /* clear output of p3_2/3/4 */
    pd3  = p3 |   0x1c;    /* set p3_2/3/4 to output */
    // use macros TOGGLE_PAD9/10/11 for further tests
#endif

    /* set general purpose outputs GPO0/1 to prevent damage */
    p3_0 = 0;
    p3_1 = 0;
    pd3_0 = 1;
    pd3_1 = 1;
    // use macros GPO0/1 for further use
    ODS(DBG_DRV,DBG_INFO,"DigOutInit() done!");
    return ERR_OK;
}




 /***********************************************************************
 *  FUNCTION:       DigOutSetGPO()
 *  DESCRIPTION:    control over one single GPO
 *  PARAMETER:      eGPO            GPO indicator
 *                  fActivate       TRUE = GPO on
 *  RETURN:         ERR_OK          ok
 *                  ERR_PARAM_ERR   parameter error
 *  COMMENT:        GPO State could have been set _directly_ with
 *                  given parameter. But we here distinguish
 *                  the true/false states to support separate debugging
 *                  breakpoints for both states.
 *********************************************************************** */
ERRCODE DigOutSetGPO(DIGOUT_GPOS eGPO, BOOL fActivate)
{
    switch (eGPO)
    {
        case DIGOUT_GPO0:
            if (fActivate)
                 GPO0  = 1;
            else GPO0  = 0;
            break;
        case DIGOUT_GPO1:
            if (fActivate)
                 GPO1 = 1;
            else GPO1 = 0;
            break;
        default:
            return ERR_PARAM_ERR;
     }
    return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       DigOutGetGPO()
 *  DESCRIPTION:    Returns current state of single GPO
 *  PARAMETER:      eGPO            GPO indicator
 *  RETURN:         fActivate       TRUE  = GPO on
 *                                  FALSE = GPO off
 *  COMMENT:        -
 *********************************************************************** */
BOOL DigOutGetGPO( DIGOUT_GPOS eGPO )
{
    BOOL fActivated;
    switch (eGPO)
    {
        case DIGOUT_GPO0:   fActivated = GPO0; break;
        case DIGOUT_GPO1:   fActivated = GPO1; break;
        default: return fActivated = FALSE;
     }
    return fActivated;
}


 