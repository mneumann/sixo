;**********************************************************************
;
;  CVS-Filename:   $RCSfile$
;  Version:        $Revision$
;  Last Changed:   $Date$
;  Changed by:     $Author$
;
;  --------------------------------------------------------------------
;
;  Created:        25-10-2001 by Ralf Schwarzer
;  Project:        SIxO
;  Module:         hardware settings
;  Purpose:        section definitions
;  Comments:       use as30 define
;
;                      KD30_USED=1
;
;                  for use with mini emulator!
;
;  ToDo:           use define to switch debug port to uart 0/1 due
;                  to conflicts to MiniEmu use!
;
;  --------------------------------------------------------------------
;
;  Compiler:       Renesas NC30WA V.5.00 Release 2
;  Options:        #define KD30_USED=1
;
;  --------------------------------------------------------------------
;
;  Licence details
;
;  This software is copyright © 2001-2004 by N&K Development, Germany
;
;  You can redistribute it and/or modify it under the terms of the
;  GNU General Public License version 2 as published by the
;  Free Software Foundation.
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License
;  along with this program; if not, write to the Free Software
;  Foundation, Inc., 59 Temple Place - Suite 330, Boston,
;  MA  02111-1307, USA.
;
;  To purchase support or enhancements for this software, contact:
;
;      N&K Development                     N&K Development
;      Arnold Neugebauer                   Ralf Schwarzer
;      Stöckener Str. 115                  Hudeplan 37 A
;      30419 Hannover                      30453 Hannover
;      arnold.neugebauer@web.de            ralf.schwarzer@htp-tel.de
;
;      or try http://www.sixo.de
;
;  --------------------------------------------------------------------
;
;  Disclaimer of warrenty
;
;  The software is provided as is without warranties of
;  any kind, either express or implied. To the fullest
;  extent allowed by applicable law, N&K Development
;  disclaims all warranties, express or implied,
;  including, but not limited to, implied warranties of
;  merchantability, fitness for a particular purpose, and
;  non-infringement.
;
;  --------------------------------------------------------------------
;
;  CVS History
;
;  This information is automatically added while 'commiting' the
;  changes to CVC ('Log message'):
;
; $Log$
; Revision 3.4  2012/02/14 21:08:03  tuberkel
; - #define COMPASS ==> COMPDRV
; - Compass SystemParam moved from devFlags2 -> 3
; - Settings 'Compass' ==> 'Extensions'
; - all Compass-Display modules enabled by default
;
; Revision 3.3  2012/02/08 03:41:30  tuberkel
; renamed #define MINIEMU ==> KD30_USED
;
; Revision 3.2  2012/02/06 20:54:14  tuberkel
; Just renamed all 'Devices' function prefixes for better readability
;
; Revision 3.1  2012/02/01 21:24:00  tuberkel
; INT2..5 enabled & ok for GPI0..3
;
; Revision 3.0  2010/11/07 09:35:17  tuberkel
; V30 Preparations:
; - STACKSIZE   0x400 --> 0x600      ; stack size
; - ISTACKSIZE  0x200 --> 0x400      ; interrupt stack size
;
; Revision 2.3  2009/07/18 06:29:34  tuberkel
; Heapsize reduced from 0x1000 ==> 0x400
; Heap only used for Bitmap-DrawLine
;
; Revision 2.2  2009/07/08 21:49:04  tuberkel
; Changed contact data: Ralf Krizsan ==> Ralf Schwarzer
;
; Revision 2.1  2009/06/21 21:24:08  tuberkel
; Changes done by AN:
; New _CompassRxIsr
;
; Revision 2.0  2006/06/26 23:25:51  tuberkel
; no message
;
; Revision 1.8  2006/02/18 14:40:47  Ralf
; - heap size decreased from 0x2000 -->0x1000
; - but heap still unused
;
;
;************************************************************************ */



;---------------------------------------------------------------
;
;        Global Symbol Defines - just to prepare line 344..
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
    .glb        _HWTDev_StimuISR     ; as timer tb5 isr vector

    .glb        _CompassRxIsr       ; as uart0 receive isr vector

    .glb        _GPI0_Int2_ISR      ; as p8_4 as INT2 source (connected to GPI0)
    .glb        _GPI1_Int3_ISR      ; as p1_5 as INT3 source (connected to GPI1)
    .glb        _GPI2_Int4_ISR      ; as p1_6 as INT4 source (connected to GPI2)
    .glb        _GPI3_Int5_ISR      ; as p1_7 as INT5 source (connected to GPI3)

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
M62TYPE                 .equ         1          ; for var vector table format


; ram sizes -------------------------------------------------------------
;   ! adapt these settings if neccessary !
HEAPSIZE                .equ    00400h          ; heap size
STACKSIZE               .equ    00600h          ; stack size
ISTACKSIZE              .equ     0400h          ; interrupt stack size


; rom sizes ------------------------------------------------------------
;   ! adapt these settings if neccessary !
ROM_DATA_SIZE           .equ   010000h          ; initialize data and const data
                                                ; (do NOT use for program code!)
ROM_CODE_INIT_SIZE      .equ     0100h          ; uC init program
ROM_CODE_PRG_SIZE       .equ   020000h          ; application program: 128 kByte
ROM_CODE_INT_SIZE       .equ    0FCFFh          ; interrupt service routines


; ram start/end addresses ----------------------------------------------
;   ! don't make any changes here !
RAM_SFR_START           .equ        0h          ; register reserved
RAM_SFR_END             .equ     03ffh
RAM_USER_START          .equ     0400h          ; user area: 20479 byte
.if KD30_USED==1
    RAM_USER_END        .equ    052ffh          ; restricted for mini emu use
    .assert "INFO: Prepared for KD30 usage!"
.else
    RAM_USER_END        .equ    053ffh          ; normal use
.endif


; rom start/end addresses ----------------------------------------------
;   ! don't make any changes here !
ROM_USER_START          .equ   0c0000h          ; flash area begin
                                                ; c0000h-cffffh is NOT usable
                                                ; as program memory after RESET!
                                                ; (see uC Manual, register pm1.3)
ROM_SPPAGES_VT          .equ   0ffe00h          ; special pages vector table, 476 bytes
ROM_FIX_VT              .equ   0fffdch          ; fixed vector table, 36 bytes

.if KD30_USED==1
    ROM_VAR_VT          .equ   0fbd00h          ; variable vector table, 256 bytes
.else                                           ; (monitor program begins at fbe00h)
    ROM_VAR_VT          .equ   0ffd00h          ; variable vector table, 256 bytes
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
        .lword        _GPI1_Int3_ISR     ; int3 used by GPI1
        .lword        _HWTDev_StimuISR    ; timerB5 used to stimulate WHEEL+RPM
        .lword        _dummy_int         ; timerB4 (for user)(vector 6)
        .lword        _dummy_int         ; timerB3 (for user)(vector 7)
        .lword        _GPI3_Int5_ISR     ; si/o4 /int5 used by GPI3
        .lword        _GPI2_Int4_ISR     ; si/o3 /int4 used by GPI2
        .lword        _dummy_int         ; Bus collision detection(for user)(v10)
        .lword        _dummy_int         ; DMA0(for user)(vector 11)
        .lword        _dummy_int         ; DMA1(for user)(vector 12)
        .lword        _dummy_int         ; Key input interrupt(for user)(vect 13)
        .lword        _dummy_int         ; A-D(for user)(vector 14)
        .lword        _dummy_int         ; uart2 transmit(for user)(vector 15)
        .lword        _dummy_int         ; uart2 receive(for user)(vector 16)
        .lword        _dummy_int         ; uart0 transmit(for user)(vector 17)
.if        COMPASSDRV==1
        .lword        _CompassRxIsr      ; uart0 receive for compass, note that
.else                                    ; there is no transmit ISR
        .lword        _dummy_int         ; uart0 receive(for user)(vector 18)
.endif
.if        KD30_USED==1
        .lword        0FF900h            ; uart1 tx interrupt for KD30 V3.02
        .lword        0FF900h            ; uart1 rx interrupt for KD30 V3.02
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
        .lword        _GPI0_Int2_ISR     ; int2 used by GPI0
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



