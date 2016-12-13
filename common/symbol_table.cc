#include <iostream>
#include "symbol_table.h"


using std::cout;
using std::endl;
using std::cerr;

namespace ycc
{
    /*********************************************
    * info part
    **********************************************/
    bool TypeInfo::operator == (const TypeInfo &t)
    {
        return name_ == t.name_;
    }

    TypeInfo TypeInfo::VOID("void", 0);
    TypeInfo TypeInfo::INT("int", 4);
    TypeInfo TypeInfo::BYTE("byte", 1);
    TypeInfo TypeInfo::SHORT("short", 2);
    TypeInfo TypeInfo::LONG("long", 8);
    TypeInfo TypeInfo::CHAR("char", 2);
    TypeInfo TypeInfo::BOOLEAN("boolean", 1);
    TypeInfo TypeInfo::FLOAT("float", 4);
    TypeInfo TypeInfo::DOUBLE("double", 8);

    SymbolInfo SymbolInfo::NONE(0, 0);


    std::string SymbolInfo::toString() const
    {
        std::string type = SymbolTable::getInstance()->getTypeName(typeIndex_);
        return "<" + type + ": " + flags_.to_string() + ">";
    }

    void MethodInfo::addParameter(int type, std::string name)
    {
        paramTypes_.push_back(type);
        parameters_.push_back(name);
    }

    bool MethodInfo::checkParameter(std::vector<int> args) const
    {
        if(args.size() != paramTypes_.size())
        {
            return false;
        }
        for(auto i = 0; i < paramTypes_.size(); i++)
        {
            if(args[i] != paramTypes_[i])
            {
                return false;
            }
        }
        return true;
    }

    bool MethodInfo::checkParameter(int type, int pos) const
    {
        if(pos >= paramTypes_.size())
        {
            return false;
        }
        return type == paramTypes_[pos];
    }

    std::string MethodInfo::toString() const
    {
        std::string ret = SymbolInfo::toString();
        ret += " : ";
        auto symbolTable = SymbolTable::getInstance();
        for(int i = 0; i < paramTypes_.size(); i++)
        {
            ret += symbolTable->getTypeName(paramTypes_[i]) + " " + parameters_[i] + ", ";
        }
        return ret;
    }

    /*********************************************
    * tables part
    *********************************************/

    ClassTable::ClassTable(const std::string &name, ClassTable *prec)
        : name_(name), prec_(prec)
    {}

    void ClassTable::add(const std::string &name, const SymbolInfo &info)
    {
        variableTable_.insert(std::pair<decltype(name), decltype(info)>(name, info));
    }

    bool ClassTable::hasVariable(const std::string &name, bool searchUp /*= true*/) const
    {
        auto iter = variableTable_.find(name);
        if(iter != variableTable_.end())
        {
            return true;
        }
        if(!searchUp)
        {
            return false;
        }
        return prec_ ? (prec_->hasVariable(name)) : false;
    }

    const SymbolInfo & ClassTable::getVariableInfo(const std::string &name) const
    {
        auto iter = variableTable_.find(name);
        if(iter != variableTable_.end())
        {
            return iter->second;
        }
        return prec_->getVariableInfo(name);
    }

    void ClassTable::setVariableInfo(const std::string &name, const SymbolInfo &info)
    {
        auto iter = variableTable_.find(name);
        if(iter != variableTable_.end())
        {
            iter->second = info;
        }
        else
        {
            prec_->setVariableInfo(name, info);
        }
    }

    void ClassTable::add(const std::string &name, const MethodInfo &info)
    {
        methodTable_.insert(std::pair<std::string, MethodInfo>(name, info));
    }

    bool ClassTable::hasMethod(const std::string &name, bool searchUp /*=true*/) const
    {
        auto iter = methodTable_.find(name);
        if(iter != methodTable_.end())
        {
            return true;
        }
        if(!searchUp)
        {
            return false;
        }
        return prec_ ? prec_->hasMethod(name) : false;
    }

    const MethodInfo &ClassTable::getMethodInfo(const std::string &name) const
    {
        auto iter = methodTable_.find(name);
        if(iter != methodTable_.end())
        {
            return iter->second;
        }
        return prec_->getMethodInfo(name);
    }

    void ClassTable::setMethodInfo(const std::string &name, const MethodInfo &info)
    {
        auto iter = methodTable_.find(name);
        if(iter != methodTable_.end())
        {
            iter->second = info;
        }
        else
        {
            prec_->setMethodInfo(name, info);
        }
    }


    void ClassTable::dump()
    {
        cout << "ClassTable " << name_ << " : " << endl;
        cout << "variable table and size : " << variableTable_.size() << endl;
        for(auto line : variableTable_)
        {
            cout << line.first << " -> " << line.second.toString() << endl;
        }
        cout << "method table and size : " << methodTable_.size() << endl;
        for(auto line : methodTable_)
        {
            cout << line.first << " -> " << line.second.toString() << endl;
        }
    }

    /********************************************************
    * SymbolTable
    ********************************************************/
    SymbolTable * SymbolTable::instance_ = nullptr;

    SymbolTable * SymbolTable::getInstance()
    {
        if(instance_ == nullptr)
        {
            instance_ = new SymbolTable();
        }
        return instance_;
    }

    SymbolTable::SymbolTable()
    {
        currentClass_ = new ClassTable("global");
        globalTable_ = currentClass_;

        addType(TypeInfo::VOID);
        addType(TypeInfo::BOOLEAN);
        addType(TypeInfo::BYTE);
        addType(TypeInfo::CHAR);
        addType(TypeInfo::SHORT);
        addType(TypeInfo::INT);
        addType(TypeInfo::LONG);
        addType(TypeInfo::FLOAT);
        addType(TypeInfo::DOUBLE);

        // for debug
        addType("String");
    }

    // type operations
    int SymbolTable::addType(const std::string &name, int wd /*=0*/)
    {
        return addType(TypeInfo(name, wd));
    }

    int SymbolTable::addType(const TypeInfo &info)
    {
        int index = typeInfoTable_.size();
        typeTable_.insert(std::pair<std::string, int>(info.getName(), index));
        typeInfoTable_.push_back(info);

        return index;
    }

    bool SymbolTable::hasType(const std::string &name) const
    {
        auto iter = typeTable_.find(name);
        return iter != typeTable_.end();
    }

    int SymbolTable::getTypeIndex(const std::string &name) const
    {
        return typeTable_.find(name)->second;
    }

    const TypeInfo & SymbolTable::getTypeInfo(int typeIndex) const
    {
        return typeInfoTable_.at(typeIndex);
    }

    std::string SymbolTable::getTypeName(int typeIndex) const
    {
        return typeInfoTable_[typeIndex].getName();
    }

    // class operations
    void SymbolTable::addClass(const std::string &name, int modifier /*=0*/)
    {
        int type = addType(name);
        SymbolInfo info(type, modifier);
        info.setAttribute(SymbolTag::CLASS);
        currentClass_->add(name, info);

        currentClass_ = new ClassTable(name, currentClass_);
        classesTable_.insert(std::pair<std::string, ClassTable*>(name, currentClass_));
    }

    // current class operations
    void SymbolTable::enterClass(const std::string &name)
    {
        currentClass_ = classesTable_.find(name)->second;
    }

    void SymbolTable::leaveClass()
    {
        currentClass_ = currentClass_->prec();
    }

    void SymbolTable::add(const std::string &name, const MethodInfo &info)
    {
        currentClass_->add(name, info);
    }

    bool SymbolTable::hasMethod(const std::string &name, bool searchUp) const
    {
        return currentClass_->hasMethod(name, searchUp);
    }

    const MethodInfo & SymbolTable::getMethodInfo(const std::string &name) const
    {
        return currentClass_->getMethodInfo(name);
    }

    void SymbolTable::setMethodInfo(const std::string &name, const MethodInfo &info)
    {
        currentClass_->setMethodInfo(name, info);
    }

    // current table operations
    void SymbolTable::add(const std::string &name, const SymbolInfo &info)
    {
        if(baseStack_.empty())
        {
            // class member
            currentClass_->add(name, info);
            return ;
        }
        subroutineTable_.push_back(name);
        localInfoTable_.push_back(info);
    }

    bool SymbolTable::hasVariable(const std::string &name, bool searchUp) const
    {
        for(int i = subroutineTable_.size()-1; i >= 0; i--)
        {
            if(subroutineTable_[i] == name)
            {
                return true;
            }
        }
        return searchUp ? currentClass_->hasVariable(name, searchUp) : false;
    }

    const SymbolInfo &SymbolTable::getVariableInfo(const std::string &name) const
    {
        for(int i = subroutineTable_.size()-1; i >= 0; i--)
        {
            if(subroutineTable_[i] == name)
            {
                return localInfoTable_[i];
            }
        }
        return currentClass_->getVariableInfo(name);
    }

    void SymbolTable::setVariableInfo(const std::string &name, const SymbolInfo &info)
    {
        for(int i = subroutineTable_.size()-1; i >= 0; i--)
        {
            if(subroutineTable_[i] == name)
            {
                localInfoTable_[i] = info;
                return ;
            }
        }

        currentClass_->setVariableInfo(name, info);
    }


    void SymbolTable::enter(const std::string &methodName)
    {
        baseStack_.clear();
        baseStack_.push_back(0);

        cout << "enter new method " << methodName << ":\n"; // for debug

        auto methodInfo = getMethodInfo(methodName);
        auto types = methodInfo.paramTypes_;
        auto param = methodInfo.parameters_;
        SymbolFlag flags(0);
        flags.set(SymbolTag::PARAMETER);

        for(int i = 0; i < types.size(); i++)
        {
            add(param[i], SymbolInfo(types[i], flags));
        }

    }

    void SymbolTable::enter()
    {
        if(baseStack_.empty())
        {
            return ;
        }

        baseStack_.push_back(subroutineTable_.size());
    }

    void SymbolTable::leave()
    {
        if(baseStack_.empty())
        {
            return ;
        }

        cout << "---------------------------------" << endl;

        while(subroutineTable_.size() > baseStack_.back())
        {
            cout << subroutineTable_.back() << " -> "
                 << localInfoTable_.back().toString() << endl;

            subroutineTable_.pop_back();
            localInfoTable_.pop_back();
        }

        baseStack_.pop_back();
        if(baseStack_.empty())
        {
            cout << "---------------------------------" << endl;
            cout << "exit method body" << endl << endl;
        }
    }

    void SymbolTable::dump()
    {
        cout << "--------------type table-----------" << endl;
        for(auto iter : typeTable_)
        {
            cout << "( " << iter.second << ", " << iter.first
                 << " width: " << typeInfoTable_[iter.second].getWidth()
                 << ") ";
        }
        cout << endl;

        cout << "--------------global table-------------" << endl;
        currentClass_->dump();
        cout << endl;

        cout << "we have " << classesTable_.size() << " class table here." << endl;
        for(auto table : classesTable_)
        {
            cout << "---------------------------" << endl;
            table.second->dump();
            cout << "---------------------------" << endl;
        }
    }



}
