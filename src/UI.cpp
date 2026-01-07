#include "UI.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

namespace UI {
    void printHeader(const std::string& title) {
        printDivider();
        std::cout << textSettings.color << " " << title << RESET << "\n";
        printDivider();
    }

    void printDivider() {
        std::cout << textSettings.color << "------------------------------" << RESET << "\n";
    }

    void printMenu(const std::vector<std::string>& options) {
        std::cout << "\n";
        for (size_t i = 0; i < options.size(); i++) {
            std::cout << textSettings.color << (i + 1) << ". " << options[i] << RESET << "\n";
        }
        std::cout << textSettings.color << "Choose: " << RESET;
    }
};