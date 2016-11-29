#ifndef AST_H_
#define AST_H_

#include <string>
#include <vector>
#include <memory>
#include "../lexer/token.h"
#include "vistor.h"

namespace ycc
{
    struct AstNode;
    struct Expr;
    struct Stmt;

    /*
    using ASTNodePtr    = std::shared_ptr<ASTNode>;
    using StmtPtr       = std::shared_ptr<Stmt>;
    using ExprPtr       = std::shared_ptr<Expr>;
    */
    using ASTNodePtr    = ASTNode*;
    using StmtPtr       = Stmt*;
    using ExprPtr       = Expr*;
    using VecNodePtr    = std::vector <ASTNodePtr>;
    using VecExprPtr    = std::vector <ExprPtr>;
    using VecStmtPtr    = std::vector <StmtPtr>;


    struct ASTNode
    {
        ASTNode(){}
        ASTNode(const TokenLocation &loc)
            : loc_(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }
        const TokenLocation &getLocation()
        {
            return loc_;
        }

        virtual ~ASTNode() = default;
      private:
        TokenLocation   loc_;
    };

    /*************************************************
     * Statements
     ************************************************/

    struct Stmt : public ASTNode
    {
        Stmt(const TokenLocation &loc)
            : ASTNode(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }
        virtual ~Stmt() = default;
    };

    struct ClassStmt : public Stmt
    {
        ClassStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        std::string                 name;
        StmtPtr                     body;
    };

    struct MethodDeclStmt : public Stmt
    {
        MethodDeclStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        std::string     name;
        StmtPtr         body;
    };

    struct PrimaryStmt : public Stmt
    {
        PrimaryStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        std::string                 type;
        VecExprPtr                  decls;
    };

    struct EmptyStmt : public Stmt
    {
        EmptyStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

    };

    struct BlockStmt : public Stmt
    {
        BlockStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        VecStmtPtr  statements;
    };

    struct IfStmt : public Stmt
    {
        IfStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        ExprPtr       condition;
        StmtPtr       thenBody;
        StmtPtr       elseBody;
    };

    struct ForStmt : public Stmt
    {
        ForStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        ExprPtr     init;
        ExprPtr     condition;
        ExprPtr     update;
        StmtPtr     body;
    };

    struct WhileStmt : public Stmt
    {
        WhileStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        ExprPtr         condition;
        StmtPtr         body;
    };

    struct DoStmt : public Stmt
    {
        DoStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        ExprPtr     condition;
        StmtPtr     body;
    };

    struct SwitchStmt : public Stmt
    {
        SwitchStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        ExprPtr                 flag;           // int
        VecStmtPtr              cases;
        VecStmtPtr              defaultBody;
    };

    struct CaseStmt : public Stmt
    {
        CaseStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        ExprPtr                 label;
        VecStmtPtr              statements;
    };

    struct BreakStmt : public Stmt
    {
        BreakStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }
        // TODO:break label
    };

    struct ContinueStmt : public Stmt
    {
        ContinueStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }
        // TODO:continue label
    };

    struct ReturnStmt : public Stmt
    {
        ReturnStmt(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        ExprPtr         returnValue;
    };

    /**************************************************
     * Expressions
     *************************************************/
    struct Expr : public Stmt
    {
        Expr(const TokenLocation &loc)
            : Stmt(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }
        void setType(int index)
        {
            typeIndex_ = index;
        }
        int getType() const
        {
            return typeIndex_;
        }

      private:
        int         typeIndex_;
    };

    struct VariableDeclExpr : public Expr
    {
        VariableDeclExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        std::string         name;
        ExprPtr             initValue;
    };

    struct IdentifierExpr : public Expr
    {
        IdentifierExpr(const TokenLocation &loc, const std::string &n)
            : Expr(loc), name(n)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        std::string     name;
    };

    struct NewExpr : public Expr
    {
        NewExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }
        ExprPtr         constructor;        // call expr
        VecExprPtr      spaceExpr;          // int
    };

    struct IndexExpr : public Expr
    {
        IndexExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        ExprPtr     left;
        ExprPtr     index;
    };

    struct CallExpr : public Expr
    {
        CallExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        ExprPtr         name;
        VecExprPtr      arguments;
    };

    struct QualifiedIdExpr : public Expr
    {
        QualifiedIdExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }
        ExprPtr         left;
        ExprPtr         right;
    };

    struct IntExpr : public Expr
    {
        IntExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        bool        isChar;
        std::string lexeme;
        long long   value;
    };

    struct RealExpr : public Expr
    {
        RealExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        std::string lexeme;
        double      value;
    };

    struct BoolExpr : public Expr
    {
        BoolExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        bool    value;
    };

    struct NullExpr : public Expr
    {
        NullExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }
    };

    struct StrExpr : public Expr
    {
        StrExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        std::string     value;
    };

    struct ArrayExpr : public Expr
    {
        ArrayExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        VecExprPtr      elems;
    };

    struct UnaryOpExpr : public Expr
    {
        UnaryOpExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        bool        isPrefix;
        TokenTag    op;
        ExprPtr     expr;
    };

    struct BinaryOpExpr : public Expr
    {
        BinaryOpExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        TokenTag    op;
        ExprPtr     left;
        ExprPtr     right;
    };

    struct TernaryOpExpr : public Expr
    {
        TernaryOpExpr(const TokenLocation &loc)
            : Expr(loc)
        {}
        virtual void accept(ASTVistor *v)
        {
            v->visit(this);
        }

        ExprPtr     condition;
        ExprPtr     thenValue;
        ExprPtr     elseValue;
    };

}

#endif
