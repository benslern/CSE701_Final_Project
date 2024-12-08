
#include "bigint.hpp"
#include <filesystem>
#include <fstream>

int main()
{

    // open test data file
    // loop through tests
    //   check if result matches expected result
    //   log answer

    // https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
    // Basic Addition Tests
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

        if (numbers[0] + numbers[1] == numbers[2] && numbers[1] + numbers[0] == numbers[2])
        {
            //std::cout << file.path() << ": test passed\n";
            test_pass += 1;
        }
        else
        {
            std::cout << file.path() << ": test failed\n";
        }
    }
    std::cout << "Basic Addition Tests: " << test_pass << "/120\n";

    // Basic Subtraction Tests
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

        if (numbers[0] - numbers[1] == numbers[2])
        {
            //std::cout << file.path() << ": test passed\n";
            test_pass += 1;
        }
        else
        {
            std::cout << file.path() << ": test failed\n";

        }
    }
    std::cout << "Basic Subtraction Tests: " << test_pass << "/120\n";

    return 0;
}
