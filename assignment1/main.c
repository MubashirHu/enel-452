/**
   Programmer: Mubashir Hussain
   Project: Assignment 1 Complex - Calculator
   Date: 2023-9-28

   Description: The purpose of this assignment is to make a complex calculator that is capable of computing 
    basic complex expressions in rectangular form. (a + j b). To achieve successful operation, the four sources of error
    that have been described in the assignment have been handled.  
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define ERR_ILLEGAL_COMMAND 1
#define ERR_MISSING_ARGUMENTS 2
#define ERR_EXTRA_ARGUMENTS 3
#define ERR_DIVIDE_BY_ZERO 4

// Error messages
const char *error_messages[] = {
    "",
    "error code 1: illegal command",
    "error code 2: missing arguments",
    "error code 3: extra arguments",
    "error code 4: divide by zero"
};

// Struct to define a complex number
struct Complex {
    double real;
    double imaginary;
};

// routine prototypes
void addComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result);
void subtractComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result);
void multiplyComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result);
void divideComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result);
int performComplexOperation(char operation, struct Complex z1, struct Complex z2);
void printResult(void);
void errorHandle(int errorcode);

struct Complex z1, z2, result;

int main() {

    char buffer[512];
    char operation;
    
    while (1){
        fprintf(stderr, "Complex Calculator\n");
        fprintf(stderr, "==================\n");
        fprintf(stderr, "This calculator can perform various operations on complex numbers.\n");
        fprintf(stderr, "Supported operations:\n");
        fprintf(stderr, "  - Addition (a or A)\n");
        fprintf(stderr, "  - Subtraction (s or S)\n");
        fprintf(stderr, "  - Multiplication (m or M)\n");
        fprintf(stderr, "  - Division (d or D)\n");
        fprintf(stderr, "  - Quit (q or Q)\n");
        fprintf(stderr, "Enter your operation and complex numbers in the format: <operation> <real1> <imaginary1> <real2> <imaginary2>\n");
        fprintf(stderr, "For example: 'a 3.5 2.0 1.0 4.5' for addition of (3.5 + 2.0i) and (1.0 + 4.5i).\n");
        fprintf(stderr, "You can also enter 'q' to quit the calculator.\n");
        fprintf(stderr, "Please ensure that you provide the correct number of arguments for each operation.\n");
        fprintf(stderr, "==================\n\n\n");
        fprintf(stderr, "Enter Complex Expression: ");

        while (fgets(buffer, sizeof(buffer), stdin) != NULL){ 

            int arguments_parsed = sscanf(buffer, "%c %lf %lf %lf %lf %lf", &operation, &z1.real, &z1.imaginary, &z2.real, &z2.imaginary, &z1.real);

            if(z2.real == 0 && z2.imaginary == 0){
                errno = ERR_DIVIDE_BY_ZERO;
            } else if (arguments_parsed < 5 && (operation != 'Q' && operation != 'q')){
                errno = ERR_MISSING_ARGUMENTS;
            } else if (arguments_parsed > 5){
                errno = ERR_EXTRA_ARGUMENTS;
            }

            int c = performComplexOperation(operation, z1, z2);

            if (!c && errno == 0){
                printResult();
            } else {
                errorHandle(errno);
            }

            memset(buffer, '0', 1);

            fprintf(stderr, "Enter Complex Expression: ");
        }
    }
    return 0;
}

/**
    printResult(): This function is responsible for outputting the 
    computed result that is stored in the result struct to stdout.

    parameters : None 
    Returns : None
 */
void printResult(void){
    printf("%.2lf + j %.2lf\n", result.real, result.imaginary);
}

/**
    performComplexOperation(): This function decides which operation should occur based on the command 
    that the user passes through stdin. 

    Parameters: <1> char - a character to decide which operation
                <2> struct Complex - a 'Complex' Struct that contains the real and imaginary number to be used as left operand
                <3> struct Complex - a 'Complex' Struct that contains the real and imaginary number to be used in right operand
    Return: int 0, showing function is successful
 */
int performComplexOperation(char operation, struct Complex z1, struct Complex z2) {

    if ((operation == 'a') || (operation == 'A')) {
        addComplexNumbers(z1, z2, &result);
    } else if ((operation == 's') || (operation == 'S')) {
        subtractComplexNumbers(z1, z2, &result);
    } else if ((operation == 'm') || (operation == 'M')) {
        multiplyComplexNumbers(z1, z2, &result);
    } else if ((operation == 'd') || (operation == 'D')) {
        divideComplexNumbers(z1, z2, &result);
    } else if ((operation == 'q') || (operation == 'Q'))  {
        fprintf(stderr, "\n\nClosing Complex - Calculator...\n\n");
        exit(0);
    } else {
        errno = ERR_ILLEGAL_COMMAND;
    }
    return 0;
}

/**
    errorHandle(): This function prints the error message to stdout based on the errorcode that is passed in. 
    The errorcode that is passed in will always be the global variable 'errno' unless otherwise stated. After 
    The error message has been sent, the errno global variable is hard reset to 0.

    Parameters : <1> int errorcode - the code from errno
    Return: None
 */
void errorHandle(int errorcode){
    switch (errorcode){
        case ERR_ILLEGAL_COMMAND:
            printf("%s\n", error_messages[ERR_ILLEGAL_COMMAND]);
            errno = 0;
            break;
        case ERR_MISSING_ARGUMENTS:
            printf("%s\n", error_messages[ERR_MISSING_ARGUMENTS]);
            errno = 0;
            break;
        case ERR_EXTRA_ARGUMENTS: 
            printf("%s\n", error_messages[ERR_EXTRA_ARGUMENTS]);
            errno = 0;
            break;
        case ERR_DIVIDE_BY_ZERO: 
            printf("%s\n", error_messages[ERR_DIVIDE_BY_ZERO]);
            errno = 0;
            break;
        default:
            errno = 0;
            break;
    }
}

/**
    addComplexNumbers(): This function adds the real and imaginary components of the two complex numbers represented by 
    the complex Struct. 

    Parameters:
    <1> z1 - A 'Complex' Struct that contains the real and imaginary numbers for the left operand
    <2> z2 - A 'Complex' Struct that contains the real and imaginary numbers for the right operand
    <3> result - Pointer to a 'Complex' Struct where the final result is stored
    Return: None
 */
void addComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result){
    result->real = z1.real + z2.real;
    result->imaginary = z1.imaginary + z2.imaginary;
}

/**
    subtractComplexNumbers(): This function subtracts the real and imaginary components of two complex numbers represented by 
    the 'Complex' Structs.

    Parameters:
    <1> z1 - A 'Complex' Struct that contains the real and imaginary numbers for the left operand
    <2> z2 - A 'Complex' Struct that contains the real and imaginary numbers for the right operand
    <3> result - Pointer to a 'Complex' Struct where the final result is stored
    Return: None
*/
void subtractComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result){
    result->real = z1.real - z2.real;
    result->imaginary = z1.imaginary - z2.imaginary;
}

/**
    multiplyComplexNumbers(): This function multiplies two complex numbers represented by 
    the 'Complex' Structs.

    Parameters:
    <1> z1 - A 'Complex' Struct that contains the real and imaginary numbers for the left operand
    <2> z2 - A 'Complex' Struct that contains the real and imaginary numbers for the right operand
    <3> result - Pointer to a 'Complex' Struct where the final result is stored 
    Return: None
*/
void multiplyComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result){
    result->real = (z1.real * z2.real) - (z1.imaginary * z2.imaginary);
    result->imaginary = (z1.real * z2.imaginary) + (z1.imaginary * z2.real);
}

/**
    divideComplexNumbers(): This function divides two complex numbers represented by 
    the 'Complex' Structs.

    Parameters:
    <1> z1 - A 'Complex' Struct that contains the real and imaginary numbers for the numerator
    <2> z2 - A 'Complex' Struct that contains the real and imaginary numbers for the denominator
    <3> result - Pointer to a 'Complex' Struct where the final result is stored
    Return: None
*/
void divideComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result){
    double denominator = (z2.real * z2.real) + (z2.imaginary * z2.imaginary);

    result->real = ((z1.real * z2.real) + (z1.imaginary * z2.imaginary)) / denominator;
    result->imaginary = ((z1.imaginary * z2.real) - (z1.real * z2.imaginary)) / denominator;
}