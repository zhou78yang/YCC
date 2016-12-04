#include <iostream>
#include "depth_vistor.h"
#include "../common/symbols.h"

using std::cout;
using std::endl;

namespace ycc
{
    DepthVistor::DepthVistor()
        : level(0)
    {}

    void DepthVistor::upgrade()
    {
        level++;
    }

    void DepthVistor::degrade()
    {
        level--;
    }

    void DepthVistor::println(const std::string &msg, bool end)
    {
        for(int i = 0; i < level-1; i++) cout << "│   ";
        if(end)
        {
            cout << "└── ";
        }
        else
        {
            cout << "├── ";
        }
        cout << msg << endl;
    }


    void DepthVistor::visit(VecNodePtr ast)
    {
        cout << "<<AST>>" << endl;
        for(auto node : ast)
        {
            node->accept(this);
        }
        cout << endl;
    }

    void DepthVistor::visit(ASTNode *node)
    {
        cout << "you should not visit here in ASTNode" << endl;
    }

    void DepthVistor::visit(Stmt *node)
    {
        cout << "you should not visit here in Stmt" << endl;
    }

    void DepthVistor::visit(EmptyStmt *node)
    {
        upgrade();
        println("<<EmptyStmt>>("+ node->getLocation().toString()+")");
        degrade();
    }

    void DepthVistor::visit(ClassStmt *node)
    {
        upgrade();
        println("<<ClassStmt>>("+ node->getLocation().toString()+")");
        println("name:");
        println("└── " + node->name);
        println("body:");
        node->body->accept(this);
        degrade();
    }

    void DepthVistor::visit(MethodDeclStmt *node)
    {
        upgrade();
        println("<<MethodDeclStmt>>("+ node->getLocation().toString()+")");
        println("name:");
        println("└── " + node->name);
        // TODO: parameter
        println("body:");
        node->body->accept(this);
        degrade();
    }

    void DepthVistor::visit(PrimaryStmt *node)
    {
        upgrade();
        println("<<PrimaryStmt>>("+ node->getLocation().toString()+")");
        println("type:");
        println("└── " + node->type);
        println("decls:");
        for(auto v : node->decls)
        {
            v->accept(this);
        }
        degrade();
    }

    void DepthVistor::visit(BlockStmt *node)
    {
        upgrade();
        println("<<BlockStmt>>("+ node->getLocation().toString()+")");
        println("statements:");
        for(auto stmt : node->statements)
        {
            stmt->accept(this);
        }
        degrade();
    }

    void DepthVistor::visit(IfStmt *node)
    {
        upgrade();
        println("<<IfStmt>>("+ node->getLocation().toString()+")");
        println("condition:");
        node->condition->accept(this);
        println("thenBody:");
        node->thenBody->accept(this);
        if(node->elseBody)
        {
            println("elseBody:");
            node->elseBody->accept(this);
        }
        degrade();
    }

    void DepthVistor::visit(ForStmt *node)
    {
        upgrade();
        println("<<ForStmt>>("+ node->getLocation().toString()+")");
        println("init:");
        node->init->accept(this);
        println("condition:");
        node->condition->accept(this);
        println("update:");
        node->update->accept(this);
        println("body:");
        node->body->accept(this);
        degrade();
    }

    void DepthVistor::visit(WhileStmt *node)
    {
        upgrade();
        println("<<WhileStmt>>("+ node->getLocation().toString()+")");
        println("condition:");
        node->condition->accept(this);
        println("body:");
        node->body->accept(this);
        degrade();
    }

    void DepthVistor::visit(DoStmt *node)
    {
        upgrade();
        println("<<DoStmt>>("+ node->getLocation().toString()+")");
        println("body:");
        node->body->accept(this);
        println("condition:");
        node->condition->accept(this);
        degrade();
    }

    void DepthVistor::visit(SwitchStmt *node)
    {
        upgrade();
        println("<<SwitchStmt>>("+ node->getLocation().toString()+")");
        println("flag:");
        node->flag->accept(this);
        println("cases:");
        for(auto c : node->cases)
        {
            c->accept(this);
        }
        if(!node->defaultBody.empty())
        {
            println("default:");
            for(auto v : node->defaultBody)
            {
                v->accept(this);
            }
        }
        degrade();
    }

    void DepthVistor::visit(CaseStmt *node)
    {
        upgrade();
        println("<<CaseStmt>>("+ node->getLocation().toString()+")");
        println("label:");
        node->label->accept(this);
        println("statements:");
        for(auto v : node->statements)
        {
            v->accept(this);
        }
        degrade();
    }

    void DepthVistor::visit(ReturnStmt *node)
    {
        upgrade();
        println("<<ReturnStmt>>("+ node->getLocation().toString()+")");
        if(node->returnValue)
        {
            println("returnValue:");
            node->returnValue->accept(this);
        }
        degrade();
    }

    void DepthVistor::visit(BreakStmt *node)
    {
        upgrade();
        println("<<BreakStmt>>("+ node->getLocation().toString()+")");
        degrade();
    }

    void DepthVistor::visit(ContinueStmt *node)
    {
        upgrade();
        println("<<ContinueStmt>>("+ node->getLocation().toString()+")");
        degrade();
    }

    void DepthVistor::visit(Expr *node)
    {
        cout << "you should not visit here in Expr" << endl;
    }

    void DepthVistor::visit(VariableDeclExpr *node)
    {
        upgrade();
        println("<<VariableDeclExpr>>("+ node->getLocation().toString()+")");
        println("name:");
        println("└── " + node->name);
        if(node->initValue)
        {
            println("initValue:");
            node->initValue->accept(this);
        }
        degrade();
    }

    void DepthVistor::visit(IdentifierExpr *node)
    {
        upgrade();
        println("<<IdentifierExpr>>("+ node->getLocation().toString()+")");
        println("name:");
        println("└── " + node->name);
        degrade();
    }

    void DepthVistor::visit(NewExpr *node)
    {
        upgrade();
        println("<<NewExpr>>("+ node->getLocation().toString()+")");
        println("constructor:");
        node->constructor->accept(this);
        degrade();
    }

    void DepthVistor::visit(IndexExpr *node)
    {
        upgrade();
        println("<<IndexExpr>>("+ node->getLocation().toString()+")");
        println("left:");
        node->left->accept(this);
        println("index:");
        node->index->accept(this);
        degrade();
    }

    void DepthVistor::visit(CallExpr *node)
    {
        upgrade();
        println("<<CallExpr>>("+ node->getLocation().toString()+")");
        println("name:");
        node->name->accept(this);
        println("arguments:");
        for(auto v : node->arguments)
        {
            v->accept(this);
        }
        degrade();
    }

    void DepthVistor::visit(QualifiedIdExpr *node)
    {
        upgrade();
        println("<<QualifiedIdExpr>>("+ node->getLocation().toString()+")");
        println("left:");
        node->left->accept(this);
        println("right:");
        node->right->accept(this);
        degrade();
    }

    void DepthVistor::visit(IntExpr *node)
    {
        upgrade();
        println("<<IntExpr>>("+ node->getLocation().toString()+")");
        println("isChar:");
        println(node->isChar?"└── true":"└── false");
        println("value:");
        println("└── " + node->lexeme);
        degrade();
    }

    void DepthVistor::visit(RealExpr *node)
    {
        upgrade();
        println("<<RealExpr>>("+ node->getLocation().toString()+")");
        println("value:");
        println("└── " + node->lexeme);
        degrade();
    }

    void DepthVistor::visit(BoolExpr *node)
    {
        upgrade();
        println("<<BoolExpr>>("+ node->getLocation().toString()+")");
        println("value:");
        println(node->value?"└── true":"└── false");
        degrade();
    }

    void DepthVistor::visit(NullExpr *node)
    {
        upgrade();
        println("<<NullExpr>>("+ node->getLocation().toString()+")");
        degrade();
    }

    void DepthVistor::visit(StrExpr *node)
    {
        upgrade();
        println("<<StrExpr>>("+ node->getLocation().toString()+")");
        println("value:");
        println("└── " + node->value);
        degrade();
    }

    void DepthVistor::visit(ArrayExpr *node)
    {
        upgrade();
        println("<<ArrayExpr>>("+ node->getLocation().toString()+")");
        println("elems:");
        for(auto elem : node->elems)
        {
            elem->accept(this);
        }
        degrade();
    }

    void DepthVistor::visit(UnaryOpExpr *node)
    {
        upgrade();
        println("<<UnaryOpExpr>>("+ node->getLocation().toString()+")");
        println("isPrefix:");
        println(node->isPrefix?"└── true":"└── false");
        println("op:");
        println("└── " + tokenDesc(node->op));
        println("expr:");
        node->expr->accept(this);
        degrade();
    }

    void DepthVistor::visit(BinaryOpExpr *node)
    {
        upgrade();
        println("<<BinaryOpExpr>>("+ node->getLocation().toString()+")");
        println("op:");
        println("└── " + tokenDesc(node->op));
        println("left:");
        node->left->accept(this);
        println("right:");
        node->right->accept(this);
        degrade();
    }

    void DepthVistor::visit(TernaryOpExpr *node)
    {
        upgrade();
        println("<<TernaryOpExpr>>("+ node->getLocation().toString()+")");
        println("condition:");
        node->condition->accept(this);
        println("thenValue:");
        node->thenValue->accept(this);
        println("elseValue:");
        node->elseValue->accept(this);
    }


}
