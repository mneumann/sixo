//===========================================================================
//IIC bit banging routines
//------------------------
//
//module:         iicbb.h
//description:    simple IIC routines which do *not* use UART2
//author:         Arnold Neugebauer
//version:        1.0
//tools:          NC30, AS30, LN30
//mcu:            Mitsubishi M30624FGLFP @ 10MHz
//notes:          No multi-master, no collision detection, no wait states (no
//                IIC bus clock extending allowed).
//                Make sure, that the routines are called with the correct
//                paremeter format (use ASSERTs for example). Parameters are
//                not checked in the low level routines.
//                The NC30 seems to have a problem with structure and union
//                types in #pragma parameter declarations. If you experience
//                problems in calling iic_rcv or iic_snd try to
//                use standard types.
//history:        first build 28.02.01
//                first release 03.02.01
//                introduced different buffers for address and data 04.03.01
//                SCL/SDA can now be any general purpose i/o port 06.03.01
//===========================================================================
#ifndef _IICBB_H   
#define _IICBB_H     

//functions available for C
//return value is 1 = OK, acknowledge received
//                0 = failed, no acknowledge received
//see header of iicbb.a30 for details


extern unsigned char iic_rcv( unsigned char *adr_buffer,
                              unsigned char number_of_adr,
                              unsigned char *dat_buffer,
                              unsigned char number_of_dat );

extern unsigned char iic_snd( unsigned char *adr_buffer,
                              unsigned char number_of_adr,
                              unsigned char *dat_buffer,
                              unsigned char number_of_dat );


#pragma PARAMETER iic_rcv( A1, R1H, A0, R0H )
#pragma PARAMETER iic_snd( A1, R1H, A0, R0H )

#endif /* _IICBB_H   */
