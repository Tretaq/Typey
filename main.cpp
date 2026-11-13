#include <algorithm>
#include <iostream>
#include <ncurses.h>
#include <vector>
#include "colors.hpp"
#include "config.hpp"
#include "Terminal.hpp"
#include "typing.hpp"
#include "worlds.hpp"

struct Test {
    TypingState state;
    std::vector<Point> layout;
    int originX = 0;
    int originY = 0;
};

Test makeTest(const std::vector<std::string>& words, const Config& config, std::mt19937 rng, int width) {
    Test test;
    test.state.target = generatePassage(words, config.wordCount,rng);
    test.layout = layoutText(test.state.target, width);
    const int lines = test.layout.back().y + 1;
    test.originX = (COLS - width)/2;
    test.originY = (LINES - lines)/2;
    return test;
}
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
    mvprintw(y + 1, originX, "Enter: new passage    Tab: retry  Esc: quit");
}
void drawFrame(const TypingState& state,const std::vector<Point>& layout, int originX, int originY, Config config) {
    erase();
    mvprintw(0,0, "Typey    seed=%u    words=%d    Tab: restart   Esc: quit",config.seed,config.wordCount);
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

int main(int argc, char *argv[]) {
    Config config;
    try {
        config = parseArgs(argc,argv);
    } catch (const std::exception& e) {
        std::cerr << "typey: invalid arguments " << e.what() << "\n";
        return 1;
    }
    std::vector<std::string> words = loadWords(config.worldFile);
    Terminal terminal;
    std::random_device rnd;
    std::mt19937 rng(config.seed);
    const int width = std::min(COLS - 4, 60);
    Test test = makeTest(words,config,rng,width);
    bool running = true;
    while (running){
        drawFrame(test.state,test.layout,test.originX,test.originY,config);
        const int key = getch();
        switch (key){
        case keyEsc: running = false ; break;
            case '\n': case KEY_ENTER:
                if (test.state.finished()) {
                    config.seed = rnd();
                    rng.seed(config.seed);
                    test = makeTest(words, config, rng, width);
                }
                break;
            case KEY_BACKSPACE: case 127: test.state.backspace(); break;
            case '\t': test.state.reset(); break;
                default:
                if (key >= ' ' && key <= '~' ) {
                    test.state.typeChar(static_cast<char>(key));
                }
                break;

        }
    }
    return 0;
}
