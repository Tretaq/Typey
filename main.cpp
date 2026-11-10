#include <algorithm>
#include <ncurses.h>
#include <vector>

#include "Terminal.hpp"
#include "typing.hpp"

const int keyEsc = 27;
void drawText(const TypingState& state, const std::vector<Point>& layout, int originX, int originY) {
    for (std::size_t i = 0; i < state.target.size(); ++i)
    {
        const int x = originX + layout[i].x;
        const int y = originY + layout[i].y;
        const char c = state.target[i];
        if (i < state.pos) {
            mvaddch(y,x,c);
        } else if (i == state.pos){
            attron(A_REVERSE);
            mvaddch(y,x,c);
            attroff(A_REVERSE);
        } else {
            attron(A_DIM);
            mvaddch(y,x,c);
            attroff(A_DIM);
        }
    }
}
void drawFrame(TypingState& state, std::vector<Point>& layout, int originX, int originY) {
    erase();
    mvprintw(0,0, "Typey   Esc: quit");
    drawText(state,layout, originX, originY);
    refresh();
}

int main() {
    Terminal terminal;
    TypingState state;
    state.target = "The quick Abrams kills Ivy on Summoners Rift while playing on ukulele";
    int width = std::min(COLS - 4, 60);
    std::vector<Point> layout = layoutText(state.target, width);

    int originX = (COLS - width) / 2;
    int originY = (LINES - (layout.back().y + 1)) /2;

    bool running = true;

    while (running){
        drawFrame(state,layout,originX,originY);

        const int key = getch();
        switch (key){
        case keyEsc: running = false ; break;
                default:
                if (key >= ' ' && key <= '~' && !state.finished()) {
                    ++state.pos;
                }
                break;

        }
    }
    return 0;
}
