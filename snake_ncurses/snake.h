#ifndef SNAKE_H
#define SNAKE_H

#include "map.h"

struct snake_body {
    int width;
    int height;
    int x;
    int y;
};

enum snake_direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
};

struct snake {
    struct snake_body *body;
    enum snake_direction direction;
    int length;
    int color;
};

struct snake *snake_new(struct map *m);
void snake_draw(struct snake *s, struct map *m);
void snake_update_keyinput(struct snake *s, int key_input);
void snake_update_after_hit_apple(struct snake *s);
bool snake_hit_itself(struct snake *s);
bool snake_hit_map_wall(struct snake *s, struct map *m);
bool snake_hit_apple(struct snake *s, int applex, int appley);
void snake_delete(struct snake *s);

#endif /* SNAKE_H */
