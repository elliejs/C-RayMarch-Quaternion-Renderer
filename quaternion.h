#ifndef QUATERNION_H
#define QUATERNION_H

#include "vec3.h"

typedef struct quaternion quaternion;
struct quaternion {
  double x,y,z,s;
};

quaternion vec3toQ(vec3, double);
vec3 purify(quaternion);
quaternion mult(quaternion, quaternion);
quaternion negate(quaternion);

#endif
