/***********************************************************************
 *
 *  CVS-Filename:   $RCSfile$
 *  Version:        $Revision$
 *  Last Changed:   $Date$
 *  Changed by:     $Author$
 *
 *  --------------------------------------------------------------------
 *
 *  Created:        2001-10-22 by Ralf Krizsan
 *  Project:        SIxO
 *  Module:         diagnostics
 *  Purpose:        common interrupt service routines and warning
 *                  functions
 *  Comments:       basic stuff like 'overflow' or 'division by zero'
 *                  for diagnostic purpose
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


// inlcudes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sfr62p.h"
#include "standard.h"
#include "diagnose.h"


// module defines
// !!! these settings have be set according to 'sect30.inc' file !!!
#define TABSIZE     10              // for diagnostic display
#define HEAPSIZE    0x2000L         // heap size defined in sect30.inc
#define HEAPGRAN    0x200L          // granularity for heap check (HEAPSIZE=n*HEAPGRAN!)
#define STACKSIZE   0x400L          // user stack size defined in sect30.inc
#define ISTACKSIZE  0x200L          // interrupt stack size defined in sect30.inc

// module global vars:
UINT16 wUnexpISRCalls   = 0;        // number of DiagnosticISR() calls
UINT8  bMaxHeapUsage    = 0;        // maximal heap usage in percent

UINT8  bUSPUse      = 0;            // maximal user stack usage in percent
UINT16 wUSPStart    = 0;            // user stack pointer start value
UINT16 wUSPMax      = 0;            // user stack pointer max value
UINT16 wUSP         = 0;            // for fast local copy in isr

UINT8  bISPUse      = 0;            // maximal interrupt stack usage in percent
UINT16 wISPStart    = 0;            // interrupt stack pointer start value
UINT16 wISPMax      = 0;            // interrupt stack pointer max value
UINT16 wISP         = 0;            // for fast local copy in isr


// local prototypes
void StandardDiagnoseScreen(void);
void ProcessorDebugScreen(void);
void StackDebugScreen(void);
void ResetM16C(void);



/* ------------------------------------------- */
/* routine, to which all unsupported interrupts are routed: */
/* WE'VE GOT A PROBLEM IF THE PROGRAM CALLS THIS FUNCTION!!! */
void DiagnosticISR(void)
{
    // should never pass this code!
    if (wUnexpISRCalls < 0xffff)
        wUnexpISRCalls++;
}



/* ------------------------------------------- */
/* standard interrupt of variable vector table */
#pragma INTERRUPT dummy_int
void dummy_int(void)
{
        DiagnosticISR();
}



/* ------------------------------------------ */
/* interrupts of fixed vector table */
#pragma INTERRUPT UndefInstrISR
void UndefInstrISR(void)
{
        DiagnosticISR();
}

#pragma INTERRUPT OverflowISR
void OverflowISR(void)
{
        DiagnosticISR();
}

#pragma INTERRUPT BreakISR
void BreakISR(void)
{
        DiagnosticISR();
}

#pragma INTERRUPT AddrMatchISR
void AddrMatchISR(void)
{
        DiagnosticISR();
}

#pragma INTERRUPT SingleStepISR                // DO NOT USE!!!
void SingleStepISR(void)
{
        DiagnosticISR();
}

#pragma INTERRUPT WatchdogISR
void WatchdogISR(void)
{
        DiagnosticISR();
}

#pragma INTERRUPT DebugISR                        // DO NOT USE!!!
void DebugISR(void)
{
        DiagnosticISR();
}

#pragma INTERRUPT NMIISR
void NMIISR(void)
{
        DiagnosticISR();
}
/* ------------------------------------------ */





/***********************************************************************
 *   FUNCTION:      DisplayDiagnose
 *   DESCRIPTION:   Special screen for system info only
 *   PARAMETER:     -
 *   RETURN:        -
 *   COMMENT:       Special screen reached per menu
 *********************************************************************** */
void DisplayDiagnose()
{
    StandardDiagnoseScreen();
    // ProcessorDebugScreen();
    // StackDebugScreen();
}


/***********************************************************************
 *   FUNCTION:      ResetM16C
 *   DESCRIPTION:   Resets the micro controller only - immediatly!
 *   PARAMETER:     -
 *   RETURN:        -
 *   COMMENT:       Special function for debugging purpose only
 *********************************************************************** */
void ResetM16C(void)
{
    prcr = 0x2;
    pm0  = 0x08;    // set to reset NOW!
    prcr = 0x0;
}




/***********************************************************************
 *  FUNCTION:       CheckActFreeSystemResources
 *  DESCRIPTION:    calculates currently unused stack & heap
 *  PARAMETER:      -
 *  RETURN:         updates module vars
 *  COMMENT:        Special function for debugging purpose only
 *                  To be called periodically
 *********************************************************************** */
void CheckActFreeSysRes(void)
{
    UINT8   bBlockCount = 0;
    UINT8   bBlockMax   = HEAPSIZE / HEAPGRAN - 1;
    void far * pMem[HEAPSIZE/HEAPGRAN];
    UINT16  wFreeMem    = 0;
    UINT32  dwScratch   = 0;

    // ----------------------------------
    // maximal user stack usage in percent
    dwScratch = ((wUSPStart - wUSPMax) * 100) / STACKSIZE;
    bUSPUse = (UINT8) dwScratch;

    // ----------------------------------------
    // maximal interrupt stack usage in percent
    dwScratch = ((wISPStart - wISPMax) * 100) / ISTACKSIZE;
    bISPUse = (UINT8) dwScratch;

    // ----------------------------------
    // init heap check array
    for (bBlockCount = 0; bBlockCount < bBlockMax; bBlockCount++)
        pMem[bBlockCount] = NULL;

    // check allocatable memory
    for (bBlockCount = 0; bBlockCount < bBlockMax; bBlockCount++)
    {
        pMem[bBlockCount] = malloc( HEAPGRAN );
        if (pMem[bBlockCount] == NULL)
            break;                          // no more mem free!
    }

    // sum and free memory
    for (bBlockCount = 0; bBlockCount < bBlockMax; bBlockCount++)
    {
        if (pMem[bBlockCount] != NULL)
        {
            free(pMem[bBlockCount]);
            wFreeMem += HEAPGRAN;
        }
    }

    // calculate heap usage in percent
    dwScratch = ((HEAPSIZE - wFreeMem)*100)/HEAPSIZE;
    if (bMaxHeapUsage < ((UINT8)dwScratch))
        bMaxHeapUsage = (UINT8)dwScratch;
}



/***********************************************************************
 *   FUNCTION:      StandardDiagnoseScreen
 *   DESCRIPTION:   Special screen for system info only
 *   PARAMETER:     -
 *   RETURN:        -
 *   COMMENT:       Special screen reached per menu
 *********************************************************************** */
void StandardDiagnoseScreen(void)
{
#ifdef ALL
    POSITION    Pos;            /* display pos */
    char        Text[30];       /* textarray */
    STRING      strText = Text; /* ptr to text array */

    /* clear display first ------------------------ */
    DispClear();


    // FIRST ROW ===================================
    Pos.PosX = 0;

    // maximal user stack usage in percent
    Pos.PosY = 0;
    sprintf (strText, "UST:%d%%", bUSPUse);
    DispCharSetString(Pos, strText, strlen(strText));

    // maximal heap usage in percent
    Pos.PosY = 1;
    sprintf (strText, "HP :%d%%", bMaxHeapUsage);
    DispCharSetString(Pos, strText, strlen(strText));

    // TimerInterrupt() processor load in percent
    Pos.PosY = 2;
    sprintf (strText, "TIL:%d%%", TimerGetTimerISRLoad());
    DispCharSetString(Pos, strText, strlen(strText));

    // number of unexpected interrupt calls (fixed vector tabel etc.)
    Pos.PosY = 3;
    sprintf (strText, "UIC:%d", wUnexpISRCalls);
    DispCharSetString(Pos, strText, strlen(strText));


    // divider row =================================
    Pos.PosX = TABSIZE-1;
    for (Pos.PosY = 0; Pos.PosY <4; Pos.PosY++)
        DispCharSetString(Pos, "|", 1);


    // SECOND ROW ==================================
    Pos.PosX = TABSIZE;

    // maximal interrupt stack usage in percent
    Pos.PosY = 0;
    sprintf (strText, "IST:%d%%", bISPUse);
    DispCharSetString(Pos, strText, strlen(strText));

    // max number of message Q entries
    Pos.PosY = 1;
    sprintf (strText, "MQE:%d", MsgQGetMaxEntries() );
    DispCharSetString(Pos, strText, strlen(strText));

    // ???
    Pos.PosY = 2;
    sprintf (strText, "---:%d", 0);
    DispCharSetString(Pos, strText, strlen(strText));

    // ???
    Pos.PosY = 3;
    sprintf (strText, "---:%d", 0);
    DispCharSetString(Pos, strText, strlen(strText));
#endif
}


/***********************************************************************
 *   FUNCTION:      ProcessorDebugScreen
 *   DESCRIPTION:   Special screen for system info only
 *   PARAMETER:     -
 *   RETURN:        -
 *   COMMENT:       -
 *********************************************************************** */
void ProcessorDebugScreen(void)
{
#ifdef ALL
    POSITION    Pos;            /* display pos */
    char        Text[30];       /* textarray */
    STRING      strText = Text; /* ptr to text array */

    // Test:
    UINT8  PM0    = 0;
    UINT8  PM1    = 0;
    UINT8  CM0    = 0;
    UINT8  CM1    = 0;

    #ifndef COMPILE_SUN
    asm(" MOV.B 04h, $$[FB]", PM0);
    asm(" MOV.B 05h, $$[FB]", PM1);
    asm(" MOV.B 06h, $$[FB]", CM0);
    asm(" MOV.B 07h, $$[FB]", CM1);
    #endif

    // clear display first ------------------------
    DispClear();
    Pos.PosX = 0;

   // TEST: debug processor mode register
    Pos.PosY = 1;
    sprintf (strText, "PM0:%x PM1:%x", PM0, PM1);
    DispCharSetString(Pos, strText, strlen(strText));

    // TEST: debug clock control register
    Pos.PosY = 2;
    sprintf (strText, "CM0:%x CM1:%x", CM0, CM1);
    DispCharSetString(Pos, strText, strlen(strText));
#endif
}


/***********************************************************************
 *   FUNCTION:      StackDebugScreen
 *   DESCRIPTION:   Special screen for system info only
 *   PARAMETER:     -
 *   RETURN:        -
 *   COMMENT:       -
 *********************************************************************** */
void StackDebugScreen(void)
{
#ifdef ALL
    POSITION    Pos;            // display pos
    char        Text[30];       // textarray
    STRING      strText = Text; // ptr to text array

    // clear display first ------------------------
    DispClear();
    Pos.PosX = 0;

    // maximal stack usage in percent
    Pos.PosY = 0;
    sprintf (strText, "Start: U%x I%x", wUSPStart, wISPStart);
    DispCharSetString(Pos, strText, strlen(strText));

    Pos.PosY = 1;
    sprintf (strText, "Act  : U%x I%x", wUSP, wISP);
    DispCharSetString(Pos, strText, strlen(strText));

    Pos.PosY = 2;
    sprintf (strText, "Max  : U%x I%x", wUSPMax, wISPMax);
    DispCharSetString(Pos, strText, strlen(strText));

    Pos.PosY = 3;
    sprintf (strText, "StackUse: U%d%% I%d%%", bUSPUse, bISPUse);
    DispCharSetString(Pos, strText, strlen(strText));
#endif
}