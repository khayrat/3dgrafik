#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <windows.h>
#include "simple_types.h"

class input_controller
{
  public:
	svertex mouse_down_pos, mouse_up_pos;
	svertex mouse_act_pos;

    char mouse_left_state, mouse_right_state;

	long event_key;

    MSG msg;

    long check( void );
	uchar key_pressed( int key ) {  return GetKeyState( key ) < 0;  }

    input_controller( void ) : mouse_left_state( 0 ), mouse_right_state( 0 ) { }
} input;

long input_controller::check( void )
{
  event_key = 0;

  if( mouse_left_state == 1 ) mouse_left_state = 0;
  if( mouse_right_state == 1 ) mouse_right_state = 0;

  if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
  {
    switch( msg.message )
	{
	  case WM_KEYDOWN     : event_key = msg.wParam;
                            break;
      case WM_MOUSEMOVE   : mouse_act_pos.sx = short( msg.lParam & 0xffff );
                            mouse_act_pos.sy = short( msg.lParam >> 16 );
                            break;
      case WM_LBUTTONDOWN : mouse_left_state = -1;  mouse_down_pos = mouse_act_pos;
							break;
      case WM_LBUTTONUP   : mouse_left_state = 1;   mouse_up_pos = mouse_act_pos;
							break;
	  case WM_RBUTTONDOWN : mouse_right_state = -1;
							break;
      case WM_RBUTTONUP   : mouse_right_state = 1;
							break;
      case WM_QUIT        : return 1;
    }

    TranslateMessage( &msg );
    DispatchMessage( &msg );
  }

  return 0;
}

#endif