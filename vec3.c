#include "vec3.h"
#include "quaternion.h"

vec3 rotate(vec3 point, vec3 axis, double rad) {
  rad /= 2.;
  quaternion q = vec3toQ(doublemult(axis, sin(rad)), cos(rad));
  quaternion p = vec3toQ(point, 0.);
  quaternion r = mult(mult(q, p), negate(q));
  return purify(r);
}

void mrotate(vec3* point, vec3 axis, double deg) {
  *point = rotate(*point, axis, deg);
}

double deg2rad(double a) {
  return (M_PI / 180.) * a;
}

vec3 cross(vec3 a, vec3 b) {
  return (vec3) {
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  };
}

double dot(vec3 a, vec3 b) {
  return
    a.x * b.x +
    a.y * b.y +
    a.z * b.z;
}

double magnitude(vec3 a) {
  return sqrt(powsum(a));
}

double powsum(vec3 a) {
  return
    pow(a.x, 2) +
    pow(a.y, 2) +
    pow(a.z, 2);
}


double distance(vec3 a, vec3 b) {
  return magnitude(vec3sub(b, a));
}

double length(vec3 a) {
  return distance(a, (vec3) {0,0,0});
}

vec3 vec3abs(vec3 a) {
  return (vec3) {
    fabs(a.x),
    fabs(a.y),
    fabs(a.z)
  };
}

//probably just slow at this point... sad :(
// double fi_sqrt(double a) {
// 	long i;
// 	double x2, y;
// 	const double threehalfs = 1.5F;
//
// 	x2 = a * 0.5F;
// 	y  = a;
// 	i  = * ( long * ) &y;                       // evil floating point bit level hacking
// 	i  = 0x5fe6eb50c7b537a9 - ( i >> 1 );       // what the fuck?
// 	y  = * ( double * ) &i;
// 	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
// //	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
//
// 	return y;
// }

vec3 normalize(vec3 a) {
  // return doublemult(a, fi_sqrt(powsum(a)));
  return doublediv(a, magnitude(a));
}

void mnormalize(vec3* a) {
  double inv_sqrt = sqrt(powsum(*a));
  a->x /= inv_sqrt;
  a->y /= inv_sqrt;
  a->z /= inv_sqrt;
}

vec3 doublediv(vec3 a, double b) {
  return (vec3) {
    a.x / b,
    a.y / b,
    a.z / b
  };
}

vec3 doublemult(vec3 a, double b) {
  return (vec3) {
    a.x * b,
    a.y * b,
    a.z * b
  };
}

vec3 doublesub(vec3 a, double b) {
  return (vec3) {
    a.x - b,
    a.y - b,
    a.z - b
  };
}

vec3 vec3sub(vec3 a, vec3 b) {
  return (vec3) {
    a.x - b.x,
    a.y - b.y,
    a.z - b.z
  };
}

vec3 vec3add(vec3 a, vec3 b) {
  return (vec3) {
    b.x + a.x,
    b.y + a.y,
    b.z + a.z
  };
}

double clamp(double z, double a, double b) {
  return fmin(fmax(z, a), b);
}
