#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <windows.h>
#include "simple_types.h"

class input_controller
{
  public:
	long event_key;

    MSG msg;

    long check( void );
	uchar key_pressed( int key ) {  return GetKeyState( key ) < 0;  }
} input;

long input_controller::check( void )
{
  event_key = 0;

  if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
  {
    switch( msg.message )
	{
	  case WM_KEYDOWN     : event_key = msg.wParam;
                            break;
      case WM_QUIT        : return 1;
    }

    TranslateMessage( &msg );
    DispatchMessage( &msg );
  }

  return 0;
}

#endif