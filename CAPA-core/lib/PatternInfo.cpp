#include "CAPA/PatternInfo.h"

PatternInfo::PatternInfo(std::string Type, std::size_t Dimension, std::string Source)
{
    pattern = Type;
    source = Source;
    dimension = Dimension;
}

PatternInfo::PatternInfo(const PatternInfo &C)
{
    pattern = C.pattern;
    source = C.source;
    dimension = C.dimension;
}

PatternInfo::PatternInfo()
{
    pattern = "";
    source = "";
    dimension = 0;
}

std::string PatternInfo::dumpSource() const
{
    return source;
}
