#pragma once
#include <cstdarg>
#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/RuleSet.h"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace CAPA;


bool areSameVariable(int n, ...)
{
    va_list args;
    va_start(args,n);
    VarDecl *tmp = va_arg(args, VarDecl*)->getCanonicalDecl();
    for (int i = 1; i < n; ++i)
    {
        VarDecl* node = va_arg(args, VarDecl*);
        if (!node || node->getCanonicalDecl() != tmp)
            return false;
    }
    return true;
}

template <typename T>
std::string node2str(const T *node, SourceManager &sm)
{
    string text = Lexer::getSourceText(CharSourceRange::getTokenRange(node->getSourceRange()),
            sm, LangOptions(), 0);
    if (text.at(text.size()-1) == ',')
        return Lexer::getSourceText(CharSourceRange::getCharRange(node->getSourceRange()),
                sm, LangOptions(), 0);
    else
        return text;
    }
