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

    void printDivider(const std::string& label) {
        if (label.empty()) {
            std::cout << textSettings.color << "-----------------------------------------" << RESET << "\n";
        } else {
            std::cout << textSettings.color << "--- [ " << label << " ] ---" << RESET << "\n";
        }
    }

    void printMenu(const std::vector<std::string>& options) {
        std::cout << "\n";
        for (size_t i = 0; i < options.size(); i++) {
            std::cout << textSettings.color << (i + 1) << ". " << options[i] << RESET << "\n";
        }
        std::cout << textSettings.color << "Choose: " << RESET;
    }

    void printDialogue(const std::string& speaker, const std::string& text) {
        std::cout << "\n" << CYAN << "[" << speaker << "]" << RESET << "\n";
        typeText(textSettings.color + "\"" + text + "\"" + RESET);
    }

    void printBattleStatus(int playerHP, int enemyHP, const std::string& enemyName) {
        printDivider("BATTLE STATUS");
        std::cout << GREEN << "PLAYER HP: " << playerHP << RESET
                << "  vs  "
                << RED << enemyName << " HP: " << enemyHP << RESET << "\n";
        printDivider();
    }

    void printSystemMessage(const std::string& msg) {
        std::cout << "\n" << YELLOW << "*** " << msg << " ***" << RESET << "\n";
    }

    void printBattleMessage(const std::string& msg) {
        std::cout << "\n" << RED << "[BATTLE]"<< RESET;
        typeText(msg);
    }
};