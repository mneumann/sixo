/* ------------------------------------------------------------------------ --
--                                                                          --
--             receive bytes from a serial interface                        --
--                                                                          --
-- ------------------------------------------------------------------------ --
--  filename : serial_rcv.cpp                                               --
--  author   : Arnold Neugebauer                                            --
--  created  : 15.02.2003                                                   --
--  platform:  Windows 95, 98, NT, 2000 (Win32)                             --
-- ------------------------------------------------------------------------ */

#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include "Tserial_event.h"

typedef enum{
   OF_VERBOSE,
   OF_BINARY
}OUTPUT_FORMAT;

OUTPUT_FORMAT output_format;


/* ======================================================== */
/* ===============  OnCharArrival     ===================== */
/* ======================================================== */

void OnCharArrival( char c )
{
   if( output_format == OF_VERBOSE ){
      printf( " char arrived: 0x%02x\n", (unsigned char) c );
	}
   if( output_format == OF_BINARY ){
      printf( "%c", c ); //stdout has been set to binary in main
	}
}


/* ======================================================== */
/* =================== OnConnected ======================== */
/* ======================================================== */

void OnConnected( void )
{
   if( output_format == OF_VERBOSE ) printf( " connected\n" );
}


/* ======================================================== */
/* ==================== OnDisconnected ==================== */
/* ======================================================== */

void OnDisconnected( void )
{
   if( output_format == OF_VERBOSE ) printf( " disconnected\n" );
}


/* ======================================================== */
/* ===================== OnCharSent ======================= */
/* ======================================================== */

void OnCharSent( void )
{
   if( output_format == OF_VERBOSE ) printf( " char sent\n" );
}


/* ======================================================== */
/* ======================== Main ========================== */
/* ======================================================== */

int main( int argc, char* argv[] )
{
	bool error_flag = TRUE;
   Tserial_event *com;
   int c;

	if( argc == 2 ){
      if( strcmp( argv[1], "verbose" ) == 0 ){
         output_format = OF_VERBOSE;
         error_flag = FALSE;
      }
		if( strcmp( argv[1], "binary" ) == 0 ){
         output_format = OF_BINARY;
         error_flag = FALSE;
      }
   }

   if( error_flag == TRUE ){
      puts( "" );
      puts( " receives bytes from a serial interface" );
      puts( "" );
      puts( " usage: serial_rcv <output format>" );
      puts( "        output format: verbose|binary" );
      puts( "" );
      puts( " press ESC to abort" );
      puts( "" );
      return( 0 );
   }

   //set stdout to binary mode
   _setmode( _fileno( stdout ), _O_BINARY );

   com = new Tserial_event();

   if( com!=0 ){
      // indicating to the serial object which function to
      // call in case of events
      com->setManagerOnCharArrival  ( OnCharArrival );
      com->setManagerOnConnected    ( OnConnected );
      com->setManagerOnDisconnected ( OnDisconnected );
      com->setManagerOnCharSent     ( OnCharSent );

      com->connect("COM3", 9600, spNONE, 8);

		if( output_format == OF_VERBOSE ){
         printf( " ------------------------------------------------\n" );
		   printf( " press ESC to abort\n\n" );
      }

      do{
         c = getch();
      }while( c!=27 ); //ESC

      com->disconnect();
      delete com;
      com = 0;
	}

   return 0;
}
