// Author: Jareth Franco
// Date: October 12, 2026
// Purpose: Share line-based input validation between the menu and account factory.
#ifndef INPUT_H
#define INPUT_H

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

namespace Input {
inline bool readText(const std::string& prompt, std::string& value) {
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, value)) return false;
        const auto first = value.find_first_not_of(" \t\r\n");
        if (first != std::string::npos) {
            const auto last = value.find_last_not_of(" \t\r\n");
            value = value.substr(first, last - first + 1);
            return true;
        }
        std::cout << "Input cannot be blank.\n";
    }
}

inline bool readAmount(const std::string& prompt, double& amount, bool allowZero) {
    std::string line;
    while (readText(prompt, line)) {
        std::istringstream input(line);
        if (input >> amount) {
            input >> std::ws;
            if (input.eof() && std::isfinite(amount) &&
                (allowZero ? amount >= 0.0 : amount > 0.0)) return true;
        }
        std::cout << (allowZero ? "Enter a finite number of 0 or more.\n"
                               : "Enter a finite number greater than 0.\n");
    }
    return false;
}
}
#endif
