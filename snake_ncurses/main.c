#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "input.h"
#include "playagain.h"
#include "welcome.h"

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
    init_pair(1, -1, COLOR_BLUE);
    init_pair(2, -1, COLOR_RED);

    int key = welcome_run();
    while (key != QUIT) {
        int score = game_run();
        key = playagain_run(score);
    }

    endwin();

    return 0;
}
