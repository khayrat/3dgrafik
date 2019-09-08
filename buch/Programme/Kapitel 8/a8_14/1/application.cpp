#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "primary_bitmap.h"
#include "font.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Horizontale Pfeiltasten:\tVeränderung der aktuellen Position\n\t\t\tinnerhalb des Bitmapverlaufes\n\nEscape:\t\t\tEnde des Programms\n ");

  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();
  
  pixel_32 *sbuffer = new pixel_32[ x_res*y_res ];

  if( sbuffer == NULL )
    exit_error( "*sbuffer: Fehler während der Reservierung von Arbeitsspeicher.\n" );

  primary_bitmap adv_surface( 64, "metal.bmp", 0.35 );

  long act_bitmap = 0;

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key == VK_ESCAPE ) break;

    if( input.event_key == VK_LEFT )
      if( --act_bitmap < 0 ) act_bitmap = 0;

    if( input.event_key == VK_RIGHT )
      if( ++act_bitmap > adv_surface.bitmap_count-1 )
        act_bitmap = adv_surface.bitmap_count-1;

    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    adv_surface.bitmaps[ adv_surface.bitmap_count-1 ].display(  45, 150, sbuffer );
    adv_surface.bitmaps[ act_bitmap ].display( 340, 150, sbuffer );

    chars.write( 50, 100, "originalbitmap", sbuffer );
    char string[ 100 ];
    sprintf( string, "bitmap an der position %ld", act_bitmap );
    chars.write( 345, 100, string, sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  if( sbuffer ) delete [] sbuffer;

  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}

