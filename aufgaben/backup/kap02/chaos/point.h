#ifndef POINT_H
#define POINT_H

#include "simple_types.h"

typedef unsigned short ushort;

struct point {
  ushort y, x;

	point(void) : y(0), x(0) {}	
  point(ushort y, ushort x) : x( x ), y( y ) { }
};

#endif
