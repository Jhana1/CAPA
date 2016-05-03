#ifndef CAPA_HELPER_SUPPRESSHELPER_H
#define CAPA_HELPER_SUPPRESSHELPER_H

#include <clang/AST/AST.h>

#include "CAPA/RuleBase.h"

bool shouldSuppress(const clang::Decl *decl, clang::ASTContext &context, CAPA::RuleBase *rule);
bool shouldSuppress(const clang::Stmt *stmt, clang::ASTContext &context, CAPA::RuleBase *rule);
bool shouldSuppress(int beginLine, clang::ASTContext& context,
                    CAPA::RuleBase* rule = nullptr);

#endif
