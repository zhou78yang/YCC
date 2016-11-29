#include "symbols.h"
#include "symbol_table.h"
#include <iostream>

namespace ycc
{
    int getSymbolPrecedence(TokenTag tag)
    {
        int ret = 0;
        switch(tag)
        {
            case TokenTag::ASSIGN:                     // =
            case TokenTag::ADD_ASSIGN:                 // +=
            case TokenTag::SUB_ASSIGN:                 // -=
            case TokenTag::MUL_ASSIGN:                 // *=
            case TokenTag::DIV_ASSIGN:                 // /=
            case TokenTag::AND_ASSIGN:                 // &=
            case TokenTag::OR_ASSIGN:                  // |=
            case TokenTag::XOR_ASSIGN:                 // ^=
            case TokenTag::MOD_ASSIGN:                 // %=
            case TokenTag::SHL_ASSIGN:                 // <<=
            case TokenTag::SHR_ASSIGN:                 // >>=
            case TokenTag::UNSIGNED_SHR_ASSIGN:        // >>>=
                ret = 0x10;
                break;
            case TokenTag::QUESTION_MARK:              // ?
            case TokenTag::COLON:                      // :
                ret = 0x20;
                break;
            case TokenTag::LOGIC_OR:                   // ||
                ret = 0x30;
                break;
            case TokenTag::LOGIC_AND:                  // &&
                ret = 0x31;
                break;
            case TokenTag::OR:                         // |
                ret = 0x40;
                break;
            case TokenTag::XOR:                        // ^
                ret = 0x41;
                break;
            case TokenTag::AND:                        // &
                ret = 0x42;
                break;
            case TokenTag::EQUAL:                      // ==
            case TokenTag::NOT_EQUAL:                  // !=
                ret = 0x50;
                break;
            case TokenTag::GREATER_THAN:               // >
            case TokenTag::LESS_THAN:                  // <
            case TokenTag::LESS_OR_EQUAL:              // <=
            case TokenTag::GREATER_OR_EQUAL:           // >=
            case TokenTag::INSTANCEOF:                 // instanceof
                ret = 0x51;
                break;
            case TokenTag::SHL:                        // <<
            case TokenTag::SHR:                        // >>
            case TokenTag::UNSIGNED_SHR:               // >>>
                ret = 0x52;
                break;
            case TokenTag::PLUS:                       // +
            case TokenTag::MINUS:                      // -
                ret = 0x60;
                break;
            case TokenTag::MULTIPLY:                   // *
            case TokenTag::DIVIDE:                     // /
            case TokenTag::MOD:                        // %
                ret = 0x61;
                break;
            case TokenTag::NOT:                        // !
            case TokenTag::TILDE:                      // ~
            case TokenTag::INCRE:                      // ++
            case TokenTag::DECRE:                      // --
                ret = 0x70;
                break;
            case TokenTag::LEFT_SQUARE:                // [
            case TokenTag::RIGHT_SQUARE:               // ]
            case TokenTag::LEFT_PAREN:                 // (
            case TokenTag::RIGHT_PAREN:                // )
                ret = 0x71;
                break;
            default:
                ret = 0;
                break;
        }
        return ret;
    }

    bool isAssignmentOperator(TokenTag tag)
    {
        switch(tag)
        {
            case TokenTag::ASSIGN:                     // =
            case TokenTag::ADD_ASSIGN:                 // +=
            case TokenTag::SUB_ASSIGN:                 // -=
            case TokenTag::MUL_ASSIGN:                 // *=
            case TokenTag::DIV_ASSIGN:                 // /=
            case TokenTag::AND_ASSIGN:                 // &=
            case TokenTag::OR_ASSIGN:                  // |=
            case TokenTag::XOR_ASSIGN:                 // ^=
            case TokenTag::MOD_ASSIGN:                 // %=
            case TokenTag::SHL_ASSIGN:                 // <<=
            case TokenTag::SHR_ASSIGN:                 // >>=
            case TokenTag::UNSIGNED_SHR_ASSIGN:        // >>>=
                return true;
            default:
                break;
        }
        return false;
    }

    bool isInfixOp(TokenTag tag)
    {
        switch(tag)
        {
            case TokenTag::LOGIC_OR:                   // ||
            case TokenTag::LOGIC_AND:                  // &&
            case TokenTag::OR:                         // |
            case TokenTag::XOR:                        // ^
            case TokenTag::AND:                        // &
            case TokenTag::EQUAL:                      // ==
            case TokenTag::NOT_EQUAL:                  // !=
            case TokenTag::GREATER_THAN:               // >
            case TokenTag::LESS_THAN:                  // <
            case TokenTag::LESS_OR_EQUAL:              // <=
            case TokenTag::GREATER_OR_EQUAL:           // >=
            case TokenTag::SHL:                        // <<
            case TokenTag::SHR:                        // >>
            case TokenTag::UNSIGNED_SHR:               // >>>
            case TokenTag::PLUS:                       // +
            case TokenTag::MINUS:                      // -
            case TokenTag::MULTIPLY:                   // *
            case TokenTag::DIVIDE:                     // /
            case TokenTag::MOD:                        // %
            case TokenTag::COLON:                      // : (!)
                return true;
            default:
                break;
        }
        return isAssignmentOperator(tag);
    }

    bool isPrefixOp(TokenTag tag)
    {
        switch(tag)
        {
            case TokenTag::NOT:                        // !
            case TokenTag::TILDE:                      // ~
            case TokenTag::INCRE:                      // ++
            case TokenTag::DECRE:                      // --
            case TokenTag::PLUS:                       // +
            case TokenTag::MINUS:                      // -
                return true;
            default:
                break;
        }
        return false;
    }

    bool isPostfixOp(TokenTag tag)
    {
        switch(tag)
        {
            case TokenTag::INCRE:                      // ++
            case TokenTag::DECRE:                      // --
                return true;
            default:
                break;
        }
        return false;
    }

    int isModifier(TokenTag tag)
    {
        int m = 0;
        switch(tag)
        {
            case TokenTag::PUBLIC:
                m = SymbolTag::PUBLIC;
                break;
            case TokenTag::PROTECTED:
                m = SymbolTag::PROTECTED;
                break;
            case TokenTag::PRIVATE:
                m = SymbolTag::PRIVATE;
                break;
            case TokenTag::STATIC:
                m = SymbolTag::STATIC;
                break;
            case TokenTag::ABSTRACT:
                m = SymbolTag::ABSTRACT;
                break;
            case TokenTag::FINAL:
                m = SymbolTag::FINAL;
                break;
            case TokenTag::NATIVE:
                m = SymbolTag::NATIVE;
                break;
            case TokenTag::SYNCHRONIZED:
                m = SymbolTag::SYNCHRONIZED;
                break;
            case TokenTag::TRANSIENT:
                m = SymbolTag::TRANSIENT;
                break;
            case TokenTag::VOLATILE:
                m = SymbolTag::VOLATILE;
                break;
            case TokenTag::STRICTFP:
                m = SymbolTag::STRICTFP;
                break;
            default:
                break;
        }

        return m;
    }

    std::string tokenDesc(TokenTag tag)
    {
        std::string ret = "";
        switch(tag)
        {
        case TokenTag::ABSTRACT:
        	ret = "abstract";
        	break;
        case TokenTag::ASSERT:
        	ret = "assert";
        	break;
        case TokenTag::BOOLEAN:
        	ret = "boolean";
        	break;
        case TokenTag::BREAK:
        	ret = "break";
        	break;
        case TokenTag::BYTE:
        	ret = "byte";
        	break;
        case TokenTag::CASE:
        	ret = "case";
        	break;
        case TokenTag::CATCH:
        	ret = "catch";
        	break;
        case TokenTag::CHAR:
        	ret = "char";
        	break;
        case TokenTag::CLASS:
        	ret = "class";
        	break;
        case TokenTag::CONST:
        	ret = "const";
        	break;
        case TokenTag::CONTINUE:
        	ret = "continue";
        	break;
        case TokenTag::DEFAULT:
        	ret = "default";
        	break;
        case TokenTag::DO:
        	ret = "do";
        	break;
        case TokenTag::DOUBLE:
        	ret = "double";
        	break;
        case TokenTag::ELSE:
        	ret = "else";
        	break;
        case TokenTag::ENUM:
        	ret = "enum";
        	break;
        case TokenTag::EXTENDS:
        	ret = "extends";
        	break;
        case TokenTag::FINAL:
        	ret = "final";
        	break;
        case TokenTag::FINALLY:
        	ret = "finally";
        	break;
        case TokenTag::FLOAT:
        	ret = "float";
        	break;
        case TokenTag::FOR:
        	ret = "for";
        	break;
        case TokenTag::IF:
        	ret = "if";
        	break;
        case TokenTag::GOTO:
        	ret = "goto";
        	break;
        case TokenTag::IMPLEMENTS:
        	ret = "implements";
        	break;
        case TokenTag::IMPORT:
        	ret = "import";
        	break;
        case TokenTag::INSTANCEOF:
        	ret = "instanceof";
        	break;
        case TokenTag::INT:
        	ret = "int";
        	break;
        case TokenTag::INTERFACE:
        	ret = "interface";
        	break;
        case TokenTag::LONG:
        	ret = "long";
        	break;
        case TokenTag::NATIVE:
        	ret = "native";
        	break;
        case TokenTag::NEW:
        	ret = "new";
        	break;
        case TokenTag::PACKAGE:
        	ret = "package";
        	break;
        case TokenTag::PRIVATE:
        	ret = "private";
        	break;
        case TokenTag::PROTECTED:
        	ret = "protected";
        	break;
        case TokenTag::PUBLIC:
        	ret = "public";
        	break;
        case TokenTag::RETURN:
        	ret = "return";
        	break;
        case TokenTag::SHORT:
        	ret = "short";
        	break;
        case TokenTag::STATIC:
        	ret = "static";
        	break;
        case TokenTag::STRICTFP:
        	ret = "strictfp";
        	break;
        case TokenTag::SUPER:
        	ret = "super";
        	break;
        case TokenTag::SWITCH:
        	ret = "switch";
        	break;
        case TokenTag::SYNCHRONIZED:
        	ret = "synchronized";
        	break;
        case TokenTag::THIS:
        	ret = "this";
        	break;
        case TokenTag::THROW:
        	ret = "throw";
        	break;
        case TokenTag::THROWS:
        	ret = "throws";
        	break;
        case TokenTag::TRANSIENT:
        	ret = "transient";
        	break;
        case TokenTag::TRY:
        	ret = "try";
        	break;
        case TokenTag::VOID:
        	ret = "void";
        	break;
        case TokenTag::VOLATILE:
        	ret = "volatile";
        	break;
        case TokenTag::WHILE:
        	ret = "while";
        	break;
        case TokenTag::IDENTIFIER:
            ret = "identifier";
            break;
        case TokenTag::INT_LITERAL:
            ret = "int literal";
            break;
        case TokenTag::REAL_LITERAL:
            ret = "real literal";
            break;
        case TokenTag::CHAR_LITERAL:
            ret = "char literal";
            break;
        case TokenTag::STR_LITERAL:
            ret = "str literal";
            break;
        case TokenTag::NONE:
        	ret = "none";
        	break;
        case TokenTag::TRUE:
        	ret = "true";
        	break;
        case TokenTag::FALSE:
        	ret = "false";
        	break;
        case TokenTag::LEFT_PAREN:
        	ret = "left paren";
        	break;
        case TokenTag::RIGHT_PAREN:
        	ret = "right paren";
        	break;
        case TokenTag::LEFT_SQUARE:
        	ret = "left square";
        	break;
        case TokenTag::RIGHT_SQUARE:
        	ret = "right square";
        	break;
        case TokenTag::LEFT_BRACE:
        	ret = "left brace";
        	break;
        case TokenTag::RIGHT_BRACE:
        	ret = "right brace";
        	break;
        case TokenTag::SEMICOLON:
        	ret = "semicolon";
        	break;
        case TokenTag::COMMA:
        	ret = "comma";
        	break;
        case TokenTag::PERIOD:
        	ret = "period";
        	break;
        case TokenTag::ASSIGN:
        	ret = "assign";
        	break;
        case TokenTag::GREATER_THAN:
        	ret = "greater than";
        	break;
        case TokenTag::LESS_THAN:
        	ret = "less than";
        	break;
        case TokenTag::EQUAL:
        	ret = "equal";
        	break;
        case TokenTag::LESS_OR_EQUAL:
        	ret = "less or equal";
        	break;
        case TokenTag::GREATER_OR_EQUAL:
        	ret = "greater or equal";
        	break;
        case TokenTag::NOT_EQUAL:
        	ret = "not equal";
        	break;
        case TokenTag::LOGIC_AND:
        	ret = "logic and";
        	break;
        case TokenTag::LOGIC_OR:
        	ret = "logic or";
        	break;
        case TokenTag::NOT:
        	ret = "not";
        	break;
        case TokenTag::TILDE:
        	ret = "tilde";
        	break;
        case TokenTag::QUESTION_MARK:
        	ret = "question mark";
        	break;
        case TokenTag::COLON:
        	ret = "colon";
        	break;
        case TokenTag::INCRE:
        	ret = "incre";
        	break;
        case TokenTag::DECRE:
        	ret = "decre";
        	break;
        case TokenTag::PLUS:
        	ret = "plus";
        	break;
        case TokenTag::MINUS:
        	ret = "minus";
        	break;
        case TokenTag::MULTIPLY:
        	ret = "multiply";
        	break;
        case TokenTag::DIVIDE:
        	ret = "divide";
        	break;
        case TokenTag::AND:
        	ret = "and";
        	break;
        case TokenTag::OR:
        	ret = "or";
        	break;
        case TokenTag::XOR:
        	ret = "xor";
        	break;
        case TokenTag::MOD:
        	ret = "mod";
        	break;
        case TokenTag::ADD_ASSIGN:
        	ret = "add assign";
        	break;
        case TokenTag::SUB_ASSIGN:
        	ret = "sub assign";
        	break;
        case TokenTag::MUL_ASSIGN:
        	ret = "mul assign";
        	break;
        case TokenTag::DIV_ASSIGN:
        	ret = "div assign";
        	break;
        case TokenTag::AND_ASSIGN:
        	ret = "and assign";
        	break;
        case TokenTag::OR_ASSIGN:
        	ret = "or assign";
        	break;
        case TokenTag::XOR_ASSIGN:
        	ret = "xor assign";
        	break;
        case TokenTag::MOD_ASSIGN:
        	ret = "mod assign";
        	break;
        case TokenTag::SHL:
        	ret = "shl";
        	break;
        case TokenTag::SHR:
        	ret = "shr";
        	break;
        case TokenTag::SHL_ASSIGN:
        	ret = "shl assign";
        	break;
        case TokenTag::SHR_ASSIGN:
        	ret = "shr assign";
        	break;
        case TokenTag::UNSIGNED_SHR:
        	ret = "unsigned shr";
        	break;
        case TokenTag::UNSIGNED_SHR_ASSIGN:
        	ret = "unsigned shr assign";
        	break;
        case TokenTag::END_OF_FILE:
            ret = "eof";
            break;
        case TokenTag::UNRESERVED:
            ret = "unreserved";
            break;
        default:
            break;
        }
        return ret;
    }
}
