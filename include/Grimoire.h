#ifndef GRIMOIRE_H_INCLUDED
#define GRIMOIRE_H_INCLUDED

#include <string>
#include <vector>

struct WordOfPower {
    std::string name;
    std::string description;
    int power;
    int level;
};

class Grimoire {
public:
    void learnWord(const std::string& name, const std::string& description, int power);
    void openMenu(); // Handles the CLI menu for Read/Update/Delete

private:
    std::vector<WordOfPower> words;

    void listWords() const;
    void upgradeWord(int index);
    void forgetWord(int index);
};

#endif