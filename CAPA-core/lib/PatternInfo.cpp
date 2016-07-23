#include "CAPA/PatternInfo.h"

PatternInfo::PatternInfo(std::string Type, std::string Source)
{
    _source = Source;
}

PatternInfo::PatternInfo(const PatternInfo &C)
{
    _source = PatternInfo.dumpSource();
}

std::string PatternInfo::dumpSource()
{
    return _source;
}
