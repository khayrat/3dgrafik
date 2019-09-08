#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "matrix.h"
#include "input_controller.h"
#include "wireframe.h"

MSG msg;
uchar handle_input( matrix *m, local_system *ls );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Pfeiltasten:\tRotation des lokalen Koordinatensystems\nEnter:\t\tKoordinatensystem nimmt die ursprüngliche\n\t\tAusrichtung an\n\nEscape:\t\tEnde des Programms\n ");

  initialise_world( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  wireframe system( "system.lns" );

  local_system ls;
  ls.pos.wz = 80;

  matrix m;

  m.scale( 25, 25, 25 );
  m.translate( 0, 0, 160 );
  system.update_pos( m );
  m.clear();

  while( 1 )
  {
    if( handle_input( &m, &ls ) ) break;

    system.update_pos( m );
    ls = m * ls;

    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );
    memset( zbuffer, 0, x_res*y_res*sizeof( zbuffer[ 0 ] ) );

    system.display( sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return msg.wParam;
}

uchar handle_input( matrix *m, local_system *ls )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  m->clear();

  m->translate( 0, 0, -160 );

  if( input.key_pressed( VK_UP    ) ) m->rotate_x(  0.01 );
  if( input.key_pressed( VK_DOWN  ) ) m->rotate_x( -0.01 );
  if( input.key_pressed( VK_LEFT  ) ) m->rotate_y(  0.01 );
  if( input.key_pressed( VK_RIGHT ) ) m->rotate_y( -0.01 );
  if( input.key_pressed( VK_PRIOR ) ) m->rotate_z(  0.01 );
  if( input.key_pressed( VK_NEXT  ) ) m->rotate_z( -0.01 );

  if( input.event_key == VK_RETURN )
    m->rows( ls->x_axis, ls->y_axis, ls->z_axis );
    
  m->translate( 0, 0, 160 );

  return 0;
}

