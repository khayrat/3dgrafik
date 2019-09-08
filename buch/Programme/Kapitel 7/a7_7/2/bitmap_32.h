#ifndef BITMAP_32_H
#define BITMAP_32_H

#include "simple_types.h"

class bitmap_32
{
  public:
    long xscale, yscale;
    pixel_32 *picture;
    pixel_32 tr;
  
    void load( char *filename, pixel_32 t );
    void load( bitmap_32 *b );

    void display( long sx, long sy, pixel_32 *sbuffer );
    void draw_partial( long picture_offset, long x_length, long y_length, long screen_offset, pixel_32 *sbuffer );

    bitmap_32( void ) : xscale( 0 ), yscale( 0 ), picture( NULL ) { }
    bitmap_32( char *filename, pixel_32 t ) {  load( filename, t );  }
    bitmap_32( const bitmap_32 &b ) {  exit_error( "bitmap_32: Aufruf Kopierkonstruktor.\n ");  }
   ~bitmap_32( void ) {  if( picture ) delete [] picture;  }  
};

void bitmap_32::display( long sx, long sy, pixel_32 *sbuffer )
{
  long x, y, z;
  long offset = sy * x_res + sx;

  for( y=0, z=0 ; y<yscale ; y++ )
  {
    for( x=0 ; x<xscale ; x++ ) 
    {
      if( picture[ z ] != tr ) sbuffer[ offset ] = picture[ z ];

      offset++;  z++;
    }

    offset += x_res - xscale;
  }  
}

void bitmap_32::draw_partial( long picture_offset, long x_length, long y_length, long screen_offset, pixel_32 *sbuffer )
{
  long x, y;
  
  for( y=0 ; y<y_length ; y++ )
  {
    for( x=0 ; x<x_length ; x++ )
    {
       if( picture[ picture_offset ] != tr )
         sbuffer[ screen_offset ] = picture[ picture_offset ];
         
      screen_offset++;  picture_offset++;
    }

    screen_offset += x_res - x_length;
    picture_offset += xscale - x_length;
  }	
}

void bitmap_32::load( bitmap_32 *b )
{
  xscale = b->xscale;  yscale = b->yscale;
  tr = b->tr;
  
  if( (picture = new pixel_32[ xscale * yscale ]) == NULL )
    exit_error( "bmp_32::bmp_32( const bmp & ): Fehler bei der Reservierung von Arbeitsspeicher.\n" );
    
  for( long x=0 ; x<xscale*yscale ; x++ )
     picture[ x ] = b->picture[ x ];
}

void bitmap_32::load( char *filename, pixel_32 t )
{
  tr = t;

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
}

#endif