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
 *  Created:        22-10-2001 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         diagnosys
 *  Purpose:        diagnostic export header
 *  Comments:       -
 *
 ************************************************************************ */

#ifndef _DIAGNOSE_H
#define _DIAGNOSE_H

enum
{
    DIAGDATA_USPUSE,
    DIAGDATA_MAXHEAPUSE,
    DIAGDATA_TIMERISRLOAD,
    DIAGDATA_UNEXPISRCALLS,
    DIAGDATA_ISPUSE,
    DIAGDATA_MAXMSGQ
};

void DisplayDiagnose(void);
void CheckActFreeSysRes(void);
void StandardDiagnoseScreen(void);
void GetDiagnoseData(UINT8 DataVar, STRING pDataVal);

#endif /* _DIAGNOSE_H */