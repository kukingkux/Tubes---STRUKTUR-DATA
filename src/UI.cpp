#include "UI.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <conio.h>

namespace UI {

    // Foramtting
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

    void printMenu(const std::vector<std::string>& options, bool choose) {
        for (size_t i = 0; i < options.size(); i++) {
            std::cout << textSettings.color << "[" << (i + 1) << "] " << options[i] << RESET << "\n";
        }

        if (choose) {
            std::cout << textSettings.color << "Choose: " << RESET;
        }
    }

    void printNarration(const std::string& text) {
        std::cout << "\n";
        typeText(text);
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

    void printTextSettings() {
        char input;
        while (true) {
            UI::printHeader("TEXT SETTINGS");

            // Typing Speed
            if (textSettings.skipTyping == false) {
                std::cout << "[1] Typing Speed: " << (textSettings.speedMs == 10 ? BOLD "Fast" RESET : "Fast")
                            << " | " << (textSettings.speedMs == 25 ? BOLD "Normal" RESET : "Normal")
                            << " | " << (textSettings.speedMs == 50 ? BOLD "Slow" RESET : "Slow") << "\n";
            }

            // Text Color
            std::string colorName = "White";
            std::string colorCode = WHITE;
            if (textSettings.color == CYAN) {
                colorName = "Cyan";
                colorCode = CYAN;
            } else if (textSettings.color == YELLOW) {
                colorName = "Yellow";
                colorCode = YELLOW;
            }
            std::cout << "[2] Text Color:   " << colorName << RESET << "\n";

            // Skip Typing
            std::cout << "[3] Skip Typing:  " << (textSettings.skipTyping ? BOLD "Yes" RESET : "No") << "\n";
            
            // Back To Main Menu
            std::cout << "[4] Back to Main Menu \n";

            UI::printDivider();
            std::cout << "(Press key to choose options [1/2/3/4])\n\n";
            input = _getch();
            
            if (input == '1') {
                if (textSettings.speedMs == 10) textSettings.speedMs = 25;
                else if (textSettings.speedMs == 25) textSettings.speedMs = 50;
                else textSettings.speedMs = 10;
            } else if (input == '2') {
                if (textSettings.color == WHITE) textSettings.color = CYAN;
                else if (textSettings.color == CYAN) textSettings.color = YELLOW;
                else textSettings.color = WHITE;
            } else if (input == '3') {
                textSettings.skipTyping = !textSettings.skipTyping;
            } else if (input == '4') {
                return;
            }
        }
    }



    // ASCII Art

    void printTitle() {
        std::cout << CYAN <<
        loadStoryText("assets/ascii_title.txt") <<
        RESET << "\n\n";
    }

    void printDragon() {
        std::cout << RED <<
        loadStoryText("assets/ascii_dragon.txt") <<
        RESET << "\n";
    }

    void printCampfire() {
        std::cout << YELLOW <<
        loadStoryText("assets/ascii_campfire.txt") <<
        RESET << "\n\n";
    }

    void printEnding(const std::string& type) {
        std::cout << "\n";
        
        if (type == "ORDER") {
            std::cout << CYAN <<
            loadStoryText("assets/ascii_endingorder.txt") <<
            RESET << "\n";
        } else if (type == "CHAOS") {
            std::cout << RED <<
            loadStoryText("assets/ascii_endingchaos.txt") <<
            RESET << "\n";
        } else {
            std::cout << GREEN <<
            loadStoryText("assets/ascii_endingbalance.txt") <<
            RESET << "\n";
        }
    }
};