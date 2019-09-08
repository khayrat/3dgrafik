#include <windows.h>
#include <time.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "polyhedron.h"
#include "matrix.h"

long act_index = 0;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Pfeiltaste links:\tVorheriger Polyeder\nPfeiltaste rechts:\tNächster Polyeder\n\nEscape:\t\tEnde des Programms\n ");

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

  srand( time( NULL ) );

  m.translate( 0, 0, -p[ 0 ].pos.wz );
  m.rotate_x( ((rand() % 20) - 10) * 0.001 );
  m.rotate_y( ((rand() % 20) - 10) * 0.001 );
  m.rotate_z( ((rand() % 20) - 10) * 0.001 );
  m.translate( 0, 0, p[ 0 ].pos.wz );

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key == VK_ESCAPE ) break;

    if( input.event_key == VK_LEFT ) {  --act_index;  if( act_index < 0 ) act_index = 4;  }
    if( input.event_key == VK_RIGHT ) {  ++act_index;  if( act_index > 4 ) act_index = 0;  }

    p[ act_index ].update_pos( m );

    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    p[ act_index ].display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return input.msg.wParam;
}
