#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <map>

namespace ycc
{
    enum class TokenTag
    {
        // include the values of :
        //  Identifier, Keyword, Literal, Separator, Operator

        // Keywords
        ABSTRACT,   CONTINUE,   FOR,        NEW,        SWITCH,
        ASSERT,     DEFAULT,    IF,         PACKAGE,    SYNCHRONIZED,
        BOOLEAN,    DO,         GOTO,       PRIVATE,    THIS,
        BREAK,      DOUBLE,     IMPLEMENTS, PROTECTED,  THROW,
        BYTE,       ELSE,       IMPORT,     PUBLIC,     THROWS,
        CASE,       ENUM,       INSTANCEOF, RETURN,     TRANSIENT,
        CATCH,      EXTENDS,    INT,        SHORT,      TRY,
        CHAR,       FINAL,      INTERFACE,  STATIC,     VOID,
        CLASS,      FINALLY,    LONG,       STRICTFP,   VOLATILE,
        CONST,      FLOAT,      NATIVE,     SUPER,      WHILE,

        // Identifier
        IDENTIFIER,                 // such as abc, str

        // Literal
        INT_LITERAL,                // such as 1, 2 and so on
        REAL_LITERAL,               // such as 3.14
        CHAR_LITERAL,               // such as 'a', 'b'
        STR_LITERAL,                // such as "hello world"
        NONE,                       // null
        TRUE,                       // true
        FALSE,                      // false

        // Separator
        LEFT_PAREN,                 // (
        RIGHT_PAREN,                // )
        LEFT_SQUARE,                // [
        RIGHT_SQUARE,               // ]
        LEFT_BRACE,                 // {
        RIGHT_BRACE,                // }
        SEMICOLON,                  // ;
        COMMA,                      // ,
        PERIOD,                     // .

        // Operator
        ASSIGN,                     // =
        GREATER_THAN,               // >
        LESS_THAN,                  // <
        EQUAL,                      // ==
        LESS_OR_EQUAL,              // <=
        GREATER_OR_EQUAL,           // >=
        NOT_EQUAL,                  // !=
        LOGIC_AND,                  // &&
        LOGIC_OR,                   // ||
        NOT,                        // !
        TILDE,                      // ~
        QUESTION_MARK,              // ?
        COLON,                      // :
        INCRE,                      // ++
        DECRE,                      // --
        PLUS,                       // +
        MINUS,                      // -
        MULTIPLY,                   // *
        DIVIDE,                     // /
        AND,                        // &
        OR,                         // |
        XOR,                        // ^
        MOD,                        // %
        ADD_ASSIGN,                 // +=
        SUB_ASSIGN,                 // -=
        MUL_ASSIGN,                 // *=
        DIV_ASSIGN,                 // /=
        AND_ASSIGN,                 // &=
        OR_ASSIGN,                  // |=
        XOR_ASSIGN,                 // ^=
        MOD_ASSIGN,                 // %=
        SHL,                        // <<
        SHR,                        // >>
        SHL_ASSIGN,                 // <<=
        SHR_ASSIGN,                 // >>=
        UNSIGNED_SHR,               // >>>
        UNSIGNED_SHR_ASSIGN,        // >>>=

        // others
        END_OF_FILE,                // EOF
        UNRESERVED
    };

    class TokenLocation
    {
    public:
        TokenLocation();
        TokenLocation(const std::string &fileName, int line, int column);
        std::string toString() const;
    private:
        std::string                 filename_;
        int                         line_;
        int                         column_;
    };

    class Token
    {
    public:
        Token();
        Token(std::string lexeme, TokenTag tag);

        std::string                 lexeme() const;
        TokenTag                    tag() const;
        std::string                 desc() const;
        std::string                 toString() const;
    private:
        std::string                 lexeme_;
        TokenTag                    tag_;
    };


    inline std::string Token::lexeme() const
    {
        return lexeme_;
    }

    inline TokenTag Token::tag() const
    {
        return tag_;
    }

    class Dictionary
    {
    public:
        Dictionary();
        TokenTag    lookup(const std::string &lexeme) const;
        bool        has(const std::string &lexeme) const;
    private:
        void        add(std::string lexeme, TokenTag);

    private:
        std::map<std::string, TokenTag> dictionary_;
    };

}

#endif
