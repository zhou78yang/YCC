#ifndef IRGENERATOR_H_
#define IRGENERATOR_H_

#include "../parser/vistor.h"
#include "../parser/ast.hpp"
#include <fstream>

namespace ycc
{
    class IRGenerator : public ASTVistor
    {
    public:
        IRGenerator(const std::string &name);
        ~IRGenerator() = default;

        void gene(VecNodePtr ast);

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
        void writeMethod_begin(std::string name,MethodInfo mInfo);
        void writeMethod_end(MethodInfo mInfo);
        void writeAlloca(std::string Name,int Type);
        void writeStore(int type,std::string oneName,std::string twoName);        // twoName->oneName
        void writeStore_value(int type,std::string oneName_value,std::string twoName);
        
        
        void writeLoad(int labelCount,int Type,std::string Name);                //Name->labelCount
        void writeLabel(int count);
        void writeJump(int Label);
        void writeCJump(int C,int thenLabel,int elseLabel);
        void writeCall(int labelCount,std::string name,MethodInfo mInfo);
        void writeBinaryOp(int labelCount,std::string op,int type,std::string lCount,std::string rCount,int model);
        

    private:
    	std::ofstream 		output_;
    	SymbolInfo      	*info;
        SymbolTable *       symbolTable_;
        int                 labelCount_;
        bool				oneVistor_;
        int					primaryType;
        bool 				fuzhi_;
        std::string			fuzhi_name;
        std::string	    	numeric(int typeIndex);
        bool 				constant_;
        std::string			constant_value;
        bool				incre_;
        bool				callRet_;
        bool				call_value;
        int 				call_index;
        std::string 		call_name;
        
        bool				ret_;
        std::string			ret_value;

        std::vector<int>    breakStack_;
        std::vector<int>    continueStack_;
    };
}

#endif
