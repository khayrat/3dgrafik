#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"
#include "input_controller.h"

double begin_x = 0,   end_x = 640;
double begin_y = -200, end_y = 200;

double t = 3.1415926535 / 180.0;
double add = 0;

double xf( double x )
{
  return 40*sin( x*0.5+add ) + 6*sin( x-add ) + 40*sin( x*2+add );
}

double yf( double y )
{
  return 40*sin( y-add ) + 8*sin( y*2 ) + 45*sin( y*2-add );
}

double xg( double x )
{
  return 5*sin( x+add ) + 40*sin( x-add ) + 100*sin( x+add );
}

double yg( double y )
{
  return 80*sin( y-add ) + 15*sin( y ) - 20*sin( y-add );
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

    draw_f( xf, begin_x, end_x, pixel_32( 200, 0, 0   ) );
    draw_f( yf, begin_x, end_x, pixel_32( 0, 0, 200 ) );

    draw_f( xg, begin_x, end_x, pixel_32( 255, 255, 0 ) );
    draw_f( yg, begin_x, end_x, pixel_32( 255, 255, 255 ) );

    screen_interface.swap_buffers();

    add += 0.04;
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
       glVertex2d( a, f( a*t ) );

  glEnd();
}

void draw_xy( double begin_x, double end_x, double begin_y, double end_y )
{
  glColor3ub( 128, 128, 128 );

  glBegin( GL_LINES );

    glVertex2d( begin_x, 0 );  glVertex2d( end_x, 0 );
    glVertex2d( 0, begin_y );  glVertex2d( 0, end_y );

    double e1, e2;

    const long step_length = 50;

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
