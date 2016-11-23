#include <iostream>
#include "./lexer/token.h"
#include "./lexer/scanner.h"

using namespace ycc;

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[])
{

    string filename(argv[1]);

    Scanner scanner(filename);

    // scanner test
    cout << "******************************************" << endl;
    cout << "* print token stream here" << endl;
    cout << "******************************************" << endl;
    while(scanner.getToken().tag() != TokenTag::END_OF_FILE)
    {
        scanner.getNextToken();
        cout << scanner.getToken().toString() << endl;

        if(scanner.getErrorFlag())
        {
            break;
        }
    }

    return 0;
}
