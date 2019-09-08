#ifndef SVERTEX_H
#define SVERTEX_H

struct svertex {
  long x, y;

	svertex(void) : x(0), y(0) {}	
  svertex(long x, long y) : x( x ), y( y ) { }
};

#endif
