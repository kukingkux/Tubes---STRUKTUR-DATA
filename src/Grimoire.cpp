#include "Grimoire.h"
#include "TextSettings.h"
#include <iostream>
#include <limits>
using namespace std;

static void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

void Grimoire::learnWord(const std::string& name, const std::string& description, int power) {
    WordOfPower newWord = {name, description, power};
    GrimoireNode* newNode = new GrimoireNode(newWord);
    cout << "\n[NEW WORD LEARNED]: " << name << "\n";
}

void Grimoire::listWords() const {
    cout << "\n=== GRIMOIRE ===\n";
    if (head == nullptr) {
        cout << "No words learned yet.\n";
        return;
    }

    GrimoireNode* current = head;
    int index = 1;
    while (current != nullptr) {
        cout << index << ". " << current->data.name
            << "(Lvl " << current->data.level << "): "
            << current->data.description << " [Power: " << current->data.power << "]\n";
        current = current->next;
        index++;
    }
    cout << "================\n";
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
        cout << "\n[UPGRADE]: " << node->data.name << " is now level " << node->data.level << "!\n";
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
        cout << "\n[FORGOTTEN]: " << toDelete->data.name << " fades from your memory.\n";
        delete toDelete;
    }
}