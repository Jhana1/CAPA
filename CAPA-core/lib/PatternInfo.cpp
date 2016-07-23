#include "CAPA/PatternInfo.h"

PatternInfo::PatternInfo(std::string Type, std::string Source)
{
    _source = Source;
}

PatternInfo::PatternInfo(const PatternInfo &C)
{
    _source = C.dumpSource();
}

PatternInfo::PatternInfo()
{
    _source = "";
}

std::string PatternInfo::dumpSource() const
{
    return _source;
}
