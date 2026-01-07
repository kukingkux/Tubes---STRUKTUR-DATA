#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <string>
#include <vector>

namespace UI {
    void printHeader(const std::string& title);
    void printDivider();
    void printMenu(const std::vector<std::string>& options);
    void printNarration(const std::string& text);
    void printDialogue(const std::string& speaker, const std::string& text);
    void printBattleStatus(int playerHP, int enemyHP, const std::string& enemyName);
    void printSystemMessage(const std::string msg);
}

#endif