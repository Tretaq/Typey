#pragma once
#include <filesystem>
#include <map>
#include <vector>

namespace fs = std::filesystem;

using PbMap = std::map<std::string,std::vector<double>>;

inline std::string makeKey(const std::string& target, int splitWords) {
    return std::to_string(splitWords) + '|' + target;
}

inline PbMap loadPbs(const fs::path& path) {
    PbMap pbs;
    std::ifstream file(path);
    std::string line;
    while (std::getline(file,line)) {
        std::istringstream ss(line);
        std::string key;
        if (!std::getline(ss,key, '\t') || key.empty()) continue;
        std::vector<double> splits;
        double t;
        while (ss >> t) {
            splits.push_back(t);
        }
        if (!splits.empty()) {
            pbs[key] = splits;
        }
    }
    return pbs;
}
inline void savePbs(const fs::path& path, const PbMap& pbs) {
    fs::ofstream
}