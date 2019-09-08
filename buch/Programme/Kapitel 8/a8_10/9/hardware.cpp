#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <time.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "global_definitions.h"
#include "input_controller.h"
#include "texture.h"
#include "marble.h"
#include "button.h"
#include "font.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  initialise_world( hInstance, 800, 600, 32 );

  bitmap_32 first_menue( "menue.bmp" );

  const long key_count = 7;
  button virtual_keys[ key_count ];

  virtual_keys[ 0 ].load( 555,  90, "dec_up.bmp", "dec_down.bmp", "dec_selected.bmp", decrease_blue_radius, 0 );
  virtual_keys[ 1 ].load( 690,  90, "inc_up.bmp", "inc_down.bmp", "inc_selected.bmp", increase_blue_radius, 0 );
  virtual_keys[ 2 ].load( 555, 199, "dec_up.bmp", "dec_down.bmp", "dec_selected.bmp", decrease_speed, 0 );
  virtual_keys[ 3 ].load( 690, 199, "inc_up.bmp", "inc_down.bmp", "inc_selected.bmp", increase_speed, 0 );

  virtual_keys[ 4 ].load( 555, 310, "dec_up.bmp", "dec_down.bmp", "dec_selected.bmp", decrease_color, 1 );
  virtual_keys[ 5 ].load( 690, 310, "inc_up.bmp", "inc_down.bmp", "inc_selected.bmp", increase_color, 1 );
  virtual_keys[ 6 ].load(  31, 544, "end_up.bmp", "end_down.bmp", "end_selected.bmp", end_program, 1 );

  const long marble_count = 50;
  marble m[ marble_count ];
  for( long x=0 ; x<marble_count ; x++ ) m[ x ].load();

  while( running == 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key != 0 ) break;

	glClear( GL_COLOR_BUFFER_BIT );

    display_window();

    long x;

    for( x=0 ; x<marble_count ; x++ )
    {
      m[ x ].update_pos();
      m[ x ].display();
    }

    first_menue.display( 544, 17 );

    for( x=0 ; x<key_count ; x++ ) virtual_keys[ x ].display();

    char string[ 500 ];
    sprintf( string, "%lf", blue_radius );
    chars.write( 561, 57, string );

    sprintf( string, "%lf", blue_speed );
    chars.write( 561, 166, string );

    sprintf( string, "%ld", act_color );
    chars.write( 561, 277, string );

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}

