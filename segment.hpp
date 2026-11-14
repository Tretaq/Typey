#include <cstddef>
#include <string>
#include <vector>

struct Segment {
    std::size_t begin;
    std::size_t end;
};
std::vector<Segment> makeSegments(const std::string& text, int wordsPerSegment) {
    if (wordsPerSegment <= 0) {
        wordsPerSegment = 1;
    }
    std::size_t begin = 0;
    int wordCount = 0;
    std::vector<Segment> segments;
    for (std::size_t i = 0 ; i < text.size() ; ++i) {
        if (text[i] == ' ') {
            wordCount++;
            if (wordCount == wordsPerSegment) {
                segments.push_back({begin, i + 1});
                begin = i + 1;
                wordCount = 0;
            }
        }
    }
    if (begin < text.size()) {
        segments.push_back({begin,text.size()});
    }
    return segments;
}
