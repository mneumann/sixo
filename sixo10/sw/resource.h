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
 *  Created:        24-11-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         devices
 *  Purpose:        list of all dialog resources (strings)
 *  Comments:       use this file for all language specific stuff
 *
 ************************************************************************ */
#ifndef _RESOURCE_H
#define _RESOURCE_H


/* main device resources */
#define RESTXT_SPEED_DESC   "km/h"
#define RESTXT_RPM_DESC     "U/Min"
#define RESTXT_DIST_DESC    "km"



/* settings device resources */
#define RESTXT_WHEELSIZEDESC    "Radumfang:"
#define RESTXT_WHEELSIZEUNIT    " mm"
#define RESTXT_VEHICKMDESC      "Gesamt:"
#define RESTXT_VEHICKMUNIT      " km"
#define RESTXT_CCFNOMDESC       "Zyl.-Fakt.:"
#define RESTXT_CCFNOMUNIT       "/"
#define RESTXT_DPLLIGHTDESC     "Licht:"
#define RESTXT_DPLCONTRDESC     "Kontrast:"

#define RESTXT_DPLLIGHTCHOICE_A " an "
#define RESTXT_DPLLIGHTCHOICE_B " aus"
#define RESTXT_DPLLIGHTCHOICE_c "auto"


/* test resources */
#define RESTXT_TEST1    "Eingabetest"
#define RESTXT_TEST2    "12340"
#define RESTXT_TEST3    "BMW"
                      /*!....!....!....!....!.*/
#define RESTXT_TEST4    "Hey Mann, das ist ein" \
                        "sehr langer Text, der" \
                        "aus dem ROM kommt und" \
                        "deshalb das RAM kaum " \
                        "belastet. Trotzdem   " \
                        "sollte er in das def-" \
                        "inierte Fenster pas- " \
                        "sen, allerding etwas " \
                        "wirkürlich umgebroch-" \
                        "en."
                      /*!....!....!....!....!....!....!..*/
#define RESTXT_TEST5    "Hey Mann, das ist ein sehr lan- " \
                        "ger Text, der aus dem ROM kommt " \
                        "und deshalb das RAM kaum belas- " \
                        "tet. Trotzdem sollte er in das  " \
                        "definierte Fenster passen, aller" \
                        "ding etwas willkürlich umgebroch" \
                        "en."
                      /*!....!....!....!.*/
#define RESTXT_TEST6    "  Hey Mann, das " \
                        "  ist ein sehr  " \
                        "  langer Text!  "


#endif /* _RESOURCE_H */

