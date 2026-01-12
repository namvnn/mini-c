#ifndef PLAYAGAIN_H
#define PLAYAGAIN_H

#include <ncurses.h>

struct playagain {
    WINDOW *win;
    int width;
    int height;
    int startx;
    int starty;
};

int playagain_run(int score);

#endif /* PLAYAGAIN_H */
