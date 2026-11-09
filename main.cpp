#include <ncurses.h>

#include "Terminal.hpp"


int main() {
    Terminal terminal;
    scrollok(stdscr, TRUE);
    printw("press Esc to quit");
    refresh();

    int ch;
    while ((ch = getch()) != 27) {
        printw("code: %d name: %s\n", ch, keyname(ch), keyname(ch));
        refresh();
    }
    return 0;
}
