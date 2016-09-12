#pragma once
#include <string>
struct PatternInfo {
    std::string source;
    std::string pattern;
    std::size_t dimension;

    PatternInfo(std::string Pattern, std::size_t Dimension, std::string Source);
    PatternInfo(const PatternInfo &C);
    PatternInfo();
    std::string dumpSource() const;

};
