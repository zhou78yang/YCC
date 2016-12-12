#include <iostream>
#include "compiler_vistor.h"
#include "../common/symbols.h"

using std::cout;
using std::endl;

namespace ycc
{

    CompilerVistor::CompilerVistor()
        : errorFlag_(false),level(0)
    {
        symbolTable_ = SymbolTable::getInstance();
    }

    void CompilerVistor::upgrade()
    {
        level++;
    }

    void CompilerVistor::degrade()
    {
        level--;
    }

    bool CompilerVistor::check(VecNodePtr ast)
    {
        // TODO: �����ӿڣ�����һ���﷨�������������飬������������������
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
        upgrade();
        degrade();
    }

    void CompilerVistor::visit(ClassStmt *node)
    {
        upgrade();
        symbolTable_->enterClass(node->name);
        node->body->accept(this);
        symbolTable_->leaveClass();
        degrade();
    }

    void CompilerVistor::visit(MethodDeclStmt *node)
    {
        upgrade();
        symbolTable_->enter(node->name);
        node->body->accept(this);
        symbolTable_->leave();
        degrade();
    }

    void CompilerVistor::visit(PrimaryStmt *node)    // ��������
    {
        upgrade();
        auto typeIndex = symbolTable_->getTypeIndex(node->type);
        info = new SymbolInfo(typeIndex, node->flags);
        for(auto v : node->decls)
        {
            v->accept(this);
        }
        degrade();
    }

    void CompilerVistor::visit(BlockStmt *node)
    {
        upgrade();
        symbolTable_->enter();
        for(auto stmt : node->statements)
        {
            stmt->accept(this);
        }
        symbolTable_->leave();
        degrade();
    }

    void CompilerVistor::visit(IfStmt *node)
    {
        upgrade();
        node->condition->accept(this);
        node->thenBody->accept(this);
        if(node->elseBody)
        {
            node->elseBody->accept(this);
        }
        degrade();
    }

    void CompilerVistor::visit(ForStmt *node)
    {
        upgrade();
        node->init->accept(this);
        node->condition->accept(this);
        node->update->accept(this);
        node->body->accept(this);
        degrade();
    }

    void CompilerVistor::visit(WhileStmt *node)
    {
        upgrade();
        node->condition->accept(this);
        node->body->accept(this);
        degrade();
    }

    void CompilerVistor::visit(DoStmt *node)
    {
        upgrade();
        node->body->accept(this);
        node->condition->accept(this);
        degrade();
    }

    void CompilerVistor::visit(SwitchStmt *node)
    {
        upgrade();
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
        degrade();
    }

    void CompilerVistor::visit(CaseStmt *node)
    {
        upgrade();
        node->label->accept(this);
        for(auto v : node->statements)
        {
            v->accept(this);
        }
        degrade();
    }

    void CompilerVistor::visit(ReturnStmt *node)
    {
        upgrade();
        if(node->returnValue)
        {
            node->returnValue->accept(this);
        }
        degrade();
    }

    void CompilerVistor::visit(BreakStmt *node)
    {
        upgrade();
        degrade();
    }

    void CompilerVistor::visit(ContinueStmt *node)
    {
        upgrade();
        degrade();
    }

    void CompilerVistor::visit(Expr *node)
    {
        cout << "you should not visit here in Expr" << endl;
    }

    void CompilerVistor::visit(VariableDeclExpr *node)
    {
        upgrade();
        if(symbolTable_->hasVariable(node->name, false)){
        	std::string error_msg = "Duplicate local variable "+node->name;   //�����Ѿ�����
			errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
		}
		else{
			symbolTable_->add(node->name, *info);
		}

        if(node->initValue)
        {
            node->initValue->accept(this);
        }
        else{
        	//TODO
        //	symbolTable_->getVariableInfo(node->name).setAttribute(SymbolTag::UNDEFINE);
		}
        degrade();
    }

    void CompilerVistor::visit(IdentifierExpr *node)
    {
        upgrade();
        if(symbolTable_->hasVariable(node->name)){
        	auto node_type = symbolTable_->getVariableInfo(node->name);
        //	if(symbolTable_->getVariableInfo(node->name).check(SymbolTag::UNDEFINE)){
        //		node->setType(symbolTable_->getTypeIndex("VOID"));
		//	}
		//	else{
				node->setType(node_type.getType());  //��������
		//	}
		}
		else{
			std::string error_msg = node->name+" cannot be resolved to a variable";   //����û�ж���
			errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
		}
        degrade();
    }

    void CompilerVistor::visit(NewExpr *node)
    {
        upgrade();
        node->constructor->accept(this);
        degrade();
    }

    void CompilerVistor::visit(IndexExpr *node)
    {
        upgrade();
        node->left->accept(this);
        node->index->accept(this);
        degrade();
    }

    void CompilerVistor::visit(CallExpr *node)
    {
        upgrade();
        for(auto v : node->arguments)
        {
            v->accept(this);
        }
        degrade();
    }

    void CompilerVistor::visit(QualifiedIdExpr *node)
    {
        upgrade();
        node->left->accept(this);
        node->right->accept(this);
        degrade();
    }

    void CompilerVistor::visit(IntExpr *node)
    {
        upgrade();
        node->setType(symbolTable_->getTypeIndex("int"));

        degrade();
    }

    void CompilerVistor::visit(RealExpr *node)
    {
        upgrade();
        node->setType(symbolTable_->getTypeIndex("double"));
        degrade();
    }

    void CompilerVistor::visit(BoolExpr *node)
    {
        upgrade();
        node->setType(symbolTable_->getTypeIndex("boolean"));

        degrade();
    }

    void CompilerVistor::visit(NullExpr *node)
    {
        upgrade();
        //TODO:

        degrade();
    }

    void CompilerVistor::visit(StrExpr *node)
    {
        upgrade();

        node->setType(symbolTable_->getTypeIndex("String"));

        degrade();
    }

    void CompilerVistor::visit(ArrayExpr *node)
    {
        upgrade();
        for(auto elem : node->elems)
        {
            elem->accept(this);
        }
        degrade();
    }

    void CompilerVistor::visit(UnaryOpExpr *node)
    {
        upgrade();
        //TODO:

        node->expr->accept(this);

        degrade();
    }

    void CompilerVistor::visit(BinaryOpExpr *node)
    {
        upgrade();
        std::string op_name = tokenDesc(node->op);
      	if(isAssignmentOperator(node->op)){         //��ֵ������

        	node->right->accept(this);
        	auto rt = node->right->getType();      // ����������
        	node->left->accept(this);
        	auto lt = node->left->getType();       //����������
	        if(maxType(lt,rt) == lt){
	        	node->setType(maxType(lt,rt));
			}
			else{
				std::string error_msg = "Type mismatch: cannot convert from "+symbolTable_->getTypeName(rt)+" to "+symbolTable_->getTypeName(lt);
				errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
			}

		}
		else
		{
			node->left->accept(this);
			//TODO:
        	auto lt = node->left->getType();     //����������
        	node->right->accept(this);
        	auto rt = node->right->getType();      // ����������
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
        degrade();
    }

    void CompilerVistor::visit(TernaryOpExpr *node)
    {
        node->condition->accept(this);
        node->thenValue->accept(this);

        int thenType =node->thenValue->getType();

        node->elseValue->accept(this);

        int elseType = node->elseValue->getType();

		if(maxType(thenType,elseType)!=thenType&&maxType(thenType,elseType)!=elseType){
			std::string error_msg = "Type mismatch: cannot convert from "+symbolTable_->getTypeName(thenType)+" to "+symbolTable_->getTypeName(elseType);
			errorReport(error_msg,node->getLocation(),ErrorType::ERROR);
		}
		else{
			node->setType(thenType);
		}
    }

}
