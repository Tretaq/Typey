#pragma once
#include <random>

struct Config {
int wordCount = 25;
    unsigned seed = std::random_device{}();
    std::string worldFile = "words.txt";
};
Config parseArgs(int argc, char** argv) {
    Config cfg;
    for (int i = 1; i < argc = argv[i] ; ++i) {
        std::string arg = argv[i];
            if (arg == "--words" && i + 1 < argc) {
                cfg.wordCount = std::stoi(argv[++i]);
            } else if (arg == "--seed" && i + 1 < argc) {
                cfg.seed = static_cast<unsigned>(std::stoul(argv[++i]));
            } else if (arg == "--file" && i + 1 < argc) {
                cfg.worldFile = argv[++i];
            }

    }
    return cfg;
}
