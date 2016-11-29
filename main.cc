#include <iostream>
#include "./lexer/token.h"
#include "./lexer/scanner.h"
#include "./parser/parser.h"
#include "./compiler/depth_vistor.h"

using namespace ycc;

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[])
{

    string filename(argv[1]);

    Scanner scanner(filename);

    /*
    // scanner & error report test
    cout << "******************************************" << endl;
    cout << "* print token stream here" << endl;
    cout << "******************************************" << endl;
    while(scanner.getToken().tag() != TokenTag::END_OF_FILE)
    {
        scanner.getNextToken();
        //cout << scanner.getToken().toString() << endl;
        // exception handler test
        ExceptionHandler::getInstance()->add(scanner.getToken().toString(), scanner.getTokenLocation(), ErrorType::NOTE);
    }
    */

    Parser parser(scanner);
    auto ast = parser.parse();
    DepthVistor *vistor = new DepthVistor();

    for(auto node : ast)
    {
        node->accept(vistor);
    }

    SymbolTable::getInstance()->dump();

    ExceptionHandler::getInstance()->report();

    return 0;
}
