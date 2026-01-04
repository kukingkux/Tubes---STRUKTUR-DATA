#ifndef GRIMOIRE_H_INCLUDED
#define GRIMOIRE_H_INCLUDED

#include <string>

struct WordOfPower {
    std::string name;
    std::string description;
    int power;
    int level;
};

// ADT: Single Linked List Node
struct GrimoireNode {
    WordOfPower data;
    GrimoireNode* next;

    GrimoireNode(WordOfPower val) : data(val), next(nullptr) {}
};

class Grimoire {
public:
    Grimoire();
    ~Grimoire(); // Destructor to clean up nodes

    void learnWord(const std::string& name, const std::string& description, int power);
    void openMenu();

    // Battle Integration
    int useWordInBattle();

    // Public for inspection if needed, but mainly used internally
    bool isEmpty() const;

private:
    GrimoireNode* head; // Head of the linked list

    void listWords() const;
    void upgradeWord(int index); // Index logic adapted for Linked List traversal
    void forgetWord(int index);

    // Helper to get node at index (0-based)
    GrimoireNode* getNodeAt(int index) const;
};

#endif