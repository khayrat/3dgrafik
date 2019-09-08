#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "bitmap_32.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  if
  (
    MessageBox
    ( 
      NULL, 
      "Dieses Programm schaltet auf Ihrem Bildschirm die Auflösung 1024x768 Pixel mit der Farbtiefe 32 Bit.\nMöchten Sie fortfahren?", "Wichtiger Hinweis:",
      MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2
    )

    == IDNO
  ) return 0;

  // Bei Verkleinerung der Auflösung müssen auch die unten angegebenen Parameter der Funktion 
  // bitmap_32::display() angepasst werden, sonst stürzt das Programm ab

  screen_interface.open_window( hInstance, 1024, 768, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  bitmap_32 bmp( "b24_1.bmp" );
  bitmap_32 bmp_small( "b24_1.bmp" );
  bitmap_32 bmp_large( "b24_1.bmp" );

  bmp_small.resize( 200, 200 );
  bmp_large.resize( 300, 300 );

  pixel_32 *sbuffer;
  if( (sbuffer = new pixel_32[ x_res * y_res ]) == NULL )
    exit_error( "*sbuffer: Fehler bei der Reservierung von Arbeitspeicher.\n" );

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key != 0 ) break;

    memset( sbuffer, 255, x_res * y_res * sizeof( sbuffer[ 0 ] ) );

    bmp_small.display( 37, 280, sbuffer );
    bmp.display( 337, 259, sbuffer );
    bmp_large.display( 677, 234, sbuffer );

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  if( sbuffer ) delete [] sbuffer;
  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}
