#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "polygon.h"
#include "matrix.h"

MSG msg;
uchar key_pressed( void );

vertex points[ 8 ] =
{
  vertex(  12,  30,  0 ),
  vertex(  30,  12,  0 ),
  vertex(  30, -12,  0 ),
  vertex(  12, -30,  0 ),
  vertex( -12, -30,  0 ),
  vertex( -30, -12,  0 ),
  vertex( -30,  12,  0 ),
  vertex( -12,  30,  0 )
};

inline void inc( long *y )
{
  if( *y == 3 ) *y = 0;
  else (*y)++;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  long pixel_count = x_res * y_res;

  pixel_32 *sbuffer;
  if( (sbuffer = new pixel_32[ pixel_count ]) == NULL )
    exit_error( "*sbuffer: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  polygon octagon( 8, pixel_32( 255, 128, 0 ), points );

  matrix m[ 4 ];

  m[ 0 ].translate( 0, 0, 90 );
  octagon.update_pos( m[ 0 ] );
  m[ 0 ].clear();

  m[ 0 ].scale( 1.01, 1, 1 );
  m[ 1 ].scale( 1, 1.01, 1 );
  m[ 2 ].scale( 0.99, 1, 1 );
  m[ 3 ].scale( 1, 0.99, 1 );

  long matrix_index = 0;
  long frame_counter = 0;

  while( key_pressed() == 0 )
  {
    if( ++frame_counter == 80 ) {  frame_counter = 0;  inc( &matrix_index );  }

    octagon.update_pos( m[ matrix_index ] );

    long x;
    for( x=0 ; x<pixel_count ; x++ ) sbuffer[ x ] = pixel_32( 0, 0, 0 );

    octagon.display( sbuffer );

    for( x=0 ; x<pixel_count ; x++ ) screen[ x ] = sbuffer[ x ];
  }
  
  if( sbuffer ) delete [] sbuffer;

  screen_interface.release_screen_pointer();

  return msg.wParam;
}

uchar key_pressed( void )
{
  if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
  {
    if( msg.message == WM_QUIT || msg.message == WM_KEYDOWN ) return 1;

    TranslateMessage( &msg );
    DispatchMessage( &msg );
  }

  return 0;
}

