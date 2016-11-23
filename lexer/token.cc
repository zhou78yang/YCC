#include "token.h"

namespace ycc
{

    TokenLocation::TokenLocation(const std::string &filename, int line, int column)
        : filename_(filename), line_(line), column_(column)
    {}

    TokenLocation::TokenLocation()
        : filename_(""), line_(1), column_(0)
    {}

    std::string TokenLocation::toString() const
    {
        return filename_ + "/"
            + std::to_string(line_) + "/"
            + std::to_string(column_) + ":";
    }

    Token::Token()
        :Token("", TokenTag::UNRESERVED)
    {}

    Token::Token(std::string lexeme, TokenTag tag)
        : lexeme_(lexeme), tag_(tag)
    {}

    std::string Token::desc() const
    {
        std::string ret = "";
        switch(tag_)
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

    std::string Token::toString() const
    {
        return "(" + lexeme_ + ", " + desc() + ")";
    }


    Dictionary::Dictionary()
    {
        // Keyword
        add("abstract",         TokenTag::ABSTRACT);
        add("assert",           TokenTag::ASSERT);
        add("boolean",          TokenTag::BOOLEAN);
        add("break",            TokenTag::BREAK);
        add("byte",             TokenTag::BYTE);
        add("case",             TokenTag::CASE);
        add("catch",            TokenTag::CATCH);
        add("char",             TokenTag::CHAR);
        add("class",            TokenTag::CLASS);
        add("const",            TokenTag::CONST);
        add("continue",         TokenTag::CONTINUE);
        add("default",          TokenTag::DEFAULT);
        add("do",               TokenTag::DO);
        add("double",           TokenTag::DOUBLE);
        add("else",             TokenTag::ELSE);
        add("enum",             TokenTag::ENUM);
        add("extends",          TokenTag::EXTENDS);
        add("final",            TokenTag::FINAL);
        add("finally",          TokenTag::FINALLY);
        add("float",            TokenTag::FLOAT);
        add("for",              TokenTag::FOR);
        add("if",               TokenTag::IF);
        add("goto",             TokenTag::GOTO);
        add("implements",       TokenTag::IMPLEMENTS);
        add("import",           TokenTag::IMPORT);
        add("instanceof",       TokenTag::INSTANCEOF);
        add("int",              TokenTag::INT);
        add("interface",        TokenTag::INTERFACE);
        add("long",             TokenTag::LONG);
        add("native",           TokenTag::NATIVE);
        add("new",              TokenTag::NEW);
        add("package",          TokenTag::PACKAGE);
        add("private",          TokenTag::PRIVATE);
        add("protected",        TokenTag::PROTECTED);
        add("public",           TokenTag::PUBLIC);
        add("return",           TokenTag::RETURN);
        add("short",            TokenTag::SHORT);
        add("static",           TokenTag::STATIC);
        add("strictfp",         TokenTag::STRICTFP);
        add("super",            TokenTag::SUPER);
        add("switch",           TokenTag::SWITCH);
        add("synchronized",     TokenTag::SYNCHRONIZED);
        add("this",             TokenTag::THIS);
        add("throw",            TokenTag::THROW);
        add("throws",           TokenTag::THROWS);
        add("transient",        TokenTag::TRANSIENT);
        add("try",              TokenTag::TRY);
        add("void",             TokenTag::VOID);
        add("volatile",         TokenTag::VOLATILE);
        add("while",            TokenTag::WHILE);

        // Literal
        add("null",             TokenTag::NONE);
        add("true",             TokenTag::TRUE);
        add("false",            TokenTag::FALSE);

        // Separator
        add("(",                TokenTag::LEFT_PAREN);
        add(")",                TokenTag::RIGHT_PAREN);
        add("[",                TokenTag::LEFT_SQUARE);
        add("]",                TokenTag::RIGHT_SQUARE);
        add("{",                TokenTag::LEFT_BRACE);
        add("}",                TokenTag::RIGHT_BRACE);
        add(";",                TokenTag::SEMICOLON);
        add(",",                TokenTag::COMMA);
        add(".",                TokenTag::PERIOD);

        // Operator
        add("=",                TokenTag::ASSIGN);
        add(">",                TokenTag::GREATER_THAN);
        add("<",                TokenTag::LESS_THAN);
        add("==",               TokenTag::EQUAL);
        add("<=",               TokenTag::LESS_OR_EQUAL);
        add(">=",               TokenTag::GREATER_OR_EQUAL);
        add("!=",               TokenTag::NOT_EQUAL);
        add("&&",               TokenTag::LOGIC_AND);
        add("||",               TokenTag::LOGIC_OR);
        add("!",                TokenTag::NOT);
        add("~",                TokenTag::TILDE);
        add("?",                TokenTag::QUESTION_MARK);
        add(":",                TokenTag::COLON);
        add("++",               TokenTag::INCRE);
        add("--",               TokenTag::DECRE);
        add("+",                TokenTag::PLUS);
        add("-",                TokenTag::MINUS);
        add("*",                TokenTag::MULTIPLY);
        add("/",                TokenTag::DIVIDE);
        add("&",                TokenTag::AND);
        add("|",                TokenTag::OR);
        add("^",                TokenTag::XOR);
        add("%",                TokenTag::MOD);
        add("+=",               TokenTag::ADD_ASSIGN);
        add("-=",               TokenTag::SUB_ASSIGN);
        add("*=",               TokenTag::MUL_ASSIGN);
        add("/=",               TokenTag::DIV_ASSIGN);
        add("&=",               TokenTag::AND_ASSIGN);
        add("|=",               TokenTag::OR_ASSIGN);
        add("^=",               TokenTag::XOR_ASSIGN);
        add("%=",               TokenTag::MOD_ASSIGN);
        add("<<",               TokenTag::SHL);
        add(">>",               TokenTag::SHR);
        add("<<=",              TokenTag::SHL_ASSIGN);
        add(">>=",              TokenTag::SHR_ASSIGN);
        add(">>>",              TokenTag::UNSIGNED_SHR);
        add(">>>=",             TokenTag::UNSIGNED_SHR_ASSIGN);
    }

    void Dictionary::add(std::string lexeme, TokenTag tag)
    {
        dictionary_.insert(std::pair<std::string, TokenTag>(lexeme, tag));
    }

    TokenTag Dictionary::lookup(const std::string &lexeme) const
    {
        TokenTag tag = TokenTag::UNRESERVED;
        auto iter = dictionary_.find(lexeme);

        if(iter != dictionary_.end())
        {
            tag = iter->second;
        }

        return tag;
    }

    bool Dictionary::has(const std::string &lexeme) const
    {
        auto iter = dictionary_.find(lexeme);

        if(iter != dictionary_.end())
        {
            return true;
        }
        return false;
    }
}
