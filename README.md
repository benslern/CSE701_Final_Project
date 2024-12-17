# CSE 701 - Final Project  
  
### Noah Bensler - 400611927  

## Overview
  
[Introduction](#Introduction)
[Implementation](#class-implementation)


## Introduction
  
This is the documentation for the Fall 2024 CSE701 final project. It is a C++ implementation of unsigned arbitrary-precision integers, bigint. It allows for the creation of integers with an arbitrarily large number of digits, and has operator overloading for the operators defined by the project outline, 
  
- Addition (`+` and `+=`)  
  
- Subtraction (`-` and `-=`)  
  
- Multiplication (`*` and `*=`)  
  
- Negation (unary `-`)  
  
- Comparison (`==`, `!=`, `<`, `>`, `<=`, and `>=`)  
  
- Insertion (`<<`, to print the integer to a stream such as `std::cout` or a file)  
  
- Increment (`++`, both pre-increment and post-increment)  
  
- Decrement (`--`, both pre-decrement and post-decrement)  
  
## Class Implementation  
  
The bigint class contains two private members, `number` and `isNeg`. The `number` variable is a vector of unsigned 8 bit integers,  
  
`std::vector<uint8_t> number;`  
  
These integers are used to represent the base 10 digits of the bigint. The value at `number[0]` is the largest digit in the bigint, and the value at `number[number.size()-1]` is the smallest. For example, the number vector for `bigint test(1276);` would be,  

| index | value |
|--|--|
| 0 | 1 |
| 1 | 2 |
| 2 | 7 |
| 3 | 6 |

The class invariant for this variable is that every digit of the number vector must be between 0 and 9. The `isNeg` variable is a boolean that represents the sign of the bigint.
  
`bool isNeg;`  

That class invariant of this variable is that it is true if and only if the bigint is negative.
  
## Constructors 
  
The bigint class has three constructors. The default constructor, the int64_t constructor, and the std::string constructor. 
  
### The Default Constructor  
  
**Declaration:**`bigint()`  
  
**Brief:** Default Constructor. Construct a new bigint object with value 0.  
  
The default constructor takes no arguments and creates a new bigint object with a value of 0. As this constructor takes no arguments, there are no edge cases or inputs of interest.  
  
### The int64_t Constructor  
  
**Declaration:**`bigint(int64_t value)`  
  
**Brief:** Signed 64-bit Integer Constructor. Construct a new bigint object with a int64_t value.  
  
**Param:** int64_t value  
  
The int64_t constructor takes an int64_t argument and creates a new bigint object with the value of the int64_t argument.  Because the argument is a int64_t integer, the constructor has several edge cases and inputs of interest.
  
```  
bigint input_of_interest_1(INT64_MAX);  
bigint input_of_interest_2(INT64_MIN);  
```  
These inputs will create bigints with the values `9223372036854775807` and `-9223372036854775808` respectively. These inputs are significant because they are the largest and smallest integers that can be stored in the int64_t argument. Any user that wants to use the int64_t constructor for bigint must ensure that the value is between these limits. If the user attempts to pass an argument outside of these bounds it will cause integer overflow and generate a bigint with an inaccurate value. It will also produce a warning. As such, integer arguments outside of this range should be avoided.  
  
The constructor also allows to create a bigint using scientific notation provided that the input value is within the range INT64_MIN to INT64_MAX.  For example,

`bigint input_of_interest_3(1.234E8);`
  
The bigint will have the value `123400000`. This is possible because C++ will automatically convert the scientific notation input to a int64_t value that the constructor requires.  However, this type of input has limitations. If the number of significant digits is greater than the exponent, then C++ will automatically truncate it to a integer value. For example,

`bigint input_of_interest_4(1.234E2)`

This input will create a bigint with a value `123` even though the actual value of the input is `123.4`.  This type of implicit conversion will generate a warning and so should be avoided.
  
The final input of interest is,  

`bigint input_of_interest_5(-0);`  

This bigint will have the value `0`. This input will not generate any warnings. 
  
### The String Constructor  
  
The string constructor takes an string argument and creates a new bigint object with the value of the string argument.  Because the argument is a string, the constructor has several edge cases and inputs of interest. The string can contain an integer with an arbitrary number of digits. This allows the string constructor to create bigints with values far larger than the limited range of the int64_t constructor. For example,

`bigint input_of_interest_1("123456789098765432110293847566574839201");`

This input will create a bigint with the value, `123456789098765432110293847566574839201`, which is many orders of magnitude larger than INT64_MAX. The constructor allows for the string to have leading and tailing spaces, and leading zeros. For example,

`bigint input_of_interest_2(" -001   ");`
`bigint input_of_interest_3("-0000");`
  
This input will create a bigints with the values `-1` and `0`. However, the string cannot contain any symbols apart from a single '-' at the beginning to indicate a negative integer.  For example, the following inputs will generate the error, 'Error: Invalid String'

```
bigint input_of_interest_4("100.1234");
bigint input_of_interest_5("1234+5678");
bigint input_of_interest_6("($#@(*&%");
bigint input_of_interest_7("1234 5678");
```
  
The string also cannot contain any letters, including scientific notation. Unlike the int64_t constructor, C++ does not automatically convert strings of scientific notation to integers. The implementation of a custom handling of this input type was outside the scope of this project. Finally, the string cannot be empty. The following inputs of interest will also generate the error, "Error: Invalid String".

```
bigint input_of_interest_8("one hundred and five");`
bigint input_of_interest_9("flkadjsf");
bigint input_of_interest_10("1.234E5");
bigint input_of_interest_11("--1234");
bigint input_of_interest_12("");
```
  
## Operators

The following sections describe the algorithms used to override the basic mathematical operators.
  
### + Operator Overload  
  
**Declaration:**`bigint operator+(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the + operator for bigints. Returns the sum operand1 + operand2.  

**Param:** bigint operand1  
  
**Param:** bigint operand2  

**Return:** bigint  
  
In order to simplify the addition algorithm, the operator overload only computes the sum of two bigints if both of those bigints are positive or both negative. If the operands have different signs, then they are handled by converting them to mathematically equivalent equations using the - operator.

* operand1 is negative and operand2 is positive. The function returns the value operand2-(-operand1). 
* operand1 is positive and operand2 is negative. The function returns the value operand1-(-operand2).

When both bigints are both positive or negative, the function uses the following algorithm to calculate the sum.

1) If the numbers have different numbers of digits, then the algorithm pads the smaller number with leading zeros. 
```
while (num1.size() <  num2.size())
{
	num1.insert(num1.begin(), 0);
}

while (num1.size() >  num2.size())
{
	num2.insert(num2.begin(), 0);
}
```

By doing this, the algorithm ensures the digits of the numbers are index aligned with digits of the same order. For example, given the numbers `123` and `56`. If the algorithm accessed the digit at `number[1]`, the digit of the first number would represent a value of 20, whereas the digit of the second number would represent a value of 6. By inserting a leading zero to change the second bigint vector to `056`, the digit at `number[1]` now becomes 50, and so is of equivalent order. This significantly simplifies the sum of each digit.

2) Set the carry over value to zero. Starting at the largest index, and looping backwards through the number vector, calculate the sum of the digits.
`uint8_t x = num1.at(i-1) + num2.at(i-1) + carry;`
If the sum is greater than or equal to 10, set x to,
`x = x%10;`

preserves the class invariant that every digit of the number vector must be between 0 and 9.
FULL DESCRIPTION OF THE ALGORITHM  
  
  
### += Operator Overload  
  
  
  
**Declaration:**`bigint operator+=(bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the += operator for bigints. Sets operand1 to the sum operand1+operand2 and returns operand1.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint  
  
  
  
The operator takes to two reference operands as input. Only the second operator is constant.  
  
The += operator overload makes use of the + operator to avoid rewriting code. It functions by setting operand1 equal to the sum of operand1 and operand2, and then returns operand1.  
  
  
  
### - Operator Overload  
  
**Declaration:** `bigint operator-(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the - binary operator for bigints. Returns the difference operand1-operand2.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint  
  
  
  
FULL DESCRIPTION OF ALGORITHM  
  
To simplify the code the - operator overload only computes  
  
the difference operand1-operand2 when operand1 and operand2  
  
are both positive and operand1>=operand2. When one or both is  
  
negative it is simpler to convert the difference to a  
  
mathematically equivalent form in which they are both positive.  
  
When operand2>operand1 it is simpler to return -(operand2-operand1).  
  
  
  
### -= Operator Overload  
  
**Declaration:**`bigint operator-=(bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the -= operator for bigints. Sets operand1 to the difference operand1-operand2 and returns operand1.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint  
  
  
  
FULL DESCRIPTION OF ALGORITHM  
  
  
  
### * Operator Overload  
  
**Declaration:**`bigint operator*(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the * operator for bigints. Returns the product operand1*operand2.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint  
  
  
  
FULL DESCRIPTION OF ALGORITHM  
  
  
  
### *= Operator Overload  
  
**Declaration:**`bigint operator*=(bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the *= operator for bigints. Sets operand1 to the product operand1*operand2 and returns operand1.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint  
  
  
  
FULL DESCRIPTION OF ALGORITHM  
  
  
  
### - Operator Overload  
  
**Declaration:**`bigint operator-(bigint &operand)`  
  
**Brief:** Overload the - unary operator for bigints. Returns the negative of operand.  
  
**Param:** bigint operand  
  
**Return:** bigint  
  
  
  
FULL DESCRIPTION OF ALGORITHM  
  
  
  
### == Operator Overload  
  
**Declaration:**`bool operator==(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the `==` operator for bigints. Returns true if operand1 == operand2, otherwise false.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** true  
  
**Return:** false  
  
The == operator makes use of a few optimizations. 
* If the operands have a different number of digits, return false.
* If the operands have different isNeg values, return false.

If the operands have the same number of digits and have the same isNeg values, then the algorithm simply loops through the digits of the operand number vectors and checks if the digits are the same. If it finds digits that do not match, it returns false, otherwise it returns true.
  
### != Operator Overload  
  
**Declaration:**`bool operator!=(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the != operator for bigints. Return true if operand1 != operand2, otherwise false.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** true  
  
**Return:** false  
  
The a!=b condition is logically equivalent to !(a==b). Therefore, instead of writing unnecessary code, the function simply returns the output, `return !(operand1==operand2)`.
  
FULL DESCRIPTION OF ALGORITHM  
  
  
  
### < Operator Overload  
  
**Declaration:**`bool operator<(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the < operator for bigints. Return true if operand1 < operand2, otherwise false.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** true  
  
**Return:** false  
  
The < operator makes use of several optimizations to speed up the algorithm. 
* If operand1 is negative and operand2 is positive, then operand1<operand2 will always be true, and so the function returns true without wasting time computing the result. 
* If operand1 is positive and operand2 is negative, then operand1<operand2 will always be false, and so the function returns false without wasting time computing the result.
* If operand1 and operand2 are both negative then return the logically equivalent form (-operand2)<(-operand1).
* If operand1 and operand2 are both positive, and operand1 has fewer digits than operand2, then operand1<operand2 is always true, and so the function returns true without computing the result.
* If operand1 and operand2 are both positive, and operand1 has more digits than operand2, then operand1<operand2 is always false, and so the function returns false without computing the result.

Finally, if operand1 and operand2 are both positive, and they have the same number of digits, the condition is computed. The algorithm operates by iteration through the digits of the bigint and checks each corresponding digits from operand1 and operand2. If it finds digits such that the digit of operand1 is greater than the digit of operand2, it returns false. If it finds digits such that the digit of operand1 is less than the digit of operand2, it returns true. For the last digit of the vectors, if the digit of operand one is greater than or equal to the digit of operand2, it returns false because this would mean that operand2>=operand1 is true. Otherwise, it returns true.
  
### <= Operator Overload  
  
**Declaration:**`bool operator<=(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the <= operator for bigints. Returns true if operand1 <= operand2, otherwise false.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** true  
  
**Return:** false  
  
The condition a<=b is logically equivalent to (a<b || a==b). Therefore, instead of writing unnecessary code, the function simply returns the output, `return (operand1<operand2 || operand1==operand2);`.

### > Operator Overload  
  
**Declaration:**`bool operator>(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the > operator for bigints. Returns true if operand1 > operand2, otherwise false.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** true  
  
**Return:** false  
  
The condition a>b is logically equivalent to !(a==b or a<b). Therefore, instead of writing unnecessary code, the function simply returns the output, `return  !(operand1==operand2 || operand1<operand2);`
  
### >= Operator Overload  
  
**Declaration:**`bool operator>=(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the >= operator for bigints. Returns true if operand1 >= operand2, otherwise false.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** true  
  
**Return:** false  
  
The condition a>=b is logically equivalent to !(a<b). Additionally, it is less computationally less expensive to evaluate !(a<b). Therefore, this operator simply uses the following output, `return !(operand1 < operand2);`
 
### << Operator Overload  
  
**Declaration:**`std::ostream &operator<<(std::ostream &out, const bigint &operand)`  
  
**Brief:** Overload the << operator for bigints. Returns a std::ostream of the operand.  
  
**Param:** std::ostream out  
  
**Param:** bigint operand  
  
**Return:** std::ostream  
  
This Operator Overload was based on the << operator overload example from section 7.1.2 of the CSE701 Lecture notes. In order to add the uint8_t values from the bigint number to the ostream it must use the unsigned() function otherwise they will simply be blank.
https://stackoverflow.com/questions/19562103/uint8-t-cant-be-printed-with-cout
  
If the operand is negative, the function adds a '-' to the out stream. Then the function loops through the operand number vector and adds each digit to the stream. Finally, it returns the stream, out.
  
### ++ Pre Operator Overload  
  
**Declaration:**`bigint operator++(bigint &operand1)`  
  
**Brief:** Overload the ++ prefix operator for bigints. Increment operand by one and return operand.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint  

Because the addition operator was only overloaded for two bigints, it is not possible to increment the operand using `operand += 1;`, as 1 is an integer not a bigint. Therefore, the function defines a new bigint, `bigint one(1);`. The function then decrements operand using `operand += one;`. Finally, it returns the updated value, operand.
  
### ++ Post Operator Overload  
  
**Declaration:**`bigint operator++(bigint &operand1, int)`  
  
**Brief:** Overload the ++ postfix operator for bigints. Increment operand by one and return the original value of operand.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint  
  
The algorithm for the ++ postfix operator begins by creating a copy of the operand, `biging result = operand;`. Because the addition operator was only overloaded for two bigints, it is not possible to increment the operand using `operand += 1;`, as 1 is an integer not a bigint. Therefore, the function defines a new bigint, `bigint one(1);`. The function then decrements operand using `operand += one;`. Finally, it returns the original value, result.   
  
### -- Pre Operator Overload  
  
**Declaration:**`bigint operator--(bigint &operand1)`  
  
**Brief:** Overload the -- prefix operator for bigints. Decrement operand by one and return operand.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint  
  
Because the subtraction operator was only overloaded for two bigints, it is not possible to decrement the operand using `operand -= 1;`, as 1 is an integer not a bigint. Therefore, the function defines a new bigint, `bigint one(1);`. The function then decrements operand using `operand -= one;`. Finally, it returns the updated value, operand.
  
FULL DESCRIPTION OF ALGORITHM  
  
  
  
### -- Post Operator Overload  
  
**Declaration:**`bigint operator--(bigint &operand1,int)`  
  
**Brief:** Overload the -- postfix operator for bigints. Decrement operand by one and return the original value of operand.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint  
  
The algorithm for the -- postfix operator begins by creating a copy of the operand, `biging result = operand;`. Because the subtraction operator was only overloaded for two bigints, it is not possible to decrement the operand using `operand -= 1;`, as 1 is an integer not a bigint. Therefore, the function defines a new bigint, `bigint one(1);`. The function then decrements operand using `operand -= one;`. Finally, it returns the original value, result.   
  
## Testing  
  
The bigint class and associated operators are tested in three ways. Constructor tests, random operator tests, and custom operator tests. The goal is to conduct comprehensive tests on each component of the bigint implementation.  
  
 ### Constructor Tests 
  The constructor tests are used to ensure the constructors work for a selection of edge cases.  
  
  
### Random Operator Tests  
  
The random operator tests  
The final test files submitted with the project used bigints with random values between 50 digits and 200 digits. This was done to speed up the tests, particularly the multiplication tests which are quite slow for large values. However, the code was stress tested for random values between 500 to 3000 digits. It successfully passed all of the tests.  
  
The random tests generate 30 tests for each possible combination of positive or negative signs. For example, the random addition tests have  
  
### Custom Operator Tests  
  
The custom operator tests function in the same way as the random operator tests, except that the values in the test files were not randomly generated. These values were selected because they represent edge cases and values that caused errors during the development process.  
  
Some specific examples  
stress test the `+`, `+=`, `-`, and `-=` operators for bigints with 20,000 digits. This was done by using the first 20,000 digits of pi which are available online, [http://www.eveandersson.com/pi/digits/pi-digits?n_decimals_to_display=20000&breakpoint=100](http://www.eveandersson.com/pi/digits/pi-digits?n_decimals_to_display=20000&breakpoint=100).