#ifndef VERTEX_H
#define VERTEX_H

#include "simple_types.h"

typedef unsigned short ushort;

struct vertex {
  ushort x, y;

	vertex(void) : x(0), y(0) {}	
  vertex(ushort x, ushort y) : x( x ), y( y ) { }
};

#endif
