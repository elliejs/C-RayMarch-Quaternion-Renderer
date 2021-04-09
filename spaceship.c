#define _XOPEN_SOURCE_EXTENDED
#include <ncursesw/ncurses.h>

#include "spaceship.h"

#include <stdlib.h>
#include <wchar.h>
#include <pthread.h>

const wchar_t ascii_ramp[16][2] =
// {" ", "▖", "▗", "▄", "▘", "▌", "▚", "▙", "▝", "▞", "▐", "▟", "▀", "▛", "▜", "█"};
{
  {0x0020, L'\0'}, {0x2596, L'\0'}, {0x2597, L'\0'}, {0x2584, L'\0'},
  {0x2598, L'\0'}, {0x258C, L'\0'}, {0x259A, L'\0'}, {0x2599, L'\0'},
  {0x259D, L'\0'}, {0x259E, L'\0'}, {0x2590, L'\0'}, {0x259F, L'\0'},
  {0x2580, L'\0'}, {0x259B, L'\0'}, {0x259C, L'\0'}, {0x2588, L'\0'}
};

extern WINDOW* frame;
extern WINDOW* typer;

spaceship makeSpaceship(vec3 at, vec3 up, vec3 to, world* world) {
  spaceship s = (spaceship) {
    cam: (camera) {
      forward: normalize(vec3sub(to, at)),
      at: at
    },
    universe: world,
    axes: NULL,
    buttons: NULL,
    kill: 0,
    pixel_shade_data: malloc(sizeof(cchar_t*) * (COLORS - 1)),
    // pixel_screen: malloc(sizeof(cchar_t) * (world->y) * world->x)
  };

  s.cam.right = normalize(cross(s.cam.forward, up));
  s.cam.up = normalize(cross(s.cam.right, s.cam.forward));

  for(int i = 0; i < COLORS - 1; i++) {
    s.pixel_shade_data[i] = malloc(sizeof(cchar_t) * 16);
    for(int j = 0; j < 16; j++) {
      setcchar(&(s.pixel_shade_data[i][j]), ascii_ramp[j], A_NORMAL, i + 1, NULL);
    }
  }

  return s;
}

ray3 getView(camera self, double y_shift, double x_shift) {
  double
    y_rot = deg2rad(y_shift * FOV),
    x_rot = deg2rad(x_shift * FOV);
  return (ray3) {
    self.at,
    rotate(
      rotate(
        self.forward,
        self.right,
        -y_rot),
      self.up,
      x_rot
    )
  };
}

void yaw(spaceship* self, double deg) {
  mrotate(&(self->cam.right), self->cam.up, deg2rad(deg));
  mrotate(&(self->cam.forward), self->cam.up, deg2rad(deg));
}

void pitch(spaceship* self, double deg) {
  mrotate(&(self->cam.up), self->cam.right, deg2rad(deg));
  mrotate(&(self->cam.forward), self->cam.right, deg2rad(deg));
}

void roll(spaceship* self, double deg) {
  mrotate(&(self->cam.up), self->cam.forward, deg2rad(deg));
  mrotate(&(self->cam.right), self->cam.forward, deg2rad(deg));
}

void thrust(spaceship* self, double amt) {
  self->cam.at = vec3add(self->cam.at, doublemult(self->cam.forward, amt));
}

void strafe(spaceship* self, double amt) {
  self->cam.at = vec3add(self->cam.at, doublemult(self->cam.right, amt));
}

void* render(void* args) {
  werase(frame);

  spaceship* self = (spaceship*) args;
  camera cam = self->cam;

  // double u = -(self->universe->x) / (double) self->universe->d,
         // u0 = u,
         double v = -(self->universe->y) / (double) self->universe->d,
         // u_delta = 2. / (double) self->universe->d,
         v_delta = 4. / (double) self->universe->d;


  pthread_t *x_threads = malloc(self->universe->y * sizeof(pthread_t));
  struct _x_thread_args {
    spaceship* self;
    double v;
    int y;
    camera cam;
  } *x_thread_args = malloc(self->universe->y * sizeof(struct _x_thread_args));
  for(int y = 0; y < self->universe->y; y++) {
     x_thread_args[y] = (struct _x_thread_args) {
      self: self,
      v: v,
      y: y,
      cam: cam
    };

    pthread_create(&(x_threads[y]), NULL, renderSubThread, (void*) &x_thread_args[y]);
    v += v_delta;
    // pthread_join(x_threads[y], NULL);
  }

  for(int i = 0; i < self->universe->y; i++) {
    pthread_join(x_threads[i], NULL);
  }

  free(x_threads);
  free(x_thread_args);
  return NULL;
}

void* renderSubThread(void* args) {

  struct args {
    spaceship* self;
    double v;
    int y;
    camera cam;
  } _args = *(struct args*) args;

  const double u_delta = 2. / (double) _args.self->universe->d,
               v_delta = 4. / (double) _args.self->universe->d;
  double u = -(_args.self->universe->x) / (double) _args.self->universe->d;

  const double uvmicro_delta[4][2] = {
    {u_delta * -0.25, v_delta *  0.25},
    {u_delta *  0.25, v_delta *  0.25},
    {u_delta * -0.25, v_delta * -0.25},
    {u_delta *  0.25, v_delta * -0.25}};


  cchar_t* line = malloc(_args.self->universe->x * sizeof(cchar_t));

  for(int x = 0; x < _args.self->universe->x; x++) {
    double specular;
    int index = 0;
    obj_idx hit_obj_idx = -1;

    //LIGHTING
    ray3 r = getView(_args.cam, _args.v, u);
    double dist = march(_args.self->universe, r, &hit_obj_idx);
    if(dist > 0) {
      vec3 hit_pos = vec3add(r.pos, doublemult(r.dir, dist));
      specular = lighting(_args.self->universe, hit_pos, hit_obj_idx);
    }

    for(int s = 0; s < 4; s++) {
      double dist = march(_args.self->universe, getView(_args.cam, _args.v + uvmicro_delta[s][1], u + uvmicro_delta[s][0]), NULL);
      if(dist <= 0) continue;

      // vec3 hit_pos = vec3add(r.pos, doublemult(r.dir, dist));
      // specular += lighting(_args.self->universe, hit_pos, hit_obj_idx);
      // num_hits++;

      index |= 1 << s;
    }

    // if (num_hits) specular /= num_hits;

    // line[x] =
    // num_hits
    //   ? _args.self->pixel_shade_data[
    //     (int) floor(
    //         fmax(
    //           AMBIENT,
    //           fmin(
    //             0.99,
    //             specular
    //           )
    //         ) * (COLORS - 2)
    //       )
    //     ][index]
    //   : _args.self->pixel_shade_data[0][0];
    line[x] = _args.self->pixel_shade_data[(int) floor(specular * (COLORS - 2))][index];

    u += u_delta;
  }

  mvwadd_wchnstr(frame, _args.y, 0, line, _args.self->universe->x);
  free(line);
  return NULL;
}
