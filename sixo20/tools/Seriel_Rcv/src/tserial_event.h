


/* ------------------------------------------------------------------------ --
--                                                                          --
--                        PC serial port connection object                  --
--                           for  event-driven programs                     --
--                                                                          --
--                                                                          --
--                                                                          --
--  Copyright @ 2001          Thierry Schneider                             --
--                            thierry@tetraedre.com                         --
--                                                                          --
--                                                                          --
--                                                                          --
-- ------------------------------------------------------------------------ --
--                                                                          --
--  Filename : sertest2.cpp                                                 --
--  Author   : Thierry Schneider                                            --
--  Created  : April 4th 2000                                               --
--  Modified : April 8th 2001                                               --
--  Plateform: Windows 95, 98, NT, 2000 (Win32)                             --
-- ------------------------------------------------------------------------ --
--                                                                          --
--  This software is given without any warranty. It can be distributed      --
--  free of charge as long as this header remains, unchanged.               --
--                                                                          --
-- ------------------------------------------------------------------------ --
--                                                                          --
-- 01.04.28      connect() function prototype modified to handle 7-bit      --
--                communication                                             --
-- 01.04.29      "ready" field added to remove a bug that occured during    --
--                 reconnect (event manager pointers cleared)               --
-- ------------------------------------------------------------------------ */


#ifndef TSERIAL_EVENT_H
#define TSERIAL_EVENT_H

#include <stdio.h>
#include <windows.h>


#define TSERIAL_SIGNAL_NBR 6               // number of events in the thread


enum serial_parity  { spNONE,    spODD,        spEVEN };

typedef void (*type_voidEvent) (void);
typedef void (*type_charEvent) (char);

#ifndef __BORLANDC__
#define bool  BOOL 
#define true  TRUE
#define false FALSE
#endif

/* -------------------------------------------------------------------- */
/* -----------------------------  Tserial  ---------------------------- */
/* -------------------------------------------------------------------- */
class Tserial_event
{
    // -------------------------------------------------------- //
protected:
    bool              ready;                       
    char              port[10];                      // port name "com1",...
    int               rate;                          // baudrate
    serial_parity     parityMode;

    HANDLE        serial_events[TSERIAL_SIGNAL_NBR]; // events to wait on
    unsigned int  threadid;                          // ...
    HANDLE        serial_handle;                     // ...
    OVERLAPPED    ovReader;                          // Overlapped structure for ReadFile
    OVERLAPPED    ovWriter;                          // Overlapped structure for WriteFile
    char          tx_in_progress;                    // BOOL indicating if a WriteFile is
                                                     // in progress
    char          rx_in_progress;                    // BOOL indicating if a ReadFile is
                                                     // in progress
    char          data_to_send;

    type_voidEvent OnConnectedManager;
    type_voidEvent OnDisconnectedManager;
    type_voidEvent OnCharSentManager;
    type_charEvent OnCharArrivalManager;

    // ............................................................
    void          SetSignal        (long mask);
    void          powerDown        (void);
    void          Start_thread     (void);

    void          OnCharArrival    (char c);
    void          OnConnected      (void);
    void          OnDisconnected   (void);
    void          OnCharSent       (void);


    // ++++++++++++++++++++++++++++++++++++++++++++++
    // .................. EXTERNAL VIEW .............
    // ++++++++++++++++++++++++++++++++++++++++++++++
public:
    void          Run          (void);
                  Tserial_event();
                 ~Tserial_event();
    int           connect          (char *port_arg, int rate_arg,
                                    serial_parity parity_arg, char ByteSize);

    void          setManagerOnCharArrival  (type_charEvent manager);
    void          setManagerOnConnected    (type_voidEvent manager);
    void          setManagerOnDisconnected (type_voidEvent manager);
    void          setManagerOnCharSent     (type_voidEvent manager);

    void          sendChar         (char c);
    int           getNbrOfBytes    (void);
    void          disconnect       (void);
};
/* -------------------------------------------------------------------- */

#endif TSERIAL_EVENT_H


