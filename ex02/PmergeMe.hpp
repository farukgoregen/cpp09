#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <sys/time.h>

class PmergeMe {
    public:
    PmergeMe();
    ~PmergeMe();

    // Ana tetikleyici fonksiyon
    void parseAndExecute(int argc, char** argv);

private:
    std::vector<int> _vector;
    std::deque<int>  _deque;

    PmergeMe(const PmergeMe& src);
    PmergeMe& operator=(const PmergeMe& rhs);
    bool isValidNumber(const std::string& str) const;
    void sortVector(std::vector<int>& arr);
    void sortDeque(std::deque<int>& arr);
    std::vector<size_t> generateJacobsthal(size_t size);


};
#endif