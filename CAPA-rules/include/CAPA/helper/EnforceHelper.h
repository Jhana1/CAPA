#ifndef CAPA_HELPER_ENFORCEHELPER_H
#define CAPA_HELPER_ENFORCEHELPER_H

#include <string>

namespace clang { class Decl; }
namespace CAPA { class RuleBase; }

bool declHasEnforceAttribute(
    const clang::Decl *decl,
    const CAPA::RuleBase& rule,
    std::string* comment = nullptr);

#endif
