#include "CAPA/helper/EnforceHelper.h"

#include "CAPA/helper/AttributeHelper.h"

bool declHasEnforceAttribute(
    const clang::Decl *decl,
    const CAPA::RuleBase& rule,
    std::string* comment) {
    return declHasActionAttribute(decl, "enforce", rule, comment);
}

