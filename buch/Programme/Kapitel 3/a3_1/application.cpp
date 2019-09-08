#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "polygon.h"

MSG msg;
uchar key_pressed( void );

svertex points[ 8 ] =
{
  svertex( 344, 300 ),
  svertex( 380, 264 ),
  svertex( 380, 216 ),
  svertex( 344, 180 ),
  svertex( 296, 180 ),
  svertex( 260, 216 ),
  svertex( 260, 264 ),
  svertex( 296, 300 )
};

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polygon eight( 8, pixel_32( 0, 0, 255 ), points );

  while( 1 )
  {
    if( key_pressed() ) break;

    eight.display( screen );
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

