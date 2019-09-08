#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "polygon.h"
#include "matrix.h"

MSG msg;
uchar key_pressed( void );

vertex points[ 4 ] =
{
  vertex( -30,  30, 0 ),
  vertex(  30,  30, 0 ),
  vertex(  30, -30, 0 ),
  vertex( -30, -30, 0 )
};

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polygon square( 4, pixel_32( 255, 255, 255 ), points );

  matrix m;
  m.translate( 0, 0, 90 );
  square.update_pos( m );
  m.clear();

  m.rotate_z( 0.00001 );

  while( 1 )
  {
    if( key_pressed() ) break;

    square.update_pos( m );
    square.display( screen );
  }
  
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

