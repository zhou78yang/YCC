#ifndef PARSER_H_
#define PARSER_H_

#include "../common/error.h"
#include "../common/symbols.h"
#include "../common/symbol_table.h"
#include "../lexer/scanner.h"
#include "ast.hpp"

namespace ycc
{
    class Parser
    {
    public:
        explicit        Parser(Scanner &scanner);
        VecNodePtr      parse();

        static void     setErrorFlag(bool flag);
        static bool     getErrorFlag();

    private:
        void            advance();
        bool            match(TokenTag tag, bool advanceToNextToken = false);
        bool            match(TokenTag tag, const std::string &msg, bool advanceToNextToken = false);
        void            addToBuffer();
        void            reduceBuffer();
        void            clearBuffer();
        void            errorReport(const std::string &msg, ErrorType type = ErrorType::ERROR);
        TokenLocation   getLocation() const;

        // parse part
        StmtPtr         parseStmt(bool optional = false);
        StmtPtr         parseEmpty();
        StmtPtr         parseClass(SymbolFlag modifiers);
        StmtPtr         parseClassBody();
        StmtPtr         parseMemberDecl(SymbolFlag modifiers);
        StmtPtr         parseVariableDecl(std::string name, std::string type, SymbolFlag modifiers);
        StmtPtr         parseMethodDecl(std::string name, std::string type, SymbolFlag modifiers);
        StmtPtr         parseBlock();
        StmtPtr         parseIf();
        StmtPtr         parseFor();
        StmtPtr         parseWhile();
        StmtPtr         parseDo();
        StmtPtr         parseSwitch();
        StmtPtr         parseCase();
        StmtPtr         parseReturn();
        StmtPtr         parseBreak();
        StmtPtr         parseContinue();
        StmtPtr         parseExprStatement();

        ExprPtr         parseExpr(bool optional = false, int precedence = 0);
        ExprPtr         parseIdentifier();
        ExprPtr         parseIndex(ExprPtr left);
        ExprPtr         parseCall(ExprPtr left);
        ExprPtr         parseQualifiedId(ExprPtr left);
        ExprPtr         parseNew();
        ExprPtr         parseInt(bool isChar = false);
        ExprPtr         parseReal();
        ExprPtr         parseBool(bool value);
        ExprPtr         parseNull();
        ExprPtr         parseStr();
        ExprPtr         parseArray();
        ExprPtr         parseUnaryOp(ExprPtr left, int precedence);
        ExprPtr         parseBinaryOp(ExprPtr left, int precedence);
        ExprPtr         parseTernaryOp(ExprPtr left, int precedence);

        // others
        void            dump(std::string msg);// for debug

    private:
        Token               token_;
        std::vector<Token>  buffer_;
        Scanner&            scanner_;
        SymbolTable*        symbolTable_;
        VecNodePtr          ast_;

        static bool         errorFlag_;
    };

    inline bool Parser::getErrorFlag()
    {
        return errorFlag_;
    }

    inline void Parser::setErrorFlag(bool flag)
    {
        errorFlag_ = flag;
    }

    inline TokenLocation Parser::getLocation() const
    {
        return scanner_.getTokenLocation();
    }
}

#endif
