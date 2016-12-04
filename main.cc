#include "./lexer/token.h"
#include "./lexer/scanner.h"
#include "./parser/parser.h"
#include "./compiler/depth_vistor.h"
#include "./main.hpp"

using namespace ycc;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(int argc, char *argv[])
{
    init(argc, argv);

    if(checkOption(OpTag::VER_INFO))
    {
        cout << APPNAME << " " << VERSION << endl;
        return 0;
    }
    if(checkOption(OpTag::HELP))
    {
        manualReport();
        return 0;
    }

    if(srcFileName.size() < 1)
    {
        cout << ">>";
        cin >> srcFileName;
    }


    if(checkOption(OpTag::DUMP_TOKENS))
    {
        // scanner & error report test
        Scanner dumpTokens(srcFileName);
        cout << "******************************************" << endl;
        cout << "* print token stream here" << endl;
        cout << "******************************************" << endl;
        while(dumpTokens.getToken().tag() != TokenTag::END_OF_FILE)
        {
            auto token = dumpTokens.getNextToken();
            auto loc = dumpTokens.getTokenLocation();
            cout << std::left << std::setw(50) << "[T] " + loc.toString();
            cout << token.toString() << "\n";
        }
        cout << endl;
    }

    Scanner scanner(srcFileName);
    Parser parser(scanner);
    auto ast = parser.parse();

    if(checkOption(OpTag::DUMP_AST))
    {
        DepthVistor *vistor = new DepthVistor();
        vistor->visit(ast);
    }

    SymbolTable::getInstance()->dump();

    ExceptionHandler::getInstance()->report();

    return 0;
}
