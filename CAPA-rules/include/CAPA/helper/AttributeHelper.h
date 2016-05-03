#ifndef CAPA_HELPER_ATTRIBUTEHELPER_H
#define CAPA_HELPER_ATTRIBUTEHELPER_H

#include <string>

namespace clang { class Decl; }
namespace CAPA { class RuleBase; }

bool declHasCAPAAttribute(
    const clang::Decl *decl,
    const std::string& attributeName,
    std::string* comment = nullptr);

bool declHasActionAttribute(
    const clang::Decl *decl,
    const std::string& action,
    const CAPA::RuleBase& rule,
    std::string* comment = nullptr);

#endif
