
#include "bigint.hpp"

int main()
{

    // open test data file
    // loop through tests
    //   check if result matches expected result
    //   log answer

    bigint x1(8234);
    bigint x2(-78);

    std::cout << x1 * x2 << "\n";

    return 0;
}
