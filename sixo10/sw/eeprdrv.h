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
 *  Created:        17-02-2002 Arnold Neugebauer
 *  Project:        SIxO
 *  Module:         eeprom driver
 *  Purpose:        eeprom api export header
 *  Comments:       - 
 *
 ************************************************************************ */
#ifndef _EEPRDRV_H
#define _EEPRDRV_H


// function prototypes
ERRCODE iicInit( void );
ERRCODE iicEepromWrite(UINT16 EepromTrgtAddr, UINT16 nBytes, UINT8 * pSrcAddr);
ERRCODE iicEepromRead(UINT16 EepromSrcAddr, UINT16 nBytes, UINT8 * pTrgtAddr);

//test functions only present in the debug version
#ifdef DEBUG
   ERRCODE iicTestEeprom( void );
   ERRCODE iicTestEepromEmu( void );
   BOOL iic_snd_test( unsigned char *adr_buffer,
                      unsigned char number_of_adr,
                      unsigned char *dat_buffer,
                      unsigned char number_of_dat );
   BOOL iic_rcv_test( unsigned char *adr_buffer,
                      unsigned char number_of_adr,
                      unsigned char *dat_buffer,
                      unsigned char number_of_dat );
#endif


#endif //_EEPRDRV_H
