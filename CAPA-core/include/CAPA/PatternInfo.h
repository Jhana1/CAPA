#pragma once
#include <string>
class PatternInfo {
private:
    std::string _source;
    
public:
    PatternInfo(std::string Type, std::string Source);
    PatternInfo(const PatternInfo &C);
    std::string dumpSource();

};
