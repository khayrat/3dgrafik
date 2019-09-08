#ifndef PRIMARY_BITMAP
#define PRIMARY_BITMAP

#include "bitmap_32.h"
#include "primary_color.h"

class primary_bitmap
{
  public:
    long xscale, yscale;

    long bitmap_count;
    bitmap_32 *bitmaps;

    primary_bitmap( long bc, char *filename );
   ~primary_bitmap( void ) {  if( bitmaps ) delete [] bitmaps;  }
};

primary_bitmap::primary_bitmap( long bc, char *filename )
{
  long border_count;

  bitmap_32 *borders;
  pixel_32 *border_colors;

  {
    FILE *input = fopen( filename, "rb" );
    if( input == NULL ) exit_nofile( "primary_bitmap::primary_bitmap()", filename );

    fscanf( input, "%ld", &border_count );

    if( (borders = new bitmap_32[ border_count ]) == NULL )
      exit_error( "*borders: Fehler während der Reservierung von Arbeitsspeicher.\n" );
 
    if( (border_colors = new pixel_32[ border_count ]) == NULL )
      exit_error( "*border_colors: Fehler während der Reservierung von Arbeitsspeicher.\n" );

    long x;

    char string[ 100 ];  
    for( x=0 ; x<border_count ; x++ )
    {  
      fscanf( input, "%s", string );
      borders[ x ].load( string );
    }

    fclose( input );

    for( x=1 ; x<border_count ; x++ )
       if( (borders[ 0 ].xscale != borders[ x ].xscale) || (borders[ 0 ].yscale != borders[ x ].yscale) )
         exit_error( "primary_bitmap::primary_bitmap(): Bitmaps müssen über dieselbe Breite und Höhe verfügen.\n" );
  }

  {
    bitmap_count = bc;

    if( (bitmaps = new bitmap_32[ bitmap_count ]) == NULL )
      exit_error( "*bitmaps: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

    bitmaps[ 0 ].load( &borders[ 0 ] );
    xscale = bitmaps[ 0 ].xscale;  yscale = bitmaps[ 0 ].yscale;

    long x;

    for( x=1 ; x<bitmap_count ; x++ ) bitmaps[ x ].load( xscale, yscale );

    for( x = 0 ; x < xscale*yscale ; x++ )
    {
      long y;

      for( y=0 ; y<border_count ; y++ ) border_colors[ y ] = borders[ y ].picture[ x ];
      primary_color palette;  palette.load( bitmap_count, border_count, border_colors );

      for( y=0 ; y<bitmap_count ; y++ )
         bitmaps[ y ].picture[ x ] = palette.components[ y ];
    }
  }

  delete [] borders;
  delete [] border_colors;
}

#endif