#ifndef TEXTURE_H
#define TEXTURE_H

#include "simple_types.h"
#include "bitmap_32.h"
#include "heightfield.h"
#include "primary_bitmap.h"

primary_bitmap bmp( 64, "texture_lib.txt" );

class texture
{
  private:
    bitmap_32 surface;
    unsigned int bitmap_id;

  public:
    void load( long count, heightfield *hf );
    void activate( void ) {  glBindTexture( GL_TEXTURE_2D, bitmap_id );  }

    texture( void ) : bitmap_id( 0 ) { }
};

void texture::load( long rcount, heightfield *hf )
{
  surface.load( rcount*bmp.xscale, rcount*bmp.yscale );

  for( long y=0 ; y<surface.yscale ; y++ )
     for( long x=0 ; x<surface.xscale ; x++ )
     {
       long tx = x % bmp.xscale;
       long ty = y % bmp.yscale;
       long offset = ty * bmp.xscale + tx;

       long hx = long( (double( x ) / (surface.xscale-1)) * (hf->xscale-1) + 0.5 );
       long hy = long( (double( y ) / (surface.yscale-1)) * (hf->yscale-1) + 0.5 );
       long h = hf->heights[ hy * hf->xscale + hx ];
       h = long( (h / 255.0) * (bmp.bitmap_count-1) + 0.5 );

       surface.picture[ y * surface.xscale + x ] = bmp.bitmaps[ h ].picture[ offset ];
     }

  glGenTextures( 1, &bitmap_id );
  activate();
  gluBuild2DMipmaps( GL_TEXTURE_2D, 4, surface.xscale, surface.yscale, GL_BGRA_EXT , GL_UNSIGNED_BYTE, surface.picture );
}

#endif