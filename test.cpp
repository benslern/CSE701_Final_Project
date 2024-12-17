
#include "bigint.hpp"
#include <filesystem>
#include <fstream>
#include <stdexcept>

// https://www.geeksforgeeks.org/passing-a-function-as-a-parameter-in-cpp/
/**
 * @brief Run the test function on each test file from a given directory.
 * Return the number of successful tests.
 *
 * @param path
 * @param test
 * @return uint8_t
 */
uint32_t run_operator_tests(const std::string &path, bool (*test)(const std::vector<bigint> &), const std::string op)
{
    std::cout << "Testing " << op << " Operator\n";
    uint32_t test_pass_count = 0;
    uint32_t num_tests = 0;
    // Loop through all test files from the directory path
    // https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
    try
    {
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

            // Pass numbers to test function and increment if it passes
            if (test(numbers))
            {
                test_pass_count++;
            }
            num_tests++;
        }
    }
    catch (std::filesystem::filesystem_error e)
    {
        throw std::invalid_argument("Direcotry does not exist or cannot be opened.");
    }

    // Return number of successful tests
    std::cout << "Test " << op << " Results: " << unsigned(test_pass_count) << "/" << unsigned(num_tests) << "\n\n";
    return test_pass_count;
}

/**
 * @brief Run tests on + and += operators. Input vector must contain three bigints
 * where the third bigint is the sum of the first two bigints.
 *
 * @param input
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
 * @brief Run tests on - and -= operators. input vector contains two bigints,
 * which were randomly generated using a Python script, and a third bigint that
 * is the difference of the two random bigints.
 *
 * @param input
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
 * @brief Run tests on * and *= operators. input vector contains two bigints,
 * which were randomly generated using a Python script, and a third bigint that
 * is the product of the two random bigints.
 *
 * @param input
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
 * @brief Run tests on - unary operator. input vector contains one bigint,
 * which was randomly generated using a Python script, and a second bigint that
 * is the negative of the random bigint.
 *
 * @param input
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
 * @brief Run tests on ++ pre operator. input vector contains one bigint,
 * which was randomly generated using a Python script, and a second bigint that
 * is the random bigint+1.
 *
 * @param input
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
 * @brief Run tests on ++ post operator. input vector contains one bigint,
 * which was randomly generated using a Python script, and a second bigint that
 * is the random bigint+1.
 *
 * @param input
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
 * @brief Run tests on -- pre operator. input vector contains one bigint,
 * which was randomly generated using a Python script, and a second bigint that
 * is the random bigint-1.
 *
 * @param input
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
 * @brief Run tests on -- post operator. input vector contains one bigint,
 * which was randomly generated using a Python script, and a second bigint that
 * is the random bigint-1.
 *
 * @param input
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
 * @brief Run tests on == operator. Input vector contains two bigints that are identical.
 *
 * @param input
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
 * @brief Run tests on != operator. Input vector contains two bigints that are not identical.
 *
 * @param input
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
 * @brief Run tests on != operator. Input vector contains two bigints.
 * Returns true if the first is less than the second, otherwise false.
 *
 * @param input
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
 * @brief Run tests on != operator. Input vector contains two bigints.
 * Returns true if the first is greater than the second, otherwise false.
 *
 * @param input
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

void run_constructor_test(std::string exp, int64_t input = 0)
{
    std::cout << "Expected bigint Value: " << exp << "\n";
    bigint test(input);
    std::cout << "Actual bigint Value:   " << test << "\n\n";
}

void run_constructor_test()
{
    std::cout << "Expected bigint Value: 0\n";
    bigint test;
    std::cout << "Actual bigint Value:   " << test << "\n\n";
}

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
    run_constructor_test("Error: Invalid String","  -001  ");

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

    std::cout << "Total Passed Tests: " << test_pass << "\n";
    std::cout << "Total Failed Tests: " << 1069 - test_pass << "\n";

    return 0;
}
