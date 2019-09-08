#ifndef ST_BITMAP_H
#define ST_BITMAP_H

#include "simple_types.h"
#include "bitmap_8.h"

class st_bitmap
{
  public:
    long xscale, yscale;
    pixel_32 *picture;
    uchar *st_map;
  
    void load( st_bitmap *b );
    void load( char *picture_name, char *map_name );

    void draw_partial( long picture_offset, long x_length, long y_length, long screen_offset, pixel_32 *sbuffer );

    st_bitmap( void ) : xscale( 0 ), yscale( 0 ), picture( NULL ), st_map( NULL ) { }
    st_bitmap( char *picture_name, char *map_name ) {  load( picture_name, map_name );  }
    st_bitmap( const st_bitmap &b ) {  exit_error( "st_bitmap: Aufruf Kopierkonstruktor.\n" );  }
   ~st_bitmap( void ) {  if( picture ) delete [] picture;  if( st_map ) delete [] st_map;  }
};


pixel_32 get_color( pixel_32 begin, pixel_32 end, long t )
{
  double r = (double( end.red ) - begin.red ) / 255.0 * t + begin.red;
  double g = (double( end.green ) - begin.green ) / 255.0 * t + begin.green;	
  double b = (double( end.blue ) - begin.blue ) / 255.0 * t + begin.blue;
  
  return pixel_32( uchar( r + 0.5 ), uchar( g + 0.5 ), uchar( b + 0.5 ) );
}

void st_bitmap::draw_partial( long picture_offset, long x_length, long y_length, long screen_offset, pixel_32 *sbuffer )
{
  long x, y, t;
  pixel_32 begin_color, end_color;

  for( y=0 ; y<y_length ; y++ )
  {
    for( x=0 ; x<x_length ; x++ )
    {
      begin_color = sbuffer[ screen_offset ];
      end_color = picture[ picture_offset ];
      t = st_map[ picture_offset ];
       	
      sbuffer[ screen_offset ] = get_color( begin_color, end_color, t );

      screen_offset++;  picture_offset++;
    }

    screen_offset += x_res - x_length;
    picture_offset += xscale - x_length;
  }	
}

void st_bitmap::load( st_bitmap *b )
{
  xscale = b->xscale;  yscale = b->yscale;

  long x;

  if( (picture = new pixel_32[ xscale * yscale ]) == NULL )
    exit_error( "*picture: Fehler bei der Reservierung von Arbeitsspeicher.\n" );
  for( x=0 ; x<xscale*yscale ; x++ )
     picture[ x ] = b->picture[ x ];
     
  if( (st_map = new uchar[ xscale * yscale ]) == NULL )
    exit_error( "bmp_32::bmp_32( const bmp & ): Fehler bei der Reservierung von Arbeitsspeicher.\n" );    
  for( x=0 ; x<xscale*yscale ; x++ )
     st_map[ x ] = b->st_map[ x ];
}

void st_bitmap::load( char *picture_name, char *map_name )
{
  bitmap_8 map( map_name );
  
  if( (st_map = new uchar[ map.xscale * map.yscale ]) == NULL )
    exit_error( "*st_map: Fehler bei der Reservierung von Arbeitsspeicher.\n" );
  
  for( long x=0 ; x<map.xscale*map.yscale ; x++ )
     st_map[ x ] = map.palette[ map.picture[ x ] ].green;

  FILE *bmp_file;

  if( (bmp_file = fopen( picture_name, "rb" )) == NULL ) 
    exit_nofile( "bitmap_32::load()", picture_name );
  
  fseek( bmp_file, 18, SEEK_SET );
  fread( (char *) &xscale, sizeof( xscale ), 1, bmp_file );
  fread( (char *) &yscale, sizeof( yscale ), 1, bmp_file );

  if( xscale != map.xscale || yscale != map.yscale )
    exit_error( "st_bitmap::load(): Bitmap und dazugehörige Transparence Map müssen dieselbe Länge und Breite besitzen.\n" );

  if( (picture = new pixel_32[ xscale * yscale ]) == NULL )
    exit_error( "*picture: Fehler bei der Reservierung von Arbeitsspeicher.\n" );

  long padding = xscale % 4;
     
  long picture_offset;
  fseek( bmp_file, 10, SEEK_SET );
  fread( (char *) &picture_offset, 4, 1, bmp_file );

  fseek( bmp_file, picture_offset, SEEK_SET );

  long y, offset;
  for( y=yscale-1 ; y>=0 ; y-- )
  {
    for( long x=0 ; x<xscale ; x++ )
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