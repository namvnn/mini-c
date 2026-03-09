#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

#define CELL_TO_COLOR(c) ((c) + 1)

const int MAX_LEVEL = 20;
const int LINES_PER_LEVEL = 10;

const int ROWS_PER_CELL = 1;
const int COLS_PER_CELL = 2;

const int N_CELLS_PER_TETROMINO = 4;
const int N_TETROMINO_CELL = 7; // The first 7 members of the `cell_t` enum
const int N_ORIENTATIONS = 4;

enum cell_t {
    C_I,
    C_J,
    C_L,
    C_O,
    C_S,
    C_T,
    C_Z,
    C_NONE,
};

enum falling_action {
    FA_NONE,
    FA_MOVE_LEFT,
    FA_MOVE_RIGHT,
    FA_ROTATE_LEFT,
    FA_ROTATE_RIGHT,
    FA_STRAIGHT_DOWN,
    FA_SWAP_HOLD,
};

struct location {
    int row;
    int col;
};

struct block {
    enum cell_t cell;
    struct location loc;
    int ori;
};

struct tetris {
    int rows;
    int cols;
    int score;
    int level;
    int gravity_ticks;
    int lines;
    enum cell_t *board;
    struct block falling;
    struct block next;
    struct block hold;
};

struct tetris *create_tetris(int rows, int cols);
void update_tetris(struct tetris *tt, enum falling_action f_action);
void delete_tetris(struct tetris *tt);
bool is_tetris_over(struct tetris *tt);

bool is_line_full(struct tetris *tt, int row);
void shift_lines(struct tetris *tt, int row);
int update_lines(struct tetris *tt);
void update_score(struct tetris *tt, int n_cleared_lines);

void apply_falling_gravity(struct tetris *tt);
void move_falling(struct tetris *tt, int direction);
void go_straight_down_falling(struct tetris *tt);
void rotate_falling(struct tetris *tt, int direction);
void swap_hold_with_falling(struct tetris *tt);
void handle_falling_action(struct tetris *tt, enum falling_action f_action);

enum cell_t get_cell(struct tetris *tt, int row, int col);
void set_cell(struct tetris *tt, int row, int col, enum cell_t cell);
bool check_cell_bounds(struct tetris *tt, int row, int col);

void set_block(struct tetris *tt, struct block b);
void remove_block(struct tetris *tt, struct block b);
bool check_block_fit(struct tetris *tt, struct block b);
void get_falling_block(struct tetris *tt);

void display_board(WINDOW *win, struct tetris *tt);
void display_next(WINDOW *win, struct tetris *tt);
void display_hold(WINDOW *win, struct tetris *tt);
void display_score(WINDOW *win, struct tetris *tt);

void fill_solid(WINDOW *win, enum cell_t cell);
void fill_empty(WINDOW *win);
void sleep_ms(int ms);

// clang-format off
struct location TETROMINOS[N_TETROMINO_CELL][N_ORIENTATIONS][N_CELLS_PER_TETROMINO] = {
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

int GRAVITY_TICKS_BY_LEVEL[MAX_LEVEL + 1] = {
    0,
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
    enum falling_action f_action = FA_NONE;

    initscr();            // Initialize curses
    cbreak();             // Take input chars one at a time, no wait for \n
    noecho();             // Do not echo key presses to screen
    curs_set(false);      // Hide the cursor
    keypad(stdscr, true); // Allow special keys like arrows
    timeout(0);           // Make getting char non-blocking
    start_color();        // Enable colors (create colors and color pairs)
    use_default_colors(); // Allow default terminal colors
    init_pair(CELL_TO_COLOR(C_I), -1, COLOR_CYAN);
    init_pair(CELL_TO_COLOR(C_J), -1, COLOR_BLUE);
    init_pair(CELL_TO_COLOR(C_L), -1, COLOR_WHITE);
    init_pair(CELL_TO_COLOR(C_O), -1, COLOR_YELLOW);
    init_pair(CELL_TO_COLOR(C_S), -1, COLOR_GREEN);
    init_pair(CELL_TO_COLOR(C_T), -1, COLOR_MAGENTA);
    init_pair(CELL_TO_COLOR(C_Z), -1, COLOR_RED);

    // clang-format off
    board = newwin(tt->rows + 2, tt->cols * 2 + 2, (LINES - (tt->rows + 2)) / 2,      (COLS - (tt->cols * 2 + 2)) / 2);
    next  = newwin(6,            10,               (LINES - (tt->rows + 2)) / 2,      ((COLS - 10) / 2) + (tt->cols * 2 + 2 + 1));
    hold  = newwin(6,            10,               (LINES - (tt->rows + 2)) / 2 + 8,  ((COLS - 10) / 2) + (tt->cols * 2 + 2 + 1));
    score = newwin(6,            10,               (LINES - (tt->rows + 2)) / 2 + 16, ((COLS - 10) / 2) + (tt->cols * 2 + 2 + 1));
    // clang-format on

    while (running) {
        update_tetris(tt, f_action);
        running = !is_tetris_over(tt);

        display_board(board, tt);
        display_next(next, tt);
        display_hold(hold, tt);
        display_score(score, tt);
        doupdate();

        sleep_ms(10);

        switch (getch()) {
            case KEY_LEFT: {
                f_action = FA_MOVE_LEFT;
                break;
            }
            case KEY_RIGHT: {
                f_action = FA_MOVE_RIGHT;
                break;
            }
            case KEY_DOWN: {
                f_action = FA_STRAIGHT_DOWN;
                break;
            }
            case KEY_UP:
            case ' ': {
                f_action = FA_ROTATE_RIGHT;
                break;
            }
            case 'q': {
                running = false;
                break;
            }
            case 'h': {
                f_action = FA_SWAP_HOLD;
                break;
            }
            default: {
                f_action = FA_NONE;
            }
        }
    }

    endwin();
    delete_tetris(tt);

    printf("Game Over!\n");

    return 0;
}

struct tetris *create_tetris(int rows, int cols) {
    int i;
    struct tetris *tt = malloc(sizeof(struct tetris));

    tt->rows = rows;
    tt->cols = cols;
    tt->score = 0;
    tt->level = 1;
    tt->gravity_ticks = GRAVITY_TICKS_BY_LEVEL[tt->level];
    tt->lines = LINES_PER_LEVEL;
    tt->board = malloc(rows * cols);
    for (i = 0; i < rows * cols; i++) {
        tt->board[i] = C_NONE;
    }

    get_falling_block(tt);
    get_falling_block(tt);
    tt->hold.cell = C_NONE;
    tt->hold.loc.row = 0;
    tt->hold.loc.col = tt->cols / 2 - 2;
    tt->hold.ori = 0;

    return tt;
}

void update_tetris(struct tetris *tt, enum falling_action action) {
    int n_cleared_lines;

    apply_falling_gravity(tt);
    handle_falling_action(tt, action);
    n_cleared_lines = update_lines(tt);
    update_score(tt, n_cleared_lines);
}

void delete_tetris(struct tetris *tt) {
    free(tt->board);
    free(tt);
}

bool is_tetris_over(struct tetris *tt) {
    int r, c;

    remove_block(tt, tt->falling);

    for (r = 0; r < 2; r++) {
        for (c = 0; c < tt->cols; c++) {
            if (get_cell(tt, r, c) != C_NONE) {
                set_block(tt, tt->falling);
                return true;
            }
        }
    }

    set_block(tt, tt->falling);

    return false;
}

bool is_line_full(struct tetris *tt, int row) {
    int col;

    for (col = 0; col < tt->cols; col++) {
        if (get_cell(tt, row, col) == C_NONE) {
            return false;
        }
    }

    return true;
}

void shift_lines(struct tetris *tt, int row) {
    int r, c;

    for (r = row - 1; r >= 0; r--) {
        for (c = 0; c < tt->cols; c++) {
            set_cell(tt, r + 1, c, get_cell(tt, r, c));
            set_cell(tt, r, c, C_NONE);
        }
    }
}

int update_lines(struct tetris *tt) {
    int r;
    int n_lines = 0;

    remove_block(tt, tt->falling);

    for (r = tt->rows - 1; r >= 0; r--) {
        if (is_line_full(tt, r)) {
            shift_lines(tt, r);
            r++;
            n_lines++;
        }
    }

    set_block(tt, tt->falling);

    return n_lines;
}

void update_score(struct tetris *tt, int n_cleared_lines) {
    if (n_cleared_lines == 0) {
        return;
    }

    tt->score += n_cleared_lines * tt->level;

    if (n_cleared_lines >= tt->lines) {
        tt->level = MIN(MAX_LEVEL, tt->level + 1);
        tt->lines = LINES_PER_LEVEL + (tt->lines - n_cleared_lines);
    } else {
        tt->lines -= n_cleared_lines;
    }
}

void apply_falling_gravity(struct tetris *tt) {
    tt->gravity_ticks--;

    if (tt->gravity_ticks > 0) {
        return;
    }

    remove_block(tt, tt->falling);
    tt->falling.loc.row++;

    if (check_block_fit(tt, tt->falling)) {
        tt->gravity_ticks = GRAVITY_TICKS_BY_LEVEL[tt->level];
        set_block(tt, tt->falling);
    } else {
        tt->falling.loc.row--;
        set_block(tt, tt->falling);
        get_falling_block(tt);
    }
}

void move_falling(struct tetris *tt, int direction) {
    remove_block(tt, tt->falling);

    tt->falling.loc.col += direction;
    if (!check_block_fit(tt, tt->falling)) {
        tt->falling.loc.col -= direction;
    }

    set_block(tt, tt->falling);
}

void go_straight_down_falling(struct tetris *tt) {
    remove_block(tt, tt->falling);

    while (check_block_fit(tt, tt->falling)) {
        tt->falling.loc.row++;
    }
    tt->falling.loc.row--;

    set_block(tt, tt->falling);
    get_falling_block(tt);
}

void rotate_falling(struct tetris *tt, int direction) {
    remove_block(tt, tt->falling);

    while (true) {
        tt->falling.ori = (tt->falling.ori + direction) % N_ORIENTATIONS;

        if (check_block_fit(tt, tt->falling)) {
            break;
        }

        tt->falling.loc.col--;
        if (check_block_fit(tt, tt->falling)) {
            break;
        }

        tt->falling.loc.col += 2;
        if (check_block_fit(tt, tt->falling)) {
            break;
        }

        tt->falling.loc.col--;
    }

    set_block(tt, tt->falling);
}

void swap_hold_with_falling(struct tetris *tt) {
    enum cell_t falling_cell;
    int falling_ori;

    remove_block(tt, tt->falling);

    if (tt->hold.cell == C_NONE) {
        tt->hold = tt->falling;
        get_falling_block(tt);
    } else {
        falling_cell = tt->falling.cell;
        falling_ori = tt->falling.ori;
        tt->falling.cell = tt->hold.cell;
        tt->falling.ori = tt->hold.ori;
        tt->hold.cell = falling_cell;
        tt->hold.ori = falling_ori;

        while (!check_block_fit(tt, tt->falling)) {
            tt->falling.loc.row--;
        }
    }

    set_block(tt, tt->falling);
}

void handle_falling_action(struct tetris *tt, enum falling_action f_action) {
    switch (f_action) {
        case FA_MOVE_LEFT: {
            move_falling(tt, -1);
            break;
        }
        case FA_MOVE_RIGHT: {
            move_falling(tt, 1);
            break;
        }
        case FA_ROTATE_LEFT: {
            rotate_falling(tt, -1);
            break;
        }
        case FA_ROTATE_RIGHT: {
            rotate_falling(tt, 1);
            break;
        }
        case FA_STRAIGHT_DOWN: {
            go_straight_down_falling(tt);
            break;
        }
        case FA_SWAP_HOLD: {
            swap_hold_with_falling(tt);
            break;
        }
        default: {
            break;
        }
    }
}

enum cell_t get_cell(struct tetris *tt, int row, int col) {
    return tt->board[tt->cols * row + col];
}

void set_cell(struct tetris *tt, int row, int col, enum cell_t cell) {
    tt->board[tt->cols * row + col] = cell;
}

bool check_cell_bounds(struct tetris *tt, int row, int col) {
    return 0 <= row && row < tt->rows && 0 <= col && col < tt->cols;
}

void set_block(struct tetris *tt, struct block b) {
    size_t i;
    struct location tl;

    for (i = 0; i < N_CELLS_PER_TETROMINO; i++) {
        tl = TETROMINOS[b.cell][b.ori][i];
        set_cell(tt, b.loc.row + tl.row, b.loc.col + tl.col, b.cell);
    }
}

void remove_block(struct tetris *tt, struct block b) {
    size_t i;
    struct location tl;

    for (i = 0; i < N_CELLS_PER_TETROMINO; i++) {
        tl = TETROMINOS[b.cell][b.ori][i];
        set_cell(tt, b.loc.row + tl.row, b.loc.col + tl.col, C_NONE);
    }
}

bool check_block_fit(struct tetris *tt, struct block b) {
    size_t i;
    int r, c;
    struct location tl;

    for (i = 0; i < N_CELLS_PER_TETROMINO; i++) {
        tl = TETROMINOS[b.cell][b.ori][i];
        r = b.loc.row + tl.row;
        c = b.loc.col + tl.col;

        if (!check_cell_bounds(tt, r, c) || get_cell(tt, r, c) != C_NONE) {
            return false;
        }
    }

    return true;
}

void get_falling_block(struct tetris *tt) {
    tt->falling = tt->next;
    tt->next.cell = rand() % N_TETROMINO_CELL;
    tt->next.loc.row = 0;
    tt->next.loc.col = tt->cols / 2 - 2;
    tt->next.ori = 0;
}

void display_board(WINDOW *win, struct tetris *tt) {
    int r, c;
    enum cell_t val;

    box(win, 0, 0);

    for (r = 0; r < tt->rows; r++) {
        wmove(win, r + 1, 1);
        for (c = 0; c < tt->cols; c++) {
            val = get_cell(tt, r, c);
            if (val == C_NONE) {
                fill_empty(win);
            } else {
                fill_solid(win, val);
            }
        }
    }

    wnoutrefresh(win);
}

void display_next(WINDOW *win, struct tetris *tt) {
    size_t i;
    int c, r;
    struct location tl;

    wclear(win);
    box(win, 0, 0);

    if (tt->next.cell == C_NONE) {
        wnoutrefresh(win);
        return;
    }

    for (i = 0; i < N_CELLS_PER_TETROMINO; i++) {
        tl = TETROMINOS[tt->next.cell][tt->next.ori][i];
        r = tl.row * ROWS_PER_CELL + 2;
        c = tt->next.cell == C_O || tt->next.cell == C_I
                ? tl.col * COLS_PER_CELL + 1
                : tl.col * COLS_PER_CELL + 2;
        wmove(win, r, c);
        fill_solid(win, tt->next.cell);
    }

    wnoutrefresh(win);
}

void display_hold(WINDOW *win, struct tetris *tt) {
    size_t i;
    int c, r;
    struct location tl;

    wclear(win);
    box(win, 0, 0);

    if (tt->hold.cell == C_NONE) {
        wnoutrefresh(win);
        return;
    }

    for (i = 0; i < N_CELLS_PER_TETROMINO; i++) {
        tl = TETROMINOS[tt->hold.cell][tt->hold.ori][i];
        r = tl.row * ROWS_PER_CELL + 2;
        c = tt->hold.cell == C_O || tt->hold.cell == C_I
                ? tl.col * COLS_PER_CELL + 1
                : tl.col * COLS_PER_CELL + 2;
        wmove(win, r, c);
        fill_solid(win, tt->hold.cell);
    }

    wnoutrefresh(win);
}

void display_score(WINDOW *win, struct tetris *tt) {
    wclear(win);
    wprintw(win, "Score %d\n", tt->score);
    wprintw(win, "Level %d\n", tt->level);
    wprintw(win, "Lines %d\n", tt->lines);
    wnoutrefresh(win);
}

void fill_solid(WINDOW *win, enum cell_t cell) {
    waddch(win, ' ' | COLOR_PAIR(CELL_TO_COLOR(cell)));
    waddch(win, ' ' | COLOR_PAIR(CELL_TO_COLOR(cell)));
}

void fill_empty(WINDOW *win) {
    waddch(win, ' ');
    waddch(win, ' ');
}

void sleep_ms(int ms) {
    struct timespec ts;

    ts.tv_sec = 0;
    ts.tv_nsec = ms * 1000 * 1000;

    nanosleep(&ts, NULL);
}
