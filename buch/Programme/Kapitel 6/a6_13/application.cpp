#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "polyhedron.h"
#include "matrix.h"

long act_polyhedron = 0;

uchar handle_input( vertex pos, matrix *m );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "R, F, D, G, E, T:\t\tRotation um die eigenen Achsen\nU, J, H, K, Z, I:\t\tVerschiebung in Richtung der Weltachsen\n\nPfeiltaste links:\t\tVorheriger Polyeder\nPfeiltaste rechts:\t\tNächster Polyeder\n\nEscape:\t\t\tEnde des Programms\n ");

  initialise_world( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polyhedron p[ 5 ];

  p[ 0 ].load( "sphere1.tg2" );
  p[ 1 ].load( "sphere2.tg2" );
  p[ 2 ].load( "torus.tg2" );
  p[ 3 ].load( "t_compound.tg2" );
  p[ 4 ].load( "c_compound.tg2" );

  matrix m;

  m.scale( 80, 80, 80 );
  m.translate( 0, 0, 800 );
  p[ 0 ].update_pos( m );
  p[ 1 ].update_pos( m );
  m.clear();

  m.translate( 0, 0, 1400 );
  p[ 2 ].update_pos( m );
  m.clear();

  m.scale( 50, 50, 50 );
  m.translate( 0, 0, 300 );
  p[ 3 ].update_pos( m );
  p[ 4 ].update_pos( m );
  m.clear();

  long x;

  for( x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;

  while( 1 )
  {
    if( clear_translation > max_clear_translation ) clear_translation -= z_max;
    else
    {
      for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;
      clear_translation = 0.0;
    }
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    if( handle_input( p[ act_polyhedron ].pos, &m ) ) break;

    p[ act_polyhedron ].update_pos( m );
    p[ act_polyhedron ].display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return input.msg.wParam;
}

uchar handle_input( vertex pos, matrix *m )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  m->clear();

  m->translate( -pos.wx, -pos.wy, -pos.wz );

  if( input.key_pressed( 'R' ) ) m->rotate_x(  0.01 );
  if( input.key_pressed( 'F' ) ) m->rotate_x( -0.01 );
  if( input.key_pressed( 'D' ) ) m->rotate_y(  0.01 );
  if( input.key_pressed( 'G' ) ) m->rotate_y( -0.01 );
  if( input.key_pressed( 'E' ) ) m->rotate_z(  0.01 );
  if( input.key_pressed( 'T' ) ) m->rotate_z( -0.01 );

  m->translate( pos.wx, pos.wy, pos.wz );

  if( input.key_pressed( 'H' ) ) m->translate( -1,  0,  0 );
  if( input.key_pressed( 'K' ) ) m->translate(  1,  0,  0 );
  if( input.key_pressed( 'Z' ) ) m->translate(  0, -1,  0 );
  if( input.key_pressed( 'I' ) ) m->translate(  0,  1,  0 );
  if( input.key_pressed( 'J' ) ) m->translate(  0,  0, -1 );
  if( input.key_pressed( 'U' ) ) m->translate(  0,  0,  1 );

  if( input.event_key == VK_LEFT  ) {  --act_polyhedron;  if( act_polyhedron < 0 ) act_polyhedron = 4;  }
  if( input.event_key == VK_RIGHT ) {  ++act_polyhedron;  if( act_polyhedron > 4 ) act_polyhedron = 0;  }

  return 0;
}
