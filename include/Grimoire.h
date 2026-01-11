#ifndef GRIMOIRE_H_INCLUDED
#define GRIMOIRE_H_INCLUDED

#include <string>

struct WordOfPower {
    std::string name;
    std::string description;
    int power;
    int level;
    bool used;
};

struct GrimoireNode {
    WordOfPower data;
    GrimoireNode* next;

    GrimoireNode(WordOfPower val) : data(val), next(nullptr) {} // Constructor Node
};

class Grimoire {
public:
    Grimoire();
    ~Grimoire(); // Destructor

    void learnWord(const std::string& name, const std::string& description, int power);
    void openMenu(bool& canUpgrade); // Menu for Read, Update, Delete
    void upgradeWord(int index, bool& canUpgrade); // Update

    int useWordInBattle();
    void resetCooldowns();

    bool isEmpty() const;
    int getWordCount() const;
    bool hasUpgradedWords() const;

private:
    GrimoireNode* head;

    void listWords() const; // Read
    void forgetWord(int index); // Delete

    GrimoireNode* getNodeAt(int index) const;
};

#endif