#include <ncurses.h>
#include <stdlib.h>

#include "map.h"

// The width of two characters horizontally is much closer to the height of
// one character vertically than the width of one character.
// Source: https://stackoverflow.com/a/60046028
#define TERMINAL_WIDTH_UNIT 2

static void map__draw_point(struct map *m, int x, int y, int color) {
    wattron(m->win, COLOR_PAIR(color));
    mvwprintw(m->win, y, x * TERMINAL_WIDTH_UNIT, "  ");
    wattroff(m->win, COLOR_PAIR(color));
}

struct map *map_new(void) {
    struct map *m = malloc(sizeof(struct map));
    if (m == NULL) {
        fprintf(stderr, "Out of memory.");
        exit(1);
    }

    m->width = 25;
    m->height = 20;
    m->startx = (COLS - m->width * TERMINAL_WIDTH_UNIT) / 2;
    m->starty = (LINES - m->height) / 2;
    m->win = newwin(m->height, m->width * TERMINAL_WIDTH_UNIT, m->starty,
                    m->startx);
    m->border_size = 1;
    m->draw_point = map__draw_point;

    return m;
}

void map_draw(struct map *m) {
    box(m->win, 0, 0);
    wrefresh(m->win);
}

void map_clear(struct map *m) {
    wclear(m->win);
}

void map_delete(struct map *m) {
    wclear(m->win);
    wrefresh(m->win);
    delwin(m->win);
    free(m);
}
