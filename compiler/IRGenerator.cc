#include <iostream>
#include "../common/symbols.h"
#include "IRGenerator.h"

using std::cout;
using std::endl;


namespace ycc
{
    IRGenerator::IRGenerator(const std::string &filename)
        : labelCount_(1),oneVistor_(true),fuzhi_(false),constant_(false),incre_(false),callRet_(false),call_value(false)
    {
    	output_.open(filename, std::ios::out);
        symbolTable_ = SymbolTable::getInstance();
        symbolTable_->dumpIR(output_);
    }

	void IRGenerator::gene(VecNodePtr ast)
	{
        for(auto node : ast)
        {
            node->accept(this);
        }
        output_ << endl;
		output_.close();
	}

	std::string IRGenerator::numeric(int typeIndex)
    {
    	return symbolTable_->getTypeIR(typeIndex);
    }



	// generator tools
    void IRGenerator::writeMethod_begin(std::string name,MethodInfo mInfo)
	{

        if(name != "main")
        {
            name = mInfo.getFullName();
        }

		output_<<endl;
    	//output_<<"Function Attrs: nounwind uwtable"<<endl;
		output_<<"define "<<numeric(mInfo.getType())<<" @"<<name<<"(";
        if(name != "main")
        {
    		for(int i = 0;i<mInfo.parameters_.size();i++){
    			output_<<" "<<numeric(mInfo.paramTypes_[i])<<" "<<"%"<<mInfo.parameters_[i];
    			if(i<mInfo.parameters_.size()-1){
    				output_<<",";
    			}
    		}
        }
	    output_<<") {"<<endl;
	}
	void IRGenerator::writeMethod_end(MethodInfo mInfo)
	{
		output_<<"\tret "<<numeric(mInfo.getType())<<endl;
		output_<<"}"<<endl;
	}

    void IRGenerator::writeAlloca(std::string name,int type)
	{
        int wd = symbolTable_->getTypeInfo(type).getWidth();
    	output_<<"\t%"<<name<<" = alloca "<<numeric(type)<<", align " << wd <<endl;
	}

    void IRGenerator::writeStore(int type,std::string oneName,std::string twoName)
	{
        auto info = symbolTable_->getVariableInfo(twoName);
        int wd = symbolTable_->getTypeInfo(type).getWidth();

    	output_<<"\tstore "<<numeric(type)<<" %"<<oneName<<", "<<numeric(type)<<"* ";
        if(info.check(SymbolTag::STATIC))
        {
            output_ << "@" << info.getFullName() << ", align " << wd << endl;
        }
        else
        {
            output_ << "%" << twoName << ", align " << wd << endl;
        }
	}

	void IRGenerator::writeLoad(int labelCount,int type,std::string name)
	{
        auto info = symbolTable_->getVariableInfo(name);
        int wd = symbolTable_->getTypeInfo(info.getType()).getWidth();
		output_<<"\t%"<<labelCount<<" = load "<< numeric(type) <<", "
               <<numeric(type)<<"* ";
        if(info.check(SymbolTag::STATIC))
        {
            output_ << "@" << info.getFullName() << ", align " << wd << endl;
        }
        else
        {
            output_ << "%" << name << ", align " << wd << endl;
        }
	}

    void IRGenerator::writeLabel(int count)
	{
		output_<<""<<endl;
    	output_<<"; <label>:"<<count<<endl;
	}

    void IRGenerator::writeJump(int Label)
	{
		output_<<"\tbr label %"<<Label<<endl;
    	//TODO: writeJump
	}
    void IRGenerator::writeCJump(int C,int thenLabel,int elseLabel)
	{
    	output_<<"\tbr i1 %"<<C<<", label %"<<thenLabel<<", label %"<<elseLabel<<endl;
	}

	void IRGenerator::writeCall(int labelCount,std::string name,MethodInfo mInfo)
	{
		if(labelCount == 0){
			output_<<"\tcall "<<numeric(mInfo.getType())<<" @"<<name<<"(";

		}
		else
		{
			output_<<"\t%"<<labelCount<<" = call "<<numeric(mInfo.getType())<<" @"<<name<<"(";

		}
	}


    void IRGenerator::visit(ASTNode *node)
    {
        output_ << "you should not visit here in ASTNode" << endl;
    }

    void IRGenerator::visit(Stmt *node)
    {
        output_ << "you should not visit here in Stmt" << endl;
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

        auto mInfo = symbolTable_->getMethodInfo(node->name);

        writeMethod_begin(node->name,mInfo);   // TODO: ������Type
        labelCount_ = 1;

        //output_<<"\t%1 = alloca i32, align 4"<<endl;
        //labelCount_++;
        node->body->accept(this);    //�ȱ���һ�η��������ı���
        //TODO:���������ı���
        oneVistor_ = false;

        //output_<<"\tstore i32 0, i32* %1, align 4"<<endl;

        node->body->accept(this);    //�ڶ��α���

        oneVistor_ = true;
        writeMethod_end(mInfo);     // TODO: ������Type

     //   symbolTable_->leave();
    }

    void IRGenerator::visit(PrimaryStmt *node)    // ��������
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
	    	int conditionLabel =labelCount_++;
	    	int bodyLabel = labelCount_++;
	        int updateLabel = labelCount_++;
	        int endLabel = labelCount_++;

            continueStack_.push_back(updateLabel);
            breakStack_.push_back(endLabel);
            //cout << "push break label: " << breakStack_.back() << endl;

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

            breakStack_.pop_back();
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
        output_ << "you should not visit here in Expr" << endl;
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
	        	callRet_ = true;
	        	constant_ = false ;
	            node->initValue->accept(this);
	            if(constant_)
				{
	            	output_<<"\tstore "<<numeric(info->getType())<<" "<<constant_value<<", "<<numeric(info->getType())<<"* %"+node->name+", align 4"<<endl;
	            	constant_ = true;
				}
				else
				{
					auto initCount = labelCount_-1;
	            	writeStore(info->getType(),std::to_string(initCount),node->name);   //store
				}
				callRet_ = false;
	        }
		}
    }

    void IRGenerator::visit(IdentifierExpr *node)
    {
        if(symbolTable_->hasVariable(node->name))
		{
        	auto nodeInfo = symbolTable_->getVariableInfo(node->name);
			node->setType(nodeInfo.getType());  //��������
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
				output_<<"\t%"<<labelCount_<<" = add nsw "<<numeric(node->getType())<<" %"<<labelCount_-1<<", 1"<<endl;
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
    	if(!oneVistor_)
		{
			call_value = true;
			auto mInfo = symbolTable_->getMethodInfo(node->callee);
	    	if(callRet_)
			{
				writeCall(labelCount_,node->callee,mInfo);
	        	labelCount_++;
			}
			else
			{
				writeCall(0,node->callee,mInfo);
			}
			call_index = 0;
			call_name = node->callee;
	        for(auto v : node->arguments)
	        {
	            v->accept(this);
	            call_index++;
	        }
	       	output_<<") "<<endl;
			constant_ = false;
			call_value = false;
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
        	if(call_value){
        		auto mInfo = symbolTable_->getMethodInfo(call_name);

        		output_<<numeric(mInfo.paramTypes_[call_index])<<" "<<node->lexeme;
			}
			else
			{
				constant_ = true;
        		constant_value = node->lexeme;
			}
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
        // TODO: string literal
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
			{         //��ֵ������

				fuzhi_ = false;
				constant_ = false;
				callRet_ = true;
	        	node->right->accept(this);
	        	auto rCount = labelCount_-1;
	        	auto rt = node->right->getType();      // ����������
	        	fuzhi_ = true;
	        	node->left->accept(this);
	        	auto lt = node->left->getType();       //����������

	        	node->setType(lt);
	        	if(constant_){    //ֱ�Ӹ�ֵ����
	        		output_<<"\tstore "<<numeric(lt)<<" "<<constant_value<<", "<<numeric(lt)<<"* %"+fuzhi_name+", align 4"<<endl;
				}
				else{
					writeStore(lt,std::to_string(rCount),fuzhi_name);          //store
				}


	        	fuzhi_ = false;
	        	constant_ = false;
	        	callRet_ = false;
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
	        	auto lt = node->left->getType();     //����������

	        	node->right->accept(this);

	        	if(constant_){
					rcon = constant_value;
					constant_=false;
				}
	        	auto rCount = labelCount_-1;
	        	auto rt = node->right->getType();      // ����������
	        	node->setType(lt);
	        	if(lcon.empty()&& rcon.empty()){    //����2�߶����ǳ���
	        		output_<<"\t%"<<labelCount_<<" = "<<change_Op(node->op)<<" "<<numeric(lt)<<" %"<<lCount<<", %"<<rCount<<endl;
				}
				else if(lcon.empty()&& !rcon.empty())      // �ұ��ǳ���
	        	{
	        		output_<<"\t%"<<labelCount_<<" = "<<change_Op(node->op)<<" "<<numeric(lt)<<" %"<<lCount<<", "<<rcon<<endl;
				}
				else if(!lcon.empty()&& rcon.empty())          // �����ǳ���
				{
					output_<<"\t%"<<labelCount_<<" = "<<change_Op(node->op)<<" "<<numeric(lt)<<" "<<lcon<<", %"<<rCount<<endl;
				}
				else          //���ǳ���
				{
					output_<<"\t%"<<labelCount_<<" = "<<change_Op(node->op)<<" "<<numeric(lt)<<" "<<lcon<<", "<<rcon<<endl;
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
