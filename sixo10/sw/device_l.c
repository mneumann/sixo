/*******************************************************************************
*
*                                         
*
* Copyright 1995-1998 MITSUBISHI ELECTRIC CORPORATION
* AND MITSUBISHI ELECTRIC SEMICONDUCTOR SYSTEMS CORPORATION
* All Rights Reserved.
*
*  $Id$
*
*   NC30 Version 0.00.00
*
*******************************************************************************/


#if defined(NC30)       /* NCxx */
    /* dummy */
#elif defined(NC77)
    #error "NC77 not supported"
#elif defined(NC79)
    #error "NC79 not supported"
#else
    #error "NC30, NC77, NC79 not defined"
#endif              /* NCxx */



#define M16C

/* KD30 MiniEmulator uses UART1 per Default,
   so we use UART0 for debugging while debugging!
   
   The release version does not use KD30, so UART1
   is free for flash programming and debug outs! 
*/
#ifdef MINIEMU
#define UART0
#endif

#include <stdio.h>
/* additional settings to stdio.h */
#define _576     5
#define _1152    6

/* ================================================= */
#ifdef MELPS7700

/* 
This code is cleared by krizsan, 13.08.2001 
because we don't have a MELPS7700.
To get it back, use the original file 
*/

#endif  /* MELPS7700 */
/* ================================================= */

#ifdef M16C

#ifdef UART0
#pragma EQU _porta  =   0x3EC           /*                                  */
#pragma EQU _pa_vct =   0x3EE           /*                                  */
#pragma EQU _portb  =   0x3ED           /*                                  */
#pragma EQU _pb_vct =   0x3EF           /*                                  */
#pragma EQU _mode1  =   0x3A0           /*                                  */
#pragma EQU _brg1   =   0x3A1           /*                                  */
#pragma EQU _sbuf1  =   0x3A2           /*                                  */
#pragma EQU _cntr1_l =  0x3A4           /*                                  */
#pragma EQU _cntr1_h =  0x3A5           /*                                  */
#pragma EQU _rbuf1  =   0x3A6           /*                                  */
#else /* UART1 : default */
#pragma EQU _porta  =   0x3E8           /*                                  */
#pragma EQU _pa_vct =   0x3EA           /*                                  */
#pragma EQU _portb  =   0x3E9           /*                                  */
#pragma EQU _pb_vct =   0x3EB           /*                                  */
#pragma EQU _mode1  =   0x3A8           /*                                  */
#pragma EQU _brg1   =   0x3A9           /*                                  */
#pragma EQU _sbuf1  =   0x3AA           /*                                  */
#pragma EQU _cntr1_l =  0x3AC           /*                                  */
#pragma EQU _cntr1_h =  0x3AD           /*                                  */
#pragma EQU _rbuf1  =   0x3AE           /*                                  */
#endif

#define TXEPTY  0x8                     /*                      */
#define TE      0x1                     /*                      */
#define TI      0x2                     /*                      */
#define RE      0x4                     /*                      */
#define RI      0x8                     /*                      */

#define ACK     0x40
#define BUSY    0x20
#define FAULT   0x10
#define RESET   0x8
#define STROBE  0x4

#define TRUE    1

char _porta;
char _pa_vct;
char _portb;
char _pb_vct;
char _mode1;
char _brg1;
char _sbuf1;
char _cntr1_l;
char _cntr1_h;
char _rbuf1;

/* uart speed settings */

#define BRG1152     4   /* f1(10Mhz/1) / 16 / 115200 - 1 = 4    */  /* UNTESTED!!! */
#define CNTR1152    0   /* f1                   */
#define BRG576      10  /* f1(10Mhz/1) / 16 / 57600 - 1 = 10    */
#define CNTR576     0   /* f1                   */
#define BRG192      31  /* f1(10Mhz/1) / 16 / 19200 - 1 = 31    */
#define CNTR192     0   /* f1                   */
#define BRG96       64  /* f1(10Mhz/1) / 16 /  9600 - 1 = 64    */
#define CNTR96      0   /* f1                   */
#define BRG48       129 /* f1(10Mhz/1) / 16 /  4800 - 1 = 129   */
#define CNTR48      0   /* f1                   */
#define BRG24       31  /* f8(10Mhz/8) / 16 /  2400 - 1 = 31    */
#define CNTR24      1   /* f8                   */
#define BRG12       64  /* f8(10Mhz/8) / 16 /  1200 - 1 = 64    */
#define CNTR12      1   /* f8                   */

#endif  /* M16C */

/*******************************************************************************
*
* init_dev 
*
* Copyright 1988-1998 MITSUBISHI ELECTRIC CORPORATION
* AND MITSUBISHI ELECTRIC SEMICONDUCTOR SYSTEMS CORPORATION
* All Rights Reserved.
*
*         : Version 2.00.00
*       Version 2.01.00
*                       
*
*           init_dev --                     
*
*           status = init_dev( stream, mode )
*
*           #include <stdio.h>
*           FILE *stream                            
*           int  mode;                
*           int  status;                      
*
*                                                                               
*
*                                                 
*
*******************************************************************************/

int init_dev( FILE _far *stream , int  mode )
{
    if ( mode != _TEXT && mode != _BIN )
        return  EOF;
    stream->_mod = mode;
    return  TRUE;
}


/*******************************************************************************
*
* speed 
*
* Copyright 1988-1998 MITSUBISHI ELECTRIC CORPORATION
* AND MITSUBISHI ELECTRIC SEMICONDUCTOR SYSTEMS CORPORATION
* All Rights Reserved.
*
*         : Version 2.00.00
*       Version 2.01.00
*                       
*       Version 3.00.00
*               ,                
*
*           speed --                   
*
*           status = speed( rate, bit, parity, stop );
*
*           #include <stdio.h>
*           int  rate;                    
*           int  bit;                   
*           int  parity;                
*           int  stop;                        
*           int  status;                      
*
*                                                                               
*
*                                                                           
*                       
*
*******************************************************************************/

int speed( int rate, int bit, int parity, int stop )
{
    int mode;                           /*                          */

    _cntr1_h = NULL;                    /*                      */
    switch( rate ) {                    /*                  */
        case _1152:
            _brg1 = BRG1152;
            _cntr1_l = CNTR1152;
            break;
        case _576:
            _brg1 = BRG576;
            _cntr1_l = CNTR576;
            break;
        case _192:
            _brg1 = BRG192;
            _cntr1_l = CNTR192;
            break;
        case _96:
            _brg1 = BRG96;
            _cntr1_l = CNTR96;
            break;
        case _48:
            _brg1 = BRG48;
            _cntr1_l = CNTR48;
            break;
        case _24:
            _brg1 = BRG24;
            _cntr1_l = CNTR24;
            break;
        case _12:
            _brg1 = BRG12;
            _cntr1_l = CNTR12;
            break;
        default:
            return EOF;
    }
    switch( bit ) {                     /*                */
        case _B8:
            mode = 0x5;
            break;
        case _B7:
            mode = 0x4;
            break;
        default:
            return EOF;
    }
    switch( parity ) {                  /*                */
        case _PN:
            break;
        case _PE:
            mode |= 0x60;
            break;
        case _PO:
            mode |= 0x40;
            break;
        default:
            return EOF;
    }
    switch( stop ) {                    /*                      */
        case _S1:
            break;
        case _S2:
            mode |= 0x10;
            break;
        default:
            return EOF;
    }
    _mode1 = mode;
    return  TRUE;
}


/*******************************************************************************
*
* ini_prn 
*
* Copyright 1988-1998 MITSUBISHI ELECTRIC CORPORATION
* AND MITSUBISHI ELECTRIC SEMICONDUCTOR SYSTEMS CORPORATION
* All Rights Reserved.
*
*         : Version 2.00.00
*       Version 2.01.00
*                       
*
*           ini_prn --                                 
*
*           status = init_prn( void );
*
*           int  status;                      
*
*                                                                         
*                         
*
*                                                                       
*
*******************************************************************************/

int init_prn()
{
/*
    int i;

    _porta = STROBE | RESET;            
    _pa_vct = 0x8c;                     
    _portb = NULL;                      
    _pb_vct = 0xff;                     
    _porta = STROBE;                    
    for ( i = 0 ; i < 0x50 ; i++ ) ;    
    _porta = RESET | STROBE;            
    for ( i = 0 ; i < 0x50 ; i++ ) ;    
    if ( _porta & FAULT )
        return EOF;
    else
*/
        return TRUE;
}


/*******************************************************************************
*
* _sget 
*
* Copyright 1988-1998 MITSUBISHI ELECTRIC CORPORATION
* AND MITSUBISHI ELECTRIC SEMICONDUCTOR SYSTEMS CORPORATION
* All Rights Reserved.
*
*         : Version 2.00.00
*       Version 2.01.00
*                       
*
*           _sget --                         
*
*           data = _sget( void );
*
*           int  data;              
*
*                                                                           
*                 
*
*                                       
*
*******************************************************************************/


int _sget( )                            /*                            */
{
    int  data;

    _cntr1_h = _cntr1_h | RE;           /*                        */
#ifndef SIMULATOR
    while ( !( _cntr1_h & RI ) ) ;      /*                                          */
#endif
    data = _rbuf1;
    return data;
}


/*******************************************************************************
*
* _sput 
*
* Copyright 1988-1998 MITSUBISHI ELECTRIC CORPORATION
* AND MITSUBISHI ELECTRIC SEMICONDUCTOR SYSTEMS CORPORATION
* All Rights Reserved.
*
*         : Version 2.00.00
*       Version 2.01.00
*                       
*
*           _sput --                         
*
*           status = _sput( c );
*
*           int  c;                     
*           int  status;                  
*
*                                                                       
*
*                                       
*
*******************************************************************************/


int _sput( int c )                          /*                            */
{
    int i;

    _cntr1_h = _cntr1_h | TE;           /* transmit enable bit set to '1' */
#ifndef SIMULATOR
    while ( !( _cntr1_h & TI ) ) ;      /* wait, until no data in transmit buffer  */
#endif
    _sbuf1 = c;                         /* transmit buffer to given char */
    
    if ( _cntr1_h & 0xf0 )
        return EOF;                     /*              */
    return  TRUE;
}


/*******************************************************************************
*
* _pput 
*
* Copyright 1988-1998 MITSUBISHI ELECTRIC CORPORATION
* AND MITSUBISHI ELECTRIC SEMICONDUCTOR SYSTEMS CORPORATION
* All Rights Reserved.
*
*         : Version 2.00.00
*       Version 2.01.00
*                       
*
*           _pput --                                       
*
*           status = _pput( c );
*
*           int  c;                     
*           int  status;                  
*
*                                                                       
*
*                                                     
*
*******************************************************************************/

int _pput( int c )                          /*                                          */
{
/*
    int  i;

    if ( _porta & FAULT )
        return EOF;
    while ( !( _porta & BUSY ) ) ;      
    _portb = c;
    for ( i = 0 ; i < 10 ; i++ ) ;      
    _porta = RESET;                     
    for ( i = 0 ; i < 10 ; i++ ) ;      
    _porta = RESET | STROBE;            
    while ( _porta & ACK ) ;            
    if ( _porta & FAULT )
        return EOF;
    else
*/    
        return TRUE;
}



/*******************************************************************************
*
*                                         
*
* Copyright 1995-1998 MITSUBISHI ELECTRIC CORPORATION
* AND MITSUBISHI ELECTRIC SEMICONDUCTOR SYSTEMS CORPORATION
* All Rights Reserved.
*
*******************************************************************************/
