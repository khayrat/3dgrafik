#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <time.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "spline.h"

double begin_x = -3, end_x = 21;
double begin_y = -7, end_y = 12;

MSG msg;
uchar key_pressed( void );
void draw_system( void );

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  glMatrixMode( GL_PROJECTION );  glLoadIdentity();
  gluOrtho2D( begin_x, end_x, begin_y, end_y );

  glClearColor( 0, 0, 0, 0 );

  const long point_count = 8;
  svertex points[ point_count ] =
  {
    svertex( 19, 1 ),  svertex(  0, -1 ),  svertex( 17,  10 ),  svertex( 8,  -5 ),
    svertex( 4,  9 ),  svertex( -1, -2 ),  svertex( 5,  -4  ),  svertex( 19,  7 )
  };

  spline k( point_count );
  k.build( points );

  while( 1 )
  {
    if( key_pressed() ) break;

    draw_system();
    k.draw();
  }

  return msg.wParam;
}

void draw_system( void )
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