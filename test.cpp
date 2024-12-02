
#include "bigint.hpp"

int main()
{

    bigint test1;
    std::cout << test1 << "\n";

    bigint test2(1234);
    std::cout << test2 << "\n";

    bigint test3(-1E16);
    std::cout << test3 << "\n";

    std::cout << "Hello World!\n";

    return 0;
}
