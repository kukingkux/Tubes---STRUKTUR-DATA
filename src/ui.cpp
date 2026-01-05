#include "ui.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace UI {

    void printDivider(const string& label) {
        cout << "\n" << CYAN << "---";
        if (!label.empty()) {
            cout << " [ " << label << " ] ";
        } else {
            cout << "--------------------";
        }
        cout << "---" << RESET << "\n";
    }

    void printHeader(const string& title) {
        cout << "\n" << CYAN << "=== " << title << " ===" << RESET << "\n";
    }

    void printNarration(const string& text) {
        printDivider("STORY");
        typeText(text);
    }

    void printDialogue(const string& text) {
        cout << YELLOW;
        typeText(text);
        cout << RESET;
    }

    void printBattleMessage(const string& text) {
        cout << RED << "[BATTLE] " << RESET;
        typeText(text);
    }

    void printSystemMessage(const string& text) {
        cout << "\n" << GREEN << ">> " << text << RESET << "\n";
    }

    void printMenu(const vector<string>& options) {
        printDivider("MENU");
        for (const auto& opt : options) {
            cout << WHITE << opt << RESET << "\n";
        }
        cout << CYAN << "-------------------------" << RESET << "\n";
    }

    // ASCII Art
    void printTitleArt() {
        cout << CYAN << R"(
  ____  _  __     _  ___  ____  _   _  _____ ___  __  __
 / ___|| |/ /    | |/ _ \|  _ \| | | || ____|_ _| \ \/ /
 \___ \| ' /  _  | | | | | |_) | |_| ||  _|  | |   |  |
  ___) | . \ | |_| | |_| |  _ <|  _  || |___ | |   |  |
 |____/|_|\_\ \___/ \___/|_| \_\_| |_||_____|___|  |_|
              SKJORHEIM: LEGENDS OF THE NORTH
)" << RESET << "\n";
    }

    void printDragonArt() {
        cout << RED << R"(
             __        _
           _/  \    _(\(o
          /     \  /  _  ^^^o
         /   !   \/  ! '!!!v'
        !  !  \ _' ( \  _
        ! . \ _!\   \ \ _!
         \ \   ! \ \ \   !
          \ \_!  / /  \ _!
           \    / /
            \  / /
             \/ /
)" << RESET << "\n";
    }

    void printCampfireArt() {
        cout << YELLOW << R"(
         (
      .  )  )
      ) ( ( (
    .  ) ) )
   (  ( ( (
   ) ) ) ) )
 ( ( ( ) ) )
  ) ) ( ( )
 ( ) ) ) (
  ) ( ( )
  ( ) )
___\___/___
)" << RESET << "\n";
    }

    void printEndingArt(const string& type) {
        if (type == "ORDER") {
            cout << CYAN << R"(
      .       .
     / \     / \
    | | |   | | |
     \ \ \ / / /
      \ \ V / /
       \ \ / /
        \ V /
         \|/
)" << RESET << "\n";
        } else if (type == "CHAOS") {
            cout << RED << R"(
      \   /
    -  ( )  -
      /   \
     /  |  \
    |   |   |
     \  |  /
      \ | /
       \|/
)" << RESET << "\n";
        } else if (type == "BALANCE") {
            cout << GREEN << R"(
       / \
      / O \
     /_____\
)" << RESET << "\n";
        }
    }

}
