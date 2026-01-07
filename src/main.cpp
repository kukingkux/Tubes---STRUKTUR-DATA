#include "StoryTree.h"
#include "BattleSystem.h"
#include "Utils.h"
#include "UI.h"
#include <iostream>
#include <vector>

void showMainMenu() {
    int choice;

    while (true) {
        UI::printTitle();

        std::vector<std::string> mainMenu;
        mainMenu.push_back("Start Game");
        mainMenu.push_back("Text Settings");
        mainMenu.push_back("Exit");
        UI::printMenu(mainMenu);

        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            choice = 0;
        }

        if (choice == 1) break;
        if (choice == 2) {
            int c;
            
            std::vector<std::string> speedOptions;
            speedOptions.push_back("Fast");
            speedOptions.push_back("Normal");
            speedOptions.push_back("Slow");

            std::cout << "\nTyping Speed:";
            UI::printMenu(speedOptions);
            std::cin >> c;

            if (c == 1) textSettings.speedMs = 10;
            if (c == 2) textSettings.speedMs = 25;
            if (c == 3) textSettings.speedMs = 50;

            std::vector<std::string> colorOptions;
            speedOptions.push_back("White");
            speedOptions.push_back("Cyan");
            speedOptions.push_back("Yellow");

            std::cout << "\nText Color:";
            UI::printMenu(colorOptions);
            std::cin >> c;

            if (c == 1) textSettings.color = WHITE;
            if (c == 2) textSettings.color = CYAN;
            if (c == 3) textSettings.color = YELLOW;

            std::vector<std::string> skipOptions;
            speedOptions.push_back("Yes");
            speedOptions.push_back("No");

            std::cout << "\nTyping Speed:";
            UI::printMenu(skipOptions);
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

    UI::printSystemMessage("Terima kasih udah main :D");
    return 0;
}