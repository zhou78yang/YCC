#include <iostream>
#include "scanner.h"

namespace ycc
{

    bool Scanner::errorFlag_ = false;

    Scanner::Scanner(const std::string &srcFileName)
        : filename_(srcFileName), line_(1), column_(0),
            currentChar_(0), state_(State::NONE)
    {
        input_.open(filename_, std::ios::in);

        if(input_.fail())
        {
            errorReport("When trying to open file " + filename_
                        + ", file open failed.");
        }
    }

    // some character handlering functions
    void Scanner::getNextChar()
    {
        if(input_.eof())
        {
            input_.close();
        }
        currentChar_ = input_.get();

        if(currentChar_ == '\n')
        {
            line_++;
            column_ = 0;
        }
        else
        {
            column_++;
        }
    }

    char Scanner::peekChar()
    {
        char c = input_.peek();
        return c;
    }

    void Scanner::addToBuffer(char c)
    {
        buffer_.push_back(c);
    }

    void Scanner::reduceBuffer()
    {
        buffer_.pop_back();
    }

    void Scanner::makeToken(std::string name, TokenTag tag)
    {
        token_ = Token(name, tag);
        buffer_.clear();
        state_ = State::NONE;
    }


    /*******************************************************
    * preprocess
    *******************************************************/
    void Scanner::preprocess()
    {
        do
        {
            while(std::isspace(currentChar_))
            {
                getNextChar();
            }

            handleLineComment();
            handleBlockComment();
        } while(std::isspace(currentChar_));
    }

    void Scanner::handleLineComment()
    {
        while(currentChar_ == '/' && peekChar() == '/')
        {
            while(!input_.eof())
            {
                getNextChar();

                if(currentChar_ == '\n')
                {
                    getNextChar();          // new line's first char
                    break;
                }
            }
        }
    }

    void Scanner::handleBlockComment()
    {
        while(currentChar_ == '/' && peekChar() == '*')
        {
            bool block_end = false;

            getNextChar();  // make currentChar to the '*'
            while(!input_.eof())
            {
                getNextChar();

                // block end
                if(currentChar_ == '*' && peekChar() == '/')
                {
                    getNextChar();      // move currentChar to '/'
                    block_end = true;
                    break;
                }
            }

            if(!block_end)
            {
                errorReport("end of file happended in comment");
            }

            if(!input_.eof())
            {
                getNextChar();
            }
        }
    }



    Token Scanner::getNextToken()
    {
        bool matched = false;

        while(!matched)
        {
            if(state_ != State::NONE)
            {
                matched = true;
            }

            switch(state_)
            {
            case State::NONE:
                getNextChar();
                break;
            case State::END_OF_FILE:
                handleEOFState();
                break;
            case State::IDENTIFIER:
                handleIdentifierState();
                break;
            case State::NUMBER:
                handleNumberState();
                break;
            case State::STRING:
                handleStringState();
                break;
            case State::OPERATOR:
                handleOperationState();
                break;
            default:
                errorReport("Match token state error.");
            }

            // change state
            if(!matched)
            {
                preprocess();

                if(input_.eof())
                {
                    state_ = State::END_OF_FILE;
                }
                else
                {
                    if(std::isalpha(currentChar_) || currentChar_ == '_')
                    {
                        state_ = State::IDENTIFIER;
                    }
                    else if(std::isdigit(currentChar_))
                    {
                        state_ = State::NUMBER;
                    }
                    else if(currentChar_ == '"' || currentChar_ == '\'')
                    {
                        state_ = State::STRING;
                    }
                    else
                    {
                        state_ = State::OPERATOR;
                    }
                }
            }
        }

        return token_;
    }

    void Scanner::handleEOFState()
    {
        updateLocation();

        makeToken("EOF", TokenTag::END_OF_FILE);
        input_.close();
    }

    void Scanner::handleIdentifierState()
    {
        updateLocation();

        addToBuffer(currentChar_);
        while(std::isalnum(peekChar()) || (peekChar() == '_'))
        {
            getNextChar();
            addToBuffer(currentChar_);
        } // end while, currentChar is the end of the identifier

        // keyword or not
        auto tokenTag = dictionary_.lookup(buffer_);
        if(tokenTag == TokenTag::UNRESERVED)
        {
            tokenTag = TokenTag::IDENTIFIER;
        }
        makeToken(buffer_, tokenTag);
    }

    void Scanner::handleNumberState()
    {
        int numberBase = 10;

        updateLocation();

        // get integer part
        addToBuffer(currentChar_);          // add the first digit
        if(currentChar_ == '0')
        {
            if( (peekChar() == 'x') || (peekChar() == 'X') )
            {
                // handle hex integer
                numberBase = 16;
                getNextChar();
                addToBuffer(currentChar_);  // add 'x' or 'X'
                handleHexNumberState();
            }
            else if( (peekChar() >= '0') && (peekChar() <= '7') )
            {
                // handle oct integer
                numberBase = 8;
                handleOctNumberState();
                if(std::isdigit(peekChar()))
                {
                    errorReport("digit is too large :" + buffer_);
                }
            }
            // else we get integer 0
        }
        else
        {
            // handle decimal integer
            handleDecNumberState();
        }

        if(peekChar() == '.')
        {
            // is real number
            getNextChar();                  // eat .
            if(numberBase == 8)
            {
                errorReport("illegal floating point literal type");
                return ;
            }
            else
            {
                handleRealNumberState(numberBase == 16);
            }
            return ;
        }
        else if((peekChar() == 'l') || (peekChar() == 'L'))
        {
            // integer type suffix
            getNextChar();
            addToBuffer(currentChar_);
        }

        // make integer literal token
        makeToken(buffer_, TokenTag::INT_LITERAL);
    }

    void Scanner::handleRealNumberState(bool hex /* = false */)
    {
        // at this moment, currentChar is '.'
        addToBuffer(currentChar_);      // add . to buffer

        // add digits(opt)
        if(hex)
        {
            handleHexNumberState();
        }
        else
        {
            handleDecNumberState();
        }

        // exponent part(opt for dec but necessary for hex)
        if(hex)
        {
            if((peekChar() != 'p') && (peekChar() != 'P'))
            {
                errorReport("malformed floating point literal");
            }
            handleExponentState();
        }
        else if((peekChar() == 'e') || (peekChar() == 'E'))
        {
            handleExponentState();
        }

        // float type suffix
        if( (peekChar() == 'f') || (peekChar() == 'F') ||
            (peekChar() == 'd') || (peekChar() == 'D') )
        {
            getNextChar();
            addToBuffer(currentChar_);
        }

        makeToken(buffer_, TokenTag::REAL_LITERAL);
    }

    void Scanner::handleExponentState()
    {
        getNextChar();
        addToBuffer(currentChar_);  // add 'p' or 'e'
        if((peekChar() == '+') || (peekChar() == '-'))
        {
            getNextChar();
            addToBuffer(currentChar_);  // add '+' or '-'
        }
        handleDecNumberState();     // add signed integer
    }

    void Scanner::handleDecNumberState()
    {
        while(std::isdigit(peekChar()))
        {
            getNextChar();
            addToBuffer(currentChar_);
        }
    }

    void Scanner::handleOctNumberState()
    {
        while((peekChar() >= '0') && (peekChar() <= '7'))
        {
            getNextChar();
            addToBuffer(currentChar_);
        }
    }

    void Scanner::handleHexNumberState()
    {
        while(std::isxdigit(peekChar()))
        {
            getNextChar();
            addToBuffer(currentChar_);
        }
    }



    void Scanner::handleStringState()
    {
        // at this moment, currentChar is ' or "
        bool is_char = false;
        bool matched = false;

        updateLocation();

        if(currentChar_ == '\'')
        {
            is_char = true;
        }

        // TODO : empty character reduce
        if(!is_char && peekChar() == '"')
        {
            // empty string
            matched = true;
            //addToBuffer((char)0);
        }
        else
        {
            while(!input_.eof())
            {
                getNextChar();
                addToBuffer(currentChar_);

                // if characater is an escape sequence
                if(currentChar_ == '\\')
                {
                    handleEscapeSeqState();
                }

                if((peekChar() == '\'') || (peekChar() == '"'))
                {
                    matched = true;
                    break;
                }
            }
            if(!matched)
            {
                errorReport("miss \" or ' until end of file");
                return ;
            }
        }

        if(is_char)
        {
            if((buffer_.length() > 1) && (buffer_[0] != '\\'))
            {
                errorReport("too large range of character literal");
                return ;
            }
            else
            {
                makeToken(buffer_, TokenTag::CHAR_LITERAL);
            }
        }
        else
        {
            makeToken(buffer_, TokenTag::STR_LITERAL);
        }
        getNextChar();  // eat ' or "
    }

    // TODO :
    //      transfer escape sequence to normal char
    //
    void Scanner::handleEscapeSeqState()
    {
        // at this moment, currentChar is '/'
        getNextChar();          // eat the next char of /

        switch(currentChar_)
        {
        case 'b':case 't':case 'n':case 'f':case 'r':
            case '"':case '\'': case '\\':
            addToBuffer(currentChar_);
            break;

        case '0':case '1':case '2':case '3':
            // hundred's digit
            addToBuffer(currentChar_);
            // ten's digit
            if( (peekChar() >= '0') && (peekChar() <= '7') )
            {
                getNextChar();
                addToBuffer(currentChar_);
            }
            else
            {
                errorReport("cannot analysis the escape sequence");
                break;
            }

            // unit's digit
            if( (peekChar() >= '0') && (peekChar() <= '7') )
            {
                getNextChar();
                addToBuffer(currentChar_);
            }
            else
            {
                errorReport("cannot analysis the escape sequence");
                break;
            }
            break;

        case '4':case'5':case '6':case '7':
            // ten's digit
            addToBuffer(currentChar_);

            // unit's digit
            if( (peekChar() >= '0') && (peekChar() <= '7') )
            {
                getNextChar();
                addToBuffer(currentChar_);
            }
            else
            {
                errorReport("cannot analysis the escape sequence");
                break;
            }
            break;

        case 'u':
            // handle unicode character
            addToBuffer(currentChar_);          // add 'u'
            for(int i = 0; i < 4; i++)
            {
                if(std::isxdigit(peekChar()))
                {
                    getNextChar();
                    addToBuffer(currentChar_);
                }
                else
                {
                    errorReport("cannot analysis the escape sequence");
                    break;
                }
            }
            break;
        default:
            errorReport("illegal character of escape sequence");
            break;
        }
    } // handle escape sequence



    void Scanner::handleOperationState()
    {
        updateLocation();

        // check float
        if((currentChar_ == '.') && (std::isdigit(peekChar())))
        {
            handleRealNumberState();
            return ;
        }

        addToBuffer(currentChar_);
        while(!input_.eof())
        {
            addToBuffer(peekChar());        // add next one symbol char
            if(!dictionary_.has(buffer_))
            {
                reduceBuffer();
                break;
            }
            else
            {
                getNextChar();
            }
        } // end while, currentChar is the final char of the symbol

        auto tokenTag = dictionary_.lookup(buffer_);
        makeToken(buffer_, tokenTag);
    }






    // others
    void Scanner::errorReport(const std::string &msg)
    {
        ExceptionHandler::getInstance()->add(msg, loc_);
        setErrorFlag(true);
    }

}
