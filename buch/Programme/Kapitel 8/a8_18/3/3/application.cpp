#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "point_cloud.h"
#include "matrix.h"
#include "input_controller.h"
#include "wireframe.h"

uchar handle_input( matrix *m, vertex pos );
void move_ls( local_system *ls );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Pfeiltasten, Bild auf, Bild ab\tRotation der Welt\nPos 1, Ende\t\tVerschiebung der Welt\n\nR, F, D, G, E, T:\t\tRotation des lokalen Koordinatensystems\nU, J, H, K, Z, I:\t\tVerschiebung des lokalen Koordinatensystems\n\nEscape:\t\t\tEnde des Programms\n ");

  initialise_world( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  point_cloud pc;
  pc.plane( 20 );

  wireframe user( "complex_system.lns" );

  matrix m;

  m.scale( 0.1, 0.1, 0.1 );
  m.translate( 0, 2, 0 );
  user.update_pos( m );
  m.clear();

  m.rotate_y( 0.2 );
  m.rotate_x( -0.35 );
  m.translate( 0, 0, 40 );
  pc.update_pos( m );
  user.update_pos( m );
  m.clear();

  while( 1 )
  {
    if( handle_input( &m, pc.pos ) ) break;

    pc.update_pos( m );
    user.update_pos( m );

    move_ls( &user.ls );

    memset( zbuffer, 0, x_res*y_res*sizeof( zbuffer[ 0 ] ) );
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    pc.display( sbuffer );
    user.display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

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

  double step = 0.5;

  if( input.key_pressed( 'K' ) ) ls->pos = ls->pos +   step  * ls->x_axis;
  if( input.key_pressed( 'H' ) ) ls->pos = ls->pos + (-step) * ls->x_axis;
  if( input.key_pressed( 'I' ) ) ls->pos = ls->pos +   step  * ls->y_axis;
  if( input.key_pressed( 'Z' ) ) ls->pos = ls->pos + (-step) * ls->y_axis;
  if( input.key_pressed( 'U' ) ) ls->pos = ls->pos +   step  * ls->z_axis;
  if( input.key_pressed( 'J' ) ) ls->pos = ls->pos + (-step) * ls->z_axis;
}

uchar handle_input( matrix *m, vertex pos )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  m->clear();

  m->translate( -pos.wx, -pos.wy, -pos.wz );

  if( input.key_pressed( VK_UP    ) ) m->rotate_x(  0.05 );
  if( input.key_pressed( VK_DOWN  ) ) m->rotate_x( -0.05 );
  if( input.key_pressed( VK_LEFT  ) ) m->rotate_y(  0.05 );
  if( input.key_pressed( VK_RIGHT ) ) m->rotate_y( -0.05 );
  if( input.key_pressed( VK_PRIOR ) ) m->rotate_z(  0.05 );
  if( input.key_pressed( VK_NEXT  ) ) m->rotate_z( -0.05 );

  m->translate( pos.wx, pos.wy, pos.wz );

  double step = 0.8;

  if( input.key_pressed( VK_HOME ) ) m->translate( 0, 0, -step );
  if( input.key_pressed( VK_END  ) ) m->translate( 0, 0,  step );

  return 0;
}

