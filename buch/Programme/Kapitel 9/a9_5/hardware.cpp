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

void move_ls( local_system *ls );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "R, F, D, G, E, T:\tRotation des Viewports\nU, J, H, K, Z, I:\tVerschiebung des Viewports\n\nEscape:\t\tEnde des Programms\n ");

  initialise_world( hInstance, 800, 600, 32 );

  landscape planet;  planet.load( 255 );

  viewport user( x_res, y_res, pr_cnst, z_min, z_max );

  matrix m;
  m.translate( 0, 30, 0 );
  user.update_pos( m );

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key == VK_ESCAPE ) break;

    move_ls( &user.ls );

    m.clear();
    m.translate( -user.ls.pos.wx, -user.ls.pos.wy, -user.ls.pos.wz );
    m.rows( user.ls.x_axis, user.ls.y_axis, user.ls.z_axis );
    m.adjust_hardware(); 

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    planet.display_polygons();

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

