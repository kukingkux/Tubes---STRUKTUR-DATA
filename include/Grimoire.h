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
    void openMenu(); // Menu for Read, Update, Delete

    int useWordInBattle();

    bool isEmpty() const;

private:
    GrimoireNode* head;

    void listWords() const; // Read
    void upgradeWord(int index); // Update
    void forgetWord(int index); // Delete

    GrimoireNode* getNodeAt(int index) const;
};

#endif