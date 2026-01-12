#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "playagain.h"

static const char *playagain__TITLE = "Your score is %d";
static const char *playagain__DESCRIPTION[2] = {
    "Press enter to play again",
    "Press q to quit",
};

static struct playagain *playagain__new(void) {
    struct playagain *pa = malloc(sizeof(struct playagain));

    pa->width = 60;
    pa->height = 12;
    pa->startx = (COLS - pa->width) / 2;
    pa->starty = (LINES - pa->height) / 2;
    pa->win = newwin(pa->height, pa->width, pa->starty, pa->startx);

    return pa;
}

static void playagain__draw(struct playagain *pa, int score) {
    box(pa->win, 0, 0);

    int score_strlen = (int)log10(score) + 1;
    mvwprintw(pa->win, 4,
              (pa->width - strlen(playagain__TITLE) - score_strlen) / 2,
              playagain__TITLE, score);

    int desc_len =
        (sizeof(playagain__DESCRIPTION) / sizeof(playagain__DESCRIPTION[0]));
    for (int i = 0; i < desc_len; i++) {
        mvwprintw(pa->win, 6 + i,
                  (pa->width - strlen(playagain__DESCRIPTION[i])) / 2,
                  playagain__DESCRIPTION[i]);
    }

    wrefresh(pa->win);
}

static int playagain__input(void) {
    while (true) {
        int key = input_getkey_block();
        if (key == QUIT || key == CONTINUE) {
            return key;
        }
    }
}

static void playagain__delete(struct playagain *pa) {
    wclear(pa->win);
    wrefresh(pa->win);
    delwin(pa->win);
    free(pa);
}

int playagain_run(int score) {
    struct playagain *pa = playagain__new();
    playagain__draw(pa, score);
    int key = playagain__input();
    playagain__delete(pa);
    return key;
}
