#pragma once
#include <string>
#include <vector>

#include "Time.hpp"

struct Point {
    int x;
    int y;
};
struct TypingState {
    std::string target;
    std::string typed;
    Phase phase = Phase::Waiting;
    Clock::time_point startTime;
    Clock::time_point endTime;
    int keystrokes = 0;
    int errors = 0;
    double elapsedSeconds() const {
        using Sec = std::chrono::duration<double>;
        switch (phase) {
            case Phase::Waiting: return 0.0;
            case Phase::Running: return Sec(Clock::now() - startTime).count();
            case Phase::Done: return Sec(endTime - startTime).count();
        }
        return 0.0;
    }
    std::size_t pos() const {
        return typed.size();
    }
    bool finished() const {
        return phase == Phase::Done;
    }

    bool isCorrect(std::size_t i) const {
        return typed[i] == target[i];
    }
    void typeChar(char c) {
        if (phase != Phase::Done){
            if (phase == Phase::Waiting) {
                startTime = Clock::now();
                phase = Phase::Running;
            }
            if (!finished()) {
                typed.push_back(c);
            }
            if (!isCorrect(typed.size())) {
                errors++;
            }
            keystrokes++;
            if (typed.size() == target.size()) {
                endTime = Clock::now();
                phase = Phase::Done;
            }
    }
    }
    void backspace() {
        if (!typed.empty() && phase != Phase::Done) {
            typed.pop_back();
        }
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