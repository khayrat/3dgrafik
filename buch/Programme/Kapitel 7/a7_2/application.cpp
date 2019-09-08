#include <windows.h>
#include <time.h>

#include "screen_interface.h"
#include "simple_types.h"
#include "input_controller.h"
#include "bitmap_8.h"

const long speed = 10;  
long begin_xstep = -1, begin_ystep = -1;
long end_xstep = 1, end_ystep = 1;
svertex begin, end;

void draw_line( long frame_counter, bitmap_8 *bmp );
void line_8( long x1, long y1, long x2, long y2, uchar c, uchar *sb, long window_xscale );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow )
{
  screen_interface.open_window( hInstance, 640, 480, 32 );
  pixel_32 *screen = (pixel_32 *) screen_interface.get_screen_pointer();

  bitmap_8 draw_to( "b8_2.bmp" );
  bitmap_8 save( "b8_2.bmp" );

  srand( time( NULL ) );

  begin.sx = rand() % draw_to.xscale;  begin.sy = rand() % draw_to.yscale;
  end.sx   = rand() % draw_to.xscale;  end.sy   = rand() % draw_to.yscale;

  long middle_x = (x_res - draw_to.xscale) / 2;
  long middle_y = (y_res - draw_to.yscale) / 2;

  long frame_counter;
  for( frame_counter = 0 ; ; frame_counter++ )
  {
    if( input.check() == 1 ) break;
    if( input.event_key != 0 ) break;

    for( long x=0 ; x<save.xscale*save.yscale ; x++ )
       draw_to.picture[ x ] = save.picture[ x ];
    
    draw_line( frame_counter, &draw_to );

    draw_to.display( middle_x, middle_y, screen );
  }

  screen_interface.release_screen_pointer();

  return input.msg.wParam;
}

void draw_line( long frame_counter, bitmap_8 *bmp )
{  
  line_8( begin.sx, begin.sy, end.sx, end.sy, 224, bmp->picture, bmp->xscale );
	
  if( frame_counter % speed == 0 )
  {
    begin.sx += begin_xstep;  
    if( begin.sx > bmp->xscale ) {  begin.sx = bmp->xscale-1; begin_xstep = -begin_xstep;  }
    else if( begin.sx < 0 ) {  begin.sx = 0; begin_xstep = -begin_xstep;  }
    
    begin.sy += begin_ystep;  
    if( begin.sy > bmp->yscale ) {  begin.sy = bmp->yscale-1; begin_ystep = -begin_ystep;  }
    else if( begin.sy < 0 ) {  begin.sy = 0; begin_ystep = -begin_ystep;  }
    
    end.sx += end_xstep;  
    if( end.sx > bmp->xscale ) {  end.sx = bmp->xscale-1; end_xstep = -end_xstep;  }
    else if( end.sx < 0 ) {  end.sx = 0; end_xstep = -end_xstep;  }
    
    end.sy += end_ystep;  
    if( end.sy > bmp->yscale ) {  end.sy = bmp->yscale-1; end_ystep = -end_ystep;  }
    else if( end.sy < 0 ) {  end.sy = 0; end_ystep = -end_ystep;  }
  }
}

void line_8( long x1, long y1, long x2, long y2, uchar c, uchar *sb, long window_xscale )
{
  long delta_x, delta_y, e, xstep, ystep, length;
  long offset = y1 * window_xscale + x1;

  delta_x = x2 - x1;  delta_y = y2 - y1;
  xstep = 1;  ystep = window_xscale;

  if( delta_x < 0 ) {  delta_x = -delta_x;  xstep = -xstep; }
  if( delta_y < 0 ) {  delta_y = -delta_y;  ystep = -ystep; }

  if( delta_y > delta_x )
  {
    long t = delta_x;  delta_x = delta_y;  delta_y = t;
    t = xstep;  xstep = ystep;  ystep = t;
  }

  length = delta_x+1;  e = delta_y;

  while( length-- > 0 )
  {
    sb[ offset ] = c;

    offset += xstep;

    e += delta_y;
    if( e >= delta_x )
    {
      e -= delta_x;  offset += ystep;
    }
  }
}
