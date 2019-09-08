#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "matrix.h"
#include "window.h"
#include "wireframe.h"

local_system left, right;

uchar handle_input( void );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "horizontale Pfeiltasten:\tVeränderung des Abstandes zwischen\n\t\t\tden Drahtgittermodellen\n\nvertikale Pfeiltasten:\tVeränderung der Größe der\n\t\t\tDrahtgittermodelle\n\nEscape:\t\t\tEnde des Programms\n ");

  initialise_world( hInstance );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  window v( 640, 480, (screen_x_res-640)/2, (screen_y_res-480)/2 );

  wireframe thing( "icosahedron.lns" );
  
  matrix m;

  m.translate( 0, 0, 300 );
  thing.update_pos( m );
  m.clear();  

  m.translate( -20, 0, 0 );  left  = m * left;   m.clear();
  m.translate(  20, 0, 0 );  right = m * right;  m.clear();

  m.translate( -thing.ls.pos.wx, -thing.ls.pos.wy, -thing.ls.pos.wz );
  m.rotate_y( 0.005 );
  m.translate( thing.ls.pos.wx, thing.ls.pos.wy, thing.ls.pos.wz );

  while( 1 )
  {
    if( handle_input() ) break;

    thing.update_pos( m );

    v.install();

    camera = left;
    thing.display( sbuffer, pixel_32( 0, 255, 0 ) );

    camera = right;
    thing.display( sbuffer, pixel_32( 255, 0, 0 ) );

    v.display( screen, 0 );
  }

  screen_interface.release_screen_pointer();
  destroy_world();

  return input.msg.wParam;
}

uchar handle_input( void )
{
  if( input.check() == 1 ) return 1;
  if( input.event_key == VK_ESCAPE ) return 1;

  if( input.key_pressed( VK_LEFT  ) ) {  left.pos.wx *= 1.01;  right.pos.wx *= 1.01;  }
  if( input.key_pressed( VK_RIGHT ) ) {  left.pos.wx *= 0.99;  right.pos.wx *= 0.99;  }
  if( input.key_pressed( VK_UP    ) ) {  left.pos.wz -= 1.01;  right.pos.wz -= 1.01;  }
  if( input.key_pressed( VK_DOWN  ) ) {  left.pos.wz += 1.01;  right.pos.wz += 1.01;  }

  return 0;
}

