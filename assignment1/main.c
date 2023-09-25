#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

struct Complex {
    double real;
    double imaginary;
};

// routine prototypes
void addComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result);
void subtractComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result);
void multiplyComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result);
void divideComplexNumbers(struct Complex z1, struct Complex z2, struct Complex* result);
void performComplexOperation(char operation, struct Complex z1, struct Complex z2);

struct Complex z1, z2, result;

int main() {
    
    return 0;
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

void performComplexOperation(char operation, struct Complex z1, struct Complex z2) {

    if ((operation == 'a') || (operation == 'A')) {
        addComplexNumbers(z1, z2, &result);
    } else if ((operation == 's') || (operation == 'S')) {
        subtractComplexNumbers(z1, z2, &result);
    } else if ((operation == 'm') || (operation == 'M')) {
        multiplyComplexNumbers(z1, z2, &result);
    } else if ((operation == 'd') || (operation == 'D')) {
        divideComplexNumbers(z1, z2, &result);
    } else {
        printf("Invalid operation code\n");
        return; // return early
    }
}

//note 
// 1. read from stdin
// 2. write to stdout, program outputs
// 3. errors to be shown on stderr, lesser outputs
