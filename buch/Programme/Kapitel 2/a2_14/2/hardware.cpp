#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"

double s = 3;
double r = 1;
double t = 1.5;

const double pi = 3.1415926535;
double begin_x = -7,  end_x = 7;
double begin_y = -7,  end_y = 7;

svertex great_circle( double alpha )
{
  return svertex( s * cos( alpha ), s * sin( alpha ) );
}

svertex small_circle( double alpha )
{
  return svertex( r * cos( alpha ), r * sin( alpha ) - s + r );
}

svertex f( double alpha )
{
  double beta = (alpha * r) / s;
  double sx = t*r * sin( alpha ) * cos( beta ) - (t*r * cos( alpha ) - s + r) * sin( beta );
  double sy = t*r * sin( alpha ) * sin( beta ) + (t*r * cos( alpha ) - s + r) * cos( beta );

  return svertex( sx, sy );
}

MSG msg;
uchar key_pressed( void );
void draw_xy( double begin_x, double end_x, double begin_y, double end_y );
void draw_f( svertex (*f)( double x ), double begin, double end, pixel_32 color );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );

  begin_y = (begin_x / x_res) * y_res;
  end_y = (end_x / x_res) * y_res;

  glMatrixMode( GL_PROJECTION );  glLoadIdentity();
  gluOrtho2D( begin_x, end_x, begin_y, end_y );

  while( 1 )
  {
    if( key_pressed() ) break;

    draw_xy( begin_x, end_x, begin_y, end_y );

    draw_f( great_circle, begin_x, end_x, pixel_32( 0, 0, 255 ) );
    draw_f( small_circle, begin_x, end_x, pixel_32( 255, 255, 0 ) );

    draw_f( f, -100, 100, pixel_32( 255, 255, 255 ) );

    screen_interface.swap_buffers();
  }

  return msg.wParam;
}

void draw_f( svertex (*f)( double x ), double begin, double end, pixel_32 color )
{
  glColor3ub( color.red, color.green, color.blue );

  glBegin( GL_LINE_STRIP );

    for( double x = begin ; x <= end ; x += 0.01 )
    {
      svertex p = f( x );
      glVertex2d( p.sx, p.sy );
    }

  glEnd();
}

void draw_xy( double begin_x, double end_x, double begin_y, double end_y )
{
  glColor3ub( 128, 128, 128 );

  glBegin( GL_LINES );

    glVertex2d( begin_x, 0 );  glVertex2d( end_x, 0 );
    glVertex2d( 0, begin_y );  glVertex2d( 0, end_y );

    double e1, e2;

    e1 = -fabs( end_y - begin_y ) / double( y_res ) * 3;
    e2 =  fabs( end_y - begin_y ) / double( y_res ) * 4;
    for( long x = long( begin_x ) ; x <= long( end_x ) ; x++ )
    {
      glVertex2d( x, e1 );  glVertex2d( x, e2 );
    }

    e1 = -fabs( end_x - begin_x ) / double( x_res ) * 3;
    e2 =  fabs( end_x - begin_x ) / double( x_res ) * 4;
    for( long y = long( begin_y ) ; y <= long( end_y ) ; y++ )
    {
      glVertex2d( e1, y );  glVertex2d( e2, y );
    }

    e1 = fabs( end_x - begin_x ) / double( x_res ) * 11;
    e2 = fabs( end_y - begin_y ) / double( y_res ) * 5;
    glVertex2d( end_x-e1, -e2 );  glVertex2d( end_x, 0 );
    glVertex2d( end_x-e1, e2 );  glVertex2d( end_x, 0 );

    e1 = fabs( end_x - begin_x ) / double( x_res ) * 5;
    e2 = fabs( end_y - begin_y ) / double( y_res ) * 11;
    glVertex2d( -e1, end_y-e2 );  glVertex2d( 0, end_y );
    glVertex2d( e1, end_y-e2 );  glVertex2d( 0, end_y );

  glEnd();
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
