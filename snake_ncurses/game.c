#include <ncurses.h>
#include <unistd.h>

#include "apple.h"
#include "game.h"
#include "help.h"
#include "input.h"
#include "map.h"
#include "scoreboard.h"
#include "snake.h"

int game_run(void) {
    struct map *m = map_new();
    struct snake *s = snake_new(m);
    struct apple *ap = apple_new(m, s);
    struct scoreboard *sb = scoreboard_new(m, s);
    struct help *h = help_new(m, sb);

    int score = 0;
    bool is_over = false;

    help_draw(h);

    while (!is_over) {
        int key = input_getkey_nonblock();
        if (key == 'q') {
            is_over = true;
            continue;
        }

        snake_update_keyinput(s, key);
        if (snake_hit_map_wall(s, m) || snake_hit_itself(s)) {
            is_over = true;
            continue;
        }
        if (snake_hit_apple(s, ap->x, ap->y)) {
            snake_update_after_hit_apple(s);
            apple_set_new_position(ap, m, s);
            scoreboard_set_score(sb, s);
        }

        apple_draw(ap, m);
        snake_draw(s, m);
        map_draw(m);
        scoreboard_draw(sb);

        usleep(75000);

        map_clear(m);
        scoreboard_clear(sb);
    }

    score = sb->score;

    scoreboard_delete(sb);
    help_delete(h);
    apple_delete(ap);
    snake_delete(s);
    map_delete(m);

    return score;
}
