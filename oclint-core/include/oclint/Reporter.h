#ifndef CAPA_REPORTER_H
#define CAPA_REPORTER_H

#include <ostream>

namespace CAPA
{

class Results;

class Reporter
{
public:
    virtual ~Reporter() {}
    virtual void report(Results *results, std::ostream &out) = 0;
    virtual const std::string name() const = 0;
};

} // end namespace CAPA

#endif
