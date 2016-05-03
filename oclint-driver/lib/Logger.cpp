#include "CAPA/Logger.h"

#include <llvm/Support/Debug.h>

#include "CAPA/Options.h"

#ifndef NDEBUG

llvm::raw_ostream &CAPA::logger::debugStream()
{
    if (llvm::DebugFlag)
    {
        return llvm::dbgs();
    }
    return llvm::nulls();
}

#endif

llvm::raw_ostream &CAPA::logger::verboseStream()
{
#ifndef NDEBUG
    if (llvm::DebugFlag || CAPA::option::enableVerbose())
#else
    if (CAPA::option::enableVerbose())
#endif
    {
        return llvm::outs();
    }
    return llvm::nulls();
}
