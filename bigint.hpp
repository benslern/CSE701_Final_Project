#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <stdio.h>
#include <cmath>

class bigint
{
public:
    // Public members

    /**
     * @brief Default Constructor. Construct a new bigint object with value 0.
     */
    bigint()
    {
        number.push_back(0);
        isNeg = false;
    }

    /**
     * @brief Signed 64-bit Integer Constructor. Construct a new bigint object with a int64_t value.
     * @param int64_t value 
     */
    bigint(int64_t value)
    {
        isNeg = (value < 0);
        do
        {
            number.push_back(static_cast<uint8_t>(abs(value % 10)));
            value /= 10;
        } while (abs(value) >= 1);

        // https: // www.reddit.com/r/cpp/comments/gctyw5/adding_two_uint16_t_s_without_wconversion_warning/
        // https://stackoverflow.com/questions/8877448/how-do-i-reverse-a-c-vector
        std::reverse(number.begin(), number.end());
    }

    /**
     * @brief String Constructor. Construct a new bigint object with a std::string value.
     */
    bigint(std::string value)
    {
        // https://stackoverflow.com/questions/83439/remove-spaces-from-stdstring-in-c
        try
        {
            if (value.length() == 0)
            {
                throw std::invalid_argument("Error: Invalid String - (" + value + ")\n");
            }
            isNeg = false;
            if (value[0] == '-')
            {   
                isNeg = true;
                value.erase(0, 1);
                
            }
            while ((value.at(0) == '0' || value.at(0) == ' ') && value.length() > 1)
            {
                value = value.substr(1, value.length() - 1);
            }
            while ((value.at(value.length() - 1) == ' ') && value.length() > 1)
            {
                value = value.substr(0, value.length() - 1);
            }
            if (value.length() == 1 && value[0] == '0'){
                isNeg = false;
            }
            for (size_t i = 0; i < value.length(); i++)
            {
                std::string s(1, value[i]);
                // number.push_back(static_cast<uint8_t>(value[i] - '0'));
                number.push_back(static_cast<uint8_t>(std::stoi(s)));
            }
            // https://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
        }
        catch (const std::invalid_argument &e)
        {
            throw std::invalid_argument("Error: Invalid String - (" + value + ")\n");
        }
    }

    // Friend function declerations

    friend std::ostream &operator<<(std::ostream &, const bigint &);
    friend bool operator==(const bigint &, const bigint &);
    friend bool operator<(const bigint &, const bigint &);
    friend bigint operator-(bigint &operand);
    friend bigint operator+(const bigint &, const bigint &);
    friend bigint operator-(const bigint &, const bigint &);
    friend bigint operator*(const bigint &, const bigint &);

private:
    // Private members

    // Store the number as a vector of 8-bit unsigned integers.
    std::vector<uint8_t> number;

    // boolean is true if the bigint is negative, otherwise false.
    bool isNeg;
};

// DOUBLE CHECK I CAN DO THIS
// Using the Operator Overload from the notes
/**
 * @brief Overload the << operator for bigints. Returns a std::ostream of the operand.
 *
 * @param out
 * @param operand
 * @return std::ostream&
 */
std::ostream &operator<<(std::ostream &out, const bigint &operand)
{
    if (operand.isNeg)
    {
        out << "-";
    }
    const std::vector<uint8_t> vec = operand.number;
    for (size_t i = 0; i < operand.number.size() - 1; i++)
        out << unsigned(vec.at(i));
    // https://stackoverflow.com/questions/19562103/uint8-t-cant-be-printed-with-cout
    out << unsigned(operand.number.at(operand.number.size() - 1));

    return out;
}

/**
 * @brief Overload the == operator for bigints. Returns true if operand1 == operand2, otherwise false.
 *
 * @param operand1
 * @param operand2
 * @return true
 * @return false
 */
bool operator==(const bigint &operand1, const bigint &operand2)
{
    // Optimization: return false if operands do not have the same number of digits
    if (operand1.number.size() != operand2.number.size())
    {
        return false;
    }
    // Optimization: return false if operands have different signs
    if (operand1.isNeg != operand2.isNeg)
    {
        return false;
    }
    // check if digits in operands are the same
    for (size_t i = 0; i < operand1.number.size(); i++)
    {
        if (operand1.number.at(i) != operand2.number.at(i))
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief Overload the != operator for bigints. Return true if operand1 != operand2, otherwise false.
 *
 * @param operand1
 * @param operand2
 * @return true
 * @return false
 */
bool operator!=(const bigint &operand1, const bigint &operand2)
{
    //!= is logically equivalent to the negation of the == operator
    return !(operand1 == operand2);
}

/**
 * @brief Overload the < operator for bigints. Return true if operand1 < operand2, otherwise false.
 *
 * @param operand1
 * @param operand2
 * @return true
 * @return false
 */
bool operator<(const bigint &operand1, const bigint &operand2)
{
    // Optimization: return true if operand1 is negative and operand2 is positive.
    if (operand1.isNeg && !operand2.isNeg)
    {
        return true;
    }
    // Optimization: return false if operand1 is positive and operand2 is negative.
    if (!operand1.isNeg && operand2.isNeg)
    {
        return false;
    }
    if (!operand1.isNeg && !operand2.isNeg)
    {
        // Optimization: return true if both operands are positive
        // and operand1 has fewer digits than operand 2.
        if (operand1.number.size() < operand2.number.size())
        {
            return true;
        }
        // Optimization: return false if both operands are positive
        // and operand1 has more digits than operand 2.
        if (operand1.number.size() > operand2.number.size())
        {
            return false;
        }
        for (size_t i = 0; i < operand1.number.size() - 1; i++)
        {
            if (operand1.number.at(i) > operand2.number.at(i))
            {
                return false;
            }
            if (operand1.number.at(i) < operand2.number.at(i))
            {
                return true;
            }
        }
        if (operand1.number.at(operand1.number.size() - 1) >= operand2.number.at(operand1.number.size() - 1))
        {
            return false;
        }
        return true;
    }
    if (operand1.isNeg && operand2.isNeg)
    {
        // Optimization: return true if both operands are negative
        // and operand1 has more digits than operand 2.
        if (operand1.number.size() > operand2.number.size())
        {
            return true;
        }
        // Optimization: return false if both operands are negative
        // and operand1 has fewer digits than operand 2.
        if (operand1.number.size() < operand2.number.size())
        {
            return false;
        }
        for (size_t i = 0; i < operand1.number.size() - 1; i++)
        {
            if (operand1.number.at(i) < operand2.number.at(i))
            {
                return false;
            }
            if (operand1.number.at(i) > operand2.number.at(i))
            {
                return true;
            }
        }
        if (operand1.number.at(operand1.number.size() - 1) <= operand2.number.at(operand1.number.size() - 1))
        {
            return false;
        }
        return true;
    }

    // something went wrong
    // throw_exception
    std::cout << "BAD THINGS\n";
    return false;
}

// Operator Overload (<=)
/**
 * @brief Overload the <= operator for bigints. Returns true if operand1 <= operand2, otherwise false.
 *
 * @param operand1
 * @param operand2
 * @return true
 * @return false
 */
bool operator<=(const bigint &operand1, const bigint &operand2)
{
    return (operand1 < operand2 || operand1 == operand2);
}

/**
 * @brief Overload the > operator for bigints. Returns true if operand1 > operand2, otherwise false.
 *
 * @param operand1
 * @param operand2
 * @return true
 * @return false
 */
bool operator>(const bigint &operand1, const bigint &operand2)
{
    // a>b is logically equivalent to the negation of (a==b or a<b)
    return !(operand1 == operand2 || operand1 < operand2);
}

/**
 * @brief Overload the >= operator for bigints. Returns true if operand1 >= operand2, otherwise false.
 *
 * @param operand1
 * @param operand2
 * @return true
 * @return false
 */
bool operator>=(const bigint &operand1, const bigint &operand2)
{
    // Computationally less expensive to check just < than (> and ==)
    return !(operand1 < operand2);
}

/**
 * @brief Overload the - unary operator for bigints. Returns the negative of the operand.
 *
 * @param operand
 * @return bigint
 */
bigint operator-(bigint &operand)
{
    bigint zero;
    if(operand!=zero){
        operand.isNeg = !operand.isNeg;
    }
    return operand;
}

/**
 * @brief Overload the + operator for bigints. Returns the sum of operand1 and operand2.
 *
 * @param operand1
 * @param operand2
 * @return bigint
 */
bigint operator+(const bigint &operand1, const bigint &operand2)
{

    if (operand1.isNeg && !operand2.isNeg)
    {
        bigint temp = operand1;
        temp.isNeg = false;
        return (operand2 - temp);
    }
    if (!operand1.isNeg && operand2.isNeg)
    {
        bigint temp = operand2;
        temp.isNeg = false;
        return (operand1 - temp);
    }

    uint8_t carry = 0;
    std::vector<uint8_t> num1 = operand1.number;
    std::vector<uint8_t> num2 = operand2.number;
    while (num1.size() < num2.size())
    {
        num1.insert(num1.begin(), 0);
    }
    while (num1.size() > num2.size())
    {
        num2.insert(num2.begin(), 0);
    }

    std::string value = "";
    for (size_t i = num1.size(); i > 0; i--)
    {
        uint8_t x = num1.at(i - 1) + num2.at(i - 1) + carry;
        if (x >= 10)
        {
            x = x % 10;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        value += std::to_string(x);
    }
    if (carry == 1)
    {
        value += "1";
    }
    if (operand1.isNeg)
    {
        value += "-";
    }
    std::reverse(value.begin(), value.end());

    bigint result(value);
    return result;
}

/**
 * @brief Overload the += operator for bigints. Sets operand1 to the
 * sum operand1+operand2 and returns operand1.
 *
 * @param operand1
 * @param operand2
 * @return bigint
 */
bigint operator+=(bigint &operand1, const bigint &operand2)
{
    operand1 = operand1 + operand2;
    return operand1;
}

// Operator Overload (*)
/**
 * @brief Overload the * operator for bigints. Returns the product operand1*operand2.
 *
 * @param operand1
 * @param operand2
 * @return bigint
 */
bigint operator*(const bigint &operand1, const bigint &operand2)
{
    bigint result;
    size_t size1 = operand1.number.size();
    size_t size2 = operand2.number.size();
    for (size_t i = 0; i < size1; i++)
    {
        for (size_t j = 0; j < size2; j++)
        {
            uint8_t term1 = (operand1.number.at(i)); //* static_cast<int64_t>(pow(10, static_cast<double>(operand1.number.size() - i - 1))));
            uint8_t term2 = (operand2.number.at(j)); //* static_cast<int64_t>(pow(10, static_cast<double>(operand2.number.size() - j - 1))));
            std::string prefix = std::to_string(term1 * term2);
            // size_t limit = (size1 - i - 1) + (size2 - j - 1);
            // for(size_t k=0;k<limit;k++){
            //     prefix += "0";
            // }
            prefix += std::string((size1 - i - 1) + (size2 - j - 1), '0');
            bigint temp(prefix);
            result += temp;
        }
    }

    if (operand1.isNeg != operand2.isNeg && result != 0)
    {
        result.isNeg = true;
    }
    return result;
}

/**
 * @brief Overload the *= operator for bigints. Sets operand1 to the
 * product operand1*operand2 and returns operand1.
 *
 * @param operand1
 * @param operand2
 * @return bigint
 */
bigint operator*=(bigint &operand1, const bigint &operand2)
{
    operand1 = operand1 * operand2;
    return operand1;
}

// Operator Overload (-)
/**
 * @brief Overload the - binary operator for bigints. Returns the difference operand1-operand2.
 *
 * @param operand1
 * @param operand2
 * @return bigint
 */
bigint operator-(const bigint &operand1, const bigint &operand2)
{
    if (operand1.isNeg && !operand2.isNeg)
    {
        bigint temp = operand1;
        temp.isNeg = false;
        temp += operand2;
        return -temp;
    }
    if (!operand1.isNeg && operand2.isNeg)
    {
        bigint temp = operand2;
        temp.isNeg = false;
        return operand1 + temp;
    }
    if (operand1.isNeg && operand2.isNeg)
    {
        bigint temp1 = operand1;
        bigint temp2 = operand2;
        temp1.isNeg = false;
        temp2.isNeg = false;
        return temp2 - temp1;
    }

    std::vector<uint8_t> num1 = operand1.number;
    std::vector<uint8_t> num2 = operand2.number;
    while (num1.size() < num2.size())
    {
        num1.insert(num1.begin(), 0);
    }
    while (num1.size() > num2.size())
    {
        num2.insert(num2.begin(), 0);
    }

    std::string value = "";
    if (operand1 >= operand2)
    {
        for (size_t i = num1.size(); i > 0; i--)
        {
            if (num1.at(i - 1) < num2.at(i - 1))
            {
                num1.at(i - 1) += 10;
                num1.at(i - 2) -= 1;
                bool steal = (num1.at(i - 2) == 255);
                size_t index = 2;
                while (steal)
                {
                    num1.at(i - index) = num1.at(i - index) + 10;
                    index += 1;
                    int8_t temp = num1.at(i - index) - 1;
                    num1.at(i - index) = static_cast<uint8_t>(temp % 10);
                    steal = (temp < 0);
                }
            }
            uint8_t x = num1.at(i - 1) - num2.at(i - 1);
            value += std::to_string(x);
        }

        while (value.at(value.length() - 1) == '0' && value.length() > 1)
        {
            value = value.substr(0, value.length() - 1);
        }
    }
    else
    {
        bigint result = operand2 - operand1;
        return -result;
    }

    std::reverse(value.begin(), value.end());

    bigint result(value);
    return result;
}

/**
 * @brief Overload the += operator for bigints. Sets operand1 to the
 * difference operand1-operand2 and returns operand1.
 *
 * @param operand1
 * @param operand2
 * @return bigint
 */
bigint operator-=(bigint &operand1, const bigint &operand2)
{
    operand1 = operand1 - operand2;
    return operand1;
}

/**
 * @brief Overload the ++ postfix operator for bigints. Increment operand1 by one
 * and return original value of operand1
 *
 * @param operand1
 * @return bigint
 */
bigint operator++(bigint &operand1, int)
{
    bigint result = operand1;
    operand1 += 1;
    return result;
}

/**
 * @brief Overload the ++ prefix operator for bigints. Increment operand1 by one
 * and return operand1
 *
 * @param operand1
 * @return bigint
 */
bigint operator++(bigint &operand1)
{
    operand1 += 1;
    return operand1;
}

/**
 * @brief Overload the -- postfix operator for bigints. Decrement operand1 by one
 * and return original value of operand1
 *
 * @param operand1
 * @return bigint
 */
bigint operator--(bigint &operand1, int)
{
    bigint result = operand1;
    operand1 -= 1;
    return result;
}

/**
 * @brief Overload the ++ postfix operator for bigints. Decrement operand1 by one
 * and return operand1
 *
 * @param operand1
 * @return bigint
 */
bigint operator--(bigint &operand1)
{
    operand1 -= 1;
    return operand1;
}