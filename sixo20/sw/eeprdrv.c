/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2002-02-17 Arnold Neugebauer
 *  Project:        SIxO
 *  Module:         eeprom driver
 *  Purpose:        eeprom api functions
 *  Comments:       The system timer is used and must be initialised by a
 *                  a call to TimerInit of the module timer.c before calling
 *                  any of the other functions of this module (especially
 *                  call TimerInit before iicInit).
 *                  The iicbb low level routines are used. iicInit of this
 *                  module has to be called before calling any of the other
 *                  functions.
 *                  The two prototypes are equiped with ISSI 24C01-3 (marked
 *                  01-3G, 01=organisation, 3=voltage/speed, G=package). The
 *                  organisation actually is 1Kbit = 1 * 128 byte. This is
 *                  likely to change. Different eeproms are possible on the
 *                  same footprint. The functions in this module are written
 *                  for organisations of 1 x 128 bytes and n x 256 byte.
 *                  The type of eeprom is automatically determined in the
 *                  function iicInit.
 *                  Automatically detecting 128 byte eeproms is suspicious
 *                  since the detection algorithm has to write to the eeprom
 *                  and a power down before writing back the original values
 *                  alters eeprom contents.
 *                  The speedgrade actually is 400KHz but the bus on the
 *                  SIxO runs always with 100KHz due to the speedgrade of
 *                  the other iic device M41T11 (clock and nvram).
 *                  The following eeproms are supported:
 *
 *                  type   size   organisation   page size
 *                  24C01  1Kbit  1 x 128bytes   8bytes
 *                  24C02  2Kbit  1 x 256bytes   8bytes
 *                 (24C04  4Kbit  2 x 256bytes   8bytes available?)
 *                  24C08  8Kbit  4 x 256bytes  16bytes
 *                  24C16 16Kbit  8 x 256bytes  16bytes
 *                 (24C32 32Kbit 16 x 256bytes  16bytes available?)
 *
 *                 The write and read functions are optimized for perfor-
 *                 mance in that they are using page write and sequential
 *                 read whenever possible.
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
 ************************************************************************ */



#include "sfr62p.h"
#include "standard.h"
#include "eeprdrv.h"
#include "iicbb.h"
#include "timer.h"
#include "debug.h"

//iic address of eeprom
#define IIC_EEPROM_DEV_ADDR 0x50  //right aligned, highest bit doesn't care


//global variables
//if eepromsize is zero, pagesize and pagemask are not initialized
UINT16 wEepromSize;
UINT16 wPageSize;
UINT16 wPageMask;
extern  UINT16  wMilliSecCounter; //system timer of the module timer.c

#if(DEBUG==1)
   //for test eeprom emulated by iic_{snd|rcv}_test
   UINT16 wTestSize;
   UINT8  test_eeprom[4096];
#endif


/***********************************************************************
 *  FUNCTION:       iicInit
 *  DESCRIPTION:    Inits SDA and SCL to input and 0. Determines eeprom
 *                  size.
 *  PARAMETER:      none
 *  RETURN:         ERR_OK success
 *  COMMENT:        Accesses all bits of port 7 data and direction
 *                  registers. See comment below.
 *                  See module header for supported eeprom types.
 *********************************************************************** */
ERRCODE iicInit( void )
{
   ERRCODE err;
   UINT16  wByteAddress;
   UINT8   dummy;
   UINT8   data7f;
   UINT8   dataff;

   //to prevent a possible low on the bus lines set to input first
   pd7 = 0x00;

   //Set SCL and SDA (bit 1 and 0) in the port P7 data register to low.
   //The higher bits are also effected but a read-modify-write is only
   //possible if all the port bits are set to output. Thus modifying
   //these bits can only be done if the overall software/hardware design
   //is known. If other modules require port 7 this must be taken into
   //account.
   p7 = 0x00;

   //determine size of iic eeprom in bytes
   wEepromSize = 4096; //set to biggest supported eeprom to avoid
                       //error by parameter checking in iicEepromRead

   if( iicEepromRead( 0, 1, &dummy ) != ERR_OK ){ //read at address 0
      wEepromSize = 0; //no eeprom detected
   }
   else{
      //the following reads at adresses 0x100, 0x200, .., 0x800
      wByteAddress = 0x80;
      do{
         wByteAddress <<= 1;
         err = iicEepromRead( wByteAddress, 1, &dummy );
      }while( (err==ERR_OK) && (wByteAddress<=0x400) );

      if( err==ERR_OK )
        wByteAddress <<= 1;
      wEepromSize = wByteAddress;

      //set page size according to eeprom type
      if( wEepromSize <= 512 ){
         wPageSize = 8;
      }
      else{
         wPageSize = 16;
      }

      wPageMask = ~( wPageSize - 1 );

      //-------- delete this if not handling 128 byte eeproms -----
      //Special for 128 byte eeproms.
      //The subaddress simply rolls over. We have to write to the
      //eeprom. This is suspicious since a power down before writing
      //back the original values alters eeprom contents.
      if( wEepromSize == 256 )
      {
         //determine if its a 128 byte eeprom
         //before the write aboard everything if an error occurs, after
         //the first write try to write back the original value in any case.
         if( iicEepromRead( 0x7f, 1, &data7f ) != ERR_OK )
            return ERR_OK;
         if( iicEepromRead( 0xff, 1, &dataff ) != ERR_OK )
            return ERR_OK;
         //if data is equal it may be a 128 byte eeprom
         if( data7f == dataff ){
            //alter contents
            dummy = dataff + 1;
            if( iicEepromWrite( 0xff, 1, &dummy ) != ERR_OK )
                return ERR_OK;
            //if address 7f is altered in the same way its a 128 byte eeprom
            iicEepromRead( 0x7f, 1, &data7f );
            //write back original value
            iicEepromWrite( 0xff, 1, &dataff );
            if( data7f == dummy )
                wEepromSize = 128;
         }
      }
      //-------- delete this if not handling 128 byte eeproms -----


   }
   ODS1(DBG_DRV,DBG_INFO,"iicInit() done! (EEPROM: %u bytes available)", wEepromSize);
   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       iicEepromSize
 *  DESCRIPTION:    publishes wEepromSize
 *  PARAMETER:      none
 *  RETURN:         size of the eeprom in bytes determined during
 *                  iicInit(), 0 if no eeprom has been detected (yet)
 *  COMMENT:
 *********************************************************************** */
UINT16 iicEepromSize( void )
{
   return wEepromSize;
}


/***********************************************************************
 *  FUNCTION:       iicEepromWrite
 *  DESCRIPTION:    write process to eeprom over iic bus
 *  PARAMETER:      EepromTrgtAddr   eeprom target adress
 *                  nBytes           number of bytes to transfer
 *                  pSrcAddr         uC ram source pointer
 *  RETURN:         ERRCODE   ERR_OK                write successfull
 *                            ERR_EEPROM_NOWRITE    write not successfull
 *                            ERR_PARAM_ERR         invalid parameter
 *  COMMENT:        A write access to the eeprom needs at least one programming
 *                  cycle of 5ms. The function returns immediately after all
 *                  bytes are send to the eeprom and before the last programming
 *                  cycle. The eeprom may be blocked another 5ms. A following
 *                  write cylce will be delayed until the eeprom is ready again.
 *                  A complete page write (16 bytes) can take up to 6.6ms.
 *********************************************************************** */
ERRCODE iicEepromWrite(UINT16 EepromTrgtAddr, UINT16 nBytes, UINT8 * pSrcAddr)
{
   UINT16 wNextPage;
   UINT16 wWriteLength;
   UINT16 wStartMs;
   UINT16 wNowMs;
   UINT8  iic_adr_buffer[2];
   BOOL   bSuccess;

   //check parameters
   if( (  nBytes  == 0 )
      || ( (EepromTrgtAddr + nBytes) >  wEepromSize )
      || ( pSrcAddr == NULL )
   ) return ERR_PARAM_ERR;

   wNextPage = ( EepromTrgtAddr & wPageMask ) + wPageSize;
   wWriteLength = wNextPage - EepromTrgtAddr;

   do{
      if( wWriteLength > nBytes ) wWriteLength = nBytes;

      //highest nibble is device address, lower nibbles are subaddress
      iic_adr_buffer[0] = IIC_EEPROM_DEV_ADDR | ( (EepromTrgtAddr & 0xFF00) >> 8 );
      iic_adr_buffer[1] = EepromTrgtAddr & 0x00FF;

      //Transmit bytes, wait for acknowledge or timeout.
      //Worst case is about 6.6ms for a complete page write (162bit+start+stop *
      //10us + 5ms prog cycle). If the bus is set slower (halfbittime in iicbb.a30
      //= 8us) we have ~7.6ms -> 8ms. Another ms is added due to the inaccuracy
      //between asynchronous processes (access of wMilliSecCounter) -> 9ms.
      //(To handle the rollover of the millisecond counter correctly the
      //subtraction now-start produces 9 as the result of ff-f6, 00-f7,
      //01-f8, 02-f9, ..., 07-fe, 08-ff, 09-00)
      TimerGetSys_msec( wStartMs );
      do{
         bSuccess = iic_snd( iic_adr_buffer,
                             2,              //number of addresses: dev addr and sub addr
                             pSrcAddr,
                             wWriteLength ); //number of bytes to transmit
         TimerGetSys_msec( wNowMs );
      }while( !bSuccess && ( (wNowMs - wStartMs) < 9 ) ); //sub takes rollover into account

      if( !bSuccess ) return ERR_EEPROM_NOWRITE;

      EepromTrgtAddr = wNextPage;
      wNextPage += wPageSize;
      pSrcAddr += wWriteLength;
      nBytes -= wWriteLength;
      wWriteLength = wPageSize;

   }while( nBytes > 0 );

   return ERR_OK;
}



/***********************************************************************
 *  FUNCTION:       iicEepromRead
 *  DESCRIPTION:    read process from eeprom over iic bus
 *  PARAMETER:      EepromSrcAddr   eeprom source address
 *                  nBytes          number of bytes to be read
 *                  pTrgtAddr       ptr to uC target ram
 *  RETURN:         ERRCODE   ERR_OK                read successfull
 *                            ERR_EEPROM_NOREAD     read not successfull
 *                            ERR_PARAM_ERR         invalid parameter
 *  COMMENT:
 *********************************************************************** */
ERRCODE iicEepromRead(UINT16 EepromSrcAddr, UINT16 nBytes, UINT8 * pTrgtAddr)
{
   UINT16 wNextSegment;
   UINT16 wReadLength;
   UINT16 wStartMs;
   UINT16 wNowMs;
   UINT8  iic_adr_buffer[2];
   BOOL   bSuccess;

   //check parameters
   if( (  nBytes  == 0 )
      || ( (EepromSrcAddr + nBytes) >  wEepromSize )
      || ( pTrgtAddr == NULL )
   ) return ERR_PARAM_ERR;

   wNextSegment = ( EepromSrcAddr & 0xFF00 ) + 0x100;
   wReadLength = wNextSegment - EepromSrcAddr;

   do{
      if( wReadLength > nBytes ) wReadLength = nBytes;

      //highest nibble is device address, lower nibbles are subaddress
      iic_adr_buffer[0] = IIC_EEPROM_DEV_ADDR | ( (EepromSrcAddr & 0xFF00) >> 8 );
      iic_adr_buffer[1] = EepromSrcAddr & 0x00FF;

      //Transmit bytes, wait for acknowledge or timeout.
      //See calculation of timeout in iicEepromWrite.
      //(To handle the rollover of the millisecond counter correctly the
      //subtraction now-start produces 9 as the result of ff-f6, 00-f7,
      //01-f8, 02-f9, ..., 07-fe, 08-ff, 09-00)
      TimerGetSys_msec( wStartMs );
      do{
         bSuccess = iic_rcv( iic_adr_buffer,
                             2,             //number of addresses: dev addr and sub addr
                             pTrgtAddr,
                             wReadLength ); //number of bytes to transmit
         TimerGetSys_msec( wNowMs );
      }while( !bSuccess && ( (wNowMs - wStartMs) < 9 ) ); //sub takes rollover into account

      if( !bSuccess ) return ERR_EEPROM_NOREAD;

      EepromSrcAddr = wNextSegment;
      wNextSegment += 0x100;
      pTrgtAddr += wReadLength;
      nBytes -= wReadLength;
      wReadLength = 0x100;

   }while( nBytes > 0 );

   return ERR_OK;
}


//======================================================================
//=
//= the following is test code, only compiled in the debug version
//=
//======================================================================

#if(DEBUG==1)
/***********************************************************************
 *  FUNCTION:       iicTestEeprom
 *  DESCRIPTION:    selfchecking test routines for iicEeprom{Write|Read}
 *  PARAMETER:      none
 *  RETURN:         ERR_OK             success
 *                  ERR_EEPROM_NOWRITE write access failed, no acknowledge
 *                  ERR_EEPROM_NOREAD  read access failed, no acknowledge
 *                  ERR_PARAM_ERR      invalid parameter
 *  COMMENT:        Tests one eeprom type.
 *                  iicInit has to be called before calling this function.
 *                  The tests are done on the hardware or with the emulation
 *                  functions (replace iic_snd by iic_snd_test and iic_rcv
 *                  by iic_rcv_test in iicEeprom{Write|Read}).
 *********************************************************************** */
ERRCODE iicTestEeprom( void )
{
   ERRCODE err;
   UINT16 wAddr;
   UINT16 wLen;
   UINT16 wIndex;
   UINT16 wA;
   UINT8 szString[] = {
      "this is a string with 35 characters"
   };
   UINT8 szBuffer[35];
   UINT8 *pNotInit = NULL;


   //fill eeprom with test pattern
   wAddr = 0;
   wLen = 35;
   do{
      err = iicEepromWrite( wAddr, wLen, szString );
      if( err != ERR_OK ) return err;
      wAddr += wLen;
      if( (wAddr + wLen) > wEepromSize ) wLen = wEepromSize - wAddr;
   }while( wAddr < wEepromSize );

   //read back test pattern
   wAddr = 0;
   wLen = 35;
   do{
      err = iicEepromRead( wAddr, wLen, szBuffer );
      if( err != ERR_OK ) return err;
      for( wIndex = 0; wIndex < wLen; wIndex++ ){
         if( szBuffer[wIndex] != szString[wIndex] ) return ERR_EEPROM_NOREAD;
         szBuffer[wIndex] = 0; //clear buffer
      }
      wAddr += wLen;
      if( (wAddr + wLen) > wEepromSize ) wLen = wEepromSize - wAddr;
   }while( wAddr < wEepromSize );

   //write at page boundaries (before, over, after)
   szBuffer[0] = 0x55;
   szBuffer[1] = 0xaa;
   szBuffer[2] = 0xbb;

   wAddr = wEepromSize - 3 * wPageSize - 3;
   err = iicEepromWrite( wAddr, 3, szBuffer );
   if( err != ERR_OK ) return err;
   err = iicEepromRead( wAddr, 3, szBuffer );
   if( err != ERR_OK ) return err;
   if( (szBuffer[0] != 0x55)
      || (szBuffer[1] != 0xaa)
      || (szBuffer[2] != 0xbb)
   ) return ERR_EEPROM_NOREAD;

   wAddr = wEepromSize - 2 * wPageSize - 1;
   err = iicEepromWrite( wAddr, 3, szBuffer );
   if( err != ERR_OK ) return err;
   err = iicEepromRead( wAddr, 3, szBuffer );
   if( err != ERR_OK ) return err;
   if( (szBuffer[0] != 0x55)
      || (szBuffer[1] != 0xaa)
      || (szBuffer[2] != 0xbb)
   ) return ERR_EEPROM_NOREAD;

   wAddr = wEepromSize - wPageSize + 1;
   err = iicEepromWrite( wAddr, 3, szBuffer );
   if( err != ERR_OK ) return err;
   err = iicEepromRead( wAddr, 3, szBuffer );
   if( err != ERR_OK ) return err;
   if( (szBuffer[0] != 0x55)
      || (szBuffer[1] != 0xaa)
      || (szBuffer[2] != 0xbb)
   ) return ERR_EEPROM_NOREAD;

   //read at segment boundary (before, over, after)
   if( wEepromSize >= 512 ){
      szBuffer[0] = 0x55;
      szBuffer[1] = 0xaa;
      szBuffer[2] = 0xbb;

      wAddr = wEepromSize - 259;
      err = iicEepromWrite( wAddr, 3, szBuffer );
      if( err != ERR_OK ) return err;
      err = iicEepromRead( wAddr, 3, szBuffer );
      if( err != ERR_OK ) return err;
      if( (szBuffer[0] != 0x55)
         || (szBuffer[1] != 0xaa)
         || (szBuffer[2] != 0xbb)
      ) return ERR_EEPROM_NOREAD;

      wAddr = wEepromSize - 257;
      err = iicEepromWrite( wAddr, 3, szBuffer );
      if( err != ERR_OK ) return err;
      err = iicEepromRead( wAddr, 3, szBuffer );
      if( err != ERR_OK ) return err;
      if( (szBuffer[0] != 0x55)
         || (szBuffer[1] != 0xaa)
         || (szBuffer[2] != 0xbb)
      ) return ERR_EEPROM_NOREAD;

      wAddr = wEepromSize - 255;
      err = iicEepromWrite( wAddr, 3, szBuffer );
      if( err != ERR_OK ) return err;
      err = iicEepromRead( wAddr, 3, szBuffer );
      if( err != ERR_OK ) return err;
      if( (szBuffer[0] != 0x55)
         || (szBuffer[1] != 0xaa)
         || (szBuffer[2] != 0xbb)
      ) return ERR_EEPROM_NOREAD;
   }

   //read back eeprom contents
   wAddr = 0;
   wLen = 35;
   do{
      err = iicEepromRead( wAddr, wLen, szBuffer );
      if( err != ERR_OK ) return err;
      for( wIndex = 0; wIndex < wLen; wIndex++ ){
         if( szBuffer[wIndex] != szString[wIndex] ){
            err = ERR_EEPROM_NOREAD;
            wA =  wAddr + wIndex;
            if( (wA == (wEepromSize - 259)) && (szBuffer[wIndex] == 0x55) ) err = ERR_OK;
            if( (wA == (wEepromSize - 258)) && (szBuffer[wIndex] == 0xaa) ) err = ERR_OK;
            if( (wA == (wEepromSize - 257)) && (szBuffer[wIndex] == 0x55) ) err = ERR_OK;
            if( (wA == (wEepromSize - 256)) && (szBuffer[wIndex] == 0xaa) ) err = ERR_OK;
            if( (wA == (wEepromSize - 255)) && (szBuffer[wIndex] == 0x55) ) err = ERR_OK;
            if( (wA == (wEepromSize - 254)) && (szBuffer[wIndex] == 0xaa) ) err = ERR_OK;
            if( (wA == (wEepromSize - 253)) && (szBuffer[wIndex] == 0xbb) ) err = ERR_OK;
            if( (wA == (wEepromSize - 3 * wPageSize - 3)) && (szBuffer[wIndex] == 0x55) ) err = ERR_OK;
            if( (wA == (wEepromSize - 3 * wPageSize - 2)) && (szBuffer[wIndex] == 0xaa) ) err = ERR_OK;
            if( (wA == (wEepromSize - 3 * wPageSize - 1)) && (szBuffer[wIndex] == 0xbb) ) err = ERR_OK;
            if( (wA == (wEepromSize - 2 * wPageSize - 1)) && (szBuffer[wIndex] == 0x55) ) err = ERR_OK;
            if( (wA == (wEepromSize - 2 * wPageSize)) && (szBuffer[wIndex] == 0xaa) ) err = ERR_OK;
            if( (wA == (wEepromSize - 2 * wPageSize + 1)) && (szBuffer[wIndex] == 0xbb) ) err = ERR_OK;
            if( (wA == (wEepromSize - wPageSize + 1)) && (szBuffer[wIndex] == 0x55) ) err = ERR_OK;
            if( (wA == (wEepromSize - wPageSize + 2)) && (szBuffer[wIndex] == 0xaa) ) err = ERR_OK;
            if( (wA == (wEepromSize - wPageSize + 3)) && (szBuffer[wIndex] == 0xbb) ) err = ERR_OK;
            if( err != ERR_OK ) return err;
         }
      }
      wAddr += wLen;
      if( (wAddr + wLen) > wEepromSize ) wLen = wEepromSize - wAddr;
   }while( wAddr < wEepromSize );

   //-------------------------------------------------------------------

   //test parameter check, function should return error
   err = iicEepromWrite( wEepromSize, 1, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicEepromWrite( 20, 0, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicEepromWrite( 0, wEepromSize + 1, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicEepromWrite( wEepromSize - 36, 37, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicEepromWrite( 20, 1, pNotInit );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   //-------------------------------------------------------------------

   //test parameter check, function should return OK
   err = iicEepromWrite( wEepromSize - 1, 1, szBuffer );
   if( err != ERR_OK ) return err;

   err = iicEepromWrite( wEepromSize - 36, 36, szBuffer );
   if( err != ERR_OK ) return err;

   //-------------------------------------------------------------------

   //test parameter check, function should return error
   err = iicEepromRead( wEepromSize, 1, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicEepromRead( 20, 0, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicEepromRead( 0, wEepromSize + 1, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicEepromRead( wEepromSize - 36, 37, szBuffer );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   err = iicEepromRead( 20, 1, pNotInit );
   if( err != ERR_PARAM_ERR ) return ERR_PARAM_ERR;

   //-------------------------------------------------------------------

   //test parameter check, function should return OK
   err = iicEepromRead( wEepromSize - 1, 1, szBuffer );
   if( err != ERR_OK ) return err;

   err = iicEepromRead( wEepromSize - 36, 36, szBuffer );
   if( err != ERR_OK ) return err;


   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       iicTestEepromEmu
 *  DESCRIPTION:    selfchecking test routines for iicEeprom{Write|Read}
 *                  with emulation of all supported eeprom types.
 *  PARAMETER:      none
 *  RETURN:         ERR_OK             success
 *                  ERR_EEPROM_NOWRITE write access failed, no acknowledge
 *                  ERR_EEPROM_NOREAD  read access failed, no acknowledge
 *                  ERR_PARAM_ERR      invalid parameter
 *  COMMENT:        Tests iicInit. Loops over all possible eeprom types.
 *                  Assumes that all accesses to the iic low level driver
 *                  are replaced by the corresponding emulation functions
 *                  (iic_snd -> iic_snd_test and iic_rcv -> iic_rcv_test).
 *********************************************************************** */
ERRCODE iicTestEepromEmu( void )
{
   ERRCODE err;
   UINT8   data7f;
   UINT8   dataff;

   //test iicInit
   wTestSize = 0;
   iicInit();
   if( wEepromSize != 0 ) return ERR_EEPROM_NOREAD;

   wTestSize = 0x80;

   do{
      //init address 0x7f and 0xff for test of special
      //128byte eeprom code in iicInit
      test_eeprom[0x7f] = 0xaa;
      test_eeprom[0xff] = 0x55;

      iicInit();

      //check if adresses 0x7f and 0xff are unchanged
      //(special code for 128byte eeproms should not alter these adresses)
      if( (test_eeprom[0x7f] != 0xaa) || (test_eeprom[0xff] != 0x55) )
         return ERR_EEPROM_NOREAD;

      if( wEepromSize != wTestSize ) return ERR_EEPROM_NOREAD;
      if( wEepromSize <= 0x200 ){
         if( (wPageSize != 8) || (wPageMask != 0xFFF8) ) return ERR_EEPROM_NOREAD;
      }
      else{
         if( (wPageSize != 16) || (wPageMask != 0xFFF0) ) return ERR_EEPROM_NOREAD;
      }
      wTestSize <<= 1;
   }while( wTestSize <= 0x1000 );

   //write and read test for all eeprom sizes
   for( wTestSize = 0x80; wTestSize <= 0x1000; wTestSize <<= 1 ){
      iicInit();  //init wEepromSize, wPageSize, wPageMask
      err = iicTestEeprom();
      if( err != ERR_OK ) return err;
   }

   return ERR_OK;
}


/***********************************************************************
 *  FUNCTION:       iic_snd_test
 *  DESCRIPTION:    emulates behaviour of iic_snd when accessing an eeprom
 *  PARAMETER:      same as iic_snd, see there
 *  RETURN:         TRUE   success
 *                  FALSE  no acknowledge
 *  COMMENT:        Different eeprom sizes are determined by the global
 *                  variable wTestSize. Set wTestSize before entering
 *                  this function.
 *********************************************************************** */
BOOL iic_snd_test( unsigned char *adr_buffer,
                   unsigned char number_of_adr,
                   unsigned char *dat_buffer,
                   unsigned char number_of_dat )
{
   UINT16 wAddr;
   UINT16 wSrcAddr;

   //Special for 128 byte eeproms. Subaddress rolls over.
   if( wTestSize == 128 ){
      adr_buffer[1] &= 0x7f;
   }

   //parameter check
   if( (number_of_adr != 2)
      || ( number_of_dat == 0 )
      || ( number_of_dat > wPageSize )
      || ( adr_buffer == NULL )
      || ( dat_buffer == NULL )
      || ( (adr_buffer[0] & 0xF0) != IIC_EEPROM_DEV_ADDR )
   ) return FALSE;

   wAddr = ( (adr_buffer[0] & 0x0F) << 8 ) + adr_buffer[1];
   if( (wAddr + number_of_dat) > wTestSize ) return FALSE;

   wSrcAddr = 0;

   do{
      test_eeprom[wAddr] = dat_buffer[wSrcAddr];
      wAddr++;
      wSrcAddr++;
   }while( wSrcAddr < number_of_dat );

   return TRUE;
}


/***********************************************************************
 *  FUNCTION:       iic_rcv_test
 *  DESCRIPTION:    emulates behaviour of iic_rcv when accessing an eeprom
 *  PARAMETER:      same as iic_rcv, see there
 *  RETURN:         TRUE   success
 *                  FALSE  no acknowledge
 *  COMMENT:        Different eeprom sizes are determined by the global
 *                  variable wTestSize. Set wTestSize before entering
 *                  this function.
 *********************************************************************** */
BOOL iic_rcv_test( unsigned char *adr_buffer,
                   unsigned char number_of_adr,
                   unsigned char *dat_buffer,
                   unsigned char number_of_dat )
{
   UINT16 wAddr;
   UINT16 wTrgtAddr;

   //Special for 128 byte eeproms. Subaddress rolls over.
   if( wTestSize == 128 ){
      adr_buffer[1] &= 0x7f;
   }

   //parameter check
   if( (number_of_adr != 2)
      || ( number_of_dat == 0 )
      || ( adr_buffer == NULL )
      || ( dat_buffer == NULL )
      || ( (adr_buffer[0] & 0xF0) != IIC_EEPROM_DEV_ADDR )
   ) return FALSE;

   wAddr = ( (adr_buffer[0] & 0x0F) << 8 ) + adr_buffer[1];
   if( (wAddr + number_of_dat) > wTestSize ) return FALSE;

   wTrgtAddr = 0;

   do{
      dat_buffer[wTrgtAddr] = test_eeprom[wAddr];
      wAddr++;
      wTrgtAddr++;
   }while( wTrgtAddr < number_of_dat );

   return TRUE;
}


#endif //DEBUG
