#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <stdio.h>
#include <cmath>

/**
 * @brief Implementation of the bigint class. It allows for the creation and 
 * basic mathematical operations on arbitrary-precision integers.
 */
class bigint
{
public:
    // Public members

    /**
     * @brief Default Constructor. Construct a new bigint object with value of 0.
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
    // These operators must be friends because they need direct
    // access to the bigint private members
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
 * @brief Overload the << operator for bigints. 
 * Returns a std::ostream of the operand.
 *
 * @param std::ostream out
 * @param bigint operand
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
 * @brief Overload the == operator for bigints. 
 * Returns true if operand1 == operand2, otherwise false.
 *
 * @param bigint operand1
 * @param bigint operand2
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
 * @brief Overload the != operator for bigints. 
 * Return true if operand1 != operand2, otherwise false.
 *
 * @param bigint operand1
 * @param bigint operand2
 * @return true
 * @return false
 */
bool operator!=(const bigint &operand1, const bigint &operand2)
{
    //!= is logically equivalent to the negation of the == operator
    return !(operand1 == operand2);
}

/**
 * @brief Overload the < operator for bigints. 
 * Return true if operand1 < operand2, otherwise false.
 *
 * @param bigint operand1
 * @param bigint operand2
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
    if (operand1.isNeg && operand2.isNeg)
    {
        //convert ((neg_op1)<(neg_op2)) to ((pos_op2)<(pos_op1))
        bigint temp1 = operand1;
        bigint temp2 = operand2;
        temp1.isNeg = false;
        temp2.isNeg = false;
        return temp2<temp1;
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
        //compute operand1<operand2 if both are positive and 
        //they have the same number of digits
        for (size_t i = 0; i < operand1.number.size() - 1; i++)
        {
            if (operand1.number.at(i) > operand2.number.at(i))
            {
                //return false if any digit of operand1 is > than the corresponding digit in operand2
                return false;
            }
            if (operand1.number.at(i) < operand2.number.at(i))
            {
                //return true if any digit of operand1 is > than the corresponding digit in operand2
                return true;
            }
        }
        //if all digits digits except the last are the same
        //return false if the last digit of operand1 is >= the last digit of operand2
        if (operand1.number.at(operand1.number.size() - 1) >= operand2.number.at(operand1.number.size() - 1))
        {
            return false;
        }
        return true;
    } 

    // this should never be called
    throw std::invalid_argument("Error - Invalid Operands");
    return false;
}

/**
 * @brief Overload the <= operator for bigints. 
 * Returns true if operand1 <= operand2, otherwise false.
 *
 * @param bigint operand1
 * @param bigint operand2
 * @return true
 * @return false
 */
bool operator<=(const bigint &operand1, const bigint &operand2)
{
    return (operand1 < operand2 || operand1 == operand2);
}

/**
 * @brief Overload the > operator for bigints. 
 * Returns true if operand1 > operand2, otherwise false.
 *
 * @param bigint operand1
 * @param bigint operand2
 * @return true
 * @return false
 */
bool operator>(const bigint &operand1, const bigint &operand2)
{
    // a>b is logically equivalent to the negation of (a==b or a<b)
    return !(operand1 == operand2 || operand1 < operand2);
}

/**
 * @brief Overload the >= operator for bigints. 
 * Returns true if operand1 >= operand2, otherwise false.
 *
 * @param bigint operand1
 * @param bigint operand2
 * @return true
 * @return false
 */
bool operator>=(const bigint &operand1, const bigint &operand2)
{
    // logically equivalent and computationally less expensive 
    // to check just the negation of < than (> and ==)
    return !(operand1 < operand2);
}

/**
 * @brief Overload the - unary operator for bigints. 
 * Returns the negative of operand.
 *
 * @param bigint operand
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
 * @brief Overload the + operator for bigints. 
 * Returns the sum operand1+operand2.
 *
 * @param bigint operand1
 * @param bigint operand2
 * @return bigint
 */
bigint operator+(const bigint &operand1, const bigint &operand2)
{

    if (operand1.isNeg && !operand2.isNeg)
    {
        //convert ((neg_op1)+(pos_op2)) to ((pos_op2)-(pos_op1))
        bigint temp = operand1;
        temp.isNeg = false;
        return (operand2 - temp);
    }
    if (!operand1.isNeg && operand2.isNeg)
    {
        //convert ((pos_op1)+(neg_op2)) to ((pos_op1)-(neg_op2))
        bigint temp = operand2;
        temp.isNeg = false;
        return (operand1 - temp);
    }

    uint8_t carry = 0;
    std::vector<uint8_t> num1 = operand1.number;
    std::vector<uint8_t> num2 = operand2.number;
    //pad with zeros to make the same length
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
        //add digits and carry 1 if necessary
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
    
    if (operand1.isNeg && operand2.isNeg)
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
 * @param bigint operand1
 * @param bigint operand2
 * @return bigint
 */
bigint operator+=(bigint &operand1, const bigint &operand2)
{
    operand1 = operand1 + operand2;
    return operand1;
}

// Operator Overload (*)
/**
 * @brief Overload the * operator for bigints. 
 * Returns the product operand1*operand2.
 *
 * @param bigint operand1
 * @param bigint operand2
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
            uint8_t term1 = (operand1.number.at(i)); 
            uint8_t term2 = (operand2.number.at(j)); 
            
            //calculate digit product
            std::string prefix = std::to_string(term1 * term2);

            //add zeros given digit position
            prefix += std::string((size1 - i - 1) + (size2 - j - 1), '0');
            
            //create temp bigint using string constructor
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
 * @param bigint operand1
 * @param bigint operand2
 * @return bigint
 */
bigint operator*=(bigint &operand1, const bigint &operand2)
{
    operand1 = operand1 * operand2;
    return operand1;
}

/**
 * @brief Overload the - binary operator for bigints. 
 * Returns the difference operand1-operand2.
 *
 * @param bigint operand1
 * @param bigint operand2
 * @return bigint
 */
bigint operator-(const bigint &operand1, const bigint &operand2)
{
    //if either operand is negative, convert to a mathematically 
    //equivalent form where both operands are positive
    if (operand1.isNeg && !operand2.isNeg)
    {
        //convert (neg_op1)-(pos_op2) to -((pos_op1)+(pos_op2))
        bigint temp = operand1;
        temp.isNeg = false;
        temp += operand2;
        return -temp;
    }
    if (!operand1.isNeg && operand2.isNeg)
    {
        //convert ((pos_op1)-(neg_op2)) to ((pos_op1)+(pos_op2))
        bigint temp = operand2;
        temp.isNeg = false;
        return operand1 + temp;
    }
    if (operand1.isNeg && operand2.isNeg)
    {
        //convert ((neg_op1)-(neg_op2)) to ((pos_op2)-(pos_op1))
        bigint temp1 = operand1;
        bigint temp2 = operand2;
        temp1.isNeg = false;
        temp2.isNeg = false;
        return temp2 - temp1;
    }

    //Only compute difference when operand1 and operand2 are positive
    
    std::vector<uint8_t> num1 = operand1.number;
    std::vector<uint8_t> num2 = operand2.number;
    //pad the operand number vectors with leading zeros
    //so that they are the same length
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
        //loop through the padded numbers in reverse
        for (size_t i = num1.size(); i > 0; i--)
        {
            //compute the difference of each digit
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

            //add difference to value string
            value += std::to_string(x);
        }
        //in case value has leading zeros, remove them
        while (value.at(value.length() - 1) == '0' && value.length() > 1)
        {
            value = value.substr(0, value.length() - 1);
        }
    }
    else
    {
        //convert ((pos_op1)-(pos_op2)) to -((pos_op2)-(pos_op1)) when op2>op1
        bigint result = operand2 - operand1;
        return -result;
    }

    std::reverse(value.begin(), value.end());

    //Because value could be a string with any number of digits,
    //we must use the string operator to create bigint
    bigint result(value);
    return result;
}

/**
 * @brief Overload the -= operator for bigints. Sets operand1 to the
 * difference operand1-operand2 and returns operand1.
 *
 * @param bigint operand1
 * @param bigint operand2
 * @return bigint
 */
bigint operator-=(bigint &operand1, const bigint &operand2)
{
    operand1 = operand1 - operand2;
    return operand1;
}

/**
 * @brief Overload the ++ postfix operator for bigints. 
 * Increment operand by one and return the original value of operand.
 *
 * @param bigint operand
 * @param Dummy argument to distinguish it from the ++ prefix operator
 * @return bigint
 */
bigint operator++(bigint &operand, int)
{
    bigint result = operand;
    operand += 1;
    return result;
}

/**
 * @brief Overload the ++ prefix operator for bigints. 
 * Increment operand by one and return operand.
 *
 * @param bigint operand
 * @return bigint
 */
bigint operator++(bigint &operand)
{
    operand += 1;
    return operand;
}

/**
 * @brief Overload the -- postfix operator for bigints. 
 * Decrement operand by one and return the original value of operand.
 *
 * @param bigint operand
 * @param Dummy argument to distinguish it from the -- prefix operator
 * @return bigint
 */
bigint operator--(bigint &operand, int)
{
    bigint result = operand;
    operand -= 1;
    return result;
}

/**
 * @brief Overload the -- prefix operator for bigints. 
 * Decrement operand by one and return operand.
 *
 * @param bigint operand
 * @return bigint
 */
bigint operator--(bigint &operand)
{
    operand -= 1;
    return operand;
}