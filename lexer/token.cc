#include "token.h"
#include "../common/symbols.h"

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
            + std::to_string(column_);
    }

    Token::Token()
        :Token("", TokenTag::UNRESERVED)
    {}

    Token::Token(std::string lexeme, TokenTag tag)
        : lexeme_(lexeme), tag_(tag)
    {}

    std::string Token::desc() const
    {
        return tokenDesc(tag_);
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
