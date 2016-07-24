#include "CAPA/ViolationSet.h"
#include <algorithm>

using namespace CAPA;

void ViolationSet::addViolation(const Violation& violation)
{
    _violations.push_back(violation);
}

void ViolationSet::removeViolation(const Violation& violation)
{
    auto start = _violations.begin();
    auto end   = _violations.end();
    auto rem   = std::find(start, end, violation);
    _violations.erase(rem);
}

int ViolationSet::numberOfViolations() const
{
    return _violations.size();
}

const std::vector<Violation>& ViolationSet::getViolations() const
{
    return _violations;
}


bool ViolationSet::operator==(const ViolationSet& rhs) const
{
    return _violations == rhs._violations;
}
