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


    std::string SymbolInfo::toString() const
    {
        std::string type = SymbolTable::getInstance()->getTypeName(typeIndex_);
        return "<" + type + ": " + flags_.to_string() + ">";
    }

    void MethodInfo::addParameter(int p)
    {
        parameters_.push_back(p);
    }

    bool MethodInfo::checkParameter(std::vector<int> args) const
    {
        if(args.size() != parameters_.size())
        {
            return false;
        }
        for(auto i = 0; i < parameters_.size(); i++)
        {
            if(args[i] != parameters_[i])
            {
                return false;
            }
        }
        return true;
    }

    bool MethodInfo::checkParameter(int type, int pos) const
    {
        if(pos >= parameters_.size())
        {
            return false;
        }
        return type == parameters_[pos];
    }

    std::string MethodInfo::toString() const
    {
        std::string ret = SymbolInfo::toString();
        ret += " :";
        auto symbolTable = SymbolTable::getInstance();
        for(auto p : parameters_)
        {
            ret += " " + symbolTable->getTypeName(p);
        }
        return ret;
    }

    /*********************************************
    * tables part
    *********************************************/
    EnvTable::EnvTable(EnvTable *prec)
        : prec_(prec)
    {}

    void EnvTable::add(const std::string &name, const SymbolInfo &info)
    {
        variableTable_.insert(std::pair<decltype(name), decltype(info)>(name, info));
    }

    bool EnvTable::hasVariable(const std::string &name, bool searchUp /*= true*/) const
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

    const SymbolInfo & EnvTable::getVariableInfo(const std::string &name) const
    {
        auto iter = variableTable_.find(name);
        if(iter != variableTable_.end())
        {
            return iter->second;
        }
        return prec_->getVariableInfo(name);
    }


    ClassTable::ClassTable(const std::string &name, EnvTable *prec)
        : name_(name), EnvTable(prec)
    {}

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
        return prec_ ? ((ClassTable*)prec_)->hasMethod(name) : false;
    }

    const MethodInfo &ClassTable::getMethodInfo(const std::string &name) const
    {
        auto iter = methodTable_.find(name);
        if(iter != methodTable_.end())
        {
            return iter->second;
        }
        return ((ClassTable*)prec_)->getMethodInfo(name);
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

    LocalTable::LocalTable(EnvTable *prec)
        : EnvTable(prec)
    {}

    void LocalTable::dump()
    {
        cout << "LocalTable :" << endl;
        cout << "variable table and size : " << variableTable_.size() << endl;
        for(auto line : variableTable_)
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
        currentTable_ = globalTable_;

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
        return typeInfoTable_[typeIndex];
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
        currentTable_->add(name, info);

        currentClass_ = new ClassTable(name, currentClass_);
        classesTable_.insert(std::pair<std::string, ClassTable*>(name, currentClass_));
        currentTable_ = currentClass_;
    }

    // current class operations
    void SymbolTable::enterClass(const std::string &name)
    {
        currentClass_ = classesTable_.find(name)->second;
        currentTable_ = currentClass_;
    }

    void SymbolTable::leaveClass()
    {
        currentClass_ = (ClassTable*)currentTable_->prec();
        currentTable_ = currentClass_;
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

    // current table operations
    void SymbolTable::add(const std::string &name, const SymbolInfo &info)
    {
        currentTable_->add(name, info);
    }

    bool SymbolTable::hasVariable(const std::string &name, bool searchUp) const
    {
        return currentTable_->hasVariable(name, searchUp);
    }

    const SymbolInfo &SymbolTable::getVariableInfo(const std::string &name) const
    {
        return currentTable_->getVariableInfo(name);
    }

    void SymbolTable::enter()
    {
        currentTable_ = new LocalTable(currentTable_);
    }

    void SymbolTable::leave()
    {
        auto oldTable = (LocalTable*)currentTable_;
        currentTable_ = currentTable_->prec();
        delete oldTable;
    }

    void SymbolTable::dump()
    {
        cout << "--------------type table-----------" << endl;
        for(auto iter : typeTable_)
        {
            cout << "( " << iter.first << ", " << iter.second << ") ";
        }
        cout << endl;

        cout << "--------------global table-------------" << endl;
        currentTable_->dump();
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
