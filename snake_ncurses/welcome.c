#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "welcome.h"

static const char *welcome__TITLE = "SNAKE";
static const char *welcome__DESCRIPTION[2] = {
    "Press enter to play",
    "Press q to quit",
};

static struct welcome *welcome__new(void) {
    struct welcome *wc = malloc(sizeof(struct welcome));

    wc->width = 60;
    wc->height = 12;
    wc->startx = (COLS - wc->width) / 2;
    wc->starty = (LINES - wc->height) / 2;
    wc->win = newwin(wc->height, wc->width, wc->starty, wc->startx);

    return wc;
}

static void welcome__draw(struct welcome *wc) {
    box(wc->win, 0, 0);

    mvwprintw(wc->win, 4, (wc->width - strlen(welcome__TITLE)) / 2,
              welcome__TITLE);

    int desc_len =
        (sizeof(welcome__DESCRIPTION) / sizeof(welcome__DESCRIPTION[0]));
    for (int i = 0; i < desc_len; i++) {
        mvwprintw(wc->win, 6 + i,
                  (wc->width - strlen(welcome__DESCRIPTION[i])) / 2,
                  welcome__DESCRIPTION[i]);
    }

    wrefresh(wc->win);
}

static int welcome__input(void) {
    while (true) {
        int key = input_getkey_block();
        if (key == QUIT || key == CONTINUE) {
            return key;
        }
    }
}

static void welcome__delete(struct welcome *wc) {
    wclear(wc->win);
    wrefresh(wc->win);
    delwin(wc->win);
    free(wc);
}

int welcome_run(void) {
    struct welcome *wc = welcome__new();
    welcome__draw(wc);
    int key = welcome__input();
    welcome__delete(wc);
    return key;
}
