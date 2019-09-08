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

    primary_bitmap( long bc, char *filename, double t );
   ~primary_bitmap( void ) {  if( bitmaps ) delete [] bitmaps;  }
};

primary_bitmap::primary_bitmap( long bc, char *filename, double t )
{
  bitmap_count = bc;

  if( (bitmaps = new bitmap_32[ bitmap_count ]) == NULL )
    exit_error( "*bitmaps: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  bitmaps[ 0 ].load( filename );
  xscale = bitmaps[ 0 ].xscale;  yscale = bitmaps[ 0 ].yscale;

  long x;

  for( x=1 ; x<bitmap_count ; x++ ) bitmaps[ x ].load( xscale, yscale );

  for( x = 0 ; x < xscale*yscale ; x++ )
  {
    uchar red = uchar( bitmaps[ 0 ].picture[ x ].red * t + 0.5 );
    uchar green = uchar( bitmaps[ 0 ].picture[ x ].green * t + 0.5 );
    uchar blue = uchar( bitmaps[ 0 ].picture[ x ].blue * t + 0.5 );
    pixel_32 begin_color( red, green, blue );

    pixel_32 borders[ 2 ] = {  begin_color, bitmaps[ 0 ].picture[ x ]  };
    primary_color palette;  palette.load( bitmap_count, 2, borders );

    for( long y=0 ; y<bitmap_count ; y++ )
       bitmaps[ y ].picture[ x ] = palette.components[ y ];
  }
}

#endif