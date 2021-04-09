#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "world.h"
#include "vec3.h"

#define FOV 45

typedef struct spaceship spaceship;
typedef struct camera camera;

struct camera {
  vec3 forward, up, right, at;
};

struct spaceship {
  camera cam;
  world* universe;
  int* axes, *buttons;
  int kill;
  cchar_t** pixel_shade_data;
  // cchar_t* pixel_screen;
};

spaceship makeSpaceship(vec3, vec3, vec3, world*);

ray3 getView(camera, double, double);
void yaw(spaceship*, double);
void pitch(spaceship*, double);
void roll(spaceship*, double);
void thrust(spaceship*, double);
void strafe(spaceship*, double);

void* renderSubThread(void*);
void* render(void*);

#endif
