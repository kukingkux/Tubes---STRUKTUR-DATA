#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED
#include <string>
#include <vector>
#include <iostream>

struct WordOfPower {
    std::string name;
    std::string description;
    int power;
    int level;
};

struct GameState {
    // Player
    int health = 100;

    // Moral axes
    int order = 0;
    int knowledge = 0;
    int chaos = 0;

    // World Flags
    bool dragonAwakened = false;
    bool joinedOrder = false;
    bool joinedScholars = false;
    bool helpedRebels = false;

    // CRUD: Words of Power Collection
    std::vector<WordOfPower> grimoire;

    // Helper functions for CRUD
    void addWord(const std::string& n, const std::string& d, int p) {
        grimoire.push_back({n, d, p, 1});
        std::cout << "\n[NEW WORD LEARNED]: " << n << "\n";
    }

    void listWords() const {
        std::cout << "\n=== GRIMOIRE ===\n";
        if (grimoire.empty()) {
            std::cout << "No words learned yet.\n";
            return;
        }
        for (size_t i = 0; i < grimoire.size(); ++i) {
            std::cout << i + 1 << ". " << grimoire[i].name
                      << " (Lvl " << grimoire[i].level << "): "
                      << grimoire[i].description << " [Power: " << grimoire[i].power << "]\n";
        }
        std::cout << "================\n";
    }

    void upgradeWord(int index) {
        if (index >= 0 && index < grimoire.size()) {
            grimoire[index].level++;
            grimoire[index].power += 5;
            std::cout << "\n[UPGRADE]: " << grimoire[index].name << " is now Level " << grimoire[index].level << "!\n";
        }
    }

    void deleteWord(int index) {
        if (index >= 0 && index < grimoire.size()) {
            std::cout << "\n[FORGOTTEN]: " << grimoire[index].name << " fades from memory.\n";
            grimoire.erase(grimoire.begin() + index);
        }
    }
};

#endif