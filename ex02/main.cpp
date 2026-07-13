#include "PmergeMe.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Error: Missing arguments. Provide a sequence of positive integers." << std::endl;
        return 1;
    }

    PmergeMe sorter;
    sorter.parseAndExecute(argc, argv);

    return 0;
}