#ifndef VEC3_H
#define VEC3_H
#include <math.h>

typedef struct vec3 vec3;
struct vec3 {
  double x,y,z;
};

vec3 rotate(vec3, vec3, double);
void mrotate(vec3*, vec3, double);

double deg2rad(double);
vec3 cross(vec3, vec3);
double dot(vec3, vec3);
double magnitude(vec3);
double distance(vec3, vec3);
double length(vec3);
vec3 normalize(vec3);
void mnormalize(vec3*);

// vector operator*(double a, vector b);
// vector operator*(vector b, double a);
// vector operator+(vector a, vector b);
// vector operator+(double a, vector b);
// vector operator-(vector a, double b);
vec3 doublediv(vec3, double);
vec3 doublemult(vec3, double);
vec3 doublesub(vec3, double);

vec3 vec3sub(vec3, vec3);
vec3 vec3add(vec3, vec3);

vec3 vec3abs(vec3);

double powsum(vec3);
double clamp(double,double,double);

#endif
