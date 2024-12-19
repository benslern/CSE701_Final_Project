
#include "bigint.hpp"
#include <filesystem>
#include <fstream>
#include <stdexcept>

// To test each operator I created a run_operator_test function that runs
// a test function argument on all the test files from a single directory.
// The test function must take a std::vector<bigint> as its only input.
// https://www.geeksforgeeks.org/passing-a-function-as-a-parameter-in-cpp/

// To get all of the files from a directory path I used the iterator from this link.
// https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c

// To read in the numbers from each file I used the example from
// section 5.3.2 of the Lecture notes

/**
 * @brief Run the test function on each test file from a given directory.
 * Return the number of successful tests.
 *
 * @param std::string path
 * @param function test
 * @return uint8_t
 */
uint32_t run_operator_tests(const std::string &path, bool (*test)(const std::vector<bigint> &), const std::string op)
{
    std::cout << "Testing " << op << " Operator\n";
    uint32_t test_pass_count = 0;
    uint32_t num_tests = 0;

    try
    {
        // Loop through all test files from the directory path
        for (const auto &file : std::filesystem::directory_iterator(path))
        {
            // Open file
            std::ifstream input(file.path());
            if (!input.is_open())
            {
                throw std::invalid_argument("File does not exist or cannot be opened.");
            }
            std::string line;
            std::vector<bigint> numbers;

            // Read in numbers from test file
            while (std::getline(input, line))
            {
                // Use the bigint string constructor for running all operator tests
                numbers.push_back(bigint(line));
            }
            input.close();

            // Pass numbers from file to test function and increment if it passes
            if (test(numbers))
            {
                test_pass_count++;
            }
            num_tests++;
        }
    }
    catch (std::filesystem::filesystem_error e)
    {
        throw std::invalid_argument("Directory does not exist or cannot be accessed.");
    }

    // Return number of successful tests
    std::cout << "Test " << op << " Results: " << unsigned(test_pass_count) << "/" << unsigned(num_tests) << "\n\n";
    return test_pass_count;
}

/**
 * @brief Run tests on + and += operators. Input vector must contain three bigints
 * where the third bigint is the sum of the first two bigints.
 *
 * @param std::vector<bigint> input
 * @return true
 * @return false
 */
bool basic_addition_test(const std::vector<bigint> &input)
{
    if (input.size() != 3)
    {
        throw std::invalid_argument("Invalid input size.");
    }

    //+ operator test
    bigint result1 = input[0] + input[1];
    bigint result2 = input[1] + input[0];
    if (!(result1 == input[2] && result2 == input[2]))
    {
        return false;
    }

    //+= operator test
    result1 = input[0];
    result2 = input[1];
    result1 += input[1];
    result2 += input[0];
    if (!(result1 == input[2] && result2 == input[2]))
    {
        return false;
    }

    return true;
}

/**
 * @brief Run tests on - and -= operators. input vector must contain three bigints
 * where the third bigint is the difference of the first two bigints.
 *
 * @param std::vector<bigint> input
 * @return true
 * @return false
 */
bool basic_subtraction_test(const std::vector<bigint> &input)
{
    if (input.size() != 3)
    {
        throw std::invalid_argument("Invalid input size");
    }

    //- operator test
    bigint result = input[0] - input[1];
    if (!(result == input[2]))
    {
        return false;
    }

    //-= operator test
    result = input[0];
    result -= input[1];
    if (!(result == input[2]))
    {
        return false;
    }

    return true;
}

/**
 * @brief Run tests on * and *= operators. input vector must contain three bigints
 * where the third bigint is the product of the first two bigints.
 *
 * @param std::vector<bigint> input
 * @return true
 * @return false
 */
bool basic_multiplication_test(const std::vector<bigint> &input)
{
    if (input.size() != 3)
    {
        throw std::invalid_argument("Invalid input size");
    }

    //* operator test
    bigint result1 = input[0] * input[1];
    bigint result2 = input[1] * input[0];
    if (!(result1 == input[2] && result2 == input[2]))
    {
        return false;
    }

    //*= operator test
    result1 = input[0];
    result2 = input[1];
    result1 *= input[1];
    result2 *= input[0];
    if (!(result1 == input[2] && result2 == input[2]))
    {
        return false;
    }

    return true;
}

/**
 * @brief Run tests on - unary operator. input vector must contain two bigints
 * where the second bigint is the negative of the first bigint.
 *
 * @param std::vector<bigint> input
 * @return true
 * @return false
 */
bool basic_negation_test(const std::vector<bigint> &input)
{
    if (input.size() != 2)
    {
        throw std::invalid_argument("Invalid input size");
    }

    bigint result = input[0];
    result = -result;
    if (!(result == input[1]))
    {
        return false;
    }

    return true;
}

/**
 * @brief Run tests on ++ pre operator. input vector must contain two bigints
 * where the second bigint is the first bigint plus one.
 *
 * @param std::vector<bigint> input
 * @return true
 * @return false
 */
bool basic_pp_pre_test(const std::vector<bigint> &input)
{
    if (input.size() != 2)
    {
        throw std::invalid_argument("Invalid input size");
    }

    bigint result1 = input[0];
    bigint result2 = ++result1;
    if (!(result1 == input[1] && result2 == input[1]))
    {
        return false;
    }

    return true;
}

/**
 * @brief Run tests on ++ post operator. input vector must contain two bigints
 * where the second bigint is the first bigint plus one.
 *
 * @param std::vector<bigint> input
 * @return true
 * @return false
 */
bool basic_pp_post_test(const std::vector<bigint> &input)
{
    if (input.size() != 2)
    {
        throw std::invalid_argument("Invalid input size");
    }

    bigint result1 = input[0];
    bigint result2 = result1++;
    if (!(result1 == input[1] && result2 == input[0]))
    {
        return false;
    }

    return true;
}

/**
 * @brief Run tests on -- pre operator. input vector must contain two bigints
 * where the second bigint is the first bigint minus one.
 *
 * @param std::vector<bigint> input
 * @return true
 * @return false
 */
bool basic_mm_pre_test(const std::vector<bigint> &input)
{
    if (input.size() != 2)
    {
        throw std::invalid_argument("Invalid input size");
    }

    bigint result1 = input[0];
    bigint result2 = --result1;
    if (!(result1 == input[1] && result2 == input[1]))
    {
        return false;
    }

    return true;
}

/**
 * @brief Run tests on -- post operator. input vector must contain two bigints
 * where the second bigint is the first bigint minus one.
 *
 * @param std::vector<bigint> input
 * @return true
 * @return false
 */
bool basic_mm_post_test(const std::vector<bigint> &input)
{
    if (input.size() != 2)
    {
        throw std::invalid_argument("Invalid input size");
    }

    bigint result1 = input[0];
    bigint result2 = result1--;
    if (!(result1 == input[1] && result2 == input[0]))
    {
        return false;
    }

    return true;
}

/**
 * @brief Run tests on ==, <=, and >= operators. Input vector must contain two bigints that are identical.
 *
 * @param std::vector<bigint> input
 * @return true
 * @return false
 */
bool basic_eq_test(const std::vector<bigint> &input)
{
    if (input.size() != 2)
    {
        throw std::invalid_argument("Invalid input size");
    }

    if (!(input[0] == input[1]))
    {
        return false;
    }

    if (!(input[0] <= input[1]))
    {
        return false;
    }

    if (!(input[0] >= input[1]))
    {
        return false;
    }

    return true;
}

/**
 * @brief Run tests on != operator. Input vector must contain two bigints that are not identical.
 *
 * @param std::vector<bigint> input
 * @return true
 * @return false
 */
bool basic_neq_test(const std::vector<bigint> &input)
{
    if (input.size() != 2)
    {
        throw std::invalid_argument("Invalid input size");
    }

    if (!(input[0] != input[1]))
    {
        return false;
    }

    return true;
}

/**
 * @brief Run tests on < and <= operators. Input vector must contain two bigints
 * where the first bigint is less than the second bigint.
 *
 * @param std::vector<bigint> input
 * @return true
 * @return false
 */
bool basic_lt_test(const std::vector<bigint> &input)
{
    if (input.size() != 2)
    {
        throw std::invalid_argument("Invalid input size");
    }

    if (!(input[0] < input[1]))
    {
        return false;
    }

    if (!(input[0] <= input[1]))
    {
        return false;
    }

    return true;
}

/**
 * @brief Run tests on > and >= operators. Input vector must contain two bigints
 * where the first bigint is greater than the second bigint.
 *
 * @param std::vector<bigint> input
 * @return true
 * @return false
 */
bool basic_gt_test(const std::vector<bigint> &input)
{
    if (input.size() != 2)
    {
        throw std::invalid_argument("Invalid input size");
    }

    if (!(input[0] > input[1]))
    {
        return false;
    }

    if (!(input[0] >= input[1]))
    {
        return false;
    }

    return true;
}

/**
 * @brief Run complex + tests. Input vector must contain three bigints.
 * 
 * @param std::vector<bigint> input 
 * @return true 
 * @return false 
 */
bool complex_addition_test(const std::vector<bigint> &input){
    if (input.size() != 3)
    {
        throw std::invalid_argument("Invalid input size");
    }

    if((input[0] + input[1]) + input[2] != (input[0] + input[2]) + input[1]){
        return false;
    }

    if((input[0] + input[1]) + input[2] != (input[1] + input[2]) + input[0]){
        return false;
    }

    return true;
}

/**
 * @brief Run complex + and - test. Input vector must contain three bigints.
 * 
 * @param std::vector<bigint> input 
 * @return true 
 * @return false 
 */
bool complex_addition_subtraction_test(const std::vector<bigint> &input){
    if (input.size() != 3)
    {
        throw std::invalid_argument("Invalid input size");
    }

    if(((input[0] - input[1]) + input[2]) != ((input[0] + input[2]) - input[1])){
        return false;
    }
    
    if(((input[0] - input[1]) + input[2]) != ((input[2] - input[1]) + input[0])){
        return false;
    }

    return true;
}

/**
 * @brief Run complex - and * test. Input vector must contain three bigints.
 * 
 * @param std::vector<bigint> input 
 * @return true 
 * @return false 
 */
bool complex_subtraction_multiplication_test(const std::vector<bigint> &input){
    
    if (input.size() != 3)
    {
        throw std::invalid_argument("Invalid input size");
    }

    if(((input[0] - input[1]) * input[2]) != ((input[0] * input[2]) - (input[1] * input[2]))){
        return false;
    }

    return true;
}

/**
 * @brief Run complex + and * test. Input vector must contain three bigints.
 * 
 * @param std::vector<bigint> input 
 * @return true 
 * @return false 
 */
bool complex_addition_multiplication_test(const std::vector<bigint> &input){
    
    if (input.size() != 3)
    {
        throw std::invalid_argument("Invalid input size");
    }

    if(((input[0] + input[1]) * input[2]) != ((input[0] * input[2]) + (input[1] * input[2]))){
        return false;
    }

    return true;
}

/**
 * @brief Run complex * test. Input vector must contain three bigints.
 * 
 * @param std::vector<bigint> input 
 * @return true 
 * @return false 
 */
bool complex_multiplication_test(const std::vector<bigint> &input){

    if (input.size() != 3)
    {
        throw std::invalid_argument("Invalid input size");
    }

    if ((input[0] * input[1]) * input[2] != (input[0] * input[2]) * input[1]){
        return false;
    }

    if ((input[0] * input[1]) * input[2] != (input[1] * input[2]) * input[0]){
        return false;
    }

    return true;
}

/**
 * @brief Run tests on the bigint String Constructor.
 * exp is a string of the expected bigint value.
 * input is a string argument for bigint.
 *
 * @param std::string exp
 * @param std::string input
 */
void run_constructor_test(std::string exp, std::string input)
{
    std::cout << "Expected bigint Value: " << exp << "\n";
    try
    {
        bigint test(input);
        std::cout << "Actual bigint Value:   " << test << "\n\n";
    }
    catch (std::invalid_argument e)
    {
        std::cout << "Actual bigint Value:   Error: Invalid String\n\n";
    }
}

/**
 * @brief Run tests on the bigint int64_t Constructor.
 * exp is a string of the expected bigint value.
 * input is a int64_t argument for bigint.
 *
 * @param std::string exp
 * @param int64_t input
 */
void run_constructor_test(std::string exp, int64_t input = 0)
{
    std::cout << "Expected bigint Value: " << exp << "\n";
    bigint test(input);
    std::cout << "Actual bigint Value:   " << test << "\n\n";
}

/**
 * @brief Run tests on the bigint default Constructor.
 *
 * @param std::string exp
 * @param int64_t input
 */
void run_constructor_test()
{
    std::cout << "Expected bigint Value: 0\n";
    bigint test;
    std::cout << "Actual bigint Value:   " << test << "\n\n";
}

/**
 * @brief Main function that runs all of the tests.
 *
 * @return int
 */
int main()
{

    // Constructor Tests

    // default constructor test
    run_constructor_test();

    // int constructor tests
    run_constructor_test("0", 0);
    run_constructor_test("0", -0);
    run_constructor_test("255", 255);
    run_constructor_test("-255", -255);
    run_constructor_test("9223372036854775807", INT64_MAX);
    run_constructor_test("-9223372036854775808", INT64_MIN);
    run_constructor_test("123400000", 1.234E8);
    run_constructor_test("-123400000", -1.234E8);

    // string constructor tests
    run_constructor_test("0", "0");
    run_constructor_test("0", "-0");
    run_constructor_test("255", "255");
    run_constructor_test("-255", "-255");
    run_constructor_test("9223372036854775807", "9223372036854775807");
    run_constructor_test("-9223372036854775808", "-9223372036854775808");
    run_constructor_test("123456789098765432110293847566574839201", "123456789098765432110293847566574839201");
    run_constructor_test("-123456789098765432110293847566574839201", "-123456789098765432110293847566574839201");
    run_constructor_test("1", "0001");
    run_constructor_test("-1", "-0001");
    run_constructor_test("0", "0000");
    run_constructor_test("0", "-0000");
    run_constructor_test("123456789", "  123456789  ");
    run_constructor_test("Error: Invalid String", "12345 6789");
    run_constructor_test("Error: Invalid String", "12345+6789");
    run_constructor_test("Error: Invalid String", "12345-6789");
    run_constructor_test("Error: Invalid String", "12345*6789");
    run_constructor_test("Error: Invalid String", "12345/6789");
    run_constructor_test("Error: Invalid String", "one thousand and twenty five");
    run_constructor_test("Error: Invalid String", "asdfflkjdsaf");
    run_constructor_test("Error: Invalid String", "100.1234");
    run_constructor_test("Error: Invalid String", "0.1234");
    run_constructor_test("Error: Invalid String", ".1234");
    run_constructor_test("Error: Invalid String", "1234 ab");
    run_constructor_test("Error: Invalid String", "#$%^&*");
    run_constructor_test("Error: Invalid String", "");
    run_constructor_test("Error: Invalid String", "--123456789");
    run_constructor_test("Error: Invalid String", "1.234E8");
    run_constructor_test("Error: Invalid String", "-1.234E8");
    run_constructor_test("Error: Invalid String", "  -001  ");

    // Operator Tests
    uint32_t test_pass = 0;

    // Basic Addition Tests
    test_pass += run_operator_tests("./data/random_test/addition_test/", &basic_addition_test, "(+) and (+=)");
    test_pass += run_operator_tests("./data/custom_test/addition_test/", &basic_addition_test, "(+) and (+=)");

    // Basic Subtraction Tests
    test_pass += run_operator_tests("./data/random_test/subtraction_test/", &basic_subtraction_test, "(-) and (-=)");
    test_pass += run_operator_tests("./data/custom_test/subtraction_test/", &basic_subtraction_test, "(-) and (-=)");

    // Basic Multiplication Tests
    test_pass += run_operator_tests("./data/random_test/multiplication_test/", &basic_multiplication_test, "(*) and (*=)");
    test_pass += run_operator_tests("./data/custom_test/multiplication_test/", &basic_multiplication_test, "(*) and (*=)");

    // Basic Negation Tests
    test_pass += run_operator_tests("./data/random_test/negation_test/", &basic_negation_test, "(-)");
    test_pass += run_operator_tests("./data/custom_test/negation_test/", &basic_negation_test, "(-)");

    // Basic ++ Pre Tests
    test_pass += run_operator_tests("./data/random_test/increment_test/pp_pre/", &basic_pp_pre_test, "(++) Pre");
    test_pass += run_operator_tests("./data/custom_test/increment_test/pp_pre/", &basic_pp_pre_test, "(++) Pre");

    // Basic ++ Post Tests
    test_pass += run_operator_tests("./data/random_test/increment_test/pp_post/", &basic_pp_post_test, "(++) Post");
    test_pass += run_operator_tests("./data/custom_test/increment_test/pp_post/", &basic_pp_post_test, "(++) Post");

    // Basic -- Pre Tests
    test_pass += run_operator_tests("./data/random_test/increment_test/mm_pre/", &basic_mm_pre_test, "(--) Pre");
    test_pass += run_operator_tests("./data/custom_test/increment_test/mm_pre/", &basic_mm_pre_test, "(--) Pre");

    // Basic ++ Post Tests
    test_pass += run_operator_tests("./data/random_test/increment_test/mm_post/", &basic_mm_post_test, "(--) Post");
    test_pass += run_operator_tests("./data/custom_test/increment_test/mm_post/", &basic_mm_post_test, "(--) Post");

    // Basic Eq Tests
    test_pass += run_operator_tests("./data/random_test/condition_test/eq/", &basic_eq_test, "(==) and (<=) and (>=)");
    test_pass += run_operator_tests("./data/custom_test/condition_test/eq/", &basic_eq_test, "(==) and (<=) and (>=)");

    // Basic Neq Tests
    test_pass += run_operator_tests("./data/random_test/condition_test/neq/", &basic_neq_test, "(!=)");
    test_pass += run_operator_tests("./data/custom_test/condition_test/neq/", &basic_neq_test, "(!=)");

    // Basic LT Tests
    test_pass += run_operator_tests("./data/random_test/condition_test/lt/", &basic_lt_test, "(<) and (<=)");
    test_pass += run_operator_tests("./data/custom_test/condition_test/lt/", &basic_lt_test, "(<) and (<=)");

    // Basic GT Tests
    test_pass += run_operator_tests("./data/random_test/condition_test/gt/", &basic_gt_test, "(>) and (>=)");
    test_pass += run_operator_tests("./data/custom_test/condition_test/gt/", &basic_gt_test, "(>) and (>=)");

    std::cout << "Total Passed Basic Tests: " << test_pass << "\n";
    std::cout << "Total Failed Basic Tests: " << 1069 - test_pass << "\n\n";

    // Complex Tests - combinations of multiple operators

    test_pass = 0;
    test_pass += run_operator_tests("./data/complex_test/", &complex_addition_test, "(+)");
    test_pass += run_operator_tests("./data/complex_test/", &complex_addition_subtraction_test, "(+) and (-)");
    test_pass += run_operator_tests("./data/complex_test/", &complex_subtraction_multiplication_test, "(-) and (*)");
    test_pass += run_operator_tests("./data/complex_test/", &complex_addition_multiplication_test, "(+) and (*)");
    test_pass += run_operator_tests("./data/complex_test/", &complex_multiplication_test, "(*)");

    std::cout << "Total Passed Complex Tests: " << test_pass << "\n";
    std::cout << "Total Failed Complex Tests: " << 150 - test_pass << "\n";

    return 0;
}