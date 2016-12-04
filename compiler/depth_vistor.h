#ifndef DEPTH_VISTOR_H_
#define DEPTH_VISTOR_H_

#include "../parser/vistor.h"
#include "../parser/ast.hpp"

namespace ycc
{
    class DepthVistor : public ASTVistor
    {
    public:
        DepthVistor();
        ~DepthVistor() = default;

        virtual void visit(VecNodePtr ast);
        virtual void visit(ASTNode *node);
        virtual void visit(Stmt *node);
        virtual void visit(EmptyStmt *node);
        virtual void visit(ClassStmt *node);
        virtual void visit(MethodDeclStmt *node);
        virtual void visit(PrimaryStmt *node);
        virtual void visit(BlockStmt *node);
        virtual void visit(IfStmt *node);
        virtual void visit(ForStmt *node);
        virtual void visit(WhileStmt *node);
        virtual void visit(DoStmt *node);
        virtual void visit(SwitchStmt *node);
        virtual void visit(CaseStmt *node);
        virtual void visit(ReturnStmt *node);
        virtual void visit(BreakStmt *node);
        virtual void visit(ContinueStmt *node);

        virtual void visit(Expr *node);
        virtual void visit(VariableDeclExpr *node);
        virtual void visit(IdentifierExpr *node);
        virtual void visit(NewExpr *node);
        virtual void visit(IndexExpr *node);
        virtual void visit(CallExpr *node);
        virtual void visit(QualifiedIdExpr *node);
        virtual void visit(IntExpr *node);
        virtual void visit(RealExpr *node);
        virtual void visit(BoolExpr *node);
        virtual void visit(NullExpr *node);
        virtual void visit(StrExpr *node);
        virtual void visit(ArrayExpr *node);
        virtual void visit(UnaryOpExpr *node);
        virtual void visit(BinaryOpExpr *node);
        virtual void visit(TernaryOpExpr *node);

    private:
        void            println(const std::string &msg, bool end = false);
        void            upgrade();
        void            degrade();

        int             level;
    };
}

#endif
