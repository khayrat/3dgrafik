#include <windows.h>
#include <time.h>

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
  message( "Steuerung:", "R und F:\tRotation um die x-Achse\nD und G:\tRoatation um die y-Achse\nE und T:\tRotation um die z-Achse\n\nPfeiltaste links:\tVorheriger Polyeder\nPfeiltaste rechts:\tNächster Polyeder\n\nEscape:\tEnde des Programms\n ");

  initialise_world( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  polyhedron p[ 5 ];
  p[ 0 ].load( "tetrahedron.tg1" );
  p[ 1 ].load( "cube.tg1" );
  p[ 2 ].load( "octahedron.tg1" );
  p[ 3 ].load( "icosahedron.tg1" );
  p[ 4 ].load( "dodecahedron.tg1" );

  matrix m;

  m.translate( 0, 0, 800 );
  for( long x=0 ; x<5 ; x++ ) p[ x ].update_pos( m );
  m.clear();

  while( 1 )
  {
    if( handle_input( &p[ act_index ] ) ) break;

    p[ act_index ].update_pos( m );

    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

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

  p->update_pos( m );

  if( input.event_key == VK_LEFT ) {  --act_index;  if( act_index < 0 ) act_index = 4;  }
  if( input.event_key == VK_RIGHT ) {  ++act_index;  if( act_index > 4 ) act_index = 0;  }

  return 0;
}
