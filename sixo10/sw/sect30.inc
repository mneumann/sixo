;***********************************************************************
;
;   $RCSfile$
;
;   $Revision$
;
;   $Date$
;
;   $Author$
;
;  --------------------------------------------------------------------
;   Company:        N&K Development, all rights reserved
;
;                   Neugebauer & Krizsan Cooperation, Germany
;                   contact:    arnold.neugebauer@web.de
;                               ralf.krizsan@web.de
;  --------------------------------------------------------------------
;   Created:        25-10-2001 by Ralf Krizsan
;   Project:        SIxO
;   Module:         hardware settings
;   Purpose:        section definitions
;   Comments:       use as30 define
;
;                       MINIEMU=1
;
;                   for use with mini emulator KD30 V3.0!
;
;   ToDo:           use define to switch debug port to uart 0/1 due
;                   to conflicts to MiniEmu use!
;
;************************************************************************

;---------------------------------------------------------------
;
;        Global Symbol Defines
;
;---------------------------------------------------------------

    .glb        _TimerInterrupt     ; as timer ta0 isr vector
    .glb        _msIntFunction      ; as timer ta1 isr vector
    .glb        _Keyboard_ISR       ; as int2 isr vector
    .glb        _PreScaleWheel_ISR  ; as timer tb2 isr vector
    .glb        _PreScaleRPM_ISR    ; as timer ta4 isr vector
    .glb        _WheelOverflow_ISR  ; as timer ta2 isr vector
    .glb        _RPMOverflow_ISR    ; as timer ta3 isr vector
    .glb        _WheelSensor_ISR    ; as int0 isr vector
    .glb        _RPMSensor_ISR      ; as int1 isr vector

    .glb        _dummy_int          ; global for unexpected call checking

    .glb        _UndefInstrISR      ; fixed vector table fct ..
    .glb        _OverflowISR        ; fixed vector table fct ..
    .glb        _BreakISR           ; fixed vector table fct ..
    .glb        _AddrMatchISR       ; fixed vector table fct ..
    .glb        _SingleStepISR      ; do not use! reserved for debugger
    .glb        _WatchdogISR        ; fixed vector table fct ..
    .glb        _DebugISR           ; do not use! reserved for debugger
    .glb        _NMIISR             ; do not use! reserved for debugger

;---------------------------------------------------------------
;
;        Target System Definitions
;
;---------------------------------------------------------------

; global switches -------------------------------------------------------
;   ! don't make any changes here !
M62TYPE                 .equ         1          ; for var vector table format


; ram sizes -------------------------------------------------------------
;   ! adapt these settings if neccessary !
HEAPSIZE                .equ    02000h          ; heap size
STACKSIZE               .equ    00400h          ; stack size
ISTACKSIZE              .equ     0200h          ; interrupt stack size


; rom sizes ------------------------------------------------------------
;   ! adapt these settings if neccessary !
ROM_DATA_SIZE           .equ   010000h          ; initialize data and const data
                                                ; (do NOT use for program code!)
ROM_CODE_INIT_SIZE      .equ     0100h          ; uC init program
ROM_CODE_PRG_SIZE       .equ   010000h          ; application program
ROM_CODE_INT_SIZE       .equ    0FCFFh          ; interrupt service routines


; ram start/end addresses ----------------------------------------------
;   ! don't make any changes here !
RAM_SFR_START           .equ        0h          ; register reserved
RAM_SFR_END             .equ     03ffh
RAM_USER_START          .equ     0400h          ; user area: 20479 byte
.if        MINIEMU==1
RAM_USER_END            .equ    052ffh          ; restricted for mini emu use
.else
RAM_USER_END            .equ    053ffh          ; normal use
.endif


; rom start/end addresses ----------------------------------------------
;   ! don't make any changes here !
ROM_USER_START          .equ   0c0000h          ; flash area begin
                                                ; c0000h-cffffh is NOT usable
                                                ; as program memory after RESET!
                                                ; (see uC Manual, register pm1.3)
ROM_SPPAGES_VT          .equ   0ffe00h          ; special pages vector table, 476 bytes
ROM_FIX_VT              .equ   0fffdch          ; fixed vector table, 36 bytes

.if        MINIEMU==1
ROM_VAR_VT              .equ   0fbd00h          ; variable vector table, 256 bytes
.else                                           ; (monitor program begins at fbe00h)
ROM_VAR_VT              .equ   0ffd00h          ; variable vector table, 256 bytes
.endif
ROM_USER_END            .equ   ROM_VAR_VT-1     ; restricted rom for mini emu monitor


; rom location calculations -------------------------------------------
;   ! don't make any changes here !
;
;   ! the following addresses are designed to be  !
;   ! relative. so don't change any settings here !
;   ! but change only 'rom sizes' above.          !
;
ROM_DATA_START          .equ   ROM_USER_START
ROM_DATA_END            .equ   ROM_USER_START+ROM_DATA_SIZE-1

; uC init program
ROM_CODE_INIT_START     .equ   ROM_DATA_END+1
ROM_CODE_INIT_END       .equ   ROM_CODE_INIT_START+ROM_CODE_INIT_SIZE-1

; aplication program
ROM_CODE_PRG_START      .equ   ROM_CODE_INIT_END+1
ROM_CODE_PRG_END        .equ   ROM_CODE_PRG_START+ROM_CODE_PRG_SIZE-1

; interrupt service routines
ROM_CODE_INT_START      .equ   ROM_CODE_PRG_END+1
ROM_CODE_INT_END        .equ   ROM_CODE_INT_START+ROM_CODE_INT_SIZE-1


;---------------------------------------------------------------
;
;        Arrangement        of section
;
;---------------------------------------------------------------
; Near RAM data        area
;---------------------------------------------------------------
; SBDATA area
        .section        data_SE,DATA
        .org            RAM_USER_START
data_SE_top:
        .section        bss_SE,DATA,ALIGN
        .section        iicbb,data,align
bss_SE_top:
        .section        data_SO,DATA
data_SO_top:
        .section        bss_SO,DATA
bss_SO_top:

; near RAM area
        .section        data_NE,DATA,ALIGN
data_NE_top:
        .section        bss_NE,DATA,ALIGN
bss_NE_top:
        .section        data_NO,DATA
data_NO_top:
        .section        bss_NO,DATA
bss_NO_top:

;---------------------------------------------------------------
; Stack        area
;---------------------------------------------------------------
        .section        stack,DATA
        .blkb           STACKSIZE
stack_top:

        .blkb           ISTACKSIZE
istack_top:

;---------------------------------------------------------------
;        heap section
;---------------------------------------------------------------
        .section        heap,DATA
heap_top:
        .blkb           HEAPSIZE

;---------------------------------------------------------------
; Near ROM data        area
; !!! this area is not available in single chip mode !!!
; (see 'c programming manual, page 100)
;---------------------------------------------------------------
;        .section       rom_NE,ROMDATA,ALIGN
;rom_NE_top:
;        .section       rom_NO,ROMDATA
;rom_NO_top:

;---------------------------------------------------------------
; Far RAM data area
; !!! this area is not available in single chip mode !!!
; (see 'c programming manual, page 100)
;---------------------------------------------------------------
;        .section        data_FE,DATA
;        .org            10000H
;data_FE_top:
;        .section        bss_FE,DATA,ALIGN
;bss_FE_top:
;        .section        data_FO,DATA
;data_FO_top:
;        .section        bss_FO,DATA
;bss_FO_top:


;---------------------------------------------------------------
; RAM area not available (just to cause linker errors)
;---------------------------------------------------------------

        .section        UNAVAILABLE,DATA
        .org            RAM_USER_END+1
        .blkb           ROM_USER_START-1-RAM_USER_END

;---------------------------------------------------------------
; Far ROM data area
;---------------------------------------------------------------
        .section        rom_FE,ROMDATA
        .org            ROM_DATA_START
rom_FE_top:
        .section        rom_FO,ROMDATA
rom_FO_top:

;---------------------------------------------------------------
; Initial data of 'data' section
;---------------------------------------------------------------
        .section        data_SEI,ROMDATA
data_SEI_top:
        .section        data_SOI,ROMDATA
data_SOI_top:
        .section        data_NEI,ROMDATA
data_NEI_top:
        .section        data_NOI,ROMDATA
data_NOI_top:
;        .section        data_FEI,ROMDATA         ; not available in single chip mode
;data_FEI_top:
;        .section        data_FOI,ROMDATA         ; not available in single chip mode
;data_FOI_top:

;---------------------------------------------------------------
; Switch Table Section
;---------------------------------------------------------------
        .section         switch_table,ROMDATA
switch_table_top:


;---------------------------------------------------------------
; code area
;---------------------------------------------------------------

        .section        ucinit
        .org            ROM_CODE_INIT_START

        .section        program
        .org            ROM_CODE_PRG_START

        .section        interrupt
        .org            ROM_CODE_INT_START

        .section        program_S


;---------------------------------------------------------------
; variable vector section
;---------------------------------------------------------------
        .section        vector                ; variable vector table
        .org            ROM_VAR_VT
        .lword        _dummy_int         ; BRK (vector 0)
        .org          (ROM_VAR_VT+16)
        .lword        _dummy_int         ; int3 (for user)(vector 4)
        .lword        _dummy_int         ; timerB5 (for user)(vector 5)
        .lword        _dummy_int         ; timerB4 (for user)(vector 6)
        .lword        _dummy_int         ; timerB3 (for user)(vector 7)
        .lword        _dummy_int         ; si/o4 /int5(for user)(vector 8)
        .lword        _dummy_int         ; si/o3 /int4(for user)(vector 9)
        .lword        _dummy_int         ; Bus collision detection(for user)(v10)
        .lword        _dummy_int         ; DMA0(for user)(vector 11)
        .lword        _dummy_int         ; DMA1(for user)(vector 12)
        .lword        _dummy_int         ; Key input interrupt(for user)(vect 13)
        .lword        _dummy_int         ; A-D(for user)(vector 14)
        .lword        _dummy_int         ; uart2 transmit(for user)(vector 15)
        .lword        _dummy_int         ; uart2 receive(for user)(vector 16)
        .lword        _dummy_int         ; uart0 transmit(for user)(vector 17)
        .lword        _dummy_int         ; uart0 receive(for user)(vector 18)
.if        MINIEMU==1
        .lword        0FF900h            ; uart1 transmit(for user)(vector 19) KD30 V3.0
        .lword        0FF900h            ; uart1 receive(for user)(vector 20) KD30 V3.0
.else
        .lword        _dummy_int         ; uart1 transmit(for user)(vector 19)
        .lword        _dummy_int         ; uart1 receive(for user)(vector 20)
.endif
        .lword        _msIntFunction     ; timer A0(for user)(vector 21)
        .lword        _TimerInterrupt    ; timer A1(for user)(vector 22)
        .lword        _WheelOverflow_ISR ; timer A2(for user)(vector 23)
        .lword        _RPMOverflow_ISR   ; timer A3(for user)(vector 24)
        .lword        _PreScaleRPM_ISR   ; timer A4(for user)(vector 25)
        .lword        _dummy_int         ; timer B0(for user)(vector 26)
        .lword        _dummy_int         ; timer B1(for user)(vector 27)
        .lword        _PreScaleWheel_ISR ; timer B2(for user)(vector 28)
        .lword        _WheelSensor_ISR   ; int0 (for user)(vector 29)
        .lword        _RPMSensor_ISR     ; int1 (for user)(vector 30)
        .lword        _dummy_int         ; int2 (for user)(vector 31)
        .lword        _dummy_int         ; vector 32 (for user or MR30)
        .lword        _dummy_int         ; vector 33 (for user or MR30)
        .lword        _dummy_int         ; vector 34 (for user or MR30)
        .lword        _dummy_int         ; vector 35 (for user or MR30)
        .lword        _dummy_int         ; vector 36 (for user or MR30)
        .lword        _dummy_int         ; vector 37 (for user or MR30)
        .lword        _dummy_int         ; vector 38 (for user or MR30)
        .lword        _dummy_int         ; vector 39 (for user or MR30)
        .lword        _dummy_int         ; vector 40 (for user or MR30)
        .lword        _dummy_int         ; vector 41 (for user or MR30)
        .lword        _dummy_int         ; vector 42 (for user or MR30)
        .lword        _dummy_int         ; vector 43 (for user or MR30)
        .lword        _dummy_int         ; vector 44 (for user or MR30)
        .lword        _dummy_int         ; vector 45 (for user or MR30)
        .lword        _dummy_int         ; vector 46 (for user or MR30)
        .lword        _dummy_int         ; vector 47 (for user or MR30)
; to vector        63 from        vector 32 is used MR30

;===============================================================
; fixed        vector section
;---------------------------------------------------------------
        .section        fvector          ; fixed vector table

;===============================================================
; special page defination
;---------------------------------------------------------------
;        macro is defined in        ncrt0.a30
;        Format:        SPECIAL        number
;
;---------------------------------------------------------------
;        SPECIAL        255
;        SPECIAL        254
;        SPECIAL        253
;           :
;           :
;        (omitted)
;           :
;           :
;        SPECIAL        24
;        SPECIAL        23
;        SPECIAL        22
;        SPECIAL        21
;        SPECIAL        20
;        SPECIAL        19
;        SPECIAL        18
;
;===============================================================
; fixed        vector section
;---------------------------------------------------------------

        .org            ROM_FIX_VT
UDI:
        .lword          _UndefInstrISR
OVER_FLOW:
        .lword          _OverflowISR
BRKI:
        .lword          _BreakISR
ADDRESS_MATCH:
        .lword          _AddrMatchISR
SINGLE_STEP:
        .lword          _SingleStepISR
WDT:
        .lword          _WatchdogISR
DBC:
        .lword          _DebugISR
NMI:
        .lword          _NMIISR
RESET:
        .lword          start



