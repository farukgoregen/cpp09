#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <string>

class RPN {
private:
    // LIFO (Son giren ilk çıkar) mantığıyla çalışan sayı kutumuz
    std::stack<int> numbers;

    // İç yardımcı fonksiyonlar
    bool isOperator(char c) const;
    bool executeOperation(char op);

    // Orthodox Canonical Form (Private Default Constructor)
    RPN();

public:
    // Orthodox Canonical Form (Dışarıya açık olanlar)
    RPN(const RPN& src);
    RPN& operator=(const RPN& rhs);
    ~RPN(){};

    // RPN ifadesini hesaplayan ana fonksiyonumuz
    static void calculate(const std::string& expression);
};

#endif