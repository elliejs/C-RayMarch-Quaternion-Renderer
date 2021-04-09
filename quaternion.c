#include "quaternion.h"

quaternion vec3toQ(vec3 xyz, double s) {
  return (quaternion) {
    xyz.x,
    xyz.y,
    xyz.z,
    s
  };
}

vec3 purify(quaternion a) {
  return (vec3) {
    a.x,
    a.y,
    a.z
  };
}
quaternion mult(quaternion a, quaternion b) {
  double
    a_s = a.s,
    b_s = b.s;
  vec3
    a_v = purify(a),
    b_v = purify(b);

  return vec3toQ(
    vec3add(
      vec3add(
        doublemult(b_v, a_s),
        doublemult(a_v, b_s)
      ),
      cross(a_v, b_v)
    ),                              //vect
    (a_s * b_s) - dot(a_v, b_v)     //scalar
  );
}

quaternion negate(quaternion a) {
  return (quaternion) {
    -a.x,
    -a.y,
    -a.z,
    a.s
  };
}
