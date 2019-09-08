#include <windows.h>
#include <time.h>

#include "screen_interface.h"

struct svertex
{
  long sx, sy;

  svertex( void ) : sx( 0 ), sy( 0 ) { }
  svertex( long x, long y ) : sx( x ), sy( y ) { }
};

uchar running = 1;
pixel_32 palette[ 256 ];
MSG msg;

uchar key_pressed( void );
void load_palette( void );
void draw_line( svertex begin, svertex end, pixel_32 color, pixel_32 *screen );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  load_palette();

  srand( time( NULL ) );

  svertex begin = svertex( rand() % 640, rand() % 480 );
  svertex end = svertex( rand() % 640, rand() % 480 );

  while( running )
  {
    draw_line( begin, end, palette[ rand() % 256 ], screen );

    begin = end;
    end = svertex( rand() % 640, rand() % 480 );
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

void load_palette( void )
{
  long x;
  uchar c;

  for( x=0, c=0 ; x<64 ; x++, c+=4 )
  {
    palette[ x     ] = pixel_32( c, 0, 0 );
    palette[ x+64  ] = pixel_32( c, c, c );
    palette[ x+128 ] = pixel_32( 0, c, 0 );
    palette[ x+192 ] = pixel_32( 0, 0, c );
  }
}

void draw_line( svertex begin, svertex end, pixel_32 c, pixel_32 *screen )
{
  long delta_x, delta_y, e, xstep, ystep, length;
  long offset = begin.sy * x_res + begin.sx;

  delta_x = end.sx - begin.sx;  delta_y = end.sy - begin.sy;
  xstep = 1;  ystep = x_res;

  if( delta_x < 0 ) {  delta_x = -delta_x;  xstep = -xstep; }
  if( delta_y < 0 ) {  delta_y = -delta_y;  ystep = -ystep; }

  if( delta_y > delta_x )
  {
    long t = delta_x;  delta_x = delta_y;  delta_y = t;
    t = xstep;  xstep = ystep;  ystep = t;
  }

  length = delta_x+1;  e = 0;

  while( length-- > 0 )
  {
    screen[ offset ] = c;

    for( double wait = 0 ; wait < 99999 ; wait++ ) 
       if( key_pressed() ) {  running = 0;  return;  }

    offset += xstep;

    e += delta_y;
    if( e >= delta_x )
    {
      e -= delta_x;  offset += ystep;
    }
  }
}

