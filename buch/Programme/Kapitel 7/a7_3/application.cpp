#include <windows.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "bitmap_8.h"

void rotate_left( bitmap_8 *dest )
{
  bitmap_8 source;  source.load( dest );

  dest->xscale = source.yscale;
  dest->yscale = source.xscale;

  long dx, dy, sx, sy;
  for( dy=0, sx=source.xscale-1 ; dy<dest->yscale ; dy++, sx-- )
  {
    for( dx=0, sy=0 ; dx<dest->xscale ; dx++, sy++ )
       dest->picture[ dy * dest->xscale + dx ] = source.picture[ sy * source.xscale + sx ];
  }
}

void rotate_right( bitmap_8 *dest )
{
  bitmap_8 source; source.load( dest );

  dest->xscale = source.yscale;
  dest->yscale = source.xscale;

  long x, y, a, b;
  for( y=0, a=0 ; y<dest->yscale ; y++, a++ )
  {
    for( x=0, b=source.yscale-1 ; x<dest->xscale ; x++, b-- )
       dest->picture[ y * dest->xscale + x ] = source.picture[ b * source.xscale + a ];
  }
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  message( "Steuerung:", "Horizontale Pfeiltasten:\tRotation der Bitmap\n\nEscape:\t\t\tEnde des Programms ");

  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  bitmap_8 b( "b8_3.bmp" );
  
  long mx = ( x_res - b.xscale ) / 2;
  long my = ( y_res - b.yscale ) / 2;

  pixel_32 *sbuffer;
  if( (sbuffer = new pixel_32[ x_res * y_res ]) == NULL )
    exit_error( "*sbuffer: Fehler bei der Reservierung von Arbeitspeicher.\n" );

  while( 1 )
  {
    if( input.check() == 1 ) break;
    if( input.event_key == VK_ESCAPE ) break;

    if( input.event_key == VK_LEFT ) 
    {
      rotate_left( &b );
      
      mx = ( x_res - b.xscale ) / 2;
      my = ( y_res - b.yscale ) / 2;
    }

    if( input.event_key == VK_RIGHT )
    {
      rotate_right( &b );
      
      mx = ( x_res - b.xscale ) / 2;
      my = ( y_res - b.yscale ) / 2;
    }

    memset( sbuffer, 0, x_res*y_res*sizeof( sbuffer[ 0 ] ) );

    b.display( mx, my, sbuffer );    

    memmove( screen, sbuffer, x_res*y_res*sizeof( screen[ 0 ] ) );
  }

  if( sbuffer ) delete [] sbuffer;
  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}
