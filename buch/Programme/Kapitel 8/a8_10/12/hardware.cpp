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

  bitmap_32 first_menue( "first_menue.bmp" );
  bitmap_32 second_menue( "second_menue.bmp" );

  const long key_count = 11;
  button virtual_keys[ key_count ];

  virtual_keys[ 0 ].load( 555,  90, "dec_up.bmp", "dec_down.bmp", "dec_selected.bmp", decrease_blue_radius, 0 );
  virtual_keys[ 1 ].load( 690,  90, "inc_up.bmp", "inc_down.bmp", "inc_selected.bmp", increase_blue_radius, 0 );
  virtual_keys[ 2 ].load( 555, 199, "dec_up.bmp", "dec_down.bmp", "dec_selected.bmp", decrease_speed, 0 );
  virtual_keys[ 3 ].load( 690, 199, "inc_up.bmp", "inc_down.bmp", "inc_selected.bmp", increase_speed, 0 );
  virtual_keys[ 4 ].load( 555, 310, "dec_up.bmp", "dec_down.bmp", "dec_selected.bmp", decrease_blue_mass, 0 );
  virtual_keys[ 5 ].load( 690, 310, "inc_up.bmp", "inc_down.bmp", "inc_selected.bmp", increase_blue_mass, 0 );

  virtual_keys[ 6 ].load( 555, 433, "dec_up.bmp", "dec_down.bmp", "dec_selected.bmp", decrease_yellow_radius, 0 );
  virtual_keys[ 7 ].load( 690, 433, "inc_up.bmp", "inc_down.bmp", "inc_selected.bmp", increase_yellow_radius, 0 );
  virtual_keys[ 8 ].load( 555, 544, "dec_up.bmp", "dec_down.bmp", "dec_selected.bmp", decrease_yellow_mass, 0 );
  virtual_keys[ 9 ].load( 690, 544, "inc_up.bmp", "inc_down.bmp", "inc_selected.bmp", increase_yellow_mass, 0 );

  virtual_keys[ 10 ].load(  31, 544, "end_up.bmp", "end_down.bmp", "end_selected.bmp", end_program, 1 );

  const long marble_count = 50;
  marble m[ marble_count ];
  texture blue;  blue.load( "blue.bmp", pixel_32( 255, 255, 254 ) );
  for( long x=0 ; x<marble_count ; x++ ) m[ x ].load( &blue );

  texture yellow;  yellow.load( "yellow.bmp", pixel_32( 255, 255, 254 ) );
  svertex yellow_pos = svertex( window_pos.sx+window_size/2, window_pos.sy+window_size/2 );

  vector yellow_dir = vector( 0, 0 );

  while( running == 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key != 0 ) break;

    glClear( GL_COLOR_BUFFER_BIT );

    display_window();

    long x;

    for( x=0 ; x<marble_count ; x++ )
    {
      yellow_dir = yellow_dir + m[ x ].update_pos( yellow_pos );
      m[ x ].display();
    }

    yellow_pos = yellow_pos + yellow_dir;
    adjust_pos( yellow_radius, &yellow_pos, &yellow_dir );
    display_texture( &yellow, yellow_pos, yellow_radius );

    first_menue.display( 544, 17 );
    second_menue.display( 544, 360 );

    for( x=0 ; x<key_count ; x++ ) virtual_keys[ x ].display();

    char string[ 500 ];
    sprintf( string, "%lf", blue_radius );
    chars.write( 561, 57, string );

    sprintf( string, "%lf", blue_speed );
    chars.write( 561, 166, string );

    sprintf( string, "%lf", blue_mass );
    chars.write( 561, 277, string );

    sprintf( string, "%lf", yellow_radius );
    chars.write( 561, 400, string );

    sprintf( string, "%lf", yellow_mass );
    chars.write( 561, 511, string );

    screen_interface.swap_buffers();
  }

  return input.msg.wParam;
}
