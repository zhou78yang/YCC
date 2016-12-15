#ifndef IRGENERATOR_H_
#define IRGENERATOR_H_

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
        void visit(VecNodePtr ast);

    private:
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
        void writeMethod_begin(int type, std::string name);
        void writeMethod_end(int type);
        void writeAlloca(std::string Name,int Type);
        void writeStore(int Type,std::string oneName,std::string twoName);        // twoName->oneName
        void writeLoad(int labelCount,int Type,std::string Name);                //Name->labelCount
        void writeLabel(int count);
        void writeJump(int Label);
        void writeCJump(int C,int thenLabel,int elseLabel);
        void writeBinaryOp();

    private:
    	SymbolInfo      	*info;
        SymbolTable *       symbolTable_;
        int                 labelCount_;
        bool				oneVistor_; 
        bool 				fuzhi_;
        std::string			fuzhi_name;
        std::string	    	numeric(int typeIndex);
        bool 				constant_;
        std::string			constant_value;
        bool				incre_;
        std::string        	change_Op(std::string op);
    };
}

#endif
