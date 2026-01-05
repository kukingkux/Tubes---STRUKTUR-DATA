#ifndef BATTLESYSTEM_H_INCLUDED
#define BATTLESYSTEM_H_INCLUDED
#include <string>
#include "Grimoire.h"

enum BattleResult {
    BATTLE_WIN,
    BATTLE_LOSE
};

struct Enemy {
    std::string name;
    int hp;
    int minDmg;
    int maxDmg;
    int type; // 1=Cultist, 2=Bandit/Wolf, 3=Dragon
};

// Updated signature to include Grimoire
BattleResult startBattle(int& playerHp, Enemy enemy, Grimoire& grimoire);

#endif