#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "scoreboard.h"
#include "snake.h"

static const char *scoreboard__TITLE = "Score";

struct scoreboard *scoreboard_new(struct map *m, struct snake *s) {
    struct scoreboard *sb = malloc(sizeof(struct scoreboard));

    sb->width = 11;
    sb->height = 5;
    sb->startx = m->startx - sb->width -
                 2; // 2 is gap between scoreboard and map
    sb->starty = m->starty;
    sb->win = newwin(sb->height, sb->width, sb->starty, sb->startx);
    scoreboard_set_score(sb, s);

    return sb;
}

void scoreboard_set_score(struct scoreboard *sb, struct snake *s) {
    sb->score = s->length;
}

void scoreboard_draw(struct scoreboard *sb) {
    box(sb->win, 0, 0);
    mvwprintw(sb->win, 0, (sb->width - strlen(scoreboard__TITLE)) / 2,
              scoreboard__TITLE);
    int score_strlen = (int)log10(sb->score) + 1;
    mvwprintw(sb->win, (sb->height - 1) / 2, (sb->width - score_strlen) / 2,
              "%d", sb->score);
    wrefresh(sb->win);
}

void scoreboard_clear(struct scoreboard *sb) {
    wclear(sb->win);
}

void scoreboard_delete(struct scoreboard *sb) {
    wclear(sb->win);
    wrefresh(sb->win);
    delwin(sb->win);
    free(sb);
}
