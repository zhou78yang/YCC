#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <map>


namespace ycc
{

    /*******************************************************
     * Symbol Info
     *******************************************************/
    class TypeInfo
    {
      public:
        TypeInfo(std::string name, int wd = 0, int of = -1)
            : name_(name), width_(wd), arrayOf_(of)
        {}

        std::string     getName() const;
        void            setWidth(int wd);
        int             getWidth() const;
        bool            isArray() const;
        int             arrayOf() const;
        bool operator   ==(const TypeInfo &);

        static TypeInfo VOID, INT, BYTE, SHORT, LONG,
                        CHAR, BOOLEAN, FLOAT, DOUBLE;

      private:
        std::string     name_;
        int             width_;
        int             arrayOf_;
    };

    inline std::string TypeInfo::getName() const
    {
        return name_;
    }
    inline void TypeInfo::setWidth(int wd)
    {
        width_ = wd;
    }
    inline int TypeInfo::getWidth() const
    {
        return width_;
    }
    inline bool TypeInfo::isArray() const
    {
        return arrayOf_ >= 0;
    }
    inline int TypeInfo::arrayOf() const
    {
        return arrayOf_;
    }

    enum SymbolTag
    {
        CLASS = 0,      VARIABLE,       MEMBER,         METHOD,
        CONSTRUCTOR,    BUILTIN,        PARAMETER,      UNDEFINE, // = null
        // modifiers
        PUBLIC,         PROTECTED,      PRIVATE,        STATIC,
        ABSTRACT,       FINAL,          NATIVE,         SYNCHRONIZED,
        TRANSIENT,      VOLATILE,       STRICTFP,       UNKNOWN
    };

    using SymbolFlag = std::bitset<32>;

    class SymbolInfo
    {
      public:
        SymbolInfo(int type, int u, int size = -1)
            : typeIndex_(type), flags_(u), arraySize_(size)
        {}
        SymbolInfo(int type, SymbolFlag flags, int size = -1)
            : typeIndex_(type), flags_(flags), arraySize_(size)
        {}

        int                 getType() const;
        bool                isArray() const;
        int                 getArraySize() const;
        void                setArraySize(int s);
        void                setAttribute(int i, int attr = 1);
        bool                check(int i) const;

        virtual std::string toString() const;   // for debug

        static SymbolInfo   NONE;

      private:
        int                 typeIndex_;
        SymbolFlag          flags_;
        int                 arraySize_;
    };

    inline int SymbolInfo::getType() const
    {
        return typeIndex_;
    }

    inline bool SymbolInfo::isArray() const
    {
        return arraySize_ >= 0;
    }

    inline int SymbolInfo::getArraySize() const
    {
        return arraySize_;
    }

    inline void SymbolInfo::setArraySize(int s)
    {
        arraySize_ = s;
    }

    inline void SymbolInfo::setAttribute(int i, int attr)
    {
        flags_.set(i, attr);
    }

    inline bool SymbolInfo::check(int i) const
    {
        return flags_.test(i);
    }


    class MethodInfo : public SymbolInfo
    {
      public:
        MethodInfo(int type, SymbolFlag flags, int size = -1)
            : SymbolInfo(type, flags, size)
        {}

        int                 parameterNum() const;
        void                addParameter(int type, std::string name);
        bool                checkParameter(std::vector<int> args) const;
        bool                checkParameter(int type, int pos) const;

        std::string         toString() const;   // for debug

        std::vector<int>            paramTypes_;
        std::vector<std::string>    parameters_;
    };

    inline int MethodInfo::parameterNum() const
    {
        return parameters_.size();
    }





    /*****************************************************
     * Table Declaration
     *****************************************************/

    class ClassTable
    {
      public:
        ClassTable(const std::string &name, ClassTable *prec = nullptr);

        void                add(const std::string &name, const SymbolInfo &info);
        bool                hasVariable(const std::string &name, bool searchUp = true) const;
        const SymbolInfo &  getVariableInfo(const std::string &name) const;
        void                setVariableInfo(const std::string &name, const SymbolInfo &info);

        void                add(const std::string &name, const MethodInfo &info);
        bool                hasMethod(const std::string &name, bool searchUp = true) const;
        const MethodInfo &  getMethodInfo(const std::string &name) const;
        void                setMethodInfo(const std::string &name, const MethodInfo &info);

        // get info
        ClassTable *        prec() const;
        const std::string & className() const;

        void                dump(); // for debug

      private:
        ClassTable *                        prec_;
        std::string                         name_;
        std::map<std::string, SymbolInfo>   variableTable_;
        std::map<std::string, MethodInfo>   methodTable_;
    };

    inline ClassTable * ClassTable::prec() const
    {
        return prec_;
    }

    inline const std::string & ClassTable::className() const
    {
        return name_;
    }



    class SymbolTable
    {
      public:
        static SymbolTable* getInstance();

        // type operations
        int                 addType(const std::string &name, int wd = 0);
        int                 addType(const TypeInfo &info);
        bool                hasType(const std::string &name) const;
        int                 getTypeIndex(const std::string &name) const;
        const TypeInfo &    getTypeInfo(int typeIndex) const;
        std::string         getTypeName(int typeIndex) const;

        void                addClass(const std::string &name, int modifier = 0);
        // current classes operations
        void                enterClass(const std::string &name);
        void                leaveClass();
        void                add(const std::string &name, const MethodInfo &info);
        bool                hasMethod(const std::string &name, bool searchUp = true) const;
        const MethodInfo &  getMethodInfo(const std::string &name) const;
        void                setMethodInfo(const std::string &name, const MethodInfo &info);

        // current table operations
        void                add(const std::string &name, const SymbolInfo &info);
        bool                hasVariable(const std::string &name, bool searchUP = true) const;
        const SymbolInfo &  getVariableInfo(const std::string &name) const;
        void                setVariableInfo(const std::string &name, const SymbolInfo &info);
        void                enter(const std::string &name);
        void                enter();
        void                leave();

        // symbol operations
        void                addStatic(const std::string &name, const SymbolInfo &info);
        std::string         getQualifier();
        std::string         getQualifier(const std::string &methodName);
        void                addLiteral(const std::string &literal);
        std::string         getLiteralName(const std::string &literal);

        void                IRdump(std::ostream &out = std::cout);
        void                dump(); // for debug

      private:
        SymbolTable();

      private:
        std::map<std::string, int>          typeTable_;
        std::vector<TypeInfo>               typeInfoTable_;

        std::map<std::string, ClassTable*>  classesTable_;
        ClassTable *                        globalTable_;
        ClassTable *                        currentClass_;
        std::string                         currentMethod_;

        std::vector<std::string>            subroutineTable_;
        std::vector<SymbolInfo>             localInfoTable_;
        std::vector<int>                    baseStack_;

        std::map<std::string, SymbolInfo>   staticTable_;
        std::map<std::string, std::string>  literalTable_;

        static SymbolTable *                instance_;
    };

}

#endif
