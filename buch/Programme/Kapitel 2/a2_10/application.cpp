#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"

MSG msg;
uchar key_pressed( void );
void draw_circle( svertex midpoint, long r, pixel_32 c, pixel_32 *screen );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  while( 1 )
  {
    if( key_pressed() ) break;
    
    long r = rand() % 100 + 5;
    long sx, sy;

    do
    {
      sx = rand() % (x_res - 2*r);
      sy = rand() % (y_res - 2*r);
    }
    while( (sx-r) < 0 || (sx+r) >= x_res || (sy-r) < 0 || (sy+r) >= y_res );

    uchar c = rand() % 155 + 100;

    draw_circle( svertex( sx, sy ), r, pixel_32( 255, 255, 255 ), screen );

    for( double wait = 0 ; wait < 9999999 ; wait++ ) ;
  }
  
  screen_interface.release_screen_pointer();

  return msg.wParam;
}

void draw_circle( svertex midpoint, long r, pixel_32 c, pixel_32 *screen )
{
  long x = 0, y = -r;
  long e = 1 - r;

  while( x <= -y )
  {
    screen[ (midpoint.sy + y) * x_res + (midpoint.sx + x) ] = c;
    screen[ (midpoint.sy + y) * x_res + (midpoint.sx - x) ] = c;
    screen[ (midpoint.sy - y) * x_res + (midpoint.sx + x) ] = c;
    screen[ (midpoint.sy - y) * x_res + (midpoint.sx - x) ] = c;

    screen[ (midpoint.sy + x) * x_res + (midpoint.sx + y) ] = c;
    screen[ (midpoint.sy + x) * x_res + (midpoint.sx - y) ] = c;
    screen[ (midpoint.sy - x) * x_res + (midpoint.sx + y) ] = c;
    screen[ (midpoint.sy - x) * x_res + (midpoint.sx - y) ] = c;

    if( e > 0 )
    {
      e += 2 * y + 2;
      y++;            
    }

    e += 2 * x + 3;
    x++;
  }
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
