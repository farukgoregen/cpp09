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

    // Ortodoks Form (Kullanılmayanları private'a hapsediyoruz)
    PmergeMe(const PmergeMe& src);
    PmergeMe& operator=(const PmergeMe& rhs);

    // Girdi kontrolü
    bool isValidNumber(const std::string& str) const;

    // Vector için Ford-Johnson Algoritması
    void sortVector(std::vector<int>& arr);
    
    // Deque için Ford-Johnson Algoritması
    void sortDeque(std::deque<int>& arr);

    // Jacobsthal Serisi Üretici
    std::vector<size_t> generateJacobsthal(size_t size);


};
#endif