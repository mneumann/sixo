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
 *  Module:         -  
 *  Purpose:        standard defines for all modules
 *  Comments:       -
 *  Changes:        added DUMMY_OFF and DUMMY_ON, AN 16.2.2003
 ************************************************************************ */ 
#ifndef _STANDARD_H
#define _STANDARD_H


/* DATA TYPES */
typedef signed char     INT8;       /* define INT8 as signed integers */
typedef unsigned char   UINT8;      /* define UINT8 as unsigned integers */

typedef short           INT16;      /* define INT16 as signed integers */
typedef unsigned short  UINT16;     /* define UINT16 as unsigned short  */

typedef unsigned long   UINT32;     /* define UINT32 as unsigned long  */
typedef long            INT32;      /* define INT32 as long  */

typedef unsigned char   CHAR;       /* define CHAR as unsigned char */
typedef unsigned char far *STRING;  /* define STRING as pointer to CHAR */


/* DATA TYPES VALUE RANGES */
#define INT8_MAX    127
#define INT8_MIN    -128
#define INT16_MAX	32767
#define INT16_MIN	-32768
#define INT32_MAX   2147483647
#define INT32_MIN   -2147483648
#define INT32_MAX_STR  "2147483647"
#define UINT8_MAX   0xFF
#define UINT16_MAX  0xFFFF
#define UINT32_MAX  0xFFFFFFFF


/* define NULL */
#ifndef NULL
#define NULL (void *)0 
#endif

/* typedefs fuer TRUE und FALSE */
typedef enum
{
    FALSE,
    TRUE
} BOOL;


/* other boolean defines */
#define LEFT	        (unsigned char) 0x0
#define RIGHT	        (unsigned char) 0x1

#define OFF             (unsigned char) 0x0
#define ON              (unsigned char) 0x1

#define DUMMY_OFF       (unsigned char) 0x0
#define DUMMY_ON        (unsigned char) 0x1


/* byte access makros */
#define LOWBYTE(word)   ((UINT8)(((UINT16)word)&0x00ff))
#define HIGHBYTE(word)  ((UINT8)((((UINT16)word)&0xff00)>>8))

/* Minimum/Maximum */
#define MIN(x,y) (x<=y?x:y)
#define MAX(x,y) (x>y?x:y)





/* Type for error handling, return code of most functions */
typedef enum
{
    ERR_OK,
    ERR_NOT_SUPPORTED,
    ERR_OUT_OF_RANGE,
    ERR_PARAM_ERR,
    ERR_NO_SUCH_ID,
    ERR_WRONG_STATE,
    ERR_NO_MEM,
    ERR_TIMEOUT,
    
    ERR_MSGQUEUE_NO_WRITE,
    ERR_MSGQUEUE_MAX_FULL,
    ERR_MSGQUEUE_NO_MSGS,
    ERR_MSGQUEUE_NO_ID,
    ERR_MSG_NOT_PROCESSED,
    ERR_MSG_PROCESSED,
    
    ERR_TIMER_NO_FUNC,
    ERR_TIMERMSG_NOT_FOUND,
    ERR_TIMERMSG_FULL,

    ERR_EEPROM_NOWRITE,
    ERR_EEPROM_NOREAD,

    ERR_CLOCK_NOWRITE,
    ERR_CLOCK_NOREAD,
    ERR_CLOCK_EXCESS,   //this is a warning, may be ignored
    ERR_NVRAM_NOWRITE,
    ERR_NVRAM_NOREAD
} ERRCODE;


/* disable/enable m16c interrupts
   note1: no interrupts will be missed!
   note2: up to 4 nops are essential when writing into interrupt control register */

#define INT_GLOB_ENABLE         asm("fset i");asm("nop");asm("nop");asm("nop");asm("nop");
#define INT_GLOB_DISABLE        asm("fclr i");asm("nop");asm("nop");asm("nop");asm("nop");

/* for code not to be interrupted */
#define ENTER_CRITICAL_SECTION  INT_GLOB_DISABLE
#define LEAVE_CRITICAL_SECTION  INT_GLOB_ENABLE


#define NOP            asm(" nop ")

#endif // _STANDARD_H
