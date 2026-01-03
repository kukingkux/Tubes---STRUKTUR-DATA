#include "StoryTree.h"
#include "TextSettings.h"
#include <iostream>

void showMainMenu() {
    int choice;

    while (true) {
        std::cout << CYAN
        << "\n=== SKJORHEIM ===\n"
        << "1. Start Game\n"
        << "2. Text Settings\n"
        << "3. Exit\n"
        << RESET
        << "Choose: ";

        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            choice = 0;
        }

        if (choice == 1) break;
        if (choice == 2) {
            int c;
            std::cout << "\nTyping Speed:\n"
                    << "1. Fast\n"
                    << "2. Normal\n"
                    << "3. Slow\n"
                    << "Choose: ";
            std::cin >> c;

            if (c == 1) textSettings.speedMs = 10;
            if (c == 2) textSettings.speedMs = 25;
            if (c == 3) textSettings.speedMs = 50;

            std::cout << "\nText Color:\n"
                    << "1. White\n"
                    << "2. Cyan\n"
                    << "3. Yellow\n"
                    << "Choose: ";
            std::cin >> c;

            if (c == 1) textSettings.color = WHITE;
            if (c == 2) textSettings.color = CYAN;
            if (c == 3) textSettings.color = YELLOW;

            std::cout << "\nSkip Typing?\n"
                    << "1. Yes\n"
                    << "2. No\n"
                    << "Choose: ";
            std::cin >> c;

            textSettings.skipTyping = (c == 1);
        }
        if (choice == 3) exit(0);
    }
}

int main() {
    GameState state;

    showMainMenu();

    StoryTree story(state);
    story.start();

    std::cout << "\nThanks for playing Skjorheim.\n";
    return 0;
}
