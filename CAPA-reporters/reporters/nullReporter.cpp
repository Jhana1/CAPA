#include "CAPA/Reporter.h"

using namespace CAPA;

class nullReporter : public Reporter
{
public:
    virtual const std::string name() const override
    {
        return "null";
    }

    virtual void report(Results *results, std::ostream &out) override
    {
    }
};

extern "C" Reporter* create()
{
  return new nullReporter();
}
