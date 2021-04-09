#define _XOPEN_SOURCE_EXTENDED
#include <ncursesw/ncurses.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <locale.h>

#include "spaceship.h"
#include "drivers.h"
// #include "gfx.h"

#define FPS 1000 / (double) 60

WINDOW *quitter, *frame, *typer;

void init_windows(void) {
  quitter = newwin(1, COLS, 0, 0);  //top window for header. Turn info errors etc...
  frame   = newwin(LINES - 2, COLS, 1, 0);  //middle window for the board and such
  typer   = newwin(1, COLS, LINES - 1, 0);
}

void init_ncurses(void) {

  initscr();              //init ncurses for GUI library
  cbreak();                 //ctrl+C won't save you
  halfdelay(2);

  noecho();         //closer control over how characters typed get displayed

  keypad(typer, TRUE);   //get arrows and F keys
  // nodelay(typer, TRUE);  //nonblocking getch()

  start_color();
  // use_default_colors();
  init_color(0, 0, 0, 0);

  double shade_step = 999. / (double) (COLORS - 2);
  for(int i = 1; i < COLORS; i++) {
    short del = shade_step * (i-1);
    init_color(i, del, del, del);
    init_pair(i, i, COLOR_BLACK);
  }
  init_windows();
}

int main(int argc, char const *argv[]) {
  setlocale(LC_ALL, "");
  srand(time(NULL));
  init_ncurses();

  world w = initWorld(LINES-2, COLS, 3, 1);
  // for(int i = 0; i < w.num_objects; i++) {
  //   w.objects[0] =
  //     (object) {
  //       df: capsuleSDF,
  //       args: (object_params) (struct capsule) {(vec3) {-4,-1,0}, (vec3) {-6,-1,5}, 2}
  //     };
  // // }
  //
  // w.objects[1] =
  //   (object) {
  //     df: octahedronSDF,
  //     args: (object_params) (struct octahedron) {2}
  //   };
  w.objects[0] =
    (object) {
      df: sphereSDF,
      args: (object_params) (struct sphere) {(vec3) {0,0,0}, 2}
    };
// }

  w.objects[1] =
  (object) {
    df: sphereSDF,
    args: (object_params) (struct sphere) {(vec3){4, 2, 0,}, 1}
  };

  w.objects[2] =
  (object) {
    df: planeSDF,
    args: (object_params) (struct plane) {(vec3){0, 1, 0,}, 2}
  };

  // w.objects[3] =
  // (object) {
  //   df: octahedronSDF,
  //   args: (object_params) (struct octahedron) {2}
  // };

  for(int i = 0; i < w.num_lights; i++) {
    double light_x = 10;//(rand() / (double) RAND_MAX) * 10 - 5;
    double light_y = 5;//(rand() / (double) RAND_MAX) * 10 - 5;
    double light_z = 0;//(rand() / (double) RAND_MAX) * 10 - 5;
    w.lights[i] = (vec3) {light_x, light_y, light_z};
  }

  spaceship s = makeSpaceship((vec3) {0,1,-7}, (vec3) {0,1,0}, (vec3) {0,0,10}, &w);

  pthread_t controller_thread;
  pthread_create(&controller_thread, NULL, controllerDriver, (void*) &s);
  while(s.buttons == NULL || s.axes == NULL) ;
  bool ballgoingup = false;
  while(!s.kill) {
    doPhysics(&s);

    pthread_t render_thread;
    pthread_create(&render_thread, NULL, render, (void*) &s);
    usleep(FPS);
    pthread_join(render_thread, NULL);
    wrefresh(frame);
    if(s.universe->objects[1].args.sphere.o.y > 2) {
      ballgoingup = false;
    } else if (s.universe->objects[1].args.sphere.o.y < -1) {
      ballgoingup = true;
    }
      s.universe->objects[1].args.sphere.o.y += ballgoingup ? 0.1 : -0.1;
  }

  pthread_join(controller_thread, NULL);

  delwin(quitter);
  delwin(frame);
  delwin(typer);
  endwin();
  return 0;
}
