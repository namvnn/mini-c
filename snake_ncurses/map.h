#ifndef MAP_H
#define MAP_H

#include <ncurses.h>

struct map {
    WINDOW *win;
    int height;
    int width;
    int startx;
    int starty;
    int border_size;
    void (*draw_point)(struct map *m, int x, int y, int color);
};

struct map *map_new(void);
void map_draw(struct map *m);
void map_clear(struct map *m);
void map_delete(struct map *m);

#endif /* MAP_H */
