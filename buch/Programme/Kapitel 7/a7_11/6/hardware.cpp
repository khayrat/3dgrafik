#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"
#include "input_controller.h"

double begin_x = -5, end_x = 105;
double begin_y = -2, end_y =  5;

double f( double x )
{
  return 0.00015*x*x+0.5;
}

double g( double x )
{
  double n = 0.5;

  return 1.5/pow( 100, n ) * pow( x, n ) + 0.5;
}

double h( double x )
{
  static double a = 0;

  a += 0.00003;

  return sin( 0.2*x-a ) + 1;
}

double get_angle( double x )
{
  double max_angle = 5;
  long lens_radius = 100;

  x = lens_radius - x;

  return (max_angle / pow( lens_radius, 3 )) * x * x * x;
}

MSG msg;
uchar key_pressed( void );
void draw_xy( double begin_x, double end_x, double begin_y, double end_y );
void draw_f( double (*f)( double x ), double begin, double end, pixel_32 color );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "D, G:\tVeränderung der Einheitenlänge auf der x-Achse\nR, F:\tVeränderung der Einheitenlänge auf der y-Achse\n\nH, K:\tHorizontale Verschiebung des Koordinatensystems\nU, J:\tVertikale Verschiebung des Koordinatensystems\n\nEscape:\tProgramm beenden" );

  screen_interface.open_window( hInstance, 800, 600, 32 );

  while( 1 )
  {
    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );  glLoadIdentity();
    gluOrtho2D( begin_x, end_x, begin_y, end_y );
    draw_xy( begin_x, end_x, begin_y, end_y );

    if( key_pressed() ) break;

    draw_f( f, 0, 100, pixel_32( 255,   0,   0 ) );
    draw_f( g, 0, 100, pixel_32( 200, 255,   0 ) );
    draw_f( h, 0, 100, pixel_32( 128,   0, 255 ) );

    draw_f( get_angle, 0, 100, pixel_32( 0, 180, 0 ) );

    screen_interface.swap_buffers();
  }

  return msg.wParam;
}

void draw_f( double (*f)( double x ), double begin, double end, pixel_32 color )
{
  glColor3ub( color.red, color.green, color.blue );

  long dl = 100;
  double step = (end - begin) / dl;

  glBegin( GL_LINE_STRIP );

    for( double a = begin ; a <= end ; a += step )
       glVertex2d( a, f( a ) );

  glEnd();
}

void draw_xy( double begin_x, double end_x, double begin_y, double end_y )
{
  glColor3ub( 128, 128, 128 );

  glBegin( GL_LINES );

    glVertex2d( begin_x, 0 );  glVertex2d( end_x, 0 );
    glVertex2d( 0, begin_y );  glVertex2d( 0, end_y );

    double e1, e2;

    const long step_length = 1;

    e1 = -fabs( end_y - begin_y ) / double( y_res ) * 3;
    e2 =  fabs( end_y - begin_y ) / double( y_res ) * 4;
    long x = (long( begin_x ) / step_length) * step_length;
    for( ; x <= long( end_x ) ; x+=step_length )
    {
      glVertex2d( x, e1 );  glVertex2d( x, e2 );
    }

    e1 = -fabs( end_x - begin_x ) / double( x_res ) * 3;
    e2 =  fabs( end_x - begin_x ) / double( x_res ) * 4;
    long y = (long( begin_y ) / step_length) * step_length;
    for( ; y <= long( end_y ) ; y+=step_length )
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
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  if( input.key_pressed( 'D' ) ) {  begin_x *= 0.99;  end_x *= 0.99;  }
  if( input.key_pressed( 'G' ) ) {  begin_x *= 1.01;  end_x *= 1.01;  }
  if( input.key_pressed( 'F' ) ) {  begin_y *= 0.99;  end_y *= 0.99;  }
  if( input.key_pressed( 'R' ) ) {  begin_y *= 1.01;  end_y *= 1.01;  }

  double x_add =  fabs( end_x - begin_x ) / double( x_res ) * 3;
  double y_add =  fabs( end_y - begin_y ) / double( y_res ) * 3;

  if( input.key_pressed( 'H' ) ) {  begin_x += x_add;  end_x += x_add;  }
  if( input.key_pressed( 'K' ) ) {  begin_x -= x_add;  end_x -= x_add;  }
  if( input.key_pressed( 'J' ) ) {  begin_y += y_add;  end_y += y_add;  }
  if( input.key_pressed( 'U' ) ) {  begin_y -= y_add;  end_y -= y_add;  }

  return 0;
}

