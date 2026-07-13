#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& src) {
    *this = src;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& rhs) {
    if (this != &rhs) {
        this->bitcoinData = rhs.bitcoinData;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

std::string BitcoinExchange::trim(const std::string& str) const {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool BitcoinExchange::isValidDate(const std::string& date) const {
    if (date.length() != 10 || date[4] != '-' || date[7] != '-') return false;

    std::string yStr = date.substr(0, 4);
    std::string mStr = date.substr(5, 2);
    std::string dStr = date.substr(8, 2);

    int year = std::atoi(yStr.c_str());
    int month = std::atoi(mStr.c_str());
    int day = std::atoi(dStr.c_str());

    if (year < 2000 || year > 2030) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    }
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (isLeap && day > 29) return false;
        if (!isLeap && day > 28) return false;
    }
    return true;
}

bool BitcoinExchange::isValidValue(const std::string& valueStr, float& value) const {
    if (valueStr.empty()) return false;
    
    char* endptr;
    value = std::strtof(valueStr.c_str(), &endptr);
    
    if (*endptr != '\0') return false;
    return true;
}

bool BitcoinExchange::loadDatabase(const std::string& dbPath) {
    std::ifstream file(dbPath.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open database file." << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos) continue;

        std::string date = trim(line.substr(0, commaPos));
        std::string rateStr = trim(line.substr(commaPos + 1));

        char* endptr;
        float rate = std::strtof(rateStr.c_str(), &endptr);
        if (*endptr == '\0') {
            bitcoinData[date] = rate;
        }
    }
    file.close();
    return true;
}

void BitcoinExchange::processInput(const std::string& inputPath) const {
    std::ifstream file(inputPath.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty() || trim(line).empty()) continue;

        size_t pipePos = line.find('|');
        if (pipePos == std::string::npos) {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string date = trim(line.substr(0, pipePos));
        std::string valStr = trim(line.substr(pipePos + 1));

        if (!isValidDate(date)) {
            std::cout << "Error: bad input => " << date << std::endl;
            continue;
        }

        float value;
        if (!isValidValue(valStr, value)) {
            std::cout << "Error: bad input => " << valStr << std::endl;
            continue;
        }

        if (value < 0) {
            std::cout << "Error: not a positive number." << std::endl;
            continue;
        }
        if (value > 1000) {
            std::cout << "Error: too large a number." << std::endl;
            continue;
        }
        std::map<std::string, float>::const_iterator it = bitcoinData.upper_bound(date);
        
        if (it != bitcoinData.begin()) {
            --it;
            float result = value * it->second;
            std::cout << date << " => " << value << " = " << result << std::endl;
        } else {
            std::cout << "Error: No exchange rate found for this date or earlier." << std::endl;
        }
    }
    file.close();
}