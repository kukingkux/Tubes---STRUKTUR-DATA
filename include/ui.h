#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <string>
#include <vector>

namespace UI {

    void printHeader(const std::string& title);
    void printNarration(const std::string& text);
    void printDialogue(const std::string& text);
    void printBattleMessage(const std::string& text);
    void printSystemMessage(const std::string& text);
    void printMenu(const std::vector<std::string>& options);
    void printDivider(const std::string& label = "");

    // ASCII Art
    void printTitleArt();
    void printDragonArt();
    void printCampfireArt();
    void printEndingArt(const std::string& type);

}

#endif // UI_H_INCLUDED
