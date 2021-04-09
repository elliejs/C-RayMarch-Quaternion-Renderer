#ifndef WORLD_H
#define WORLD_H

#include "ray3.h"
#include "vec3.h"
#include "object.h"

#define MAX_STEPS 1000
#define HIT_DIST 0.001
#define MAX_DIST 300
#define EPSILON 0.001

#define AMBIENT 0.1

typedef struct world world;
struct world {
  int y, x, d;

  vec3* lights;
  object* objects;

  int num_objects, num_lights;
};

world initWorld(int, int, int, int);

double march(world*, ray3, obj_idx*);
double world_dist(world*, vec3, obj_idx*);
double lighting(world*, vec3, obj_idx);
vec3 normal(world*, vec3, obj_idx);

#endif
