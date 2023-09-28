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

/*
fgets() function to read the input as a string in one fell swoop, 
sscanf() to parse it
fprintf() to print the output. 
*/

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

            fprintf(stderr, "Enter Complex Expression: ");
        }
    }
    return 0;
}

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

void printResult(void){
    printf("%.2lf + j %.2lf\n", result.real, result.imaginary);
}

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
            //works fine
            errno = 0;
            break;
    }
}

// Function to add two complex numbers
void addComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result){
    result->real = z1.real + z2.real;
    result->imaginary = z1.imaginary + z2.imaginary;
}

// Function to subtract two complex numbers
void subtractComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result){
    result->real = z1.real - z2.real;
    result->imaginary = z1.imaginary - z2.imaginary;
}

// Function to multiply two complex numbers
void multiplyComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result){
    result->real = (z1.real * z2.real) - (z1.imaginary * z2.imaginary);
    result->imaginary = (z1.real * z2.imaginary) + (z1.imaginary * z2.real);
}

// Function to divide two complex numbers
void divideComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result){
    double denominator = (z2.real * z2.real) + (z2.imaginary * z2.imaginary);

    result->real = ((z1.real * z2.real) + (z1.imaginary * z2.imaginary)) / denominator;
    result->imaginary = ((z1.imaginary * z2.real) - (z1.real * z2.imaginary)) / denominator;
}

// note 
// 1. read from stdin
// 2. write to stdout, program outputs
// 3. errors to be shown on stderr, lesser outputs
