
#include "bigint.hpp"
#include <filesystem>
#include <fstream>

int main()
{

    // https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
    // Basic Addition Tests
    std::cout << "Testing (+) and (+=) Operators\n";
    std::string path = "./data/addition_test/";
    int test_pass = 0;
    for (const auto &file : std::filesystem::directory_iterator(path))
    {
        // std::cout << file.path() << "\n";
        std::ifstream input(file.path());
        std::string line;
        bigint numbers[3];
        int index = 0;
        while (std::getline(input, line))
        {
            // std::cout << line << "\n";
            numbers[index] = bigint(line);
            index += 1;
        }
        input.close();

        bigint result1 = numbers[0] + numbers[1];
        bigint result2 = numbers[1] + numbers[0];
        if (result1 == numbers[2] && result2 == numbers[2])
        {
            test_pass += 1;
        }

        result1 = numbers[0];
        result2 = numbers[1];

        result1 += numbers[1];
        result2 += numbers[0];
        if (result1 == numbers[2] && result2 == numbers[2])
        {
            test_pass += 1;
        }
    }
    std::cout << "Basic Addition Tests: " << test_pass << "/240\n";

    // Basic Subtraction Tests
    std::cout << "Testing (-) and (-=) Operators\n";
    test_pass = 0;
    path = "./data/subtraction_test/";
    for (const auto &file : std::filesystem::directory_iterator(path))
    {
        // std::cout << file.path() << "\n";
        std::ifstream input(file.path());
        std::string line;
        bigint numbers[3];
        int index = 0;
        while (std::getline(input, line))
        {
            // std::cout << line << "\n";
            numbers[index] = bigint(line);
            index += 1;
        }
        input.close();

        bigint result1 = numbers[0] - numbers[1];
        if (result1 == numbers[2])
        {
            test_pass += 1;
        }

        result1 = numbers[0];
        result1 -= numbers[1];
        if (result1 == numbers[2])
        {
            test_pass += 1;
        }
    }
    std::cout << "Basic Subtraction Tests: " << test_pass << "/240\n";

    // Basic Multiplication Tests
    std::cout << "Testing (*) and (*=) Operators\n";
    test_pass = 0;
    path = "./data/multiplication_test/";
    for (const auto &file : std::filesystem::directory_iterator(path))
    {
        // std::cout << file.path() << "\n";
        std::ifstream input(file.path());
        std::string line;
        bigint numbers[3];
        int index = 0;
        while (std::getline(input, line))
        {
            // std::cout << line << "\n";
            numbers[index] = bigint(line);
            index += 1;
        }
        input.close();

        bigint result1 = numbers[0] * numbers[1];
        bigint result2 = numbers[1] * numbers[0];
        if (result1 == numbers[2] && result2 == numbers[2])
        {
            //std::cout << file.path() << ": test passed\n";
            test_pass += 1;
        } else {
            std::cout << file.path() << ": test failed\n";
            std::cout << numbers[0] << "\n";
            std::cout << numbers[1] << "\n";
            std::cout << numbers[2] << "\n";
            std::cout << result1 << "\n";
            std::cout << result2 << "\n";
        }

        result1 = numbers[0];
        result2 = numbers[1];
        result1 *= numbers[1];
        result2 *= numbers[0];
        if (result1 == numbers[2] && result2 == numbers[2])
        {
            //std::cout << file.path() << ": test passed\n";
            test_pass += 1;
        } else {
            std::cout << file.path() << ": test failed\n";
        }
    }
    std::cout << "Basic Multiplication Tests: " << test_pass << "/240\n";

    //- (unary)

    //== != < > <= >=

    // test << somehow?

    //++ pre and post

    //-- pre and post

    return 0;
}
