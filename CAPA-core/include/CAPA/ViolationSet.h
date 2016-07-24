#ifndef CAPA_VIOLATIONSET_H
#define CAPA_VIOLATIONSET_H

#include <vector>

#include "CAPA/Violation.h"

namespace CAPA
{

class ViolationSet
{
private:
    std::vector<Violation> _violations;

public:
    void addViolation(const Violation& violation);
    void removeViolation(const Violation& violation);
    int numberOfViolations() const;
    const std::vector<Violation>& getViolations() const;

    bool operator==(const ViolationSet& rhs) const;

    // TODO: getViolation(int index)
};

} // end namespace CAPA

#endif
