#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

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
        void                setAttribute(int i);
        bool                check(int i);

        virtual std::string toString() const;   // for debug

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

    inline void SymbolInfo::setAttribute(int i)
    {
        flags_.set(i);
    }

    inline bool SymbolInfo::check(int i)
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
        void                addParameter(int type);
        bool                checkParameter(std::vector<int> args) const;
        bool                checkParameter(int type, int pos) const;

        std::string         toString() const;   // for debug

      private:
        std::vector<int>    parameters_;
    };

    inline int MethodInfo::parameterNum() const
    {
        return parameters_.size();
    }





    /*****************************************************
     * Table Declaration
     *****************************************************/
    class EnvTable
    {
      public:
        EnvTable(EnvTable *prec);

        void                add(const std::string &name, const SymbolInfo &info);
        bool                hasVariable(const std::string &name, bool searchUp = true) const;
        const SymbolInfo &  getVariableInfo(const std::string &name) const;

        EnvTable *          prec() const;
        virtual void        dump() = 0;     // for debug

      protected:
        EnvTable *                          prec_;
        std::map<std::string, SymbolInfo>   variableTable_;
    };

    inline EnvTable *EnvTable::prec() const
    {
        return prec_;
    }

    class ClassTable : public EnvTable
    {
      public:
        ClassTable(const std::string &name, EnvTable *prec = nullptr);

        void                add(const std::string &name, const MethodInfo &info);
        bool                hasMethod(const std::string &name, bool searchUp = true) const;
        const MethodInfo &  getMethodInfo(const std::string &name) const;

        const std::string & className() const;

        void                dump(); // for debug

      private:
        std::string                         name_;
        std::map<std::string, MethodInfo>   methodTable_;
    };

    inline const std::string & ClassTable::className() const
    {
        return name_;
    }

    class LocalTable : public EnvTable
    {
      public:
        LocalTable(EnvTable *prec = nullptr);
        void                dump(); // for debug
    };



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

        // current table operations
        void                add(const std::string &name, const SymbolInfo &info);
        bool                hasVariable(const std::string &name, bool searchUP = true) const;
        const SymbolInfo &  getVariableInfo(const std::string &name) const;
        void                enter();
        void                leave();

        void                dump(); // for debug

      private:
        SymbolTable();

      private:
        std::map<std::string, int>          typeTable_;
        std::vector<TypeInfo>               typeInfoTable_;
        std::map<std::string, ClassTable*>  classesTable_;
        EnvTable *                          globalTable_;
        EnvTable *                          currentTable_;
        ClassTable *                        currentClass_;

        static SymbolTable *                instance_;
    };

}

#endif
