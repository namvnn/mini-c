#include <stdlib.h>
#include <string.h>

#include "help.h"
#include "map.h"
#include "scoreboard.h"

static const char *help__TITLE = "Help";
static const char *help__DESCRIPTION[5] = {
    "Up     w, up arrow",
    "Right  a, right arrow",
    "Down   s, down arrow",
    "Left   d, left arrow",
    "Quit   q",
};

struct help *help_new(struct map *m, struct scoreboard *sb) {
    struct help *h = malloc(sizeof(struct help));

    h->width = 26;
    h->height = 9;
    h->startx = m->startx - h->width - 2; // 2 is gap between help and map
    h->starty = m->starty + sb->height +
                1; // 1 is gap between help and scoreboard
    h->win = newwin(h->height, h->width, h->starty, h->startx);

    return h;
}

void help_draw(struct help *h) {
    box(h->win, 0, 0);
    mvwprintw(h->win, 0, (h->width - strlen(help__TITLE)) / 2, help__TITLE);

    int desc_len = (sizeof(help__DESCRIPTION) / sizeof(help__DESCRIPTION[0]));
    for (int i = 0; i < desc_len; i++) {
        mvwprintw(h->win, 2 + i, 2, help__DESCRIPTION[i]);
    }

    wrefresh(h->win);
}

void help_clear(struct help *h) {
    wclear(h->win);
}

void help_delete(struct help *h) {
    wclear(h->win);
    wrefresh(h->win);
    delwin(h->win);
    free(h);
}
