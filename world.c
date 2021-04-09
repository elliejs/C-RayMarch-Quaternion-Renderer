#include "world.h"
#include <stdlib.h>
#include <math.h>

world initWorld(int y, int x, int num_objects, int num_lights) {
  world w = (world) {
    y: y, x: x, d: fmax(x,y),

    num_lights: num_lights,
    lights: malloc(num_lights * sizeof(vec3)),

    num_objects: num_objects,
    objects: malloc(num_objects * sizeof(object))
  };

  return w;
}

double march(world* self, ray3 r, obj_idx* hit_obj_idx) {
  double dist = 0;
  for(int i = 0; i < MAX_STEPS; i++) {
    vec3 pos = vec3add(r.pos, doublemult(r.dir, dist));
    double dist_step = world_dist(self, pos, hit_obj_idx);

    dist += dist_step;

    if(dist >= MAX_DIST)
      return 0;
    if(dist_step < HIT_DIST)
      return dist;
  }
  return -1;
}

double world_dist(world* self, vec3 p, obj_idx* hit_obj_idx) {
  double min_dist = MAX_DIST;
  for(int i = 0; i < self->num_objects; i++) {
    double dist = self->objects[i].df(self->objects[i].args, p);
    if(dist < min_dist) {
      min_dist = dist;
      if(hit_obj_idx)
        *hit_obj_idx = i;
    }
  }
  return min_dist;
}

double lighting(world* self, vec3 hit, obj_idx obj_index) {
  vec3 n = normal(self, hit, obj_index);

  double specular = 0;
  for(int i = 0; i < self->num_lights; i++) {
    vec3 safe_hit = vec3add(hit, doublemult(n, HIT_DIST + EPSILON));
      vec3 light_dir_from_surf = normalize(vec3sub(self->lights[i], safe_hit));
    double interfere_dist = march(self, (ray3) {safe_hit, light_dir_from_surf}, NULL);
    double light_dist = distance(safe_hit, self->lights[i]);
    double light_alignment = dot(n, light_dir_from_surf);
    double k = 4;
    if(light_alignment > 0) {
      //normal is at least towards light
      if (interfere_dist > 0
          && interfere_dist < light_dist) {
        //shadow
        specular += AMBIENT * k * interfere_dist / light_dist;
      } else {
        //light
        specular += fmax(AMBIENT, light_alignment);
      }
    } else {
      specular += AMBIENT;
    }
  }
  specular /= (double) self->num_lights;
  return specular;
}

vec3 normal(world* self, vec3 pos, obj_idx index) {
  object_params args = self->objects[index].args;
  dist_func df = self->objects[index].df;
  return normalize((vec3) {
    df(args, (vec3){pos.x + EPSILON, pos.y, pos.z}) -
    df(args, (vec3){pos.x - EPSILON, pos.y, pos.z}),
    df(args, (vec3){pos.x, pos.y + EPSILON, pos.z}) -
    df(args, (vec3){pos.x, pos.y - EPSILON, pos.z}),
    df(args, (vec3){pos.x, pos.y, pos.z + EPSILON}) -
    df(args, (vec3){pos.x, pos.y, pos.z - EPSILON})
  });
}
