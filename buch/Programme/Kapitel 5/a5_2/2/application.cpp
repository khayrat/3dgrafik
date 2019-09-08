#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "polyhedron.h"
#include "matrix.h"

long act_index = 0;
uchar handle_input( polyhedron *p );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "R, F, D, G, E, T:\t\tRotation um die eigenen Achsen\nU, J, H, K, Z, I:\t\tVerschiebung in Richtung der Weltachsen\n\nPfeiltasten oben und unten:\tDrahtgittermodell, Gefüllte Polygone\nPfeiltasten rechts und links:\tVorheriger, Nächster Polyeder\n\nEscape:\t\t\tEnde des Programms\n ");

  initialise_world( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polyhedron p[ 5 ];
  p[ 0 ].load( "tetrahedron.tg1" );
  p[ 1 ].load( "cube.tg1" );
  p[ 2 ].load( "octahedron.tg1" );
  p[ 3 ].load( "icosahedron.tg1" );
  p[ 4 ].load( "dodecahedron.tg1" );

  matrix m;

  long x;

  m.translate( 0, 0, 800 );
  for( x=0 ; x<5 ; x++ ) p[ x ].update_pos( m );
  m.clear();

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

    if( handle_input( &p[ act_index ] ) ) break;

    p[ act_index ].update_pos( m );
    p[ act_index ].display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return input.msg.wParam;
}

uchar handle_input( polyhedron *p )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  matrix m;

  m.translate( -p->pos.wx, -p->pos.wy, -p->pos.wz );

  if( input.key_pressed( 'R' ) ) m.rotate_x(  0.01 );
  if( input.key_pressed( 'F' ) ) m.rotate_x( -0.01 );
  if( input.key_pressed( 'D' ) ) m.rotate_y(  0.01 );
  if( input.key_pressed( 'G' ) ) m.rotate_y( -0.01 );
  if( input.key_pressed( 'E' ) ) m.rotate_z(  0.01 );
  if( input.key_pressed( 'T' ) ) m.rotate_z( -0.01 );

  m.translate( p->pos.wx, p->pos.wy, p->pos.wz );

  if( input.key_pressed( 'H' ) ) m.translate( -1,  0,  0 );
  if( input.key_pressed( 'K' ) ) m.translate(  1,  0,  0 );
  if( input.key_pressed( 'Z' ) ) m.translate(  0, -1,  0 );
  if( input.key_pressed( 'I' ) ) m.translate(  0,  1,  0 );
  if( input.key_pressed( 'J' ) ) m.translate(  0,  0, -1 );
  if( input.key_pressed( 'U' ) ) m.translate(  0,  0,  1 );

  p->update_pos( m );

  if( input.event_key == VK_LEFT ) {  --act_index;  if( act_index < 0 ) act_index = 4;  }
  if( input.event_key == VK_RIGHT ) {  ++act_index;  if( act_index > 4 ) act_index = 0;  }
  if( input.event_key == VK_UP ) display_mode = 0;
  if( input.event_key == VK_DOWN ) display_mode = 1;

  return 0;
}
