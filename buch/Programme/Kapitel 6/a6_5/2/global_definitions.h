#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H

#include "simple_types.h"

const long largest_point_count = 5000;
hvertex wpoint[ largest_point_count+10 ];

const float pr_const = 800;

float z_min = 1;
float z_max = 100000;

vector light( -1, -1, 1 );

#endif