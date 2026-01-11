#include "StoryTree.h"
#include "BattleSystem.h"
#include "Utils.h"
#include "UI.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
// #include <conio.h>

void showMainMenu() {
    char choice;

    while (true) {
        UI::printTitle();
        UI::printHeader("MAIN MENU");

        std::vector<std::string> mainMenu;
        mainMenu.push_back("Start Game");
        mainMenu.push_back("Text Settings");
        if (textSettings.devMode) mainMenu.push_back("Dev Mode (Enabled)");
        mainMenu.push_back("Exit");
        
        UI::printMenu(mainMenu, false);
        UI::printDivider();
        std::cout << "(Press key to choose options [1/2/3])\n\n";

        // choice = _getch();
        std::cin >> choice;

        if (choice == '1') break;
        if (choice == '2') {
            UI::printTextSettings();
        }
        if (choice == '3' || choice == '4')
            exit(0);
    }
}

int main(int argc, char* argv[]) {
    // Dev Mode
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-dev") == 0) {
            textSettings.devMode = true;
            textSettings.skipTyping = true;
            UI::printSystemMessage("LAUNCHED WITH DEV MODE");
        }
    }

    // Program
    GameState state;

    showMainMenu();

    StoryTree story(state);
    story.start();

    UI::printSystemMessage("Terima kasih udah main :D");
    return 0;
}