##<a name="index"/>目录
* [简介](#简介)
* [Java语言介绍](#Java语言介绍)
    * [词法结构](#词法结构)
        * [空白和注释](#空白和注释)
        * [标识符](#标识符)
        * [关键字](#关键字)
        * [字面值](#字面值)
        * [分隔符](#分隔符)
        * [运算符](#运算符)
    * [语法规则](#语法规则)
    * [语义规则](#语义规则)
        * [基本类型](#基本类型)
        * [转换和提升](#转换和提升)
* [使用说明](#使用说明)
* [模块说明](#模块说明)
    * [词法分析器](#词法分析器)
        * [词法单元](#词法单元)
        * [内置字典](#内置字典)
        * [状态机图](#状态机图)
        * [词法分析](#词法分析)
    * [语法分析器](#语法分析器)
        * [文法](#文法)
        * [语法树](#语法树)
        * [语法分析](#语法分析)
    * [符号表](#符号表)
    * [语义分析器](#语义分析器)
        * [语义规则](#语义规则)
    * [中间代码生成器](#中间代码生成器)
    * [目标代码生成器](#目标代码生成器)

# <a name="简介">简介
YCC是一款简易的Java编译器，源语言是Java语言的子集，目标语言是AT&T汇编。源语言词法，语法，语义相关内容均参考标准 [Java SE 6](http://docs.oracle.com/javase/6/docs/api/)。

# <a name="Java语言介绍">Java语言介绍

## <a name="词法结构">词法结构

### <a name="空白和注释">空白和注释

1. 空白被定义为ASCII空格，水平制表符，换页符以及行终止符。
2. 注释有两种类型:
```
/* 文本 */    块注释
// 文本       行注释
```

### <a name="标识符">标识符

标识符是<strong>Java字母</strong>和<strong>Java数字</strong>的不限长序列。
Java字母包括大写和小写的ASCII字母`A~Z`和`a~z`以及下划线`_`，Java数字包括`0~9`ASCII数字。但标识符不能以数字打头且不能具有与 <strong>关键字</strong>， <strong>布尔值</strong>， <strong>空值</strong>相同的拼写。

### <a name="关键字">关键字

```
// Keywords
abstract,   continue,   for,        new,        switch,
assert,     default,    if,         package,    synchronized,
boolean,    do,         goto,       private,    this,
break,      double,     implements, protected,  throw,
byte,       else,       import,     public,     throws,
case,       enum,       instanceof, return,     transient,
catch,      extends,    int,        short,      try,
char,       final,      interface,  static,     void,
class,      finally,    long,       strictfp,   volatile,
const,      float,      native,     super,      while,
```

### <a name="字面值">字面值

```
IntegerLiteral
FloatingPointLiteral
CharacterLiteral
StringLiteral
BooleanLiteral
NullLiteral

```

### <a name="分隔符">分隔符

```
  (  )  [  ]  {  }  ;  ,   .   
```

### <a name="运算符">运算符

```
 =  >  <  ==  <=  >=  !=  &&  ||  !
 ~  ?  :  ++  --  +  -  *  /   &  |
 ^  %  +=  -=  *=  /=  &=  |=  ^=  %=  
 <<  >>  <<=  >>=  >>>  >>>=
```


## <a name="语法规则">语法规则

说明：
* []：表示可选（即一个或零个）
* {}：表示零个或多个
* x | y：表示x或y
* one of：表示其中的一个
* 下标<sub>（opt）</sub>：表示可选
* 为了节约篇幅，使用全大写表示某类终结符的一个, 如IDENTIFIER, NUMBER, STRING，分别表示一个标识符，一个整型字面值，一个字符串字面值

```
ClassDeclaration    ::= {MODIFIER} class IDENTIFIER [extends Type] ClassBody
ClassBody           ::= "{" {ClassBodyDecl} "}"
ClassBodyDecl       ::= ;             |
                        [static]Block |
                        {MODIFIER} MemberDeclaration
MemberDeclaration   ::= VariableDeclaration |
                        MethodDeclaration   |
                        ClassDeclaration
VariableDeclaration ::= Type IDENTIFIER [=Expr] { , IDENTIFIER [=Expr] } ;
MethodDeclaration   ::= Type IDENTIFIER([Type IDENTIFIER{,Type IDENTIFIER}]) Block
Type                ::= ClassName {"[""]"} | BasicType
Block               ::= '{' Stmts '}'
Stmts               ::= [Stmt Stmts]
Stmt                ::= ;           |
                        Block       |
                        IfStmt      |
                        ForStmt     |
                        WhileStmt   |
                        DoStmt      |
                        SwitchStmt  |
                        ReturnStmt  |
                        BreakStmt   |
                        ContinueStmt |
                        ExprStatement
IfStmt              ::= if(Expr) Stmt [else Stmt]
ForStmt             ::= for(Expr1; Expr2; Expr3) Stmt
WhileStmt           ::= while(Expr) Stmt
DoStmt              ::= do Stmt while(Expr);
SwitchStmt          ::= switch(Expr)'{' {CaseBody} [DefaultBody] '}'
CaseBody            ::= case Expr: Stmts
DefaultBody         ::= default: Stmts
BreakStmt           ::= break ;
ContinueStmt        ::= continue ;
ReturnStmt          ::= return [Expr] ;
ExprStatement       ::= VariableDeclaration | Expr;
Expr                ::= Identifier          |
                        IntegerLiteral      |
                        CharacterLiteral    |
                        BooleanLiteral      |
                        NullLiteral         |
                        FloatingPointLiteral |
                        StringLiteral       |
                        ArrayLiteral        |
                        PrefixOpExpr        |
                        PostfixOpExpr       |
                        InfixOpExpr         |
                        TernaryOpExpr       |
                        ParExpr             |
                        NewExpr
NewExpr             ::= new BasicType{'['Expr']'}       |
                        new ClassName([Expr {,Expr}])
ParExpr             ::= "(" Expr ")"
Identifier          ::= IDENTIFIER |
                        IndexExpr  |
                        CallExpr   |
                        QualifiedIdentifier
IndexExpr           ::= Identifier[Expr]
CallExpr            ::= IDENTIFIER([Expr {,Expr}])
QualifiedIdentifier ::= Identifier1.Identifier2
ArrayLiteral        ::= "{" [Expr {,Expr}] "}"
PrefixOpExpr        ::= PREFIX_OPERATOR Expr
PostfixOpExpr       ::= Expr POSTFIX_OPERATOR
InfixOpExpr         ::= Expr1 INFIX_OPERATOR Expr2
TernaryOpExpr       ::= Expr1 ? Expr2 : Expr3

```

## <a name="语义规则">语义规则
// TODO

### <a name="基本类型">基本类型
// TODO

### <a name="转换和提升">转换和提升
// TODO




# <a name="使用说明">使用说明
// TODO

# <a name="模块说明">模块说明
DYJavac编译器主要分成词法分析器，语法分析器，语义分析器，中间代码生成器，目标代码生成器（虚拟机）组成。


## <a name="词法分析器">词法分析器

### <a name="词法单元位置">词法单元位置

TokenLocation存储的是Token所在的位置，即文件名，行，列，用于辅助错误信息的生成，对外接口：

```cpp
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
```

### <a name="词法单元">词法单元

[Token](./lexer/token.h)即为一个<词素，标签>对，词法分析器的产出物，语法分析器的基本单元，它的定义如下：

```cpp
class Token
{
public:
    Token();                                        
    Token(std::string lexeme, TokenTag tag);
    std::string                 lexeme() const;     // 获取词素
    TokenTag                    tag() const;        // 获取标签
    std::string                 desc() const;       // 标签描述
    std::string                 toString() const;   // 词法单元描述
private:
    std::string                 lexeme_;
    TokenTag                    tag_;
};
```

### <a name="内置字典">内置字典

[Dictionary](./lexer/token.h)是一个内置字典，存储了词素到标签的转换，包含了关键字，分隔符，运算符，以及几类字面值的标签和一个标识符标签。其作用就是辅助词法分析器Scanner在词法分析时确定词法单元标签使用的.

```cpp
class Dictionary
{
public:
    Dictionary();
    TokenTag    lookup(const std::string &lexeme) const;    // 获取一个词素对应的标签
    bool        has(const std::string &lexeme) const;       // 判断一个词素是否在内置的字典中
private:
    ...
};
```


### <a name="状态机图">状态机图

![词法分析状态转换图](http://cdn.duitang.com/uploads/item/201507/25/20150725014433_ZvKSY.thumb.224_0.jpeg "词法分析状态转换图")

*注：每次调用getNextToken()获取下一个Token时，currentChar都会从指向上一个Token末位的位置转移到当前Token末位的位置，生成的Token流效果可以看*[./test/tokens.txt](./test/tokens.txt)


### <a name="词法分析">词法分析

词法分析器Scanner的作用就是将源文件中的字符转换为一个Token流，即词法分析器的基本元素是一个字符，输入的基本单元是字符，输出单元Token。

Scanner的定义如下：
```cpp

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

    ...

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

```

词法分析器主要提供了两种外部接口`getNextToken()`用来获取下一个Token，`getToken()`用来获取当前的Token。在词法分析器内部，我们主要使用以下四种操作：

- getNextChar() : 从源文件中获取下一个字符
- peekChar()    : 来获知当前字符的下一个字符(但并不将它取出来)。Scanner则可以使用一种预知下一字符的方式来分析词素，实现词法分析的状态转换。
- addToBuffer()和reduceBuffer() : 对缓冲区进行添加和去尾。(*当然，在所有的情况下，我们使用reduceBuffer()抛弃掉的字符都是通过peekChar()得到的。*)

#### 支持的词素示例
- [x] 标识符：String, i3, MAX_VALUE, isLetterOrDigit
- [x] int值：0, 2, 0372, 0xDadaCafe, 2016, 0x00FF00FF
- [x] long值：0l, 0777L, 0x10000000L, 0xC0B0L
- [x] 十进制浮点值：3.14e-9D, .0314d, 2016.e3, 30.
- [x] 十六进制浮点值：0x400A9F21P-3, 0x.APF, 0x0F0f01.C0B0P5
- [x] 布尔值：true, false
- [x] 字符值：'', 'a', '"', '%', '\t', '\\', '\'', '\177', '\u03a9', '\uFfFf'
- [x] 字符串值："", "'", "\"", "this is a string", "This is \101 string\u005F"
- [x] 空值：null


## <a name="语法分析器">语法分析器

### <a name="文法">文法

语法规则见上文的Java[语法规则](#语法规则).

### <a name="语法树">语法树

[抽象语法树](./parser/ast.hpp)是使用[parser](./parser/parser.h)来实现。而我们使用设计模式中的访问者模式，通过[vistor接口](./parser/vistor.h)来访问语法树，实现语义分析，中间代码生成等操作。访问者模式很适用于我们这种数据结构相对未定的系统，它把数据结构和作用于结构上的操作之间的耦合解脱开，是的操作集合可以相对自由的演化。访问者模式具有如下优点：      

1. 访问者模式使得增加新的操作变得很容易。
2. 访问者模式将有关的行为集中到一个访问者对象中，而不是分散到一个个的结点类中。
3. 访问者模式可以跨过几个类的等级结构访问属于不同等级结构的成员类。

访问者模式的结构如下：

![访问者模式结构](http://images.cnblogs.com/cnblogs_com/maobisheng/DesignPatterns_Creational/23.Visitor.jpg "访问者模式结构")

#### 语法树节点类型列表

* ASTNode
* ClassStmt
* MethodDeclStmt
* PrimaryStmt
* VariableDeclExpr
* EmptyStmt
* BlockStmt
* Stmt
* IfStmt
* ForStmt
* WhileStmt
* DoStmt
* SwitchStmt
* CaseStmt
* BreakStmt
* ContinueStmt
* ReturnStmt
* Expr
* IdentifierExpr
* NewExpr
* IndexExpr
* CallExpr
* QualifiedIdExpr
* IntExpr
* RealExpr
* BoolExpr
* NullExpr
* StrExpr
* ArrayExpr
* UnaryOpExpr
* BinaryOpExpr
* TernaryOpExpr

### <a name="语法分析">语法分析

### [语法分析器 parser](./parser/parser.h)

```cpp

class Parser
{
public:
    explicit        Parser(Scanner &scanner);
    VecNodePtr      parse();                        // 进行语法分析，返回一棵语法树

    static void     setErrorFlag(bool flag);
    static bool     getErrorFlag();

private:
    void            advance();                      // 读取下一个Token
    bool            match(TokenTag tag, bool advanceToNextToken = false);                           // 判断Token是否符合
    bool            match(TokenTag tag, const std::string &msg, bool advanceToNextToken = false);   // 带错误处理的判断
    void            addToBuffer();                  // 将当前Token添加到buffer中
    void            reduceBuffer();                 // buffer退出一个Token
    void            clearBuffer();                  // 清空buffer
    void            errorReport(const std::string &msg, ErrorType type = ErrorType::ERROR);         // 错误处理
    TokenLocation   getLocation() const;            // 获取当前Token的位置

    ...

private:
    Token               token_;                     // 当前Token
    std::vector<Token>  buffer_;                    // 缓冲区
    Scanner&            scanner_;                   // 词法分析器
    SymbolTable*        symbolTable_;               // 符号表
    VecNodePtr          ast_;                       // 抽象语法树

    static bool         errorFlag_;
};

```







## <a name="符号表">符号表

### <a name="符号表基本信息">符号表基本信息类型

#### 1. TypeInfo(类型信息)
```cpp
class TypeInfo
{
  public:
    TypeInfo(std::string name, int wd, int of = -1)
        : name_(name), width_(wd), arrayOf_(of)
    {}

    std::string     getName() const;        // 返回类型名
    void            setWidth(int wd);       // 设置对象占用空间
    int             getWidth() const;       // 获取对象占用空间
    bool            isArray() const;        // 是否数组类型
    int             arrayOf() const;        // 数组单元类型
    bool operator   ==(const TypeInfo &);   // 判断两个类型是否相同

  private:
    std::string     name_;
    int             width_;
    bool            isArray_;
    int             arrayOf_;
};

```

#### 2. SymbolInfo(符号信息——用于变量)
```cpp
enum SymbolTag
{
    CLASS = 0,      VARIABLE,       CONSTANT,       METHOD,
    CONSTRUCTOR,    BUILTIN,        PARAMETER,      UNDEFINE, // = null
    // modifiers
    PUBLIC,         PROTECTED,      PRIVATE,        STATIC,
    ABSTRACT,       FINAL,          NATIVE,         SYNCHRONIZED,
    TRANSIENT,      VOLATILE,       STRICTFP,       UNKNOWN
};

class SymbolInfo
{
  public:
    SymbolInfo(int type, int u, size = -1)
        : typeIndex_(type), flags_(u), arraySize_(size)
    {}
    SymbolInfo(int type, std::bitset<64> flags, size = -1)
        : typeIndex_(type), flags_(flags), arraySize_(size)
    {}

    int                 getType() const;
    bool                isArray() const;
    int                 getArraySize() const;
    void                setArraySize(int s);
    bool                check(int i);   // 检查标识表某位是否为真

    virtual std::string toString() const;

  private:
    int                 typeIndex_;     // 变量类型/返回类型
    std::bitset<64>     flags_;         // 标识表，相关位为SymbolTag
    int                 arraySize_;     // 若是数组类型，存储数组长度，不是数组则为-1
};

```

#### 3. MethodInfo(用于存储方法信息)

```cpp
class MethodInfo : public SymbolInfo
{
  public:
    MethodInfo(int type, size = -1)
        : SymbolInfo(type, 1<<SymbolTag::METHOD, size)
    {}
    MethodInfo(int type, std::bitset<64> flags, size = -1)
        : SymbolInfo(type, flags, size)
    {}

    int                 parameterNum() const;
    void                addParameter(int type);
    bool                checkParameter(std::vector<int> args) const;
    bool                checkParameter(int type, int pos) const;
    std::string         toString() const;

  private:
    std::vector<int>    parameters_;
};

```

### <a name="符号表结构">符号表结构
```cpp
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

    // classes operations
    void                addClass(const std::string &name, int modifier = 0);
    void                enterClass(const std::string &name);
    void                leaveClass();

    // current table operations
    void                add(const std::string &name, const SymbolInfo &info);
    void                add(const std::string &name, const MethodInfo &info);
    bool                hasVariable(const std::string &name, bool searchUP = true) const;
    bool                hasMethod(const std::string &name, bool searchUp = true) const;
    const SymbolInfo &  getVariableInfo(const std::string &name) const;
    const MethodInfo &  getMethodInfo(const std::string &name) const;

    void                dump(); // for debug

  private:
    SymbolTable();

  private:
    std::map<std::string, int>          typeTable_;
    std::vector<TypeInfo>               typeInfoTable_;
    ClassTable*                         currentTable_;
    std::map<std::string, ClassTable*>  classesTable_;

    static SymbolTable *                instance_;
};

```

## <a name="语义分析器">语义分析器
// TODO

### <a name="语义规则">语义规则
// TODO

## <a name="中间代码生成器">中间代码生成器
// TODO

## <a name="目标代码生成器">目标代码生成器
// TODO
