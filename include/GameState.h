#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED
#include <string>
#include "Grimoire.h"

struct GameState {
    // Player
    int health = 100;

    // Moral axes
    int orderPoints = 0;
    int chaosPoints = 0;
    int order = 0; // Deprecated/Legacy
    int knowledge = 0; // Deprecated/Legacy
    int chaos = 0; // Deprecated/Legacy

    // World Flags
    bool dragonAwakened = false;
    bool joinedOrder = false;
    bool joinedScholars = false;
    bool helpedRebels = false;
    bool canUpgradeWord = false;

    // CRUD: Words of Power
    Grimoire grimoire;
};

#endif