#include "object.h"
#include "world.h"
#include <math.h>

//vec3 o, double r
double sphereSDF(object_params args, vec3 p) {
  return distance(args.sphere.o, p) - args.sphere.r;
}

//double s
double octahedronSDF(object_params args, vec3 p) {
  p = vec3abs(p);
  double m = p.x + p.y + p.z - args.octahedron.s;
  vec3 q;
       if( 3.0*p.x < m ) q = p;
  else if( 3.0*p.y < m ) q = (vec3) {p.y,p.z,p.x};
  else if( 3.0*p.z < m ) q = (vec3) {p.z,p.x,p.y};
  else return m*0.57735027;

  double k = clamp(0.5*(q.z-q.y+args.octahedron.s),0.0,args.octahedron.s);
  return length((vec3){q.x,q.y-args.octahedron.s+k,q.z-k});
}

//vec3 a, vec3 b, double r
double capsuleSDF(object_params args, vec3 p) {
  vec3 pa = vec3sub(p,args.capsule.a), ba = vec3sub(args.capsule.b,args.capsule.a);

  double h = clamp(dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
  return length(vec3sub(pa, doublemult(ba,h))) - args.capsule.r;
}

double planeSDF(object_params args, vec3 p) {
  return dot(p, args.plane.n) + args.plane.h;
}
