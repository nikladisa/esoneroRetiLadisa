/*
 * math_functions.h
 *
 *  Created on: 17 nov 2023
 *      Author: ladis
 */

#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

// Function for additional
    int add(int a, int b) {
    return a + b;
}

// Function for multiplication
    int mult(int a, int b) {
    return a * b;
}

// Function for sub
int sub(int a, int b) {
    return a - b;
}

// Function for division
    double division(int a, int b) {
    if (b != 0) {
        return a / b;
    } else
    {
        return 0.0;
    }
}

#endif // MATH_FUNCTIONS_H
