#ifndef VERTEX_H
#define VERTEX_H

struct vertex {
  double x, y;

	vertex(void) : x(0.0), y(0.0) {}	
  vertex(double x, double y) : x( x ), y( y ) { }
};

#endif
