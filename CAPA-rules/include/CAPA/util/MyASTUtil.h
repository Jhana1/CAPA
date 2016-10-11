#pragma once
#include <string>
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
    bool invalid;
    StringRef rawText = Lexer::getSourceText(CharSourceRange::getTokenRange(node->getSourceRange()),
            sm, LangOptions(), &invalid);
    
    std::string text = rawText.str();

    if (invalid) { return ""; }

    if (text.at(text.size()-1) == ',')
        return Lexer::getSourceText(CharSourceRange::getCharRange(node->getSourceRange()),
                sm, LangOptions(), 0);
    else
        return text;
}

bool markedIgnore(const Decl *d, SourceManager &sm)
{
    if (!d) { return false; }
    const RawComment *rc = d->getASTContext().getRawCommentForDeclNoCache(d);
    if (rc)
    {
        SourceRange range = rc->getSourceRange();

        PresumedLoc startPos = sm.getPresumedLoc(range.getBegin());
        PresumedLoc endPos = sm.getPresumedLoc(range.getEnd());

        std::string raw = rc->getBriefText(d->getASTContext());
        return (raw.find("CAPA:IGNORE") != std::string::npos);
    }
    return false;
}
