#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

void display_board(WINDOW *w);
void display_next(WINDOW *w);
void display_hold(WINDOW *w);
void display_score(WINDOW *w);

void sleep_ms(int ms);

int main(void) {
    bool running = true;
    WINDOW *board, *next, *hold, *score;
    int rows = 22, cols = 10;

    srand(time(NULL));

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
    board = newwin(rows + 2, cols * 2 + 2, (LINES - (rows + 2)) / 2,      (COLS - (cols * 2 + 2)) / 2);
    next  = newwin(6,        10,           (LINES - (rows + 2)) / 2,      ((COLS - 10) / 2) + (cols * 2 + 2 + 1));
    hold  = newwin(6,        10,           (LINES - (rows + 2)) / 2 + 8,  ((COLS - 10) / 2) + (cols * 2 + 2 + 1));
    score = newwin(6,        10,           (LINES - (rows + 2)) / 2 + 16, ((COLS - 10) / 2) + (cols * 2 + 2 + 1));
    // clang-format on

    while (running) {
        display_board(board);
        display_next(next);
        display_hold(hold);
        display_score(score);
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
    printf("Game Over!\n");

    return 0;
}

void display_board(WINDOW *w) {
    box(w, 0, 0);
    wnoutrefresh(w);
}

void display_next(WINDOW *w) {
    box(w, 0, 0);
    wnoutrefresh(w);
}

void display_hold(WINDOW *w) {
    box(w, 0, 0);
    wnoutrefresh(w);
}

void display_score(WINDOW *w) {
    werase(w);
    wprintw(w, "Score %d\n", 0);
    wprintw(w, "Level %d\n", 1);
    wnoutrefresh(w);
}

void sleep_ms(int ms) {
    struct timespec ts;

    ts.tv_sec = 0;
    ts.tv_nsec = ms * 1000 * 1000;

    nanosleep(&ts, NULL);
}
