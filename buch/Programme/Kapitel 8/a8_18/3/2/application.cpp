#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "point_cloud.h"
#include "matrix.h"
#include "input_controller.h"
#include "wireframe.h"

MSG msg;
uchar handle_input( matrix *m, vertex pos );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Pfeiltasten, Bild auf, Bild ab\tRotation der Welt\nPos 1, Ende\t\tVerschiebung der Welt\n\nEscape:\t\t\tEnde des Programms\n ");

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

    memset( zbuffer, 0, x_res*y_res*sizeof( zbuffer[ 0 ] ) );
    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    pc.display( sbuffer );
    user.display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return msg.wParam;
}

uchar handle_input( matrix *m, vertex pos )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  m->clear();

  m->translate( -pos.wx, -pos.wy, -pos.wz );

  if( input.key_pressed( VK_UP    ) ) m->rotate_x(  0.01 );
  if( input.key_pressed( VK_DOWN  ) ) m->rotate_x( -0.01 );
  if( input.key_pressed( VK_LEFT  ) ) m->rotate_y(  0.01 );
  if( input.key_pressed( VK_RIGHT ) ) m->rotate_y( -0.01 );
  if( input.key_pressed( VK_PRIOR ) ) m->rotate_z(  0.01 );
  if( input.key_pressed( VK_NEXT  ) ) m->rotate_z( -0.01 );

  m->translate( pos.wx, pos.wy, pos.wz );

  double step = 0.2;

  if( input.key_pressed( VK_HOME ) ) m->translate( 0, 0, -step );
  if( input.key_pressed( VK_END  ) ) m->translate( 0, 0,  step );

  return 0;
}

