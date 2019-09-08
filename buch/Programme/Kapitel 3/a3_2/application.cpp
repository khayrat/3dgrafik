#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "polygon.h"

MSG msg;
uchar key_pressed( void );

vertex points_near[ 8 ] =
{
  vertex(  12,  30,  40 ),
  vertex(  30,  12,  40 ),
  vertex(  30, -12,  40 ),
  vertex(  12, -30,  40 ),
  vertex( -12, -30,  40 ),
  vertex( -30, -12,  40 ),
  vertex( -30,  12,  40 ),
  vertex( -12,  30,  40 )
};

vertex points_far[ 8 ] =
{
  vertex(  12,  30,  90 ),
  vertex(  30,  12,  90 ),
  vertex(  30, -12,  90 ),
  vertex(  12, -30,  90 ),
  vertex( -12, -30,  90 ),
  vertex( -30, -12,  90 ),
  vertex( -30,  12,  90 ),
  vertex( -12,  30,  90 )
};

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polygon green( 8, pixel_32( 126, 254, 2 ), points_near );
  polygon red( 8, pixel_32( 140, 0, 0 ), points_far );

  while( 1 )
  {
    if( key_pressed() ) break;

    green.display( screen );
    red.display( screen );
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

