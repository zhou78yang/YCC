#ifndef SYMBOLS_H_
#define SYMBOLS_H_

#include "../lexer/token.h"

namespace ycc
{
    extern int  getSymbolPrecedence(TokenTag tag);
    extern bool isAssignmentOperator(TokenTag tag);
    extern bool isInfixOp(TokenTag tag);
    extern bool isPrefixOp(TokenTag tag);
    extern bool isPostfixOp(TokenTag tag);
    extern int  isModifier(TokenTag tag);
    extern bool isBasicType(TokenTag tag);
    extern std::string tokenDesc(TokenTag tag);
}

#endif
