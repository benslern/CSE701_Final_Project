#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <stdio.h>

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
        while (value >= 1)
        {
            number.push_back(static_cast<uint8_t>(value % 10));
            value /= 10;
        }
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

    std::vector<uint8_t> getNumber() const
    {
        return number;
    }

    bool getIsNeg() const
    {
        return isNeg;
    }

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
    if (operand.getIsNeg())
    {
        out << "-";
    }
    const std::vector<uint8_t> vec = operand.getNumber();
    for (size_t i = 0; i < vec.size() - 1; i++)
        out << unsigned(vec[i]);
    // https://stackoverflow.com/questions/19562103/uint8-t-cant-be-printed-with-cout
    out << unsigned(vec[vec.size() - 1]);
    return out;
}

// Operator Overload (+)
// bigint operator+(bigint &operand1, bigint &operand2)
//{
//    // Function body
//    bigint result;
//    return result;
//}
