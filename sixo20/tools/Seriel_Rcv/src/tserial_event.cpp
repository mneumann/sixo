


/* ------------------------------------------------------------------------ --
--                                                                          --
--                        PC serial port connection object                  --
--                           for event-driven programs                      --
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
--  Modified : April 24th 2001                                              --
--  Plateform: Windows 95, 98, NT, 2000 (Win32)                             --
-- ------------------------------------------------------------------------ --
--                                                                          --
--  This software is given without any warranty. It can be distributed      --
--  free of charge as long as this header remains, unchanged.               --
--                                                                          --
-- ------------------------------------------------------------------------ --
--                                                                          --
-- 01.04.24      Comments added                                             --
-- 01.04.28      Bug 010427 corrected. OnDisconnectedManager was not        --
--                initialized                                               --
-- 01.04.28      connect() function prototype modified to handle 7-bit      --
--                communication                                             --
-- 01.04.29      "ready" field added to remove a bug that occured during    --
--                 reconnect (event manager pointers cleared)               --
--                 I removed the "delete" in Tserial_event_thread_start     --
--                 because it was destroying the object even if we would    --
--                 use it again                                             --
--                                                                          --
--                                                                          --
-- ------------------------------------------------------------------------ --
--                                                                          --
--    Note to Visual C++ users:  Don't forget to compile with the           --
--     "Multithreaded" option in your project settings                      --
--                                                                          --
--         See   Project settings                                           --
--                   |                                                      --
--                   *--- C/C++                                             --
--                          |                                               --
--                          *--- Code generation                            --
--                                       |                                  --
--                                       *---- Use run-time library         --
--                                                     |                    --
--                                                     *---- Multithreaded  --
--                                                                          --
--                                                                          --
--                                                                          --
-- ------------------------------------------------------------------------ */




/* ---------------------------------------------------------------------- */

#define STRICT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <conio.h>
#include <windows.h>


#include "Tserial_event.h"

#define SIG_POWER_ON    0    // signals definition for external threads
#define SIG_POWER_DOWN  1
#define SIG_COM_READER  3
#define SIG_COM_WRITER  4
#define SIG_DATA_IN     5    // for byte by byte operation

void Tserial_event_thread_start(void *arg);

typedef unsigned (WINAPI *PBEGINTHREADEX_THREADFUNC) (LPVOID lpThreadParameter);
typedef unsigned *PBEGINTHREADEX_THREADID;

/* ---------------------------------------------------------------------- */
/* ---------------------  Tserial_event_thread_start  ------------------- */
/* ---------------------------------------------------------------------- */
/**
    This function is not part of the Tserial_event object. It is simply used
    to start the thread from an external point of the object.
*/
void Tserial_event_thread_start(void *arg)
{
    class Tserial_event *serial_unit;

    serial_unit = (Tserial_event *) arg;
    
    if (serial_unit!=0)
        serial_unit->Run();
}

/* -------------------------------------------------------------------- */
/* -------------------------    Tserial_event ------------------------- */
/* -------------------------------------------------------------------- */
Tserial_event::Tserial_event()
{
    int i;

    ready            = false;
    parityMode       = spNONE;
    port[0]          = 0;
    rate             = 0;
    threadid         = 0;
    serial_handle    = INVALID_HANDLE_VALUE;
    tx_in_progress   = 0;
    rx_in_progress   = 0;

    OnConnectedManager   = 0;
    OnCharSentManager    = 0;
    OnCharArrivalManager = 0;
    OnDisconnectedManager= 0;   // bug 010427 correction

    /* -------------------------------------------------------------- */
    // creating Events for the different sources
    for (i=0; i<TSERIAL_SIGNAL_NBR; i++)
    {
        if ((i==SIG_COM_READER) || (i==SIG_COM_WRITER) )
            serial_events[i] = CreateEvent(NULL, TRUE, FALSE, NULL);  // Manual Reset
        else
            serial_events[i] = CreateEvent(NULL, FALSE, FALSE, NULL); // Auto reset
    }
}

/* -------------------------------------------------------------------- */
/* --------------------------    ~Tserial_event ----------------------- */
/* -------------------------------------------------------------------- */
Tserial_event::~Tserial_event()
{
    int i;

    /* -------------------------------------------------------- */
    for (i=0; i<TSERIAL_SIGNAL_NBR; i++)         // deleting the events
    {
        if (serial_events[i]!=INVALID_HANDLE_VALUE)
            CloseHandle(serial_events[i]);
        serial_events[i] = INVALID_HANDLE_VALUE;
    }

    if (serial_handle!=INVALID_HANDLE_VALUE)
        CloseHandle(serial_handle);
    serial_handle = INVALID_HANDLE_VALUE;
}

/* ---------------------------------------------------------------------- */
/* ------------------------      Start_thread       --------------------- */
/* ---------------------------------------------------------------------- */
void Tserial_event::Start_thread(void)
{
    SetSignal(SIG_POWER_ON);   // sending a power on signal to myself
    _beginthreadex(NULL,0,(PBEGINTHREADEX_THREADFUNC) Tserial_event_thread_start,
                  (void *) this, 0, &threadid);
}

/* -------------------------------------------------------------------- */
/* --------------------------    SetSignal    ------------------------- */
/* -------------------------------------------------------------------- */
void Tserial_event::SetSignal(long mask)
{
    if ((mask<TSERIAL_SIGNAL_NBR) && (mask>=0))
    {
        if (serial_events[mask]!=INVALID_HANDLE_VALUE)
            SetEvent(serial_events[mask]);
    }
}
/* -------------------------------------------------------------------- */
/* --------------------------    disconnect   ------------------------- */
/* -------------------------------------------------------------------- */
void Tserial_event::disconnect(void)
{
    ready = false;
    SetSignal(SIG_POWER_DOWN);
}
/* -------------------------------------------------------------------- */
/* --------------------------    powerDown    ------------------------- */
/* -------------------------------------------------------------------- */
void Tserial_event::powerDown(void)
{
    ready = false;
    if (serial_handle!=INVALID_HANDLE_VALUE)
        CloseHandle(serial_handle);
    serial_handle = INVALID_HANDLE_VALUE;

    OnDisconnected();
}
/* -------------------------------------------------------------------- */
/* --------------------------    connect      ------------------------- */
/* -------------------------------------------------------------------- */
/**
     Serial port, file and overlapped structures initialization
*/
int  Tserial_event::connect (char *port_arg, int rate_arg,
                                serial_parity parity_arg, char ByteSize)
{
    int erreur;
    DCB  dcb;
    COMMTIMEOUTS cto = { 0, 0, 0, 0, 0 };

    /* --------------------------------------------- */
    if (serial_handle!=INVALID_HANDLE_VALUE)
        CloseHandle(serial_handle);
    serial_handle = INVALID_HANDLE_VALUE;

    if (port_arg!=0)
    {
        strncpy(port, port_arg, 10);
        rate      = rate_arg;
        parityMode= parity_arg;

        erreur    = 0;
        ZeroMemory(&ovReader,sizeof(ovReader));      // clearing the overlapped
        ZeroMemory(&ovWriter,sizeof(ovWriter));
        memset(&dcb,0,sizeof(dcb));

        /* -------------------------------------------------------------------- */
        // set DCB to configure the serial port
        dcb.DCBlength       = sizeof(dcb);                   
        
        /* ---------- Serial Port Config ------- */
        dcb.BaudRate        = rate;

        switch(parityMode)
        {
            case spNONE:
                            dcb.Parity      = NOPARITY;
                            dcb.fParity     = 0;
                            break;
            case spEVEN:
                            dcb.Parity      = EVENPARITY;
                            dcb.fParity     = 1;
                            break;
            case spODD:
                            dcb.Parity      = ODDPARITY;
                            dcb.fParity     = 1;
                            break;
        }


        dcb.StopBits        = ONESTOPBIT;
        dcb.ByteSize        = (BYTE) ByteSize;
        
        dcb.fOutxCtsFlow    = 0;
        dcb.fOutxDsrFlow    = 0;
        dcb.fDtrControl     = DTR_CONTROL_DISABLE;
        dcb.fDsrSensitivity = 0;
        dcb.fRtsControl     = RTS_CONTROL_DISABLE;
        dcb.fOutX           = 0;
        dcb.fInX            = 0;
        
        /* ----------------- misc parameters ----- */
        dcb.fErrorChar      = 0;
        dcb.fBinary         = 1;
        dcb.fNull           = 0;
        dcb.fAbortOnError   = 0;
        dcb.wReserved       = 0;
        dcb.XonLim          = 2;
        dcb.XoffLim         = 4;
        dcb.XonChar         = 0x13;
        dcb.XoffChar        = 0x19;
        dcb.EvtChar         = 0;
        
        /* -------------------------------------------------------------------- */
        serial_handle    = CreateFile(port, GENERIC_READ | GENERIC_WRITE,
                   0, NULL, OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);
                   // opening serial port

        ovReader.hEvent = serial_events[SIG_COM_READER];
        ovWriter.hEvent = serial_events[SIG_COM_WRITER];

        if (serial_handle    != INVALID_HANDLE_VALUE)
        {
            if(!SetCommMask(serial_handle, 0))
                erreur = 1;

            // set timeouts
            if(!SetCommTimeouts(serial_handle,&cto))
                erreur = 2;

            // set DCB
            if(!SetCommState(serial_handle,&dcb))
                erreur = 4;
        }
        else
            erreur = 8;
    }
    else
        erreur = 16;


    /* --------------------------------------------- */
    if (erreur!=0)
    {
        CloseHandle(serial_handle);
        serial_handle = INVALID_HANDLE_VALUE;
    }
    else
    {
        Start_thread();
    }

    /* --------------------------------------------- */
    return(erreur);
}
/* -------------------------------------------------------------------- */
/* --------------------------    OnCharArrival ------------------------ */
/* -------------------------------------------------------------------- */
void          Tserial_event::OnCharArrival    (char c)
{
    if ((OnCharArrivalManager!=0) && ready)
        OnCharArrivalManager(c);
}
/* -------------------------------------------------------------------- */
/* --------------------------  OnConnected    ------------------------- */
/* -------------------------------------------------------------------- */
void          Tserial_event::OnConnected      (void)
{
    if ((OnConnectedManager!=0) && ready)
        OnConnectedManager();
}
/* -------------------------------------------------------------------- */
/* --------------------------  OnDisconnected ------------------------- */
/* -------------------------------------------------------------------- */
void          Tserial_event::OnDisconnected      (void)
{
    if (OnDisconnectedManager!=0)
        OnDisconnectedManager();
}
/* -------------------------------------------------------------------- */
/* --------------------------   OnCharSent    ------------------------- */
/* -------------------------------------------------------------------- */
void          Tserial_event::OnCharSent       (void)
{
    if ((OnCharSentManager!=0) && ready)
        OnCharSentManager();
}
/* -------------------------------------------------------------------- */
/* ---------------------  setManagerOnCharArrival --------------------- */
/* -------------------------------------------------------------------- */
/*
    Transmitting a null parameter will clear the variable in any case
*/
void         Tserial_event::setManagerOnCharArrival(type_charEvent manager)
{
    if ((OnCharArrivalManager==0) || (manager==0))
        OnCharArrivalManager = manager;
}
/* -------------------------------------------------------------------- */
/* ----------------------  setManagerOnConnected  --------------------- */
/* -------------------------------------------------------------------- */
void         Tserial_event::setManagerOnConnected  (type_voidEvent manager)
{
    if ((OnConnectedManager==0) || (manager==0))
        OnConnectedManager = manager;
}
/* -------------------------------------------------------------------- */
/* -----------------------  setManagerOnDisconnected  ----------------- */
/* -------------------------------------------------------------------- */
void         Tserial_event::setManagerOnDisconnected  (type_voidEvent manager)
{
    if ((OnDisconnectedManager==0) || (manager==0))
        OnDisconnectedManager = manager;
}
/* -------------------------------------------------------------------- */
/* --------------------   setManagerOnCharSent   ---------------------- */
/* -------------------------------------------------------------------- */
void         Tserial_event::setManagerOnCharSent   (type_voidEvent manager)
{
    if ((OnCharSentManager==0) || (manager==0))
        OnCharSentManager = manager;
}
/* -------------------------------------------------------------------- */
/* -----------------------   getNbrOfBytes  --------------------------- */
/* -------------------------------------------------------------------- */
int Tserial_event::getNbrOfBytes    (void)
{
    struct _COMSTAT status;
    int             n;
    unsigned long   etat;

    n = 0;

    if (serial_handle!=INVALID_HANDLE_VALUE)
    {
        ClearCommError(serial_handle, &etat, &status);
        n = status.cbInQue;
    }
    return(n);
}
/* -------------------------------------------------------------------- */
/* --------------------------    sendChar     ------------------------- */
/* -------------------------------------------------------------------- */
void Tserial_event::sendChar (char data)
{
    data_to_send = data;
    SetSignal(SIG_DATA_IN);
}
/* -------------------------------------------------------------------- */
/* --------------------------       Run       ------------------------- */
/* -------------------------------------------------------------------- */

/** 
 this function is the main loop of the Tserial object. There is a
 do while() loop executed until either an error or a PowerDown is 
 received.
 this is not a busy wait since we use the WaitForMultipleObject function
*/

void Tserial_event::Run(void)
{
    int           done;
    long          status;
    unsigned long read_nbr, result_nbr;
    char          data;
    char          success;

    ready             = true;
    done              = 0;
    tx_in_progress    = 0;
    rx_in_progress    = 0;
    OnConnected();
    GetLastError();     // just to clear any pending error

    /* ----------------------------------------------------------- */
    while(!done)
    {
        /* ---------------------------------------------------- */
        /*            checking for incoming                     */
        /* ---------------------------------------------------- */
        if (!rx_in_progress)      // if no reception is in progress
        {                         // I start a new one
            do
            {
                success = (char) ReadFile(serial_handle,&data,1,&read_nbr,&ovReader);
                // reading one byte only to have immediate answer on each byte
                if (success)
                {
                    // immediate return => data processing 
                    ResetEvent(serial_events[SIG_COM_READER]);
                    OnCharArrival(data);
                    rx_in_progress = 0;
                }
                else
                {
                    if(GetLastError() != ERROR_IO_PENDING )
                        done=1;
                    else
                        rx_in_progress=1; // read is pending
                                          // will wait for completion in
                                          // WaitForMultipleObjects
                }
            }
            while(success); 
            // as long as the read is returning data, I'll read them
        }

        /* ---------------------------------------------------- */
        /*            Waiting  for signals                      */
        /* ---------------------------------------------------- */
        if (!done)
        {
            // Main wait function. Waiting for something to happen. 
            // This may be either the completion of a Read or a Write or
            // the reception of Power On, Power Down, new Tx
            //
            status = WaitForMultipleObjects(TSERIAL_SIGNAL_NBR, serial_events,
                                            FALSE, INFINITE);

            /* processing answer to filter other failures */
            status = status - WAIT_OBJECT_0;
            if ((status<0) || (status>=TSERIAL_SIGNAL_NBR))
                done=1;
            else
            {
                /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
                /* ++++++++++++++++++++ EVENT DISPATCHER ++++++++++++++++++ */
                /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
                switch(status)
                {
                    /* ######################################################## */
                    case SIG_POWER_ON :
                        /* * /
                        printf("Tserial : power on\n");
                        /* */
                        break;
                    /* ######################################################## */
                    case SIG_POWER_DOWN:
                        /* * /
                        printf("Tserial : power down\n");
                        /* */
                        done = 1;
                        break;
                    /* ######################################################## */
                    case SIG_DATA_IN:
                        // Signal asserted that there is a new valid data
                        // in the "data_to_send" variable
                        success = (char) WriteFile(serial_handle, &data_to_send, 1,
                                            &result_nbr, &ovWriter);
                        // sending data on the port
                        if (success)
                        {
                            // as long as the write is returning immediately,
                            // I'll be ready to send additonnal data
                            tx_in_progress = 0;
                            OnCharSent();
                            // calling the frame to indicate that the transmission
                            // is over and we're ready to receive a new data
                        }
                        else
                        {
                            if(GetLastError() == ERROR_IO_PENDING )
                                tx_in_progress=1;       // write is pending
                            else
                                done = 1;               // failure !
                        }

                        break;
                    /* ######################################################## */
                    case SIG_COM_READER:
                        // reading the result of the terminated read
                        if (GetOverlappedResult(serial_handle, &ovReader,
                            &result_nbr, FALSE))
                        {
                            // no error => OK
                            // Read operation completed successfully
                            ResetEvent(serial_events[SIG_COM_READER]);
                            // Write operation completed successfully
                            if (result_nbr)
                                OnCharArrival(data);
                            // if incoming data, I process them
                            rx_in_progress = 0; // read has ended
                        }
                        else
                        {
                            // GetOverlapped didn't succeed !
                            // What's the reason ?
                            if(GetLastError() == ERROR_IO_PENDING )
                                // still pending ?? ok no proble, I'll
                                // wait for completion
                                rx_in_progress = 1; // read not ended
                            else
                                // other reason !? this is a failure
                                // exiting
                                done = 1;  // failure
                        }
                        break;
                    /* ######################################################## */
                    case SIG_COM_WRITER:
                        // WriteFile has terminated
                        // checkin the result of the operation
                        if (GetOverlappedResult(serial_handle, &ovWriter,
                            &result_nbr, FALSE))
                        {
                            // Write operation completed successfully
                            ResetEvent(serial_events[SIG_COM_WRITER]);
                            // Write operation completed successfully
                            tx_in_progress = 0; // write has ended
                            OnCharSent();       // checking if there are
                                                // other messages waiting
                        }
                        else
                        {
                            // GetOverlapped didn't succeed !
                            // What's the reason ?
                            if(GetLastError() == ERROR_IO_PENDING )
                                // still pending ?? ok no proble, I'll
                                // wait for completion
                                tx_in_progress = 1; // write not ended
                            else
                                // other reason !? this is a failure
                                // exiting
                                done = 1;  // failure
                        }
                        break;
                    /* ######################################################## */
                }
                /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
                /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
                /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
            }
        }
    };

    powerDown();
}
/* -------------------------------------------------------------------- */
/* -------------------------------------------------------------------- */

