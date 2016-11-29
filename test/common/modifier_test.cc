#include <iostream>
#include "../common/symbols.h"
#include "../common/symbols.cc"

using namespace ycc;
using std::cout;
using std::endl;

int main()
{
    cout << isModifier(TokenTag::PUBLIC) << endl;
    cout << isModifier(TokenTag::FINAL) << endl;
    cout << isModifier(TokenTag::CLASS) << endl;

    return 0;
}
