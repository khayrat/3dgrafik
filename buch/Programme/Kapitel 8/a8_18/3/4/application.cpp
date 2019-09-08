#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "polyhedron.h"
#include "matrix.h"
#include "wireframe.h"

void move_ls( local_system *ls );
uchar handle_input( matrix *m, vertex pos );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Pfeiltasten, Bild auf, Bild ab\tRotation der Welt\nPos 1, Ende\t\tVerschiebung der Welt\n\nR, F, D, G, E, T:\t\tRotation des lokalen Koordinatensystems\nU, J, H, K, Z, I:\t\tVerschiebung des lokalen Koordinatensystems\n\nEscape:\t\t\tEnde des Programms\n ");

  initialise_world( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();
  
  wireframe user( "complex_system.lns" );

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

  m.translate( -100, 0, 600 );
  user.update_pos( m );

  uchar x;

  while( 1 )
  {
    if( handle_input( &m, p[ 2 ].pos ) ) break;

    move_ls( &user.ls );

    for( x=0 ; x<5 ; x++ ) p[ x ].update_pos( m );
    user.update_pos( m );

    memset( zbuffer, 0, x_res*y_res*sizeof( zbuffer[ 0 ] ) );
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    for( x=0 ; x<5 ; x++ ) p[ x ].display( sbuffer );
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

  double step = 2;

  if( input.key_pressed( VK_HOME ) ) m->translate( 0, 0, -step );
  if( input.key_pressed( VK_END  ) ) m->translate( 0, 0,  step );

  return 0;
}

