#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <ncurses.h>

#include "map.h"
#include "snake.h"

struct scoreboard {
    WINDOW *win;
    int width;
    int height;
    int startx;
    int starty;
    int score;
};

struct scoreboard *scoreboard_new(struct map *m, struct snake *s);
void scoreboard_set_score(struct scoreboard *sb, struct snake *s);
void scoreboard_draw(struct scoreboard *sb);
void scoreboard_clear(struct scoreboard *sb);
void scoreboard_delete(struct scoreboard *sb);

#endif /* SCOREBOARD_H */
