#include "CAPA/ViolationSet.h"
#include "CAPA/RuleBase.h"
#include <algorithm>
#include <iostream>

using namespace CAPA;

void ViolationSet::addViolation(const Violation& violation)
{
    const RuleBase *rule = violation.rule;
    _violations.push_back(violation);
}

void ViolationSet::filterViolations() 
{
    std::sort(_violations.begin(),
              _violations.end(),
              [](Violation a, Violation b){ return (a < b);}
              );
    _violations.erase(std::unique(_violations.begin(), _violations.end()), _violations.end());

    auto it = std::stable_partition(_violations.begin(), _violations.end(), [](Violation v){ 
            return v.patternInfo.pattern == "Vectorisable Function Declaration";});
    
    std::stable_partition(it, _violations.end(), [](Violation v){
            return v.patternInfo.pattern != "Vectorisable";});
}

void ViolationSet::removeViolation(const Violation& violation)
{
    auto start = _violations.begin();
    auto end   = _violations.end();
    auto rem   = std::find(start, end, violation);
    if (rem != end)
    {
        _violations.erase(rem);
    }
}

int ViolationSet::numberOfViolations() 
{
    filterViolations();
    return _violations.size();
}

const std::vector<Violation>& ViolationSet::getViolations()
{
    filterViolations();
    return _violations;
}


bool ViolationSet::operator==(const ViolationSet& rhs) const
{
    return _violations == rhs._violations;
}
