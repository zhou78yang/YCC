#ifndef IRGENERATOR_H_
#deifne IRGENERATOR_H_

#include "../parser/vistor.h"
#include "../parser/ast.hpp"

namespace ycc
{
    class IRGenerator : public ASTVistor
    {
    public:
        IRGenerator();
        ~IRGenerator() = default;

        void gene(VecNodePtr ast);

    private:
		void visit(VecNodePtr ast);
        void visit(ASTNode *node);
        void visit(Stmt *node);
        void visit(EmptyStmt *node);
        void visit(ClassStmt *node);
        void visit(MethodDeclStmt *node);
        void visit(PrimaryStmt *node);
        void visit(BlockStmt *node);
        void visit(IfStmt *node);
        void visit(ForStmt *node);
        void visit(WhileStmt *node);
        void visit(DoStmt *node);
        void visit(SwitchStmt *node);
        void visit(CaseStmt *node);
        void visit(ReturnStmt *node);
        void visit(BreakStmt *node);
        void visit(ContinueStmt *node);

        void visit(Expr *node);
        void visit(VariableDeclExpr *node);
        void visit(IdentifierExpr *node);
        void visit(NewExpr *node);
        void visit(IndexExpr *node);
        void visit(CallExpr *node);
        void visit(QualifiedIdExpr *node);
        void visit(IntExpr *node);
        void visit(RealExpr *node);
        void visit(BoolExpr *node);
        void visit(NullExpr *node);
        void visit(StrExpr *node);
        void visit(ArrayExpr *node);
        void visit(UnaryOpExpr *node);
        void visit(BinaryOpExpr *node);
        void visit(TernaryOpExpr *node);

        // generator tools
        void writeMethod();
        void writeAlloca();
        void writeStore();
        void writeLabel();
        void writeJump();
        void writeCJump();
        ...

    private:
        SymbolTable *       symbolTable_;
        int                 labelCount_;
    };
}

#endif
