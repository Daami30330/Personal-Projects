#include "NotationConverter.hpp"
#include <deque>
#include <sstream>

class NotationConverter : public NotationConverterInterface {
public:
    std::string postfixToInfix(std::string inStr) override {
        std::deque<std::string> operands;
        std::istringstream iss(inStr);
        std::string token;

        while (iss >> token) {
            // Check if the token is an operand (letter or number)
            if (isOperand(token)) {
                operands.push_back(token);
            } else {
                std::string operand2 = operands.back();
                operands.pop_back();
                std::string operand1 = operands.back();
                operands.pop_back();

                std::string result = "(" + operand1 + " " + token + " " + operand2 + ")";
                operands.push_back(result);
            }
        }

        return operands.back();
    }

    std::string postfixToPrefix(std::string inStr) override {
        std::deque<std::string> operands;
        std::istringstream iss(inStr);
        std::string token;

        while (iss >> token) {
            // Check if the token is an operand (letter or number)
            if (isOperand(token)) {
                operands.push_back(token);
            } else {
                std::string operand2 = operands.back();
                operands.pop_back();
                std::string operand1 = operands.back();
                operands.pop_back();

                std::string result = token + " " + operand1 + " " + operand2;
                operands.push_back(result);
            }
        }

        return operands.back();
    }

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

        return oss.str();
    }

    std::string infixToPrefix(std::string inStr) override {
        // Convert infix to postfix, then reverse the result
        std::string postfix = infixToPostfix(inStr);
        reverseString(postfix);
        return postfix;
    }

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
                std::string operand2 = operands.back();
                operands.pop_back();
                std::string operand1 = operands.back();
                operands.pop_back();

                std::string result = "(" + operand1 + " " + token + " " + operand2 + ")";
                operands.push_back(result);
            }
        }

        return operands.back();
    }

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
                std::string operand2 = operands.back();
                operands.pop_back();
                std::string operand1 = operands.back();
                operands.pop_back();

                std::string result = operand1 + " " + operand2 + " " + token;
                operands.push_back(result);
            }
        }

        return operands.back();
    }

private:
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    bool isOperand(const std::string& token) {
        // Check if the token is an operand (letter or number)
        return std::all_of(token.begin(), token.end(), [](char c) {
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
        });
    }

    int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0; // Parentheses
    }

    void reverseString(std::string& str) {
        int start = 0;
        int end = str.length() - 1;

        while (start < end) {
            std::swap(str[start], str[end]);
            ++start;
            --end;
        }
    }
};
