#ifndef CAPA_DRIVER_H
#define CAPA_DRIVER_H

#include <string>

namespace llvm
{
    template<typename T> class ArrayRef;
}

namespace clang
{
namespace tooling
{
    class CompilationDatabase;
}
}

#include "CAPA/Analyzer.h"

namespace CAPA
{

class ViolationSet;

class Driver
{
public:
    void run(const clang::tooling::CompilationDatabase &compilationDatabase,
        llvm::ArrayRef<std::string> sourcePaths, CAPA::Analyzer &analyzer);
};

} // end namespace CAPA

#endif
