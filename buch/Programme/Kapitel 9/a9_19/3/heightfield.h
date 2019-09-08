#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include <time.h>

#include "simple_types.h"
#include "screen_interface.h"

class heightfield
{
  private:
    long grad;

    void divide( long xl, long xr, long level );
    void divide( long xt, long yt, long xb, long yb, long level );
    long random_color( long xl, long yl, long xm, long ym, long xr, long yr, long level );

  public:
    long xscale, yscale;
    uchar *heights;

    void plasma_fractal( long g );
    void seamless_plasma( long g );
    void enlarge( long sight_radius );
    void display( long sx, long sy, pixel_32 *pal, pixel_32 *sbuffer );

    heightfield( void ) : xscale( 0 ), yscale( 0 ), heights( NULL ) { }
    heightfield( long xs, long ys );
    heightfield( const heightfield &h ) {  exit_error( "heightfield: Aufruf Kopierkonstruktor.\n" );  }
   ~heightfield( void ) {  if( heights ) delete [] heights;  }
};

void heightfield::enlarge( long sight_radius )
{
  if( xscale != yscale )
    exit_error( "heightfield::enlarge() ist lediglich für quadratische Höhenfelder definiert.\n" );

  long updated_xscale = xscale + 2 * sight_radius;

  uchar *enlarged_heights = new uchar[ updated_xscale * updated_xscale ];
  if( enlarged_heights == NULL ) 
    exit_error( "*enlarged_heights: Fehler während der Reservierung von Arbeitsspeicher.\n" );

  long max_border = xscale * xscale - 2 - xscale;
  long act_border = max_border - (sight_radius - 1) * xscale;
  long act_offset = act_border - (sight_radius - 1);
 
  long z = 0;
  for( long y=0 ; y<updated_xscale ; y++ )
  {
    act_offset = act_border - (sight_radius - 1);	
  	
    for( long x=0 ; x<updated_xscale ; x++ )
    {
      enlarged_heights[ z++ ] = heights[ act_offset ];
     
      act_offset++;
      if( act_offset > act_border ) act_offset -= xscale - 1;
    }
    
    act_border += xscale;
    if( act_border > max_border ) act_border -= (xscale - 1) * xscale;
  }

  xscale = yscale = updated_xscale;
  delete [] heights;
  heights = enlarged_heights;
}

heightfield::heightfield( long xs, long ys )
{
  xscale = xs;  yscale = ys;

  if( (heights = new uchar[ xscale * yscale ]) == NULL )
    exit_error( "*heights: Fehler bei der Reservierung von Arbeitsspeicher.\n" );
}

void heightfield::seamless_plasma( long g )
{
  memset( heights, 0, xscale * yscale * sizeof( heights[ 0 ] ) );

  srand( time( NULL ) );
  uchar act_h = rand() % 256;

  heights[ 0 ] = act_h;
  heights[ xscale - 1 ] = act_h;
  heights[ xscale * yscale - 1 ] = act_h;
  heights[ (yscale-1) * xscale ] = act_h;

  grad = g;

  divide( 0, xscale-1, 0 );

  long x;

  for( x=1 ; x<xscale-1 ; x++ )
  {
    heights[ x*xscale ] = heights[ x ];
    heights[ x*xscale + (xscale-1) ] = heights[ x ];

    heights[ x ] = 0;
  }

  divide( 0, xscale-1, 0 );

  for( x=1 ; x<xscale-1 ; x++ )
     heights[ (yscale-1)*xscale + x ] = heights[ x ];

  divide( 0, 0, xscale-1, yscale-1, 0 );
}

void heightfield::plasma_fractal( long g )
{
  memset( heights, 0, xscale * yscale * sizeof( heights[ 0 ] ) );

  heights[ 0 ] = rand() % 256;
  heights[ xscale - 1 ] = rand() % 256;
  heights[ xscale * yscale - 1 ] = rand() % 256;
  heights[ (yscale-1) * xscale ] = rand() % 256;

  grad = g;

  divide( 0, 0, xscale-1, yscale-1, 0 );
}

void heightfield::divide( long xl, long xr, long level )
{
  if( xr - xl < 2 ) return;

  long x = (xl + xr) / 2;

  long act_h;

  act_h = random_color( xl, 0, x, 0, xr, 0, level );
  if( act_h < 0 ) act_h = 0;  if( act_h > 255 ) act_h = 255;
  if( heights[ x ] == 0 ) heights[ x ] = uchar( act_h );

  divide( xl, x, level + 1 );
  divide( x, xr, level + 1 );
}

void heightfield::divide( long xt, long yt, long xb, long yb, long level )
{
  if( (xb - xt < 2) && (yb - yt < 2) ) return;

  long x = (xt + xb) / 2;
  long y = (yt + yb) / 2;

  long act_h;

  act_h = random_color( xt, yt, x, yt, xb, yt, level );
  act_h = act_h + random_color( xb, yt, xb, y, xb, yb, level );
  act_h = act_h + random_color( xt, yb, x, yb, xb, yb, level );
  act_h = act_h + random_color( xt, yt, xt, y, xt, yb, level );

  act_h = act_h / 4;
  if( act_h < 0 ) act_h = 0;  if( act_h > 255 ) act_h = 255;
  if( heights[ y * xscale + x ] == 0 ) heights[ y * xscale + x ] = uchar( act_h );

  divide( xt, yt, x, y, level + 1 );
  divide( x, yt, xb, y, level + 1 );
  divide( x, y, xb, yb, level + 1 );
  divide( xt, y, x, yb, level + 1 );
}

long heightfield::random_color( long xl, long yl, long xm, long ym, long xr, long yr, long level )
{
  long act_h;

  act_h = heights[ ym * xscale + xm ];
  if( act_h == 0 )
  {
    act_h = long( (rand() % grad) >> level );
    if( rand() % 2 ) act_h = -act_h;

    act_h = ( (heights[ yl * xscale + xl ] + heights[ yr * xscale + xr ]) / 2 ) + act_h;
    if( act_h < 1 ) act_h = 1;  if( act_h > 255 ) act_h = 255;
    heights[ ym * xscale + xm ] = uchar( act_h );
  }

  return act_h;
}

void heightfield::display( long sx, long sy, pixel_32 *pal, pixel_32 *sbuffer )
{
  long x, y, z;
  long offset = sy * x_res + sx;

  for( y=0, z=0 ; y<yscale ; y++ )
  {
    for( x=0 ; x<xscale ; x++ ) 
    {
      sbuffer[ offset ] = pal[ heights[ z ] ];

      offset++;  z++;
    }

    offset += x_res - xscale;
  }  
}

#endif