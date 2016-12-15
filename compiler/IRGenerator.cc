#include <iostream>
#include "../common/symbols.h"
#include "IRGenerator.h"

using std::cout;
using std::endl;

namespace ycc
{
    IRGenerator::IRGenerator()
        : labelCount_(1),oneVistor_(true),fuzhi_(false),constant_(false),incre_(false)
    {
        symbolTable_ = SymbolTable::getInstance();
    }

	void IRGenerator::gene(VecNodePtr ast){
		
	}
	
	std::string IRGenerator::numeric(int typeIndex)
    {
        auto type = symbolTable_->getTypeInfo(typeIndex);

        if(type == TypeInfo::BYTE)
		{
        	return "i32";
		}
		else if(type == TypeInfo::CHAR)
		{
			return "i8*";
		}
		else if(type == TypeInfo::INT)
		{
			return "i32";
		}
		else if(type == TypeInfo::SHORT)
		{
			return "i32";
		}
		else if(type == TypeInfo::LONG)
		{
			return "i32";
		}
		else if(type == TypeInfo::FLOAT)
		{
			return "double";
		}
		else if(type == TypeInfo::DOUBLE)
		{
			return "double";
		}
		
		else if(type == TypeInfo::BOOLEAN)
		{
			return "i8";
		}
		else
		{
			return "error";
		}
    }
    
    std::string IRGenerator::change_Op(std::string op){
    	if(op == "plus")
		{
			return "add nsw";
		}
		else if(op == "minus") 
		{
			return "sub nsw";
		}
		else if(op == "multiply")
		{
			return "mul nsw";
		}
		else if(op == "divide")
		{
			return "sdiv";
		}
		else if(op == "equal")    //==
		{
			return "icmp seq";
		}
		else if(op == "less than")     //<
		{
			return "icmp slt";
		}
		else if(op == "less or equal")   //<=
		{
			return "icmp sle";
		}
		else if(op == "greater than")   //>
		{
			return "icmp sgt";
		}
		else if(op == "greater or equal")    //>=
		{
			return "icmp sge";
		}
		else if(op == "not equal")   //!=
		{
			return "icmp sne";
		}
		else
		{
			return "error";	
		}
	}

	// generator tools
    void IRGenerator::writeMethod_begin(int type, std::string name)
	{  
    	cout<<"Function Attrs: nounwind uwtable"<<endl;
		cout<<"define "+symbolTable_->getTypeName(type)+" @"+name+"() #0 {"<<endl;
	}
	void IRGenerator::writeMethod_end(int type)
	{  
		cout<<"\tret "+symbolTable_->getTypeName(type)<<endl;
		cout<<"}"<<endl;
	}
	
    void IRGenerator::writeAlloca(std::string Name,int Type)
	{
    	cout<<"\t%"<<Name<<" = alloca "<<numeric(Type)<<", align 4"<<endl;
	}
	
    void IRGenerator::writeStore(int Type,std::string oneName,std::string twoName)
	{
    	cout<<"\tstore "<<numeric(Type)<<" %"<<oneName<<", "<<numeric(Type)<<"* %"+twoName+", align 4"<<endl;
	}
	
	void IRGenerator::writeLoad(int labelCount,int Type,std::string Name)
	{
		cout<<"\t%"<<labelCount<<" = load "<<numeric(Type)<<", "<<numeric(Type)<<"* %"<<Name<<", align 4"<<endl;
	}
	
    void IRGenerator::writeLabel(int count)
	{
		cout<<""<<endl;
    	cout<<"; <label>:"<<count<<endl;
	}
	
    void IRGenerator::writeJump(int Label)
	{
		cout<<"\tbr label %"<<Label<<endl;
    	//TODO: writeJump
	}
    void IRGenerator::writeCJump(int C,int thenLabel,int elseLabel)
	{
    	cout<<"\tbr i1 %"<<C<<", label %"<<thenLabel<<", label %"<<elseLabel<<endl;
	}
    
    void IRGenerator::visit(VecNodePtr ast)
    {
        for(auto node : ast)
        {
            node->accept(this);
        }
        cout << endl;
    }

    void IRGenerator::visit(ASTNode *node)
    {
        cout << "you should not visit here in ASTNode" << endl;
    }

    void IRGenerator::visit(Stmt *node)
    {
        cout << "you should not visit here in Stmt" << endl;
    }

    void IRGenerator::visit(EmptyStmt *node)
    {
        
    }

    void IRGenerator::visit(ClassStmt *node)
    {
        
        symbolTable_->enterClass(node->name);
        node->body->accept(this);
        symbolTable_->leaveClass();
        
    }

    void IRGenerator::visit(MethodDeclStmt *node)
    {
        
        // TODO: parameter
      //  symbolTable_->enter(node->name);
        
        writeMethod_begin(symbolTable_->getTypeIndex("void"),node->name);   // TODO: 方法的Type 
        labelCount_ = 1;
        
        cout<<"\t%1 = alloca i32, align 4"<<endl;
        labelCount_++;
        node->body->accept(this);    //先遍历一次方法里面的变量 
        //TODO:输出定义的变量 
        oneVistor_ = false;
        
        cout<<"\tstore i32 0, i32* %1, align 4"<<endl;
        
        node->body->accept(this);    //第二次遍历 
        
        oneVistor_ = true;
        writeMethod_end(symbolTable_->getTypeIndex("void"));     // TODO: 方法的Type 
        
     //   symbolTable_->leave();
    }

    void IRGenerator::visit(PrimaryStmt *node)    // 定义变量 
    {
        auto typeIndex = symbolTable_->getTypeIndex(node->type);
        	
	    info = new SymbolInfo(typeIndex, node->flags);
	    for(auto v : node->decls)
	    {
	        v->accept(this);
	    }
        
    }

    void IRGenerator::visit(BlockStmt *node)
    {
        
    //    symbolTable_->enter();
        for(auto stmt : node->statements)  
        {
            stmt->accept(this);
        }
        
     //   symbolTable_->leave();
        
    }

    void IRGenerator::visit(IfStmt *node)
    {
        if(!oneVistor_)
		{
        	node->condition->accept(this);
	        int  thenLabel ;
	        int  elseLabel ;
	        int  endLabel  ;
	        if(node->elseBody)
			{
	        	thenLabel = labelCount_;
	        	elseLabel = labelCount_+1;
	        	endLabel = labelCount_+2;
	        	writeCJump(labelCount_-1,thenLabel,elseLabel);  //CJump;
	        	labelCount_ = labelCount_+3;
			}
	        else
			{
	        	thenLabel = labelCount_;
	        	endLabel = labelCount_+1;
	        	writeCJump(labelCount_-1,thenLabel,endLabel);  //CJump;
	        	labelCount_ = labelCount_+2;
			}
	        
	        writeLabel(thenLabel);
	        node->thenBody->accept(this);
	        writeJump(endLabel);
	        if(node->elseBody)
	        {
	        	writeLabel(elseLabel);
	            node->elseBody->accept(this);
	            writeJump(endLabel);
	        }
	        writeLabel(endLabel);
		}
        
    }

    void IRGenerator::visit(ForStmt *node)
    {
    	if(!oneVistor_)
		{
	    	int conditionLabel =labelCount_;
	    	int bodyLabel = labelCount_+1;
	        int updateLabel = labelCount_+2;
	        int endLabel = labelCount_+3;
	        labelCount_ = labelCount_ +4;
	        
	        node->init->accept(this);
	        writeJump(conditionLabel);
	        
	        writeLabel(conditionLabel);
	        node->condition->accept(this);
	        
	        writeCJump(labelCount_-1,bodyLabel,endLabel); 
	        
	        writeLabel(bodyLabel);
	        node->body->accept(this);
	        
	        writeJump(updateLabel);
	        
	        writeLabel(updateLabel);
	        node->update->accept(this);
	        writeJump(conditionLabel);
	        writeLabel(endLabel);
    	}    
    }

    void IRGenerator::visit(WhileStmt *node)
    {
    	if(!oneVistor_)
    	{
    		int conditionLabel = labelCount_;
	    	int bodyLabel = labelCount_+1;
	    	int endLabel = labelCount_+2;
	    	labelCount_= labelCount_+3;
	    	writeJump(conditionLabel);
	    	
	    	writeLabel(conditionLabel);
	        node->condition->accept(this);
	        writeCJump(labelCount_-1,bodyLabel,endLabel);  //CJump;
	        
	        writeLabel(bodyLabel);
	        node->body->accept(this);
	        
	        writeLabel(endLabel);
		}
    	
    }

    void IRGenerator::visit(DoStmt *node)
    {
        
        node->body->accept(this);
        node->condition->accept(this);
        
    }

    void IRGenerator::visit(SwitchStmt *node)
    {
       /* if(!oneVistor_){
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
        */
        
    }

    void IRGenerator::visit(CaseStmt *node)
    {
        
        node->label->accept(this);
        for(auto v : node->statements)
        {
            v->accept(this);
        }
    }

    void IRGenerator::visit(ReturnStmt *node)
    {
        
        if(node->returnValue)
        {
            node->returnValue->accept(this);
        }
        
    }

    void IRGenerator::visit(BreakStmt *node)
    {
        
        
    }

    void IRGenerator::visit(ContinueStmt *node)
    {
        
    }

    void IRGenerator::visit(Expr *node)
    {
        cout << "you should not visit here in Expr" << endl;
    }

    void IRGenerator::visit(VariableDeclExpr *node)    
    {
        if(oneVistor_){
        	if(symbolTable_->hasVariable(node->name,false))
			{
	        	
			}
			else
			{
				symbolTable_->add(node->name, *info);
			}
	    	writeAlloca(node->name,info->getType());   //alloca
		}
		else
		{
			if(node->initValue)
	        {
	        	constant_ = false ;
	            node->initValue->accept(this);
	            if(constant_)
				{
	            	cout<<"\tstore "<<numeric(info->getType())<<" "<<constant_value<<", "<<numeric(info->getType())<<"* %"+node->name+", align 4"<<endl;
	            	constant_ = true;
				}
				else
				{
					auto initCount = labelCount_-1;
	            	writeStore(info->getType(),std::to_string(initCount),node->name);   //store
				}
	            
	        }
		}
    }

    void IRGenerator::visit(IdentifierExpr *node)
    {
        if(symbolTable_->hasVariable(node->name))
		{
        	auto node_type = symbolTable_->getVariableInfo(node->name);
			node->setType(node_type.getType());  //设置类型 
		}
		if(!oneVistor_){
			if(fuzhi_)
			{
				fuzhi_name = node->name;
			}
			else if(incre_)
			{
				writeLoad(labelCount_,node->getType(),node->name);   //Load
				labelCount_++;
				cout<<"\t%"<<labelCount_<<" = add nsw "<<numeric(node->getType())<<" %"<<labelCount_-1<<", 1"<<endl;
				labelCount_++;
				writeStore(node->getType(),std::to_string(labelCount_-1),node->name);
				
			}
			else
			{
				writeLoad(labelCount_,node->getType(),node->name);    //load
				labelCount_++;
			}
		}
		
    }

    void IRGenerator::visit(NewExpr *node)
    {
        
        node->constructor->accept(this);
        
    }

    void IRGenerator::visit(IndexExpr *node)
    {
        
        node->left->accept(this);
        node->index->accept(this);
        
    }

    void IRGenerator::visit(CallExpr *node)
    {
        
        for(auto v : node->arguments)
        {
            v->accept(this);
        }
        
    }

    void IRGenerator::visit(QualifiedIdExpr *node)     //  .
    {
        
        node->left->accept(this);
        
        node->right->accept(this);
        auto rt = node->right->getType();
        node->setType(rt);
    }

    void IRGenerator::visit(IntExpr *node)
    {
        if(!oneVistor_){
        	constant_ = true;
        	constant_value = node->lexeme;
		}
        
    }

    void IRGenerator::visit(RealExpr *node)
    {
        node->setType(symbolTable_->getTypeIndex("double"));
    }

    void IRGenerator::visit(BoolExpr *node)
    {
        node->setType(symbolTable_->getTypeIndex("boolean"));
        if(!oneVistor_)
		{
        	if(node->value)
			{
        		constant_ = true;
        		constant_value = "1";
			}
			else
			{
				constant_ = true;
        		constant_value = "0";
			}
		}
    }

    void IRGenerator::visit(NullExpr *node)
    {
        
        //TODO:
    }

    void IRGenerator::visit(StrExpr *node)
    {
        
        
        node->setType(symbolTable_->getTypeIndex("String"));
        
        
    }

    void IRGenerator::visit(ArrayExpr *node)
    {
        
        for(auto elem : node->elems)
        {
            elem->accept(this);
        }
        
    }

    void IRGenerator::visit(UnaryOpExpr *node)
    {
        
        auto et = node->expr->getType();
        incre_ = false;
        if(!oneVistor_)
		{
        	if(tokenDesc(node->op) == "incre"){
        		incre_ = true;
			}
		}
        node->expr->accept(this);
        incre_ = false;
    }

    void IRGenerator::visit(BinaryOpExpr *node)
    {
        if(!oneVistor_){
        	std::string op_name = tokenDesc(node->op);
	      	if(isAssignmentOperator(node->op))
			{         //赋值操作符 
				fuzhi_ = false;
				constant_ = false;
	        	node->right->accept(this);
	        	auto rCount = labelCount_-1;
	        	auto rt = node->right->getType();      // 右子树类型 
	        	fuzhi_ = true;
	        	node->left->accept(this);
	        	auto lt = node->left->getType();       //左子树类型 
	        	
	        	node->setType(lt);
	        	if(constant_){
	        		cout<<"\tstore "<<numeric(lt)<<" "<<constant_value<<", "<<numeric(lt)<<"* %"+fuzhi_name+", align 4"<<endl;
				}
				else{
					writeStore(lt,std::to_string(rCount),fuzhi_name);          //store
				}
	        
	        	fuzhi_ = false;
	        	constant_ = false;
			}
			else
			{
				constant_=false;
				std::string lcon,rcon;
				
				node->left->accept(this);
				
				if(constant_){
					lcon = constant_value;
					constant_=false;
				}
				auto lCount = labelCount_-1; 
	        	auto lt = node->left->getType();     //左子树类型 
	        	
	        	node->right->accept(this);
	        	
	        	if(constant_){
					rcon = constant_value;
					constant_=false;
				}
	        	auto rCount = labelCount_-1;
	        	auto rt = node->right->getType();      // 右子树类型  
	        	node->setType(lt);
	        	if(lcon.empty()&& rcon.empty()){    //运算2边都不是常数 
	        		cout<<"\t%"<<labelCount_<<" = "<<change_Op(tokenDesc(node->op))<<" "<<numeric(lt)<<" %"<<lCount<<", %"<<rCount<<endl;
				}
				else if(lcon.empty()&& !rcon.empty())      // 右边是常数 
	        	{
	        		cout<<"\t%"<<labelCount_<<" = "<<change_Op(tokenDesc(node->op))<<" "<<numeric(lt)<<" %"<<lCount<<", "<<rcon<<endl;
				}
				else if(!lcon.empty()&& rcon.empty())          // 左边是常数 
				{
					cout<<"\t%"<<labelCount_<<" = "<<change_Op(tokenDesc(node->op))<<" "<<numeric(lt)<<" "<<lcon<<", %"<<rCount<<endl;
				}
				else          //都是常数 
				{
					cout<<"\t%"<<labelCount_<<" = "<<change_Op(tokenDesc(node->op))<<" "<<numeric(lt)<<" "<<lcon<<", "<<rcon<<endl;
				}
				labelCount_++;
			}
		} 
    }

    void IRGenerator::visit(TernaryOpExpr *node)
    {
        
        
        node->condition->accept(this);
        
        node->thenValue->accept(this);
        
        int thenType =node->thenValue->getType();
        
        node->elseValue->accept(this);
        
        int elseType = node->elseValue->getType();
		
		node->setType(thenType);
    }
}


