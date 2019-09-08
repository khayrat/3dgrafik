#include <windows.h>

#include "screen_interface.h"
#include "palette.h"

struct svertex
{
  long sx, sy;
  
  svertex( long x, long y ) : sx( x ), sy( y ) { }
};

MSG msg;
uchar key_pressed( void );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  svertex points[ 3 ] =
  {
    svertex(  95, 435 ),
    svertex( 320,  45 ),
    svertex( 545, 435 )
  };

  svertex act_point = points[ rand() % 3 ];

  while( 1 )
  {
    if( key_pressed() ) break;

    uchar index = rand() % 3;

    act_point.sx = (act_point.sx + points[ index ].sx) / 2;
    act_point.sy = (act_point.sy + points[ index ].sy) / 2;

    screen[ act_point.sy * x_res + act_point.sx ] = palette[ 15 ];

    for( double wait=0 ; wait<999999 ; wait++ ) ;
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
