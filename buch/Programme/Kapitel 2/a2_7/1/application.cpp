#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"

double a=0.001, b=-0.0093, c=-0.06, d=0.464;

double f( double x )
{
  return a*pow( x, 4 ) + b*pow( x, 3 ) + c*pow( x, 2 ) + d*pow( x, 1 );
}

double df( double x )
{
  return 4*a*pow( x, 3 ) + 3*b*pow( x, 2 ) + 2*c*x + d;
}

double ddf( double x )
{
  return 12*a*x*x + 6*b*x + 2*c;
}

double begin_x = -9,  end_x = 14;
double begin_y = -3,  end_y =  4;

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
  
    draw_function( f, begin_x, end_x, pixel_32( 255, 255, 0 ), screen );
    draw_function( df, begin_x, end_x, pixel_32( 255, 0, 0 ), screen );
    draw_function( ddf, begin_x, end_x, pixel_32( 0, 0, 255 ), screen );
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
  for( double x = start ; x <= end ; x += 0.01 )
  {
    svertex p = project( x, f( x ) );

    if( p.sx >= 0 && p.sx < x_res && p.sy >= 0 && p.sy < y_res )
      screen[ p.sy * x_res + p.sx ] = color;
  }
}

void draw_xy( pixel_32 color, pixel_32 *screen )
{
  draw_line( project( begin_x, 0 ), project( end_x, 0 ), color, screen );
  draw_line( project( 0, begin_y ), project( 0, end_y ), color, screen );
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