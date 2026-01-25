#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int getkey_block(void) {
    nodelay(stdscr, false);
    return getch();
}

int getkey_nonblock(void) {
    nodelay(stdscr, true);
    return getch();
}

int main(void) {
    srand(time(NULL));

    initscr();            // Initialize curses
    cbreak();             // Take input chars one at a time, no wait for \n
    noecho();             // Do not echo key presses to screen
    curs_set(false);      // Hide the cursor
    keypad(stdscr, true); // Allow special keys like arrows
    start_color();        // Enable colors (create colors and color pairs)
    use_default_colors(); // Allow default terminal colors
    refresh();            // Draw the screen

    endwin();

    return 0;
}
