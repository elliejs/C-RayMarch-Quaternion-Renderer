#ifndef OBJECT_H
#define OBJECT_H

#include "vec3.h"

typedef union object_params object_params;
typedef struct object object;

struct sphere     {vec3 o; double r;};
struct octahedron {double s;};
struct capsule    {vec3 a, b; double r;};
struct plane      {vec3 n; double h;};

union object_params {
  struct sphere sphere;
  struct octahedron octahedron;
  struct capsule capsule;
  struct plane plane;
};

typedef double(*dist_func)(object_params, vec3);

struct object {
  dist_func df;
  object_params args;
};

typedef int obj_idx;

double sphereSDF(object_params, vec3);
double octahedronSDF(object_params, vec3);
double capsuleSDF(object_params, vec3);
double planeSDF(object_params, vec3);

#endif
