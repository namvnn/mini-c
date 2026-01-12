#include <ncurses.h>

int input_getkey_block(void) {
    nodelay(stdscr, false);
    return getch();
}
int input_getkey_nonblock(void) {
    nodelay(stdscr, true);
    return getch();
}
