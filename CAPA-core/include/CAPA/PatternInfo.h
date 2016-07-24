#pragma once
#include <string>
class PatternInfo {
private:
    std::string source;
    std::string pattern;
public:
    PatternInfo(std::string Pattern, std::string Source);
    PatternInfo(const PatternInfo &C);
    PatternInfo();
    std::string dumpSource() const;

};
