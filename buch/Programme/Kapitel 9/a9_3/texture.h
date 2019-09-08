#ifndef TEXTURE_H
#define TEXTURE_H

#include "simple_types.h"
#include "bitmap_32.h"

class texture : public bitmap_32
{
  private:
    unsigned int bitmap_id;

  public:
    void load( char *filename );
    void activate( void ) {  glBindTexture( GL_TEXTURE_2D, bitmap_id );  }

    texture( void ) : bitmap_id( 0 ) {  bitmap_32();  }
    texture( char *filename ) {  load( filename );  }
};

void texture::load( char *filename )
{
  bitmap_32::load( filename );

  glGenTextures( 1, &bitmap_id );
  activate();
  gluBuild2DMipmaps( GL_TEXTURE_2D, 4, xscale, yscale, GL_BGRA_EXT , GL_UNSIGNED_BYTE, picture );
}

#endif