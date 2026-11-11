#pragma once
#include <ncurses.h>

class Terminal {
public:
    Terminal() {
        initscr();
        start_color();
        use_default_colors();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        set_escdelay(25);
        curs_set(0);
        init_pair(1,COLOR_GREEN,-1);// default bg
        init_pair(2,COLOR_RED,-1);
        timeout(50);
    }
    ~Terminal() {
        endwin();
    }
    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;
};