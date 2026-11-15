#pragma once
#include <filesystem>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
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
    std::error_code ec;
    fs::create_directories(path.parent_path(),ec);
    const fs::path tmp = path.string() + ".tmp";
    {
        std::ofstream out(tmp);
        out << std::fixed << std::setprecision(3);
        for (const auto& [key, splits] : pbs) {
            out << key << '\t';
            for (double t : splits) {
                out << t << ' ';
            }
            out << '\n';
        }
    }
    fs::rename(tmp, path, ec);
}