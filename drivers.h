#ifndef DRIVERS_H
#define DRIVERS_H

#include "spaceship.h"

#define JOYSTICK_MAX (double) 32767
#define MAX_ROT 2.0
#define MAX_THRUST 0.3

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_X 3
#define BUTTON_Y 4

#define R_STICK_Y 3
#define R_STICK_X 2
#define L_STICK_Y 1
#define L_STICK_X 0

#define D_PAD_Y 7
#define D_PAD_X 6

#define R_BUMPER 7
#define L_BUMPER 6
#define L_TRIGGER 5
#define R_TRIGGER 4

#define BUTTON_OPTION 11

void* controllerDriver(void*);
void doPhysics(spaceship*);


#endif
