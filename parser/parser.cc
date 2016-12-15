#include "parser.h"
#include <iostream>

namespace ycc
{
    bool Parser::errorFlag_ = false;

    Parser::Parser(Scanner &scanner)
        : scanner_(scanner)
    {
        symbolTable_ = SymbolTable::getInstance();
        advance();          // get first token
    }

    // inner operations
    void Parser::advance()
    {
        token_ = scanner_.getNextToken();
        //std::cout << token_.toString() << std::endl;
    }

    bool Parser::match(TokenTag tag, bool advanceToNextToken /* = false */)
    {
        if(token_.tag() == tag)
        {
            if(advanceToNextToken)
            {
                advance();
            }
            return true;
        }
        return false;
    }

    bool Parser::match(TokenTag tag, const std::string &name, bool advanceToNextToken /* = false */)
    {
        if(token_.tag() != tag)
        {
            errorReport("expected " + tokenDesc(tag) + ", but find '" + name + "'");
            return false;
        }
        if(advanceToNextToken)
        {
            advance();
        }
        return true;
    }

    void Parser::addToBuffer()
    {
        buffer_.push_back(token_);
    }

    void Parser::reduceBuffer()
    {
        buffer_.pop_back();
    }

    void Parser::clearBuffer()
    {
        buffer_.clear();
    }

    void Parser::errorReport(const std::string &msg, ErrorType type)
    {
        ExceptionHandler::getInstance()->add(msg, getLocation(), type);
        setErrorFlag(true);
    }



    VecNodePtr Parser::parse()
    {
        ast_.clear();
        while(true)
        {
            if(match(TokenTag::END_OF_FILE))
            {
                break;
            }

            SymbolFlag modifiers(0);
            while(isModifier(token_.tag()))
            {
                modifiers.set(isModifier(token_.tag()));
                advance();
            }

            //dump("parse class begin");
            ASTNodePtr currentNodePtr = parseClass(modifiers);
            //dump("parse class end");

            if(currentNodePtr != nullptr)
            {
                ast_.push_back(currentNodePtr);
            }
        }

        return ast_;
    }


    /****************************************************
     * Statements
     ***************************************************/

    //ClassDeclaration ::= {MODIFIER} class IDENTIFIER [extends Type] ClassBody
    StmtPtr Parser::parseClass(SymbolFlag modifiers)
    {
        match(TokenTag::CLASS, token_.lexeme(), true);  // class
        auto node = new ClassStmt(getLocation());
        node->name = token_.lexeme();
        symbolTable_->addClass(node->name);
        advance();                                      // IDENTIFIER

        //dump("parse class " + node->name + " begin");
        match(TokenTag::LEFT_BRACE, token_.lexeme(), true);    // {
        node->body = parseClassBody();
        match(TokenTag::RIGHT_BRACE, token_.lexeme(), true);   // }
        //dump("parse class " + node->name + " end");

        symbolTable_->leaveClass();

        return node;
    }

    // ClassBody ::= EmptyStmt | [static]Block | {MODIFIER} MemberDeclaration
    StmtPtr Parser::parseClassBody()
    {
        auto classBody = new BlockStmt(getLocation());
        while(!match(TokenTag::RIGHT_BRACE))
        {
            // reduce empty statement
            while(match(TokenTag::SEMICOLON))
            {
                advance();
            }

            // static block
            if(match(TokenTag::LEFT_BRACE))
            {
                classBody->statements.push_back(parseBlock());
                continue;
            }

            // member declaration
            SymbolFlag modifiers(0);
            while(isModifier(token_.tag()))
            {
                modifiers.set(isModifier(token_.tag()));
                advance();
            }
            classBody->statements.push_back(parseMemberDecl(modifiers));
        }

        return classBody;
    }

    StmtPtr Parser::parseEmpty()
    {
        auto node = new EmptyStmt(getLocation());
        advance();
        return node;
    }

    // MemberDeclaration ::= Type IDENTIFIER MemberDeclRest
    // MemberDeclRest ::= VariableDeclRest | MethodDeclRest
    StmtPtr Parser::parseMemberDecl(SymbolFlag modifiers)
    {
        modifiers.set(SymbolTag::MEMBER);

        // member class declaration
        if(match(TokenTag::CLASS))
        {
            return parseClass(modifiers);
        }

        auto type = token_.lexeme();
        if(!symbolTable_->hasType(type))
        {
            errorReport("undefined type " + type);
            // TODO:handle undefine type
        }
        advance();                  // eat type
        while(match(TokenTag::LEFT_SQUARE, true))
        {

            int typeIndex = symbolTable_->getTypeIndex(type);
            int wd = symbolTable_->getTypeInfo(typeIndex).getWidth();

            match(TokenTag::RIGHT_SQUARE, token_.lexeme(), true);
            type += "[]";
            if(!symbolTable_->hasType(type))
            {
                symbolTable_->addType(type, wd);
            }
        }

        auto name = token_.lexeme();
        advance();                  // eat name

        // member method declaration
        if(match(TokenTag::LEFT_PAREN, true))
        {
            modifiers.set(SymbolTag::METHOD);
            return parseMethodDecl(name, type, modifiers);
        }

        // member variable declaration
        if(match(TokenTag::ASSIGN) || match(TokenTag::COMMA) || match(TokenTag::SEMICOLON))
        {
            modifiers.set(SymbolTag::VARIABLE);
            return parseVariableDecl(name, type, modifiers);
        }

        errorReport("unexpected token");
        return nullptr;
    }

    // VariableDeclaration ::= Type IDENTIFIER [=Expr] { , IDENTIFIER [=Expr] } ;
    StmtPtr Parser::parseVariableDecl(std::string name, std::string type, SymbolFlag modifiers)
    {
        //dump("parse variable declaration begin");
        // current token is '=' , ',' or ';'
        auto node = new PrimaryStmt(getLocation());
        node->type = type;
        node->flags = modifiers;

        // get symbol info
        SymbolInfo info(symbolTable_->getTypeIndex(type), modifiers);

        auto var = new VariableDeclExpr(getLocation());
        var->name = name;

        /* we should not add variable to symbol table in parser
        // add class member to symbol table
        if(modifiers.test(SymbolTag::MEMBER))
        {
            if(symbolTable_->hasVariable(var->name))
            {
                errorReport("member variable redefined: " + name);
            }
            else
            {
                symbolTable_->add(var->name, info);
            }
        }
        */

        // eat [=Expr]
        if(match(TokenTag::ASSIGN, true))
        {
            //dump("parse init value begin");
            var->initValue = parseExpr();
            //dump("parse init value end");
        }

        node->decls.push_back(var);

        // eat { , IDENTIFIER [ = Expr ] }
        while(!match(TokenTag::SEMICOLON))
        {
            if(!match(TokenTag::COMMA, token_.lexeme(), true))
            {
                break;
            }

            auto var = new VariableDeclExpr(getLocation());
            var->name = token_.lexeme();

            /* we should not add variable to symbol table in parser
            // add class member to symbol table
            if(modifiers.test(SymbolTag::MEMBER))
            {
                if(symbolTable_->hasVariable(var->name))
                {
                    errorReport("member redefined: " + name);
                }
                else
                {
                    symbolTable_->add(var->name, info);
                }
            }
            */
            advance();

            // eat [=Expr]
            if(match(TokenTag::ASSIGN, true))
            {
                var->initValue = parseExpr();
            }
            node->decls.push_back(var);
        }
        match(TokenTag::SEMICOLON, token_.lexeme(), true);

        //dump("parse variable declaration end");
        return node;
    }

    // MethodDeclaration ::= Type IDENTIFIER([Type IDENTIFIER { , Type IDENTIFIER} ]) Block
    StmtPtr Parser::parseMethodDecl(std::string name, std::string type, SymbolFlag modifiers)
    {
        bool errorFlag = false; // if error flag is true, don't add method to symbol table
        //dump("parse method declaration begin");
        // current token is ')' or first parameter
        auto node = new MethodDeclStmt(getLocation());
        node->name = name;

        MethodInfo info(symbolTable_->getTypeIndex(type), modifiers);

        // eat parameters
        if(!match(TokenTag::RIGHT_PAREN))
        {
            auto parameterType = token_.lexeme();
            advance();

            if(!symbolTable_->hasType(parameterType))
            {
                errorReport("undefine type of parameter");
                errorFlag = true;
                while(!match(TokenTag::RIGHT_PAREN))
                {
                    advance();
                }
            }

            while(match(TokenTag::LEFT_SQUARE, true))
            {

                int typeIndex = symbolTable_->getTypeIndex(parameterType);
                int wd = symbolTable_->getTypeInfo(typeIndex).getWidth();

                match(TokenTag::RIGHT_SQUARE, token_.lexeme(), true);
                parameterType += "[]";
                if(!symbolTable_->hasType(parameterType))
                {
                    symbolTable_->addType(parameterType, wd);
                }
            }

            // TODO : parameter name check
            auto parameter = token_.lexeme();
            advance();

            // add parameter to method info
            info.addParameter(symbolTable_->getTypeIndex(parameterType), parameter);

            // eat { ,type name }
            while(!match(TokenTag::RIGHT_PAREN))
            {
                if(!match(TokenTag::COMMA, token_.lexeme(), true))
                {
                    break;
                }

                parameterType = token_.lexeme();      // get type
                advance();

                while(match(TokenTag::LEFT_SQUARE, true))
                {
                    int typeIndex = symbolTable_->getTypeIndex(parameterType);
                    int wd = symbolTable_->getTypeInfo(typeIndex).getWidth();

                    match(TokenTag::RIGHT_SQUARE, token_.lexeme(), true);
                    parameterType += "[]";
                    if(!symbolTable_->hasType(parameterType))
                    {
                        symbolTable_->addType(parameterType, wd);
                    }
                }

                auto parameter = token_.lexeme();
                advance();

                // add parameter to symbol table
                info.addParameter(symbolTable_->getTypeIndex(parameterType), parameter);
            }
        }
        match(TokenTag::RIGHT_PAREN, token_.lexeme(), true);

        // TODO : I haven't check overload method declaration
        if(!errorFlag)
        {
            symbolTable_->add(name, info);
        }

        if(!match(TokenTag::LEFT_BRACE)){
            errorReport("expected '{' before method body");
        }
        node->body = parseBlock();

        //dump("parse method declaration end");
        return node;
    }

    // Block ::= '{' {Stmt} '}'
    StmtPtr Parser::parseBlock()
    {
        auto node = new BlockStmt(getLocation());
        advance();

        while(!match(TokenTag::RIGHT_BRACE))
        {
            auto stmt = parseStmt();
            node->statements.push_back(stmt);
        }

        match(TokenTag::RIGHT_BRACE, token_.lexeme(), true);

        return node;
    }

    StmtPtr Parser::parseStmt(bool optional /* = false */)
    {
        // current token is the head of a statement
        switch(token_.tag())
        {
        case TokenTag::SEMICOLON:             // ;
            return parseEmpty();
        case TokenTag::LEFT_BRACE:            // {
            return parseBlock();
        case TokenTag::IF:                    // if
            return parseIf();
        case TokenTag::FOR:                   // for
            return parseFor();
        case TokenTag::WHILE:                 // while
            return parseWhile();
        case TokenTag::DO:                    // do
            return parseDo();
        case TokenTag::SWITCH:                // switch
            return parseSwitch();
        case TokenTag::RETURN:                // return
            return parseReturn();
        case TokenTag::CONTINUE:              // continue
            return parseContinue();
        case TokenTag::BREAK:                 // break
            return parseBreak();
        case TokenTag::RIGHT_PAREN:           // close separator
        case TokenTag::RIGHT_SQUARE:
        case TokenTag::RIGHT_BRACE:
            // optional statement
            break;
        case TokenTag::COMMA:                 // error separator
        case TokenTag::PERIOD:
            errorReport("unexpected token " + token_.lexeme());
            advance();
            return parseStmt(optional);
        case TokenTag::END_OF_FILE:           // eof
            break;
        default:
            return parseExprStatement();
        }

        if(!optional){
            errorReport("expected statement");
        }
        return nullptr;
    }

    // IfStmt ::= if(Expr) Stmt [else Stmt]
    StmtPtr Parser::parseIf()
    {
        auto node = new IfStmt(getLocation());
        advance();                                          // if
        match(TokenTag::LEFT_PAREN, token_.lexeme(), true); // (
        node->condition = parseExpr();                      // expr
        match(TokenTag::RIGHT_PAREN, token_.lexeme(), true);// )
        node->thenBody = parseStmt();                       // Stmt

        // eat [else Stmt]
        if(match(TokenTag::ELSE, true))
        {
            node->elseBody = parseStmt();
        }

        return node;
    }

    // ForStmt ::= for(init; cond; update) Stmt
    StmtPtr Parser::parseFor()
    {
        auto node = new ForStmt(getLocation());
        advance();                                          // for
        match(TokenTag::LEFT_PAREN, token_.lexeme(), true); // (
        node->init = parseExpr(true);                       // init
        match(TokenTag::SEMICOLON, token_.lexeme(), true);  // ;
        node->condition = parseExpr(true);                  // cond
        match(TokenTag::SEMICOLON, token_.lexeme(), true);  // ;
        node->update = parseExpr(true);                     // update
        match(TokenTag::RIGHT_PAREN, token_.lexeme(), true);// )
        node->body = parseStmt();                           // Stmt

        return node;
    }

    // WhileStmt ::= while(cond) Stmt
    StmtPtr Parser::parseWhile()
    {
        auto node = new WhileStmt(getLocation());
        advance();                                          // while
        match(TokenTag::LEFT_PAREN, token_.lexeme(), true); // (
        node->condition = parseExpr();                      // cond
        match(TokenTag::RIGHT_PAREN, token_.lexeme(), true);// )
        node->body = parseStmt();                           // Stmt

        return node;
    }

    // DoStmt ::= do Stmt while(cond);
    StmtPtr Parser::parseDo()
    {
        auto node = new DoStmt(getLocation());
        advance();                                          // do
        node->body = parseStmt();                           // Stmt
        match(TokenTag::WHILE, token_.lexeme(), true);      // while
        match(TokenTag::LEFT_PAREN, token_.lexeme(), true); // (
        node->condition = parseExpr();                      // cond
        match(TokenTag::RIGHT_PAREN, token_.lexeme(), true);// )
        match(TokenTag::SEMICOLON, token_.lexeme(), true);  // ;

        return node;
    }

    // SwitchStmt ::= switch(Expr)'{' {CaseBody} [DefaultBody] '}'
    // DefaultBody ::= default: {Stmt}
    StmtPtr Parser::parseSwitch()
    {
        auto node = new SwitchStmt(getLocation());
        advance();                                          // switch
        match(TokenTag::LEFT_PAREN, token_.lexeme(), true); // (
        node->flag = parseExpr();                           // expr
        match(TokenTag::RIGHT_PAREN, token_.lexeme(), true);// )
        match(TokenTag::LEFT_BRACE, token_.lexeme(), true); // {

        // parse {CaseBody}
        while(match(TokenTag::CASE))
        {
            node->cases.push_back(parseCase());
        }

        // parse [DefaultBody]
        if(match(TokenTag::DEFAULT, true))
        {
            match(TokenTag::COLON, token_.lexeme(), true);  // :
            while(!match(TokenTag::RIGHT_BRACE))
            {
                auto stmt = parseStmt();
                if(!stmt)
                {
                    break;
                }
                node->defaultBody.push_back(stmt);
            }
        }
        else
        {
            errorReport("expected default statement", ErrorType::WARNING);
        }

        match(TokenTag::RIGHT_BRACE, token_.lexeme(), true);    // }

        return node;
    }

    // CaseBody ::= case Expr: {Stmt}
    StmtPtr Parser::parseCase()
    {
        auto node = new CaseStmt(getLocation());
        advance();                                      // case
        // TODO: label may support int constants
        node->label = parseInt(token_.tag() == TokenTag::CHAR_LITERAL);  // Expr
        match(TokenTag::COLON, token_.lexeme(), true);  // :

        while(true)
        {
            if(match(TokenTag::CASE) || match(TokenTag::END_OF_FILE)
            || match(TokenTag::DEFAULT) || match(TokenTag::RIGHT_BRACE))
            {
                break;
            }
            node->statements.push_back(parseStmt());
        }
        return node;
    }

    // ReturnStmt ::= return [Expr];
    StmtPtr Parser::parseReturn()
    {
        auto node = new ReturnStmt(getLocation());
        advance();                                          // return;
        node->returnValue = parseExpr(true);                // Expr
        match(TokenTag::SEMICOLON, token_.lexeme(), true);  // ;

        return node;
    }

    // BreakStmt ::= break;
    StmtPtr Parser::parseBreak()
    {
        auto node = new BreakStmt(getLocation());
        match(TokenTag::BREAK, token_.lexeme(), true);      // break
        match(TokenTag::SEMICOLON, token_.lexeme(), true);  // ;

        return node;
    }

    // ContinueStmt ::= continue;
    StmtPtr Parser::parseContinue()
    {
        auto node = new ContinueStmt(getLocation());
        match(TokenTag::CONTINUE, token_.lexeme(), true);   // continue
        match(TokenTag::SEMICOLON, token_.lexeme(), true);  // ;

        return node;
    }

    // ExprStatement ::= VariableDeclaration | Expr;
    StmtPtr Parser::parseExprStatement()
    {
        // check variable declaration
        SymbolFlag modifiers(0);
        while(isModifier(token_.tag()))
        {
            modifiers.set(isModifier(token_.tag()));
            advance();
        }
        if(symbolTable_->hasType(token_.lexeme()))
        {
            auto type = token_.lexeme();
            advance();                      // eat type

            // array check
            while(match(TokenTag::LEFT_SQUARE, true))
            {
                int typeIndex = symbolTable_->getTypeIndex(type);
                int wd = symbolTable_->getTypeInfo(typeIndex).getWidth();

                match(TokenTag::RIGHT_SQUARE, token_.lexeme(), true);
                type += "[]";
                if(!symbolTable_->hasType(type))
                {
                    symbolTable_->addType(type, wd);
                }
            }

            auto name = token_.lexeme();   // eat name
            advance();

            modifiers.set(SymbolTag::VARIABLE);
            return parseVariableDecl(name, type, modifiers);
        }

        // normal express statement
        auto node = parseExpr();
        match(TokenTag::SEMICOLON, token_.lexeme(), true);

        return node;
    }

    /***************************************************
     * Expresses
     **************************************************/

    ExprPtr Parser::parseExpr(bool optional /* = false */, int precedence /* = 0 */)
    {
        ExprPtr left = nullptr;
        switch(token_.tag())
        {
        case TokenTag::IDENTIFIER:
            left = parseIdentifier();
            break;
        case TokenTag::NEW:
            left = parseNew();
            break;
        case TokenTag::INT_LITERAL:
            left = parseInt();
            break;
        case TokenTag::CHAR_LITERAL:
            left = parseInt(true);
            break;
        case TokenTag::TRUE:
            left = parseBool(true);
            break;
        case TokenTag::FALSE:
            left = parseBool(false);
            break;
        case TokenTag::NONE:
            left = parseNull();
            break;
        case TokenTag::REAL_LITERAL:
            left = parseReal();
            break;
        case TokenTag::STR_LITERAL:
            left = parseStr();
            break;
        case TokenTag::LEFT_PAREN:
            advance();
            left = parseExpr();
            match(TokenTag::RIGHT_PAREN, token_.lexeme(), true);
            break;
        case TokenTag::LEFT_BRACE:
            left = parseArray();
            break;
        case TokenTag::SEMICOLON:
        case TokenTag::RIGHT_BRACE:
        case TokenTag::RIGHT_SQUARE:
        case TokenTag::RIGHT_PAREN:
            return nullptr;
        default:
            break;
        }

        // check prefix op
        if((!left) && (isPrefixOp(token_.tag())))
        {
            auto p = getSymbolPrecedence(token_.tag());
            left = parseUnaryOp(nullptr, p);
        }

        if(!left)
        {
            if(!optional)
            {
                errorReport("left should not null");
            }
            return nullptr;
        }

        // if left != nullptr, current token should be an operator
        while(true)
        {
            auto p = getSymbolPrecedence(token_.tag());
            if(isInfixOp(token_.tag()))
            {
                if(p >= precedence)
                {
                    left = parseBinaryOp(left, p);
                    continue;
                }
                else
                {
                    break;
                }
            }

            if(match(TokenTag::QUESTION_MARK))
            {
                return parseTernaryOp(left, p);
            }

            if(isPostfixOp(token_.tag()))
            {
                left = parseUnaryOp(left, p);
                continue;
            }
            break;
        }

        return left;
    }

    // Identifier ::= IDENTIFIER | IndexExpr | CallExpr | QualifiedIdentifier
    ExprPtr Parser::parseIdentifier()
    {
        // eat name
        auto left = new IdentifierExpr(getLocation(), token_.lexeme());
        advance();

        // parse Index
        if(match(TokenTag::LEFT_SQUARE))
        {
            auto node = parseIndex(left);
            while(match(TokenTag::LEFT_SQUARE))
            {
                node = parseIndex(node);
            }
            return node;
        }

        // call expr
        if(match(TokenTag::LEFT_PAREN))
        {
            return parseCall(left);
        }

        if(match(TokenTag::PERIOD))
        {
            return parseQualifiedId(left);
        }

        return left;
    }

    // IndexExpr ::= Identifier[Expr]
    ExprPtr Parser::parseIndex(ExprPtr left)
    {
        auto node = new IndexExpr(getLocation());
        node->left = left;
        advance();                                              // '['
        node->index = parseExpr();                              // Expr
        match(TokenTag::RIGHT_SQUARE, token_.lexeme(), true);   // ']'

        if(match(TokenTag::PERIOD))
        {
            return parseQualifiedId(node);
        }

        return node;
    }

    // CallExpr ::= Identifier([Expr {,Expr}])
    ExprPtr Parser::parseCall(IdentifierExpr *left)
    {
        auto node = new CallExpr(left->getLocation());
        node->callee = left->name;

        advance();                  // eat '('
        // eat [expr]
        if(!match(TokenTag::RIGHT_PAREN))
        {
            node->arguments.push_back(parseExpr());

            // eat {,expr}
            while(!match(TokenTag::RIGHT_PAREN))
            {
                match(TokenTag::COMMA, token_.lexeme(), true);
                node->arguments.push_back(parseExpr());
            }
        }
        match(TokenTag::RIGHT_PAREN, token_.lexeme(), true);

        if(match(TokenTag::PERIOD))
        {
            return parseQualifiedId(node);
        }

        return node;
    }

    // QualifiedIdentifier ::= Identifier1.Identifier2
    ExprPtr Parser::parseQualifiedId(ExprPtr left)
    {
        auto node = new QualifiedIdExpr(getLocation());
        advance();      // eat period .
        node->left = left;
        node->right = parseIdentifier();

        return node;
    }

    ExprPtr Parser::parseNew()
    {
        auto node = new NewExpr(getLocation());
        advance();  // eat "new"

        node->constructor = parseIdentifier();

        return node;
    }

    ExprPtr Parser::parseInt(bool isChar)
    {
        auto node = new IntExpr(getLocation());
        node->isChar = isChar;
        node->lexeme = token_.lexeme();
        // TODO : get terminal value
        advance();

        return node;
    }

    ExprPtr Parser::parseReal()
    {
        auto node = new RealExpr(getLocation());
        node->lexeme = token_.lexeme();
        // TODO : get terminal value
        advance();

        return node;
    }

    ExprPtr Parser::parseBool(bool value)
    {
        auto node = new BoolExpr(getLocation());
        node->value = value;
        advance();

        return node;
    }

    ExprPtr Parser::parseNull()
    {
        auto node = new NullExpr(getLocation());
        advance();

        return node;
    }

    ExprPtr Parser::parseStr()
    {
        auto node = new StrExpr(getLocation());
        node->value = token_.lexeme();
        advance();

        return node;
    }

    // Array ::= '{' [Expr {, Expr} ] '}'
    ExprPtr Parser::parseArray()
    {
        auto node = new ArrayExpr(getLocation());

        advance();                          // eat {
        if(!match(TokenTag::RIGHT_BRACE))
        {
            node->elems.push_back(parseExpr());
            while(!match(TokenTag::RIGHT_BRACE))
            {
                if(!match(TokenTag::COMMA, token_.lexeme(), true))
                {
                    break;
                }
                node->elems.push_back(parseExpr());
            }
        }
        match(TokenTag::RIGHT_BRACE, token_.lexeme(), true);

        return node;
    }

    ExprPtr Parser::parseUnaryOp(ExprPtr left, int precedence)
    {

        auto node = new UnaryOpExpr(getLocation());
        node->op = token_.tag();
        if(left)
        {
            node->isPrefix = false;
            node->expr = left;
            advance();              // advance to next token
        }
        else
        {
            node->isPrefix = true;
            advance();              // advance to next token
            node->expr = parseExpr(false, precedence);
        }

        return node;
    }

    ExprPtr Parser::parseBinaryOp(ExprPtr left, int precedence)
    {
        auto node = new BinaryOpExpr(getLocation());
        node->left = left;
        node->op = token_.tag();
        advance();
        node->right = parseExpr(false, precedence);

        return node;
    }

    ExprPtr Parser::parseTernaryOp(ExprPtr left, int precedence)
    {
        auto node = new TernaryOpExpr(getLocation());
        node->condition = left;
        match(TokenTag::QUESTION_MARK, token_.lexeme(), true);
        node->thenValue = parseExpr(false, precedence);
        match(TokenTag::COLON, token_.lexeme(), true);
        node->elseValue = parseExpr(false, precedence);
        return node;
    }

    // others
    void Parser::dump(std::string msg)
    {
        std::cout << msg << " at "
                  << token_.lexeme() << std::endl;
    }
}
