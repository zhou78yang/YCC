#ifndef ERROR_H_
#define ERROR_H_

#include <string>
#include <vector>
#include "../lexer/token.h"

namespace ycc
{
    enum class ErrorType
    {
        WARNING,
        ERROR,
        NOTE
    };

    class Exception
    {
      public:
        Exception(const std::string &msg, const TokenLocation &loc, ErrorType type = ErrorType::ERROR);

        std::string         toString() const;

      private:
        std::string         msg_;
        TokenLocation       loc_;
        ErrorType           type_;
    };

    class ExceptionHandler
    {
      public:
        static ExceptionHandler *   getInstance();
        void                        report();
        bool                        hasError() const;
        void                        add(Exception e);
        void                        add(const std::string &msg,
                                        const TokenLocation &loc,
                                        ErrorType type = ErrorType::ERROR);

      private:
        ExceptionHandler(){}

        //ifstream                    input_;
        static ExceptionHandler *   instance_;
        std::vector<Exception>      exceptionList_;
    };

    inline bool ExceptionHandler::hasError() const
    {
        return !exceptionList_.empty();
    }
}

#endif
