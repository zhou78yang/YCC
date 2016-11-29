#ifndef SCANNER_H_
#define SCANNER_H_

#include <fstream>
#include <string>
#include "token.h"
#include "../common/error.h"

namespace ycc
{
    class Scanner
    {
    public:
        enum class State
        {
            NONE,
            END_OF_FILE,
            IDENTIFIER,
            NUMBER,
            STRING,
            OPERATOR
        };
    public:
        explicit        Scanner(const std::string &filename);
        const Token &   getToken() const;
        Token           getNextToken();
        TokenLocation   getTokenLocation() const;

        static bool     getErrorFlag();
        static void     setErrorFlag(bool flag);

    private:

        void            getNextChar();
        char            peekChar();
        void            addToBuffer(char c);
        void            reduceBuffer();
        void            makeToken(std::string name, TokenTag tag);
        void            updateLocation();

        void            preprocess();
        void            handleLineComment();
        void            handleBlockComment();
        void            handleEOFState();
        void            handleIdentifierState();
        void            handleNumberState();
        void            handleRealNumberState(bool hex = false);
        void            handleExponentState();
        void            handleDecNumberState();
        void            handleOctNumberState();
        void            handleHexNumberState();
        void            handleStringState();
        void            handleEscapeSeqState();
        void            handleOperationState();

        void            errorReport(const std::string &msg);

    private:
        // locaation
        std::string     filename_;
        std::ifstream   input_;
        long            line_;
        long            column_;
        TokenLocation   loc_;
        // character and token
        char            currentChar_;
        State           state_;
        Token           token_;
        Dictionary      dictionary_;
        std::string     buffer_;
        // error flag
        static bool     errorFlag_;
        // temp value
        long long       intValue_;      // int char byte shot long
        double          realValue_;     // float double
        std::string     strValue_;      // char[]
    };

    inline const Token &Scanner::getToken() const
    {
        return token_;
    }

    inline bool Scanner::getErrorFlag()
    {
        return errorFlag_;
    }

    inline void Scanner::setErrorFlag(bool flag)
    {
        errorFlag_ = flag;
    }

    inline TokenLocation Scanner::getTokenLocation() const
    {
        return loc_;
    }

    inline void Scanner::updateLocation()
    {
        loc_ = TokenLocation(filename_, line_, column_);
    }

}

#endif
