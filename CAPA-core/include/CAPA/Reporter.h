#ifndef CAPA_REPORTER_H
#define CAPA_REPORTER_H

#include "CAPA/BenchmarkSet.h"
#include <ostream>

namespace CAPA
{

class Results;

class Reporter
{
public:
    virtual ~Reporter() {}
    virtual void report(Results *results, BenchmarkSet &benchmarks, std::ostream &out) = 0;
    virtual const std::string name() const = 0;
};

} // end namespace CAPA

#endif
