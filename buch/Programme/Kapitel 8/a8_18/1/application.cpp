#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "polyhedron.h"
#include "matrix.h"

uchar handle_input( matrix *m, vertex pos );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "R, F, D, G, E, T:\tRotation der Welt um die eigenen Achsen\nU, J, H, K, Z, I:\tVerschiebung der Welt in Richtung der globalen Achsen\n\nEscape:\t\tEnde des Programms\n ");

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
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );
    memset( zbuffer, 0, x_res*y_res*sizeof( zbuffer[ 0 ] ) );

    if( handle_input( &m, p[ 2 ].pos ) ) break;

    for( uchar x=0 ; x<5 ; x++ )
    {
      p[ x ].update_pos( m );
      p[ x ].display( sbuffer );
    }

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return input.msg.wParam;
}

uchar handle_input( matrix *m, vertex pos )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  m->clear();

  m->translate( -pos.wx, -pos.wy, -pos.wz );

  if( input.key_pressed( 'R' ) ) m->rotate_x(  0.01 );
  if( input.key_pressed( 'F' ) ) m->rotate_x( -0.01 );
  if( input.key_pressed( 'D' ) ) m->rotate_y( -0.01 );
  if( input.key_pressed( 'G' ) ) m->rotate_y(  0.01 );
  if( input.key_pressed( 'E' ) ) m->rotate_z( -0.01 );
  if( input.key_pressed( 'T' ) ) m->rotate_z(  0.01 );

  m->translate( pos.wx, pos.wy, pos.wz );

  if( input.key_pressed( 'H' ) ) m->translate( -1,  0,  0 );
  if( input.key_pressed( 'K' ) ) m->translate(  1,  0,  0 );
  if( input.key_pressed( 'I' ) ) m->translate(  0,  1,  0 );
  if( input.key_pressed( 'Z' ) ) m->translate(  0, -1,  0 );
  if( input.key_pressed( 'U' ) ) m->translate(  0,  0,  1 );
  if( input.key_pressed( 'J' ) ) m->translate(  0,  0, -1 );

  return 0;
}

