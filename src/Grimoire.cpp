#include "Grimoire.h"
#include "utils.h"
#include "ui.h"
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

static void clearInputBuffer() {
    clearInput();
}

Grimoire::Grimoire() : head(nullptr) {}

Grimoire::~Grimoire() {
    GrimoireNode* current = head;
    while (current != nullptr) {
        GrimoireNode* next = current->next;
        delete current;
        current = next;
    }
}

bool Grimoire::isEmpty() const {
    return head == nullptr;
}

int Grimoire::getWordCount() const {
    int count = 0;
    GrimoireNode* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

bool Grimoire::hasUpgradedWords() const {
    GrimoireNode* current = head;
    while (current != nullptr) {
        if (current->data.level > 1) return true;
        current = current->next;
    }
    return false;
}

void Grimoire::learnWord(const std::string& name, const std::string& description, int power) {
    WordOfPower newWord = {name, description, power};
    GrimoireNode* newNode = new GrimoireNode(newWord);
    newNode->next = head;
    head = newNode;
    UI::printSystemMessage("NEW WORD LEARNED: " + name);
}

void Grimoire::listWords() const {
    UI::printHeader("GRIMOIRE");
    if (head == nullptr) {
        cout << "No words learned yet.\n";
        return;
    }

    GrimoireNode* current = head;
    int index = 1;
    while (current != nullptr) {
        cout << index << ". " << current->data.name
            << " (Lvl " << current->data.level << "): "
            << current->data.description << " [Power: " << current->data.power << "]\n";
        current = current->next;
        index++;
    }
    UI::printDivider();
}

GrimoireNode* Grimoire::getNodeAt(int index) const {
    if (index < 0) return nullptr;

    GrimoireNode* current = head;
    int count = 0;
    while (current != nullptr) {
        if (count == index) return current;
        count++;
        current = current->next;
    }
    return nullptr;
}

void Grimoire::upgradeWord(int index) {
    GrimoireNode* node = getNodeAt(index);
    if (node) {
        node->data.level++;
        node->data.power += 5;
        UI::printSystemMessage("UPGRADE: " + node->data.name + " is now level " + std::to_string(node->data.level) + "!");
    }
}

void Grimoire::forgetWord(int index) {
    if (index < 0 || head == nullptr) return;

    GrimoireNode* toDelete = nullptr;

    if (index == 0) {
        // Delete head
        toDelete = head;
        head = head->next;
    } else {
        // Delete non-head
        GrimoireNode* prev = getNodeAt(index - 1);
        if (prev && prev->next) {
            toDelete = prev->next;
            prev->next = toDelete->next;
        }
    }

    if (toDelete) {
        UI::printSystemMessage("FORGOTTEN: " + toDelete->data.name + " fades from your memory.");
        delete toDelete;
    }
}

int Grimoire::useWordInBattle() {
    if (isEmpty()) {
        UI::printBattleMessage("You have no words to use!");
        return 0;
    }

    listWords();
    cout << "Select a word to cast (0 to cancel): ";

    int idx;
    cin >> idx;

    if (cin.fail()) {
        clearInputBuffer();
        return 0;
    }

    if (idx <= 0) return 0;

    GrimoireNode* node = getNodeAt(idx - 1);
    if (node) {
        UI::printBattleMessage("You channel the energy of " + node->data.name + "...");
        return node->data.power;
    }

    return 0;
}

void Grimoire::openMenu() {
    while(true) {
        vector<string> options = {
            "1. View Words (Read)",
            "2. Meditate/Upgrade (Update)",
            "3. Forget Word (Delete)",
            "4. Close Grimoire"
        };
        UI::printHeader("GRIMOIRE MANAGEMENT");
        UI::printMenu(options);
        cout << "Choose: ";
        
        int choice;
        cin >> choice;
        
        if (cin.fail()) {
            clearInputBuffer();
            continue;
        }

        if (choice == 1) {
            listWords();
            cout << "(Press Enter)";
            cin.ignore(); cin.get();
        } else if(choice == 2) {
            listWords();
            if (isEmpty()) continue;

            cout << "Enter number to upgrade (0 to cancel): ";
            int idx;
            cin >> idx;
            if (cin.fail()) {
                clearInputBuffer();
                continue;
            }

            if (idx > 0) {
                upgradeWord(idx-1);
            }
        } else if (choice == 3) {
            listWords();
            if (isEmpty()) continue;

            cout << "Enter number to forget (0 to cancel): ";
            int idx;
            cin >> idx;
            if (cin.fail()) {
                clearInputBuffer();
                continue;
            }

            if (idx > 0) {
                forgetWord(idx - 1);
            }
        } else if (choice == 4) {
            break;
        }
    }
}
