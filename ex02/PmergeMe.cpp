#include "PmergeMe.hpp"
#include <cstdlib>
#include <iomanip>

PmergeMe::PmergeMe() {}
PmergeMe::~PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& src) { *this = src; }
PmergeMe& PmergeMe::operator=(const PmergeMe& rhs) {
    if (this != &rhs) {
        this->_vector = rhs._vector;
        this->_deque = rhs._deque;
    }
    return *this;
}

bool PmergeMe::isValidNumber(const std::string& str) const {
    if (str.empty()) return false;
    for (size_t i = 0; i < str.length(); ++i) {
        if (!std::isdigit(str[i])) return false;
    }
    return true;
}

// Jacobsthal sayılarını üreten matematiksel fonksiyonumuz
std::vector<size_t> PmergeMe::generateJacobsthal(size_t size) {
    std::vector<size_t> jacob;
    if (size == 0) return jacob;
    
    jacob.push_back(1);
    if (size == 1) return jacob;
    
    jacob.push_back(3);
    size_t prev = 1, curr = 3;
    
    while (curr < size) {
        size_t next = curr + 2 * prev;
        jacob.push_back(next);
        prev = curr;
        curr = next;
    }
    return jacob;
}

// ---------------- VECTOR MOTORU (Ford-Johnson) ----------------
void PmergeMe::sortVector(std::vector<int>& arr) {
    if (arr.size() < 2) return;

    std::vector<std::pair<int, int> > pairs;
    int odd_value = -1;
    bool has_odd = (arr.size() % 2 != 0);

    if (has_odd) {
        odd_value = arr.back();
        arr.pop_back();
    }

    // 1. ADIM: Çiftleri oluştur ve kendi içinde dövüştür (Büyük solda, Küçük sağda)
    for (size_t i = 0; i < arr.size(); i += 2) {
        int first = arr[i];
        int second = arr[i+1];
        if (first < second) std::swap(first, second);
        pairs.push_back(std::make_pair(first, second));
    }

    // 2. ADIM: Recursive (Özyineleme) ile büyükleri sırala
    std::vector<int> main_chain;
    for (size_t i = 0; i < pairs.size(); ++i) {
        main_chain.push_back(pairs[i].first);
    }
    sortVector(main_chain);

    // 3. ADIM: Küçük sayıları kaybetme sırasına göre hizala
    std::vector<int> pend_chain;
    for (size_t i = 0; i < main_chain.size(); ++i) {
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (main_chain[i] == pairs[j].first) {
                pend_chain.push_back(pairs[j].second);
                break;
            }
        }
    }

    // İlk eleman hilesi: Kıyaslama yapmadan en başa at
    if (!pend_chain.empty()) {
        main_chain.insert(main_chain.begin(), pend_chain[0]);
    }

    // 4. ADIM: Kalanları Jacobsthal sırasına göre Binary Search ile ekle
    std::vector<size_t> jacob = generateJacobsthal(pend_chain.size());
    size_t inserted_count = 1;
    
    for (size_t i = 1; i < jacob.size(); ++i) {
        size_t index = jacob[i] - 1;
        if (index >= pend_chain.size()) index = pend_chain.size() - 1;

        while (index >= inserted_count) {
            std::vector<int>::iterator pos = std::upper_bound(main_chain.begin(), main_chain.end(), pend_chain[index]);
            main_chain.insert(pos, pend_chain[index]);
            if (index == inserted_count) break;
            index--;
        }
        inserted_count = jacob[i];
    }

    if (has_odd) {
        std::vector<int>::iterator pos = std::upper_bound(main_chain.begin(), main_chain.end(), odd_value);
        main_chain.insert(pos, odd_value);
    }

    arr = main_chain;
}

// ---------------- DEQUE MOTORU (Ford-Johnson) ----------------
// Algoritma mantığı birebir aynıdır, sadece std::deque tipi kullanılır
void PmergeMe::sortDeque(std::deque<int>& arr) {
    if (arr.size() < 2) return;

    std::deque<std::pair<int, int> > pairs;
    int odd_value = -1;
    bool has_odd = (arr.size() % 2 != 0);

    if (has_odd) {
        odd_value = arr.back();
        arr.pop_back();
    }

    for (size_t i = 0; i < arr.size(); i += 2) {
        int first = arr[i];
        int second = arr[i+1];
        if (first < second) std::swap(first, second);
        pairs.push_back(std::make_pair(first, second));
    }

    std::deque<int> main_chain;
    for (size_t i = 0; i < pairs.size(); ++i) {
        main_chain.push_back(pairs[i].first);
    }
    sortDeque(main_chain);

    std::deque<int> pend_chain;
    for (size_t i = 0; i < main_chain.size(); ++i) {
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (main_chain[i] == pairs[j].first) {
                pend_chain.push_back(pairs[j].second);
                break;
            }
        }
    }

    if (!pend_chain.empty()) {
        main_chain.push_front(pend_chain[0]);
    }

    std::vector<size_t> jacob = generateJacobsthal(pend_chain.size());
    size_t inserted_count = 1;
    
    for (size_t i = 1; i < jacob.size(); ++i) {
        size_t index = jacob[i] - 1;
        if (index >= pend_chain.size()) index = pend_chain.size() - 1;

        while (index >= inserted_count) {
            std::deque<int>::iterator pos = std::upper_bound(main_chain.begin(), main_chain.end(), pend_chain[index]);
            main_chain.insert(pos, pend_chain[index]);
            if (index == inserted_count) break;
            index--;
        }
        inserted_count = jacob[i];
    }

    if (has_odd) {
        std::deque<int>::iterator pos = std::upper_bound(main_chain.begin(), main_chain.end(), odd_value);
        main_chain.insert(pos, odd_value);
    }

    arr = main_chain;
}

// ---------------- ANA ÇALIŞTIRMA VE ZAMAN ÖLÇÜMÜ ----------------
void PmergeMe::parseAndExecute(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (!isValidNumber(arg)) {
            std::cerr << "Error: Invalid argument '" << arg << "'" << std::endl;
            return;
        }
        int num = std::atoi(arg.c_str());
        if (num < 0) {
            std::cerr << "Error: Only positive integers allowed." << std::endl;
            return;
        }
        _vector.push_back(num);
        _deque.push_back(num);
    }

    std::cout << "Before: ";
    for (size_t i = 0; i < _vector.size(); ++i) {
        std::cout << _vector[i] << " ";
        if (i == 5 && _vector.size() > 6) {
            std::cout << "[...]";
            break;
        }
    }
    std::cout << std::endl;

    // Vector süresini ölç ve sırala
    struct timeval start, end;
    gettimeofday(&start, NULL);
    sortVector(_vector);
    gettimeofday(&end, NULL);
    double vec_time = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

    // Deque süresini ölç ve sırala
    gettimeofday(&start, NULL);
    sortDeque(_deque);
    gettimeofday(&end, NULL);
    double deq_time = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

    std::cout << "After:  ";
    for (size_t i = 0; i < _vector.size(); ++i) {
        std::cout << _vector[i] << " ";
        if (i == 5 && _vector.size() > 6) {
            std::cout << "[...]";
            break;
        }
    }
    std::cout << std::endl;

    std::cout << "Time to process a range of " << _vector.size() 
              << " elements with std::vector : " << std::fixed << std::setprecision(3) << vec_time << " us" << std::endl;
    std::cout << "Time to process a range of " << _deque.size() 
              << " elements with std::deque  : " << std::fixed << std::setprecision(3) << deq_time << " us" << std::endl;
}