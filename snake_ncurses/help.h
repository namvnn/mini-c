#ifndef HELP_H
#define HELP_H

#include <ncurses.h>

#include "map.h"
#include "scoreboard.h"

struct help {
    WINDOW *win;
    int width;
    int height;
    int startx;
    int starty;
};

struct help *help_new(struct map *m, struct scoreboard *sb);
void help_draw(struct help *h);
void help_clear(struct help *h);
void help_delete(struct help *h);

#endif /* HELP_H */
