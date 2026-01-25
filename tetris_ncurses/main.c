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

    srand(time(NULL));

    initscr();            // Initialize curses
    cbreak();             // Take input chars one at a time, no wait for \n
    noecho();             // Do not echo key presses to screen
    curs_set(false);      // Hide the cursor
    keypad(stdscr, true); // Allow special keys like arrows
    start_color();        // Enable colors (create colors and color pairs)
    use_default_colors(); // Allow default terminal colors
    refresh();            // Draw the screen TODO: need this?

    board = newwin(2, 2, 0, 5);
    next = newwin(2, 2, 0, 8);
    hold = newwin(2, 2, 0, 0);
    score = newwin(2, 2, 4, 0);

    while (running) {
        display_board(board);
        display_next(next);
        display_hold(hold);
        display_score(score);
        doupdate();
        sleep_ms(10);
    }

    endwin();

    printf("Game Over!\n");

    return 0;
}

void display_board(WINDOW *w) {
    wnoutrefresh(w);
}

void display_next(WINDOW *w) {
    wnoutrefresh(w);
}

void display_hold(WINDOW *w) {
    wnoutrefresh(w);
}

void display_score(WINDOW *w) {
    wnoutrefresh(w);
}

void sleep_ms(int ms) {
    struct timespec ts;

    ts.tv_sec = 0;
    ts.tv_nsec = ms * 1000 * 1000;

    nanosleep(&ts, NULL);
}
