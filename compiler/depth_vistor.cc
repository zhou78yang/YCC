#include <iostream>
#include "depth_vistor.h"
#include "../common/symbols.h"

using std::cout;
using std::endl;

namespace ycc
{
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
        cout << ";" << endl;
    }

    void DepthVistor::visit(ClassStmt *node)
    {
        cout << "class " << node->name << " : " << endl;
        node->body->accept(this);
    }

    void DepthVistor::visit(MethodDeclStmt *node)
    {
        cout << "function " << node->name
             << " : " << endl;
        node->body->accept(this);
    }

    void DepthVistor::visit(PrimaryStmt *node)
    {
        cout << node->type << " ";
        for(auto v : node->decls)
        {
            v->accept(this);
            cout << ", ";
        }
        cout << endl;
    }

    void DepthVistor::visit(BlockStmt *node)
    {
        cout << "{" << endl;
        for(auto stmt : node->statements)
        {
            stmt->accept(this);
            cout << endl;
        }
        cout << "}" << endl;
    }

    void DepthVistor::visit(IfStmt *node)
    {
        cout << "if ";
        node->condition->accept(this);
        cout << " : " << endl;
        node->thenBody->accept(this);
        if(node->elseBody)
        {
            cout << "else : " << endl;
            node->elseBody->accept(this);
        }
    }

    void DepthVistor::visit(ForStmt *node)
    {
        cout << "for ( ";
        node->init->accept(this);
        cout << " ; ";
        node->condition->accept(this);
        cout << " ; ";
        node->update->accept(this);
        cout << ")" << endl;
        node->body->accept(this);
    }

    void DepthVistor::visit(WhileStmt *node)
    {
        cout << "while ";
        node->condition->accept(this);
        cout << " : " << endl;
        node->body->accept(this);
    }

    void DepthVistor::visit(DoStmt *node)
    {
        cout << "do " << endl;
        node->body->accept(this);
        cout << "while ";
        node->condition->accept(this);
        cout << endl;
    }

    void DepthVistor::visit(SwitchStmt *node)
    {
        cout << "switch ";
        node->flag->accept(this);
        cout << "{" << endl;
        for(auto c : node->cases)
        {
            c->accept(this);
        }
        if(!node->defaultBody.empty())
        {
            cout << "default:" << endl;
            for(auto v : node->defaultBody)
            {
                v->accept(this);
            }
        }
        cout << "}" << endl;
    }

    void DepthVistor::visit(CaseStmt *node)
    {
        cout << "case ";
        node->label->accept(this);
        cout << " : " << endl;
        for(auto v : node->statements)
        {
            v->accept(this);
        }
    }

    void DepthVistor::visit(ReturnStmt *node)
    {
        cout << "return : ";
        if(node->returnValue) node->returnValue->accept(this);
        cout << endl;
    }

    void DepthVistor::visit(BreakStmt *node)
    {
        cout << "break;" << endl;
    }

    void DepthVistor::visit(ContinueStmt *node)
    {
        cout << "continue;" << endl;
    }

    void DepthVistor::visit(Expr *node)
    {
        cout << "you should not visit here in Expr" << endl;
    }

    void DepthVistor::visit(VariableDeclExpr *node)
    {
        cout << node->name;
        if(node->initValue)
        {
            cout << " = ";
            node->initValue->accept(this);
        }
    }

    void DepthVistor::visit(IdentifierExpr *node)
    {
        cout << node->name << " ";
    }

    void DepthVistor::visit(NewExpr *node)
    {
        cout << "new ";
        node->constructor->accept(this);
    }

    void DepthVistor::visit(IndexExpr *node)
    {
        node->left->accept(this);
        cout << "[";
        node->index->accept(this);
        cout << "]";
    }

    void DepthVistor::visit(CallExpr *node)
    {
        node->name->accept(this);
        cout << "( ";
        for(auto v : node->arguments)
        {
            v->accept(this);
            cout << ", ";
        }

        cout << ") ";
    }

    void DepthVistor::visit(QualifiedIdExpr *node)
    {
        node->left->accept(this);
        cout << ".";
        node->right->accept(this);
    }

    void DepthVistor::visit(IntExpr *node)
    {
        if(node->isChar)
        {
            char c = (char)node->value;
            cout << c << " ";
        }
        else
        {
            cout << node->lexeme << " ";
        }
    }

    void DepthVistor::visit(RealExpr *node)
    {
        cout << node->lexeme << " ";
    }

    void DepthVistor::visit(BoolExpr *node)
    {
        if(node->value) cout << "true ";
        else cout << "false ";
    }

    void DepthVistor::visit(NullExpr *node)
    {
        cout << "null ";
    }

    void DepthVistor::visit(StrExpr *node)
    {
        cout << "\"" << node->value << "\" ";
    }

    void DepthVistor::visit(ArrayExpr *node)
    {
        cout << "Array : {";
        for(auto elem : node->elems)
        {
            elem->accept(this);
        }
        cout << "} ";
    }

    void DepthVistor::visit(UnaryOpExpr *node)
    {
        if(node->isPrefix)
        {
            cout << tokenDesc(node->op) << " ";
            node->expr->accept(this);
        }
        else
        {
            node->expr->accept(this);
            cout << tokenDesc(node->op) << " ";
        }
    }

    void DepthVistor::visit(BinaryOpExpr *node)
    {
        cout << "(";
        node->left->accept(this);
        cout << tokenDesc(node->op) << " ";
        node->right->accept(this);
        cout << ")";
    }

    void DepthVistor::visit(TernaryOpExpr *node)
    {
        node->condition->accept(this);
        cout << "? ";
        node->thenValue->accept(this);
        cout << ": ";
        node->elseValue->accept(this);
    }


}
