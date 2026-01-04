#include "Grimoire.h"
#include "TextSettings.h" // For colors
#include <iostream>
#include <limits>

using namespace std;

// Helper to clear input buffer (duplicated from StoryTree, maybe move to util? but keeping it simple here)
static void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void Grimoire::learnWord(const std::string& name, const std::string& description, int power) {
    words.push_back({name, description, power, 1});
    cout << "\n[NEW WORD LEARNED]: " << name << "\n";
}

void Grimoire::listWords() const {
    cout << "\n=== GRIMOIRE ===\n";
    if (words.empty()) {
        cout << "No words learned yet.\n";
        return;
    }
    for (size_t i = 0; i < words.size(); ++i) {
        cout << i + 1 << ". " << words[i].name
             << " (Lvl " << words[i].level << "): "
             << words[i].description << " [Power: " << words[i].power << "]\n";
    }
    cout << "================\n";
}

void Grimoire::upgradeWord(int index) {
    if (index >= 0 && index < (int)words.size()) {
        words[index].level++;
        words[index].power += 5;
        cout << "\n[UPGRADE]: " << words[index].name << " is now Level " << words[index].level << "!\n";
    }
}

void Grimoire::forgetWord(int index) {
    if (index >= 0 && index < (int)words.size()) {
        cout << "\n[FORGOTTEN]: " << words[index].name << " fades from memory.\n";
        words.erase(words.begin() + index);
    }
}

void Grimoire::openMenu() {
    while (true) {
        cout << CYAN "\n=== GRIMOIRE MANAGEMENT ===\n" RESET;
        cout << "1. View Words (Read)\n";
        cout << "2. Meditate/Upgrade (Update)\n";
        cout << "3. Forget Word (Delete)\n";
        cout << "4. Close Grimoire\n";
        cout << "Choose: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            clearInputBuffer();
            continue;
        }

        if (choice == 1) {
            // READ
            listWords();
            cout << "(Press Enter)";
            cin.ignore(); cin.get();
        }
        else if (choice == 2) {
            // UPDATE
            listWords();
            if (words.empty()) continue;

            cout << "Enter number to upgrade (0 to cancel): ";
            int idx;
            cin >> idx;
            if (cin.fail()) { clearInputBuffer(); continue; }

            if (idx > 0 && idx <= (int)words.size()) {
                upgradeWord(idx - 1);
            }
        }
        else if (choice == 3) {
            // DELETE
            listWords();
            if (words.empty()) continue;

            cout << "Enter number to forget (0 to cancel): ";
            int idx;
            cin >> idx;
            if (cin.fail()) { clearInputBuffer(); continue; }

            if (idx > 0 && idx <= (int)words.size()) {
                forgetWord(idx - 1);
            }
        }
        else if (choice == 4) {
            break;
        }
    }
}
