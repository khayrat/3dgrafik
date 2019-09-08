#ifndef BITMAP_8_H
#define BITMAP_8_H

#include "simple_types.h"

class bitmap_8
{
  private:	
    pixel_32 palette[ 256 ];

  public:
    long xscale, yscale;
    uchar *picture;
    
    void load( char *filename );
    void display( long sx, long sy, pixel_32 *sbuffer );
    
    bitmap_8( void ) : xscale( 0 ), yscale( 0 ), picture( NULL ) { }
    bitmap_8( char *filename ) {  load( filename );  }
    bitmap_8( const bitmap_8 &b ) {  exit_error( "bitmap_8: Aufruf Kopierkonstruktor.\n ");  }
   ~bitmap_8( void ) {  if( picture ) delete [] picture;  }  
};

void bitmap_8::load( char *filename )
{
  FILE *bmp_file;

  if( (bmp_file = fopen( filename, "rb" )) == NULL ) 
    exit_nofile( "bitmap_8::load()", filename );
  
  fseek( bmp_file, 18, SEEK_SET );
  fread( (char *) &xscale, sizeof( xscale ), 1, bmp_file );
  fread( (char *) &yscale, sizeof( yscale ), 1, bmp_file );

  if( (picture = new uchar[ xscale * yscale ]) == NULL )
    exit_error( "*picture: Nicht genug Arbeitsspeicher.\n" );

  long padding;
  for( padding=0 ; ; padding++ ) 
     if( (xscale + padding) % 4 == 0 ) break;

  long picture_offset;
  fseek( bmp_file, 10, SEEK_SET );
  fread( (char *) &picture_offset, 4, 1, bmp_file );

  fseek( bmp_file, picture_offset, SEEK_SET );
  for( long y=yscale-1 ; y>=0 ; y-- )
  {
    for( long x=0 ; x<xscale ; x++ ) 
       fread( &picture[ y * xscale + x ], sizeof( picture[ 0 ] ), 1, bmp_file );

    fseek( bmp_file, padding, SEEK_CUR );
  }  

  fseek( bmp_file, 54, SEEK_SET );
  fread( (char *) palette, sizeof( palette ), 1, bmp_file );

  fclose( bmp_file );
}

void bitmap_8::display( long sx, long sy, pixel_32 *sbuffer )
{
  long x, y, z;
  long offset = sy * x_res + sx;

  for( y=0, z=0 ; y<yscale ; y++ )
  {
    for( x=0 ; x<xscale ; x++ ) sbuffer[ offset++ ] = palette[ picture[ z++ ] ];

    offset += x_res - xscale;
  }
}

#endif