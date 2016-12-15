#include <iostream>
#include "compiler_vistor.h"
#include "../common/symbols.h"

using std::cout;
using std::endl;

namespace ycc
{

    CompilerVistor::CompilerVistor()
        : errorFlag_(false),variableFlag_(false)
    {
        symbolTable_ = SymbolTable::getInstance();
    }

    bool CompilerVistor::check()
    {
        // TODO: 对外接口，遍历一遍语法树进行语义检查，返回语义分析结果？
        return errorFlag_;
    }

    // tool for semantic check
    bool CompilerVistor::numeric(int typeIndex)
    {
        auto type = symbolTable_->getTypeInfo(typeIndex);

        if( (type == TypeInfo::BYTE) || (type == TypeInfo::CHAR)
         || (type == TypeInfo::INT)  || (type == TypeInfo::SHORT)
         || (type == TypeInfo::LONG) || (type == TypeInfo::FLOAT)
         || (type == TypeInfo::DOUBLE) )
        {
            return true;
        }
        return false;
    }

    int CompilerVistor::maxType(int type1, int type2)
    {
        return type1 > type2 ? type1 : type2;
    }

    void CompilerVistor::errorReport(const std::string &msg, const TokenLocation &loc, ErrorType errorType)
    {
        ExceptionHandler::getInstance()->add(msg, loc, errorType);
        errorFlag_ = true;
    }
    
    
    void CompilerVistor::visit(VecNodePtr ast)
    {
        for(auto node : ast)
        {
            node->accept(this);
        }
        cout << endl;
    }

    void CompilerVistor::visit(ASTNode *node)
    {
        cout << "you should not visit here in ASTNode" << endl;
    }

    void CompilerVistor::visit(Stmt *node)
    {
        cout << "you should not visit here in Stmt" << endl;
    }

    void CompilerVistor::visit(EmptyStmt *node)
    {
        
    }

    void CompilerVistor::visit(ClassStmt *node)
    {
        
        symbolTable_->enterClass(node->name);
        node->body->accept(this);
        symbolTable_->leaveClass();
        
    }

    void CompilerVistor::visit(MethodDeclStmt *node)
    {
        
        // TODO: parameter
        symbolTable_->enter(node->name);
        node->body->accept(this);
        symbolTable_->leave();
        
    }

    void CompilerVistor::visit(PrimaryStmt *node)    // 定义变量 
    {
        
        auto typeIndex = symbolTable_->getTypeIndex(node->type);
        info = new SymbolInfo(typeIndex, node->flags);
        for(auto v : node->decls)
        {
            v->accept(this);
        }
        
    }

    void CompilerVistor::visit(BlockStmt *node)
    {
        
        symbolTable_->enter();
        for(auto stmt : node->statements)
        {
            stmt->accept(this);
        }
        symbolTable_->leave();
        
    }

    void CompilerVistor::visit(IfStmt *node)
    {
        
        node->condition->accept(this);
        node->thenBody->accept(this);
        if(node->elseBody)
        {
            node->elseBody->accept(this);
        }
        
    }

    void CompilerVistor::visit(ForStmt *node)
    {
        
        node->init->accept(this);
        node->condition->accept(this);
        node->update->accept(this);
        node->body->accept(this);
        
    }

    void CompilerVistor::visit(WhileStmt *node)
    {
        node->condition->accept(this);
        node->body->accept(this);
    }

    void CompilerVistor::visit(DoStmt *node)
    {
        
        node->body->accept(this);
        node->condition->accept(this);
        
    }

    void CompilerVistor::visit(SwitchStmt *node)
    {
        
        node->flag->accept(this);
        for(auto c : node->cases)
        {
            c->accept(this);
        }
        if(!node->defaultBody.empty())
        {
            for(auto v : node->defaultBody)
            {
                v->accept(this);
            }
        }
        
    }

    void CompilerVistor::visit(CaseStmt *node)
    {
        
        node->label->accept(this);
        for(auto v : node->statements)
        {
            v->accept(this);
        }
    }

    void CompilerVistor::visit(ReturnStmt *node)
    {
        
        if(node->returnValue)
        {
            node->returnValue->accept(this);
        }
        
    }

    void CompilerVistor::visit(BreakStmt *node)
    {
        
        
    }

    void CompilerVistor::visit(ContinueStmt *node)
    {
        
    }

    void CompilerVistor::visit(Expr *node)
    {
        cout << "you should not visit here in Expr" << endl;
    }

    void CompilerVistor::visit(VariableDeclExpr *node)    
    {
        
        if(symbolTable_->hasVariable(node->name,false))
		{
        	std::string error_msg = "Duplicate local variable "+node->name;   //变量已经定义 
			errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
		}
		else
		{
			symbolTable_->add(node->name, *info);
		}
    	
        if(node->initValue)
        {
            node->initValue->accept(this);
        }
        else
		{
        	//TODO
        //	symbolTable_->getVariableInfo(node->name).setAttribute(SymbolTag::UNDEFINE);
		}
        
    }

    void CompilerVistor::visit(IdentifierExpr *node)
    {
        
        if(symbolTable_->hasVariable(node->name))
		{
        	auto node_type = symbolTable_->getVariableInfo(node->name);
        	variableFlag_ = true;
        //	if(symbolTable_->getVariableInfo(node->name).check(SymbolTag::UNDEFINE))
		//{
        //		node->setType(symbolTable_->getTypeIndex("VOID"));
		//	}
		//	else
		//{
				node->setType(node_type.getType());  //设置类型 
		//	}
		}
		else
		{
			std::string error_msg = node->name+" cannot be resolved to a variable";   //变量没有定义 
			errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
		}
        
    }

    void CompilerVistor::visit(NewExpr *node)
    {
        
        node->constructor->accept(this);
        
    }

    void CompilerVistor::visit(IndexExpr *node)
    {
        
        node->left->accept(this);
        node->index->accept(this);
        
    }

    void CompilerVistor::visit(CallExpr *node)
    {
        
        for(auto v : node->arguments)
        {
            v->accept(this);
        }
        
    }

    void CompilerVistor::visit(QualifiedIdExpr *node)     //  .
    {
        
        node->left->accept(this);
        
        node->right->accept(this);
        auto rt = node->right->getType();
        node->setType(rt);
    }

    void CompilerVistor::visit(IntExpr *node)
    {
        
        node->setType(symbolTable_->getTypeIndex("int"));
        
    }

    void CompilerVistor::visit(RealExpr *node)
    {
        
        node->setType(symbolTable_->getTypeIndex("double"));
        
    }

    void CompilerVistor::visit(BoolExpr *node)
    {
        
        node->setType(symbolTable_->getTypeIndex("boolean"));
        
        
    }

    void CompilerVistor::visit(NullExpr *node)
    {
        
        //TODO:
        
        
    }

    void CompilerVistor::visit(StrExpr *node)
    {
        
        
        node->setType(symbolTable_->getTypeIndex("String"));
        
        
    }

    void CompilerVistor::visit(ArrayExpr *node)
    {
        
        for(auto elem : node->elems)
        {
            elem->accept(this);
        }
        
    }

    void CompilerVistor::visit(UnaryOpExpr *node)
    {
        
        //TODO:
        
        node->expr->accept(this);
        
        auto et = node->expr->getType();
        if(!numeric(et))
		{
        	std::string error_msg = "Type mismatch: cannot convert from "+symbolTable_->getTypeName(et)+" to int";  
			errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
		}
        
    }

    void CompilerVistor::visit(BinaryOpExpr *node)
    {
        
        std::string op_name = tokenDesc(node->op);
      	if(isAssignmentOperator(node->op))
		  {         //赋值操作符 
        	node->right->accept(this);
        	auto rt = node->right->getType();      // 右子树类型 
        	
        	variableFlag_ = false;
        	node->left->accept(this);
        	auto lt = node->left->getType();       //左子树类型 
        	if(!variableFlag_)
			{
        		std::string error_msg = "The left-hand side of an assignment must be a variable"; 
        		errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
			}
			else
			{
				if(maxType(lt,rt) == lt)
				{
		        	node->setType(maxType(lt,rt));
				}
				else
				{
					std::string error_msg = "Type mismatch: cannot convert from "+symbolTable_->getTypeName(rt)+" to "+symbolTable_->getTypeName(lt);  
					errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
				}
			}
		}
		else
		{
			node->left->accept(this);
			//TODO:
        	auto lt = node->left->getType();     //左子树类型 
        	node->right->accept(this);
        	auto rt = node->right->getType();      // 右子树类型 
        	if(numeric(lt)&&numeric(rt))
        	{
        		node->setType(maxType(lt,rt));
			}
			else
			{
				std::string error_msg = "Type "+symbolTable_->getTypeName(lt)+" and "+symbolTable_->getTypeName(rt)+" cannot be arithmetic";
				errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
			}
		}
    }

    void CompilerVistor::visit(TernaryOpExpr *node)
    {
        
        node->condition->accept(this);
        node->thenValue->accept(this);
        
        int thenType =node->thenValue->getType();
        
        node->elseValue->accept(this);
        
        int elseType = node->elseValue->getType();
		
		if(maxType(thenType,elseType)!=thenType&&maxType(thenType,elseType)!=elseType)
		{
			std::string error_msg = "Type mismatch: cannot convert from "+symbolTable_->getTypeName(thenType)+" to "+symbolTable_->getTypeName(elseType);  
			errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
		}
		else
		{
			node->setType(thenType);
		}
    }

}


