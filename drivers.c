#define _XOPEN_SOURCE_EXTENDED
#include <ncursesw/ncurses.h>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <stdlib.h>

#include "drivers.h"

void* controllerDriver(void* args) {
  spaceship* self = (spaceship*) args;

  int js = open("/dev/input/js0", O_RDONLY);

  char num_axes, num_buttons;
  ioctl (js, JSIOCGAXES, &num_axes);
  ioctl (js, JSIOCGBUTTONS, &num_buttons);

  self->axes = malloc(num_axes * sizeof(int));
  self->buttons = malloc(num_buttons * sizeof(int));

  struct js_event e;

  do { read(js, &e, sizeof(e)); } while(e.type & JS_EVENT_INIT);

  while(1) {
    read(js, &e, sizeof(e));
    if(e.type & JS_EVENT_AXIS) {
      self->axes[e.number] = e.value;
    } else {
      if(e.number == BUTTON_OPTION) {
        self->kill = 1;
        break;
      }
    }
  }

  close(js);
  return NULL;
}

void doPhysics(spaceship* s) {
  pitch (s, MAX_ROT    * s->axes[R_STICK_Y] / -JOYSTICK_MAX);
  yaw   (s, MAX_ROT    * s->axes[R_STICK_X] / JOYSTICK_MAX);

  strafe(s, MAX_THRUST * s->axes[L_STICK_X] / -JOYSTICK_MAX);
  thrust(s, MAX_THRUST * s->axes[L_STICK_Y] / -JOYSTICK_MAX);
}
