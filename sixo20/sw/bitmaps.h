/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-10-25 by Ralf Schwarzer
 *  Project:        resources
 *  Module:         bitmaps definitions
 *  Purpose:        bitmap raw data resources API export header
 *  Comments:
 *                  Currently a 128x64 (WxH) pixel display is supported.
 *                  This results in 8 pages a 8bit height. (=64 pixel H)
 *
 *                  All bitmaps orientation is horizontal,
 *                  divided into vertical 8 bit stripes
 *                  to support lcds hor. pages
 *
 *                  No segmentation into left/right
 *                  display half is done here.
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
 * Revision 3.0  2010/11/07 12:50:38  tuberkel
 * V30 Preparations:
 * - new Bike Logo enumtype
 *
 * Revision 2.1  2009/07/08 21:49:04  tuberkel
 * Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
 *
 * Revision 2.0  2006/06/26 23:25:50  tuberkel
 * no message
 *
 * Revision 1.4  2006/03/18 08:43:16  Ralf
 * changed logo handling
 *
 *
 ************************************************************************ */


#ifndef BITMAP_H
#define BITMAP_H

// SIxO logos
// Note:    these enums have to in the same order like
//          the BMP pointers in fpBikeLogos[]!!!
typedef enum
{
    eLogo_NKDEV,
    eLogo_BMW,
    eLogo_HUSQV,
    eLogo_BAGHIRA,
    eLogo_SIXO,
    eLogo_AfricaTwin,
    eLogo_BMW_1100GS,
    eLogo_KTM,
    eLogo_HUSQVRS,
    eLogo_HPN,
    eLogo_Motobau,
    eLogo_vforce,
} LOGO_TYPE;



#define BIKELOGO_ARRAYSIZE  12
#define BIKELOGO_FIRST      0
#define BIKELOGOG_LAST      (BIKELOGO_ARRAYSIZE-1)

#endif // BITMAP_H
