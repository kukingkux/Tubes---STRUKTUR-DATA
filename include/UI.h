#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <string>
#include <vector>

namespace UI {
    // Formatting
    void printHeader(const std::string& title);
    void printDivider(const std::string& label = "");
    void printMenu(const std::vector<std::string>& options, bool choose = true);
    void printNarration(const std::string& text);
    void printDialogue(const std::string& speaker, const std::string& text);
    void printBattleStatus(int playerHP, int enemyHP, const std::string& enemyName);
    void printSystemMessage(const std::string& msg);
    void printBattleMessage(const std::string& msg);

    // ASCII Art
    void printTitle();
    void printDragon();
    void printCampfire();
    void printEnding(const std::string& ending);
}

#endif