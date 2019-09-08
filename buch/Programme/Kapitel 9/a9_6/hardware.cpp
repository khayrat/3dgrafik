#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "landscape.h"
#include "matrix.h"
#include "viewport.h"

local_system global_view;

uchar handle_input( void );
void move_ls( local_system *ls );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Pfeiltasten, Bild auf, Bild ab\tRotation der Welt\nPos 1, Ende\t\tVerschiebung der Welt\n\nR, F, D, G, E, T:\t\tRotation des Viewports\nU, J, H, K, Z, I:\t\tVerschiebung des Viewports\n\nEscape:\t\t\tEnde des Programms\n ");

  initialise_world( hInstance, 800, 600, 32 );

  double length;
  viewport user;  length = user.load( x_res, y_res, pr_cnst, z_min, local_z_max );

  matrix m;

  m.translate( 0, 5, 0 );
  user.update_pos( m );
  m.clear();

  landscape planet;  planet.load( 50, length );

  m.rotate_y( 0.2 );
  m.rotate_x( -0.5 );
  m.translate( 0, 0, 50 );
  global_view = m * global_view;

  while( 1 )
  {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( handle_input() ) break;

    move_ls( &user.ls );
    planet.adjust_pos( &user.ls );

    m.clear();
    m.columns( global_view.x_axis, global_view.y_axis, global_view.z_axis );
    m.translate( global_view.pos.wx, global_view.pos.wy, global_view.pos.wz );
    m.adjust_hardware(); 

    planet.display_points();
    user.display();

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}

void rotate( local_system *ls, char c, double a )
{
  matrix m;

  switch( c )
  {
    case 'x' : m.rotate_x( a );  break;
    case 'y' : m.rotate_y( a );  break;
    case 'z' : m.rotate_z( a );  break;
  }

  m.columns( ls->x_axis, ls->y_axis, ls->z_axis );
  m.translate( ls->pos.wx, ls->pos.wy, ls->pos.wz );

  local_system new_ls;
  *ls = m * new_ls;
}

void move_ls( local_system *ls )
{
  double angle = 0.05;

  if( input.key_pressed( 'R' ) ) rotate( ls, 'x',  angle );
  if( input.key_pressed( 'F' ) ) rotate( ls, 'x', -angle );
  if( input.key_pressed( 'G' ) ) rotate( ls, 'y',  angle );
  if( input.key_pressed( 'D' ) ) rotate( ls, 'y', -angle );
  if( input.key_pressed( 'E' ) ) rotate( ls, 'z',  angle );
  if( input.key_pressed( 'T' ) ) rotate( ls, 'z', -angle );

  double step = 0.1;

  if( input.key_pressed( 'K' ) ) ls->pos = ls->pos +   step  * ls->x_axis;
  if( input.key_pressed( 'H' ) ) ls->pos = ls->pos + (-step) * ls->x_axis;
  if( input.key_pressed( 'I' ) ) ls->pos = ls->pos +   step  * ls->y_axis;
  if( input.key_pressed( 'Z' ) ) ls->pos = ls->pos + (-step) * ls->y_axis;
  if( input.key_pressed( 'U' ) ) ls->pos = ls->pos +   step  * ls->z_axis;
  if( input.key_pressed( 'J' ) ) ls->pos = ls->pos + (-step) * ls->z_axis;
}

uchar handle_input( void )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  matrix m;

  m.translate( -global_view.pos.wx, -global_view.pos.wy, -global_view.pos.wz );

  double angle = 0.01;

  if( input.key_pressed( VK_UP    ) ) m.rotate_x(  angle );
  if( input.key_pressed( VK_DOWN  ) ) m.rotate_x( -angle );
  if( input.key_pressed( VK_LEFT  ) ) m.rotate_y(  angle );
  if( input.key_pressed( VK_RIGHT ) ) m.rotate_y( -angle );
  if( input.key_pressed( VK_PRIOR ) ) m.rotate_z(  angle );
  if( input.key_pressed( VK_NEXT  ) ) m.rotate_z( -angle );

  m.translate( global_view.pos.wx, global_view.pos.wy, global_view.pos.wz );

  double step = 2;

  if( input.key_pressed( VK_HOME ) ) m.translate( 0, 0, -step );
  if( input.key_pressed( VK_END  ) ) m.translate( 0, 0,  step );

  global_view = m * global_view;

  return 0;
}
