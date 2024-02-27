#include "NotationConverter.hpp"
#include <deque>
#include <sstream>

class NotationConverter : public NotationConverterInterface {
public:
    // Convert postfix expression to infix expression
    std::string postfixToInfix(std::string inStr) override {
        std::deque<std::string> operands;
        std::istringstream iss(inStr);
        std::string token;

        while (iss >> token) {
            // Check if the token is a letter or number
            if (isOperand(token)) {
                operands.push_back(token);
            } else {
                // If it's a letter or number, pop two operands from deque, construct infix expression and push back
                std::string operand2 = operands.back();
                operands.pop_back();
                std::string operand1 = operands.back();
                operands.pop_back();

                std::string result = "(" + operand1 + " " + token + " " + operand2 + ")";
                operands.push_back(result);
            }
        }

        return operands.back(); // Return final infix expression
    }

    // Convert postfix expression to prefix expression
    std::string postfixToPrefix(std::string inStr) override {
        std::deque<std::string> operands;
        std::istringstream iss(inStr);
        std::string token;

        while (iss >> token) {
            // Check if the token is a letter or number
            if (isOperand(token)) { 
                operands.push_back(token);
            } else {
                // If it's an operator, pop two operands from deque, construct prefix expression and push back
                std::string operand2 = operands.back();
                operands.pop_back();
                std::string operand1 = operands.back();
                operands.pop_back();

                std::string result = token + " " + operand1 + " " + operand2;
                operands.push_back(result);
            }
        }

        return operands.back(); // Return final prefix expression
    }

    // Convert infix expression to postfix expression
    std::string infixToPostfix(std::string inStr) override {
        std::istringstream iss(inStr);
        std::ostringstream oss;
        std::deque<char> operators;
        char token;

        while (iss >> token) {
            if (isOperand(std::string(1, token))) {
                oss << token << " ";
            } else if (token == '(') {
                operators.push_back(token);
            } else if (token == ')') {
                while (!operators.empty() && operators.back() != '(') {
                    oss << operators.back() << " ";
                    operators.pop_back();
                }
                if (!operators.empty()) operators.pop_back(); // Discard the '('
            } else { // It's an operator
                while (!operators.empty() && precedence(operators.back()) >= precedence(token)) {
                    oss << operators.back() << " ";
                    operators.pop_back();
                }
                operators.push_back(token);
            }
        }

        while (!operators.empty()) {
            oss << operators.back() << " ";
            operators.pop_back();
        }

        return oss.str(); // Return final postfix expression
    }

    // Convert infix expression to prefix expression
    std::string infixToPrefix(std::string inStr) override {
        // Convert infix to postfix, then reverse the result to get prefix
        std::string postfix = infixToPostfix(inStr);
        reverseString(postfix);
        return postfix;
    }

    // Convert prefix expression to infix expression
    std::string prefixToInfix(std::string inStr) override {
        // Reverse the input string, then convert from postfix to infix
        reverseString(inStr);
        std::istringstream iss(inStr);
        std::deque<std::string> operands;
        std::string token;

        while (iss >> token) {
            // Check if the token is an operand (letter or number)
            if (isOperand(token)) {
                operands.push_back(token);
            } else {
                // If it's an operator, pop two operands from deque, construct infix expression and push back
                std::string operand2 = operands.back();
                operands.pop_back();
                std::string operand1 = operands.back();
                operands.pop_back();

                std::string result = "(" + operand1 + " " + token + " " + operand2 + ")";
                operands.push_back(result);
            }
        }

        return operands.back(); // Return final infix expression
    }

    // Convert prefix expression to postfix expression
    std::string prefixToPostfix(std::string inStr) override {
        // Reverse the input string, then convert from postfix to prefix
        reverseString(inStr);
        std::istringstream iss(inStr);
        std::deque<std::string> operands;
        std::string token;

        while (iss >> token) {
            // Check if the token is an operand (letter or number)
            if (isOperand(token)) {
                operands.push_back(token);
            } else {
                // If it's an operator, pop two operands from deque, construct postfix expression and push back
                std::string operand2 = operands.back();
                operands.pop_back();
                std::string operand1 = operands.back();
                operands.pop_back();

                std::string result = operand1 + " " + operand2 + " " + token;
                operands.push_back(result);
            }
        }

        return operands.back(); // Return final postfix expression
    }

private:
    // Check if a character is an operator
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    // Check if a token is an operand
    bool isOperand(const std::string& token) {
        // Check if the token is an operand (letter or number)
        return std::all_of(token.begin(), token.end(), [](char c) {
            return (c >= 'a' && c <= 'z') || (c >= 'A' &
