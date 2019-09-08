#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "polyhedron.h"
#include "matrix.h"

uchar handle_input( polyhedron *p );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  initialise_world( hInstance, 800, 600, 32 );

  polyhedron s[ 3 ];

  s[ 0 ].cone( 24, 24, 20 );
  s[ 1 ].cone( 24, 24, 20 );
  s[ 2 ].cone( 24, 24, 20 );
 
  matrix m;
  m.scale( 20, 20, 20 );
  m.translate( -500, 0, 2000 );
  s[ 0 ].update_pos( m );
  m.clear();

  m.scale( 20, 20, 20 );
  m.rotate_x( 0.5 * 3.1415926535 );
  m.translate( -500, 0, 2000 );
  s[ 1 ].update_pos( m );
  m.clear();

  m.scale( 20, 20, 20 );
  m.rotate_z( 0.5 * 3.1415926535 );
  m.translate( -500, 0, 2000 );
  s[ 2 ].update_pos( m );
  m.clear();

  m.translate( 0, 0, -2000 );
  m.rotate_y( 0.02 );  
  m.translate( 0, 0, 2000 );

  matrix rm[ 3 ];
  rm[ 0 ].rotate_x( 0.034 );
  rm[ 0 ].rotate_y( -0.041 );
  rm[ 1 ].rotate_y( 0.012 );
  rm[ 1 ].rotate_z( -0.03 );
  rm[ 2 ].rotate_x( 0.031 );
  rm[ 2 ].rotate_z( -0.056 );

  while( 1 )
  {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( input.check() == 1 ) break;
    if( input.event_key != 0 ) return 1;

    for( uchar x=0 ; x<3 ; x++ )
	{
      matrix a;
	  a.translate( -s[ x ].pos.wx, -s[ x ].pos.wy, -s[ x ].pos.wz );
      a = a * rm[ x ];
	  a.translate( s[ x ].pos.wx, s[ x ].pos.wy, s[ x ].pos.wz );

      a = a * m;

	  s[ x ].update_pos( a );

      s[ x ].display_polygons();
    }

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}
