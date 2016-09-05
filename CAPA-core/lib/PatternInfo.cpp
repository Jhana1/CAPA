#include "CAPA/PatternInfo.h"

PatternInfo::PatternInfo(std::string Type, std::string Source)
{
    pattern = Type;
    source = Source;
}

PatternInfo::PatternInfo(const PatternInfo &C)
{
    pattern = C.pattern;
    source = C.source;
}

PatternInfo::PatternInfo()
{
    pattern = "";
    source = "";
}

std::string PatternInfo::dumpSource() const
{
    return source;
}