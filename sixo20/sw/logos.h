/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2012-05-24 by Ralf Schwarzer
 *  Project:        SIxO
 *  Module:         display
 *  Purpose:        bitmap raw data resources - logos
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
 * Revision 3.2  2013/03/26 21:08:34  tuberkel
 * New MotoGuzzi-Logo 'rgSIxOLogo_MotoGuzzi_128x56'
 *
 * Revision 3.1  2012/05/24 19:30:03  tuberkel
 * Moved BMP-Logos to 'logos.c'
 *
 *
 *
 ************************************************************************ */


#ifndef LOGOS_H
#define LOGOS_H

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
    eLogo_teneristi,
    eLogo_coolride,
    eLogo_MotoGuzzi   /* @uwe - GUZZI-Version */
} LOGO_TYPE;



#define BIKELOGO_ARRAYSIZE  15
#define BIKELOGO_FIRST      0
#define BIKELOGOG_LAST      (BIKELOGO_ARRAYSIZE-1)

#endif // LOGOS_H
