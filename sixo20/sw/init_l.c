/*******************************************************************************
*
* init
*
* Copyright 1995-1998 MITSUBISHI ELECTRIC CORPORATION
* AND MITSUBISHI ELECTRIC SEMICONDUCTOR SYSTEMS CORPORATION
* All Rights Reserved.
*
*  $Id$
*
*	NC30 Version 0.00.00
*
*         : Version 0.00.00
*
*           init --
*
*           void init( void )
*           #include <stdio.h>
*
*
*
*           FILE                          UART
*
*
*******************************************************************************/


#if defined(NC30)		/* NCxx */
	/* dummy */
#elif defined(NC77)
	#error "NC77 not supported"
#elif defined(NC79)
	#error "NC79 not supported"
#else
	#error "NC30, NC77, NC79 not defined"
#endif				/* NCxx */


/* KD30 MiniEmulator uses UART1 per Default,
   so we use UART0 for debugging while debugging!

   The release version does not use KD30, so UART1
   is free for flash programming and debug outs!
*/
#if(MINIEMU==1)
#define UART0
#endif


#include <stdio.h>
/* additional settings to stdio.h */
#define _576     5
#define _1152    6


FILE _iob[4];

void init( void );

void init( void )
{
    stdin->_cnt = stdout->_cnt = stdaux->_cnt = stdprn->_cnt = 0;
    stdin->_flag = _IOREAD;
    stdout->_flag = _IOWRT;
    stdaux->_flag = _IORW;
    stdprn->_flag = _IOWRT;

    stdin->_mod = _TEXT;
    stdout->_mod = _TEXT;
    stdaux->_mod = _BIN;
    stdprn->_mod = _TEXT;

    stdin->_func_in = _sget;
    stdout->_func_in = NULL;
    stdaux->_func_in = _sget;
    stdprn->_func_in = NULL;

    stdin->_func_out = _sput;
    stdout->_func_out = _sput;
    stdaux->_func_out = _sput;
    stdprn->_func_out = _pput;

#ifdef UART0
    /* speed(_96, _B8, _PN, _S2); default */
    speed(_576, _B8, _PN, _S1);
#else /* UART1 : default */
    /* these setting work well! */
    /* speed(_96, _B8, _PN, _S2); */
    /* speed(_192, _B8, _PN, _S1); */
    speed(_576, _B8, _PN, _S1);
    /*speed(_1152, _B8, _PN, _S1); */ /* this is too fast for m16c */
#endif
    /* init_prn(); */   /* we use port4 for our own purpose! krz, 13.08.2001 */
}


/*******************************************************************************
*
* init
*
* Copyright 1995-1998 MITSUBISHI ELECTRIC CORPORATION
* AND MITSUBISHI ELECTRIC SEMICONDUCTOR SYSTEMS CORPORATION
* All Rights Reserved.
*
*******************************************************************************/
