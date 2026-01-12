#include <stdbool.h>
#include <stdlib.h>

#include "map.h"
#include "snake.h"

struct snake *snake_new(struct map *m) {
    struct snake *s = malloc(sizeof(struct snake));

    int maxlength = m->height * m->width;
    s->body = malloc(sizeof(struct snake_body) * maxlength);
    for (int i = 0; i < maxlength; i++) {
        s->body[i].x = 0;
        s->body[i].y = 0;
        s->body[i].width = 1;
        s->body[i].height = 1;
    }
    s->body[0].x = 1;
    s->body[0].y = 1;
    s->length = 1;
    s->direction = EAST;
    s->color = 1;

    return s;
}

void snake_draw(struct snake *s, struct map *m) {
    for (int i = 0; i < s->length; i++) {
        if (s->body[i].x != 0 && s->body[i].y != 0) {
            m->draw_point(m, s->body[i].x, s->body[i].y, s->color);
        }
    }
}

void snake_update_keyinput(struct snake *s, int key_input) {
    for (int i = s->length - 1; i > 0; i--) {
        s->body[i] = s->body[i - 1];
    }

    switch (key_input) {
        case 'w':
        case KEY_UP: {
            if (s->direction != SOUTH) {
                s->direction = NORTH;
            }
            break;
        }
        case 's':
        case KEY_DOWN: {
            if (s->direction != NORTH) {
                s->direction = SOUTH;
            }
            break;
        }
        case 'd':
        case KEY_RIGHT: {
            if (s->direction != WEST) {
                s->direction = EAST;
            }
            break;
        }
        case 'a':
        case KEY_LEFT: {
            if (s->direction != EAST) {
                s->direction = WEST;
            }
            break;
        }
    }

    switch (s->direction) {
        case NORTH: {
            s->body[0].y--;
            break;
        }
        case SOUTH: {
            s->body[0].y++;
            break;
        }
        case WEST: {
            s->body[0].x--;
            break;
        }
        case EAST: {
            s->body[0].x++;
            break;
        }
    }
}

void snake_update_after_hit_apple(struct snake *s) {
    s->length += 1;
}

bool snake_hit_itself(struct snake *s) {
    for (int i = 1; i < s->length; i++) {
        if (s->body[i].x == s->body[0].x && s->body[i].y == s->body[0].y) {
            return true;
        }
    }
    return false;
}

bool snake_hit_map_wall(struct snake *s, struct map *m) {
    return s->body[0].x <= 0 || s->body[0].x >= m->width - m->border_size ||
           s->body[0].y <= 0 || s->body[0].y >= m->height - m->border_size;
}

bool snake_hit_apple(struct snake *s, int applex, int appley) {
    return s->body[0].x == applex && s->body[0].y == appley;
}

void snake_delete(struct snake *s) {
    free(s->body);
    free(s);
}
