/***********************************************************************
 *
 *  $RCSfile$
 *
 *  $Revision$
 *
 *  $Date$
 *
 *  $Author$
 *
 *  --------------------------------------------------------------------
 *  Company:        N&K Development, all rights reserved
 *
 *                  Neugebauer & Krizsan Cooperation, Germany
 *                  contact:    arnold.neugebauer@web.de
 *                              ralf.krizsan@web.de
 *  --------------------------------------------------------------------
 *  Created:        25-10-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         standard lib
 *  Purpose:        export header for number to ascii type conversions
 *  Comments:       -
 *
 ************************************************************************ */
#ifndef _XTOA_H
#define _XTOA_H

char far* itoa (  int val, char far* buf, int radix);
char far* ltoa (  long val, char far* buf, int radix);
char far* ultoa ( unsigned long val, char far* buf, int radix);

#endif /* _XTOA_H */