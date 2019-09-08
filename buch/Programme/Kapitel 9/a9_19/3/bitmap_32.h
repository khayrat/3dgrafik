#ifndef BITMAP_32_H
#define BITMAP_32_H

#include "simple_types.h"
#include "global_definitions.h"
#include "heightfield.h"

class bitmap_32
{
  public:
    long xscale, yscale;
    pixel_32 *picture;
  
    void load( long xs, long ys );
    void load( char *filename );
    void load( bitmap_32 *b );
    void shadow( heightfield *hf );
    void display( long sx, long sy, pixel_32 *sbuffer );
    
    bitmap_32( void ) : xscale( 0 ), yscale( 0 ), picture( NULL ) { }
    bitmap_32( char *filename ) {  load( filename );  }
    bitmap_32( const bitmap_32 &b ) {  exit_error( "bitmap_32: Aufruf Kopierkonstruktor.\n ");  }
   ~bitmap_32( void ) {  if( picture ) delete [] picture;  }  
};

void bitmap_32::shadow( heightfield *hf )
{
  if( hf->xscale != xscale || hf->yscale != yscale )
    exit_error( "bitmap_32::shadow(): Dimensionen von Bitmap und Höhenfeld sind ungleich.\n" );

  double t = 1.0 / sqrt( light.x*light.x + light.z*light.z );
  vector inv_light = -t * light;

  for( long sy = 0 ; sy < yscale ; sy++ )
     for( long sx = 0 ; sx < xscale ; sx++ )
     {
       double x = sx; double y = sy; double h = hf->heights[ sy * xscale + sx ];
       uchar c = 255;

       while( x >= 0 && x < xscale && y >= 0 && y < yscale )
       {
         long offset = long( y ) * xscale + long( x );

         if( h < hf->heights[ offset ] ) {  c = 0;  break;  }

         x += inv_light.x;  y -= inv_light.z;  h += inv_light.y;
       }

       picture[ sy * xscale + sx ] = pixel_32( c, c, c );
     }
}

void bitmap_32::load( bitmap_32 *b )
{
  xscale = b->xscale;  yscale = b->yscale;

  if( (picture = new pixel_32[ xscale * yscale ]) == NULL )
    exit_error( "*picture: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  memmove( picture, b->picture, xscale*yscale*sizeof( picture[ 0 ] ) );
}

void bitmap_32::load( long xs, long ys )
{
  xscale = xs;  yscale = ys;

  if( (picture = new pixel_32[ xscale * yscale ]) == NULL )
    exit_error( "*picture: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  memset( picture, 0, xscale * yscale * sizeof( picture[ 0 ] ) );
}

void bitmap_32::load( char *filename )
{
  FILE *bmp_file;

  if( (bmp_file = fopen( filename, "rb" )) == NULL ) 
    exit_nofile( "bitmap_32::load()", filename );
  
  fseek( bmp_file, 18, SEEK_SET );
  fread( (char *) &xscale, sizeof( xscale ), 1, bmp_file );
  fread( (char *) &yscale, sizeof( yscale ), 1, bmp_file );

  if( (picture = new pixel_32[ xscale * yscale ]) == NULL )
    exit_error( "*picture: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  long padding = xscale % 4;
     
  long picture_offset;
  fseek( bmp_file, 10, SEEK_SET );
  fread( (char *) &picture_offset, 4, 1, bmp_file );

  fseek( bmp_file, picture_offset, SEEK_SET );

  long x, y, offset;
  for( y=yscale-1 ; y>=0 ; y-- )
  {
    for( x=0 ; x<xscale ; x++ )
    {
      offset = y * xscale + x;	
    	
      fread( &picture[ offset ].blue,  1, 1, bmp_file );
      fread( &picture[ offset ].green, 1, 1, bmp_file );
      fread( &picture[ offset ].red,   1, 1, bmp_file );
    }
    
    fseek( bmp_file, padding, SEEK_CUR );
  }

  fclose( bmp_file );
}

#endif