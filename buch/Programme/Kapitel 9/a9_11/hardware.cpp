#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <time.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"

void add_point( long *pc, long max_pc, vertex *points );
long convex_hull( vertex *points, long max_pc, vertex *hull );
void draw_points( long pc, vertex *points, char c );

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "linke Maustaste:\tSetzen eines neuen Punktes\n\nEscape:\t\tProgramm beenden\n" );

  screen_interface.open_window( hInstance, 800, 600, 32 );
  glMatrixMode( GL_PROJECTION );  glLoadIdentity();
  gluOrtho2D( 0, x_res, 0, y_res );

  glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
  glEnable( GL_POINT_SMOOTH );

  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable( GL_BLEND );

  glClearColor( 1, 1, 1, 0 );

  const long point_count = 500;
  vertex points[ point_count ];
  vertex hull[ point_count ];

  long act_pc = 3;
  for( long x=0 ; x<act_pc ; x++ ) points[ x ] = vertex( rand() % x_res, rand() % y_res );

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.key_pressed( VK_ESCAPE ) ) break;

    add_point( &act_pc, point_count, points );

	glClear( GL_COLOR_BUFFER_BIT );

    long convex_count = convex_hull( points, act_pc, hull );

    draw_points( act_pc, points, 0 );
    draw_points( convex_count, hull, 1 );

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}

long convex_hull( vertex *points, long pc, vertex *hull )
{
  points[ 0 ].wy = 0;
  hull[ 0 ] = points[ 0 ];
  long left_index = 0;

  for( long x=1 ; x<pc ; x++ )
  {
    points[ x ].wy = 0;

    if( points[ x ].wx < hull[ 0 ].wx )
    {
      hull[ 0 ] = points[ x ];
      left_index = x;
    }
  }

  vector v = vector( 0, 0, -100 );
  long act;

  for( act = 0 ; act < pc-1 ; act++ )
  {
    long index = 0;
    double least_cos = 1;

    for( long x=0 ; x<pc ; x++ )
    {
      vector new_v = points[ x ] - hull[ act ];

      if( new_v.length < 0.00001 ) continue;

      double cos_alpha = dot( v, new_v ) / (v.length * new_v.length);
      if( cos_alpha <= least_cos )
      {
        index = x;
        least_cos = cos_alpha;
      }
    }

    if( index == left_index ) break;

    hull[ act+1 ] = points[ index ];

    v = hull[ act ] - hull[ act+1 ];
  }

  return act + 1;
}

void add_point( long *pc, long max_pc, vertex *points )
{
  if( input.mouse_left_state != 1 ) return;
  if( *pc >= max_pc - 1 ) return;

  points[ *pc ] = vertex( input.mouse_up_pos.sx,  y_res-input.mouse_up_pos.sy );
  *pc += 1;
}

void draw_points( long pc, vertex *points, char c )
{
  if( c == 0 )
  {
    glPointSize( 9 );
    glColor3ub( 0, 0, 200 );
  
    glBegin( GL_POINTS );
  }

  else
  {
    glColor3ub( 0, 0, 0 );
  
    glBegin( GL_LINE_LOOP );
  }

    for( long x=0 ; x<pc ; x++ )
       glVertex2d( points[ x ].wx, points[ x ].wz );

  glEnd();
}
