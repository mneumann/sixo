/***********************************************************************
 *        FILE:           isr.c
 *        REVISION:       0.0.1
 * --------------------------------------------------------------------
 *        COMPANY:        SIxO Incorporated
 *                        all rights reserved
 * --------------------------------------------------------------------
 *        DEVELOPED:      16-02-2001 by Ralf Krizsan
 *        MODULE:         interrupts
 *        PURPOSE:        interrupt service routines
 *        COMMENTS:       -

 * --------------------------------------------------------------------
 *        HISTORY:        (Date/Rev/Author/ChangeDescription)

  08-07-2001  0.0.1  krz  first draft

************************************************************************ */

/* ------------------------------------------- */
/* routine, to which all unsupported interrupts are routed: */
/* WE'VE GOT A PROBLEM IF THE PROGRAM CALLS THIS FUNCTION!!! */
void DiagnosticISR(void)
{
    // should never pass this code!
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





