#pragma once
#include <fstream>
#include <random>
#include <string>
#include <vector>

inline std::vector<std::string> loadWords(const std::string& path) {
    std::vector<std::string> words;
    std::ifstream file(path);
    std::string word;
    while (file >>  word) {
        words.push_back(word);
    }
    if (words.empty()) {
        words = {"The","quick","Abrams","kills","Ivy","on","Summoners","Rift","while","playing","Undertale"};
    }
    return words;
}
inline std::string generatePassage(const std::vector<std::string>& words, int count, std::mt19937& rng) {
    std::uniform_int_distribution<std::size_t>pick(0,words.size() - 1);
    std::string passage;
    for (int i = 0; i < count; i++) {
        if (i > 0) {
            passage += ' ';
        }
        passage += words[pick(rng)];
    }
    return passage;
}