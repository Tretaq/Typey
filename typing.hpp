#pragma once
#include <string>
#include <vector>
#include <chrono>

#include "segment.hpp"
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
    std::vector<Segment> segments;
    std::vector<double> splits;
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
        if (finished()) return;
        if (phase == Phase::Waiting) {
            startTime = Clock::now();
            phase = Phase::Running;
        }
        if (c != target[typed.size()]) {
            ++errors;
        }
        ++keystrokes;
        typed.push_back(c);
        if (typed.size() == target.size()) {
            endTime = Clock::now();
            phase = Phase::Done;
        }
        if (splits.size() < segments.size() && typed.size() >= segments[splits.size()].end) {
            splits.push_back(elapsedSeconds());
        }
    }
    void backspace() {
        if (!typed.empty() && phase != Phase::Done) {
            typed.pop_back();
        }
        if (!splits.empty() && typed.size() < segments[splits.size() -1].end) {
            splits.pop_back();
        }
    }
    size_t correctChars() const {
        size_t count =0;
        for (size_t i = 0 ; i < typed.size(); i++) {
            if (target[i] == typed[i]) {
                count++;
            }
        }
        return count;
    }
     double wpm() const {
        const double minutes = elapsedSeconds() / 60.0;
        if (minutes < 1.00 / 60.0) return 0.0;
        return (correctChars() / 5.0) / minutes;
    }
    double rawWpm() const {
        const double minutes = elapsedSeconds() / 60.0;
        if (minutes < 1.00 / 60.0) return 0.0;
        return (keystrokes / 5.0) / minutes;
    }
    int accuracy() const{
        if (keystrokes == 0) return 100;
        return 100 * (keystrokes - errors) / keystrokes;
    }
    void reset() {
        typed = "";
        phase = Phase::Waiting;
        keystrokes = 0;
        errors = 0;

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