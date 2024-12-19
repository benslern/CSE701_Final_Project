# CSE 701 - Final Project  
  
### Noah Bensler - 400611927  

## Overview
  
* [Introduction](#introduction)
* [Implementation](#class-implementation)
* [Constructors](#constructors)
	* [The Default Constructor](#the-default-constructor)
	* [The Int64_t Constructor](#the-int64_t-constructor)
	* [The String Constructor](#the-string-constructor)
* [Operators](#operators)
     * [+  Operator Overload](#-operator-overload)
     * [+= Operator Overload](#-operator-overload-1)
     * [-  Operator Overload](#--operator-overload)
     * [-= Operator Overload](#--operator-overload-1)
     * [*  Operator Overload](#-operator-overload-2)
     * [*= Operator Overload](#-operator-overload-3)
     * [-  Operator Overload](#--operator-overload-2)
     * [== Operator Overload](#-operator-overload-4)
     * [!= Operator Overload](#-operator-overload-5)
     * [<  Operator Overload](#-operator-overload-6)
     * [<= Operator Overload](#-operator-overload-7)
     * [>  Operator Overload](#-operator-overload-8)
     * [>= Operator Overload](#-operator-overload-9)
     * [<< Operator Overload](#-operator-overload-10)
     * [++ Pre  Operator Overload](#-pre-operator-overload)
     * [++ Post Operator Overload](#-post-operator-overload)
     * [-- Pre  Operator Overload](#---pre-operator-overload)
     * [-- Post Operator Overload](#---post-operator-overload)
* [Testing](#testing)
     * [Constructor Tests](#constructor-tests)
     * [Random Operator Tests](#random-operator-tests)
     * [Custom Operator Tests](#custom-operator-tests)

## Introduction
  
This README is documentation for the Fall 2024 CSE701 final project. It is a C++ implementation of signed arbitrary-precision integers: bigint. It allows for the creation of signed integers with an arbitrarily large number of digits, and has operator overloading for the operators defined by the project outline, 
  
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
  
The integers in the vector are used to represent the base 10 digits of the bigint. The vector is sorted from the highest order digits to the lowest order digits. Therefore, the value at `number[0]` is the highest order digit in the bigint, and the value at `number[number.size()-1]` is the lowest order. For example, the `number` vector for a bigint with a value of `1276` would be,  

| index | value |
|--|--|
| 0 | 1 |
| 1 | 2 |
| 2 | 7 |
| 3 | 6 |

The class invariant for this variable is that every digit of the number vector must be between 0 and 9. The `isNeg` variable is a boolean that represents the sign of the bigint.
  
`bool isNeg;`  

The class invariant of this variable is that it is true if and only if the bigint is negative. The class also contains the declarations of several operator overloads that need to be friends of the bigint class. These operators must be friends because the algorithms they implement need access to the bigint private members. These operators are,

- Addition (`+`) 

- Subtraction (`-`)

- Multiplication (`*`)

- Negation (unary `-`)

- Comparison (`==`, and `<`)

- Insertion (`<<`)

Because these operators access the private members of bigint, the algorithms they implement must ensure that the class invariants are upheld. 

## Constructors 
  
The bigint class has three constructors: The default constructor, the int64_t constructor, and the std::string constructor. 
  
### The Default Constructor  
  
**Declaration:**`bigint()`  
  
**Brief:** Default Constructor. Construct a new bigint object with value 0.  
  
The default constructor takes no arguments and creates a new bigint object with a value of 0. As this constructor takes no arguments, there are no edge cases or inputs of interest. The vector `number` contains only the uint8_t 0. Therefore, the class invariant for `number` is upheld. The `isNeg` variable is set to false. Therefore, the class invariant for `isNeg` is also upheld.
  
### The int64_t Constructor  
  
**Declaration:**`bigint(int64_t value)`  
  
**Brief:** Signed 64-bit Integer Constructor. Construct a new bigint object with a int64_t value.  
  
**Param:** int64_t value  
  
The int64_t constructor takes an int64_t argument and creates a new bigint with the value of the int64_t argument. The int64_t constructor implements the following algorithm. 

1) Set the `isNeg` variable to the condition `value<0`. This ensures that `isNeg` is only true when the argument is negative and so the class invariant for `isNeg` is upheld.

2) push_back the uint8_t integer `abs(value % 10)` to the `number` vector. This equation represents the lowest order digit of `value`. Because of the modulo 10, the integer being added to `number` can only be in the range 0 to 9, and so the class invariant for `number` is upheld.

3) Update `value` using `value /= 10`. This step performs integer division on value and so truncates the smallest order digit from `value` while reducing the order of the remaining digits.

4) Repeat steps 2 and 3 while `abs(value) >= 1`

5) Reverse `number`. This ensures the digits of the bigint are in the correct order.

Because the argument for this constructor is a int64_t integer, the constructor has several edge cases and inputs of interest. For example,
  
```  
bigint input_of_interest_1(INT64_MAX);  
bigint input_of_interest_2(INT64_MIN);  
```  

These inputs will create bigints with the values `9223372036854775807` and `-9223372036854775808` respectively. These inputs are significant because they are the largest and smallest integers that can be stored in the int64_t argument. Any user that wants to use the int64_t constructor for bigint must ensure that the value is between these limits. If the user attempts to pass an argument outside of these bounds it will cause integer overflow and generate a bigint with an inaccurate value. Attempting this will also produce a warning. As such, integer arguments outside of this range should be avoided. The constructor also allows to create a bigint using scientific notation provided that the input value is within the range INT64_MIN to INT64_MAX. For example,

`bigint input_of_interest_3(1.234E8);`
  
The bigint will have the value `123400000`. This is possible because C++ will automatically convert the scientific notation input to a int64_t value that the constructor requires.  However, this type of input has limitations. If the number of significant digits is greater than the exponent, then C++ will automatically truncate it to a integer value. For example,

`bigint input_of_interest_4(1.234E2)`

This input will create a bigint with a value `123` even though the actual value of the input is `123.4`. This type of implicit conversion will generate a warning and so should be avoided. The final input of interest is,  

`bigint input_of_interest_5(-0);`  

This bigint will have the value `0`. This input will not generate any warnings. Again, this is possible because C++ will automatically disregard the unnecessary '-' and simply pass an int64_t of 0 to the constructor.
  
### The String Constructor  
  
The string constructor takes a std::string argument and creates a new bigint object with the value of the string argument. The std::string constructor implements the following algorithm.

1) Throw an exception of the argument is an empty string

2) If the first character of the argument is '-' set `isNeg` to true and remove the '-'. 

3) Remove any leading spaces and zeros. Remove any tailing spaces.

4) Handle the case of "-0". If the prepped string is "0", set `isNeg` to false. This, along with step 2, ensures the class invariant for `isNeg` is upheld.

5) Loop through the remaining characters of the value argument, convert the characters to uint8_t integers using std::stoi, and push_back to the `number` vector. 

The above steps of the algorithm are contained within a try-catch block. If the string argument violates the expected format, the constructor throws an error "Error: Invalid String". In particular, the std::stoi conversion from string to int will throw an error if the character in the loop is not an integer. Additionally, because the character being converted can only have 1 digit, any integer value being converted can only be in the range 0 to 9. Therefore, the class invariant of `number` is upheld. Because the argument is a string, the constructor has several edge cases and inputs of interest. The string can contain an integer with an arbitrary number of digits. This allows the string constructor to create bigints with values far larger than the limited range of the int64_t constructor. For example,

`bigint input_of_interest_1("99999999999999999999999999999999999999999");`

This input will create a bigint with the value, `99999999999999999999999999999999999999999`, which is many orders of magnitude larger than INT64_MAX. The constructor allows for the string to have leading and tailing spaces, and leading zeros. For example,

`bigint input_of_interest_2(" 0015678  ");`
`bigint input_of_interest_3("-00002");`
  
This input will create a bigints with the values `15678` and `2`. However, the string cannot contain any symbols, apart from a single '-' at the beginning to indicate a negative integer. For example, the following inputs will generate the error, 'Error: Invalid String'.

```
bigint input_of_interest_4("100.1234");
bigint input_of_interest_5("1234+5678");
bigint input_of_interest_6("($#@(*&%");
bigint input_of_interest_7("1234 5678");
bigint input_of_interest_8("  -001  ");
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

The following sections describe the algorithms used to override the basic mathematical operators listed in the [Introduction](#introduction).
  
### + Operator Overload  
  
**Declaration:**`bigint operator+(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the + operator for bigints. Returns the sum operand1 + operand2.  

**Param:** bigint operand1  
  
**Param:** bigint operand2  

**Return:** bigint  
  
This operator overload is a friend of the bigint class because it requires access to the operands private members. Therefore the algorithm it implements must ensure the class invariants are upheld. In order to simplify the addition algorithm, the operator overload only computes the sum of two bigints if both of those bigints are positive or both negative. If the operands have different signs, then they are handled by converting them to mathematically equivalent equations using the - operators.

* operand1 is negative and operand2 is positive. The function returns the value operand2-(-operand1). 
* operand1 is positive and operand2 is negative. The function returns the value operand1-(-operand2).

When both bigints are both positive or negative, the function uses the following algorithm to calculate the sum.

1) If the numbers have different numbers of digits, then the algorithm pads the smaller number with leading zeros. By doing this, the algorithm ensures the digits of the numbers are index aligned with digits of the same order. For example, given the numbers `123` and `56`. If the algorithm accessed the digit at `number[1]`, the digit of the first number would represent a value of 20, whereas the digit of the second number would represent a value of 6. By inserting a leading zero and changing the second bigint vector to `056`, the digit at `number[1]` now becomes 50, and so is of equivalent order. Additionally, create a string `value` that will represent the digits of the new bigint.

2) Set the carry over value to zero. Starting at the largest index, and thus smallest order digits, and looping backwards through the `number` vector, calculate the sum of the digits. `uint8_t x = num1.at(i-1) + num2.at(i-1) + carry;`

3) In the loop, if the sum is greater than or equal to 10, set x using `x = x%10;` and set carry to 1. If the sum is less than 10, simply reset carry to 0. This ensures that x has a value between 0 and 9 and so the class invariant of `number` is upheld. It also ensures that the one is carried only when necessary. 
  
4) as the last step of the loop, add the x digit to the `value` string.

5) After the loop is complete, add a '1' to the `value` string if count is 1.

6) Add a '-' to the string if the operands being added are both negative.

7) Finally, reverse the string and use the string constructor to create a new bigint with the string constructor. Return the new bigint. The string operator must be used because the bigints being added can have values far greater than the limit of the int64_t constructor. 
  
### += Operator Overload  
  
**Declaration:**`bigint operator+=(bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the += operator for bigints. Sets operand1 to the sum operand1 + operand2 and returns operand1.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint  
  
The operator takes to two operands as input. Only the second operator is constant because operand1 has its value updated. The += operator overload makes use of the + operator to avoid rewriting the algorithm from the `+` operator overload. This operator functions by setting operand1 equal to the sum of operand1 and operand2, and then returns operand1.  
  
### - Operator Overload  
  
**Declaration:** `bigint operator-(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the - binary operator for bigints. Returns the difference operand1 - operand2.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint  
  
This operator overload is a friend of the bigint class. Therefore, the algorithm it implements must ensure the class invariants are upheld. In order to simplify the subtraction algorithm, the operator overload only computes the differences of two bigints if both of those bigints are positive and operand1 >= operand2. If either operand is negative, or operand1 < operand2, then they are handled by converting them to mathematically equivalent equations using the + and - operators.

* operand1 is negative and operand2 is positive. The function returns the value -((-operand1)+operand2). 
* operand1 is positive and operand2 is negative. The function returns the value operand1+(-operand2).
* operand1 and operand2 are negative. The function returns the value ((-operand2)-(-operand1)).
* operand1 and operand2 are negative, and operand1 < operand2. The function returns the value -(operand2-operand1).

When both bigints are both positive and operand1 >= operand2, the function uses the following algorithm to calculate the sum. Additionally, because of these conditions the difference is always positive and so the resulting bigint always has a `isNeg` that is false. This ensures the `isNeg` class invariant is upheld.

1) If the numbers have different numbers of digits, then the algorithm pads the smaller number with leading zeros. By doing this, the algorithm ensures the digits of the numbers are index aligned with digits of the same order. For example, given the numbers `123` and `56`. If the algorithm accessed the digit at `number[1]`, the digit of the first number would represent a value of 20, whereas the digit of the second number would represent a value of 6. By inserting a leading zero and changing the second bigint vector to `056`, the digit at `number[1]` now becomes 50, and so is of equivalent order. Additionally, create a string `value` that will represent the digits of the new bigint.
  
2) Loop through the digits in reverse and compute the difference of each digit. 

3) if the difference is negative 'steal' 10 from the next digit by adding 10 to the difference and decrease the next highest order digit by one. Add the difference to the `value` string. Because the digits being used are between 0 and 9, and because the 10 is only added when the differences is negative, the resulting value must be in the range 0 to 9, and so the class invariant for `number` is upheld.

4) After the loop remove any leading zeros that may be left over from the difference calculations.

5) Finally, reverse the `value` string and create a new bigint using the string constructor.   

### -= Operator Overload  
  
**Declaration:**`bigint operator-=(bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the -= operator for bigints. Sets operand1 to the difference operand1-operand2 and returns operand1.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint  
  
The operator takes to two operands as input. Only the second operator is constant because operand1 has its value updated. The -= operator overload makes use of the binary - operator to avoid rewriting the algorithm from the `-` operator overload. This operator functions by setting operand1 equal to the difference of operand1 and operand2, and then returns operand1.  

### * Operator Overload  
  
**Declaration:**`bigint operator*(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the * operator for bigints. Returns the product operand1*operand2.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint  
  
This operator overload is a friend of the bigint class. Therefore, the algorithm it implements must ensure the class invariants are upheld. The operator implements the following algorithm.

1) Create a bigint 'result' using the default constructor and loop through the digits of the two operands using nested loops. 

2) Within the loop, calculate the product of the two selected digits and convert the result to a string. 

4) Add zeros to the string such that the number of zeros is equal to the sum of the order of the digits. 

5) Using the string constructor create a new temporary bigint and add it two the original `result` bigint. While the product of the digits may be greater than 9, the product is converted to a string using concatenation and so each character of the string must be between 0 and 9. This ensures the class invariant is upheld.

6) Set the `isNeg` value to true if only one operand is negative. This ensures the class invariant of `isNeg` is upheld.
  
### *= Operator Overload  
  
**Declaration:**`bigint operator*=(bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the *= operator for bigints. Sets operand1 to the product operand1*operand2 and returns operand1.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** bigint    
  
The operator takes to two operands as input. Only the second operator is constant because operand1 has its value updated. The `*=` operator overload makes use of the `*` operator to avoid rewriting the algorithm from the `*` operator overload. This operator functions by setting operand1 equal to the product of operand1 and operand2, and then returns operand1.  
  
### - Operator Overload  
  
**Declaration:**`bigint operator-(bigint &operand)`  
  
**Brief:** Overload the - unary operator for bigints. Returns the negative of operand.  
  
**Param:** bigint operand  
  
**Return:** bigint  
  
The operator takes one operand as input. The operator creates a new bigint `result` with a value equal to the operand. It then checks if the bigint is equal to a bigint with a value of zero, created using the default operator. If it is not equal, the algorithm flips the value of the `result` bigints `isNeg` variable. This ensures that the bigint cannot have a value of -0 and so preserves the class invariant for `isNeg`. Finally, it returns the new bigint `result`.
  
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
  
The a!=b condition is logically equivalent to !(a==b). Therefore, instead of rewriting the == operator algorithm, the function simply returns the equation, `!(operand1==operand2)`.
  
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
  
The condition a<=b is logically equivalent to (a<b || a==b). Therefore, instead of writing the `<` and `==` operators, the function simply returns the equation, `(operand1<operand2 || operand1==operand2);`.

### > Operator Overload  
  
**Declaration:**`bool operator>(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the > operator for bigints. Returns true if operand1 > operand2, otherwise false.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** true  
  
**Return:** false  
  
The condition a>b is logically equivalent to !(a==b or a<b). Therefore, instead of writing the `<` and `==` operators, the function simply returns the equation, `!(operand1==operand2 || operand1<operand2);`
  
### >= Operator Overload  
  
**Declaration:**`bool operator>=(const bigint &operand1, const bigint &operand2)`  
  
**Brief:** Overload the >= operator for bigints. Returns true if operand1 >= operand2, otherwise false.  
  
**Param:** bigint operand1  
  
**Param:** bigint operand2  
  
**Return:** true  
  
**Return:** false  
  
The condition a>=b is logically equivalent to !(a<b). Additionally, it is less computationally less expensive to evaluate !(a<b) rather than evaluate using the > and == operators. This is because the `>` operator itself uses the `<` and `==` operators. Therefore, this operator simply returns the equation, `!(operand1 < operand2);`
 
### << Operator Overload  
  
**Declaration:**`std::ostream &operator<<(std::ostream &out, const bigint &operand)`  
  
**Brief:** Overload the << operator for bigints. Returns a std::ostream of the operand.  
  
**Param:** std::ostream out  
  
**Param:** bigint operand  
  
**Return:** std::ostream  
  
This Operator Overload was based on the << operator overload example from section 7.1.2 of the CSE701 Lecture notes. In order to add the uint8_t values from the bigint number to the ostream it must use the unsigned() function otherwise they will simply be blank. This solution was found from the url: https://stackoverflow.com/questions/19562103/uint8-t-cant-be-printed-with-cout
  
The `<<` operator overload uses the following algorithm. If the operand is negative, the function adds a '-' to the out stream. Then the function loops through the operand number vector and adds each digit to the stream. Finally, it returns the stream, out.
  
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
  
The bigint class and associated operators are tested in four ways. Constructor tests, random operator basic tests, custom operator basic tests, and random complex operator tests. The goal is to conduct comprehensive tests on each component of the bigint implementation.  
  
### Constructor Tests 

The constructor tests are used to ensure the constructors work for a comprehensive selection of edge cases. These edge cases are based on the edge cases identified in the [Constructors](#constructors) descriptions. The constructor tests operate by taking either a int64_t or std::string input argument and create a bigint using that argument. It then outputs the expected value of the bigint and the actual value of the bigint using the `<<` operator. This demonstrates that constructors are able to handle each of the intended edge cases. Additionally, these tests serve as validation for the `<<` stream operator.
  
### Random Operator Tests  
  
Each test file in the data directory contains one test. The files are formatted so they have one number per line, and have 2 or 3 numbers per file, depending on the structure of the specific test. The are 30 random tests for each possible combination of positive or negative signs for the given operand. For example, the random addition tests takes two operands and so have 30 tests for each of the following: positive bigint + positive bigint, positive bigint + negative bigint, negative bigint + positive bigint, and negative bigint + negative bigint. The three numbers in the file will be two positive or negative random numbers and a third number that is the sum of the first two. This technique is repeated for each operator.

The final test files submitted with the project used bigints with random values between 50 digits and 100 digits. This was done to speed up the tests, particularly the multiplication tests which are quite slow for bigints with a large number of digits. However, the code was stress tested for random values up to several thousand digits. It successfully passed all of the tests. 

To execute a test, the user calls the run_operator_tests function by passing it the directory to read test files from, the test function to execute on the test files, and a string describing which operators are being tested. The run_operator_tests function reads in the numbers from each test file from the directory. The function then uses these numbers to call the provided test function and outputs the number of passed tests.
  
### Custom Operator Tests  
  
The custom operator tests function in the same way as the random operator tests, except that the values in the test files were not randomly generated. These values were selected because they represent edge cases. Some of these edge cases are discussed in the operator algorithm descriptions, the remaining were chosen because they represent specific errors that were encountered during the development process.  

### Complex Operator Tests

The complex operator tests operate the same way as the random and custom operator tests, except that the tests they execute utilize multiple operators. These tests demonstrate that the operators are consistent when used in more complex equations than the basic operator tests.
