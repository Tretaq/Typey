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
    if (word.empty()) {
        words = {"The","quick","Abrams","kills","Ivy","on","Summoners","Rift","while","playing","Undertale"};
    }
    return words;
}
std::generatePassage(const std::vector<std::string>& words, int count, std::mt19937& rng) {

}