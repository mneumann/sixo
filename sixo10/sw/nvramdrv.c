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
 *  Purpose:        non volatile ram api functions
 *  Comments:       Physically the non volatile ram and the clock is
 *                  located in one chip accessed over the iic bus.
 *                  The iicbb low level routines are used. iicInit of
 *                  the module eeprdrv must be called before calling any
 *                  of the functions of this module.
 ************************************************************************ */


#include "standard.h"
#include "nvramdrv.h"
#include "iicbb.h"


//for the current hardware implementation IIC_NVRAM_DEV_ADDR and
//IIC_CLK_DEV_ADR are the same because the clock and the nvram are
//located physically in the same device
#define IIC_NVRAM_DEV_ADDR 0x68  //right aligned, highest bit doesn't care

//offset between first register of the device (subaddress 0) and first
//nvram location
#define IIC_NVRAM_OFFSET   8

#define NVRAMSIZE 56


/***********************************************************************
 *  FUNCTION:       iicNvramWrite
 *  DESCRIPTION:    write process to nvram over iic bus
 *  PARAMETER:      NvramTrgtAddr   nvram target adress 0..(NVRAMSIZE-1)
 *                  nBytes          number of bytes to transfer
 *                                  1..(NVRAMSIZE-NvramTrgtAddr)
 *                  pSrcAddr        uC ram source pointer
 *  RETURN:         ERR_OK          success
 *                  ERR_NVRAM_NOWRITE  write access failed, no acknowledge
 *                  ERR_PARAM_ERR      invalid parameter
 *  COMMENT:        An offset of IIC_NVRAM_OFFSET is added to the given
 *                  target address because nvram starts physically at
 *                  subaddress IIC_NVRAM_OFFSET.
 *                  In case of a power down the hardware terminates an
 *                  access in progress and write protects itself to prevent
 *                  erroneous data being written to the nvram.
 *                  Data consistency is beeing garanteed on a byte level,
 *                  i.e. a byte access will be completed (right before the
 *                  power down) or not (right after or during the power
 *                  down). On the other hand this means a multiple byte
 *                  access in progress during a power down may write only
 *                  some of its bytes to the nvram while the rest is
 *                  rejected.
 *********************************************************************** */
ERRCODE iicNvramWrite( UINT8 NvramTrgtAddr, UINT8 nBytes, UINT8* pSrcAddr )
{
   UINT8 iic_adr_buffer[2];

   //check parameters
   if( (  nBytes  == 0 )
      || ( (NvramTrgtAddr + nBytes) >  NVRAMSIZE )
      || ( pSrcAddr == NULL )
   ) return ERR_PARAM_ERR;

   //send data over iic bus
   iic_adr_buffer[0] = IIC_NVRAM_DEV_ADDR;
   iic_adr_buffer[1] = NvramTrgtAddr + IIC_NVRAM_OFFSET;

   if( !iic_snd( iic_adr_buffer,
                 2,              //number of addresses: dev addr and sub addr
                 pSrcAddr,
                 nBytes )        //number of bytes to transmit
   ) return ERR_NVRAM_NOWRITE;

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       iicNvramRead
 *  DESCRIPTION:    read process from nvram over iic bus
 *  PARAMETER:      NvramSrcAddr nvram source address 0..(NVRAMSIZE-1)
 *                  nBytes       number of bytes to be read
 *                               1..(NVRAMSIZE-NvramTrgtAddr)
 *                  pTrgtAddr    ptr to uC target ram
 *  RETURN:         ERR_OK             success
 *                  ERR_NVRAM_NOREAD   read access failed, no acknowledge
 *                  ERR_PARAM_ERR      invalid parameter
 *  COMMENT:        an offset of IIC_NVRAM_OFFSET is added to the given
 *                  target address because nvram starts physically at
 *                  subaddress IIC_NVRAM_OFFSET.
 *********************************************************************** */
ERRCODE iicNvramRead( UINT8 NvramSrcAddr, UINT8 nBytes, UINT8* pTrgtAddr)
{
   UINT8 iic_adr_buffer[2];

   //check parameters
   if( (  nBytes  == 0 )
      || ( (NvramSrcAddr + nBytes) >  NVRAMSIZE )
      || ( pTrgtAddr == NULL )
   ) return ERR_PARAM_ERR;

   //read data from iic bus
   iic_adr_buffer[0] = IIC_NVRAM_DEV_ADDR;
   iic_adr_buffer[1] = NvramSrcAddr + IIC_NVRAM_OFFSET;

   if( !iic_rcv( iic_adr_buffer,
                 2,              //number of addresses: dev addr and sub addr
                 pTrgtAddr,
                 nBytes )        //number of bytes to transmit
   ) return ERR_NVRAM_NOREAD;

   return ERR_OK;
}


//======================================================================
//=
//= the following is test code, only compiled in the debug version
//=
//======================================================================

#ifdef DEBUG
/***********************************************************************
 *  FUNCTION:       iicTestNvram
 *  DESCRIPTION:    selfchecking test routines for iicNvram{Write|Read}
 *  PARAMETER:      none
 *  RETURN:         ERR_OK             success
 *                  ERR_NVRAM_NOWRITE  write access failed, no acknowledge
 *                  ERR_NVRAM_NOREAD   read access failed, no acknowledge
 *                  ERR_PARAM_ERR      invalid parameter
 *  COMMENT:
 *********************************************************************** */
ERRCODE iicTestNvram( void )
{
   ERRCODE err;
   UINT8 szString[] = {
      "this is a string with at least 56 characters equal to nvram size"
   };
   UINT8 szBuffer[56];
   UINT8 dIndex;
   UINT8 *pNotInit = NULL;

   //write and read back 56 bytes
   err = iicNvramWrite( 0, 56, szString );
   if( err != ERR_OK ) return err;
   err = iicNvramRead( 0, 56, szBuffer );
   if( err != ERR_OK ) return err;

   for( dIndex = 0; dIndex < 56; dIndex++ ){
      if( szString[dIndex] != szBuffer[dIndex] ) return ERR_NVRAM_NOREAD;
   }

   //write 10 times 0xaa
   for( dIndex = 0; dIndex < 10; dIndex++ ){
      szBuffer[dIndex] = 0xaa;
   }
   err = iicNvramWrite( 20, 10, szBuffer );
   if( err != ERR_OK ) return err;

   //write 3 times 0x55 with single accesses
   szBuffer[0] = 0x55;
   err = iicNvramWrite( 30, 1, szBuffer );
   if( err != ERR_OK ) return err;
   err = iicNvramWrite( 31, 1, szBuffer );
   if( err != ERR_OK ) return err;
   err = iicNvramWrite( 32, 1, szBuffer );
   if( err != ERR_OK ) return err;

   //read back ram contents
   err = iicNvramRead( 0, 56, szBuffer );
   if( err != ERR_OK ) return err;

   for( dIndex = 0; dIndex < 20; dIndex++ ){
      if( szString[dIndex] != szBuffer[dIndex] ) return ERR_NVRAM_NOREAD;
   }

   for( dIndex = 20; dIndex < 30; dIndex++ ){
      if( szBuffer[dIndex] != 0xaa ) return ERR_NVRAM_NOREAD;
   }

   for( dIndex = 30; dIndex < 33; dIndex++ ){
      if( szBuffer[dIndex] != 0x55 ) return ERR_NVRAM_NOREAD;
   }

   for( dIndex = 33; dIndex < 56; dIndex++ ){
      if( szBuffer[dIndex] != szBuffer[dIndex] ) return ERR_NVRAM_NOREAD;
   }

   //read back some bytes with single accesses
   err = iicNvramRead( 0, 1, szBuffer );
   if( err != ERR_OK ) return err;
   if( szBuffer[0] != 't' ) return ERR_NVRAM_NOREAD;

   err = iicNvramRead( 29, 1, szBuffer );
   if( err != ERR_OK ) return err;
   if( szBuffer[0] != 0xaa ) return ERR_NVRAM_NOREAD;

   err = iicNvramRead( 32, 1, szBuffer );
   if( err != ERR_OK ) return err;
   if( szBuffer[0] != 0x55 ) return ERR_NVRAM_NOREAD;

   err = iicNvramRead( 55, 1, szBuffer );
   if( err != ERR_OK ) return err;
   if( szBuffer[0] != 'v' ) return ERR_NVRAM_NOREAD;

   //-------------------------------------------------------------------

   //test parameter check, function should return error
   err = iicNvramWrite( 56, 1, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   
   err = iicNvramWrite( 20, 0, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicNvramWrite( 0, 57, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicNvramWrite( 20, 37, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicNvramWrite( 20, 1, pNotInit );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   //-------------------------------------------------------------------

   //test parameter check, function should return OK
   err = iicNvramWrite( 55, 1, szBuffer );
   if( err != ERR_OK ) return err;

   err = iicNvramWrite( 20, 36, szBuffer );
   if( err != ERR_OK ) return err;

   //-------------------------------------------------------------------

   //test parameter check, function should return error
   err = iicNvramRead( 56, 1, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;
   
   err = iicNvramRead( 20, 0, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicNvramRead( 0, 57, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicNvramRead( 20, 37, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicNvramRead( 20, 1, pNotInit );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   //-------------------------------------------------------------------

   //test parameter check, function should return OK
   err = iicNvramRead( 55, 1, szBuffer );
   if( err != ERR_OK ) return err;

   err = iicNvramRead( 20, 36, szBuffer );
   if( err != ERR_OK ) return err;


   return ERR_OK;
}

#endif //DEBUG
