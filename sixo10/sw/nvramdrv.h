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
 *  Created:        16-02-2002 by Arnold Neugebauer
 *  Project:        SIxO
 *  Module:         non volatile ram driver
 *  Purpose:        non volatile ram api export header
 *  Comments:
 ************************************************************************ */

#ifndef _NVRAMDRV_H
#define _NVRAMDRV_H


//function prototypes
ERRCODE iicInit( void );
ERRCODE iicNvramWrite( UINT8 NvramTrgtAddr, UINT8 nBytes, UINT8* pSrcAddr );
ERRCODE iicNvramRead( UINT8 NvramSrcAddr, UINT8 nBytes, UINT8* pTrgtAddr );

//test functions only present in the debug version
#ifdef DEBUG
   ERRCODE iicTestNvram( void );
#endif


#endif //_NVRAMDRV_H
