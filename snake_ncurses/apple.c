#include <stdlib.h>

#include "apple.h"
#include "map.h"
#include "snake.h"

static int apple__random_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

struct apple *apple_new(struct map *m, struct snake *s) {
    struct apple *ap = malloc(sizeof(struct apple));
    if (ap == NULL) {
        fprintf(stderr, "Out of memory.");
        exit(1);
    }

    ap->width = 1;
    ap->height = 1;
    ap->color = 2;
    apple_set_new_position(ap, m, s);

    return ap;
}

void apple_draw(struct apple *ap, struct map *m) {
    m->draw_point(m, ap->x, ap->y, ap->color);
}

void apple_set_new_position(struct apple *ap, struct map *m, struct snake *s) {
    bool found = false;
    while (!found) {
        ap->x = apple__random_range(1, m->width - m->border_size - ap->width);
        ap->y = apple__random_range(1, m->height - m->border_size - ap->height);

        bool is_within_snake_body = false;
        for (int i = 0; i < s->length; i++) {
            if (s->body[i].x == ap->x && s->body[i].y == ap->y) {
                is_within_snake_body = true;
            }
        }
        if (is_within_snake_body) {
            continue;
        }

        found = true;
    }
}

void apple_delete(struct apple *ap) {
    free(ap);
}
