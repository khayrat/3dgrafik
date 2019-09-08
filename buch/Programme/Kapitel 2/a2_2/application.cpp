#include <windows.h>

#include "screen_interface.h"
#include "palette.h"

MSG msg;
uchar key_pressed( void );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  // Beim Einstellen einer höheren Auflösung müssen auch die Konstanten inneralb der beiden
  // for()-Schleifen entsprechend angepasst werden

  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  long start_x = (640 - 256) / 2;
  long start_y = (480 - 200) / 2;

  while( 1 )
  {
    if( key_pressed() ) break;

    for( long x=0 ; x<256 ; x++ )
       for( long y=0 ; y<200 ; y++ )
          if( x >= 0 && x < 639 && y >= 0 && y < 479 )
            screen[ (y + start_y) * x_res + (x + start_x) ] = palette[ x ];
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
