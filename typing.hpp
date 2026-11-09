#pragma once
#include <string>
#include <vector>

struct Point {
    int x;
    int y;
};
struct TypingState {
    std::string target;
    std::size_t pos = 0;

    bool finished() const {
        return pos >= target.size();
    }
};