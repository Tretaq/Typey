#include <algorithm>
#include <ncurses.h>
#include <vector>
#include "colors.hpp"
#include "Terminal.hpp"
#include "typing.hpp"

const int keyEsc = 27;
void drawText(const TypingState& state, const std::vector<Point>& layout, int originX, int originY) {
    for (std::size_t i = 0; i < state.target.size(); ++i)
    {
        const int x = originX + layout[i].x;
        const int y = originY + layout[i].y;
        const char c = state.target[i];
        Color color = ColRed;

        if (i < state.pos()) {
            if (state.isCorrect(i)) {
                color = ColGreen;
                attron(COLOR_PAIR(color));
                mvaddch(y,x,c);
                attroff(COLOR_PAIR(color));

            } else {
                attron(A_REVERSE);
                attron(COLOR_PAIR(color));
                mvaddch(y,x,c);
                attroff(COLOR_PAIR(color));
                attroff(A_REVERSE);
            }
        } else if (i == state.pos()){
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
void drawStats(const TypingState& state, int originX, int y) {
    mvprintw(y, originX, "WPM %.0f  ACC %d%%    %.1fs", state.wpm(), state.accuracy(), state.elapsedSeconds());
}
void drawResult(const TypingState& state, int originX, int y) {
    attron(A_BOLD);
    mvprintw(y,originX, "DONE   WPM %.0f    raw %.0f    ACC %d%%    %.2fs",state.wpm(),state.rawWpm(),state.accuracy(),state.elapsedSeconds());
    attroff(A_BOLD);
    mvprintw(y + 1, originX, "Enter: retry  Esc: quit");
}
void drawFrame(const TypingState& state,const std::vector<Point>& layout, int originX, int originY) {
    erase();
    mvprintw(0,0, "Typey   Esc: quit");
    drawText(state,layout, originX, originY);

    const int lines = layout.back().y + 1;
    const int statsY = originY + lines +  1;
    if (state.finished()) {
        drawResult(state,originX,statsY);
    } else {
        drawStats(state, originX, statsY);
    }
    refresh();
}

int main() {
    Terminal terminal;
    TypingState state;
    state.target = "The quick Abrams kills Ivy on Summoners Rift while playing on Undertale";
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
            case '\n': case KEY_ENTER: if (state.finished()) state.reset(); break;
            case KEY_BACKSPACE: case 127: state.backspace(); break;
                default:
                if (key >= ' ' && key <= '~' ) {
                    state.typeChar(static_cast<char>(key));
                }
                break;

        }
    }
    return 0;
}
