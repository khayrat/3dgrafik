#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <time.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "spline.h"

double max_dist = 5;
double begin_x = 0, end_x = 800;
double begin_y = 600, end_y = 0;

void draw_control_points( long pc, svertex *points );

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  MessageBox
  ( 
    NULL, 
    "Die Kontrollpunkte der Spline werden hervorgehoben, sobald der Mauszeiger sich über ihre Oberfläche befindet.\n\nDie Bewegung der Punkte mit der Maus wird in Form des nachfolgenden Programms implementiert.",
    "Hinweis:",
    MB_OK | MB_ICONINFORMATION
  );

  screen_interface.open_window( hInstance, 800, 600, 32 );
  glMatrixMode( GL_PROJECTION );  glLoadIdentity();
  gluOrtho2D( begin_x, end_x, begin_y, end_y );

  glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
  glEnable( GL_POINT_SMOOTH );

  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable( GL_BLEND );

  glClearColor( 0, 0, 0, 0 );

  const long point_count = 8;
  svertex points[ point_count ] = 
  {
    svertex( 753, 342 ), svertex( 114, 421 ), svertex( 691,  48 ), svertex( 397, 546 ),
    svertex( 263,  67 ), svertex(  56, 467 ), svertex( 290, 522 ), svertex( 740, 151 )
  };

  spline k( point_count );

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key != 0 ) break;

	glClear( GL_COLOR_BUFFER_BIT );

    k.build( points );

    k.draw();
    draw_control_points( point_count, points );

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}

double dist( svertex a, svertex b )
{
  return sqrt( (b.sx - a.sx)*(b.sx - a.sx) + (b.sy - a.sy)*(b.sy - a.sy) );
}

void draw_control_points( long pc, svertex *points )
{
  glPointSize( 9 );

  glBegin( GL_POINTS );

    for( long x=0 ; x<pc ; x++ )
    {
      if( dist( points[ x ], input.mouse_act_pos ) <= max_dist )
        glColor3ub( 255, 255, 255 );

      else glColor3ub( 0, 0, 200 );

      glVertex2d( points[ x ].sx, points[ x ].sy );
    }

  glEnd();
}

