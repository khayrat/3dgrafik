#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "polyhedron.h"
#include "matrix.h"
#include "wireframe.h"

void move_ls( local_system *ls );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "R, F, D, G, E, T:\tRotation des Viewports\nU, J, H, K, Z, I:\tVerschiebung des Viewports\n\nEscape:\t\tEnde des Programms\n ");

  initialise_world( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polyhedron p[ 5 ];
  p[ 0 ].load( "icosahedron.tg2", 0 );
  p[ 1 ].load( "octahedron.tg2", 4 );
  p[ 2 ].load( "dodecahedron.tg2", 2 );
  p[ 3 ].load( "tetrahedron.tg2", 3 );
  p[ 4 ].load( "cube.tg2", 1 );

  matrix m;

  m.scale( 40, 40, 40 );  m.translate( -200, 0, 800 );  p[ 0 ].update_pos( m );  m.clear();
  m.scale( 30, 30, 30 );  m.translate(  200, 0, 800 );  p[ 1 ].update_pos( m );  m.clear();
  m.scale( 40, 40, 40 );  m.translate(    0, 0, 600 );  p[ 2 ].update_pos( m );  m.clear();
  m.scale( 20, 20, 20 );  m.translate(  200, 0, 400 );  p[ 3 ].update_pos( m );  m.clear();
  m.scale( 20, 20, 20 );  m.translate( -200, 0, 400 );  p[ 4 ].update_pos( m );  m.clear();

  while( 1 )
  {
    if( input.check() == 1 ) return 1;
    if( input.event_key == VK_ESCAPE ) return 1;
    
    move_ls( &camera );

    memset( zbuffer, 0, x_res*y_res*sizeof( zbuffer[ 0 ] ) );
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    for( uchar x=0 ; x<5 ; x++ ) p[ x ].display( sbuffer );

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
