#pragma once
#include <ncurses.h>

class Terminal {
public:
    Terminal() {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        set_escdelay(25);
    }
    ~Terminal() {
        endwin();
    }
    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;
};