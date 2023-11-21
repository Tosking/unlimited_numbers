#include <iostream>
#include "unumber.h"
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    Unumber num("10000000000000000000");
    Unumber num2("88888888");
    num.print();
    num2.print();
    (num - num2)._debug();

    return 0;
}
