#ifndef CAPA_LOGGER_H
#define CAPA_LOGGER_H

#include <llvm/Support/raw_ostream.h>

namespace CAPA
{
namespace logger
{
#ifndef NDEBUG
    llvm::raw_ostream &debugStream();
#endif
    llvm::raw_ostream &verboseStream();
} // end namespace logger
} // end namespace CAPA

#ifndef NDEBUG

#define LOG_DEBUG(MSG) CAPA::logger::debugStream() << MSG
#define LOG_DEBUG_LINE(MSG) LOG_DEBUG(MSG << "\n")

#else

#define LOG_DEBUG(MSG)
#define LOG_DEBUG_LINE(MSG)

#endif

#define LOG_VERBOSE(MSG) CAPA::logger::verboseStream() << MSG
#define LOG_VERBOSE_LINE(MSG) LOG_VERBOSE(MSG << "\n")

#endif
