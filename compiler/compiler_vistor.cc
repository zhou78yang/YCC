#include <iostream>
#include "compiler_vistor.h"
#include "../common/symbols.h"

using std::cout;
using std::endl;

namespace ycc
{

    CompilerVistor::CompilerVistor()
        : errorFlag_(false),variableFlag_(false),initVariable_(true),call_value(false)
    {
        symbolTable_ = SymbolTable::getInstance();
    }

    bool CompilerVistor::check(VecNodePtr ast)
    {
        for(auto node : ast)
        {
            node->accept(this);
        }
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



    // visiting part
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
        // do nothing
    }

    void CompilerVistor::visit(ClassStmt *node)
    {
        symbolTable_->enterClass(node->name);
        node->body->accept(this);
        symbolTable_->leaveClass();
    }

    void CompilerVistor::visit(MethodDeclStmt *node)
    {
        symbolTable_->enter(node->name);
        node->body->accept(this);
        symbolTable_->leave();
    }

    void CompilerVistor::visit(PrimaryStmt *node)    // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    {
        auto typeIndex = symbolTable_->getTypeIndex(node->type);
        info = new SymbolInfo(typeIndex, node->flags);  // info for decls
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
        // do nothing
    }

    void CompilerVistor::visit(ContinueStmt *node)
    {
        // do nothing
    }

    void CompilerVistor::visit(Expr *node)
    {
        cout << "you should not visit here in Expr" << endl;
    }

    void CompilerVistor::visit(VariableDeclExpr *node)
    {
        if(symbolTable_->hasVariable(node->name, false))
		{
        	std::string error_msg = "Duplicate local variable " + node->name;   //ï¿½ï¿½ï¿½ï¿½ï¿½Ñ¾ï¿½ï¿½ï¿½ï¿½ï¿½
			errorReport(error_msg, node->getLocation(), ErrorType::ERROR);
		}
		else
		{
			if(!node->initValue)
			{
				info->setAttribute(SymbolTag::UNDEFINED);
			}
			symbolTable_->add(node->name, *info);
		}

        if(node->initValue)
        {
            // for identifier check
        	initVariable_ = true;
            node->initValue->accept(this);
            initVariable_ = false;
        }

    }

    void CompilerVistor::visit(IdentifierExpr *node)
    {

        if(symbolTable_->hasVariable(node->name))
		{
        	auto nodeInfo = symbolTable_->getVariableInfo(node->name);
        	variableFlag_ = true;
        	if(nodeInfo.check(SymbolTag::UNDEFINED) && initVariable_)
			{
        		node->setType(symbolTable_->getTypeIndex("void"));
			}
			else
			{
				node->setType(nodeInfo.getType());  //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			}

			variableName_ = node->name;
		}
		else
		{
            // can not find this identifier
			std::string error_msg = node->name + " cannot be resolved to a variable";   //ï¿½ï¿½ï¿½ï¿½Ã»ï¿½Ð¶ï¿½ï¿½ï¿½
			errorReport(error_msg, node->getLocation(), ErrorType::ERROR);
		}
    }

    void CompilerVistor::visit(NewExpr *node)
    {
        // TODO: check constructor
        node->constructor->accept(this);
    }

    void CompilerVistor::visit(IndexExpr *node)
    {
        // TODO: check index can be an integer
        node->left->accept(this);
        node->index->accept(this);
    }

    void CompilerVistor::visit(CallExpr *node)
    {
    	call_value = true;
    	call_name = node->callee;
        call_index = 0;

        for(auto v : node->arguments)
        {
            v->accept(this);
            call_index++;
        }
        MethodInfo mInfo = symbolTable_->getMethodInfo(node->callee);
        node->setType(mInfo.getType());

        call_value = false;
    }

    void CompilerVistor::visit(QualifiedIdExpr *node)     //  .
    {
        // TODO: check left is the qualifier of right
        node->left->accept(this);
        node->right->accept(this);
        auto rt = node->right->getType();
        node->setType(rt);
    }

    void CompilerVistor::visit(IntExpr *node)
    {
        node->setType(symbolTable_->getTypeIndex("int"));
        if(call_value)
        {
        	MethodInfo mInfo = symbolTable_->getMethodInfo(call_name);
        	if(call_index<mInfo.parameters_.size()){
        		if(mInfo.paramTypes_[call_index]!=node->getType()){
        			std::string error_msg = "Type mismatch: cannot convert from "+symbolTable_->getTypeName(node->getType())+" to "+symbolTable_->getTypeName(mInfo.paramTypes_[call_index]);
					errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
				}
			}
		}
    }

    void CompilerVistor::visit(RealExpr *node)
    {

        node->setType(symbolTable_->getTypeIndex("double"));
        if(call_value)
        {
        	MethodInfo mInfo = symbolTable_->getMethodInfo(call_name);
        	if(call_index<mInfo.parameters_.size()){
        		if(mInfo.paramTypes_[call_index]!=node->getType()){
        			std::string error_msg = "Type mismatch: cannot convert from "+symbolTable_->getTypeName(node->getType())+" to "+symbolTable_->getTypeName(mInfo.paramTypes_[call_index]);
					errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
				}
			}
		}
    }

    void CompilerVistor::visit(BoolExpr *node)
    {
        node->setType(symbolTable_->getTypeIndex("boolean"));
    }

    void CompilerVistor::visit(NullExpr *node)
    {
        node->setType(symbolTable_->getTypeIndex("void"));
    }

    void CompilerVistor::visit(StrExpr *node)
    {
        node->value = symbolTable_->addLiteral(node->value);
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
      	if(isAssignmentOperator(node->op))
		{         //ï¿½ï¿½Öµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        	node->right->accept(this);
        	initVariable_ = true;
        	auto rt = node->right->getType();      // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        	initVariable_ = false;

        	variableFlag_ = false;
        	node->left->accept(this);

        	auto lt = node->left->getType();       //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        	if(!variableFlag_)
			{
        		std::string error_msg = "The left-hand side of an assignment must be a variable";
        		errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
			}
			else
			{
				if(numeric(rt))
				{
					if(maxType(lt,rt) == lt)
					{
			        	node->setType(maxType(lt,rt));
			        	SymbolInfo sInfo = symbolTable_->getVariableInfo(variableName_);     //ï¿½ï¿½ÖµÖ®ï¿½ï¿½ï¿½Ñ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Îªï¿½Ñ¾ï¿½ï¿½ï¿½Öµ
			        	sInfo.setAttribute(SymbolTag::UNDEFINED,0);
                        symbolTable_->setVariableInfo(variableName_, sInfo);
					}
					else
					{
						std::string error_msg = "Type mismatch: cannot convert from "+symbolTable_->getTypeName(rt)+" to "+symbolTable_->getTypeName(lt);
						errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
					}
				}
				else
				{
					std::string error_msg = "Type mismatch: cannot convert from "+symbolTable_->getTypeName(rt)+" to "+symbolTable_->getTypeName(lt);
					errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
					node->setType(rt);
				}
			}
		}
		else if(isCompareOperator(node->op))    //CompareOp ¼Æ½ÏÔËËã·û 
		{
			node->left->accept(this);
			//TODO:
        	auto lt = node->left->getType();     //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        	node->right->accept(this);
        	auto rt = node->right->getType();      // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        	if(numeric(lt)&&numeric(rt))
        	{
        		node->setType(symbolTable_->getTypeIndex("boolean"));   //Type -> bool
			}
			else
			{
				std::string error_msg = "Incompatible operand types "+symbolTable_->getTypeName(lt)+" and "+symbolTable_->getTypeName(rt);
				errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
			}
		} 
		else if(isLogicOperator(node->op))    //LogicOp   Âß¼­ÔËËã 
		{
			node->left->accept(this);
			//TODO:
        	auto lt = node->left->getType();     //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        	node->right->accept(this);
        	auto rt = node->right->getType();      // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        	auto type = symbolTable_->getTypeInfo(lt);
        	
        	if((lt == rt)&&(type == TypeInfo::BOOLEAN))
        	{
        		node->setType(symbolTable_->getTypeIndex("boolean"));   //Type -> bool
			}
			else
			{
				std::string error_msg = "The operator "+tokenDesc(node->op)+" is undefined for the argument type(s) "+symbolTable_->getTypeName(lt)+", "+symbolTable_->getTypeName(rt);
				errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
			}
		}
		else
		{
            // TODO: logic or arithmetic
			node->left->accept(this);
			//TODO:
        	auto lt = node->left->getType();     //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        	node->right->accept(this);
        	auto rt = node->right->getType();      // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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
        // TODO: check condition is bool
        node->condition->accept(this);
        node->thenValue->accept(this);

        int thenType =node->thenValue->getType();
        node->elseValue->accept(this);
        int elseType = node->elseValue->getType();

		//if(maxType(thenType,elseType)!=thenType&&maxType(thenType,elseType)!=elseType)
        if(thenType != elseType)
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
