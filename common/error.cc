#include <iostream>
#include "error.h"

namespace ycc
{
    std::string errorDesc(ErrorType t)
    {
        std::string ret;
        switch(t)
        {
        case ErrorType::WARNING:
            ret = "warning";
            break;
        case ErrorType::ERROR:
            ret = "error";
            break;
        default:
            ret = "note";
            break;
        }
        return ret;
    }

    Exception::Exception(const std::string &msg, const TokenLocation &loc, ErrorType type /*=ERROR*/)
        : msg_(msg), loc_(loc), type_(type)
    {}

    std::string Exception::toString() const
    {
        return loc_.toString() + ": " + errorDesc(type_) + ": " + msg_;
    }

    ExceptionHandler * ExceptionHandler::instance_ = nullptr;

    ExceptionHandler * ExceptionHandler::getInstance()
    {
        if(instance_ == nullptr)
        {
            instance_ = new ExceptionHandler();
        }
        return instance_;
    }

    void ExceptionHandler::report()
    {
        for(auto e : exceptionList_)
        {
            std::cout << e.toString() << std::endl;
        }

    }

    void ExceptionHandler::add(Exception e)
    {
        exceptionList_.push_back(e);
    }

    void ExceptionHandler::add(const std::string &msg, const TokenLocation &loc, ErrorType type)
    {
        exceptionList_.push_back(Exception(msg, loc, type));
    }

}
