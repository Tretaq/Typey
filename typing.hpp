#pragma once
#include <string>
#include <vector>

struct Point {
    int x;
    int y;
};
struct TypingState {
    std::string target;
    std::string typed;

    std::size_t pos() const {
        return typed.size();
    }
    bool finished() const {
        return pos() >= target.size();
    }

    bool isCorrect(std::size_t i) const {
        return typed[i] == target[i];
    }
    void typeChar(char c) {
        if (typed.size() != target.size()) {
            typed.push_back(c);
        }
        return;
    }
    void backspace() {
        if (!typed.empty()) {
            typed.pop_back();
        }
        return;
    }
};



inline std::vector<Point> layoutText(const std::string& text, int width) {
    std::vector<Point> out(text.size());
    int x = 0;
    int y = 0;
    std::size_t i = 0;
    while (i < text.size()) {
        std::size_t end = i;
        while (end < text.size() &&  text[end] != ' ') {
            ++end;
        }
        if (end < text.size()) ++ end;
        const int len = static_cast<int>(end - i);
        if (x + len > width && x > 0) {
            x = 0;
            ++y;
        }
        for (std::size_t k = i ; k < end ; ++k) {
            out[k] = {x , y};
            ++x;
        }
        i = end;
    }
    return out;
}