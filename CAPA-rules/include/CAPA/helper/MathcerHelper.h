#pragma once
#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/util/MyASTUtil.h"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace CAPA;

auto VarBind = [](std::string binding)
{
    return declRefExpr(to(varDecl().bind(binding)));
};

auto DVarBind = [](std::string binding)
{
    return hasDescendant(VarBind(binding));
};

auto LoopInit = [](std::string level)
{
    return anyOf(
        declStmt(hasSingleDecl(varDecl(hasInitializer(
            integerLiteral(anything()))).bind("InitVar" + level))),
        binaryOperator(
            hasOperatorName("="),
            hasLHS(VarBind("InitVar" + level))));
};

auto LoopIncrement = [](std::string level) 
{
    return anyOf(
            unaryOperator(anyOf(
                    hasOperatorName("++"),
                    hasOperatorName("--")),
                hasUnaryOperand(VarBind("IncVar" + level))),
            binaryOperator(anyOf(
                    hasOperatorName("+="),
                    hasOperatorName("-=")),
                hasLHS(DVarBind("IncVar" + level)),
                hasRHS(expr().bind("Stride" + level))));
};

auto WhileLoop = [](std::string binding, std::string level, auto injectBody)
{
    return whileStmt(
            hasCondition(anyOf(
                hasDescendant(unaryOperator(anyOf(
                        hasOperatorName("++"),
                        hasOperatorName("--")),
                    hasUnaryOperand(DVarBind("IncVar" + level)))),
                hasDescendant(binaryOperator(
                    anyOf(
                        hasOperatorName("+="),
                        hasOperatorName("-=")
                    ),
                    hasLHS(DVarBind("IncVar" + level)),
                    hasRHS(expr().bind("Stride" + level))
            )))),
            hasBody(injectBody)).bind(binding + level);
};

auto ForLoop = [](std::string binding, std::string level, auto injectBody)
{
    return forStmt(
            hasLoopInit(LoopInit(level)),
            hasIncrement(LoopIncrement(level)),
            hasBody(injectBody)).bind(binding);
};

auto MatrixBind = [](std::string binding)
{
    return arraySubscriptExpr(
            hasBase(hasDescendant(arraySubscriptExpr(
                hasBase(DVarBind(binding + "Base")),
                hasIndex(DVarBind(binding + "Row"))))),
            hasIndex(DVarBind(binding + "Column")));
};

auto VectorBind = [](std::string binding)
{
    return arraySubscriptExpr(
            hasBase(DVarBind(binding + "Base")),
            hasIndex(DVarBind(binding + "Index")));
};

auto VectorScanBind = [](std::string binding)
{
    return arraySubscriptExpr(
            hasBase(DVarBind(binding + "Base")),
            hasIndex(anyOf(
                binaryOperator(forEachDescendant(VarBind(binding + "IndexBO"))),
                DVarBind(binding + "Index"))));
};

auto BinaryOperatorBind = [](std::string operatorName, std::string binding, auto injectLeft, auto injectRight)
{
    return binaryOperator(
            hasOperatorName(operatorName),
            hasLHS(injectLeft),
            hasRHS(injectRight)).bind(binding);
};

auto AllBinaryOperatorBind = [](std::string binding, auto injectLeft, auto injectRight)
{
    return binaryOperator(anyOf(
                hasOperatorName("+="),
                hasOperatorName("-="),
                hasOperatorName("/="),
                hasOperatorName("*="),
                hasOperatorName("<<="),
                hasOperatorName(">>="),
                hasOperatorName("|="),
                hasOperatorName("&="),
                hasOperatorName("%="),
                hasOperatorName("^=")),
            hasLHS(injectLeft),
            hasRHS(injectRight)).bind(binding);
};
