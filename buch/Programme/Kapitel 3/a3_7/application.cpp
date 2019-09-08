#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "polygon.h"
#include "matrix.h"

MSG msg;
uchar key_pressed( void );

vertex points[ 6 ] =
{
  vertex( -32,  0,   0 ),
  vertex( -16,  28,  0 ),
  vertex(  16,  28,  0 ),
  vertex(  32,  0,   0 ),
  vertex(  16, -28,  0 ),
  vertex( -16, -28,  0 )
};

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polygon hexagon( 6, pixel_32( 0, 0, 200 ), points );

  matrix m;

  m.translate( 90, 90, 180 );
  hexagon.update_pos( m );
  m.clear();

  m.rotate_z( 0.01 );

  long pixel_count = x_res * y_res;

  pixel_32 *sbuffer;
  if( (sbuffer = new pixel_32[ pixel_count ]) == NULL )
    exit_error( "*sbuffer: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  while( key_pressed() == 0 )
  {
    hexagon.update_pos( m );

    long x;
    for( x=0 ; x<pixel_count ; x++ ) sbuffer[ x ] = pixel_32( 0, 0, 0 );

    hexagon.display( sbuffer );

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

