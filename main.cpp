#include <ncurses.h>
#include <vector>

#include "Terminal.hpp"
#include "typing.hpp"

void drawText(const TypingState& state, const std::vector<Point>& layout, int originX, int originY) {
    for (std::size_t )
}

int main() {
    Terminal terminal;
    TypingState state;
    state.target = "The quick Abrams kills Ivy on Summoners Rift while playing on ukulele";
    refresh();

    int ch;
    while ((ch = getch()) != 27) {
        printw("code: %d name: %s\n", ch, keyname(ch), keyname(ch));
        refresh();
    }
    return 0;
}
