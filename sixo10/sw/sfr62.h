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
 *  Created:        22-10-2001 by Ralf Krizsan
 *  Module:         hardware settings
 *  Purpose:        M16C62 register definitions
 *  Comments:       -
 *
 ************************************************************************ */


/*******************************************************
*    pin & port enable/diable macros                   *
********************************************************/

#define PIN_NA   0      /* Pin not available */
#define PIN_IN   0      /* Pin is IN port */
#define PIN_OUT  1      /* Pin is OUT port */

#define PORT_NA  0x00   /* Port not available */
#define PORT_IN  0x00   /* Port is IN port */
#define PORT_OUT 0xFF   /* Port is OUT port */


/*******************************************************
*    declare SFR address                               *
********************************************************/

#pragma ADDRESS          pm0_addr      0004H        /* Processor mode register 0 */
#pragma ADDRESS          pm1_addr      0005H        /* Processor mode register 1 */
#pragma ADDRESS          cm0_addr      0006H        /* System clock control register 0 */
#pragma ADDRESS          cm1_addr      0007H        /* System clock control register 1 */
#pragma ADDRESS          csr_addr      0008H        /* Chip select control register */
#pragma ADDRESS          aier_addr     0009H        /* Addrese match interrupt enable register */
#pragma ADDRESS          prcr_addr     000aH        /* Protect register */
#pragma ADDRESS          dbr_addr      000bH        /* data bank register (62) */
#pragma ADDRESS          wdts_addr     000eH        /* Watchdog timer start register */
#pragma ADDRESS          wdc_addr      000fH        /* Watchdog timer control register */
#pragma ADDRESS          rmad0_addr    0010H        /* Address match interrupt register 0 */
#pragma ADDRESS          rmad1_addr    0014H        /* Address match interrupt register 1 */

#pragma ADDRESS          sar0_addr     0020H        /* DMA0 source pointer */
#pragma ADDRESS          dar0_addr     0024H        /* DMA0 destination pointer */
#pragma ADDRESS          tcr0_addr     0028H        /* DMA0 transfer counter */
#pragma ADDRESS          dm0con_addr   002cH        /* DMA0 control register */
#pragma ADDRESS          sar1_addr     0030H        /* DMA1 source pointer */
#pragma ADDRESS          dar1_addr     0034H        /* DMA1 destination pointer */
#pragma ADDRESS          tcr1_addr     0038H        /* DMA1 transfer counter */
#pragma ADDRESS          dm1con_addr   003cH        /* DMA1 control register */

#pragma ADDRESS          int3ic_addr   0044H        /* INT3 interrupt control register (62) */
#pragma ADDRESS          tb5ic_addr    0045H        /* Timer B5 interrupt control register (62) */
#pragma ADDRESS          tb4ic_addr    0046H        /* Timer B4 interrupt control register (62) */
#pragma ADDRESS          tb3ic_addr    0047H        /* Timer B3 interrupt control register (62) */
#pragma ADDRESS          s4ic_addr     0048H        /* Si/o4 interrupt control register (62) */
#pragma ADDRESS          s3ic_addr     0049H        /* Si/o3 interrupt control register (62) */
#pragma ADDRESS          int5ic_addr   0048H        /* INT5 interrupt control register */
#pragma ADDRESS          int4ic_addr   0049H        /* INT4 interrupt control register */
#pragma ADDRESS          bcnic_addr    004aH        /* Bus collision detection interrupt control register      (61)*/
#pragma ADDRESS          dm0ic_addr    004bH        /* DMA0 interrupt control register */
#pragma ADDRESS          dm1ic_addr    004cH        /* DMA1 interrupt control register */
#pragma ADDRESS          kupic_addr    004dH        /* Key input interrupt control register */
#pragma ADDRESS          adic_addr     004eH        /* A-D conversion interrupt control register */

#pragma ADDRESS          s2tic_addr    004fH        /* UART2 transmit interrupt control register       (61) */
#pragma ADDRESS          s2ric_addr    0050H        /* UART2 receive interrupt control register     (61) */
#pragma ADDRESS          s0tic_addr    0051H        /* UART0 transmit interrupt control register */
#pragma ADDRESS          s0ric_addr    0052H        /* UART0 receive interrupt control register */
#pragma ADDRESS          s1tic_addr    0053H        /* UART1 transmit interrupt control register */
#pragma ADDRESS          s1ric_addr    0054H        /* UART1 receive interrupt control register */

#pragma ADDRESS          ta0ic_addr    0055H        /* Timer A0 interrupt control register */
#pragma ADDRESS          ta1ic_addr    0056H        /* Timer A1 interrupt control register */
#pragma ADDRESS          ta2ic_addr    0057H        /* Timer A2 interrupt control register */
#pragma ADDRESS          ta3ic_addr    0058H        /* Timer A3 interrupt control register */
#pragma ADDRESS          ta4ic_addr    0059H        /* Timer A4 interrupt control register */
#pragma ADDRESS          tb0ic_addr    005aH        /* Timer B0 interrupt control register */
#pragma ADDRESS          tb1ic_addr    005bH        /* Timer B1 interrupt control register */
#pragma ADDRESS          tb2ic_addr    005cH        /* Timer B2 interrupt control register */

#pragma ADDRESS          int0ic_addr   005dH        /* INT0 interrupt control register */
#pragma ADDRESS          int1ic_addr   005eH        /* INT1 interrupt control register */
#pragma ADDRESS          int2ic_addr   005fH        /* INT2 interrupt control register */

#pragma ADDRESS          tbsr_addr     0340H        /* Timer B3,4,5 Count start flag (62) */
#pragma ADDRESS          ta11_addr     0342H        /* Timer A1-1 (62) */
#pragma ADDRESS          ta21_addr     0344H        /* Timer A2-1 (62) */
#pragma ADDRESS          ta41_addr     0346H        /* Timer A4-1 (62) */

#pragma ADDRESS          invc0_addr    0348H        /* Three-phase PWM control regester 0 (62) */
#pragma ADDRESS          invc1_addr    0349H        /* Three-phase PWM control register 1 (62) */
#pragma ADDRESS          idb0_addr     034aH        /* Three-phase output buffer register 0 (62) */
#pragma ADDRESS          idb1_addr     034bH        /* Three-phase output buffer register 1 (62) */
#pragma ADDRESS          dtt_addr      034cH        /* (62) */
#pragma ADDRESS          ictb2_addr    034dH        /* (62) */

#pragma ADDRESS          tb3_addr      0350H        /* Timer B3 register (62) */
#pragma ADDRESS          tb4_addr      0352H        /* Timer B4 register (62) */
#pragma ADDRESS          tb5_addr      0354H        /* Timer B5 register (62) */
#pragma ADDRESS          tb3mr_addr    035bH        /* Timer B3 mode register (62) */
#pragma ADDRESS          tb4mr_addr    035cH        /* Timer B4 mode register (62) */
#pragma ADDRESS          tb5mr_addr    035dH        /* Timer B5 mode register (62) */

#pragma ADDRESS          ifsr_addr     035fH        /* interrupt couse select register (62) */
#pragma ADDRESS          s3trr_addr    0360H        /* Si/o3 transmit/receive register (62) */
#pragma ADDRESS          s3c_addr      0362H        /* Si/o3 control register (62) */
#pragma ADDRESS          s3brg_addr    0363H        /* Si/o3 bit rate generator (62) */
#pragma ADDRESS          s4trr_addr    0364H        /* Si/o4 transmit/receive register */
#pragma ADDRESS          s4c_addr      0366H        /* Si/o4 control register */
#pragma ADDRESS          s4brg_addr    0367H        /* Si/o4 bit rate generator (62) */

#pragma ADDRESS          u2smr2_addr   0376H        /* UART2 special mode register2      (62)*/
#pragma ADDRESS          u2smr_addr    0377H        /* UART2 special mode register    (62)*/
#pragma ADDRESS          u2mr_addr     0378H        /* UART2 transmit/receive mode register    (61)*/
#pragma ADDRESS          u2brg_addr    0379H        /* UART2 bit rate generator        (61)*/
#pragma ADDRESS          u2tb_addr     037aH        /* UART2 transmit buffer register      (61)*/
#pragma ADDRESS          u2c0_addr     037cH        /* UART2 transmit/receive control register 0    (61)*/
#pragma ADDRESS          u2c1_addr     037dH        /* UART2 transmit/receive control register 1    (61)*/
#pragma ADDRESS          u2rb_addr     037eH        /* UART2 receive buffer register    (61)*/

#pragma ADDRESS          tabsr_addr    0380H        /* Count start flag */
#pragma ADDRESS          cpsrf_addr    0381H        /* Clock prescaler reset flag */
#pragma ADDRESS          onsf_addr     0382H        /* One-shot start flag */
#pragma ADDRESS          trgsr_addr    0383H        /* Trigger select register */
#pragma ADDRESS          udf_addr      0384H        /* Up-down flag */

#pragma ADDRESS          ta0_addr      0386H        /* Timer A0 */
#pragma ADDRESS          ta1_addr      0388H        /* Timer A1 */
#pragma ADDRESS          ta2_addr      038aH        /* Timer A2 */
#pragma ADDRESS          ta3_addr      038cH        /* Timer A3 */
#pragma ADDRESS          ta4_addr      038eH        /* Timer A4 */
#pragma ADDRESS          tb0_addr      0390H        /* Timer B0 */
#pragma ADDRESS          tb1_addr      0392H        /* Timer B1 */
#pragma ADDRESS          tb2_addr      0394H        /* Timer B2 */
#pragma ADDRESS          ta0mr_addr    0396H        /* Timer A0 mode register */
#pragma ADDRESS          ta1mr_addr    0397H        /* Timer A1 mode register */
#pragma ADDRESS          ta2mr_addr    0398H        /* Timer A2 mode register */
#pragma ADDRESS          ta3mr_addr    0399H        /* Timer A3 mode register */
#pragma ADDRESS          ta4mr_addr    039aH        /* Timer A4 mode register */
#pragma ADDRESS          tb0mr_addr    039bH        /* Timer B0 mode register */
#pragma ADDRESS          tb1mr_addr    039cH        /* Timer B1 mode register */
#pragma ADDRESS          tb2mr_addr    039dH        /* Timer B2 mode register */

#pragma ADDRESS          u0mr_addr     03a0H        /* UART0 transmit/receive mode register */
#pragma ADDRESS          u0brg_addr    03a1H        /* UART0 bit rate generator */
#pragma ADDRESS          u0tb_addr     03a2H        /* UART0 transmit buffer register */
#pragma ADDRESS          u0c0_addr     03a4H        /* UART0 transmit/receive control register 0 */
#pragma ADDRESS          u0c1_addr     03a5H        /* UART0 transmit/receive control register 1 */
#pragma ADDRESS          u0rb_addr     03a6H        /* UART0 receive buffer register */
#pragma ADDRESS          u1mr_addr     03a8H        /* UART1 transmit/receive mode register */
#pragma ADDRESS          u1brg_addr    03a9H        /* UART1 bit rate generator */
#pragma ADDRESS          u1tb_addr     03aaH        /* UART1 transmit buffer register */
#pragma ADDRESS          u1c0_addr     03acH        /* UART1 transmit/receive control register 0 */
#pragma ADDRESS          u1c1_addr     03adH        /* UART1 transmit/receive control register 1 */
#pragma ADDRESS          u1rb_addr     03aeH        /* UART1 receive buffer register */
#pragma ADDRESS          ucon_addr     03b0H        /* UART2 transmit/receive control register 2 */

#pragma ADDRESS          fmcr_addr     03b7H        /* Flash Control Register */
#pragma ADDRESS          dm0sl_addr    03b8H        /* DMA0 cause select register */
#pragma ADDRESS          dm1sl_addr    03baH        /* DMA1 cause select register */
#pragma ADDRESS          crcd_addr     03bcH        /* CRC data register */
#pragma ADDRESS          crcin_addr    03beH        /* CRC input register */

#pragma ADDRESS          ad0_addr      03c0H        /* A-D register 0 */
#pragma ADDRESS          ad1_addr      03c2H        /* A-D register 1 */
#pragma ADDRESS          ad2_addr      03c4H        /* A-D register 2 */
#pragma ADDRESS          ad3_addr      03c6H        /* A-D register 3 */
#pragma ADDRESS          ad4_addr      03c8H        /* A-D register 4 */
#pragma ADDRESS          ad5_addr      03caH        /* A-D register 5 */
#pragma ADDRESS          ad6_addr      03ccH        /* A-D register 6 */
#pragma ADDRESS          ad7_addr      03ceH        /* A-D register 7 */
#pragma ADDRESS          adcon2_addr   03d4H        /* A-D control register 2 */
#pragma ADDRESS          adcon0_addr   03d6H        /* A-D control register 0 */
#pragma ADDRESS          adcon1_addr   03d7H        /* A-D control register 1 */
#pragma ADDRESS          da0_addr      03d8H        /* D-A register 0 */
#pragma ADDRESS          da1_addr      03daH        /* D-A register 1 */
#pragma ADDRESS          dacon_addr    03dcH        /* D-A control register */

#pragma ADDRESS          p0_addr       03e0H        /* Port P0 */
#pragma ADDRESS          p1_addr       03e1H        /* Port P1 */
#pragma ADDRESS          pd0_addr      03e2H        /* Port P0 direction register */
#pragma ADDRESS          pd1_addr      03e3H        /* Port P1 direction register */
#pragma ADDRESS          p2_addr       03e4H        /* Port P2 */
#pragma ADDRESS          p3_addr       03e5H        /* Port P3 */
#pragma ADDRESS          pd2_addr      03e6H        /* Port P2 direction register */
#pragma ADDRESS          pd3_addr      03e7H        /* Port P3 direction register */
#pragma ADDRESS          p4_addr       03e8H        /* Port P4 */
#pragma ADDRESS          p5_addr       03e9H        /* Port P5 */
#pragma ADDRESS          pd4_addr      03eaH        /* Port P4 direction register */
#pragma ADDRESS          pd5_addr      03ebH        /* Port P5 direction register */
#pragma ADDRESS          p6_addr       03ecH        /* Port P6 */
#pragma ADDRESS          p7_addr       03edH        /* Port P7 */
#pragma ADDRESS          pd6_addr      03eeH        /* Port P6 direction register */
#pragma ADDRESS          pd7_addr      03efH        /* Port P7 direction register */
#pragma ADDRESS          p8_addr       03f0H        /* Port P8 */
#pragma ADDRESS          p9_addr       03f1H        /* Port P9 */
#pragma ADDRESS          pd8_addr      03f2H        /* Port P8 direction register */
#pragma ADDRESS          pd9_addr      03f3H        /* Port P9 direction register */
#pragma ADDRESS          p10_addr      03f4H        /* Port P10 */
#pragma ADDRESS          pd10_addr     03f6H        /* Port P10 direction register */
#pragma ADDRESS          pur0_addr     03fcH        /* Pull-up control register 0 */
#pragma ADDRESS          pur1_addr     03fdH        /* Pull-up control register 1 */
#pragma ADDRESS          pur2_addr     03feH        /* Pull-up control register 2 */
#pragma ADDRESS          pcr_addr      03ffH        /* Port control register */


/********************************************************
*    declare  SFR char                  *
********************************************************/
unsigned char    da0_addr;                  /* D-A register 0 */
#define          da0          da0_addr

unsigned char    da1_addr;                  /* D-A register 1 */
#define          da1          da1_addr

/********************************************************
*    declare SFR bit                    *
********************************************************/
struct bit_def {
        char    b0:1;
        char    b1:1;
        char    b2:1;
        char    b3:1;
        char    b4:1;
        char    b5:1;
        char    b6:1;
        char    b7:1;
};
union byte_def{
    struct bit_def bit;
    char    byte;
};

/*------------------------------------------------------
    Processor mode register 0 bit
------------------------------------------------------*/
union byte_def pm0_addr;
#define          pm0          pm0_addr.byte

#define          pm00          pm0_addr.bit.b0          /* Processor mode bit */
#define          pm01          pm0_addr.bit.b1          /* Processor mode bit */
#define          pm02          pm0_addr.bit.b2          /* R/W mode select bit*/
#define          pm03          pm0_addr.bit.b3          /* Software reset bit */
#define          pm04          pm0_addr.bit.b4          /* Multiplexed bus space select bit */
#define          pm05          pm0_addr.bit.b5          /* Multiplexed bus space select bit */
#define          pm06          pm0_addr.bit.b6          /* Port P40~P43 function select bit */
#define          pm07          pm0_addr.bit.b7          /* BCLK output disable bit */

/*------------------------------------------------------
    Processor mode register 1 bit
------------------------------------------------------*/
union byte_def pm1_addr;
#define          pm1          pm1_addr.byte

#define          pm13          pm1_addr.bit.b3          /* Intermal reserved area expansion bit (62) */
#define          pm14          pm1_addr.bit.b4          /* Memory area expansion bit (62) */
#define          pm15          pm1_addr.bit.b5          /*                              (62) */
#define          pm16          pm1_addr.bit.b6          /* Reserved bit */
#define          pm17          pm1_addr.bit.b7          /* Wait bit */

/*------------------------------------------------------
    System clock control register 0
------------------------------------------------------*/
union byte_def cm0_addr;
#define          cm0          cm0_addr.byte

#define          cm00          cm0_addr.bit.b0          /* Clock output function select bit */
#define          cm01          cm0_addr.bit.b1          /* Clock output function select bit */
#define          cm02          cm0_addr.bit.b2          /* WAIT peripheral function clock stop bit */
#define          cm03          cm0_addr.bit.b3          /* Xcin-Xcout drive capacity select bit */
#define          cm04          cm0_addr.bit.b4          /* Port Xc select bit */
#define          cm05          cm0_addr.bit.b5          /* Main clock stop bit */
#define          cm06          cm0_addr.bit.b6          /* Main clock division select bit 0 */
#define          cm07          cm0_addr.bit.b7          /* System clock select bit */

/*------------------------------------------------------
    System clock control register 1
------------------------------------------------------*/
union byte_def cm1_addr;
#define          cm1          cm1_addr.byte

#define          cm10          cm1_addr.bit.b0          /* All clock stop control bit */
#define          cm15          cm1_addr.bit.b5          /* Xin-Xout drive capacity select bit */
#define          cm16          cm1_addr.bit.b6          /* Main clock division select bit */
#define          cm17          cm1_addr.bit.b7          /* Main clock division select bit */

/*------------------------------------------------------
    Chip select control register
------------------------------------------------------*/
union byte_def csr_addr;
#define          csr          csr_addr.byte

#define          cs0          csr_addr.bit.b0        /* CS0 output enable bit */
#define          cs1          csr_addr.bit.b1        /* CS1 output enable bit */
#define          cs2          csr_addr.bit.b2        /* CS2 output enable bit */
#define          cs3          csr_addr.bit.b3        /* CS3 output enable bit */
#define          cs0w          csr_addr.bit.b4          /* CS0 wait bit */
#define          cs1w          csr_addr.bit.b5          /* CS1 wait bit */
#define          cs2w          csr_addr.bit.b6          /* CS2 wait bit */
#define          cs3w          csr_addr.bit.b7          /* CS3 wait bit */

/*------------------------------------------------------
    Addrese match interrupt enable register
------------------------------------------------------*/
union byte_def aier_addr;
#define          aier          aier_addr.byte

#define          aier0        aier_addr.bit.b0    /* Addrese match interrupt 0 enable bit */
#define          aier1        aier_addr.bit.b1    /* Addrese match interrupt 1 enable bit */

/*------------------------------------------------------
    Protect register
------------------------------------------------------*/
union byte_def prcr_addr;
#define          prcr          prcr_addr.byte

#define          prc0          prcr_addr.bit.b0      /* Enables writing to system clock control register 0,1 */
#define          prc1          prcr_addr.bit.b1      /* Enables writing to processor mode register 0,1 */
#define          prc2          prcr_addr.bit.b2      /* Enables writing to port P9 direction register */

/*------------------------------------------------------
    Data bank register (62)
------------------------------------------------------*/
union byte_def dbr_addr;
#define          dbr              dbr_addr.byte

#define          ofs          dbr_addr.bit.b2    /* Offset bit */
#define          bsr0          dbr_addr.bit.b3      /* Bank select bit 0 */
#define          bsr1          dbr_addr.bit.b4      /* Bank select bit 1 */
#define          bsr2          dbr_addr.bit.b5      /* Bank select bit 2 */

/*------------------------------------------------------
    Watchdog timer start register
------------------------------------------------------*/
union byte_def wdts_addr;
#define          wdts          wdts_addr.byte

/*------------------------------------------------------
    CRC input register
------------------------------------------------------*/
union byte_def crcin_addr;
#define          crcin        crcin_addr.byte

/*------------------------------------------------------
    Watchdog timer control register
------------------------------------------------------*/
union byte_def wdc_addr;
#define          wdc          wdc_addr.byte

#define          wdc5          wdc_addr.bit.b5
#define          wdc6          wdc_addr.bit.b6
#define          wdc7          wdc_addr.bit.b7          /* Prescaler select bit */

/*------------------------------------------------------
    Count start flag
------------------------------------------------------*/
union byte_def tabsr_addr;
#define          tabsr        tabsr_addr.byte

#define          ta0s          tabsr_addr.bit.b0    /* Timer A0 count start flag */
#define          ta1s          tabsr_addr.bit.b1    /* Timer A1 count start flag */
#define          ta2s          tabsr_addr.bit.b2    /* Timer A2 count start flag */
#define          ta3s          tabsr_addr.bit.b3    /* Timer A3 count start flag */
#define          ta4s          tabsr_addr.bit.b4    /* Timer A4 count start flag */
#define          tb0s          tabsr_addr.bit.b5    /* Timer B0 count start flag */
#define          tb1s          tabsr_addr.bit.b6    /* Timer B1 count start flag */
#define          tb2s          tabsr_addr.bit.b7    /* Timer B2 count start flag */

/*------------------------------------------------------
    Timer B3,4,5 Count start flag (62)
------------------------------------------------------*/
union byte_def tbsr_addr;
#define          tbsr          tbsr_addr.byte

#define          tb3s          tbsr_addr.bit.b5      /* Timer B3 count start flag */
#define          tb4s          tbsr_addr.bit.b6      /* Timer B4 count start flag */
#define          tb5s          tbsr_addr.bit.b7      /* Timer B5 count start flag */

/*------------------------------------------------------
    Three-phase PWM control regester 0 (62)
------------------------------------------------------*/
union byte_def invc0_addr;
#define          invc0        invc0_addr.byte

#define          inv00        invc0_addr.bit.b0    /* (62) */
#define          inv01        invc0_addr.bit.b1    /* (62) */
#define          inv02        invc0_addr.bit.b2    /* (62) */
#define          inv03        invc0_addr.bit.b3    /* (62) */
#define          inv04        invc0_addr.bit.b4    /* (62) */
#define          inv05        invc0_addr.bit.b5    /* (62) */
#define          inv06        invc0_addr.bit.b6    /* (62) */
#define          inv07        invc0_addr.bit.b7    /* (62) */

/*------------------------------------------------------
    Three-phase PWM control regester 1 (62)
------------------------------------------------------*/
union byte_def invc1_addr;
#define          invc1        invc1_addr.byte

#define          inv10        invc1_addr.bit.b0    /* (62) */
#define          inv11        invc1_addr.bit.b1    /* (62) */
#define          inv12        invc1_addr.bit.b2    /* (62) */
#define          inv14        invc1_addr.bit.b4    /* Reserved bit */

/*------------------------------------------------------
    Three-phase output buffer register 0 (62)
------------------------------------------------------*/
union byte_def idb0_addr;
#define          idb0          idb0_addr.byte

#define          du0           idb0_addr.bit.b0      /* (62) */
#define          dub0          idb0_addr.bit.b1      /* (62) */
#define          dv0           idb0_addr.bit.b2      /* (62) */
#define          dvb0          idb0_addr.bit.b3      /* (62) */
#define          dw0           idb0_addr.bit.b4      /* (62) */
#define          dwb0          idb0_addr.bit.b5      /* (62) */

/*------------------------------------------------------
    Three-phase output buffer register 1 (62)
------------------------------------------------------*/
union byte_def idb1_addr;
#define          idb1          idb1_addr.byte

#define          du1           idb1_addr.bit.b0      /* (62) */
#define          dub1          idb1_addr.bit.b1      /* (62) */
#define          dv1           idb1_addr.bit.b2      /* (62) */
#define          dvb1          idb1_addr.bit.b3      /* (62) */
#define          dw1           idb1_addr.bit.b4      /* (62) */
#define          dwb1          idb1_addr.bit.b5      /* (62) */

/*------------------------------------------------------
     (62)
------------------------------------------------------*/
union byte_def dtt_addr;
#define          dtt              dtt_addr.byte

/*------------------------------------------------------
     (62)
------------------------------------------------------*/
union byte_def ictb2_addr;
#define          ictb2        ictb2_addr.byte

/*------------------------------------------------------
    One-shot start flag
------------------------------------------------------*/
union byte_def onsf_addr;
#define          onsf          onsf_addr.byte

#define          ta0os        onsf_addr.bit.b0    /* Timer A0 one-shot start flag */
#define          ta1os        onsf_addr.bit.b1    /* Timer A1 one-shot start flag */
#define          ta2os        onsf_addr.bit.b2    /* Timer A2 one-shot start flag */
#define          ta3os        onsf_addr.bit.b3    /* Timer A3 one-shot start flag */
#define          ta4os        onsf_addr.bit.b4    /* Timer A4 one-shot start flag */
#define          ta0tgl       onsf_addr.bit.b6    /* Timer A0 event/trigger select bit */
#define          ta0tgh       onsf_addr.bit.b7    /* Timer A0 event/trigger select bit */

/*------------------------------------------------------
    Clock prescaler reset flag
------------------------------------------------------*/
union byte_def cpsrf_addr;
#define          cpsrf        cpsrf_addr.byte

#define          cpsr          cpsrf_addr.bit.b7    /* Clock prescaler reset flag */

/*------------------------------------------------------
    Trigger select register
------------------------------------------------------*/
union byte_def trgsr_addr;
#define          trgsr        trgsr_addr.byte

#define          ta1tgl        trgsr_addr.bit.b0      /* Timer A1 event/trigger select bit */
#define          ta1tgh        trgsr_addr.bit.b1      /* Timer A1 event/trigger select bit */
#define          ta2tgl        trgsr_addr.bit.b2      /* Timer A2 event/trigger select bit */
#define          ta2tgh        trgsr_addr.bit.b3      /* Timer A2 event/trigger select bit */
#define          ta3tgl        trgsr_addr.bit.b4      /* Timer A3 event/trigger select bit */
#define          ta3tgh        trgsr_addr.bit.b5      /* Timer A3 event/trigger select bit */
#define          ta4tgl        trgsr_addr.bit.b6      /* Timer A4 event/trigger select bit */
#define          ta4tgh        trgsr_addr.bit.b7      /* Timer A4 event/trigger select bit */

/*------------------------------------------------------
    Up/down flag
------------------------------------------------------*/
union byte_def udf_addr;
#define          udf          udf_addr.byte

#define          ta0ud        udf_addr.bit.b0          /* Timer A0 up/down flag */
#define          ta1ud        udf_addr.bit.b1          /* Timer A1 up/down flag */
#define          ta2ud        udf_addr.bit.b2          /* Timer A2 up/down flag */
#define          ta3ud        udf_addr.bit.b3          /* Timer A3 up/down flag */
#define          ta4ud        udf_addr.bit.b4          /* Timer A4 up/down flag */
#define          ta2p         udf_addr.bit.b5          /* Timer A2 two-phase pulse signal processing select bit */
#define          ta3p         udf_addr.bit.b6          /* Timer A3 two-phase pulse signal processing select bit */
#define          ta4p         udf_addr.bit.b7          /* Timer A4 two-phase pulse signal processing select bit */

/*------------------------------------------------------
    UART transmit/receive control register 2
------------------------------------------------------*/
union byte_def ucon_addr;
#define          ucon          ucon_addr.byte

#define          u0irs        ucon_addr.bit.b0    /* UART0 transmit interrupt cause select bit */
#define          u1irs        ucon_addr.bit.b1    /* UART1 transmit interrupt cause select bit */
#define          u0rrm        ucon_addr.bit.b2    /* UART0 continuous receive mode enable bit */
#define          u1rrm        ucon_addr.bit.b3    /* UART1 continuous receive mode enable bit */
#define          clkmd0       ucon_addr.bit.b4    /* CLK/CLKS select bit 0 */
#define          clkmd1       ucon_addr.bit.b5    /* CLK/CLKS select bit 1 */
#define          rcsp         ucon_addr.bit.b6    /* Separate CTS/RTS bit */

/*------------------------------------------------------
    UART2 transmit/receive control register 1                        (61)
------------------------------------------------------*/
union byte_def u2c1_addr;
#define          u2c1          u2c1_addr.byte
#define          te_u2c1      u2c1_addr.bit.b0      /* Transmit enable bit */
#define          ti_u2c1      u2c1_addr.bit.b1      /* Transmit buffer empty flag */
#define          re_u2c1      u2c1_addr.bit.b2      /* Receive enable bit */
#define          ri_u2c1      u2c1_addr.bit.b3      /* Receive complete flag */
#define          u2irs        u2c1_addr.bit.b4      /* UART2 transmit interrupt cause select bit */
#define          u2rrm        u2c1_addr.bit.b5      /* UART2 continuous receive mode enable bit */
#define          u2lch        u2c1_addr.bit.b6      /* Data logic select bit */
#define          u2ere        u2c1_addr.bit.b7      /* Error signal output enable bit */

/*------------------------------------------------------
    UART2 special mode register2    (62)
------------------------------------------------------*/
union byte_def u2smr2_addr;
#define          u2smr2            u2smr2_addr.byte

#define          iicm2    u2smr2_addr.bit.b0          /* IIC mode selection bit 2*/
#define          csc       u2smr2_addr.bit.b1        /* Clock-synchronous bit */
#define          swc       u2smr2_addr.bit.b2        /* SCL wait output bit */
#define          als       u2smr2_addr.bit.b3        /* SDA output stop bit */
#define          stac      u2smr2_addr.bit.b4        /* UART2 initialization bit */
#define          swc2      u2smr2_addr.bit.b5        /* SCL wait output bit 2 */
#define          sdhi      u2smr2_addr.bit.b6        /* SDA output disable bit */
#define          shtc      u2smr2_addr.bit.b7        /* Start/stop condition control bit */

/*------------------------------------------------------
    UART2 special mode register          (62)
------------------------------------------------------*/
union byte_def u2smr_addr;
#define          u2smr    u2smr_addr.byte
#define          iicm      u2smr_addr.bit.b0    /* IIC mode selection bit */
#define          abc       u2smr_addr.bit.b1    /* Arbitration lost detecting flag control bit */
#define          bbs       u2smr_addr.bit.b2    /* Bus busy flag */
#define          lsyn      u2smr_addr.bit.b3    /* SCLL sync output enable bit */
#define          abscs     u2smr_addr.bit.b4    /* Bus collision detect sampling clock select bit */
#define          acse      u2smr_addr.bit.b5    /* Auto clear function select bit of transmit enable bit */
#define          sss       u2smr_addr.bit.b6    /* Transmit start condition select bit */

/*------------------------------------------------------
    A-D control register 0
------------------------------------------------------*/
union byte_def adcon0_addr;
#define          adcon0        adcon0_addr.byte

#define          ch0          adcon0_addr.bit.b0    /* Analog input pin select bit */
#define          ch1          adcon0_addr.bit.b1    /* Analog input pin select bit */
#define          ch2          adcon0_addr.bit.b2    /* Analog input pin select bit */
#define          md0          adcon0_addr.bit.b3    /* A-D operation mode select bit 0 */
#define          md1          adcon0_addr.bit.b4    /* A-D operation mode select bit 0 */
#define          trg          adcon0_addr.bit.b5    /* Trigger select bit */
#define          adst         adcon0_addr.bit.b6    /* A-D conversion start flag*/
#define          cks0         adcon0_addr.bit.b7    /* Frequency select bit 0 */

/*------------------------------------------------------
    A-D control register 1
------------------------------------------------------*/
union byte_def adcon1_addr;
#define          adcon1        adcon1_addr.byte

#define          scan0        adcon1_addr.bit.b0    /* A-D sweep pin select bit */
#define          scan1        adcon1_addr.bit.b1    /* A-D sweep pin select bit */
#define          md2          adcon1_addr.bit.b2    /* A-D operation mode select bit 1 */
#define          bits         adcon1_addr.bit.b3    /* 8/10 bit mode select bit */
#define          cks1         adcon1_addr.bit.b4    /* Frequency select bit 1 */
#define          vcut         adcon1_addr.bit.b5    /* Vref connect bit */
#define          opa0         adcon1_addr.bit.b6    /* External op-amp connection mode bit */
#define          opa1         adcon1_addr.bit.b7    /* External op-amp connection mode bit */

/*------------------------------------------------------
    A-D control register 2
------------------------------------------------------*/
union byte_def adcon2_addr;
#define          adcon2        adcon2_addr.byte

#define          smp          adcon2_addr.bit.b0    /* A-D conversion method select bit */

/*------------------------------------------------------
    D-A control register
------------------------------------------------------*/
union byte_def dacon_addr;
#define          dacon        dacon_addr.byte

#define          da0e          dacon_addr.bit.b0    /* D-A0 output enable bit */
#define          da1e          dacon_addr.bit.b1    /* D-A1 output enable bit */

/*------------------------------------------------------
    Port P0
------------------------------------------------------*/
union byte_def p0_addr;
#define          p0        p0_addr.byte

#define          p0_0          p0_addr.bit.b0        /* Port P0  bit0 */
#define          p0_1          p0_addr.bit.b1        /* Port P0  bit1 */
#define          p0_2          p0_addr.bit.b2        /* Port P0  bit2 */
#define          p0_3          p0_addr.bit.b3        /* Port P0  bit3 */
#define          p0_4          p0_addr.bit.b4        /* Port P0  bit4 */
#define          p0_5          p0_addr.bit.b5        /* Port P0  bit5 */
#define          p0_6          p0_addr.bit.b6        /* Port P0  bit6 */
#define          p0_7          p0_addr.bit.b7        /* Port P0  bit7 */

/*------------------------------------------------------
    Port P0 direction register
------------------------------------------------------*/
union byte_def pd0_addr;
#define          pd0          pd0_addr.byte

#define          pd0_0        pd0_addr.bit.b0          /* P0 direction register  bit0 */
#define          pd0_1        pd0_addr.bit.b1          /* P0 direction register  bit1 */
#define          pd0_2        pd0_addr.bit.b2          /* P0 direction register  bit2 */
#define          pd0_3        pd0_addr.bit.b3          /* P0 direction register  bit3 */
#define          pd0_4        pd0_addr.bit.b4          /* P0 direction register  bit4 */
#define          pd0_5        pd0_addr.bit.b5          /* P0 direction register  bit5 */
#define          pd0_6        pd0_addr.bit.b6          /* P0 direction register  bit6 */
#define          pd0_7        pd0_addr.bit.b7          /* P0 direction register  bit7 */

/*------------------------------------------------------
    Port P1
------------------------------------------------------*/
union byte_def p1_addr;
#define          p1        p1_addr.byte

#define          p1_0          p1_addr.bit.b0        /* Port P1  bit0 */
#define          p1_1          p1_addr.bit.b1        /* Port P1  bit1 */
#define          p1_2          p1_addr.bit.b2        /* Port P1  bit2 */
#define          p1_3          p1_addr.bit.b3        /* Port P1  bit3 */
#define          p1_4          p1_addr.bit.b4        /* Port P1  bit4 */
#define          p1_5          p1_addr.bit.b5        /* Port P1  bit5 */
#define          p1_6          p1_addr.bit.b6        /* Port P1  bit6 */
#define          p1_7          p1_addr.bit.b7        /* Port P1  bit7 */

/*------------------------------------------------------
    Port P1 direction register
------------------------------------------------------*/
union byte_def pd1_addr;
#define          pd1          pd1_addr.byte

#define          pd1_0        pd1_addr.bit.b0          /* P1 direction register  bit0 */
#define          pd1_1        pd1_addr.bit.b1          /* P1 direction register  bit1 */
#define          pd1_2        pd1_addr.bit.b2          /* P1 direction register  bit2 */
#define          pd1_3        pd1_addr.bit.b3          /* P1 direction register  bit3 */
#define          pd1_4        pd1_addr.bit.b4          /* P1 direction register  bit4 */
#define          pd1_5        pd1_addr.bit.b5          /* P1 direction register  bit5 */
#define          pd1_6        pd1_addr.bit.b6          /* P1 direction register  bit6 */
#define          pd1_7        pd1_addr.bit.b7          /* P1 direction register  bit7 */

/*------------------------------------------------------
    Port P2
------------------------------------------------------*/
union byte_def p2_addr;
#define          p2        p2_addr.byte

#define          p2_0          p2_addr.bit.b0        /* Port P2  bit0 */
#define          p2_1          p2_addr.bit.b1        /* Port P2  bit1 */
#define          p2_2          p2_addr.bit.b2        /* Port P2  bit2 */
#define          p2_3          p2_addr.bit.b3        /* Port P2  bit3 */
#define          p2_4          p2_addr.bit.b4        /* Port P2  bit4 */
#define          p2_5          p2_addr.bit.b5        /* Port P2  bit5 */
#define          p2_6          p2_addr.bit.b6        /* Port P2  bit6 */
#define          p2_7          p2_addr.bit.b7        /* Port P2  bit7 */

/*------------------------------------------------------
    Port P2 direction register
------------------------------------------------------*/
union byte_def pd2_addr;
#define          pd2          pd2_addr.byte

#define          pd2_0        pd2_addr.bit.b0          /* P2 direction register  bit0 */
#define          pd2_1        pd2_addr.bit.b1          /* P2 direction register  bit1 */
#define          pd2_2        pd2_addr.bit.b2          /* P2 direction register  bit2 */
#define          pd2_3        pd2_addr.bit.b3          /* P2 direction register  bit3 */
#define          pd2_4        pd2_addr.bit.b4          /* P2 direction register  bit4 */
#define          pd2_5        pd2_addr.bit.b5          /* P2 direction register  bit5 */
#define          pd2_6        pd2_addr.bit.b6          /* P2 direction register  bit6 */
#define          pd2_7        pd2_addr.bit.b7          /* P2 direction register  bit7 */

/*------------------------------------------------------
    Port P3
------------------------------------------------------*/
union byte_def p3_addr;
#define          p3        p3_addr.byte

#define          p3_0          p3_addr.bit.b0        /* Port P3  bit0 */
#define          p3_1          p3_addr.bit.b1        /* Port P3  bit1 */
#define          p3_2          p3_addr.bit.b2        /* Port P3  bit2 */
#define          p3_3          p3_addr.bit.b3        /* Port P3  bit3 */
#define          p3_4          p3_addr.bit.b4        /* Port P3  bit4 */
#define          p3_5          p3_addr.bit.b5        /* Port P3  bit5 */
#define          p3_6          p3_addr.bit.b6        /* Port P3  bit6 */
#define          p3_7          p3_addr.bit.b7        /* Port P3  bit7 */

/*------------------------------------------------------
    Port P3 direction register
------------------------------------------------------*/
union byte_def pd3_addr;
#define          pd3          pd3_addr.byte

#define          pd3_0        pd3_addr.bit.b0          /* P3 direction register  bit0 */
#define          pd3_1        pd3_addr.bit.b1          /* P3 direction register  bit1 */
#define          pd3_2        pd3_addr.bit.b2          /* P3 direction register  bit2 */
#define          pd3_3        pd3_addr.bit.b3          /* P3 direction register  bit3 */
#define          pd3_4        pd3_addr.bit.b4          /* P3 direction register  bit4 */
#define          pd3_5        pd3_addr.bit.b5          /* P3 direction register  bit5 */
#define          pd3_6        pd3_addr.bit.b6          /* P3 direction register  bit6 */
#define          pd3_7        pd3_addr.bit.b7          /* P3 direction register  bit7 */

/*------------------------------------------------------
    Port P4
------------------------------------------------------*/
union byte_def p4_addr;
#define          p4        p4_addr.byte

#define          p4_0          p4_addr.bit.b0        /* Port P4  bit0 */
#define          p4_1          p4_addr.bit.b1        /* Port P4  bit1 */
#define          p4_2          p4_addr.bit.b2        /* Port P4  bit2 */
#define          p4_3          p4_addr.bit.b3        /* Port P4  bit3 */
#define          p4_4          p4_addr.bit.b4        /* Port P4  bit4 */
#define          p4_5          p4_addr.bit.b5        /* Port P4  bit5 */
#define          p4_6          p4_addr.bit.b6        /* Port P4  bit6 */
#define          p4_7          p4_addr.bit.b7        /* Port P4  bit7 */

/*------------------------------------------------------
    Port P4 direction register
------------------------------------------------------*/
union byte_def pd4_addr;
#define          pd4          pd4_addr.byte

#define          pd4_0        pd4_addr.bit.b0          /* P4 direction register  bit0 */
#define          pd4_1        pd4_addr.bit.b1          /* P4 direction register  bit1 */
#define          pd4_2        pd4_addr.bit.b2          /* P4 direction register  bit2 */
#define          pd4_3        pd4_addr.bit.b3          /* P4 direction register  bit3 */
#define          pd4_4        pd4_addr.bit.b4          /* P4 direction register  bit4 */
#define          pd4_5        pd4_addr.bit.b5          /* P4 direction register  bit5 */
#define          pd4_6        pd4_addr.bit.b6          /* P4 direction register  bit6 */
#define          pd4_7        pd4_addr.bit.b7          /* P4 direction register  bit7 */

/*------------------------------------------------------
    Port P5
------------------------------------------------------*/
union byte_def p5_addr;
#define          p5        p5_addr.byte

#define          p5_0          p5_addr.bit.b0        /* Port P5  bit0 */
#define          p5_1          p5_addr.bit.b1        /* Port P5  bit1 */
#define          p5_2          p5_addr.bit.b2        /* Port P5  bit2 */
#define          p5_3          p5_addr.bit.b3        /* Port P5  bit3 */
#define          p5_4          p5_addr.bit.b4        /* Port P5  bit4 */
#define          p5_5          p5_addr.bit.b5        /* Port P5  bit5 */
#define          p5_6          p5_addr.bit.b6        /* Port P5  bit6 */
#define          p5_7          p5_addr.bit.b7        /* Port P5  bit7 */

/*------------------------------------------------------
    Port P5 direction register
------------------------------------------------------*/
union byte_def pd5_addr;
#define          pd5          pd5_addr.byte

#define          pd5_0        pd5_addr.bit.b0          /* P5 direction register  bit0 */
#define          pd5_1        pd5_addr.bit.b1          /* P5 direction register  bit1 */
#define          pd5_2        pd5_addr.bit.b2          /* P5 direction register  bit2 */
#define          pd5_3        pd5_addr.bit.b3          /* P5 direction register  bit3 */
#define          pd5_4        pd5_addr.bit.b4          /* P5 direction register  bit4 */
#define          pd5_5        pd5_addr.bit.b5          /* P5 direction register  bit5 */
#define          pd5_6        pd5_addr.bit.b6          /* P5 direction register  bit6 */
#define          pd5_7        pd5_addr.bit.b7          /* P5 direction register  bit7 */

/*------------------------------------------------------
    Port P6  bit
------------------------------------------------------*/
union byte_def p6_addr;
#define          p6        p6_addr.byte

#define          p6_0          p6_addr.bit.b0        /* Port P6  bit0 */
#define          p6_1          p6_addr.bit.b1        /* Port P6  bit1 */
#define          p6_2          p6_addr.bit.b2        /* Port P6  bit2 */
#define          p6_3          p6_addr.bit.b3        /* Port P6  bit3 */
#define          p6_4          p6_addr.bit.b4        /* Port P6  bit4 */
#define          p6_5          p6_addr.bit.b5        /* Port P6  bit5 */
#define          p6_6          p6_addr.bit.b6        /* Port P6  bit6 */
#define          p6_7          p6_addr.bit.b7        /* Port P6  bit7 */

/*------------------------------------------------------
    Port P6 direction register  bit
------------------------------------------------------*/
union byte_def pd6_addr;
#define          pd6          pd6_addr.byte

#define          pd6_0        pd6_addr.bit.b0          /* P6 direction register  bit0 */
#define          pd6_1        pd6_addr.bit.b1          /* P6 direction register  bit1 */
#define          pd6_2        pd6_addr.bit.b2          /* P6 direction register  bit2 */
#define          pd6_3        pd6_addr.bit.b3          /* P6 direction register  bit3 */
#define          pd6_4        pd6_addr.bit.b4          /* P6 direction register  bit4 */
#define          pd6_5        pd6_addr.bit.b5          /* P6 direction register  bit5 */
#define          pd6_6        pd6_addr.bit.b6          /* P6 direction register  bit6 */
#define          pd6_7        pd6_addr.bit.b7          /* P6 direction register  bit7 */

/*------------------------------------------------------
    Port P7  bit
------------------------------------------------------*/
union byte_def p7_addr;
#define          p7        p7_addr.byte

#define          p7_0          p7_addr.bit.b0        /* Port P7  bit0 */
#define          p7_1          p7_addr.bit.b1        /* Port P7  bit1 */
#define          p7_2          p7_addr.bit.b2        /* Port P7  bit2 */
#define          p7_3          p7_addr.bit.b3        /* Port P7  bit3 */
#define          p7_4          p7_addr.bit.b4        /* Port P7  bit4 */
#define          p7_5          p7_addr.bit.b5        /* Port P7  bit5 */
#define          p7_6          p7_addr.bit.b6        /* Port P7  bit6 */
#define          p7_7          p7_addr.bit.b7        /* Port P7  bit7 */

/*------------------------------------------------------
    Port P7 direction register  bit
------------------------------------------------------*/
union byte_def pd7_addr;
#define          pd7          pd7_addr.byte

#define          pd7_0        pd7_addr.bit.b0          /* P7 direction register  bit0 */
#define          pd7_1        pd7_addr.bit.b1          /* P7 direction register  bit1 */
#define          pd7_2        pd7_addr.bit.b2          /* P7 direction register  bit2 */
#define          pd7_3        pd7_addr.bit.b3          /* P7 direction register  bit3 */
#define          pd7_4        pd7_addr.bit.b4          /* P7 direction register  bit4 */
#define          pd7_5        pd7_addr.bit.b5          /* P7 direction register  bit5 */
#define          pd7_6        pd7_addr.bit.b6          /* P7 direction register  bit6 */
#define          pd7_7        pd7_addr.bit.b7          /* P7 direction register  bit7 */

/*------------------------------------------------------
    Port P8  bit
------------------------------------------------------*/
union byte_def p8_addr;
#define          p8        p8_addr.byte

#define          p8_0          p8_addr.bit.b0        /* Port P8  bit0 */
#define          p8_1          p8_addr.bit.b1        /* Port P8  bit1 */
#define          p8_2          p8_addr.bit.b2        /* Port P8  bit2 */
#define          p8_3          p8_addr.bit.b3        /* Port P8  bit3 */
#define          p8_4          p8_addr.bit.b4        /* Port P8  bit4 */
#define          p8_5          p8_addr.bit.b5        /* Port P8  bit5 */
#define          p8_6          p8_addr.bit.b6        /* Port P8  bit6 */
#define          p8_7          p8_addr.bit.b7        /* Port P8  bit7 */

/*------------------------------------------------------
    Port P8 direction register  bit’è‹`
------------------------------------------------------*/
union byte_def pd8_addr;
#define          pd8          pd8_addr.byte

#define          pd8_0        pd8_addr.bit.b0          /* P8 direction register  bit0 */
#define          pd8_1        pd8_addr.bit.b1          /* P8 direction register  bit1 */
#define          pd8_2        pd8_addr.bit.b2          /* P8 direction register  bit2 */
#define          pd8_3        pd8_addr.bit.b3          /* P8 direction register  bit3 */
#define          pd8_4        pd8_addr.bit.b4          /* P8 direction register  bit4 */
#define          pd8_5        pd8_addr.bit.b5          /* P8 direction register  bit5 */
#define          pd8_6        pd8_addr.bit.b6          /* P8 direction register  bit6 */
#define          pd8_7        pd8_addr.bit.b7          /* P8 direction register  bit7 */

/*------------------------------------------------------
    Port P9 bit
------------------------------------------------------*/
union byte_def p9_addr;
#define          p9        p9_addr.byte

#define          p9_0          p9_addr.bit.b0        /* Port P‚X  bit0 */
#define          p9_1          p9_addr.bit.b1        /* Port P‚X  bit1 */
#define          p9_2          p9_addr.bit.b2        /* Port P‚X  bit2 */
#define          p9_3          p9_addr.bit.b3        /* Port P‚X  bit3 */
#define          p9_4          p9_addr.bit.b4        /* Port P‚X  bit4 */
#define          p9_5          p9_addr.bit.b5        /* Port P‚X  bit5 */
#define          p9_6          p9_addr.bit.b6        /* Port P‚X  bit6 */
#define          p9_7          p9_addr.bit.b7        /* Port P‚X  bit7 */

/*------------------------------------------------------
    Port P9 direction register
------------------------------------------------------*/
union byte_def pd9_addr;
#define          pd9          pd9_addr.byte

#define          pd9_0        pd9_addr.bit.b0          /* P‚X direction register  bit0 */
#define          pd9_1        pd9_addr.bit.b1          /* P‚X direction register  bit1 */
#define          pd9_2        pd9_addr.bit.b2          /* P‚X direction register  bit2 */
#define          pd9_3        pd9_addr.bit.b3          /* P‚X direction register  bit3 */
#define          pd9_4        pd9_addr.bit.b4          /* P‚X direction register  bit4 */
#define          pd9_5        pd9_addr.bit.b5          /* P‚X direction register  bit5 */
#define          pd9_6        pd9_addr.bit.b6          /* P‚X direction register  bit6 */
#define          pd9_7        pd9_addr.bit.b7          /* P‚X direction register  bit7 */

/*------------------------------------------------------
    Port P10
------------------------------------------------------*/
union byte_def p10_addr;
#define          p10          p10_addr.byte

#define          p10_0        p10_addr.bit.b0          /* Port P10  bit0 */
#define          p10_1        p10_addr.bit.b1          /* Port P10  bit1 */
#define          p10_2        p10_addr.bit.b2          /* Port P10  bit2 */
#define          p10_3        p10_addr.bit.b3          /* Port P10  bit3 */
#define          p10_4        p10_addr.bit.b4          /* Port P10  bit4 */
#define          p10_5        p10_addr.bit.b5          /* Port P10  bit5 */
#define          p10_6        p10_addr.bit.b6          /* Port P10  bit6 */
#define          p10_7        p10_addr.bit.b7          /* Port P10  bit7 */

/*------------------------------------------------------
    Port P10 direction register
------------------------------------------------------*/
union byte_def pd10_addr;
#define          pd10          pd10_addr.byte

#define          pd10_0        pd10_addr.bit.b0    /* P10 direction register  bit0 */
#define          pd10_1        pd10_addr.bit.b1    /* P10 direction register  bit1 */
#define          pd10_2        pd10_addr.bit.b2    /* P10 direction register  bit2 */
#define          pd10_3        pd10_addr.bit.b3    /* P10 direction register  bit3 */
#define          pd10_4        pd10_addr.bit.b4    /* P10 direction register  bit4 */
#define          pd10_5        pd10_addr.bit.b5    /* P10 direction register  bit5 */
#define          pd10_6        pd10_addr.bit.b6    /* P10 direction register  bit6 */
#define          pd10_7        pd10_addr.bit.b7    /* P10 direction register  bit7 */

/*------------------------------------------------------
    Pull-up control register 0
------------------------------------------------------*/
union byte_def pur0_addr;
#define          pur0          pur0_addr.byte

#define          pu00          pur0_addr.bit.b0      /* Pull-up P00 to P03 */
#define          pu01          pur0_addr.bit.b1      /* Pull-up P04 to P07 */
#define          pu02          pur0_addr.bit.b2      /* Pull-up P10 to P13 */
#define          pu03          pur0_addr.bit.b3      /* Pull-up P14 to P17 */
#define          pu04          pur0_addr.bit.b4      /* Pull-up P20 to P23 */
#define          pu05          pur0_addr.bit.b5      /* Pull-up P24 to P27 */
#define          pu06          pur0_addr.bit.b6      /* Pull-up P30 to P33 */
#define          pu07          pur0_addr.bit.b7      /* Pull-up P34 to P37 */

/*------------------------------------------------------
    Pull-up control register 1
------------------------------------------------------*/
union byte_def pur1_addr;
#define          pur1          pur1_addr.byte

#define          pu10          pur1_addr.bit.b0      /* Pull-up P40 to P43 */
#define          pu11          pur1_addr.bit.b1      /* Pull-up P44 to P47 */
#define          pu12          pur1_addr.bit.b2      /* Pull-up P50 to P53 */
#define          pu13          pur1_addr.bit.b3      /* Pull-up P54 to P57 */
#define          pu14          pur1_addr.bit.b4      /* Pull-up P60 to P63 */
#define          pu15          pur1_addr.bit.b5      /* Pull-up P64 to P67 */
#define          pu16          pur1_addr.bit.b6      /* Pull-up P70 to P73 */
#define          pu17          pur1_addr.bit.b7      /* Pull-up P74 to P77 */

/*------------------------------------------------------
    Pull-up control register2
------------------------------------------------------*/
union byte_def pur2_addr;
#define          pur2          pur2_addr.byte

#define          pu20          pur2_addr.bit.b0      /* Pull-up P80 to P83 */
#define          pu21          pur2_addr.bit.b1      /* Pull-up P84 to P87 */
#define          pu22          pur2_addr.bit.b2      /* Pull-up P90 to P93 */
#define          pu23          pur2_addr.bit.b3      /* Pull-up P94 to P97 */
#define          pu24          pur2_addr.bit.b4      /* Pull-up P100 to P103 */
#define          pu25          pur2_addr.bit.b5      /* Pull-up P104 to P107 */

/*------------------------------------------------------
    Port control register
------------------------------------------------------*/
union byte_def pcr_addr;
#define          pcr          pcr_addr.byte

#define          pcr0          pcr_addr.bit.b0      /* Port P1 control register */

union byte_def u0brg_addr;
#define          u0brg        u0brg_addr.byte

union byte_def u1brg_addr;
#define          u1brg        u1brg_addr.byte

union byte_def u2brg_addr;                                      /* (61) */
#define          u2brg        u2brg_addr.byte

union byte_def s3brg_addr;                                      /* (62) */
#define          s3brg        s3brg_addr.byte

union byte_def s4brg_addr;                                      /* (62) */
#define          s4brg        s4brg_addr.byte

union byte_def s3trr_addr;                                      /* (62) */
#define          s3trr        s3trr_addr.byte

union byte_def s4trr_addr;                                      /* (62) */
#define          s4trr        s4trr_addr.byte

/********************************************************
*    declare SFR union                  *
********************************************************/

/*------------------------------------------------------
    DMAi control register union
------------------------------------------------------*/
union{
    struct{
        char    dmbit:1;            /* Transfer unit bit select bit */
        char    dmasl:1;            /* Repeat transfer mode select bit */
        char    dmas:1;                  /* DMA request bit */
        char    dmae:1;                  /* DMA enable bit */
        char    dsd:1;                  /* Source address direction select bit */
        char    dad:1;                  /* Destination addressdirection select bit */
        char    b6:1;
        char    b7:1;
    }bit;
    char    byte;
}dm0con_addr,dm1con_addr;
/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          dm0con        dm0con_addr.byte

#define          dmbit_dm0con      dm0con_addr.bit.dmbit    /* Transfer unit bit select bit */
#define          dmasl_dm0con      dm0con_addr.bit.dmasl    /* Repeat transfer mode select bit */
#define          dmas_dm0con      dm0con_addr.bit.dmas      /* DMA request bit */
#define          dmae_dm0con      dm0con_addr.bit.dmae      /* DMA enable bit */
#define          dsd_dm0con    dm0con_addr.bit.dsd    /* Source address direction select bit */
#define          dad_dm0con    dm0con_addr.bit.dad    /* Destination addressdirection select bit */

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          dm1con        dm1con_addr.byte

#define          dmbit_dm1con      dm1con_addr.bit.dmbit    /* Transfer unit bit select bit */
#define          dmasl_dm1con      dm1con_addr.bit.dmasl    /* Repeat transfer mode select bit */
#define          dmas_dm1con      dm1con_addr.bit.dmas      /* DMA request bit */
#define          dmae_dm1con      dm1con_addr.bit.dmae      /* DMA enable bit */
#define          dsd_dm1con    dm1con_addr.bit.dsd    /* Source address direction select bit */
#define          dad_dm1con    dm1con_addr.bit.dad    /* Destination addressdirection select bit */

/*------------------------------------------------------
    interrupt control register (61)
------------------------------------------------------*/
union{
    struct{
        char    ilvl0:1;            /* Interrupt priority level select bit */
        char    ilvl1:1;            /* Interrupt priority level select bit */
        char    ilvl2:1;            /* Interrupt priority level select bit */
        char    ir:1;               /* Interrupt request bit */
        char    pol:1;              /* Polarity select bit */
        char    lvs:1;
        char    b6:1;
        char    b7:1;
    }bit;
    char    byte;
}bcnic_addr,dm0ic_addr,dm1ic_addr,kupic_addr,adic_addr,
 s2tic_addr,s2ric_addr,s0tic_addr,s0ric_addr,s1tic_addr,s1ric_addr,
 s3ic_addr,s4ic_addr,
 ta0ic_addr,ta1ic_addr,ta2ic_addr,ta3ic_addr,ta4ic_addr,
 tb0ic_addr,tb1ic_addr,tb2ic_addr,tb3ic_addr,tb4ic_addr,tb5ic_addr,
 int0ic_addr,int1ic_addr,int2ic_addr,int3ic_addr,int4ic_addr,int5ic_addr;

/*------------------------------------------------------
     bit    (61)
------------------------------------------------------*/
#define          bcnic        bcnic_addr.byte

#define          ilvl0_bcnic      bcnic_addr.bit.ilvl0
#define          ilvl1_bcnic      bcnic_addr.bit.ilvl1
#define          ilvl2_bcnic      bcnic_addr.bit.ilvl2
#define          ir_bcnic      bcnic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          dm0ic        dm0ic_addr.byte

#define          ilvl0_dm0ic      dm0ic_addr.bit.ilvl0
#define          ilvl1_dm0ic      dm0ic_addr.bit.ilvl1
#define          ilvl2_dm0ic      dm0ic_addr.bit.ilvl2
#define          ir_dm0ic      dm0ic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          dm1ic        dm1ic_addr.byte

#define          ilvl0_dm1ic      dm1ic_addr.bit.ilvl0
#define          ilvl1_dm1ic      dm1ic_addr.bit.ilvl1
#define          ilvl2_dm1ic      dm1ic_addr.bit.ilvl2
#define          ir_dm1ic      dm1ic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          kupic        kupic_addr.byte

#define          ilvl0_kupic      kupic_addr.bit.ilvl0
#define          ilvl1_kupic      kupic_addr.bit.ilvl1
#define          ilvl2_kupic      kupic_addr.bit.ilvl2
#define          ir_kupic      kupic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          adic          adic_addr.byte

#define          ilvl0_adic    adic_addr.bit.ilvl0
#define          ilvl1_adic    adic_addr.bit.ilvl1
#define          ilvl2_adic    adic_addr.bit.ilvl2
#define          ir_adic          adic_addr.bit.ir

/*------------------------------------------------------
     bit    (61)
------------------------------------------------------*/
#define          s2tic        s2tic_addr.byte

#define          ilvl0_s2tic      s2tic_addr.bit.ilvl0
#define          ilvl1_s2tic      s2tic_addr.bit.ilvl1
#define          ilvl2_s2tic      s2tic_addr.bit.ilvl2
#define          ir_s2tic      s2tic_addr.bit.ir

/*------------------------------------------------------
     bit    (61)
------------------------------------------------------*/
#define          s2ric        s2ric_addr.byte

#define          ilvl0_s2ric      s2ric_addr.bit.ilvl0
#define          ilvl1_s2ric      s2ric_addr.bit.ilvl1
#define          ilvl2_s2ric      s2ric_addr.bit.ilvl2
#define          ir_s2ric      s2ric_addr.bit.ir


/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          s0tic        s0tic_addr.byte

#define          ilvl0_s0tic      s0tic_addr.bit.ilvl0
#define          ilvl1_s0tic      s0tic_addr.bit.ilvl1
#define          ilvl2_s0tic      s0tic_addr.bit.ilvl2
#define          ir_s0tic      s0tic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          s0ric        s0ric_addr.byte

#define          ilvl0_s0ric      s0ric_addr.bit.ilvl0
#define          ilvl1_s0ric      s0ric_addr.bit.ilvl1
#define          ilvl2_s0ric      s0ric_addr.bit.ilvl2
#define          ir_s0ric      s0ric_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          s1tic        s1tic_addr.byte

#define          ilvl0_s1tic      s1tic_addr.bit.ilvl0
#define          ilvl1_s1tic      s1tic_addr.bit.ilvl1
#define          ilvl2_s1tic      s1tic_addr.bit.ilvl2
#define          ir_s1tic      s1tic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          s1ric        s1ric_addr.byte

#define          ilvl0_s1ric      s1ric_addr.bit.ilvl0
#define          ilvl1_s1ric      s1ric_addr.bit.ilvl1
#define          ilvl2_s1ric      s1ric_addr.bit.ilvl2
#define          ir_s1ric      s1ric_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          s3ic          s3ic_addr.byte

#define          ilvl0_s3ic    s3ic_addr.bit.ilvl0
#define          ilvl1_s3ic    s3ic_addr.bit.ilvl1
#define          ilvl2_s3ic    s3ic_addr.bit.ilvl2
#define          ir_s3ic          s3ic_addr.bit.ir
#define          pol_s3ic      s3ic_addr.bit.pol
#define          lvs_s3ic      s3ic_addr.bit.lvs

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          s4ic          s4ic_addr.byte

#define          ilvl0_s4ic    s4ic_addr.bit.ilvl0
#define          ilvl1_s4ic    s4ic_addr.bit.ilvl1
#define          ilvl2_s4ic    s4ic_addr.bit.ilvl2
#define          ir_s4ic          s4ic_addr.bit.ir
#define          pol_s4ic      s4ic_addr.bit.pol
#define          lvs_s4ic      s4ic_addr.bit.lvs

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          ta0ic        ta0ic_addr.byte

#define          ilvl0_ta0ic      ta0ic_addr.bit.ilvl0
#define          ilvl1_ta0ic      ta0ic_addr.bit.ilvl1
#define          ilvl2_ta0ic      ta0ic_addr.bit.ilvl2
#define          ir_ta0ic      ta0ic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          ta1ic        ta1ic_addr.byte

#define          ilvl0_ta1ic      ta1ic_addr.bit.ilvl0
#define          ilvl1_ta1ic      ta1ic_addr.bit.ilvl1
#define          ilvl2_ta1ic      ta1ic_addr.bit.ilvl2
#define          ir_ta1ic      ta1ic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          ta2ic        ta2ic_addr.byte

#define          ilvl0_ta2ic      ta2ic_addr.bit.ilvl0
#define          ilvl1_ta2ic      ta2ic_addr.bit.ilvl1
#define          ilvl2_ta2ic      ta2ic_addr.bit.ilvl2
#define          ir_ta2ic      ta2ic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          ta3ic        ta3ic_addr.byte

#define          ilvl0_ta3ic      ta3ic_addr.bit.ilvl0
#define          ilvl1_ta3ic      ta3ic_addr.bit.ilvl1
#define          ilvl2_ta3ic      ta3ic_addr.bit.ilvl2
#define          ir_ta3ic      ta3ic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          ta4ic        ta4ic_addr.byte

#define          ilvl0_ta4ic      ta4ic_addr.bit.ilvl0
#define          ilvl1_ta4ic      ta4ic_addr.bit.ilvl1
#define          ilvl2_ta4ic      ta4ic_addr.bit.ilvl2
#define          ir_ta4ic      ta4ic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          tb0ic        tb0ic_addr.byte

#define          ilvl0_tb0ic      tb0ic_addr.bit.ilvl0
#define          ilvl1_tb0ic      tb0ic_addr.bit.ilvl1
#define          ilvl2_tb0ic      tb0ic_addr.bit.ilvl2
#define          ir_tb0ic      tb0ic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          tb1ic        tb1ic_addr.byte

#define          ilvl0_tb1ic      tb1ic_addr.bit.ilvl0
#define          ilvl1_tb1ic      tb1ic_addr.bit.ilvl1
#define          ilvl2_tb1ic      tb1ic_addr.bit.ilvl2
#define          ir_tb1ic      tb1ic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          tb2ic        tb2ic_addr.byte

#define          ilvl0_tb2ic      tb2ic_addr.bit.ilvl0
#define          ilvl1_tb2ic      tb2ic_addr.bit.ilvl1
#define          ilvl2_tb2ic      tb2ic_addr.bit.ilvl2
#define          ir_tb2ic      tb2ic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          tb3ic        tb3ic_addr.byte

#define          ilvl0_tb3ic      tb3ic_addr.bit.ilvl0
#define          ilvl1_tb3ic      tb3ic_addr.bit.ilvl1
#define          ilvl2_tb3ic      tb3ic_addr.bit.ilvl2
#define          ir_tb3ic      tb3ic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          tb4ic        tb4ic_addr.byte

#define          ilvl0_tb4ic      tb4ic_addr.bit.ilvl0
#define          ilvl1_tb4ic      tb4ic_addr.bit.ilvl1
#define          ilvl2_tb4ic      tb4ic_addr.bit.ilvl2
#define          ir_tb4ic      tb4ic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          tb5ic        tb5ic_addr.byte

#define          ilvl0_tb5ic      tb5ic_addr.bit.ilvl0
#define          ilvl1_tb5ic      tb5ic_addr.bit.ilvl1
#define          ilvl2_tb5ic      tb5ic_addr.bit.ilvl2
#define          ir_tb5ic      tb5ic_addr.bit.ir

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          int5ic        int5ic_addr.byte

#define          ilvl0_int5ic      int5ic_addr.bit.ilvl0
#define          ilvl1_int5ic      int5ic_addr.bit.ilvl1
#define          ilvl2_int5ic      int5ic_addr.bit.ilvl2
#define          ir_int5ic    int5ic_addr.bit.ir
#define          pol_int5ic    int5ic_addr.bit.pol
#define          lvs_int5ic    int5ic_addr.bit.lvs

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          int4ic        int4ic_addr.byte

#define          ilvl0_int4ic      int4ic_addr.bit.ilvl0
#define          ilvl1_int4ic      int4ic_addr.bit.ilvl1
#define          ilvl2_int4ic      int4ic_addr.bit.ilvl2
#define          ir_int4ic    int4ic_addr.bit.ir
#define          pol_int4ic    int4ic_addr.bit.pol
#define          lvs_int4ic    int4ic_addr.bit.lvs

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          int0ic        int0ic_addr.byte

#define          ilvl0_int0ic      int0ic_addr.bit.ilvl0
#define          ilvl1_int0ic      int0ic_addr.bit.ilvl1
#define          ilvl2_int0ic      int0ic_addr.bit.ilvl2
#define          ir_int0ic    int0ic_addr.bit.ir
#define          pol_int0ic    int0ic_addr.bit.pol
#define          lvs_int0ic    int0ic_addr.bit.lvs

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          int1ic        int1ic_addr.byte

#define          ilvl0_int1ic      int1ic_addr.bit.ilvl0
#define          ilvl1_int1ic      int1ic_addr.bit.ilvl1
#define          ilvl2_int1ic      int1ic_addr.bit.ilvl2
#define          ir_int1ic    int1ic_addr.bit.ir
#define          pol_int1ic    int1ic_addr.bit.pol
#define          lvs_int1ic    int1ic_addr.bit.lvs

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          int2ic        int2ic_addr.byte

#define          ilvl0_int2ic      int2ic_addr.bit.ilvl0
#define          ilvl1_int2ic      int2ic_addr.bit.ilvl1
#define          ilvl2_int2ic      int2ic_addr.bit.ilvl2
#define          ir_int2ic    int2ic_addr.bit.ir
#define          pol_int2ic    int2ic_addr.bit.pol
#define          lvs_int2ic    int2ic_addr.bit.lvs

/*------------------------------------------------------
     bit
------------------------------------------------------*/
#define          int3ic        int3ic_addr.byte

#define          ilvl0_int3ic      int3ic_addr.bit.ilvl0
#define          ilvl1_int3ic      int3ic_addr.bit.ilvl1
#define          ilvl2_int3ic      int3ic_addr.bit.ilvl2
#define          ir_int3ic    int3ic_addr.bit.ir
#define          pol_int3ic    int3ic_addr.bit.pol
#define          lvs_int3ic    int3ic_addr.bit.lvs

/*------------------------------------------------------
    Timer mode register
------------------------------------------------------*/
union{
    struct{
        char    tmod0:1;            /* Operation mode select bit */
        char    tmod1:1;            /* Operation mode select bit */
        char    mr0:1;
        char    mr1:1;
        char    mr2:1;
        char    mr3:1;
        char    tck0:1;                  /* Count source select bit */
        char    tck1:1;                  /* Count source select bit */
    }bit;
    char    byte;
}ta0mr_addr,ta1mr_addr,ta2mr_addr,ta3mr_addr,ta4mr_addr,
 tb0mr_addr,tb1mr_addr,tb2mr_addr,tb3mr_addr,tb4mr_addr,tb5mr_addr;

/*------------------------------------------------------
     bit
ta0mr
------------------------------------------------------*/
#define          ta0mr        ta0mr_addr.byte

#define          tmod0_ta0mr      ta0mr_addr.bit.tmod0          /* Operation mode select bit */
#define          tmod1_ta0mr      ta0mr_addr.bit.tmod1          /* Operation mode select bit */
#define          mr0_ta0mr    ta0mr_addr.bit.mr0
#define          mr1_ta0mr    ta0mr_addr.bit.mr1
#define          mr2_ta0mr    ta0mr_addr.bit.mr2
#define          mr3_ta0mr    ta0mr_addr.bit.mr3
#define          tck0_ta0mr    ta0mr_addr.bit.tck0        /* Count source select bit */
#define          tck1_ta0mr    ta0mr_addr.bit.tck1        /* Count source select bit */

/*------------------------------------------------------
     bit
ta1mr
------------------------------------------------------*/
#define          ta1mr        ta1mr_addr.byte

#define          tmod0_ta1mr      ta1mr_addr.bit.tmod0          /* Operation mode select bit */
#define          tmod1_ta1mr      ta1mr_addr.bit.tmod1          /* Operation mode select bit */
#define          mr0_ta1mr    ta1mr_addr.bit.mr0
#define          mr1_ta1mr    ta1mr_addr.bit.mr1
#define          mr2_ta1mr    ta1mr_addr.bit.mr2
#define          mr3_ta1mr    ta1mr_addr.bit.mr3
#define          tck0_ta1mr    ta1mr_addr.bit.tck0        /* Count source select bit */
#define          tck1_ta1mr    ta1mr_addr.bit.tck1        /* Count source select bit */

/*------------------------------------------------------
     bit
ta2mr
------------------------------------------------------*/
#define          ta2mr        ta2mr_addr.byte

#define          tmod0_ta2mr      ta2mr_addr.bit.tmod0          /* Operation mode select bit */
#define          tmod1_ta2mr      ta2mr_addr.bit.tmod1          /* Operation mode select bit */
#define          mr0_ta2mr    ta2mr_addr.bit.mr0
#define          mr1_ta2mr    ta2mr_addr.bit.mr1
#define          mr2_ta2mr    ta2mr_addr.bit.mr2
#define          mr3_ta2mr    ta2mr_addr.bit.mr3
#define          tck0_ta2mr    ta2mr_addr.bit.tck0        /* Count source select bit */
#define          tck1_ta2mr    ta2mr_addr.bit.tck1        /* Count source select bit */

/*------------------------------------------------------
     bit
ta3mr
------------------------------------------------------*/
#define          ta3mr        ta3mr_addr.byte

#define          tmod0_ta3mr      ta3mr_addr.bit.tmod0          /* Operation mode select bit */
#define          tmod1_ta3mr      ta3mr_addr.bit.tmod1          /* Operation mode select bit */
#define          mr0_ta3mr    ta3mr_addr.bit.mr0
#define          mr1_ta3mr    ta3mr_addr.bit.mr1
#define          mr2_ta3mr    ta3mr_addr.bit.mr2
#define          mr3_ta3mr    ta3mr_addr.bit.mr3
#define          tck0_ta3mr    ta3mr_addr.bit.tck0        /* Count source select bit */
#define          tck1_ta3mr    ta3mr_addr.bit.tck1        /* Count source select bit */

/*------------------------------------------------------
     bit
ta4mr
------------------------------------------------------*/
#define          ta4mr        ta4mr_addr.byte

#define          tmod0_ta4mr      ta4mr_addr.bit.tmod0          /* Operation mode select bit */
#define          tmod1_ta4mr      ta4mr_addr.bit.tmod1          /* Operation mode select bit */
#define          mr0_ta4mr    ta4mr_addr.bit.mr0
#define          mr1_ta4mr    ta4mr_addr.bit.mr1
#define          mr2_ta4mr    ta4mr_addr.bit.mr2
#define          mr3_ta4mr    ta4mr_addr.bit.mr3
#define          tck0_ta4mr    ta4mr_addr.bit.tck0        /* Count source select bit */
#define          tck1_ta4mr    ta4mr_addr.bit.tck1        /* Count source select bit */

/*------------------------------------------------------
     bit
 tb0mr
 ------------------------------------------------------*/
#define          tb0mr        tb0mr_addr.byte

#define          tmod0_tb0mr      tb0mr_addr.bit.tmod0          /* Operation mode select bit */
#define          tmod1_tb0mr      tb0mr_addr.bit.tmod1          /* Operation mode select bit */
#define          mr0_tb0mr    tb0mr_addr.bit.mr0
#define          mr1_tb0mr    tb0mr_addr.bit.mr1
#define          mr2_tb0mr    tb0mr_addr.bit.mr2
#define          mr3_tb0mr    tb0mr_addr.bit.mr3
#define          tck0_tb0mr    tb0mr_addr.bit.tck0        /* Count source select bit */
#define          tck1_tb0mr    tb0mr_addr.bit.tck1        /* Count source select bit */

/*------------------------------------------------------
     bit
tb1mr
------------------------------------------------------*/
#define          tb1mr        tb1mr_addr.byte

#define          tmod0_tb1mr      tb1mr_addr.bit.tmod0          /* Operation mode select bit */
#define          tmod1_tb1mr      tb1mr_addr.bit.tmod1          /* Operation mode select bit */
#define          mr0_tb1mr    tb1mr_addr.bit.mr0
#define          mr1_tb1mr    tb1mr_addr.bit.mr1
#define          mr2_tb1mr    tb1mr_addr.bit.mr2
#define          mr3_tb1mr    tb1mr_addr.bit.mr3
#define          tck0_tb1mr    tb1mr_addr.bit.tck0        /* Count source select bit */
#define          tck1_tb1mr    tb1mr_addr.bit.tck1        /* Count source select bit */

/*------------------------------------------------------
     bit
tb2mr
------------------------------------------------------*/
#define          tb2mr        tb2mr_addr.byte

#define          tmod0_tb2mr      tb2mr_addr.bit.tmod0          /* Operation mode select bit */
#define          tmod1_tb2mr      tb2mr_addr.bit.tmod1          /* Operation mode select bit */
#define          mr0_tb2mr    tb2mr_addr.bit.mr0
#define          mr1_tb2mr    tb2mr_addr.bit.mr1
#define          mr2_tb2mr    tb2mr_addr.bit.mr2
#define          mr3_tb2mr    tb2mr_addr.bit.mr3
#define          tck0_tb2mr    tb2mr_addr.bit.tck0        /* Count source select bit */
#define          tck1_tb2mr    tb2mr_addr.bit.tck1        /* Count source select bit */

/*------------------------------------------------------
     bit
tb3mr
------------------------------------------------------*/
#define          tb3mr        tb3mr_addr.byte

#define          tmod0_tb3mr      tb3mr_addr.bit.tmod0          /* Operation mode select bit */
#define          tmod1_tb3mr      tb3mr_addr.bit.tmod1          /* Operation mode select bit */
#define          mr0_tb3mr    tb3mr_addr.bit.mr0
#define          mr1_tb3mr    tb3mr_addr.bit.mr1
#define          mr2_tb3mr    tb3mr_addr.bit.mr2
#define          mr3_tb3mr    tb3mr_addr.bit.mr3
#define          tck0_tb3mr    tb3mr_addr.bit.tck0        /* Count source select bit */
#define          tck1_tb3mr    tb3mr_addr.bit.tck1        /* Count source select bit */

/*------------------------------------------------------
     bit
tb4mr
------------------------------------------------------*/
#define          tb4mr        tb4mr_addr.byte

#define          tmod0_tb4mr      tb4mr_addr.bit.tmod0          /* Operation mode select bit */
#define          tmod1_tb4mr      tb4mr_addr.bit.tmod1          /* Operation mode select bit */
#define          mr0_tb4mr    tb4mr_addr.bit.mr0
#define          mr1_tb4mr    tb4mr_addr.bit.mr1
#define          mr2_tb4mr    tb4mr_addr.bit.mr2
#define          mr3_tb4mr    tb4mr_addr.bit.mr3
#define          tck0_tb4mr    tb4mr_addr.bit.tck0        /* Count source select bit */
#define          tck1_tb4mr    tb4mr_addr.bit.tck1        /* Count source select bit */

/*------------------------------------------------------
     bit
tb5mr
------------------------------------------------------*/
#define          tb5mr        tb5mr_addr.byte

#define          tmod0_tb5mr      tb5mr_addr.bit.tmod0          /* Operation mode select bit */
#define          tmod1_tb5mr      tb5mr_addr.bit.tmod1          /* Operation mode select bit */
#define          mr0_tb5mr    tb5mr_addr.bit.mr0
#define          mr1_tb5mr    tb5mr_addr.bit.mr1
#define          mr2_tb5mr    tb5mr_addr.bit.mr2
#define          mr3_tb5mr    tb5mr_addr.bit.mr3
#define          tck0_tb5mr    tb5mr_addr.bit.tck0        /* Count source select bit */
#define          tck1_tb5mr    tb5mr_addr.bit.tck1        /* Count source select bit */

/*------------------------------------------------------
     bit
ifsr_addr
------------------------------------------------------*/
union byte_def ifsr_addr;
#define          ifsr          ifsr_addr.byte

#define          ifsr0        ifsr_addr.bit.b0        /* (62) */
#define          ifsr1        ifsr_addr.bit.b1        /* (62) */
#define          ifsr2        ifsr_addr.bit.b2        /* (62) */
#define          ifsr3        ifsr_addr.bit.b3        /* (62) */
#define          ifsr4        ifsr_addr.bit.b4        /* (62) */
#define          ifsr5        ifsr_addr.bit.b5        /* (62) */
#define          ifsr6        ifsr_addr.bit.b6        /* (62) */
#define          ifsr7        ifsr_addr.bit.b7        /* (62) */

/*------------------------------------------------------
    UARTi transmit/receive mode register
------------------------------------------------------*/
union{
    struct{
        char    smd0:1;                  /* Serial I/O mode select bit */
        char    smd1:1;                  /* Serial I/O mode select bit */
        char    smd2:1;                  /* Serial I/O mode select bit */
        char    ckdir:1;            /* Internal/external clock select bit */
        char    stps:1;                  /* Stop bit length select bit */
        char    pry:1;                  /* Odd/even parity select bit */
        char    prye:1;                  /* Parity enable bit */
        char    slep:1;                  /* Sleep select bit */
    }bit;
    char    byte;
}u0mr_addr,u1mr_addr,u2mr_addr;                                      /* (61) */

/*------------------------------------------------------
     bit
u0mr
------------------------------------------------------*/
#define          u0mr          u0mr_addr.byte

#define          smd0_u0mr    u0mr_addr.bit.smd0      /* Serial I/O mode select bit */
#define          smd1_u0mr    u0mr_addr.bit.smd1      /* Serial I/O mode select bit */
#define          smd2_u0mr    u0mr_addr.bit.smd2      /* Serial I/O mode select bit */
#define          ckdir_u0mr    u0mr_addr.bit.ckdir    /* Internal/external clock select bit */
#define          stps_u0mr    u0mr_addr.bit.stps      /* Stop bit length select bit */
#define          pry_u0mr      u0mr_addr.bit.pry    /* Odd/even parity select bit */
#define          prye_u0mr    u0mr_addr.bit.prye      /* Parity enable bit */
#define          slep_u0mr    u0mr_addr.bit.slep      /* Sleep select bit */

/*------------------------------------------------------
     bit
u1mr
------------------------------------------------------*/
#define          u1mr          u1mr_addr.byte

#define          smd0_u1mr    u1mr_addr.bit.smd0      /* Serial I/O mode select bit */
#define          smd1_u1mr    u1mr_addr.bit.smd1      /* Serial I/O mode select bit */
#define          smd2_u1mr    u1mr_addr.bit.smd2      /* Serial I/O mode select bit */
#define          ckdir_u1mr    u1mr_addr.bit.ckdir    /* Internal/external clock select bit */
#define          stps_u1mr    u1mr_addr.bit.stps      /* Stop bit length select bit */
#define          pry_u1mr      u1mr_addr.bit.pry    /* Odd/even parity select bit */
#define          prye_u1mr    u1mr_addr.bit.prye      /* Parity enable bit */
#define          slep_u1mr    u1mr_addr.bit.slep      /* Sleep select bit */

/*------------------------------------------------------
     bit    (61)
u2mr
------------------------------------------------------*/
#define          u2mr          u2mr_addr.byte

#define          smd0_u2mr    u2mr_addr.bit.smd0      /* Serial I/O mode select bit */
#define          smd1_u2mr    u2mr_addr.bit.smd1      /* Serial I/O mode select bit */
#define          smd2_u2mr    u2mr_addr.bit.smd2      /* Serial I/O mode select bit */
#define          ckdir_u2mr    u2mr_addr.bit.ckdir    /* Internal/external clock select bit */
#define          stps_u2mr    u2mr_addr.bit.stps      /* Stop bit length select bit */
#define          pry_u2mr      u2mr_addr.bit.pry    /* Odd/even parity select bit */
#define          prye_u2mr    u2mr_addr.bit.prye      /* Parity enable bit */
#define          iopol_u2mr    u2mr_addr.bit.slep      /* TxD RxD I/O polarity reverse bit */

/*------------------------------------------------------
    UARTi transmit/receive control register 0
------------------------------------------------------*/
union{
    struct{
        char    clk0:1;                  /* BRG Count source select bit */
        char    clk1:1;                  /* BRG Count source select bit */
        char    crs:1;                  /* CTS/RTS function select bit */
        char    txept:1;            /* Transmit register empty flag */
        char    crd:1;                  /* CTS/RTS disable bit */
        char    nch:1;                  /* Data output select bit */
        char    ckpol:1;            /* CLK polarity select bit */
        char    uform:1;            /* Transfer format select bit */
    }bit;
    char    byte;
}u0c0_addr,u1c0_addr,u2c0_addr;                              /* (61) */

/*------------------------------------------------------
     bit

------------------------------------------------------*/
#define          u0c0          u0c0_addr.byte

#define          clk0_u0c0    u0c0_addr.bit.clk0      /* BRGCount source select bit */
#define          clk1_u0c0    u0c0_addr.bit.clk1      /* BRGCount source select bit */
#define          crs_u0c0      u0c0_addr.bit.crs    /* CTS/RTS function select bit */
#define          txept_u0c0    u0c0_addr.bit.txept    /* Transmit register empty flag */
#define          crd_u0c0      u0c0_addr.bit.crd    /* CTS/RTS disable bit */
#define          nch_u0c0      u0c0_addr.bit.nch    /* Data output select bit */
#define          ckpol_u0c0    u0c0_addr.bit.ckpol    /* CLK polarity select bit */
#define          uform_u0c0    u0c0_addr.bit.uform    /* Transfer format select bit */

/*------------------------------------------------------
     bit

------------------------------------------------------*/
#define          u1c0          u1c0_addr.byte

#define          clk0_u1c0    u1c0_addr.bit.clk0      /* BRGCount source select bit */
#define          clk1_u1c0    u1c0_addr.bit.clk1      /* BRGCount source select bit */
#define          crs_u1c0      u1c0_addr.bit.crs    /* CTS/RTS function select bit */
#define          txept_u1c0    u1c0_addr.bit.txept    /* Transmit register empty flag */
#define          crd_u1c0      u1c0_addr.bit.crd    /* CTS/RTS disable bit */
#define          nch_u1c0      u1c0_addr.bit.nch    /* Data output select bit */
#define          ckpol_u1c0    u1c0_addr.bit.ckpol    /* CLK polarity select bit */
#define          uform_u1c0    u1c0_addr.bit.uform    /* Transfer format select bit */

/*------------------------------------------------------
     bit    (61)

------------------------------------------------------*/
#define          u2c0          u2c0_addr.byte

#define          clk0_u2c0    u2c0_addr.bit.clk0      /* BRGCount source select bit */
#define          clk1_u2c0    u2c0_addr.bit.clk1      /* BRGCount source select bit */
#define          crs_u2c0      u2c0_addr.bit.crs    /* CTS/RTS function select bit */
#define          txept_u2c0    u2c0_addr.bit.txept    /* Transmit register empty flag */
#define          crd_u2c0      u2c0_addr.bit.crd    /* CTS/RTS disable bit */
#define          ckpol_u2c0    u2c0_addr.bit.ckpol    /* CLK polarity select bit */
#define          uform_u2c0    u2c0_addr.bit.uform    /* Transfer format select bit */

/*------------------------------------------------------
    UARTi transmit/receive control register 1
------------------------------------------------------*/
union{
    struct{
        char    te:1;                /* Transmit enable bit */
        char    ti:1;                /* Transmit buffer empty flag */
        char    re:1;                /* Receive enable bit */
        char    ri:1;                /* Receive complete flag */
        char    b4:1;
        char    b5:1;
        char    b6:1;
        char    b7:1;
    }bit;
    char    byte;
}u0c1_addr,u1c1_addr;

/*------------------------------------------------------
     bit

------------------------------------------------------*/
#define          u0c1          u0c1_addr.byte

#define          te_u0c1          u0c1_addr.bit.te      /* Transmit enable bit */
#define          ti_u0c1          u0c1_addr.bit.ti      /* Transmit buffer empty flag */
#define          re_u0c1          u0c1_addr.bit.re      /* Receive enable bit */
#define          ri_u0c1          u0c1_addr.bit.ri      /* Receive complete flag */

/*------------------------------------------------------
     bit

------------------------------------------------------*/
#define          u1c1          u1c1_addr.byte

#define          te_u1c1          u1c1_addr.bit.te      /* Transmit enable bit */
#define          ti_u1c1          u1c1_addr.bit.ti      /* Transmit buffer empty flag */
#define          re_u1c1          u1c1_addr.bit.re      /* Receive enable bit */
#define          ri_u1c1          u1c1_addr.bit.ri      /* Receive complete flag */

/*------------------------------------------------------
    UARTi receive buffer register
------------------------------------------------------*/
union{
    struct{
        char    b0:1;
        char    b1:1;
        char    b2:1;
        char    b3:1;
        char    b4:1;
        char    b5:1;
        char    b6:1;
        char    b7:1;
        char    b8:1;
        char    b9:1;
        char    b10:1;
        char    abt:1;                  /* Arbitrastion lost detecting flag */
        char    oer:1;                  /* Overrun error flag */
        char    fer:1;                  /* Framing error flag*/
        char    per:1;                  /* Parity error flag */
        char    sum:1;                  /* Error sum flag */
    }bit;
     struct{
        char    low;                /* Low  8 bit */
        char    high;                /* High 8 bit */
    }byte;
    unsigned short      word;
}u0rb_addr,u1rb_addr,u2rb_addr;                              /* (61) */

/*------------------------------------------------------
     bit

------------------------------------------------------*/
#define          u0rb          u0rb_addr.word
#define          u0rbl        u0rb_addr.byte.low
#define          u0rbh        u0rb_addr.byte.high

#define          abt_u0rb      u0rb_addr.bit.abt    /* Arbitrastion lost detecting flag */
#define          oer_u0rb      u0rb_addr.bit.oer    /* Overrun error flag */
#define          fer_u0rb      u0rb_addr.bit.fer    /* Framing error flag*/
#define          per_u0rb      u0rb_addr.bit.per    /* Parity error flag */
#define          sum_u0rb      u0rb_addr.bit.sum    /* Error sum flag */

/*------------------------------------------------------
     bit

------------------------------------------------------*/
#define          u1rb          u1rb_addr.word
#define          u1rbl        u1rb_addr.byte.low
#define          u1rbh        u1rb_addr.byte.high

#define          abt_u1rb      u1rb_addr.bit.abt    /* Arbitrastion lost detecting flag */
#define          oer_u1rb      u1rb_addr.bit.oer    /* Overrun error flag */
#define          fer_u1rb      u1rb_addr.bit.fer    /* Framing error flag*/
#define          per_u1rb      u1rb_addr.bit.per    /* Parity error flag */
#define          sum_u1rb      u1rb_addr.bit.sum    /* Error sum flag */

/*------------------------------------------------------
     bit    (61)

------------------------------------------------------*/
#define          u2rb          u2rb_addr.word
#define          u2rbl        u2rb_addr.byte.low
#define          u2rbh        u2rb_addr.byte.high

#define          abt_u2rb      u2rb_addr.bit.abt    /* Arbitrastion lost detecting flag */
#define          oer_u2rb      u2rb_addr.bit.oer    /* Overrun error flag */
#define          fer_u2rb      u2rb_addr.bit.fer    /* Framing error flag*/
#define          per_u2rb      u2rb_addr.bit.per    /* Parity error flag */
#define          sum_u2rb      u2rb_addr.bit.sum    /* Error sum flag */

/*------------------------------------------------------
    Si/o3,4 control register (62)
------------------------------------------------------*/
union{
    struct{
        char    smi0:1;                  /* (62) */
        char    smi1:1;                  /* (62) */
        char    smi2:1;                  /* (62) */
        char    smi3:1;                  /* (62) */
        char    smi4:1;                  /* (62) */
        char    smi5:1;                  /* (62) */
        char    smi6:1;                  /* (62) */
        char    smi7:1;                  /* (62) */
    }bit;
    char    byte;
}s3c_addr,s4c_addr;                              /* (62) */

/*------------------------------------------------------
     bit

------------------------------------------------------*/
#define          s3c          s3c_addr.byte

#define          smi0_s3c      s3c_addr.bit.smi0    /* (62) */
#define          smi1_s3c      s3c_addr.bit.smi1    /* (62) */
#define          smi2_s3c      s3c_addr.bit.smi2    /* (62) */
#define          smi3_s3c      s3c_addr.bit.smi3    /* (62) */
#define          smi4_s3c      s3c_addr.bit.smi4    /* (62) */
#define          smi5_s3c      s3c_addr.bit.smi5    /* (62) */
#define          smi6_s3c      s3c_addr.bit.smi6    /* (62) */
#define          smi7_s3c      s3c_addr.bit.smi7    /* (62) */

/*------------------------------------------------------
     bit

------------------------------------------------------*/
#define          s4c          s4c_addr.byte

#define          smi0_s4c      s4c_addr.bit.smi0    /* (62) */
#define          smi1_s4c      s4c_addr.bit.smi1    /* (62) */
#define          smi2_s4c      s4c_addr.bit.smi2    /* (62) */
#define          smi3_s4c      s4c_addr.bit.smi3    /* (62) */
#define          smi4_s4c      s4c_addr.bit.smi4    /* (62) */
#define          smi5_s4c      s4c_addr.bit.smi5    /* (62) */
#define          smi6_s4c      s4c_addr.bit.smi6    /* (62) */
#define          smi7_s4c      s4c_addr.bit.smi7    /* (62) */

/*------------------------------------------------------
    Flash Control Register
------------------------------------------------------*/
union{
    struct{
        char    fmcr0:1;            /* RY/BY status bit */
        char    fmcr1:1;            /* Flash entry bit */
        char    fmcr2:1;            /* Lock disable bit */
        char    fmcr3:1;            /* Flash reset bit */
        char    b4:1;
        char    fmcr5:1;            /* User ROM select bit */
        char    b6:1;
        char    b7:1;
    }bit;
    char    byte;
}fmcr_addr;

/*------------------------------------------------------
     bit

------------------------------------------------------*/
#define          fmcr          fmcr_addr.byte

#define          fmcr0    fmcr_addr.bit.fmcr0      /*  RY/BY status bit */
#define          fmcr1    fmcr_addr.bit.fmcr1      /*  Flash entry bit */
#define          fmcr2    fmcr_addr.bit.fmcr2      /* Lock disable bit */
#define          fmcr3    fmcr_addr.bit.fmcr3      /* DMA request cause select bit */
#define          fmcr5    fmcr_addr.bit.fmcr5      /*  User ROM select bit */

/*------------------------------------------------------
    DMAi request cause register
------------------------------------------------------*/
union{
    struct{
        char    dsel0:1;            /* DMA request cause select bit */
        char    dsel1:1;            /* DMA request cause select bit */
        char    dsel2:1;            /* DMA request cause select bit */
        char    dsel3:1;            /* DMA request cause select bit */
        char    b4:1;
        char    b5:1;
        char    dms:1;
        char    dsr:1;                  /* Software DMA request bit */
    }bit;
    char    byte;
}dm0sl_addr,dm1sl_addr;

/*------------------------------------------------------
     bit

------------------------------------------------------*/
#define          dm0sl        dm0sl_addr.byte

#define          dsel0_dm0sl      dm0sl_addr.bit.dsel0      /* DMA request cause select bit */
#define          dsel1_dm0sl      dm0sl_addr.bit.dsel1      /* DMA request cause select bit */
#define          dsel2_dm0sl      dm0sl_addr.bit.dsel2      /* DMA request cause select bit */
#define          dsel3_dm0sl      dm0sl_addr.bit.dsel3      /* DMA request cause select bit */
#define          dms_dm0sl    dm0sl_addr.bit.dms
#define          dsr_dm0sl    dm0sl_addr.bit.dsr      /* Software DMA request bit */

/*------------------------------------------------------
     bit

------------------------------------------------------*/
#define          dm1sl        dm1sl_addr.byte

#define          dsel0_dm1sl      dm1sl_addr.bit.dsel0      /* DMA request cause select bit */
#define          dsel1_dm1sl      dm1sl_addr.bit.dsel1      /* DMA request cause select bit */
#define          dsel2_dm1sl      dm1sl_addr.bit.dsel2      /* DMA request cause select bit */
#define          dsel3_dm1sl      dm1sl_addr.bit.dsel3      /* DMA request cause select bit */
#define          dms_dm1sl    dm1sl_addr.bit.dms
#define          dsr_dm1sl    dm1sl_addr.bit.dsr      /* Software DMA request bit */

/********************************************************
*    declare SFR union                  *
********************************************************/

union{
    struct{
        char    b0:1;
        char    b1:1;
        char    b2:1;
        char    b3:1;
        char    b4:1;
        char    b5:1;
        char    b6:1;
        char    b7:1;
        char    b8:1;
        char    b9:1;
        char    b10:1;
        char    b11:1;
        char    b12:1;
        char    b13:1;
        char    b14:1;
        char    b15:1;
        char    b16:1;
        char    b17:1;
        char    b18:1;
        char    b19:1;
     }bit;
     struct{
        char    low;                /* low  8 bit */
        char    mid;                /* mid  8 bit */
        char    high;                /* high 8 bit */
        char    nc;                  /* non use */
    }byte;
    unsigned long    dword;
}rmad0_addr,rmad1_addr,sar0_addr,sar1_addr,dar0_addr,dar1_addr;
#define          rmad0        rmad0_addr.dword      /* Address match interrupt register0 32 bit */
#define          rmad0l       rmad0_addr.byte.low   /* Address match interrupt register0 low  8 bit */
#define          rmad0m       rmad0_addr.byte.mid   /* Address match interrupt register0 mid  8 bitt */
#define          rmad0h       rmad0_addr.byte.high  /* Address match interrupt register0 high 8 bit */
#define          rmad1        rmad1_addr.dword      /* Address match interrupt register1 32 bit */
#define          rmad1l       rmad1_addr.byte.low   /* Address match interrupt register1 low  8 bit */
#define          rmad1m       rmad1_addr.byte.mid   /* Address match interrupt register1 mid  8 bit */
#define          rmad1h       rmad1_addr.byte.high  /* Address match interrupt register1 high 8 bit */
#define          sar0         sar0_addr.dword       /* DMA0 source pointer 32 bit */
#define          sar0l        sar0_addr.byte.low    /* DMA0 source pointer low  8 bit */
#define          sar0m        sar0_addr.byte.mid    /* DMA0 source pointer mid  8 bit */
#define          sar0h        sar0_addr.byte.high   /* DMA0 source pointer high 8 bit */
#define          sar1         sar1_addr.dword       /* DMA1 source pointer 32 bit */
#define          sar1l        sar1_addr.byte.low    /* DMA1 source pointer low  8 bit */
#define          sar1m        sar1_addr.byte.mid    /* DMA1 source pointer mid  8 bit */
#define          sar1h        sar1_addr.byte.high   /* DMA1 source pointer high 8 bit */
#define          dar0         dar0_addr.dword       /* DMA0 destination pointer 32 bit */
#define          dar0l        dar0_addr.byte.low    /* DMA0 destination pointer low  8 bit */
#define          dar0m        dar0_addr.byte.mid    /* DMA0 destination pointer mid  8 bit */
#define          dar0h        dar0_addr.byte.high   /* DMA0 destination pointer high 8 bit */
#define          dar1         dar1_addr.dword       /* DMA1 destination pointer 32 bit */
#define          dar1l        dar1_addr.byte.low    /* DMA1 destination pointer low  8 bit */
#define          dar1m        dar1_addr.byte.mid    /* DMA1 destination pointer mid  8 bit */
#define          dar1h        dar1_addr.byte.high   /* DMA1 destination pointer high 8 bit */

union{
    struct{
        char    b0:1;
        char    b1:1;
        char    b2:1;
        char    b3:1;
        char    b4:1;
        char    b5:1;
        char    b6:1;
        char    b7:1;
        char    b8:1;
        char    b9:1;
        char    b10:1;
        char    b11:1;
        char    b12:1;
        char    b13:1;
        char    b14:1;
        char    b15:1;
     }bit;
    struct{
        char    low;                /* low  8 bit */
        char    high;               /* high 8 bit */
    }byte;
    unsigned short      word;
}tcr0_addr,tcr1_addr,
 ta0_addr,ta1_addr,ta2_addr,ta3_addr,ta4_addr,tb0_addr,tb1_addr,tb2_addr,
 tb3_addr,tb4_addr,tb5_addr, u0tb_addr,u1tb_addr,u2tb_addr,crcd_addr,
 ad0_addr,ad1_addr,ad2_addr,ad3_addr,ad4_addr,ad5_addr,ad6_addr,ad7_addr;            /* (61) */

#define          tcr0     tcr0_addr.word              /* DMA0 transfer counter 16 bit */
#define          tcr0l    tcr0_addr.byte.low          /* DMA0 transfer counter low  8 bit */
#define          tcr0h    tcr0_addr.byte.high         /* DMA0 transfer counter high 8 bit */
#define          tcr1     tcr1_addr.word              /* DMA1 transfer counter 16 bit */
#define          tcr1l    tcr1_addr.byte.low          /* DMA1 transfer counter low  8 bit */
#define          tcr1h    tcr1_addr.byte.high         /* DMA1 transfer counter high 8 bit */
#define          ta0      ta0_addr.word               /* Timer A0 register 16 bit */
#define          ta0l     ta0_addr.byte.low           /* Timer A0 register low  8 bit */
#define          ta0h     ta0_addr.byte.high          /* Timer A0 register high 8 bit */
#define          ta1      ta1_addr.word               /* Timer A1 register 16 bit */
#define          ta1l     ta1_addr.byte.low           /* Timer A1 register low  8 bit */
#define          ta1h     ta1_addr.byte.high          /* Timer A1 register high 8 bit */
#define          ta2      ta2_addr.word               /* Timer A2 register 16 bit */
#define          ta2l     ta2_addr.byte.low           /* Timer A2 register low  8 bit */
#define          ta2h     ta2_addr.byte.high          /* Timer A2 register high 8 bit */
#define          ta3      ta3_addr.word               /* Timer A3 register 16 bit */
#define          ta3l     ta3_addr.byte.low           /* Timer A3 register low  8 bit */
#define          ta3h     ta3_addr.byte.high          /* Timer A3 register high 8 bit */
#define          ta4      ta4_addr.word               /* Timer A4 register 16 bit */
#define          ta4l     ta4_addr.byte.low           /* Timer A4 register low  8 bit */
#define          ta4h     ta4_addr.byte.high          /* Timer A4 register high 8 bit */
#define          ta11     ta11_addr.word              /* Timer A1-1 register 16 bit */
#define          ta11l    ta11_addr.byte.low          /* Timer A1-1 register low  8 bit */
#define          ta11h    ta11_addr.byte.high         /* Timer A1-1 register high 8 bit */
#define          ta21     ta21_addr.word              /* Timer A2-1 register 16 bit */
#define          ta21l    ta21_addr.byte.low          /* Timer A2-1 register low  8 bit */
#define          ta21h    ta21_addr.byte.high         /* Timer A2-1 register high 8 bit */
#define          ta41     ta41_addr.word              /* Timer A4-1 register 16 bit */
#define          ta41l    ta41_addr.byte.low          /* Timer A4-1 register low  8 bit */
#define          ta41h    ta41_addr.byte.high         /* Timer A4-1 register high 8 bit */
#define          tb0      tb0_addr.word               /* Timer B0 register 16 bit */
#define          tb0l     tb0_addr.byte.low           /* Timer B0 register low  8 bit */
#define          tb0h     tb0_addr.byte.high          /* Timer B0 register high 8 bit */
#define          tb1      tb1_addr.word               /* Timer B1 register 16 bit */
#define          tb1l     tb1_addr.byte.low           /* Timer B1 register low  8 bit */
#define          tb1h     tb1_addr.byte.high          /* Timer B1 register high 8 bit */
#define          tb2      tb2_addr.word               /* Timer B2 register 16 bit */
#define          tb2l     tb2_addr.byte.low           /* Timer B2 register low  8 bit */
#define          tb2h     tb2_addr.byte.high          /* Timer B2 register high 8 bit */
#define          tb3      tb3_addr.word               /* Timer B3 register 16 bit */
#define          tb3l     tb3_addr.byte.low           /* Timer B3 register low  8 bit */
#define          tb3h     tb3_addr.byte.high          /* Timer B3 register high 8 bit */
#define          tb4      tb4_addr.word               /* Timer B4 register 16 bit */
#define          tb4l     tb4_addr.byte.low           /* Timer B4 register low  8 bit */
#define          tb4h     tb4_addr.byte.high          /* Timer B4 register high 8 bit */
#define          tb5      tb5_addr.word               /* Timer B5 register 16 bit */
#define          tb5l     tb5_addr.byte.low           /* Timer B5 register low  8 bit */
#define          tb5h     tb5_addr.byte.high          /* Timer B5 register high 8 bit */
#define          u0tb     u0tb_addr.word              /* UART0 Transmit buffer register 16 bit */
#define          u0tbl    u0tb_addr.byte.low          /* UART0 Transmit buffer register low  8 bit */
#define          u0tbh    u0tb_addr.byte.high         /* UART0 Transmit buffer register high 8 bit */
#define          u1tb     u1tb_addr.word              /* UART1 Transmit buffer register 16 bit */
#define          u1tbl    u1tb_addr.byte.low          /* UART1 Transmit buffer register low  8 bit */
#define          u1tbh    u1tb_addr.byte.high         /* UART1 Transmit buffer register high 8 bit */
#define          u2tb     u2tb_addr.word              /* UART2 Transmit buffer register 16 bit */
#define          u2tbl    u2tb_addr.byte.low          /* UART2 Transmit buffer register low  8 bit */
#define          u2tbh    u2tb_addr.byte.high         /* UART2 Transmit buffer register high 8 bit */
#define          crcd     crcd_addr.word              /* CRC data register 16 bit */
#define          crcdl    crcd_addr.byte.low          /* CRC data register low  8 bit */
#define          crcdh    crcd_addr.byte.high         /* CRC data register high 8 bit */
#define          ad0      ad0_addr.word               /* A-D register0 16 bit */
#define          ad0l     ad0_addr.byte.low           /* A-D register0 low  8 bit */
#define          ad0h     ad0_addr.byte.high          /* A-D register0 high 8 bit */
#define          ad1      ad1_addr.word               /* A-D register1 16 bit */
#define          ad1l     ad1_addr.byte.low           /* A-D register1 low  8 bit */
#define          ad1h     ad1_addr.byte.high          /* A-D register1 high 8 bit */
#define          ad2      ad2_addr.word               /* A-D register2 16 bit */
#define          ad2l     ad2_addr.byte.low           /* A-D register2 low  8 bit */
#define          ad2h     ad2_addr.byte.high          /* A-D register2 high 8 bit */
#define          ad3      ad3_addr.word               /* A-D register3 16 bit */
#define          ad3l     ad3_addr.byte.low           /* A-D register3 low  8 bit */
#define          ad3h     ad3_addr.byte.high          /* A-D register3 high 8 bit */
#define          ad4      ad4_addr.word               /* A-D register4 16 bit */
#define          ad4l     ad4_addr.byte.low           /* A-D register4 low  8 bit */
#define          ad4h     ad4_addr.byte.high          /* A-D register4 high 8 bit */
#define          ad5      ad5_addr.word               /* A-D register5 16 bit */
#define          ad5l     ad5_addr.byte.low           /* A-D register5 low  8 bit */
#define          ad5h     ad5_addr.byte.high          /* A-D register5 high 8 bit */
#define          ad6      ad6_addr.word               /* A-D register6 16 bit */
#define          ad6l     ad6_addr.byte.low           /* A-D register6 low  8 bit */
#define          ad6h     ad6_addr.byte.high          /* A-D register6 high 8 bit */
#define          ad7      ad7_addr.word               /* A-D register7 16 bit */
#define          ad7l     ad7_addr.byte.low           /* A-D register7 low  8 bit */
#define          ad7h     ad7_addr.byte.high          /* A-D register7 high 8 bit */

