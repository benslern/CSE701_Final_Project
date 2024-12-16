# CSE 701 - Final Project  
### Noah Bensler - 400611927  
The documentation should explain what your code does and how it works, including any relevant equations, algorithms, and concepts. The documentation must include several examples of possible inputs of interest and the outputs they generate. Only Markdown documentation will be accepted; LaTeX, Word, or other formats are not allowed.  
 
This is the documentation for the Fall 2024 CSE701 final project. It is a C++ implementation of arbitrary-precision integers, bigint. It allow for the creation of arbitrarily large integers, allowing with operator overloading for the following operators,  
- Addition (`+` and `+=`)  
- Subtraction (`-` and `-=`)  
- Multiplication (`*` and `*=`)  
- Negation (unary `-`)  
- Comparison (`==`, `!=`, `<`, `>`, `<=`, and `>=`)  
- Insertion (`<<`, to print the integer to a stream such as `std::cout` or a file)  
- Increment (`++`, both pre-increment and post-increment)  
- Decrement (`--`, both pre-decrement and post-decrement)  
 
## Implementation  
The bigint class contains two private members, `number` and `isNeg`. The `number` variable is a vector of unsigned 8 bit integers,  
`std::vector<uint8_t> number;`  
These integers are used to represent the digits of the bigint. The value at `number[0]` is the largest digit in the bigint, and the value at `number[number.size()-1]` is the smallest.

The `isNeg` variable is a boolean that is true if and only if the bigint is negative,
`bool isNeg;`

The following sections describe the algorithms used to override the various operators.

### + Operator Overload
**Declaration:**`bigint operator+(const bigint &operand1, const bigint &operand2)
`
**Brief:**
**Param:** bigint operand1

**Param:** bigint operand2

**Return:** bigint

### += Operator Overload

**Declaration:**`bigint operator+=(bigint &operand1, const bigint &operand2)`
**Brief:**
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

### -= Operator Overload
**Declaration:**`bigint operator-=(bigint &operand1, const bigint &operand2)`
**Brief:**
**Param:** bigint operand1
**Param:** bigint operand2
**Return:** bigint

FULL DESCRIPTION OF ALGORITHM

### * Operator Overload
**Declaration:**`bigint operator*(const bigint &operand1, const bigint &operand2)`
**Brief:**
**Param:** bigint operand1
**Param:** bigint operand2
**Return:** bigint

FULL DESCRIPTION OF ALGORITHM

### *= Operator Overload
**Declaration:**`bigint operator*=(bigint &operand1, const bigint &operand2)`
**Brief:**
**Param:** bigint operand1
**Param:** bigint operand2
**Return:** bigint

FULL DESCRIPTION OF ALGORITHM

### - Operator Overload
**Declaration:**`bigint operator-(bigint &operand)`
**Brief:**
**Param:** bigint operand
**Return:** bigint

FULL DESCRIPTION OF ALGORITHM

### == Operator Overload
**Declaration:**`bool operator==(const bigint &operand1, const bigint &operand2)`
**Brief:**
**Param:** bigint operand1
**Param:** bigint operand2
**Return:** true
**Return:** false

FULL DESCRIPTION OF ALGORITHM

### != Operator Overload
**Declaration:**`bool operator!=(const bigint &operand1, const bigint &operand2)`
**Brief:**
**Param:** bigint operand1
**Param:** bigint operand2
**Return:** true
**Return:** false

FULL DESCRIPTION OF ALGORITHM

### < Operator Overload
**Declaration:**`bool operator<(const bigint &operand1, const bigint &operand2)`
**Brief:**
**Param:** bigint operand1
**Param:** bigint operand2
**Return:** true
**Return:** false

FULL DESCRIPTION OF ALGORITHM

### <= Operator Overload
**Declaration:**`bool operator<=(const bigint &operand1, const bigint &operand2)`
**Brief:**
**Param:** bigint operand1
**Param:** bigint operand2
**Return:** true
**Return:** false

FULL DESCRIPTION OF ALGORITHM

### > Operator Overload
**Declaration:**`bool operator>(const bigint &operand1, const bigint &operand2)`
**Brief:**
**Param:** bigint operand1
**Param:** bigint operand2
**Return:** true
**Return:** false

FULL DESCRIPTION OF ALGORITHM

### >= Operator Overload
**Declaration:**`bool operator>=(const bigint &operand1, const bigint &operand2)`
**Brief:**
**Param:** bigint operand1
**Param:** bigint operand2
**Return:** true
**Return:** false

FULL DESCRIPTION OF ALGORITHM

### << Operator Overload
**Declaration:**`std::ostream &operator<<(std::ostream &out, const bigint &operand)`
**Brief:**
**Param:** std::ostream out
**Param:** bigint operand
**Return:** std::ostream

FULL DESCRIPTION OF ALGORITHM

### ++ Pre Operator Overload
**Declaration:**`bigint operator++(bigint &operand1)`
**Brief:**
**Param:** bigint operand1
**Param:** bigint operand2
**Return:** bigint

FULL DESCRIPTION OF ALGORITHM

### ++ Post Operator Overload
**Declaration:**`bigint operator++(bigint &operand1, int)`
**Brief:**
**Param:** bigint operand1
**Param:** bigint operand2
**Return:** bigint

FULL DESCRIPTION OF ALGORITHM

### -- Pre Operator Overload
**Declaration:**`bigint operator--(bigint &operand1)`
**Brief:**
**Param:** bigint operand1
**Param:** bigint operand2
**Return:** bigint

FULL DESCRIPTION OF ALGORITHM

### -- Post Operator Overload
**Declaration:**`bigint operator--(bigint &operand1,int)`
**Brief:**
**Param:** bigint operand1
**Param:** bigint operand2
**Return:** bigint

FULL DESCRIPTION OF ALGORITHM