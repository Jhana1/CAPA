#pragma once
#include "CAPA/AbstractASTMatcherRule.h"
#include "CAPA/util/MyASTUtil.h"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace CAPA;

namespace CAPA {

// Variable Binding Combinators
auto VarBind = [](std::string binding)
{
    return declRefExpr(to(varDecl().bind(binding)));
};

auto DVarBind = [](std::string binding)
{
    return hasDescendant(VarBind(binding));
};

auto VectorBind = [](std::string binding)
{
    return arraySubscriptExpr(
            hasBase(DVarBind(binding + "Base")),
            hasIndex(DVarBind(binding + "Index")));
};

auto VectorBindUnless = [](std::string binding, auto injectUnless)
{
    return arraySubscriptExpr(
            hasBase(DVarBind(binding + "Base")),
            hasIndex(DVarBind(binding + "Index")),
            unless(injectUnless));
};

auto VectorBindScan = [](std::string binding)
{
    return arraySubscriptExpr(
            hasBase(DVarBind(binding + "Base")),
            hasIndex(anyOf(
                binaryOperator(forEachDescendant(VarBind(binding + "IndexBO"))),
                DVarBind(binding + "Index"))));
};

auto MatrixBind = [](std::string binding)
{
    return arraySubscriptExpr(
            hasBase(hasDescendant(arraySubscriptExpr(
                hasBase(DVarBind(binding + "Base")),
                hasIndex(DVarBind(binding + "Row"))))),
            hasIndex(DVarBind(binding + "Column")));
};


// Loop Binding Combinators
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
                hasLHS(VarBind("IncVar" + level)),
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
            hasCondition(binaryOperator(hasRHS(expr().bind(binding + "CondRHS")))),
            hasIncrement(LoopIncrement(level)),
            hasBody(injectBody)).bind(binding);
};

auto ForLoopUnless = [](std::string binding, auto injectBody, auto injectUnless)
{
    return forStmt(
            hasBody(injectBody),
            unless(hasBody(injectUnless))).bind(binding);
};

auto WhileLoopUnless = [](std::string binding, auto injectBody, auto injectUnless)
{
    return whileStmt(
            hasBody(injectBody),
            unless(hasBody(injectUnless))).bind(binding);
};

// Operator Binding Combinators
auto BinaryOperatorBind = [](std::string operatorName, std::string binding, auto injectLeft, auto injectRight)
{
    return binaryOperator(
            hasOperatorName(operatorName),
            hasLHS(injectLeft),
            hasRHS(injectRight)).bind(binding);
};

auto BinaryOperatorBindUnless = [](std::string operatorName, std::string binding, auto injectLeft, auto injectRight, auto injectUnless)
{
    return binaryOperator(
            hasOperatorName(operatorName),
            hasLHS(injectLeft),
            hasRHS(injectRight),
            unless(injectUnless)).bind(binding);
};


auto BinaryOperatorBindAll = [](std::string binding, auto injectLeft, auto injectRight)
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

auto BinaryOperatorBindReduce = [](std::string binding, auto injectLeft, auto injectRight)
{
    return binaryOperator(
            hasOperatorName("="),
            hasLHS(injectLeft),
            hasRHS(injectRight),
            unless(hasDescendant(arraySubscriptExpr(hasDescendant(binaryOperator()))))).bind(binding);
};


auto BinaryOperatorBindReduceAll = [](std::string binding, auto injectLeft, auto injectRight)
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
            hasRHS(injectRight),
            unless(hasDescendant(arraySubscriptExpr(hasDescendant(binaryOperator()))))).bind(binding); 
};



} // End Namespace CAPA
