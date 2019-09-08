#ifndef BITMAP_32_H
#define BITMAP_32_H

#include "simple_types.h"

class bitmap_32
{
  public:
    long xscale, yscale;
    pixel_32 *picture;
  
    void load( char *filename );
    void display( long sx, long sy, pixel_32 *sbuffer );
    
    bitmap_32( void ) : xscale( 0 ), yscale( 0 ), picture( NULL ) { }
    bitmap_32( char *filename ) {  load( filename );  }
    bitmap_32( const bitmap_32 &b ) {  exit_error( "bitmap_32: Aufruf Kopierkonstruktor.\n ");  }
   ~bitmap_32( void ) {  if( picture ) delete [] picture;  }  
};


void bitmap_32::display( long sx, long sy, pixel_32 *sbuffer )
{
  long x, y, z;
  long offset = sy * x_res + sx;

  for( y=0, z=0 ; y<yscale ; y++ )
  {
    for( x=0 ; x<xscale ; x++ ) sbuffer[ offset++ ] = picture[ z++ ];

    offset += x_res - xscale;
  }  
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