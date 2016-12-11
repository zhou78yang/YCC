#include "compiler_vistor.h"

namespace ycc
{

    CompilerVistor::CompilerVistor()
        : errorFlag_(false)
    {
        symbolTable_ = SymbolTable::getInstance();
    }

    bool CompilerVistor::check(VecNodePtr ast)
    {
        // TODO: 对外接口，遍历一遍语法树进行语义检查，返回语义分析结果？

        return errorFlag_;
    }


    // TODO: visit parts










    // tool for semantic check
    bool CompilerVistor::numeric(int typeIndex)
    {
        auto type = symbolTable_->getTypeInfo(typeIndex);

        if( (type == TypeInfo::BYTE) || (type == TypeInfo::CHAR)
         || (type == TypeInfo::INT)  || (type == TypeInfo::SHORT)
         || (type == TypeInfo::LONG) || (type == TypeInfo::FLOAT)
         || (type == TypeInfo::DOUBLE) )
        {
            return true;
        }
        return false;
    }

    int CompilerVistor::maxType(int type1, int type2)
    {
        return type1 > type2 ? type1 : type2;
    }

    // others
    void CompilerVistor::errorReport(const std::string &msg, const TokenLocation &loc, ErrorType errorType)
    {
        // TODO
    }

}
