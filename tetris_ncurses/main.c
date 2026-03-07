#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

const char *EMPTY = " ";
const char *FILL = "\u2588";

const size_t N_CELLS_PER_TETROMINO = 4;
const size_t N_TETROMINOS = 7;
const size_t N_ORIENTATIONS = 4;

const size_t MAX_GRAVITY_LEVEL = 20;
const size_t LINES_PER_LEVEL = 10;

enum cell {
    C_EMPTY,
    C_I,
    C_J,
    C_L,
    C_O,
    C_S,
    C_T,
    C_Z,
};

enum tetromino {
    T_I,
    T_J,
    T_L,
    T_O,
    T_S,
    T_T,
    T_Z,
};

enum action {
    LEFT,
    RIGHT,
};

struct location {
    int row;
    int col;
};

struct block {
    enum tetromino tet;
    int ori;
    struct location loc;
};

struct tetris {
    int rows;
    int cols;
    int score;
    int level;
    int ticks;
    int lines;
    char *board;
    struct block current;
    struct block next;
    struct block hold;
};

struct tetris *create_tetris(int rows, int cols);
void update_tetris(struct tetris *tt);
void is_tetris_over(struct tetris *tt);
void delete_tetris(struct tetris *tt);

void update_board(WINDOW *w);
void update_next_block(WINDOW *w);
void update_hold_block(WINDOW *w);
void update_score(WINDOW *w, struct tetris *tt);

void sleep_ms(int ms);

// clang-format off
struct location TETROMINOS[N_TETROMINOS][N_ORIENTATIONS][N_CELLS_PER_TETROMINO] = {
    // I
    {
        { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 } },
        { { 0, 2 }, { 1, 2 }, { 2, 2 }, { 3, 2 } },
        { { 3, 0 }, { 3, 1 }, { 3, 2 }, { 3, 3 } },
        { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 } },
    },
    // J
    {
        { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 } },
        { { 0, 1 }, { 0, 2 }, { 1, 1 }, { 2, 1 } },
        { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 2 } },
        { { 0, 1 }, { 1, 1 }, { 2, 0 }, { 2, 1 } },
    },
    // L
    {
        { { 0, 2 }, { 1, 0 }, { 1, 1 }, { 1, 2 } },
        { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 2, 2 } },
        { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 0 } },
        { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
    },
    // O
    {
        { { 0, 1 }, { 0, 2 }, { 1, 1 }, { 1, 2 } },
        { { 0, 1 }, { 0, 2 }, { 1, 1 }, { 1, 2 } },
        { { 0, 1 }, { 0, 2 }, { 1, 1 }, { 1, 2 } },
        { { 0, 1 }, { 0, 2 }, { 1, 1 }, { 1, 2 } },
    },
    // S
    {
        { { 0, 1 }, { 0, 2 }, { 1, 0 }, { 1, 1 } },
        { { 0, 1 }, { 1, 1 }, { 1, 2 }, { 2, 2 } },
        { { 1, 1 }, { 1, 2 }, { 2, 0 }, { 2, 1 } },
        { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
    },
    // T
    {
        { { 0, 1 }, { 1, 0 }, { 1, 1 }, { 1, 2 } },
        { { 0, 1 }, { 1, 1 }, { 1, 2 }, { 2, 1 } },
        { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 1 } },
        { { 0, 1 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
    },
    // Z
    {
        { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
        { { 0, 2 }, { 1, 1 }, { 1, 2 }, { 2, 1 } },
        { { 1, 0 }, { 1, 1 }, { 2, 1 }, { 2, 2 } },
        { { 0, 1 }, { 1, 0 }, { 1, 1 }, { 2, 0 } },
    },
};

int TICKS_BY_LEVEL[MAX_GRAVITY_LEVEL] = {
    50, 48, 46, 44, 42,
    40, 38, 36, 34, 32,
    30, 28, 26, 24, 22,
    20, 16, 12, 8,  4 ,
};
// clang-format on

int main(void) {
    srand(time(NULL));

    bool running = true;
    WINDOW *board, *next, *hold, *score;
    struct tetris *tt = create_tetris(22, 10);

    initscr();            // Initialize curses
    cbreak();             // Take input chars one at a time, no wait for \n
    noecho();             // Do not echo key presses to screen
    curs_set(false);      // Hide the cursor
    keypad(stdscr, true); // Allow special keys like arrows
    timeout(0);           // Make getting char non-blocking
    start_color();        // Enable colors (create colors and color pairs)
    use_default_colors(); // Allow default terminal colors
    refresh();            // Draw the screen TODO: need this?

    // clang-format off
    board = newwin(tt->rows + 2, tt->cols * 2 + 2, (LINES - (tt->rows + 2)) / 2,      (COLS - (tt->cols * 2 + 2)) / 2);
    next  = newwin(6,            10,               (LINES - (tt->rows + 2)) / 2,      ((COLS - 10) / 2) + (tt->cols * 2 + 2 + 1));
    hold  = newwin(6,            10,               (LINES - (tt->rows + 2)) / 2 + 8,  ((COLS - 10) / 2) + (tt->cols * 2 + 2 + 1));
    score = newwin(6,            10,               (LINES - (tt->rows + 2)) / 2 + 16, ((COLS - 10) / 2) + (tt->cols * 2 + 2 + 1));
    // clang-format on

    while (running) {
        update_board(board);
        update_next_block(next);
        update_hold_block(hold);
        update_score(score, tt);
        doupdate();
        sleep_ms(10);

        switch (getch()) {
            case KEY_LEFT: {
                break;
            }
            case KEY_RIGHT: {
                break;
            }
            case KEY_UP: {
                break;
            }
            case KEY_DOWN: {
                break;
            }
            case 'q': {
                running = false;
                break;
            }
            case 'p': {
                break;
            }
            case 'h': {
                break;
            }
            default: {
            }
        }
    }

    endwin();
    delete_tetris(tt);

    printf("Game Over!\n");
    printf("%lu\n", sizeof(char *));

    return 0;
}

struct tetris *create_tetris(int rows, int cols) {
    struct tetris *tt = malloc(sizeof(struct tetris));

    tt->rows = rows;
    tt->cols = cols;
    tt->score = 0;
    tt->level = 1;
    tt->ticks = TICKS_BY_LEVEL[tt->level];
    tt->lines = LINES_PER_LEVEL;
    tt->board = malloc(rows * cols);
    memset(tt->board, C_EMPTY, rows * cols);

    return tt;
}

void delete_tetris(struct tetris *tt) {
    free(tt->board);
    free(tt);
}

void update_board(WINDOW *w) {
    box(w, 0, 0);
    wnoutrefresh(w);
}

void update_next_block(WINDOW *w) {
    box(w, 0, 0);
    wnoutrefresh(w);
}

void update_hold_block(WINDOW *w) {
    box(w, 0, 0);
    wnoutrefresh(w);
}

void update_score(WINDOW *w, struct tetris *tt) {
    werase(w);
    wprintw(w, "Score %d\n", tt->score);
    wprintw(w, "Level %d\n", tt->level);
    wprintw(w, "Lines %d\n", tt->lines);
    wnoutrefresh(w);
}

void sleep_ms(int ms) {
    struct timespec ts;

    ts.tv_sec = 0;
    ts.tv_nsec = ms * 1000 * 1000;

    nanosleep(&ts, NULL);
}
