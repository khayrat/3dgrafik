#ifndef BITMAP_32_H
#define BITMAP_32_H

#include "simple_types.h"
#include "heightfield.h"

class bitmap_32
{
  public:
    long xscale, yscale;
    pixel_32 *picture;
  
    void load( long xs, long ys );
    void load( heightfield *hf, pixel_32 *pal );

    void shadow( heightfield *hf, vector light );
    void display( long sx, long sy );
    
    bitmap_32( void ) : xscale( 0 ), yscale( 0 ), picture( NULL ) { }
    bitmap_32( long xs, long ys ) {  load( xs, ys );  }
    bitmap_32( const bitmap_32 &b ) {  exit_error( "bitmap_32: Aufruf Kopierkonstruktor.\n ");  }
   ~bitmap_32( void ) {  if( picture ) delete [] picture;  }  
};

void bitmap_32::shadow( heightfield *hf, vector light )
{
  if( hf->xscale != xscale || hf->yscale != yscale )
    exit_error( "bitmap_32::shadow(): Dimensionen von Bitmap und Höhenfeld sind ungleich.\n" );

  double t = 1.0 / sqrt( light.x*light.x + light.z*light.z );
  light = t * light;

  for( long sy = 0 ; sy < yscale ; sy++ )
     for( long sx = 0 ; sx < xscale ; sx++ )
     {
       double x = sx; double y = sy; double h = hf->heights[ sy * xscale + sx ];
       uchar c = 255;

       while( x >= 0 && x < xscale && y >= 0 && y < yscale )
       {
         long offset = long( y ) * xscale + long( x );

         if( h < hf->heights[ offset ] ) {  c = 0;  break;  }

         x += light.x;  y -= light.z;  h += light.y;
       }

       picture[ sy * xscale + sx ] = pixel_32( c, c, c );
     }
}

void bitmap_32::load( long xs, long ys )
{
  xscale = xs;  yscale = ys;

  if( (picture = new pixel_32[ xscale * yscale ]) == NULL )
    exit_error( "*picture: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  memset( picture, 0, xscale * yscale * sizeof( picture[ 0 ] ) );
}

void bitmap_32::load( heightfield *hf, pixel_32 *pal )
{
  xscale = hf->xscale;  yscale = hf->yscale;

  if( (picture = new pixel_32[ xscale * yscale ]) == NULL )
    exit_error( "*picture: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  for( long x=0 ; x<xscale*yscale ; x++ )
     picture[ x ] = pal[ hf->heights[ x ] ];
}

void bitmap_32::display( long sx, long sy )
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
         glColor3ub( picture[ z ].red, picture[ z ].green, picture[ z ].blue );
         glVertex2d( sx+x, sy+y );

         z++;
       }

  glEnd();

  glMatrixMode( GL_PROJECTION );
  glPopMatrix();
}

#endif