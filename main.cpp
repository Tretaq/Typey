#include <ncurses.h>
#include <vector>

#include "Terminal.hpp"
#include "typing.hpp"

const int keyEsc = 27;
void drawText(const TypingState& state, const std::vector<Point>& layout, int originX, int originY) {
    for (std::size_t i = 0; i < state.target.size(); ++i)
    {
        
    }
}

int main() {
    Terminal terminal;
    TypingState state;
    state.target = "The quick Abrams kills Ivy on Summoners Rift while playing on ukulele";
    const int len = static_cast<int>(state.target.size());
    bool running = true;
    while (running){
        erase();
        mvprintw(LINES / 2, (COLS - len) / 2, "%s", state.target.c_str() );
        refresh();
        const int key = getch();
        switch (key)
        {
        case keyEsc: break;
            default: break;
        }
    }
    return 0;
}
