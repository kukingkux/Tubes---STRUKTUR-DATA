#include "StoryTree.h"
#include "BattleSystem.h"
#include "utils.h"
#include "ui.h"
#include <iostream>
#include <vector>
#include <string>

void showMainMenu() {
    int choice;

    while (true) {
        UI::printTitleArt();
        std::vector<std::string> options = {
            "1. Start Game",
            "2. Text Settings",
            "3. Exit"
        };
        UI::printMenu(options);
        std::cout << "Choose: ";

        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            choice = 0;
        }

        if (choice == 1) break;
        if (choice == 2) {
            int c;
            std::vector<std::string> speedOpts = {"1. Fast", "2. Normal", "3. Slow"};
            UI::printMenu(speedOpts);
            std::cout << "Choose Speed: ";
            std::cin >> c;

            if (c == 1) textSettings.speedMs = 10;
            if (c == 2) textSettings.speedMs = 25;
            if (c == 3) textSettings.speedMs = 50;

            std::vector<std::string> colorOpts = {"1. White", "2. Cyan", "3. Yellow"};
            UI::printMenu(colorOpts);
            std::cout << "Choose Color: ";
            std::cin >> c;

            if (c == 1) textSettings.color = WHITE;
            if (c == 2) textSettings.color = CYAN;
            if (c == 3) textSettings.color = YELLOW;

            std::vector<std::string> skipOpts = {"1. Yes", "2. No"};
            UI::printMenu(skipOpts);
            std::cout << "Skip Typing? ";
            std::cin >> c;

            textSettings.skipTyping = (c == 1);
        }
        if (choice == 3)
            exit(0);
    }
}

int main() {
    GameState state;

    showMainMenu();

    StoryTree story(state);
    story.start();

    std::cout << "\nTerima kasih udah main :D\n";
    return 0;
}