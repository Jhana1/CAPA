#include "CAPA/PatternInfo.h"

PatternInfo::PatternInfo(std::string Type, std::size_t Dimension, std::string Source, int Priority)
{
    pattern = Type;
    source = Source;
    dimension = Dimension;
    priority = Priority;
}

PatternInfo::PatternInfo(const PatternInfo &C)
{
    pattern = C.pattern;
    source = C.source;
    dimension = C.dimension;
    priority = C.priority;
}

PatternInfo::PatternInfo()
{
    pattern = "";
    source = "";
    dimension = 0;
    priority = 10;
}

std::string PatternInfo::dumpSource() const
{
    return source;
}
