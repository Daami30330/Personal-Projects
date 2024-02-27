#include "calculator.hpp"


double Calculator::add(double num1, double num2) {
    return num1 + num2;
}

double Calculator::subtract(double num1, double num2) {
    return num1 - num2;
}

double Calculator::multiply(double num1, double num2) {
    return num1 * num2;
}

double Calculator::divide(double num1, double num2) {
    if (num2 == 0) {
        // Handle division by zero error
        return 0;  // You can customize the error handling
    }
    return num1 / num2;
}
