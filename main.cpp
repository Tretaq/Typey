#include <algorithm>
#include <iostream>
#include <ncurses.h>
#include <vector>
#include "colors.hpp"
#include "config.hpp"
#include "pb.hpp"
#include "Terminal.hpp"
#include "typing.hpp"
#include "worlds.hpp"

struct Test {
    TypingState state;
    std::vector<Point> layout;
    int originX = 0;
    int originY = 0;
    std::string key;
    std::vector<double> pb;
    bool newPb = false;
};

Test makeTest(const std::vector<std::string>& words, const Config& config, std::mt19937 rng, int width, const PbMap& pbs) {
    Test test;
    test.state.target = generatePassage(words, config.wordCount,rng);
    test.state.segments = makeSegments(test.state.target, config.splitWords);
    test.layout = layoutText(test.state.target, width);
    const int lines = test.layout.back().y + 1;
    test.originX = (COLS - width)/2;
    test.originY = (LINES - lines)/2;
    test.key = makeKey(test.state.target, config.splitWords);
    auto it = pbs.find(test.key);
    if (it != pbs.end() && it->second.size() == test.state.segments.size()) {
        test.pb = it->second;
    }
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
void drawDelta(int row, int x, double delta) {
    const Color color = (delta <= 0.0) ? ColGreen : ColRed;
    attron(COLOR_PAIR(color));
    mvprintw(row,x, "%+.2f",delta);
    attroff(COLOR_PAIR(color));
}
void drawSplits(const TypingState& state, int x, int y, std::vector<double> pb) {

    const std::size_t done = state.splits.size();
    const bool hasPb = pb.size() == state.segments.size();
    const int deltaX = x + 26;
    const double elapsed = state.elapsedSeconds();
    attron(A_DIM);
    mvprintw(y - 1, x, "%2x    %7s     %-10s%s", "#", "split", "seg",hasPb ? "vs PB" : "");
    attroff(A_DIM);
    for (size_t i = 0 ; i < state.segments.size() ; i++) {
        int row = y + static_cast<int>(i);
        if (i < done) {
            double prev = (i > 0) ? state.splits[i - 1] : 0.0;
            mvprintw(row, x , "%2zu    %6.2fs   +%.2f", i + 1, state.splits[i], state.splits[i] - prev);
            if (hasPb) {
                drawDelta(row,deltaX,state.splits[i] - pb[i]);
            }
        } else if (i == done) {
            double last = (done > 0) ? state.splits[done - 1] : 0.0;
            attron(A_BOLD);
            mvprintw(row,x,"%2zu    %6s    +%.2f", i + 1, "...", state.elapsedSeconds() - last);
            attroff(A_BOLD);
            if (hasPb && elapsed > pb[i]) {
                drawDelta(row,deltaX,elapsed - pb[i]);
            }
        } else {
            attron(A_DIM);
            mvprintw(row,x,"%2zu       ...         ", i + 1);
            attroff(A_DIM);
        }
    }
}
void drawFrame(const Test& test, const Config& config) {
    const TypingState& state = test.state;
    erase();
    mvprintw(0,0, "Typey    seed=%u    words=%d    Tab: restart   Esc: quit",config.seed,config.wordCount);
    drawText(state,test.layout, test.originX, test.originY);
    const int lines = test.layout.back().y + 1;
    const int statsY = test.originY + lines +  1;
    drawSplits(state, test.originX, statsY + 3,test.pb);
    if (state.finished()) {
        drawResult(state,test.originX,statsY);
    } else {
        drawStats(state, test.originX, statsY);
    }
    refresh();
}
void onFinish(Test& test, PbMap& pbs, const std::string& pbFile) {
    const TypingState& s = test.state;

    if (s.splits.size() != s.segments.size()) return;
    if (s.correctChars() != s.target.size()) return;
    const double total = s.splits.back();
    if (!test.pb.empty() && total >= test.pb.back()) return;
    test.pb = s.splits;
    pbs[test.key] = s.splits;
    test.newPb = true;
    savePbs(pbFile, pbs);
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
    PbMap pbs = loadPbs(config.pbFile);

    Test test = makeTest(words,config,rng,width,pbs);
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
                    test = makeTest(words, config, rng, width, pbs);
                }
                break;
            case KEY_BACKSPACE: case 127: test.state.backspace(); break;
            case '\t': test.state.reset(); break;
                default:
                if (key >= ' ' && key <= '~' ) {
                    const bool wasDone = test.state.finished();
                    test.state.typeChar(static_cast<char>(key));
                    if(!wasDone && test.state.finished()) {
                        onFinish(test, pbs,  config.pbFile);
                    }
                }
                break;

        }
    }
    return 0;
}
