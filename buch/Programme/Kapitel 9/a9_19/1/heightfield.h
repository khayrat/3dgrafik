#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include "simple_types.h"
#include "screen_interface.h"

class heightfield
{
  private:
    long grad;

    void divide( long xt, long yt, long xb, long yb, long level );
    long random_color( long xl, long yl, long xm, long ym, long xr, long yr, long level );

  public:
    long xscale, yscale;
    uchar *heights;

    void plasma_fractal( long g );
    void display( long sx, long sy, pixel_32 *pal );

    heightfield( void ) : xscale( 0 ), yscale( 0 ), heights( NULL ) { }
    heightfield( long xs, long ys );
    heightfield( const heightfield &h ) {  exit_error( "heightfield: Aufruf Kopierkonstruktor.\n" );  }
   ~heightfield( void ) {  if( heights ) delete [] heights;  }
};

heightfield::heightfield( long xs, long ys )
{
  xscale = xs;  yscale = ys;

  if( (heights = new uchar[ xscale * yscale ]) == NULL )
    exit_error( "*heights: Fehler bei der Reservierung von Arbeitsspeicher.\n" );
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

    if( act_h > 255 ) {  act_h = act_h % 256;  act_h = 255 - act_h;  }
    if( act_h < 1 ) {  act_h = act_h % 256;  act_h = -act_h + 1;  }

    heights[ ym * xscale + xm ] = uchar( act_h );
  }

  return act_h;
}

void heightfield::display( long sx, long sy, pixel_32 *pal )
{
  glMatrixMode( GL_PROJECTION );
  glPushMatrix();

  glLoadIdentity();
  gluOrtho2D( 0, x_res, y_res, 0 );

  glPointSize( 1 );
  glBegin( GL_POINTS );

    long x, y, z;

    for( y=0, z=0 ; y<yscale ; y++ )
       for( x=0 ; x<xscale ; x++ ) 
       {
         glColor3ub( pal[ heights[ z ] ].red, pal[ heights[ z ] ].green, pal[ heights[ z ] ].blue );
         glVertex2d( sx+x, sy+y );

         z++;
       }

  glEnd();

  glMatrixMode( GL_PROJECTION );
  glPopMatrix();
}

#endif