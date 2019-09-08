#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"

double begin_x = -7,    end_x = 7;
double begin_y = -4.4,  end_y = 4.4;

vertex k[ 4 ] = { vertex( -4, 0 ), vertex( 6, 4 ), vertex( -6, -4 ), vertex( 4, 0 ) };

MSG msg;
uchar key_pressed( void );

void draw_xy( pixel_32 color, pixel_32 *screen );
void draw_function( vertex (*f)( double x ), double start, double end, pixel_32 color, pixel_32 *screen );
void draw_line( svertex begin, svertex end, pixel_32 c, pixel_32 *screen );

svertex project( vertex p );
svertex project( double x, double y ) {  return project( vertex( x, y ) );  }

vertex b( double t )
{
  double bs0 = (1-t)*(1-t)*(1-t);
  double bs1 = 3*t*(1-t)*(1-t);
  double bs2 = 3*t*t*(1-t);
  double bs3 = t*t*t;

  double wx = bs0 * k[ 0 ].wx + bs1 * k[ 1 ].wx + bs2 * k[ 2 ].wx + bs3 * k[ 3 ].wx;
  double wy = bs0 * k[ 0 ].wy + bs1 * k[ 1 ].wy + bs2 * k[ 2 ].wy + bs3 * k[ 3 ].wy;

  return vertex( wx, wy );
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  while( 1 )
  {
    if( key_pressed() ) break;

    draw_xy( pixel_32( 128, 128, 128 ), screen );
    
    for( char x=0 ; x<3 ; x++ )
       draw_line( project( k[ x ] ), project( k[ x+1 ] ), pixel_32( 0, 0, 150 ), screen );

    draw_function( b, 0, 1, pixel_32( 255, 255, 0 ), screen );
  }
  
  screen_interface.release_screen_pointer();

  return msg.wParam;
}

svertex project( vertex p )
{
  long sx = long( (p.wx - begin_x) * (double( x_res-1 ) / (end_x - begin_x)) );
  long sy = long( (p.wy - end_y) * (double( y_res-1 ) / (begin_y - end_y)) );

  return svertex( sx, sy );
}

void draw_function( vertex (*f)( double x ), double start, double end, pixel_32 color, pixel_32 *screen )
{
  svertex p = project( f( start ) );

  double dl = 0.001;
  for( double a = start + dl ; a <= end ; a += dl )
  {
    svertex q = project( f( a ) );

    draw_line( p, q, color, screen );

    p = q;
  }
}

void draw_xy( pixel_32 color, pixel_32 *screen )
{
  draw_line( project( begin_x, 0 ), project( end_x, 0 ), color, screen );
  draw_line( project( 0, begin_y ), project( 0, end_y ), color, screen );

  svertex right = project( end_x, 0 );
  svertex top = project( 0, end_y );

  draw_line( svertex( right.sx-11, right.sy-5 ), right, color, screen );
  draw_line( svertex( right.sx-11, right.sy+5 ), right, color, screen );
  draw_line( svertex( top.sx-5, top.sy+11 ), top, color, screen );
  draw_line( svertex( top.sx+5, top.sy+11 ), top, color, screen );

  for( long x = long( begin_x ) ; x <= long( end_x ) ; x++ )
  {
    svertex p = project( x, 0 );

    if( p.sx < right.sx - 11 )
      draw_line( svertex( p.sx, p.sy+3 ), svertex( p.sx, p.sy-3 ), color, screen );
  }

  for( long y = long( begin_y ) ; y <= long( end_y ) ; y++ )
  {
    svertex p = project( 0, y );

    if( p.sy > top.sy + 11 )
      draw_line( svertex( p.sx-3, p.sy ), svertex( p.sx+3, p.sy ), color, screen );
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

void draw_line( svertex begin, svertex end, pixel_32 c, pixel_32 *screen )
{
  if
  (
    begin.sx < 0 || begin.sx >= x_res || end.sx < 0 || end.sx >= x_res ||
    begin.sy < 0 || begin.sy >= y_res || end.sy < 0 || end.sy >= y_res
  ) 
  return;

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

    offset += xstep;

    e += delta_y;
    if( e >= delta_x )
    {
      e -= delta_x;  offset += ystep;
    }
  }
}