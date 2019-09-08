#ifndef FONT_H
#define FONT_H

#include <stdio.h>

#include "simple_types.h"
#include "screen_interface.h"
#include "bitmap_32.h"

class char_block
{
  private:
	bitmap_32 *signs;

  public:
    char begin_char, end_char;

    void load( FILE *input, bitmap_32 *font );
    uchar display( ushort *sx, ushort sy, char c, pixel_32 color );

    char_block( void ) {  begin_char = end_char = 0;  signs = NULL;  }
   ~char_block( void ) {  if( signs ) delete [] signs;  }
};

class font
{
  private:
    long cb_count;

    char_block *cb;

  public:
    pixel_32 color;

    void write( ushort sx, ushort sy, char *string );

    font( char *filename, pixel_32 c );
   ~font( void ) {  if( cb ) delete [] cb;  }
} chars( "chars.txt", pixel_32( 0, 0, 0 ) );

uchar char_block::display( ushort *sx, ushort sy, char c, pixel_32 color )
{
  if( begin_char <= c && end_char >= c )
  {
    signs[ c - begin_char ].display( *sx, sy, color, pixel_32( 255, 255, 255 ) );
    *sx += (ushort) signs[ c - begin_char ].xscale + 2;

    return 1;
  }

  return 0;
}

void char_block::load( FILE *input, bitmap_32 *font )
{
  fscanf( input, "%d %d", &begin_char, &end_char );

  ushort char_count = end_char - begin_char + 1;
  if( (signs = new bitmap_32[ char_count ]) == NULL )
    exit_error( "*signs: Nicht genug Speicher.\n" );

  long bx, by, xs, ys;

  for( ushort z=0 ; z<char_count ; z++ )
  {
    fscanf( input, "%ld %ld %ld %ld", &bx, &by, &xs, &ys );
    
    signs[ z ].xscale = xs;
    signs[ z ].yscale = ys;

    if( (signs[ z ].picture = new pixel_32[ xs*ys ]) ==  NULL )
      exit_error( "*picture: Nicht genug Arbeitsspeicher.\n" );

    for( long y=0 ; y<ys ; y++ )
       for( long x=0 ; x<xs ; x++ )
          signs[ z ].picture[ y*xs+x ] = font->picture[ (by+y)*font->xscale + (bx+x) ];
  }
}

void font::write( ushort sx, ushort sy, char *string )
{
  ushort delay = 0;
  ushort length = strlen( string );

  for( ushort x=0 ; x<length ; x++ )
  {
    uchar drawn = 0;

    for( ushort y=0 ; y<cb_count ; y++ )
       if( cb[ y ].display( &sx, sy, string[ x ], color ) ) drawn = 1;
    
    if( drawn == 0 )
      cb[ cb_count-1 ].display( &sx, sy, cb[ cb_count-1 ].begin_char, color );
  }
}

font::font( char *filename, pixel_32 c )
{
  color = c;

  FILE *input = fopen( filename, "rb" );
  if( input == NULL ) exit_nofile( "font::font()", filename );

  char def_name[ 500 ];
  fscanf( input, "%s", def_name );

  bitmap_32 font_def;
  font_def.load( def_name );

  fscanf( input, "%ld", &cb_count );
  if( (cb = new char_block[ cb_count ]) == NULL )
    exit_error( "*cb: Not enough memory.\n" );

  for( long x=0 ; x<cb_count ; x++ )
     cb[ x ].load( input, &font_def );

  fclose( input );
}

#endif