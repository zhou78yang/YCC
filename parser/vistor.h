#ifndef VISTOR_H_
#define VISTOR_H_

namespace ycc
{
    class ASTVistor
    {
    public:
        virtual ~ASTVistor() = default;
        virtual void visit(struct ASTNode *node) = 0;
        virtual void visit(struct Stmt *node) = 0;
        virtual void visit(struct EmptyStmt *node) = 0;
        virtual void visit(struct ClassStmt *node) = 0;
        virtual void visit(struct MethodDeclStmt *node) = 0;
        virtual void visit(struct PrimaryStmt *node) = 0;
        virtual void visit(struct VariableDeclExpr *node) = 0;
        virtual void visit(struct BlockStmt *node) = 0;
        virtual void visit(struct IfStmt *node) = 0;
        virtual void visit(struct ForStmt *node) = 0;
        virtual void visit(struct WhileStmt *node) = 0;
        virtual void visit(struct DoStmt *node) = 0;
        virtual void visit(struct SwitchStmt *node) = 0;
        virtual void visit(struct CaseStmt *node) = 0;
        virtual void visit(struct ReturnStmt *node) = 0;
        virtual void visit(struct BreakStmt *node) = 0;
        virtual void visit(struct ContinueStmt *node) = 0;

        virtual void visit(struct Expr *node) = 0;
        virtual void visit(struct IdentifierExpr *node) = 0;
        virtual void visit(struct NewExpr *node) = 0;
        virtual void visit(struct IndexExpr *node) = 0;
        virtual void visit(struct CallExpr *node) = 0;
        virtual void visit(struct QualifiedIdExpr *node) = 0;
        virtual void visit(struct IntExpr *node) = 0;
        virtual void visit(struct RealExpr *node) = 0;
        virtual void visit(struct BoolExpr *node) = 0;
        virtual void visit(struct NullExpr *node) = 0;
        virtual void visit(struct StrExpr *node) = 0;
        virtual void visit(struct ArrayExpr *node) = 0;
        virtual void visit(struct UnaryOpExpr *node) = 0;
        virtual void visit(struct BinaryOpExpr *node) = 0;
        virtual void visit(struct TernaryOpExpr *node) = 0;
    };
}

#endif
