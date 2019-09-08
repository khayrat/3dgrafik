#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "polyhedron.h"
#include "matrix.h"

uchar handle_input( vertex pos, matrix *m );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "R, F, D, G, E, T:\tRotation um die eigenen Achsen\nU, J, H, K, Z, I:\tVerschiebung in Richtung der Weltachsen\n\nEscape:\t\tEnde des Programms\n ");

  initialise_world( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polyhedron cube( "square.tg2" );

  matrix m;
  m.scale( 5, 5, 5 );
  m.rotate_z( 0.7854 );
  m.rotate_x( 0.5236 );
  m.rotate_y( -0.1571 );
  m.translate( 0, 0, 15 );
  cube.update_pos( m );
  m.clear();

  for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;

  while( 1 )
  {
    if( clear_translation > max_clear_translation ) clear_translation -= z_max;
    else
    {
      for( long x=0 ; x<x_res*y_res ; x++ ) zbuffer[ x ] = z_max;
      clear_translation = 0.0;
    }
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    if( handle_input( cube.pos, &m ) ) break;

    cube.update_pos( m );
    cube.display( sbuffer );

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

  double angle = 0.001;

  if( input.key_pressed( 'R' ) ) m->rotate_x(  angle );
  if( input.key_pressed( 'F' ) ) m->rotate_x( -angle );
  if( input.key_pressed( 'D' ) ) m->rotate_y(  angle );
  if( input.key_pressed( 'G' ) ) m->rotate_y( -angle );
  if( input.key_pressed( 'E' ) ) m->rotate_z(  angle );
  if( input.key_pressed( 'T' ) ) m->rotate_z( -angle );

  m->translate( pos.wx, pos.wy, pos.wz );

  double step = 0.1;

  if( input.key_pressed( 'H' ) ) m->translate( -step,  0,     0    );
  if( input.key_pressed( 'K' ) ) m->translate(  step,  0,     0    );
  if( input.key_pressed( 'Z' ) ) m->translate(  0,    -step,  0    );
  if( input.key_pressed( 'I' ) ) m->translate(  0,     step,  0    );
  if( input.key_pressed( 'J' ) ) m->translate(  0,     0,    -step );
  if( input.key_pressed( 'U' ) ) m->translate(  0,     0,     step );

  return 0;
}
