#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"
#include "polyhedron.h"
#include "input_controller.h"

vertex begin( 0, -100, 800 );
vertex end( 0, 100, 800 );

uchar handle_input( void );
void display_axis( void );
local_system get_system( vertex begin, vertex end );

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "R, F, D, G, E, T:\tVerschiebung des blauen Punktes, um die Ausrichtung\n\t\tder Gerade zu verändern\n\nEscape:\t\tEnde des Programms\n ");

  initialise_world( hInstance, 640, 480, 32 );

  glClearColor( 1, 1, 1, 0 );

  polyhedron cube;
  cube.load( "cube.tg3" );

  matrix m;
  m.translate( 100, 0, 800 );
  cube.update_pos( m );
  m.clear();

  while( 1 )
  {
    if( handle_input() == 1 ) break;
  
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    local_system ls = get_system( begin, end );

    m.clear();
    m.translate( -ls.pos.wx, -ls.pos.wy, -ls.pos.wz );
    m.rows( ls.x_axis, ls.y_axis, ls.z_axis );
    m.rotate_x( 0.01 );
    m.columns( ls.x_axis, ls.y_axis, ls.z_axis );
    m.translate( ls.pos.wx, ls.pos.wy, ls.pos.wz );

    cube.update_pos( m );

    display_axis();
    cube.display();

	screen_interface.swap_buffers();
  }
  
  return input.msg.wParam;
}

local_system get_system( vertex begin, vertex end )
{
  local_system ls;

  ls.pos = begin;
  ls.x_axis = end - begin;

  vector v = vector( ls.x_axis.x, -ls.x_axis.z, -ls.x_axis.y );
  if( fabs( fabs( dot( ls.x_axis, v ) ) - dot( ls.x_axis, ls.x_axis ) ) < 0.001 )
    v = vector( ls.x_axis.y, -ls.x_axis.z, -ls.x_axis.x );
  ls.z_axis = cross( ls.x_axis, v );

  ls.y_axis = cross( ls.z_axis, ls.x_axis );

  ls.x_axis.set_length( 1 );
  ls.y_axis.set_length( 1 );
  ls.z_axis.set_length( 1 );

  return ls;
}

void display_axis( void )
{
  vector v = 10 * (end - begin);

  glColor3ub( 200, 200, 200 );
  glBegin( GL_LINES );
    glVertex3d( begin.wx-v.x, begin.wy-v.y, begin.wz-v.z );
    glVertex3d( begin.wx+v.x, begin.wy+v.y, begin.wz+v.z );
  glEnd(); 

  glPointSize( 8 );

  glEnable( GL_BLEND );
  glBegin( GL_POINTS );
    glVertex3d( begin.wx, begin.wy, begin.wz );
    glColor3ub( 0, 0, 200 );
    glVertex3d( end.wx, end.wy, end.wz );
  glEnd(); 
  glDisable( GL_BLEND );
}

uchar handle_input( void )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  double step = 0.5;

  if( input.key_pressed( 'D' ) ) end.wx -= step;
  if( input.key_pressed( 'G' ) ) end.wx += step;
  if( input.key_pressed( 'E' ) ) end.wy -= step;
  if( input.key_pressed( 'T' ) ) end.wy += step;
  if( input.key_pressed( 'F' ) ) end.wz -= step;
  if( input.key_pressed( 'R' ) ) end.wz += step;

  return 0;
}