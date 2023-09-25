#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

// commands for complex arithematic, A,S,M,D,Q

struct Complex {
    double real;
    double imaginary;
};

// routine prototypes
void _add_complex(struct Complex z1, struct Complex z2, struct Complex* result);
void _subtract_complex(struct Complex z1, struct Complex z2, struct Complex* result);
void _mulitply_complex(struct Complex z1, struct Complex z2, struct Complex* result);
void _divide_complex(struct Complex z1, struct Complex z2, struct Complex* result);

int main() {
    
    // while 'q' or "Q" has not been entered, keep reading

    return 0;
}

// Function to add two complex numbers
void _add_complex(struct Complex z1, struct Complex z2, struct Complex* result)
{
    result->real = z1.real + z2.real;
    result->imaginary = z1.imaginary + z2.imaginary;
}

// Function to subtract two complex numbers
void _subtract_complex(struct Complex z1, struct Complex z2, struct Complex* result)
{
    result->real = z1.real - z2.real;
    result->imaginary = z1.imaginary - z2.imaginary;
}

// Function to multiply two complex numbers
void _multiply_complex(struct Complex z1, struct Complex z2, struct Complex* result)
{
    result->real = (z1.real * z2.real) - (z1.imaginary * z2.imaginary);
    result->imaginary = (z1.real * z2.imaginary) + (z1.imaginary * z2.real);
}

// Function to divide two complex numbers
void _divide_complex(struct Complex z1, struct Complex z2, struct Complex* result)
{
    double denominator = (z2.real * z2.real) + (z2.imaginary * z2.imaginary);

    result->real = ((z1.real * z2.real) + (z1.imaginary * z2.imaginary)) / denominator;
    result->imaginary = ((z1.imaginary * z2.real) - (z1.real * z2.imaginary)) / denominator;
}

//note 
// 1. read from stdin
// 2. write to stdout, program outputs
// 3. errors to be shown on stderr, lesser outputs
