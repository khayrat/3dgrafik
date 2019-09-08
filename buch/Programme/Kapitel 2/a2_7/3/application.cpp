#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"

double f1( double x )
{
  return sin( x*0.5 ) * 2;
}

double f2( double x )
{
  return sin( x ) * 0.3;
}

double f3( double x )
{
  return sin( x*2 ) * 2;
}

double s( double x )
{
  return f1( x ) + f2( x ) + f3( x );
}

const double pi = 3.1415926535;
double begin_x = -4*pi,  end_x = 4*pi;
double begin_y = -5,     end_y = 5;

MSG msg;
uchar key_pressed( void );

void draw_xy( pixel_32 color, pixel_32 *screen );
void draw_function( double (*f)( double x ), double start, double end, pixel_32 color, pixel_32 *screen );

svertex project( double x, double y );
void draw_line( svertex begin, svertex end, pixel_32 c, pixel_32 *screen );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  while( 1 )
  {
    if( key_pressed() ) break;

    draw_xy( pixel_32( 128, 128, 128 ), screen );
    
    draw_function( f1, begin_x, end_x, pixel_32( 200, 0, 0 ), screen );
    draw_function( f2, begin_x, end_x, pixel_32( 0, 200, 0 ), screen );
    draw_function( f3, begin_x, end_x, pixel_32( 0, 0, 200 ), screen );
    draw_function( s,  begin_x, end_x, pixel_32( 255, 255, 0 ), screen );
  }
  
  screen_interface.release_screen_pointer();

  return msg.wParam;
}

svertex project( double x, double y )
{
  long sx = long( (x - begin_x) * (double( x_res-1 ) / (end_x - begin_x)) );
  long sy = long( (y - end_y) * (double( y_res-1 ) / (begin_y - end_y)) );

  return svertex( sx, sy );
}

void draw_function( double (*f)( double x ), double start, double end, pixel_32 color, pixel_32 *screen )
{
  svertex p = project( start, f( start ) );

  double step = fabs( end_x - begin_x ) / double( x_res );
  for( double x = start + step ; x <= end ; x += step )
  {
    svertex q = project( x, f( x ) );

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