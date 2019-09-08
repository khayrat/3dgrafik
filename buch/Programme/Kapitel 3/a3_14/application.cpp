#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "polygon.h"
#include "matrix.h"

MSG msg;
uchar key_pressed( void );

vertex points[ 3 ] =
{
  vertex(   0,  40,  0 ),
  vertex(  35, -20,  0 ),
  vertex( -35, -20,  0 )
};

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polygon yellow( 3, pixel_32( 255, 255, 0 ), points );
  polygon blue( 3, pixel_32( 0, 0, 255 ), points );

  matrix m;

  m.translate( -100, 50, 150 );
  yellow.update_pos( m );
  m.clear();

  m.translate( 100, -50, 150 );
  blue.update_pos( m );
  m.clear();

  m.scale( 3, 3, 3 );
  blue.update_pos( m );

  while( key_pressed() == 0 )
  {
    yellow.display( screen );

    blue.display( screen );
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

