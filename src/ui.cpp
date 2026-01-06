#include "ui.h"
#include "utils.h"
#include <iostream>
#include <iomanip>

namespace UI {

void printHeader(const std::string& title) {
    printDivider();
    std::cout << textSettings.color << "  " << title << RESET << "\n";
    printDivider();
}

void printDivider(const std::string& label) {
    if (label.empty()) {
        std::cout << textSettings.color << "----------------------------------------------------" << RESET << "\n";
    } else {
        std::cout << textSettings.color << "--- [ " << label << " ] ---" << RESET << "\n";
    }
}

void printMenu(const std::vector<std::string>& options) {
    std::cout << "\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << textSettings.color << (i + 1) << ". " << options[i] << RESET << "\n";
    }
    std::cout << textSettings.color << "Choose: " << RESET;
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
    std::cout << "\n" << RED << "[BATTLE] " << RESET;
    typeText(msg);
}

// ASCII Art

void printTitle() {
    std::cout << CYAN << R"(
  ____  _    _  _  ____  ____  _  _  ____  __  __
 / ___)( )/ ( )( )/ ___)(  _ \( )( )(  __)(  \/  )
 \___ \ )  (  )()(  _ \ )   / )__(  ) _)  )    (
 (____/(_)\_)(_)  \____/(_)\_)(_)(_)(____)(_/\/\_)
    )" << RESET << "\n\n";
}

void printDragon() {
    std::cout << RED << R"(
              __                  __
             ( _)                ( _)
            / / \              / / \
           / /   \            / /   \
          / /     \          / /     \
         / /       \        / /       \
        / /         \      / /         \
       / /           \    / /           \
      / /             \  / /             \
     / /               \/ /               \
    / /                 \/                 \
   / /                                      \
  / /                                        \
 (_/                                          \_)
    )" << RESET << "\n";
}

void printCampfire() {
    std::cout << YELLOW << R"(
         (
      )  )
     (  ( (
      )  ) )
   ( (  (  )
  ( ) )  )  )
 ._(__)__. ._
 |       | |
 |_______| |
    )" << RESET << "\n";
}

void printEnding(const std::string& type) {
    std::cout << "\n";
    if (type == "ORDER") {
        std::cout << CYAN << R"(
    .---.
   /     \
  | ORDER |
   \     /
    '---'
        )" << RESET << "\n";
    } else if (type == "CHAOS") {
        std::cout << RED << R"(
    (   )
   ( CHAOS )
    (   )
        )" << RESET << "\n";
    } else {
        std::cout << GREEN << R"(
    .---.
   /BAL  \
  |  ANCE |
   \     /
    '---'
        )" << RESET << "\n";
    }
    std::cout << "\n";
}

} // namespace UI
