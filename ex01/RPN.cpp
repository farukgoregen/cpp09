#include "RPN.hpp"
#include <sstream>
#include <cstdlib>

RPN::RPN() {}

RPN::RPN(const RPN& src) {
    *this = src;
}

RPN& RPN::operator=(const RPN& rhs) {
    if (this != &rhs) {
        this->numbers = rhs.numbers;
    }
    return *this;
}

bool RPN::isOperator(char c) const {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

bool RPN::executeOperation(char op) {
    if (numbers.size() < 2) return false;

    int rhs = numbers.top();
    numbers.pop();
    int lhs = numbers.top();
    numbers.pop();

    if (op == '+') numbers.push(lhs + rhs);
    else if (op == '-') numbers.push(lhs - rhs);
    else if (op == '*') numbers.push(lhs * rhs);
    else if (op == '/') {
        if (rhs == 0) {
            std::cerr << "Error: Division by zero." << std::endl;
            return false;
        }
        numbers.push(lhs / rhs);
    }
    return true;
}

void RPN::calculate(const std::string& expression) {
    RPN rpnInstance;
    std::string token;
    std::istringstream iss(expression);

    while (iss >> token) {
        if (token.length() != 1) {
            std::cerr << "Error" << std::endl;
            return;
        }

        char c = token[0];

        if (std::isdigit(c)) {
            rpnInstance.numbers.push(c - '0');
        } else if (rpnInstance.isOperator(c)) {
            if (!rpnInstance.executeOperation(c)) {
                std::cerr << "Error" << std::endl;
                return;
            }
        } else {
            std::cerr << "Error" << std::endl;
            return;
        }
    }

    if (rpnInstance.numbers.size() != 1) {
        std::cerr << "Error" << std::endl;
        return;
    }

    std::cout << rpnInstance.numbers.top() << std::endl;
}