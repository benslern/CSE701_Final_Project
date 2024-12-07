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

    // Default Constructor
    bigint()
    {
        number.push_back(0);
        isNeg = false;
    }

    // Signed 64-bit Integer Constructor
    bigint(int64_t value)
    {
        isNeg = (value < 0);
        value = abs(value);
        do
        {
            number.push_back(static_cast<uint8_t>(value % 10));
            value /= 10;
        } while (value >= 1);

        // https: // www.reddit.com/r/cpp/comments/gctyw5/adding_two_uint16_t_s_without_wconversion_warning/
        // https://stackoverflow.com/questions/8877448/how-do-i-reverse-a-c-vector
        std::reverse(number.begin(), number.end());
    }

    // String Constructor
    bigint(std::string value)
    {
        try
        {
            isNeg = false;
            if (stoi(value) < 0)
            {
                isNeg = true;
                value.erase(0, 1);
            }
            for (size_t i = 0; i < value.length(); i++)
            {
                number.push_back(static_cast<uint8_t>(value[i] - '0'));
            }
            // https://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
        }
        catch (const std::invalid_argument &e)
        {
            throw std::invalid_argument("Error: Invalid String - (" + value + ")\n");
        }
    }

    friend std::ostream &operator<<(std::ostream &, const bigint &);
    friend bool operator==(const bigint &, const bigint &);
    friend bool operator<(const bigint &, const bigint &);
    friend bigint operator-(bigint &operand);
    friend bigint operator+(const bigint &, const bigint &);
    friend bigint operator-(const bigint &, const bigint &);
    friend bigint operator*(const bigint &, const bigint &);

private:
    // Private members

    // Store the number as a vector of 8-bit unsigned integers
    std::vector<uint8_t> number;
    bool isNeg;
};

// Operator Overload (<<)
// DOUBLE CHECK I CAN DO THIS
// Using the Operator Overload from the notes
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

// Operator Overload (==)
bool operator==(const bigint &operand1, const bigint &operand2)
{
    if (operand1.number.size() != operand2.number.size())
    {
        return false;
    }
    if (operand1.isNeg != operand2.isNeg)
    {
        return false;
    }
    for (size_t i = 0; i < operand1.number.size(); i++)
    {
        if (operand1.number.at(i) != operand2.number.at(i))
        {
            return false;
        }
    }
    return true;
}

bool operator!=(const bigint &operand1, const bigint &operand2)
{
    return !(operand1 == operand2);
}

bool operator<(const bigint &operand1, const bigint &operand2)
{
    if (operand1.isNeg && !operand2.isNeg)
    {
        return true;
    }
    if (!operand1.isNeg && operand2.isNeg)
    {
        return false;
    }
    if (!operand1.isNeg && !operand2.isNeg)
    {
        if (operand1.number.size() < operand2.number.size())
        {
            return true;
        }
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
        if (operand1.number.size() > operand2.number.size())
        {
            return true;
        }
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
bool operator<=(const bigint &operand1, const bigint &operand2)
{
    return (operand1 < operand2 || operand1 == operand2);
}

// Operator Overload (>)
bool operator>(const bigint &operand1, const bigint &operand2)
{
    return !(operand1 == operand2 || operand1 < operand2);
}

// Operator Overload (>=)
bool operator>=(const bigint &operand1, const bigint &operand2)
{
    return !(operand1 < operand2);
}

// Operator Overload (-)
bigint operator-(bigint &operand)
{
    operand.isNeg = !operand.isNeg;
    return operand;
}

// Operator Overload (+)
bigint operator+(const bigint &operand1, const bigint &operand2)
{

    if (operand1.isNeg && !operand2.isNeg)
    {
        bigint temp = operand1;
        temp.isNeg = false;
        return operand2 - operand1;
    }
    if (!operand1.isNeg && operand2.isNeg)
    {
        bigint temp = operand2;
        temp.isNeg = false;
        return operand1 - operand2;
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

bigint operator+=(bigint &operand1, const bigint &operand2)
{
    operand1 = operand1 + operand2;
    return operand1;
}

// Operator Overload (*)
bigint operator*(const bigint &operand1, const bigint &operand2)
{
    bigint result;
    for (size_t i = 0; i < operand1.number.size(); i++)
    {
        for (size_t j = 0; j < operand2.number.size(); j++)
        {
            // std::cout << i << " " << j << "\n";
            int64_t term1 = (operand1.number.at(i) * static_cast<int64_t>(pow(10, static_cast<double>(operand1.number.size() - i - 1))));
            int64_t term2 = (operand2.number.at(j) * static_cast<int64_t>(pow(10, static_cast<double>(operand2.number.size() - j - 1))));
            bigint temp(term1 * term2);
            // std::cout << temp << "\n";
            result += temp;
        }
    }

    if(operand1.isNeg != operand2.isNeg){
        result.isNeg = true;
    }
    return result;
}

// Operator Overload (-)
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
                num1.at(i - 2) = (num1.at(i - 2) - 1) % 10;
            }
            uint8_t x = num1.at(i - 1) - num2.at(i - 1);
            value += std::to_string(x);
        }

        while (value.at(value.length() - 1) == '0' && value.length() > 1)
        {
            value = value.substr(value.length() - 1, 1);
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

bigint operator-=(bigint &operand1, const bigint &operand2)
{
    operand1 = operand1 - operand2;
    return operand1;
}

bigint operator++(bigint &operand1, int)
{
    bigint result = operand1;
    operand1 += 1;
    return result;
}

bigint operator++(bigint &operand1)
{
    operand1 += 1;
    return operand1;
}

bigint operator--(bigint &operand1, int)
{
    bigint result = operand1;
    operand1 -= 1;
    return result;
}

bigint operator--(bigint &operand1)
{
    operand1 -= 1;
    return operand1;
}