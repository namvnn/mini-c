#ifndef WELCOME_H
#define WELCOME_H

#include <ncurses.h>

struct welcome {
    WINDOW *win;
    int width;
    int height;
    int startx;
    int starty;
};

int welcome_run(void);

#endif /* WELCOME_H */
