#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <string>

class RPN {
private:
    std::stack<int> numbers;
    bool isOperator(char c) const;
    bool executeOperation(char op);
    RPN();

public:
    RPN(const RPN& src);
    RPN& operator=(const RPN& rhs);
    ~RPN(){};
    static void calculate(const std::string& expression);
};

#endif