
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

int main(int argc, char const *argv[]) {
  // printf("%d\n", c);
    // if ((e.type & ~JS_EVENT_INIT) == JS_EVENT_BUTTON) {
  	// 	if (e.value) {
  	// 		buttons_state |= (1 << e.number);
  	// 	} else {
  	// 		buttons_state &= ~(1 << e.number);  // buttons_state ^= (1 << e.number); //??
    //   }
    // }
  // }
  initscr();
  start_color();

  printf("%d %d\n", COLORS, COLOR_PAIRS);
  endwin();
  return 0;
}
