#include <windows.h>

#include "screen_interface.h"

struct svertex
{
  long sx, sy;

  svertex( void ) : sx( 0 ), sy( 0 ) { }
  svertex( long x, long y ) : sx( x ), sy( y ) { }
};

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

  svertex begin = svertex( rand() % x_res, rand() % y_res );
  svertex end = svertex( rand() % x_res, rand() % y_res );

  uchar running = 1;
  while( running )
  {
    draw_line( begin, end, palette[ rand() % 256 ], screen );

    for( double wait = 0 ; wait < 9999999 ; wait++ ) 
       if( key_pressed() ) {  running = 0;  break;  }

    begin = end;
    end = svertex( rand() % x_res, rand() % y_res );
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

void draw_line( svertex begin, svertex end, pixel_32 color, pixel_32 *screen )
{
  double detail_level = 0.001;

  for( double t=0 ; t<=1 ; t+=detail_level )
  {
    long x = long( begin.sx + t * (end.sx - begin.sx) );
    long y = long( begin.sy + t * (end.sy - begin.sy) );

    screen[ y * x_res + x ] = color;
  }
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